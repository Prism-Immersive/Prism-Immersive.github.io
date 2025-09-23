#include <cstdint>
#define LOG_TAG "ThreadContext"
#define LOG_LEVEL LOG_LEVEL_ERROR
#include "ThreadContext.h"

#include <dlfcn.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <unwind.h>

#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "Log.h"
#include "Utils.h"
#include "data/ProgramCache.h"
#include "opengl/GLDef.h"
#include "opengl/Program.h"

thread_local void *egl_thread_context = nullptr;
thread_local std::shared_ptr<ThreadContext> thread_context_ptr = nullptr;
EglDisplay *egl_default_display = nullptr;
MainServer *main_server = nullptr;
StaticContextValues *process_static_context_values = nullptr;
static pthread_mutex_t context_value_lock = PTHREAD_MUTEX_INITIALIZER;
EglFence *egl_fence = nullptr;
std::unique_ptr<ProgramCache> main_program_cache = nullptr;
std::unique_ptr<Loader> main_loader = nullptr;

pid_t my_pid = 0;
pid_t composer_pid = 0;

std::unordered_map<EGLImage, EGLImageInfo *> all_image_info;
std::unordered_map<uint64_t, int> global_gbuffer_types;
std::unordered_map<uint64_t, uint64_t> global_gbuffer_died_times;

// EGLImage may be 32 bit

#define SIG_NUM 5
struct sigaction crash_old_handlers[SIG_NUM];
int sigN[SIG_NUM] = {SIGSEGV, SIGBUS, SIGILL, SIGQUIT, SIGABRT};
const char *sig_name[SIG_NUM] = {"SIGSEGV", "SIGBUS", "SIGILL", "SIGQUIT",
                                 "SIGABRT"};

int crash_init = 0;

static void lib_constructor() __attribute__((__constructor__));

static void lib_constructor() {
    main_loader = std::make_unique<Loader>();
    if (!main_loader->is_cloud_rendering())
        return;
    egl_default_display = new EglDisplay();
    main_server = new MainServer();
    egl_fence = new EglFence();
    main_program_cache = std::make_unique<ProgramCache>();
}

static void lib_destructor() __attribute__((__destructor__));

static void lib_destructor() {
    if (egl_default_display)
        delete egl_default_display;
    egl_default_display = nullptr;
    if (main_server)
        delete main_server;
    main_server = nullptr;
    if (egl_fence)
        delete egl_fence;
    egl_fence = nullptr;
}

void register_client_status_callback(ClientStatusCallback callback) {
    if (main_server) {
        main_server->register_client_status_callback(callback);
    }
}

void callstack_dump(std::string &dump);
// uint64_t write_network(int fd, const void* const __pass_object_size0 buf,
// uint64_t count, int need_flush);

static _Unwind_Reason_Code unwind_callback(struct _Unwind_Context *context,
                                           void *arg) {
    std::vector<_Unwind_Word> &stack = *(std::vector<_Unwind_Word> *)arg;
    stack.push_back(_Unwind_GetIP(context));
    return _URC_NO_REASON;
}

void callstack_dump(std::string &dump) {
    std::vector<_Unwind_Word> stack;
    _Unwind_Backtrace(unwind_callback, (void *)&stack);

    char buff[1024];
    for (int i = 0; i < stack.size(); i++) {
        Dl_info info;
        if (!dladdr((void *)stack[i], &info)) {
            continue;
        }
        long addr = (char *)stack[i] - (char *)info.dli_fbase - 1;
        if (info.dli_sname == nullptr || strlen(info.dli_sname) == 0) {
            sprintf(buff, "#%02x pc %08lx  %s\n", i, addr, info.dli_fname);
        } else {
            sprintf(buff, "#%02x pc %08lx  %s (%s+00)\n", i, addr,
                    info.dli_fname, info.dli_sname);
        }
        dump.append(buff);
    }
}

void crash_capture(int sig, siginfo_t *siginfo, void *context) {
    int sig_t = 0;
    for (int i = 0; i < SIG_NUM; i++) {
        if (sig == sigN[i]) {
            sig_t = i;
            break;
        }
    }
    char temp[50];
    temp[49] = 0;

    std::string dump = egl_default_display->get_proc_name() + " crash(" +
                       sig_name[sig_t] + ") stack:\n";
    callstack_dump(dump);

    //    log_host("crash", dump.c_str());

    crash_old_handlers[sig_t].sa_sigaction(sig, siginfo, context);

    exit(1);
}

