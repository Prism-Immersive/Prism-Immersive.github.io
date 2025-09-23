#include "MainRenderer.h"

#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

#include <cassert>
#include <cstring>

#include "Log.h"
#include "Utils.h"
#include "egl/GPU.h"
#include "opengl/Context.h"
#include "opengl/GLTranslator.h"
#include "opengl/GLv1.h"
#include "opengl/Helper.h"

#define LOG_TAG "MainRenderer"

#ifdef DSA
int MainRenderer::DSA_ENABLE = 1;
#else
int MainRenderer::DSA_ENABLE = 0;
#endif

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB 0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA 1
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH 2
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL 4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE 8

EGLint MainRenderer::display_config_attrib_list[] = {
        EGL_RED_SIZE,
        8,
        EGL_GREEN_SIZE,
        8,
        EGL_BLUE_SIZE,
        8,
        EGL_ALPHA_SIZE,
        ((ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL) & ES_WINDOW_ALPHA)
        ? 8
        : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,
        ((ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL) & ES_WINDOW_DEPTH)
        ? 24
        : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,
        ((ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL) & ES_WINDOW_STENCIL)
        ? 8
        : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS,
        ((ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL) &
         ES_WINDOW_MULTISAMPLE)
        ? 1
        : 0,
        EGL_SAMPLES,
        ((ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_STENCIL) &
         ES_WINDOW_MULTISAMPLE)
        ? 4
        : 0,
        EGL_RENDERABLE_TYPE,
        EGL_OPENGL_ES2_BIT,
        EGL_NONE};

EglDisplay egl_default_display;

int MainRenderer::HOST_OPENGL_VERSION = 0;
int MainRenderer::COMPOSER_REFRESH_RATE = 120;
const char MainRenderer::GPU_VENDOR[] = "ARM";
const char MainRenderer::GPU_VERSION[] = "OpenGL ES 3.1 (";
const char MainRenderer::GPU_RENDERER[] = "Mali-G77";
const char MainRenderer::GPU_SHADER_LANGUAGE_VERSION[] =
        "OpenGL ES GLSL ES 3.10";
const char *MainRenderer::SPECIAL_EXTENSIONS[] = {
        /*1*/ "GL_OES_EGL_image",
        /*2*/ "GL_OES_EGL_image_external",
        /*3*/ "GL_OES_EGL_sync",
        /*4*/ "GL_OES_depth24",
        /*5*/ "GL_OES_depth32",
        /*6*/ "GL_OES_texture_float",
        /*15*/ "GL_OES_texture_float_linear",
        /*7*/ "GL_OES_texture_half_float",
        /*8*/ "GL_OES_texture_half_float_linear",
        /*9*/ "GL_OES_compressed_ETC1_RGB8_texture",
        /*10*/ "GL_OES_depth_texture",
        /*11*/ "GL_OES_EGL_image_external_essl3",
        /*12*/ "GL_KHR_texture_compression_astc_ldr",
        /*13*/ "GL_KHR_texture_compression_astc_hdr",
        /*14*/ "GL_OES_vertex_array_object",
        // /*14*/ "GL_EXT_shader_framebuffer_fetch",
        
        // /*15*/ "GL_EXT_multisampled_render_to_texture",
        
        
        
        /*16*/ "GL_EXT_color_buffer_float",
        /*17*/ "GL_EXT_color_buffer_half_float",
        /*18*/ "GL_OES_element_index_uint",
        /*19*/ "GL_OES_texture_float_linear",
        /*20*/ "GL_OES_compressed_paletted_texture",
        /*21*/ "GL_OES_packed_depth_stencil",
        /*22*/ "GL_OES_texture_npot",
        /*23*/ "GL_OES_rgb8_rgba8",
        /*24*/ "GL_OES_framebuffer_object",
        /*25*/ "GL_ARB_texture_non_power_of_two",
        /*26*/ "GL_OES_blend_func_separate",
        /*27*/ "GL_OES_blend_equation_separate",
        /*28*/ "GL_OES_blend_subtract",
        /*29*/ "GL_OES_byte_coordinates",
        /*30*/ "GL_OES_point_size_array",
        /*31*/ "GL_OES_point_sprite",
        /*32*/ "GL_OES_single_precision",
        /*33*/ "GL_OES_stencil_wrap",
        /*34*/ "GL_OES_texture_env_crossbar",
        /*35*/ "GL_OES_texture_mirrored_repeat",
        /*36*/ "GL_OES_texture_cube_map",
        /*37*/ "GL_OES_draw_texture",
        /*38*/ "GL_OES_fbo_render_mipmap",
        /*39*/ "GL_OES_stencil8",
        /*41*/ "GL_EXT_blend_minmax",
        /*42*/ "GL_OES_standard_derivatives",
        /*43*/ "GL_EXT_robustness",
        /*44*/ "GL_EXT_copy_image",
        /*45*/ "GL_EXT_texture_buffer",
        /*46*/ "GL_OES_vertex_half_float",

        
};
MainRenderer *MainRenderer::instance = nullptr;
std::atomic_flag MainRenderer::singleton_lock = ATOMIC_FLAG_INIT;

