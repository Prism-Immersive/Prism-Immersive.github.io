#ifndef EGL_CONTEXT_H
#define EGL_CONTEXT_H

#include <EGL/egl.h>

class EglContext {
   public:
    static void native_make_current(void *context);

    static void d_eglCreateContext(void *context, EGLDisplay dpy,
                                   EGLConfig config, EGLContext share_context,
                                   const EGLint *attrib_list,
                                   EGLContext guest_context);
    static void d_eglMakeCurrent(void *context, EGLDisplay dpy,
                                 EGLSurface draw_surface_handle,
                                 EGLSurface read_surface_handle,
                                 EGLContext ctx_handle, uint64_t gbuffer_id,
                                 int width, int height, int hal_format,
                                 int64_t mc_fence);
    static EGLBoolean d_eglDestroyContext(void *context, EGLDisplay dpy,
                                          EGLContext ctx);
    static EGLBoolean d_eglTerminate(void *context, EGLDisplay dpy);
    static EGLBoolean d_eglReleaseThread(void *context);
    static void d_eglTerminateThread(void *context);
};

#endif  // EGL_CONTEXT_H