GLThreadContext::GLThreadContext(GLThreadContext *share_context) {
    if (share_context == nullptr) {
        share_opengl_context = nullptr;
        context_share_cnt = new int[1];
        context_share_thread_ids = new std::unordered_map<int, int>();
        context_share_cnt[0] = 1;
    } else {
        share_opengl_context = share_context;
        share_context->share_opengl_context = this;
        context_share_cnt = share_context->context_share_cnt;
        context_share_thread_ids = share_context->context_share_thread_ids;
        share_context->context_share_cnt[0]++;
    }
    config_id = 0;

    // initialize pixel_store_status
    PixelStoreStatus *pixel_store = &pixel_store_status;
    memset(pixel_store, 0, sizeof(PixelStoreStatus));
    pixel_store->unpack_alignment = 4;
    pixel_store->pack_alignment = 4;

    if (share_context == nullptr) {
        create_resource_context(&resource_context, nullptr);
    } else {
        create_resource_context(&resource_context,
                                &(share_context->resource_context));
    }

    BindingStatus *binding_status = &context_binding_status;
    memset(binding_status, 0, sizeof(BindingStatus));
    binding_status->bound_buffer = new BoundBuffer;
    binding_status->active_texture = 0;
    binding_status->drawbuffers[0] = GL_BACK;
    for (int i = 1; i < 16; i++) {
        binding_status->drawbuffers[i] = GL_NONE;
    }
    binding_status->fragment_shader_derivative_hint = GL_DONT_CARE;
    binding_status->generate_mipmap_hint = GL_DONT_CARE;
    binding_status->read_buffer = GL_BACK;

    binding_status->transform_feedback_active = GL_FALSE;
    binding_status->transform_feedback_paused = GL_FALSE;

    EnableStatus *enable_status_local = &enable_status;
    memset(enable_status_local, 0, sizeof(EnableStatus));

    frame_buffer_map = std::make_unique<
        std::unordered_map<GLuint, std::unique_ptr<FrameBufferInfo>>>();
    if (share_context == nullptr) {
        program_info_map = new std::unordered_map<GLuint, GLSLProgram *>();
        texture_info_map = new std::unordered_map<GLuint, TextureInfo *>();
        texture_gbuffer_map = new std::unordered_map<GLuint, EGLImageInfo *>();
        render_buffer_map = std::make_shared<
            std::unordered_map<GLuint, std::unique_ptr<RenderBufferInfo>>>();
        shader_map = std::make_shared<
            std::unordered_map<GLuint, std::unique_ptr<GLSLShader>>>();
        buffer_ptr = new BufferMapped;
#ifdef USE_LOCAL_COMPILER
        glsl_compiler = std::make_unique<GLSLCompiler>(nullptr);
#endif
    } else {
        program_info_map = share_context->program_info_map;
        texture_info_map = share_context->texture_info_map;
        texture_gbuffer_map = share_context->texture_gbuffer_map;
        render_buffer_map = share_context->render_buffer_map;
        shader_map = share_context->shader_map;
        buffer_ptr = share_context->buffer_ptr;
#ifdef USE_LOCAL_COMPILER
        glsl_compiler =
            std::make_unique<GLSLCompiler>(share_context->glsl_compiler.get());
#endif
    }

    pthread_mutex_lock(&context_value_lock);
    if (process_static_context_values == nullptr) {
        auto *static_values_local = (StaticContextValues *)malloc(
            sizeof(StaticContextValues) + 512 * 100 + 400);
        process_static_context_values = static_values_local;

        memset(static_values_local, 0,
               sizeof(StaticContextValues) + 512 * 100 + 400);

        ALOGD("StaticValues size %lu",
              (unsigned long)sizeof(StaticContextValues) + 512 * 100 + 400);

        
        int b[] = {2, 3, 4};
        r_glGetStaticValues(nullptr,
                            sizeof(StaticContextValues) + 512 * 100 + 400,
                            static_values_local);

        static_values_local->vendor =
            (uintptr_t)((char *)static_values_local +
                        (unsigned long long)(static_values_local->vendor) +
                        sizeof(StaticContextValues));
        static_values_local->version =
            (uintptr_t)((char *)static_values_local +
                        (unsigned long long)(static_values_local->version) +
                        sizeof(StaticContextValues));
        static_values_local->renderer =
            (uintptr_t)((char *)static_values_local +
                        (unsigned long long)(static_values_local->renderer) +
                        sizeof(StaticContextValues));
        static_values_local->shading_language_version =
            (uintptr_t)((char *)static_values_local +
                        (unsigned long long)(static_values_local
                                                 ->shading_language_version) +
                        sizeof(StaticContextValues));
        static_values_local->extensions_gles2 =
            (uintptr_t)((char *)static_values_local +
                        (unsigned long long)(static_values_local
                                                 ->extensions_gles2) +
                        sizeof(StaticContextValues));

        ALOGD("s %d %d:%s", static_values_local->major_version,
              static_values_local->num_extensions,
              (char *)static_values_local + sizeof(StaticContextValues));

        ALOGD("vendor:%s", (char *)static_values_local->vendor);
        ALOGD("version:%s", (char *)static_values_local->version);
        ALOGD("main_renderer:%s", (char *)static_values_local->renderer);
        ALOGD("shading_language_version %d:%s",
              static_values_local->num_extensions,
              (char *)static_values_local->shading_language_version);

        for (int i = 0; i < static_values_local->num_extensions; i++) {
            static_values_local->extensions[i] =
                (uintptr_t)((char *)static_values_local +
                            (unsigned long long)(static_values_local
                                                     ->extensions[i]) +
                            sizeof(StaticContextValues));
            ALOGD("extension %d :%s|", i,
                  (char *)static_values_local->extensions[i]);
        }

        composer_pid = (int)static_values_local->composer_pid;

        process_static_context_values = static_values_local;
        this->static_values = process_static_context_values;
    } else {
        this->static_values = process_static_context_values;
    }
    pthread_mutex_unlock(&context_value_lock);

    binding_status->texture_binding =
        new TextureBindingStatus[this->static_values
                                     ->max_combined_texture_image_units];
    memset(binding_status->texture_binding, 0,
           sizeof(TextureBindingStatus) *
               this->static_values->max_combined_texture_image_units);

    binding_status->sampler_binding_2d =
        new GLuint[this->static_values->max_combined_texture_image_units];
    memset(
        binding_status->sampler_binding_2d, 0,
        sizeof(GLuint) * this->static_values->max_combined_texture_image_units);

    binding_status->gbuffer_2D_binding = nullptr;
    binding_status->gbuffer_external_binding = nullptr;

    egl_default_display->add_context(this);
}