void MainRenderer::init_static_context_values() {
    static_context_values = (Static_Context_Values *) malloc(
            sizeof(Static_Context_Values) + 512 * 100 + 400);
    memset(static_context_values, 0,
           sizeof(Static_Context_Values) + 512 * 100 + 400);

    static_context_values->composer_HZ = COMPOSER_REFRESH_RATE;
    static_context_values->composer_pid = 0;

    // initialize static status
    static_context_values->major_version = OPENGL_MAJOR_VERSION;
    static_context_values->minor_version = OPENGL_MINOR_VERSION;

    GLHelper::prepare_integer_value(static_context_values);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        ALOGD("error when creating static vaules %x\n", error);
    }
    
    if (static_context_values->max_vertex_attribs > 32) {
        static_context_values->max_vertex_attribs = 32;
    }

    if (static_context_values->max_image_units > 16) {
        static_context_values->max_image_units = 16;
    }

    if (static_context_values->max_vertex_attrib_bindings > 32) {
        static_context_values->max_vertex_attrib_bindings = 32;
    }

    static_context_values->num_compressed_texture_formats += 1;
    if (static_context_values->num_compressed_texture_formats > 128) {
        static_context_values->compressed_texture_formats[127] =
                GL_ETC1_RGB8_OES;
        static_context_values->num_compressed_texture_formats = 128;
    } else {
        static_context_values->compressed_texture_formats
        [static_context_values->num_compressed_texture_formats - 1] =
                GL_ETC1_RGB8_OES;
    }

    
    // static_context_values->num_program_binary_formats =0;
    // static_context_values->num_shader_binary_formats = 0;

    if (static_context_values->num_program_binary_formats > 8) {
        static_context_values->num_program_binary_formats = 8;
    }
    if (static_context_values->num_shader_binary_formats > 8) {
        static_context_values->num_shader_binary_formats = 8;
    }
    if (static_context_values->num_compressed_texture_formats > 128) {
        static_context_values->num_compressed_texture_formats = 128;
    }

    char *string_loc =
            ((char *) static_context_values) + sizeof(Static_Context_Values);

    char *temp_loc = string_loc;

    const char *gl_string;
    gl_string = (const char *) glGetString(GL_VENDOR);
    static_context_values->vendor = (unsigned long long) (temp_loc - string_loc);

    memcpy(temp_loc, GPU_VENDOR, sizeof(GPU_VENDOR) - 1);
    temp_loc += sizeof(GPU_VENDOR) - 1;
    *temp_loc = 0;
    temp_loc++;
    ALOGD("\ngl vendor:%s\n", (char *) gl_string);

    gl_string = (const char *) glGetString(GL_VERSION);

    if (gl_string != nullptr && gl_string[0] == '4') {
        int major_version = gl_string[0] - '0';
        int minor_version = gl_string[2] - '0';
        if (major_version <= 4 && major_version >= 1 && minor_version >= 1 &&
            minor_version <= 9) {
            HOST_OPENGL_VERSION = major_version * 10 + minor_version;
        }
        // HOST_OPENGL_VERSION = 0;
    }

    static_context_values->version =
            (unsigned long long) (temp_loc - string_loc);

    memcpy(temp_loc, GPU_VERSION, sizeof(GPU_VERSION) - 1);
    temp_loc += sizeof(GPU_VERSION) - 1;
    memcpy(temp_loc, gl_string, strlen(gl_string));
    temp_loc += strlen(gl_string);
    *temp_loc = ')';
    temp_loc++;
    *temp_loc = 0;
    temp_loc++;
    ALOGD("gl version:%s\n",
          string_loc + (unsigned long) (static_context_values->version));

    gl_string = (const char *) glGetString(GL_RENDERER);
    static_context_values->renderer =
            (unsigned long long) (temp_loc - string_loc);

    memcpy(temp_loc, GPU_RENDERER, sizeof(GPU_RENDERER) - 1);
    temp_loc += sizeof(GPU_RENDERER) - 1;
    // memcpy(temp_loc, gl_string, strlen(gl_string));
    // temp_loc += strlen(gl_string);
    // *temp_loc = ')';
    // temp_loc++;
    *temp_loc = 0;
    temp_loc++;
    ALOGD("gl renderer:%s\n", (char *) gl_string);

    static_context_values->shading_language_version =
            (unsigned long long) (temp_loc - string_loc);
    memcpy(temp_loc, GPU_SHADER_LANGUAGE_VERSION,
           sizeof(GPU_SHADER_LANGUAGE_VERSION) - 1);
    temp_loc += sizeof(GPU_SHADER_LANGUAGE_VERSION) - 1;
    *temp_loc = 0;
    temp_loc++;
    ALOGD("gl shading_language_version:%s\n",
          string_loc +
          (unsigned long) (static_context_values->shading_language_version));

    char *extensions_start = temp_loc;

    // int no_need_extensions_cnt = 0;
    int num_extensions = static_context_values->num_extensions;

    // num_extensions = 0;
    

    DSA_ENABLE = 0;

    ALOGD("host gl %d DSA_enable %d\n", HOST_OPENGL_VERSION, DSA_ENABLE);

    for (int i = 0; i < SPECIAL_EXTENSIONS_SIZE; i++) {
        static_context_values->extensions[i] = temp_loc - string_loc;

        memcpy(temp_loc, SPECIAL_EXTENSIONS[i], strlen(SPECIAL_EXTENSIONS[i]));
        temp_loc += strlen(SPECIAL_EXTENSIONS[i]);
        *temp_loc = 0;
        temp_loc++;
    }

    num_extensions = SPECIAL_EXTENSIONS_SIZE;

    static_context_values->num_extensions = num_extensions;

    int extensions_len = temp_loc - extensions_start;

    memcpy(temp_loc, extensions_start, extensions_len);
    for (int i = 0; i < extensions_len; i++) {
        if (*temp_loc == 0) {
            *temp_loc = ' ';
        }
        temp_loc++;
    }
    temp_loc--;
    if (*temp_loc == ' ') {
        *temp_loc = 0;
    }

    static_context_values->extensions_gles2 =
            (extensions_start - string_loc + extensions_len);
    ALOGD(
            "extensions len %d num %d: %s|\n", extensions_len, num_extensions,
            string_loc + (unsigned long) (static_context_values->extensions_gles2));
    assert(temp_loc < ((char *) static_context_values) +
                      sizeof(Static_Context_Values) + 512 * 100 + 400);
    static_context_values_initialized = true;
}

