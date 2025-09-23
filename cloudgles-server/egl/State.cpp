#include "State.h"

#include "EglUtils.h"
#include "ThreadContext.h"

#define LOG_TAG "EGL_STATUS"

#include <sys/types.h>
EGLint d_eglGetError_special(void *context) {
    auto thread_context = (ThreadContext *)context;


    if (thread_context->egl_error != EGL_SUCCESS) {
        ALOGE("EGL error is 0x%x", thread_context->egl_error);
    }
    EGLint error = thread_context->egl_error;
    thread_context->egl_error = EGL_SUCCESS;
    return error;
}

const char *d_eglQueryString_special(void *context, EGLDisplay dpy,
                                     EGLint name) {
    (void)context;
    if (dpy || name != EGL_EXTENSIONS) {
        CHECK_DISPLAY_INIT(dpy, nullptr);
    }
    const char *ret = egl_default_display->query_string(name);
    if (!ret) {
        RETURN_ERROR(nullptr, EGL_BAD_PARAMETER);
    }
    return ret;
}

EGLBoolean d_eglBindAPI_special(void *context, EGLenum api) {
    (void)context;
    if (api != EGL_OPENGL_ES_API) {
        return EGL_FALSE;
    }
    return EGL_TRUE;
}

EGLenum d_eglQueryAPI_special(void *context) {
    (void)context;
    return EGL_OPENGL_ES_API;
}