GLThreadContext::~GLThreadContext() {
    // clean BindingStatus internal resources
    BindingStatus *binding_status = &(this->context_binding_status);
    delete binding_status->bound_buffer;
    delete[] binding_status->uniform_buffer_binding_index;
    delete[] binding_status->feedback_buffer_binding_index;
    delete[] binding_status->atomic_counter_buffer_binding_index;
    delete[] binding_status->shader_storage_buffer_binding_index;

    if (this->context_share_cnt[0] == 1) {
        // clean all_program_info
        auto program_iterator = this->program_info_map->begin();
        while (program_iterator != this->program_info_map->end()) {
            GLSLProgram *now_program = program_iterator->second;
            delete now_program;
            program_iterator++;
        }
        delete this->program_info_map;

        auto texture_iterator = this->texture_info_map->begin();
        while (texture_iterator != this->texture_info_map->end()) {
            TextureInfo *info = texture_iterator->second;
            delete info;
            texture_iterator++;
        }
        delete this->texture_info_map;
        delete this->texture_gbuffer_map;

        
        BufferMapped *map_ptr = (this->buffer_ptr);
        for (auto &it : map_ptr->buffer_maps) {
            MapResult *map_res = it.second;
            if (map_res != nullptr) {
                if (map_res->map_data != nullptr &&
                    map_res->dynamic_data == nullptr) {
                    delete[] map_res->map_data;
                } else
                    delete[] map_res->dynamic_data;

                delete map_res;
            }
        }
    }

    // clean resources and shared resources
    ResourceContext *res = &(this->resource_context);
    delete res->exclusive_resources;
    if (this->context_share_cnt[0] == 1) {
        delete res->share_resources;
    }
    this->context_share_cnt[0]--;
    if (this->context_share_cnt[0] == 0) {
        delete[] this->context_share_cnt;
        delete this->context_share_thread_ids;
    }
    if (egl_default_display) egl_default_display->destroy_context(this);
}