MainRenderer *MainRenderer::get_instance() {
    if (instance == nullptr) {
        SPIN_LOCK(singleton_lock);
        if (instance == nullptr) {
            instance = new MainRenderer();
        }
        SPIN_UNLOCK(singleton_lock);
    }
    return instance;
}

MainRenderer::MainRenderer() {
    static_context_values = nullptr;

    main_program_id = 0;
    main_draw_vao = 0;

    window_height = 0;
    window_width = 0;

    main_renderer_should_run = false;
};

void MainRenderer::prepare_egl() {
    int width = ANativeWindow_getWidth(main_native_window);
    int height = ANativeWindow_getHeight(main_native_window);

    EGLConfig config;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

    EGLNativeDisplayType eglNativeDisplay = nullptr;

    ALOGD("window width: %d; height: %d", width, height);
    main_native_display = eglGetDisplay(eglNativeDisplay);
    if (main_native_display == EGL_NO_DISPLAY) {
        ALOGE("no display, %s", __FUNCTION__);
    }

    // Initialize EGL
    if (!eglInitialize(main_native_display, &majorVersion, &minorVersion)) {
        ALOGE("initialize egl failed, %s", __FUNCTION__);
    }

    {
        EGLint numConfigs = 0;
        // Choose config
        if (!eglChooseConfig(main_native_display, display_config_attrib_list,
                             &config, 1, &numConfigs)) {
            ALOGE("eglChooseConfig return false");
        }
        if (numConfigs < 1) {
            ALOGE("no suitable egl config, %s", __FUNCTION__);
        }
    }

    // For Android, need to get the EGL_NATIVE_VISUAL_ID and set it using
    // ANativeWindow_setBuffersGeometry
    {
        EGLint format = 0;
        eglGetConfigAttrib(main_native_display, config, EGL_NATIVE_VISUAL_ID,
                           &format);
        ANativeWindow_setBuffersGeometry(main_native_window, 0, 0, format);
    }

    // Create a surface
    main_native_surface = eglCreateWindowSurface(main_native_display, config,
                                                 main_native_window, nullptr);

    if (main_native_surface == EGL_NO_SURFACE) {
        ALOGE("EGL_NO_SURFACE");
        return;
    }
    // Create a GL context
    main_native_context = eglCreateContext(main_native_display, config,
                                           EGL_NO_CONTEXT, contextAttribs);
    ALOGD("now context %lx", (unsigned long) main_native_context);
    if (main_native_context == EGL_NO_CONTEXT) {
        ALOGE("EGL_NO_CONTEXT");
        return;
    }

    // Make the context current
    if (!eglMakeCurrent(main_native_display, main_native_surface,
                        main_native_surface, main_native_context)) {
        ALOGE("eglMakeCurrent failed");
        return;
    }
    ALOGD("surface is %lx", (unsigned long) main_native_surface);

    
    EGLBoolean ret = eglSwapInterval(main_native_display, 0);
    ALOGD("eglSwapInterval %d", ret);
}

