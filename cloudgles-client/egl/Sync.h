#ifndef EGL_SYNC_H
#define EGL_SYNC_H

#include <EGL/egl.h>

class EglSync {
   public:
    static EGLBoolean d_eglWaitGL(void *context);

    static EGLBoolean d_eglWaitNative(void *context, EGLint engine);

    static EGLBoolean d_eglWaitClient(void *context);

    static EGLint d_eglClientWaitSync(void *context, EGLDisplay dpy,
                                      EGLSync sync, EGLint flags,
                                      EGLTime timeout);

    static EGLBoolean d_eglGetSyncAttrib(void *context, EGLDisplay dpy,
                                         EGLSync sync, EGLint attribute,
                                         EGLint *value);

    static void d_eglCreateSync(void *context, EGLDisplay dpy, EGLenum type,
                                const EGLint *attrib_list, EGLSync guest_sync);

    static EGLBoolean d_eglDestroySync(void *context, EGLDisplay dpy,
                                       EGLSync sync);

    static EGLBoolean d_eglWaitSync(void *context, EGLDisplay dpy, EGLSync sync,
                                    EGLint flags);
};

#endif  // EGL_SYNC_H