int64_t GLThreadContext::fetch_mc_fence_add() {
    SPIN_LOCK(mc_fence_lock);
    if (mc_fence == INT64_MAX) {
        mc_fence = 0;
    }
    int64_t fence_val = mc_fence;
    mc_fence++;
    SPIN_UNLOCK(mc_fence_lock);
    return fence_val;
}

ThreadContext::ThreadContext() {
    pid = getpid();
    tid = gettid();
    remote_mem = remote_mem_mapper->map_memory(sizeof(uint64_t));
}

ThreadContext::~ThreadContext() {
    eglTerminateThread();
    remote_mem_mapper->unmap_memory(remote_mem);
    delete thread_packet_streamer;
    delete default_call_streamer;
    delete remote_mem_mapper;
    delete surface_read;
    delete surface_draw;
    delete opengl_context;
}

void ThreadContext::unbind_surfaces() const {
    // If current surface_draw exists and needs to be destroyed，
    // MakeCurrent will trigger the destruction
    if (surface_draw != nullptr) {
        surface_draw->reset_frame();
        if (surface_draw->need_destroy) {
            delete surface_draw;
        } else {
            surface_draw->has_make_current = 0;
        }
    }

    // If current surface_read exists and needs to be destroyed，
    // and current surface_draw is not the same as current surface_read
    // (otherwise it's already destroyed in the previous step), MakeCurrent will
    // trigger the destruction
    if (surface_read != nullptr && surface_draw != surface_read) {
        if (surface_read->need_destroy) {
            delete surface_read;
        } else {
            surface_read->has_make_current = 0;
        }
    }
}

void ThreadContext::unbind_gl_context() const {
    if (opengl_context != nullptr) {
        opengl_context->context_share_thread_ids->erase(gettid());
#ifdef USE_LOCAL_COMPILER
        opengl_context->glsl_compiler->make_current(false);
#endif
        ALOGD("make current destroy");
        if (opengl_context->need_destroy) {
            delete opengl_context;
        } else {
            opengl_context->has_make_current = 0;
        }
    }
}

void ThreadContext::make_current(EGLDisplay dpy, EGLSurface draw,
                                 EGLSurface read, EGLContext ctx) {
    unbind_surfaces();
    unbind_gl_context();

    display = (EglDisplay *)dpy;

    surface_draw = (EglSurface *)draw;
    if (draw != EGL_NO_SURFACE) {
        surface_draw->has_make_current = 1;
        surface_draw->call_streamer->start_frames();
    }

    surface_read = (EglSurface *)read;
    if (read != EGL_NO_SURFACE) {
        surface_read->has_make_current = 1;
    }

    opengl_context = (GLThreadContext *)ctx;
    if (ctx != EGL_NO_CONTEXT) {
        opengl_context->has_make_current = 1;
        opengl_context->context_share_thread_ids->insert({gettid(), 1});
#ifdef USE_LOCAL_COMPILER
        opengl_context->glsl_compiler->make_current(true);
#endif
    }

    if (surface_draw != nullptr) surface_draw->init(swap_interval);
}

