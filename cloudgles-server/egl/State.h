#ifndef EGL_STATUS_H
#define EGL_STATUS_H

#include <EGL/egl.h>

EGLint d_eglGetError_special(void *context);

const char *d_eglQueryString_special(void *context, EGLDisplay dpy,
                                     EGLint name);

EGLBoolean d_eglBindAPI_special(void *context, EGLenum api);

EGLenum d_eglQueryAPI_special(void *context);

#endif  // EGL_STATUS_H
