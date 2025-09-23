#define LOG_LEVEL LOG_LEVEL_DEBUG
#define ENABLE_OPENGL_DEBUG

#include "Context.h"

#include <GLES3/gl32.h>

#include "Log.h"
#include "Surface.h"
#include "Utils.h"
#include "renderer/MainController.h"
#include "renderer/RenderThread.h"

#define LOG_TAG "EglContext"

#ifdef ENABLE_OPENGL_DEBUG

static void gl_debug_output(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam) {
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    if (severity == GL_DEBUG_SEVERITY_LOW ||
        severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

#ifdef ENABLE_OPENGL_PERFORMANCE_WARNING

#else
    if (type == GL_DEBUG_TYPE_PERFORMANCE) {
        return;
    }
#endif

    ALOGD("debug message(%u):%s", id, message);
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            ALOGD("Source: API ");
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            ALOGD("Source: Window System ");
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            ALOGD("Source: Shader Compiler ");
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            ALOGD("Source: Third Party ");
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            ALOGD("Source: APPLICATION ");
            break;
        case GL_DEBUG_SOURCE_OTHER:
            break;
        default:
            break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            ALOGD("Type: Error ");
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            ALOGD("Type: Deprecated Behaviour ");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            ALOGD("Type: Undefined Behaviour ");
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            ALOGD("Type: Portability ");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            ALOGD("Type: Performance ");
            break;
        case GL_DEBUG_TYPE_MARKER:
            ALOGD("Type: Marker ");
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            ALOGD("Type: Push Group ");
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            ALOGD("Type: Pop Group ");
            break;
        case GL_DEBUG_TYPE_OTHER:
            ALOGD("Type: Other ");
            break;
        default:
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            ALOGD("Severity: high");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            ALOGD("Severity: medium");
            break;
        case GL_DEBUG_SEVERITY_LOW:
            ALOGD("Severity: low");
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            ALOGD("Severity: notification");
            break;
        default:
            break;
    }
}

#endif

void EglContext::d_eglCreateContext(void *context, EGLDisplay dpy,
                                    EGLConfig config, EGLContext share_context,
                                    const EGLint *attrib_list,
                                    EGLContext guest_context) {
    auto *thread_context = (ThreadContext *)context;
    ProcessContext *process_context = thread_context->process_context.get();
    GLContext *real_share_context = nullptr;
    if (share_context != nullptr) {
        real_share_context = process_context->gl_context_map[share_context];
    }

    int independ_mode = 0;

    for (int i = 0; attrib_list[i] != EGL_NONE; i += 2) {
        if (attrib_list[i] == 0xffffff && attrib_list[i + 1] == 0xffffff) {
            independ_mode = 1;
        }
    }
#ifdef DEBUG_INDEPEND_WINDOW
    independent_mode = 1;
#endif

    GLContext *opengl_context =
        GLContext::opengl_context_create(real_share_context, independ_mode);

    // TODO: only set attributes when make current, when window is created
    for (int i = 0; attrib_list[i] != EGL_NONE; i += 2) {
        ALOGD("eglcontext %llx attrib_list %x %x\n", (uint64_t)opengl_context,
              attrib_list[i], attrib_list[i + 1]);
    }
    ALOGD("#%llx context create share %llx\n", (uint64_t)opengl_context,
          (uint64_t)real_share_context);
    ALOGD("context create guest %llx host %llx\n", (uint64_t)guest_context,
          (uint64_t)opengl_context);

    opengl_context->guest_context = guest_context;
    process_context->gl_context_map[guest_context] = opengl_context;
}

