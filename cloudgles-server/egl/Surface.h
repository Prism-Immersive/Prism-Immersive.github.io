#ifndef EGL_CONTEXT_H
#define EGL_CONTEXT_H

#include <cstdint>
#if defined(ANDROID_SYSTEM)
#include <system/window.h>
#if ANDROID_PLATFORM_SDK_VERSION > 28
#include <nativebase/nativebase.h>
#endif
#include <ui/GraphicBuffer.h>
#else
#include "Constants.h"
#endif

#include <EGL/egl.h>

#include <memory>

#include "network/FrameCallStreamer.h"

#define NO_SURFACE (-1)
#define PBUFFER_SURFACE (0)
#define WINDOW_SURFACE (1)
#define MAX_FRAME_NUM (INT32_MAX - 1)
#define MAX_SWAP_DATA_NUM (4096)

class EglSurface {
   public:
    EGLint surface_type;

    // Flow control related
    bool over_sleep = false;
    int64_t remain_sleep_time = 0;
    int64_t frame_end_time = 0;
    int64_t frame_server_start_time = 0;
    int64_t frame_client_start_time = 0;
    uint64_t swap_time_data[MAX_SWAP_DATA_NUM] = {0};
    int swap_time_write_cursor = 0;
    int swap_time_read_cursor = 0;

    // EGL surface related
    EGLint config_id = 0;
    EGLint width = 0;
    EGLint height = 0;
    int window_format = 1;
    EGLint horizontal_resolution = 0;
    EGLint vertical_resolution = 0;
    EGLint gl_colorspace = EGL_GL_COLORSPACE_SRGB;
    EGLint largest_pbuffer = EGL_FALSE;
    EGLint mipmap_level = 0;
    EGLint mipmap_texture = EGL_FALSE;
    EGLint multisample_resolve = EGL_MULTISAMPLE_RESOLVE_DEFAULT;
    EGLint pixel_aspect_resolve = 0;
    EGLint render_buffer = EGL_BACK_BUFFER;
    EGLint swap_behavior = EGL_BUFFER_PRESERVED;
    EGLint texture_format = EGL_NO_TEXTURE;
    EGLint texture_target = EGL_NO_TEXTURE;
    EGLint vg_alpha_format = EGL_VG_ALPHA_FORMAT_NONPRE;
    EGLint vg_colorspace = EGL_VG_COLORSPACE_sRGB;

    // EGL context related
    EGLint has_make_current = 0;
    EGLint need_destroy = 0;

    // EGL fence related
    uint64_t fence_key = 0;

    // Android native window related
    int fence_fd = 0;
    ANativeWindow *native_window = nullptr;
#if defined(ANDROID_SYSTEM)
    ANativeWindowBuffer *native_window_buffer = nullptr;
#endif
    uint64_t gbuffer_id;
#ifndef ANDROID_SYSTEM
    uint64_t gbuffer_id_generator = 0;
#endif

    std::unique_ptr<FrameCallStreamer> call_streamer =
        std::make_unique<FrameCallStreamer>();

    explicit EglSurface(EGLint surface_type);

    ~EglSurface();

    void init(int swap_interval);
    int set_surface_attrib(const EGLint *attrib_list);
    void reset_frame_statistic();
    void reset_frame();
};

EGLSurface d_eglGetCurrentSurface_special(void *context, EGLint readdraw);

EGLBoolean d_eglQueryContext_special(void *context, EGLDisplay dpy,
                                     EGLContext ctx, EGLint attribute,
                                     EGLint *value);

EGLBoolean d_eglQuerySurface_special(void *context, EGLDisplay dpy,
                                     EGLSurface surface, EGLint attribute,
                                     EGLint *value);

EGLContext d_eglGetCurrentContext_special(void *context);

EGLSurface d_eglCreatePbufferSurface_special(void *context, EGLDisplay dpy,
                                             EGLConfig config,
                                             const EGLint *attrib_list);

EGLSurface d_eglCreateWindowSurface_special(void *context, EGLDisplay dpy,
                                            EGLConfig config,
                                            EGLNativeWindowType win,
                                            const EGLint *attrib_list);

EGLContext d_eglCreateContext_special(void *context, EGLDisplay dpy,
                                      EGLConfig config,
                                      EGLContext share_context,
                                      const EGLint *attrib_list);

EGLBoolean d_eglDestroyContext_special(void *context, EGLDisplay dpy,
                                       EGLContext ctx);

EGLBoolean d_eglDestroySurface_special(void *context, EGLDisplay dpy,
                                       EGLSurface surface);

EGLBoolean d_eglSurfaceAttrib_special(void *context, EGLDisplay dpy,
                                      EGLSurface surface, EGLint attribute,
                                      EGLint value);

#endif  // EGL_CONTEXT_H
