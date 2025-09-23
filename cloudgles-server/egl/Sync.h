#ifndef EGL_SYNC_H
#define EGL_SYNC_H

#include <EGL/egl.h>

EGLBoolean d_eglWaitGL_special(void *context);

EGLBoolean d_eglWaitNative_special(void *context, EGLint engine);

EGLBoolean d_eglWaitClient_special(void *context);

EGLint d_eglClientWaitSync_special(void *context, EGLDisplay dpy, EGLSync sync,
                                   EGLint flags, EGLTime timeout);

EGLBoolean d_eglGetSyncAttrib_special(void *context, EGLDisplay dpy,
                                      EGLSync sync, EGLint attribute,
                                      EGLint *value);

EGLSync d_eglCreateSync_special(void *context, EGLDisplay dpy, EGLenum type,
                                const EGLint *attrib_list);

EGLBoolean d_eglDestroySync_special(void *context, EGLDisplay dpy,
                                    EGLSync sync);

EGLBoolean d_eglWaitSync_special(void *context, EGLDisplay dpy, EGLSync sync,
                                 EGLint flags);

#endif  // DISBUG_EGL_SYNC_H