void MainRenderer::prepare_opengl() {
    GLv1::prepare_draw_texi();
    init_static_context_values();
    GLHelper::main_window_opengl_prepare(&main_program_id, &main_draw_vao);

    glBindVertexArray(main_draw_vao);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    rotate_loc = glGetUniformLocation(main_program_id, "rotation");
    glUniform1i(rotate_loc, main_render_rotation == 0 ? 0 : 1);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    
    glDisable(GL_BLEND);
}

void *MainRenderer::main_render_thread(void *renderer) {
    auto *main_renderer = (MainRenderer *) renderer;
    main_renderer->prepare_egl();
    main_renderer->prepare_opengl();
    main_renderer->main_renderer_should_run = true;
    main_renderer->render();
    return nullptr;
}

void MainRenderer::start(ANativeWindow *window) {
    main_native_window = window;
    pthread_create(&render_thread_id, nullptr, main_render_thread, this);
    pthread_setname_np(render_thread_id, "main_renderer");
}

void MainRenderer::force_stop() {
    main_renderer_should_run = false;
    pthread_join(render_thread_id, nullptr);

    std::queue<Message *> empty;
    std::swap(message_queue, empty);

    main_gbuffer_map.clear();
    main_gbuffer_type_map.clear();
    dying_gbuffer_list.clear();
}

GraphicBuffer *MainRenderer::get_gbuffer_from_global_map(uint64_t gbuffer_id) {
    SPIN_LOCK(main_gbuffer_map_lock);
    GraphicBuffer *gbuffer = main_gbuffer_map[gbuffer_id];
    if (gbuffer != nullptr) {
        gbuffer->remain_life_time =
                (gbuffer->usage_type == GBUFFER_TYPE_BITMAP ? MAX_BITMAP_LIFE_TIME
                                                            : MAX_WINDOW_LIFE_TIME);
    }
    SPIN_UNLOCK(main_gbuffer_map_lock);

    return gbuffer;
}

