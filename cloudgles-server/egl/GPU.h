#ifndef EGL_GPU_H
#define EGL_GPU_H

#include <EGL/egl.h>
#include <EGL/eglext.h>


#define SWAP_SYNC_MAX_DELAY 30

#define MAX_BITMAP_LIFE_TIME (10 / 2)

EGLBoolean d_eglMakeCurrent_special(void *context, EGLDisplay dpy,
                                    EGLSurface draw, EGLSurface read,
                                    EGLContext ctx);

EGLBoolean d_eglBindTexImage_special(void *context, EGLDisplay dpy,
                                     EGLSurface surface, EGLint buffer);

EGLBoolean d_eglReleaseTexImage_special(void *context, EGLDisplay dpy,
                                        EGLSurface surface, EGLint buffer);

EGLImage d_eglCreateImage_special(void *context, EGLDisplay dpy, EGLContext ctx,
                                  EGLenum target, EGLClientBuffer buffer,
                                  const EGLint *attrib_list);

EGLImageKHR d_eglCreateImageKHR_special(void *context, EGLDisplay dpy,
                                        EGLContext ctx, EGLenum target,
                                        EGLClientBuffer buffer,
                                        const EGLint *attrib_list);

EGLBoolean d_eglDestroyImage_special(void *context, EGLDisplay dpy,
                                     EGLImage image);

// EGLClientBuffer d_eglGetNativeClientBufferANDROID_special(void *context,
// AHardwareBuffer *buffer);

EGLBoolean d_eglSwapInterval_special(void *context, EGLDisplay dpy,
                                     EGLint interval);

/******* file '2-2' *******/

EGLBoolean d_eglSwapBuffers_special(void *context, EGLDisplay dpy,
                                    EGLSurface surface);

#endif  // EGL_GPU_H
