#include "Sync.h"

#include <errno.h>

#include "EglDef.h"
#include "EglUtils.h"
#include "ThreadContext.h"
#include "opengl/GLDef.h"

#define LOG_TAG "egl_sync"


ResourceCreator egl_sync_resource;

EGLBoolean d_eglWaitGL_special(void *context) {
    (void)context;
    void *opengl_context =
        ((ThreadContext *)egl_thread_context)->opengl_context;
    if (opengl_context == nullptr) {
        // RETURN_ERROR(EGL_FALSE, EGL_BAD_MATCH);
        return EGL_TRUE;
    }
    r_glFinish(opengl_context);
    return EGL_TRUE;
}

EGLBoolean d_eglWaitNative_special(void *context, EGLint engine) {
    if (engine != EGL_CORE_NATIVE_ENGINE) {
        // EGL_BAD_PARAMETER
        RETURN_ERROR(EGL_FALSE, EGL_BAD_PARAMETER);
    }

    r_eglWaitNative(context, engine);
    
    struct timespec req {
        .tv_sec = 0, .tv_nsec = 1000000L
    };
    struct timespec rem;
    if (nanosleep(&req, &rem) == -1) {
        ALOGE("nanosleep error %d, %s", errno, strerror(errno));
    }
    return true;
}

EGLBoolean d_eglWaitClient_special(void *context) {
    (void)context;
    void *opengl_context =
        ((ThreadContext *)egl_thread_context)->opengl_context;
    if (opengl_context == nullptr) {
        // RETURN_ERROR(EGL_FALSE, EGL_BAD_MATCH);
        return EGL_TRUE;
    }
    r_glFinish(opengl_context);
    return EGL_TRUE;
}

EGLSync d_eglCreateSync_special(void *context, EGLDisplay dpy, EGLenum type,
                                const EGLint *attrib_list) {
    if (type != EGL_SYNC_FENCE && type != EGL_SYNC_NATIVE_FENCE_ANDROID) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_PARAMETER);
        return EGL_NO_SYNC;
    }

    auto sync = (EGLSync)(unsigned long long)egl_sync_resource.create_id();
    EGLint temp_attrib = EGL_NONE;
    if (attrib_list == nullptr) {
        attrib_list = &temp_attrib;
    }

    r_eglCreateSync(context, dpy, type, attrib_list, sync);

    ALOGD("Create sync %lld opengl context %llx", (long long)sync,
          (unsigned long long)opengl_context);

    return sync;
}

EGLBoolean d_eglGetSyncAttrib_special(void *context, EGLDisplay dpy,
                                      EGLSync sync, EGLint attribute,
                                      EGLint *value) {
    (void)context;
    (void)dpy;
    void const *opengl_context =
        ((ThreadContext *)egl_thread_context)->opengl_context;
    // if (opengl_context == NULL)
    // {
    //     RETURN_ERROR(EGL_FALSE, EGL_BAD_MATCH);
    // }
    ALOGD("Get sync attrib %x sync 0x%lx  opengl context %llx", attribute,
          (long)sync, (unsigned long long)opengl_context);

    if (!egl_sync_resource.has_id((GLuint)(unsigned long long)sync)) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_PARAMETER);
    }

    if (attribute == EGL_SYNC_TYPE) {
        *value = EGL_SYNC_FENCE;
    } else if (attribute == EGL_SYNC_STATUS) {
        GLint temp_status = 0;
        GLsizei len;

        
        *value = EGL_SIGNALED;
        //        r_eglGetSyncAttrib(context, dpy, sync, attribute, value);
        //        if (temp_status == GL_SIGNALED)
        //        {
        //            *value = EGL_SIGNALED;
        //        }
        //        else if (temp_status == GL_UNSIGNALED)
        //        {
        //            *value = EGL_UNSIGNALED;
        //        }
        //        else
        //        {
        
        //        }
    } else if (attribute == EGL_SYNC_CONDITION) {
        *value = EGL_SYNC_PRIOR_COMMANDS_COMPLETE;
    } else {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
    }
    return EGL_TRUE;
}

EGLBoolean d_eglDestroySync_special(void *context, EGLDisplay dpy,
                                    EGLSync sync) {
    void const *opengl_context =
        ((ThreadContext *)egl_thread_context)->opengl_context;

    ALOGD("destroy sync 0x%lx  opengl context %llx", (long)sync,
          (unsigned long long)opengl_context);

    if (!egl_sync_resource.has_id((GLuint)(unsigned long long)sync)) {
        return EGL_FALSE;
    } else {
        egl_sync_resource.delete_id((GLuint)(unsigned long long)sync);
        r_eglDestroySync(context, dpy, sync);
        return EGL_TRUE;
    }
}

EGLBoolean d_eglWaitSync_special(void *context, EGLDisplay dpy, EGLSync sync,
                                 EGLint flags) {
    void const *opengl_context =
        ((ThreadContext *)egl_thread_context)->opengl_context;
    ALOGD("eglWaitSync with context %llx sync %lld flag %x",
          (unsigned long long)opengl_context, (unsigned long long)sync, flags);

    if (flags != 0) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_PARAMETER);
    }

    if (!egl_sync_resource.has_id((GLuint)(unsigned long long)sync)) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_PARAMETER);
    } else {
        r_eglWaitSync(context, dpy, sync, flags);
        return EGL_TRUE;
    }
}

EGLint d_eglClientWaitSync_special(void *context, EGLDisplay dpy, EGLSync sync,
                                   EGLint flags, EGLTime timeout) {
    // ALOGD("Wait sync is 0x%lx", (long)sync);
    void const *opengl_context =
        ((ThreadContext *)egl_thread_context)->opengl_context;
    ALOGD("eglClientWaitSync with context %llx sync %llx",
          (unsigned long long)opengl_context, (unsigned long long)sync);

    GLenum ret;
    if (!egl_sync_resource.has_id((GLuint)(unsigned long long)sync)) {
        ALOGD("no find sync %lld", (long long)sync);
        RETURN_ERROR(EGL_FALSE, EGL_BAD_PARAMETER);
    } else {
        
        //
        ret = r_eglClientWaitSync(context, dpy, sync, flags, timeout);
        //        if (timeout == EGL_FOREVER)
        //        {
        
        //            ret = r_eglClientWaitSync(context, dpy, sync, flags,
        //            100000000); while (ret == EGL_TIMEOUT_EXPIRED)
        //            {
        //                ret = r_eglClientWaitSync(context, dpy, sync, flags,
        //                100000000);
        //            }
        //        }
        //        else
        //        {
        //            ret = r_eglClientWaitSync(context, dpy, sync, flags,
        //            100000000);
        //        }
        ALOGD("client wait sync return %x", ret);
        return ret;
    }
}