size_t ThreadContext::get_context_size() const {
    size_t context_size = 0;
    context_size += sizeof(ThreadContext);
    context_size += sizeof(EglSurface);
    context_size += sizeof(GLThreadContext);
    context_size += sizeof(StaticContextValues);
    context_size += sizeof(EglDisplay);

    GLThreadContext *gl_context = opengl_context;
    if (gl_context == nullptr) {
        return context_size;
    }

    BoundBuffer const *bound_buffer =
        gl_context->context_binding_status.bound_buffer;
    if (bound_buffer->ebo_mm != nullptr) {
        context_size += bound_buffer->ebo_mm_num * 4;
    }

    context_size += sizeof(BufferStatus);
    context_size += sizeof(AttribPoint) * bound_buffer->vao_point_data.size();
    BindingStatus const *binding_status = &(gl_context->context_binding_status);
    context_size += binding_status->uniform_max_index;
    context_size += binding_status->uniform_max_index;
    context_size += binding_status->feedback_max_index;
    context_size += binding_status->atomic_counter_max_index;
    context_size += binding_status->shader_storage_max_index;

    auto mapIterator = gl_context->program_info_map->begin();

    while (mapIterator != gl_context->program_info_map->end()) {
        GLSLProgram const *now_program = mapIterator->second;
        // TODO: calculate the size of the program
        // context_size += now_program->uniform_detected_num * 20 +
        //                 now_program->max_uniform_gl_name_len *
        //                     now_program->uniform_detected_num;
        // context_size += now_program->attrib_detected_num * 20 +
        //                 now_program->max_attrib_gl_name_len *
        //                     now_program->attrib_detected_num;
        // context_size += now_program->uniform_block_num * 16 +
        //                 now_program->max_uniform_block_name_len *
        //                     now_program->uniform_block_num;
        context_size += sizeof(GLSLProgram);
        mapIterator++;
    }

    context_size += sizeof(TextureInfo) * gl_context->texture_info_map->size();
    for (auto it : bound_buffer->ebo_buffer) {
        ElementArrayBuffer const *ebo_data = it.second;
        if (ebo_data != nullptr) {
            context_size += ebo_data->size;
            if (ebo_data->dp_max != nullptr &&
                bound_buffer->ebo_mm != nullptr) {
                for (int i = 1; i <= bound_buffer->ebo_mm[ebo_data->dp_num];
                     i++) {
                    context_size += (ebo_data->dp_num + 5) * 2;
                }
                context_size += bound_buffer->ebo_mm[ebo_data->dp_num] * 2 + 2;
            }
        }
    }

    // buffer_ptr
    BufferMapped const *map_ptr = (gl_context->buffer_ptr);
    for (auto &it : map_ptr->buffer_maps) {
        MapResult const *map_res = it.second;
        if (map_res != nullptr) {
            if (map_res->dynamic_data != nullptr) {
                context_size += map_res->total_len;
            } else if (map_res->map_data != nullptr) {
                context_size += map_res->length;
            }
        }
    }

    context_size += sizeof(ExclusiveResource) + sizeof(ShareResource);
    context_size += 12 * 100;
    return context_size;
}

/**
 * @brief Initialize thread_context
 *
 */
void *thread_context_init() {
    // When the library is executed in a different process
    // Init everything
    if (unlikely(my_pid != getpid())) {
        ALOGD("thread_context_init called in different process");
        lib_destructor();
        lib_constructor();
    }
    my_pid = getpid();

    main_server->start();
    main_server->send_control_packet(MSG_CREATE_THREAD);

    thread_context_ptr = std::make_shared<ThreadContext>();

    return thread_context_ptr.get();
}

void send_to_host(void *context, unsigned char *send_buf, size_t send_buf_len,
                  char *ret_buf, size_t ret_buf_len) {
    (void)context;
    auto *thread_context = (ThreadContext *)egl_thread_context;
    if (unlikely(thread_context == nullptr || send_buf_len < 16 ||
                 send_buf_len % 16 != 0)) {
        return;
    }

    auto *real_surface = thread_context->surface_draw;

    if (unlikely(real_surface != nullptr &&
                 real_surface->frame_server_start_time == 0)) {
        struct timeval now {};
        gettimeofday(&now, nullptr);
        real_surface->frame_server_start_time =
            now.tv_sec * 1000000 + now.tv_usec;
    }

    uint64_t fun_id = *(uint64_t *)send_buf;
    char thread_name[16];
    pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
    ALOGD("%s: call remote function %s", thread_name,
          get_function_name(fun_id).c_str());

    FrameCallType call_type = FRAME_CALL_NORMAL;

    // When server calls MakeCurrent to set the surface, the client has not
    // called it yet At this time, avoid using the call_streamer of the server
    // surface, since the client is still using the context_less call_poller
    if (real_surface != nullptr && fun_id != FUNID_eglMakeCurrent) {
        if (fun_id == FUNID_eglSwapBuffers) call_type = FRAME_CALL_END;
        if (ret_buf != nullptr && ret_buf_len != -1)
            call_type = FRAME_CALL_FLUSH;

        real_surface->call_streamer->call_and_stream(send_buf, send_buf_len,
                                                     call_type);
        if (call_type == FRAME_CALL_FLUSH) {
            ALOGE("Call synchronous function %s",
                  get_function_name(fun_id).c_str());
            FrameCallStreamer::call_return(ret_buf, ret_buf_len);
        }
    } else {
        call_type = FRAME_CALL_CONTEXT_LESS;
        thread_context->default_call_streamer->call_and_stream(
            send_buf, send_buf_len, call_type);
        if (ret_buf != nullptr && ret_buf_len != -1) {
            FrameCallStreamer::call_return(ret_buf, ret_buf_len);
        }
    }
}