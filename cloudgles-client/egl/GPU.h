#ifndef EGL_GPU_H
#define EGL_GPU_H

#include <EGL/egl.h>

class EglGPU {
   public:
    static EGLBoolean d_eglSwapBuffers_sync(void *context, EGLDisplay dpy,
                                            EGLSurface surface,
                                            uint64_t gbuffer_id, int width,
                                            int height, int hal_format);

    static EGLBoolean d_eglBindTexImage(void *context, EGLDisplay dpy,
                                        EGLSurface surface, EGLint buffer);

    static EGLBoolean d_eglReleaseTexImage(void *context, EGLDisplay dpy,
                                           EGLSurface surface, EGLint buffer);

    // EGLClientBuffer d_eglGetNativeClientBufferANDROID(void *context,
    // AHardwareBuffer *buffer);

    static EGLBoolean d_eglSwapInterval(void *context, EGLDisplay dpy,
                                        EGLint interval);

    static void d_eglQueueBuffer(void *context, uint64_t gbuffer_id,
                                 int is_composer);

    static EGLBoolean d_eglSwapBuffers(void *context, EGLDisplay dpy,
                                       EGLSurface surface,
                                       uint64_t gbuffer_id, int width,
                                       int height, int hal_format,
                                       uint64_t remote_mem);

    static void d_eglSetGraphicBufferID(void *context, EGLSurface draw,
                                        uint64_t gbuffer_id);
};

#endif  // EGL_GPU_H