void MainRenderer::add_gbuffer_to_global(GraphicBuffer *global_gbuffer) {
    SPIN_LOCK(main_gbuffer_map_lock);
    main_gbuffer_map[global_gbuffer->gbuffer_id] = global_gbuffer;
    SPIN_UNLOCK(main_gbuffer_map_lock);
}

void MainRenderer::set_global_gbuffer_type(uint64_t gbuffer_id, int type) {
    SPIN_LOCK(main_gbuffer_type_map_lock);
    if (type == GBUFFER_TYPE_NONE) {
        //        g_hash_table_remove(main_gbuffer_type_map,
        //        (gpointer)(gbuffer_id));
        main_gbuffer_type_map.erase(gbuffer_id);
    } else {
        //        g_hash_table_insert(main_gbuffer_type_map,
        //        (gpointer)(gbuffer_id), GINT_TO_POINTER(type));
        main_gbuffer_type_map[gbuffer_id] = type;
    }
    SPIN_UNLOCK(main_gbuffer_type_map_lock);
}

int MainRenderer::get_global_gbuffer_type(uint64_t gbuffer_id) {
    //    ATOMIC_LOCK(gbuffer_global_types_lock);
    SPIN_LOCK(main_gbuffer_type_map_lock);
    //    int type = (int)(uint64_t)g_hash_table_lookup(main_gbuffer_type_map,
    //    (gpointer)(gbuffer_id));
    int type = main_gbuffer_type_map[gbuffer_id];
    //    ATOMIC_UNLOCK(gbuffer_global_types_lock);
    SPIN_UNLOCK(main_gbuffer_type_map_lock);
    return type;
}

void MainRenderer::set_display_gbuffer(GraphicBuffer *gbuffer) {
    main_display_gbuffer = gbuffer;
}

void MainRenderer::render() {
    struct timespec start_time{
    }, end_time{};
    struct timespec sleep_start_time{
    }, sleep_end_time{};
    int64_t remain_sleep_time = 0;
    while (main_renderer_should_run) {
        clock_gettime(CLOCK_MONOTONIC, &start_time);

        handle_window_event();

        if (main_display_gbuffer != nullptr) {
            opengl_paint(main_display_gbuffer);
            eglSwapBuffers(main_native_display, main_native_surface);
        }

        clock_gettime(CLOCK_MONOTONIC, &end_time);
        int64_t spent_time = ELAPSED_MICROSECONDS(start_time, end_time);
        int64_t need_sleep_time =
                1000000 / COMPOSER_REFRESH_RATE - spent_time + remain_sleep_time;
        if (need_sleep_time <= 0) need_sleep_time = 0;

        sleep_start_time = end_time;
        usleep(need_sleep_time);
        clock_gettime(CLOCK_MONOTONIC, &sleep_end_time);
        remain_sleep_time =
                need_sleep_time -
                ELAPSED_MICROSECONDS(sleep_start_time, sleep_end_time);
    }
}

void MainRenderer::handle_window_event() {
    bool is_empty_queue;
    Message *msg;
    while (true) {
        SPIN_LOCK(message_queue_lock);
        is_empty_queue = message_queue.empty();
        if (!is_empty_queue) {
            msg = message_queue.front();
            message_queue.pop();
        }
        SPIN_UNLOCK(message_queue_lock);
        if (is_empty_queue) break;

        switch (msg->msg) {
            case MAIN_DESTROY_GBUFFER: {
                auto *gbuffer = (GraphicBuffer *) msg->data;
                if (gbuffer->gbuffer_id == 0) {
                    GraphicBuffer::destroy_gbuffer(gbuffer);
                } else {
                    dying_gbuffer_list.push_back(gbuffer);
                }
                break;
            }
            case MAIN_CANCEL_GBUFFER: {
                auto *gbuffer = (GraphicBuffer *) msg->data;
                if (gbuffer != nullptr) {
                    dying_gbuffer_list.erase(
                            std::remove(dying_gbuffer_list.begin(),
                                        dying_gbuffer_list.end(), gbuffer),
                            dying_gbuffer_list.end());
                }
                break;
            }

            case MAIN_DESTROY_ALL_EGLSYNC: {
                auto *status = (ResourceMapStatus *) msg->data;
                if (status == nullptr || status->max_id == 0) {
                    break;
                }
                for (int i = 1; i <= status->max_id; i++) {
                    if (status->resource_id_map[i] != 0) {
                        glDeleteSync((GLsync) status->resource_id_map[i]);
                    }
                }
                delete status->resource_id_map;
                delete status->resource_is_init;
                delete status->gbuffer_ptr_map;
                delete status;
                break;
            }
            case MAIN_DESTROY_ONE_SYNC: {
                auto sync = (GLsync) msg->data;
                if (sync == nullptr) {
                    break;
                }

                glDeleteSync(sync);
                break;
            }
            default: {
                ALOGE("Unknown message type %d", msg->msg);
                break;
            }
        }
        delete msg;
    }
    int result;
    auto it = dying_gbuffer_list.begin();
    while (!dying_gbuffer_list.empty() && it != dying_gbuffer_list.end()) {
        result = try_destroy_gbuffer(*it);
        if (result)
            it = dying_gbuffer_list.erase(it);
        else
            ++it;
    }
}

