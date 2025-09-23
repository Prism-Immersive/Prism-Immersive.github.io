#include "Lifecycle.h"

#include "Display.h"
#include "EglDef.h"
#include "EglFunc.h"
#include "EglUtils.h"
#include "ThreadContext.h"
#include "opengl/ExtFunc.h"

#define LOG_TAG "EGL_LIFECYCLE"

/**
 * @brief 
 * @param dpy 
 * @param major 
 * @param minor 
 * @return 
 */
EGLBoolean d_eglInitialize_special(void *context, EGLDisplay dpy, EGLint *major,
                                   EGLint *minor) {
    CHECK_DISPLAY(dpy, EGL_FALSE);
    auto *g_display = (EglDisplay *)(dpy);

    if (!g_display->initialize()) {
        return EGL_FALSE;
    }
    ((ThreadContext *)context)->display = (EglDisplay *)dpy;

    if (major != nullptr) *major = g_display->get_major_ver();
    if (minor != nullptr) *minor = g_display->get_minor_ver();

    return EGL_TRUE;
}

__eglMustCastToProperFunctionPointerType d_eglGetProcAddress_special(
    void *context, const char *procname) {
    (void)context;
    void *func = nullptr;
    if (!strncmp(procname, "egl", 3)) {
        
        for (int i = 0; i < EGL_FUNC_TABLE_SIZE; i++) {
            if (!strcmp(procname, egl_func_table[i].func_name)) {
                func = egl_func_table[i].func_address;
                break;
            }
        }
    } else {
        // GL OES functions
        for (int i = 0; i < GL_EXT_FUNC_TABLE_SIZE; i++) {
            if (!strcmp(procname, gl_ext_func_table[i].func_name)) {
                func = gl_ext_func_table[i].func_address;
                break;
            }
        }
    }
    return (__eglMustCastToProperFunctionPointerType)func;
}

EGLBoolean d_eglTerminate_special(void *context, EGLDisplay dpy) {
    CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    egl_default_display->terminate();
    r_eglTerminate(context, dpy);
    return EGL_TRUE;
}

EGLBoolean d_eglReleaseThread_special(void *context) {
    auto *thread_context = (ThreadContext *)context;

    r_eglReleaseThread(context);

    thread_context->egl_error = EGL_SUCCESS;
    thread_context->opengl_context = nullptr;
    thread_context->surface_read = nullptr;
    thread_context->surface_draw = nullptr;
    thread_context->display = nullptr;
    return EGL_TRUE;
}