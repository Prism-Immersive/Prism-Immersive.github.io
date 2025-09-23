#ifndef THREAD_CONTEXT_H
#define THREAD_CONTEXT_H

#include <asm-generic/ioctl.h>

#include <atomic>
#include <memory>
#include <unordered_map>

#include "GLES/gl.h"
#include "data/DataTracer.h"
#include "data/ProgramCache.h"
#include "data/ResourceCompressor.h"
#include "egl/Display.h"
#include "egl/Fence.h"
#include "egl/Surface.h"
#include "network/FlowController.h"
#include "network/MainServer.h"
#include "network/RemoteMemMapper.h"
#include "network/ThreadPacketStreamer.h"
#include "opengl/Compiler.h"
#include "opengl/Framebuffer.h"
#include "opengl/Loader.h"
#include "opengl/Memory.h"
#include "opengl/Program.h"
#include "opengl/Resource.h"
#include "opengl/State.h"
#include "opengl/Texture.h"

#define GBUFFER_TYPE_NONE 0
#define GBUFFER_TYPE_WINDOW 1
#define GBUFFER_TYPE_BITMAP 2
#define GBUFFER_TYPE_NATIVE 3
#define GBUFFER_TYPE_BITMAP_NEED_DATA 4
#define GBUFFER_TYPE_TEXTURE 5
#define GBUFFER_TYPE_FBO 6
#define GBUFFER_TYPE_FBO_NEED_DATA 7

// TODO: comment out this define to verify the shader array parsing
// If the rendering is incorrect, please restore this define
// #define LOCAL_NONE_WITH_REMOTE_CHECK

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define CLOUD_GBUFFER_WIDTH 400
#define CLOUD_GBUFFER_HEIGHT 400

#define CLOUD_GBUFFER_ID 1

// OpenGL render context, including resources and operation status
class GLThreadContext {
   public:
    // gl resources
    ResourceContext resource_context{};

    // gl context status
    PixelStoreStatus pixel_store_status{};
    BindingStatus context_binding_status{};
    EnableStatus enable_status{};

    // static_values should be the same for all threads
    StaticContextValues *static_values;

    // Program, Shader, Texture, EGLImage, RenderBuffer are sharable between
    // contexts Use pointer for easy sharing
    std::unordered_map<GLuint, GLSLProgram *>
        *program_info_map;  // program ID -> program info
    std::shared_ptr<std::unordered_map<GLuint, std::unique_ptr<GLSLShader>>>
        shader_map;  // shader ID -> shader info
    std::unordered_map<GLuint, TextureInfo *>
        *texture_info_map;  // texture ID -> texture info
    std::unordered_map<GLuint, EGLImageInfo *>
        *texture_gbuffer_map;  // texture ID -> graphic buffer info
    std::shared_ptr<
        std::unordered_map<GLuint, std::unique_ptr<RenderBufferInfo>>>
        render_buffer_map;  // render buffer ID -> render buffer info

    // Query objects and Container objects are not sharable between contexts
    std::unordered_map<GLuint, GLenum>
        query_type_map;  // query ID -> query type
    std::unique_ptr<
        std::unordered_map<GLuint, std::unique_ptr<FrameBufferInfo>>>
        frame_buffer_map;  // frame buffer ID -> frame buffer info

#ifdef USE_LOCAL_COMPILER
    std::unique_ptr<GLSLCompiler> glsl_compiler;
#endif

    BufferMapped *buffer_ptr;
    uint64_t error_status = 0;

    void *share_opengl_context;
    int *context_share_cnt;
    std::unordered_map<int, int> *context_share_thread_ids;

    int has_make_current = 0;
    int need_destroy = 0;
    int config_id;

    explicit GLThreadContext(GLThreadContext *share_context);
    ~GLThreadContext();

    int64_t fetch_mc_fence_add();

   private:
    std::atomic_flag mc_fence_lock = ATOMIC_FLAG_INIT;
    int64_t mc_fence = 0;  // make current fence
};

class ThreadContext {
   public:
    int pid;
    int tid;
    EGLint egl_error = EGL_SUCCESS;
    EGLint swap_interval = 1;
    EglDisplay *display = nullptr;

    FrameCallStreamer *default_call_streamer = new FrameCallStreamer();
    ThreadPacketStreamer *thread_packet_streamer = new ThreadPacketStreamer();
    RemoteMemMapper *remote_mem_mapper = new RemoteMemMapper();
    RemoteMem *remote_mem;

    FrameCompressor frame_compressor;
    ResourceCompressor resource_compressor;
    DataTracer data_tracer;
    FlowController flow_controller;

    EglSurface *surface_read = nullptr;
    EglSurface *surface_draw = nullptr;
    GLThreadContext *opengl_context = nullptr;

    ThreadContext();
    ThreadContext(const ThreadContext &context) = delete;
    ~ThreadContext();
    ThreadContext &operator=(const ThreadContext &) = delete;

    void unbind_surfaces() const;
    void unbind_gl_context() const;
    void make_current(EGLDisplay dpy, EGLSurface draw, EGLSurface read,
                      EGLContext ctx);
    size_t get_context_size() const;
};

extern thread_local void *egl_thread_context;
extern thread_local std::shared_ptr<ThreadContext> thread_context_ptr;
extern EglDisplay *egl_default_display;
extern MainServer *main_server;
extern StaticContextValues *process_static_context_values;
extern EglFence *egl_fence;
extern std::unique_ptr<Loader> main_loader;

extern std::unique_ptr<ProgramCache> main_program_cache;

extern pid_t my_pid;
extern pid_t composer_pid;

extern std::unordered_map<EGLImage, EGLImageInfo *> all_image_info;
extern std::unordered_map<uint64_t, int> global_gbuffer_types;
extern std::unordered_map<uint64_t, uint64_t> global_gbuffer_died_times;

void *thread_context_init(void);
void send_to_host(void *context, unsigned char *send_buf, size_t send_buf_len,
                  char *ret_buf, size_t ret_buf_len);

extern "C" void register_client_status_callback(ClientStatusCallback callback);

#endif  // THREAD_CONTEXT_H