void EglContext::native_make_current(void *context) {
    if (context != nullptr) {
        if (!eglMakeCurrent(MainRenderer::get_instance()->main_native_display,
                            MainRenderer::get_instance()
                                ->main_native_context_surface_map[context],
                            MainRenderer::get_instance()
                                ->main_native_context_surface_map[context],
                            context)) {
            ALOGE("eglMakeCurrent failed");
        }
    } else {
        eglMakeCurrent(MainRenderer::get_instance()->main_native_display,
                       EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
}

void EglContext::d_eglMakeCurrent(void *context, EGLDisplay dpy,
                                  EGLSurface draw_surface_handle,
                                  EGLSurface read_surface_handle,
                                  EGLContext ctx_handle, uint64_t gbuffer_id,
                                  int width, int height, int hal_format,
                                  int64_t mc_fence) {
    auto *thread_context = (ThreadContext *)context;
    ProcessContext *process_context = thread_context->process_context.get();

    EglSurface *draw_surface =
        process_context->surface_map[draw_surface_handle];
    EglSurface *read_surface =
        process_context->surface_map[read_surface_handle];
    GLContext *opengl_context = process_context->gl_context_map[ctx_handle];

    ALOGD(
        "make current guest draw_surface_handle %llx, draw_surface %llx, "
        "read_surface_handle %llx, read_surface %llx,"
        "ctx_handle %llx, ctx %llx, gbuffer_id %llx mc_fence %lld",
        (long long)draw_surface_handle, (long long)draw_surface,
        (long long)read_surface_handle, (long long)read_surface,
        (long long)ctx_handle, (long long)opengl_context, gbuffer_id, mc_fence);

    if (opengl_context != nullptr &&
        thread_context->opengl_context != opengl_context)
        opengl_context->fence_sync(mc_fence);

    if (draw_surface != nullptr &&
        thread_context->cur_draw_surface != draw_surface &&
        draw_surface->is_current) {
        wait_event(draw_surface->reset_current_event);
    }
    if (read_surface != nullptr && read_surface != draw_surface &&
        thread_context->cur_read_surface != read_surface &&
        read_surface->is_current) {
        wait_event(read_surface->reset_current_event);
    }

    if (thread_context->make_current(draw_surface, read_surface, opengl_context,
                                     gbuffer_id)) {
        return;
    }

#ifdef ENABLE_OPENGL_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                          GL_TRUE);
#endif

    // set the viewport
    opengl_context->view_w = width;
    opengl_context->view_h = height;
    if (gbuffer_id != 0) {
        opengl_context->view_x = 0;
        opengl_context->view_y = 0;
    } else {
        opengl_context->view_x = 0;
        opengl_context->view_y = 0;
        if (draw_surface != nullptr) {
            opengl_context->view_w = draw_surface->width;
            opengl_context->view_h = draw_surface->height;
        }
    }
    ALOGD("context %llx gbuffer_id %llx makecurrent glviewport w %d h %d\n",
          (int64_t)opengl_context, gbuffer_id, opengl_context->view_w,
          opengl_context->view_h);
    glViewport(opengl_context->view_x, opengl_context->view_y,
               opengl_context->view_w, opengl_context->view_h);

    GLContext::opengl_context_init(opengl_context);
    if (draw_surface != nullptr) {
        draw_surface->init();
        // connect gbuffer with surface (create if not exist)
        draw_surface->set_gbuffer(gbuffer_id, hal_format, width, height);
    }
    if (read_surface != nullptr && read_surface != draw_surface) {
        read_surface->init();
        GraphicBuffer *gbuffer = read_surface->gbuffer;
        if (gbuffer == nullptr) {
            gbuffer = draw_surface->gbuffer;
        }
        read_surface->set_gbuffer(gbuffer->gbuffer_id, gbuffer->hal_format,
                                  gbuffer->width, gbuffer->height);
    }

    

    // set opengl_context's read and draw fbo
    GLint read_fbo = 0;
    GLint write_fbo = 0;

    if (draw_surface != nullptr && read_surface != nullptr) {
        read_fbo = (GLint)read_surface->gbuffer->data_fbo;
        
        if (draw_surface->gbuffer->sampler_num > 1) {
            write_fbo = (GLint)draw_surface->gbuffer->sampler_fbo;
        } else {
            write_fbo = (GLint)draw_surface->gbuffer->data_fbo;
        }
    }

    opengl_context->read_fbo0 = read_fbo;
    opengl_context->draw_fbo0 = write_fbo;

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, opengl_context->draw_fbo0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, opengl_context->read_fbo0);

    ALOGD("context %llx makecurrent fbo %d %d gbuffer %llx\n",
          (int64_t)opengl_context, opengl_context->draw_fbo0,
          opengl_context->read_fbo0, draw_surface->gbuffer->gbuffer_id);

    opengl_context->draw_surface = draw_surface;
}

EGLBoolean EglContext::d_eglTerminate(void *context, EGLDisplay dpy) {
    auto *thread_context = (ThreadContext *)context;
    ProcessContext *process_context = thread_context->process_context.get();
    // If display_id is EGL_DEFAULT_DISPLAY, a default display is returned.
    // Multiple calls made to eglGetDisplay with the same display_id will return
    // the same EGLDisplay handle.
    

    
    //  TODO: Callbacks for gl_context_map clear
    //     ALOGE("Callbacks for gl_context_map clear");
    //     g_hash_table_remove_all(process_context->context_map);
    //     process_context->gl_context_map.clear();
    unordered_map_clear_with_callback(
        process_context->gl_context_map,
        static_cast<std::function<void(GLContext *)>>(
            GLContext::g_context_map_destroy));
    // TODO: Callbacks for surface_map clear
    //    ALOGE("Callbacks for surface_map clear");
    //    g_hash_table_remove_all(process_context->surface_map);
    //    process_context->surface_map.clear();
    unordered_map_clear_with_callback(
        process_context->surface_map,
        static_cast<std::function<void(EglSurface *)>>(
            EglSurface::g_surface_map_destroy));
    
    
    //  g_hash_table_remove_all(process_context->native_window_surface_map);
    //  g_hash_table_remove_all(process_context->gbuffer_image_map);

    
    // thread_context->opengl_context = NULL;
    // thread_context->cur_read_surface = NULL;
    // thread_context->cur_draw_surface = NULL;

    ALOGD("eglTerminate context %llx\n",
          (uint64_t)thread_context->opengl_context);
    return GL_TRUE;
}

EGLBoolean EglContext::d_eglReleaseThread(void *context) { return EGL_TRUE; }

EGLBoolean EglContext::d_eglDestroyContext(void *context, EGLDisplay dpy,
                                           EGLContext ctx) {
    

    auto *thread_context = (ThreadContext *)context;
    ProcessContext *process_context = thread_context->process_context.get();

    
    ALOGD("context remove guest %llx\n", (uint64_t)ctx);
    //    g_hash_table_remove(process_context->context_map,
    //    GUINT_TO_POINTER(ctx));
    // TODO: Callbacks for gl_context_map deletion
    //    ALOGE("Callbacks for gl_context_map deletion");
    //    process_context->gl_context_map.erase(ctx);
    unordered_map_erase_with_callback(
        process_context->gl_context_map, ctx,
        static_cast<std::function<void(GLContext *)>>(
            GLContext::g_context_map_destroy));
    return EGL_TRUE;
}

void EglContext::d_eglTerminateThread(void *context) {
    auto *thread_context = (ThreadContext *)context;
    MainController::get_instance()->destroy_render_thread(
        static_cast<pid_t>(thread_context->process_context->remote_pid),
        static_cast<pid_t>(thread_context->remote_tid));
}
