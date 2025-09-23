#include "Sync.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglext_angle.h>
#include <EGL/eglplatform.h>
#include <GLES/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl3platform.h>

#include "Context.h"
#include "Log.h"
#include "opengl/Resource.h"
#include "renderer/MainRenderer.h"
#include "renderer/RenderThread.h"

#define LOG_TAG "EglSync"

EGLBoolean EglSync::d_eglWaitGL(void *context) {
    
    glFinish();
    return EGL_TRUE;
}

EGLBoolean EglSync::d_eglWaitNative(void *context, EGLint engine) {
    
    glFinish();
    return EGL_TRUE;
}

EGLBoolean EglSync::d_eglWaitClient(void *context) {
    
    glFinish();
    return EGL_TRUE;
}

EGLBoolean EglSync::d_eglGetSyncAttrib(void *context, EGLDisplay dpy,
                                       EGLSync sync, EGLint attribute,
                                       EGLint *value) {
    unsigned int guest_sync_int = (unsigned int)(uint64_t)sync;

    ThreadContext *thread_context = (ThreadContext *)context;
    ResourceMapStatus *status =
        thread_context->process_context->egl_sync_resource;

    GLsync host_sync =
        (GLsync)GLResource::get_host_resource_id(status, guest_sync_int);

    if (host_sync == NULL || attribute != EGL_SYNC_STATUS) {
        ALOGD(
            "error! egGetSyncAttrib guest sync %d host sync %lld, "
            "opengl_context %llx, attribute %x\n",
            guest_sync_int, (uint64_t)host_sync,
            (uint64_t)thread_context->opengl_context, attribute);
        if (attribute == EGL_SYNC_STATUS) {
            *value = EGL_UNSIGNALED;
        }
        return EGL_TRUE;
    }

    if (thread_context->opengl_context == NULL) {
        ALOGD("eglGetSyncAttrib with null opengl context");
        //        egl_makeCurrent(dummy_window_for_sync);
        ALOGE("egl_makeCurrent not implemented");
    }

    GLint now_status = 0;
    GLsizei len = 0;
    glGetSynciv(host_sync, GL_SYNC_STATUS, (GLsizei)sizeof(GLint), &len,
                &now_status);

    if (thread_context->opengl_context == NULL) {
        EglContext::native_make_current(NULL);
    }

    if (now_status == GL_SIGNALED) {
        *value = EGL_SIGNALED;
    } else if (now_status == GL_UNSIGNALED) {
        *value = EGL_UNSIGNALED;
    } else {
        
    }

    return EGL_TRUE;
}

void EglSync::d_eglCreateSync(void *context, EGLDisplay dpy, EGLenum type,
                              const EGLint *attrib_list, EGLSync guest_sync) {
    ThreadContext *thread_context = (ThreadContext *)context;

    if (thread_context->opengl_context == NULL) {
        ALOGD("eglCreateSync with null opengl context");
        //        egl_makeCurrent(dummy_window_for_sync);
        ALOGE("egl_makeCurrent not implemented");
    }

    GLsync host_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    if (thread_context->opengl_context == NULL) {
        EglContext::native_make_current(NULL);
    }

    ResourceMapStatus *status =
        thread_context->process_context->egl_sync_resource;

    unsigned int guest_sync_int = (unsigned int)(uint64_t)guest_sync;
    unsigned long long host_sync_long = (unsigned long long)host_sync;

    GLResource::create_host_map_ids(status, 1, &guest_sync_int,
                                    &host_sync_long);

    return;
}

EGLBoolean EglSync::d_eglDestroySync(void *context, EGLDisplay dpy,
                                     EGLSync sync) {
    unsigned int guest_sync_int = (unsigned int)(uint64_t)sync;

    ThreadContext *thread_context = (ThreadContext *)context;

    ResourceMapStatus *status =
        thread_context->process_context->egl_sync_resource;

    if (!GLResource::guest_has_resource_id(status, guest_sync_int)) {
        return EGL_FALSE;
    }

    GLsync host_sync =
        (GLsync)GLResource::get_host_resource_id(status, guest_sync_int);

    GLResource::remove_host_map_ids(status, 1, &guest_sync_int);
    if (thread_context->opengl_context == NULL) {
        //        send_message_to_main_window(MAIN_DESTROY_ONE_SYNC, host_sync);
        MainRenderer::get_instance()->send_window_message(MAIN_DESTROY_ONE_SYNC,
                                                          host_sync);
    } else {
        glDeleteSync(host_sync);
    }

    return EGL_TRUE;
}

EGLBoolean EglSync::d_eglWaitSync(void *context, EGLDisplay dpy, EGLSync sync,
                                  EGLint flags) {
    unsigned int guest_sync_int = (unsigned int)(uint64_t)sync;

    ThreadContext *thread_context = (ThreadContext *)context;
    ResourceMapStatus *status =
        thread_context->process_context->egl_sync_resource;

    GLsync host_sync =
        (GLsync)GLResource::get_host_resource_id(status, guest_sync_int);

    if (host_sync != NULL) {
        if (thread_context->opengl_context == NULL) {
            ALOGD("eglWaitSync with null opengl context");
            //            egl_makeCurrent(dummy_window_for_sync);
            ALOGE("egl_makeCurrent not implemented");
        }
        glWaitSync(host_sync, 0, GL_TIMEOUT_IGNORED);
        if (thread_context->opengl_context == NULL) {
            //            egl_makeCurrent(NULL);
            ALOGE("egl_makeCurrent not implemented");
        }
        return EGL_TRUE;
    }
    return EGL_FALSE;
}

EGLint EglSync::d_eglClientWaitSync(void *context, EGLDisplay dpy, EGLSync sync,
                                    EGLint flags, EGLTime timeout) {
    unsigned int guest_sync_int = (unsigned int)(uint64_t)sync;
    GLenum ret = 0;

    ThreadContext *thread_context = (ThreadContext *)context;
    ResourceMapStatus *status =
        thread_context->process_context->egl_sync_resource;

    GLsync host_sync =
        (GLsync)GLResource::get_host_resource_id(status, guest_sync_int);
    if (host_sync != NULL) {
        
        
        //
        if (thread_context->opengl_context == NULL) {
            ALOGD("eglClientWaitSync with null opengl context\n");
            //            egl_makeCurrent(dummy_window_for_sync);
            ALOGE("egl_makeCurrent not implemented");
        }
        if ((flags & EGL_SYNC_FLUSH_COMMANDS_BIT) != 0) {
            glFlush();
        }
        ret = glClientWaitSync(host_sync, GL_SYNC_FLUSH_COMMANDS_BIT,
                               (GLuint64)timeout);
        if (thread_context->opengl_context == NULL) {
            EglContext::native_make_current(NULL);
        }
        if (ret == GL_TIMEOUT_EXPIRED) {
            return EGL_TIMEOUT_EXPIRED;
        } else if (ret == GL_CONDITION_SATISFIED ||
                   ret == GL_ALREADY_SIGNALED) {
            return EGL_CONDITION_SATISFIED;
        } else {
            return EGL_CONDITION_SATISFIED;
        }
    }
    ALOGD("eglClientWaitSync guest_sync %d no host sync\n", guest_sync_int);
    return EGL_FALSE;
}