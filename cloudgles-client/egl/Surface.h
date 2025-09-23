#ifndef EGL_SURFACE_H
#define EGL_SURFACE_H

#include <EGL/egl.h>
#include <GLES/gl.h>

#include "egl/Config.h"
#include "network/FrameCallPoller.h"
#include "renderer/GraphicBuffer.h"

#define P_SURFACE 1
#define WINDOW_SURFACE 2

class EglSurface {
   public:
    int type = 0;
    EglConfig *config = nullptr;
    GraphicBuffer *gbuffer = nullptr;
    // window's width and height, only used by pbuffer
    int width = 0;
    int height = 0;
    int is_current = 0;
    void *reset_current_event = nullptr;
    int need_destroy = 0;

    int64_t last_calc_time = 0;
    int now_screen_hz = 0;

    int sampler_num = 0;
    int format = 0;
    GLenum pixel_type = 0;
    int internal_format = 0;
    GLenum depth_internal_format = 0;
    GLenum stencil_internal_format = 0;

    GLuint now_fbo_loc = 0;
    GLuint data_fbo[3] = {0};
    GLuint sampler_fbo[3] = {0};
    GLuint connect_texture[3] = {0};

    FrameCallPoller *call_poller = nullptr;

    EglSurface(EGLConfig egl_config, int surface_width, int surface_height,
               int surface_type);
    EglSurface(const EglSurface &buffer) = delete;
    EglSurface &operator=(const EglSurface &) = delete;
    ~EglSurface();

    void init();

    void reset();

    void set_gbuffer(uint64_t gbuffer_id, int hal_format, int gbuffer_width,
                     int gbuffer_height);

    void swap_gbuffer(void *render_context, uint64_t next_gbuffer_id,
                      int gbuffer_width, int gbuffer_height, int hal_format);

    // void destroy_real_image(EGL_Image *real_image);

    static void d_eglIamComposer(void *context, EGLSurface surface,
                                 unsigned int pid);

    static void d_eglCreatePbufferSurface(void *context, EGLDisplay dpy,
                                          EGLConfig config,
                                          const EGLint *attrib_list,
                                          EGLSurface guest_surface);

    static EGLBoolean d_eglDestroySurface(void *context, EGLDisplay dpy,
                                          EGLSurface surface);

    static EGLBoolean d_eglSurfaceAttrib(void *context, EGLDisplay dpy,
                                         EGLSurface surface, EGLint attribute,
                                         EGLint value);

    static EGLint d_eglCreateImage(void *context, EGLDisplay dpy,
                                   EGLContext ctx, EGLenum target,
                                   EGLClientBuffer buffer,
                                   const EGLint *attrib_list,
                                   EGLImage guest_image);

    // void d_eglRemainImage(void *context, EGLImage image);

    static EGLBoolean d_eglDestroyImage(void *context, EGLDisplay dpy,
                                        EGLImage image);

    //    static int egl_surface_init(EglSurface *d_buffer, void *now_window,
    //    int need_draw);

    static void g_surface_map_destroy(EglSurface *data);

    // void init_image_texture(EGL_Image *image);
    static void d_eglCreateWindowSurface(void *context, EGLDisplay dpy,
                                         EGLConfig config,
                                         EGLNativeWindowType win,
                                         const EGLint *attrib_list,
                                         EGLSurface guest_surface);
};

#endif  // EGL_SURFACE_H