void MainRenderer::send_window_message(int msg, void *data) {
    auto *message = new Message();
    message->msg = msg;
    message->data = data;

    SPIN_LOCK(message_queue_lock);
    message_queue.push(message);
    SPIN_UNLOCK(message_queue_lock);
}

void MainRenderer::opengl_paint(GraphicBuffer *gbuffer) {
    if (gbuffer != nullptr) {
        gbuffer->remain_life_time = MAX_WINDOW_LIFE_TIME;

        if (unlikely(should_rotate)) {
            glUniform1i(rotate_loc, main_render_rotation == 0 ? 0 : 1);
            should_rotate = false;
        }

        if (window_width == 0 || window_height == 0) {
            window_width = ANativeWindow_getWidth(main_native_window);
            window_height = ANativeWindow_getHeight(main_native_window);
            glViewport(0, 0, window_width, window_height);
        }

        if (gbuffer->data_sync != nullptr) {
            // glClientWaitSync(gbuffer->data_sync, GL_SYNC_FLUSH_COMMANDS_BIT,
            // 1000000000);
            glWaitSync(gbuffer->data_sync, 0, GL_TIMEOUT_IGNORED);
            if (gbuffer->delete_sync != nullptr) {
                glDeleteSync(gbuffer->delete_sync);
            }
            gbuffer->delete_sync = gbuffer->data_sync;
            gbuffer->data_sync = nullptr;
        }

        glBindTexture(GL_TEXTURE_2D, gbuffer->data_texture);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        gbuffer->data_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }
}

int MainRenderer::try_destroy_gbuffer(GraphicBuffer *gbuffer) {
    if (gbuffer == nullptr || gbuffer->is_dying == 0) {
        return 1;
    }

    if (gbuffer->remain_life_time > 0) {
        gbuffer->remain_life_time--;
        return 0;
    }

    if (main_display_gbuffer == gbuffer) {
        main_display_gbuffer = nullptr;
    }

    if (gbuffer->gbuffer_id != 0) {
        
        remove_gbuffer_from_global_map(gbuffer->gbuffer_id);
    }

    if (gbuffer->usage_type == GBUFFER_TYPE_BITMAP) {
        set_global_gbuffer_type(gbuffer->gbuffer_id,
                                GBUFFER_TYPE_BITMAP_NEED_DATA);
    } else if (gbuffer->usage_type == GBUFFER_TYPE_FBO) {
        set_global_gbuffer_type(gbuffer->gbuffer_id,
                                GBUFFER_TYPE_FBO_NEED_DATA);
    } else {
        set_global_gbuffer_type(gbuffer->gbuffer_id, GBUFFER_TYPE_NONE);
    }
    GraphicBuffer::destroy_gbuffer(gbuffer);
    // printf("gbuffer %llx is dead\n",gbuffer->gbuffer_id);

    return 1;
}

void MainRenderer::remove_gbuffer_from_global_map(uint64_t gbuffer_id) {
    SPIN_LOCK(main_gbuffer_map_lock);
    main_gbuffer_map.erase(gbuffer_id);
    SPIN_UNLOCK(main_gbuffer_map_lock);
}

void MainRenderer::set_rotation(int rotation) {
    main_render_rotation = rotation;
    should_rotate = true;
}
