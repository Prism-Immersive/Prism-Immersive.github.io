#define LOG_TAG "EglGPU"
#define LOG_LEVEL LOG_LEVEL_ERROR

#include "GPU.h"

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

#include <ctime>

#include "Context.h"
#include "Log.h"
#include "Surface.h"
#include "Utils.h"
#include "renderer/MainRenderer.h"
#include "renderer/RenderThread.h"

EGLBoolean EglGPU::d_eglSwapBuffers_sync(void *context, EGLDisplay dpy,
                                         EGLSurface surface,
                                         uint64_t gbuffer_id, int width,
                                         int height, int hal_format) {
    auto *thread_context = (ThreadContext *)context;
    ProcessContext *process_context = thread_context->process_context.get();

    EglSurface *real_surface = process_context->surface_map[surface];

    if (real_surface == nullptr) {
        return EGL_FALSE;
    }
    GLContext *real_opengl_context = thread_context->opengl_context;

    if (real_surface != thread_context->cur_draw_surface) {
        ALOGD(
            "error! real_surface != thread_context->cur_draw_surface "
            "%llx %llx\n",
            (uint64_t)real_surface, (uint64_t)thread_context->cur_draw_surface);
    }

    real_surface->swap_gbuffer(context, gbuffer_id, width, height, hal_format);

    if (real_surface->sampler_num > 1) {
        real_opengl_context->draw_fbo0 = real_surface->gbuffer->sampler_fbo;
    } else {
        real_opengl_context->draw_fbo0 = real_surface->gbuffer->data_fbo;
    }

    if (thread_context->cur_read_surface != nullptr) {
        real_opengl_context->read_fbo0 =
            thread_context->cur_read_surface->gbuffer->data_fbo;
        glBindFramebuffer(GL_READ_FRAMEBUFFER, real_opengl_context->read_fbo0);
    }

    return EGL_TRUE;
}

void EglGPU::d_eglQueueBuffer(void *context, uint64_t gbuffer_id,
                              int is_composer) {
    auto *thread_context = (ThreadContext *)context;
    GLContext *opengl_context = thread_context->opengl_context;

    GraphicBuffer *gbuffer =
        MainRenderer::get_instance()->get_gbuffer_from_global_map(
            (uint64_t)gbuffer_id);

    if (gbuffer_id == 0 || gbuffer == nullptr) {
        ALOGE("error! context %llx queuebuffer id %llx not exist!\n",
              (uint64_t)opengl_context, (uint64_t)gbuffer_id);
        return;
    }

    if (gbuffer->sampler_num > 1) {
        if (opengl_context != NULL && opengl_context->enable_scissor == 1) {
            glDisable(GL_SCISSOR_TEST);
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer->sampler_fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer->data_fbo);
        glBlitFramebuffer(0, 0, gbuffer->width, gbuffer->height, 0, 0,
                          gbuffer->width, gbuffer->height, GL_COLOR_BUFFER_BIT,
                          GL_NEAREST);
        if (opengl_context != NULL && opengl_context->enable_scissor == 1) {
            glEnable(GL_SCISSOR_TEST);
        }
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, opengl_context->draw_fbo0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, opengl_context->read_fbo0);
    }

    if (MainRenderer::get_instance()->static_context_values->composer_pid !=
            0 &&
        ((int)(gbuffer->gbuffer_id >> 32)) !=
            MainRenderer::get_instance()->static_context_values->composer_pid) {
        ALOGD("reverse gbuffer %llx\n", gbuffer->gbuffer_id);
        GraphicBuffer::reverse_gbuffer(gbuffer);
    }

    if (gbuffer->delete_sync != nullptr) {
        glDeleteSync(gbuffer->delete_sync);
    }

    gbuffer->delete_sync = gbuffer->data_sync;
    gbuffer->data_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    gbuffer->is_writing = 0;

    int64_t start = get_real_time();
    if (opengl_context->should_use_flush <=
        MainRenderer::COMPOSER_REFRESH_RATE) {
        glFinish();
        if (opengl_context->should_use_flush != -1 &&
            static_cast<double>(get_real_time() - start) <
                1000000. / MainRenderer::COMPOSER_REFRESH_RATE)
            opengl_context->should_use_flush++;
        else {
            opengl_context->should_use_flush = 0;
        }
    } else {
        glFlush();
        if (static_cast<double>(get_real_time() - start) > 1500)
            opengl_context->should_use_flush = -1;
    }

    MainRenderer::get_instance()->set_display_gbuffer(gbuffer);
}

EGLBoolean EglGPU::d_eglSwapBuffers(void *context, EGLDisplay dpy,
                                    EGLSurface surface,
                                    uint64_t gbuffer_id, int width, int height,
                                    int hal_format, uint64_t remote_mem) {
    auto *thread_context = (ThreadContext *)context;
    ProcessContext *process_context = thread_context->process_context.get();

    EglSurface *real_surface = process_context->surface_map[surface];
    if (real_surface == nullptr) {
        ALOGE("real surface is null!");
        ALOGD("surface %lx real %lx dpy %lx server_swap_time %lld\n", surface,
              real_surface, dpy, server_swap_time);
        return EGL_FALSE;
    }

    ALOGD("#%llx swapbuffer real_surface %llx\n",
          thread_context->opengl_context, real_surface);

    EGLBoolean ret = d_eglSwapBuffers_sync(context, dpy, surface, gbuffer_id,
                                           width, height, hal_format);

    int64_t now_time = get_real_time();

    if (ret == EGL_TRUE) {
       thread_context->remote_mem = remote_mem;
    }

    // Calculate the refresh rate of the screen
    if (now_time - real_surface->last_calc_time > 1000000 &&
        real_surface->last_calc_time != 0) {
        double hz = real_surface->now_screen_hz * 1000000.0 /
                    (now_time - real_surface->last_calc_time);
        ALOGE("%llx surface draw %.2lfHz COMPOSER_REFRESH_RATE %d\n",
              (uint64_t)real_surface, hz, MainRenderer::COMPOSER_REFRESH_RATE);
        real_surface->now_screen_hz = 0;

        real_surface->last_calc_time = now_time;
        if (hz > 132.0) {
            MainRenderer::COMPOSER_REFRESH_RATE = 144;
        } else if (hz > 105.0) {
            MainRenderer::COMPOSER_REFRESH_RATE = 120;
        } else if (hz > 82.5) {
            MainRenderer::COMPOSER_REFRESH_RATE = 90;
        } else if (hz > 67.5) {
            MainRenderer::COMPOSER_REFRESH_RATE = 75;
        } else {
            MainRenderer::COMPOSER_REFRESH_RATE = 60;
        }
    } else if (real_surface->last_calc_time == 0) {
        real_surface->last_calc_time = now_time;
        real_surface->now_screen_hz = 0;
    }

    real_surface->now_screen_hz += 1;
    return ret;
}

EGLBoolean EglGPU::d_eglSwapInterval(void *context, EGLDisplay dpy,
                                     EGLint interval) {
    return EGL_TRUE;
}

EGLBoolean EglGPU::d_eglBindTexImage(void *context, EGLDisplay dpy,
                                     EGLSurface surface, EGLint buffer) {
    return EGL_FALSE;
}

EGLBoolean EglGPU::d_eglReleaseTexImage(void *context, EGLDisplay dpy,
                                        EGLSurface surface, EGLint buffer) {
    return EGL_FALSE;
}

void EglGPU::d_eglSetGraphicBufferID(void *context, EGLSurface draw,
                                     uint64_t gbuffer_id) {}