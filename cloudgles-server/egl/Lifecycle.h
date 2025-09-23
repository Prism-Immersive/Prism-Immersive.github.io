#ifndef EGL_LIFECYCLE_H
#define EGL_LIFECYCLE_H

#include <EGL/egl.h>

EGLBoolean d_eglInitialize_special(void *context, EGLDisplay dpy, EGLint *major,
                                   EGLint *minor);

__eglMustCastToProperFunctionPointerType d_eglGetProcAddress_special(
    void *context, const char *procname);

EGLBoolean d_eglTerminate_special(void *context, EGLDisplay dpy);

EGLBoolean d_eglReleaseThread_special(void *context);

#endif  // EGL_LIFECYCLE_H
