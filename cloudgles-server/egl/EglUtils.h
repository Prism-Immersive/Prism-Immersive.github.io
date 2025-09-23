#ifndef EGL_UTIL_H
#define EGL_UTIL_H

#include <sys/types.h>
#include <unistd.h>

#include "EGL/egl.h"
#include "Log.h"

const char *eglErrToStr(EGLint err);

#define RETURN_ERROR(ret, err)                                                 \
    ALOGE("tid %d: %s(%d): error 0x%x (%s)", gettid(), __FUNCTION__, __LINE__, \
          err, eglErrToStr(err));                                              \
    ((ThreadContext *)egl_thread_context)->egl_error = err;                    \
    return ret;

#define CHECK_DISPLAY(dpy, ret)                     \
    if ((dpy) != (EGLDisplay)egl_default_display) { \
        RETURN_ERROR(ret, EGL_BAD_DISPLAY);         \
    }

#define CHECK_DISPLAY_INIT(dpy, ret)            \
    CHECK_DISPLAY(dpy, ret);                    \
    if (!egl_default_display->is_init()) {      \
        RETURN_ERROR(ret, EGL_NOT_INITIALIZED); \
    }

#define CHECK_CONFIG(cfg, ret)                        \
    if (!egl_default_display->is_valid_config(cfg)) { \
        RETURN_ERROR(ret, EGL_BAD_CONFIG);            \
    }

#define CHECK_CONFIG_NOT_MATCHED(target, attrib, op)   \
    if ((((EGLint)target->attrib) != EGL_DONT_CARE) && \
        (target->attrib op attrib)) {                  \
        return false;                                  \
    }

#define CHECK_CONTEXT_STRICT(ctx, ret)                             \
    if (!(ctx) || !egl_default_display->is_valid_context((ctx))) { \
        RETURN_ERROR(ret, EGL_BAD_CONTEXT);                        \
    }

#define CHECK_CONTEXT(ctx, ret)                          \
    if (((ctx) != EGL_NO_CONTEXT) &&                     \
        !egl_default_display->is_valid_context((ctx))) { \
        RETURN_ERROR(ret, EGL_BAD_CONTEXT);              \
    }

#define CHECK_SURFACE_STRICT(surface, ret)                                   \
    if (!(surface) || (!egl_default_display->is_valid_surface((surface)))) { \
        RETURN_ERROR(EGL_BAD_SURFACE, EGL_FALSE);                            \
    }

#define CHECK_SURFACE(surface, ret)                            \
    if (((surface) != EGL_NO_SURFACE) &&                       \
        (!egl_default_display->is_valid_surface((surface)))) { \
        RETURN_ERROR(EGL_BAD_SURFACE, EGL_FALSE);              \
    }

#endif  // EGL_UTIL_H
