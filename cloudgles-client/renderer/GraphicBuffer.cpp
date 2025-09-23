#include "GraphicBuffer.h"

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

#include "Constants.h"
#include "Log.h"
#include "Utils.h"
#include "egl/Context.h"
#include "egl/Surface.h"
#include "renderer/MainRenderer.h"
#include "renderer/RenderThread.h"

#define LOG_TAG "GraphicBuffer"

GraphicBuffer *GraphicBuffer::create_gbuffer_from_hal(int width, int height,
                                                      int hal_format,
                                                      EglSurface *surface,
                                                      uint64_t gbuffer_id) {
    int sampler_num = 0;
    int format = GL_RGBA;
    
    int pixel_type = GL_UNSIGNED_BYTE;
    int internal_format = GL_RGBA8;
    int depth_internal_format = 0;
    int stencil_internal_format = 0;
    if (surface != nullptr) {
        sampler_num = surface->sampler_num;
        format = surface->format;
        pixel_type = surface->pixel_type;
        internal_format = surface->internal_format;
        // row_byte_len = surface->row_byte_len;
        depth_internal_format = surface->depth_internal_format;
        stencil_internal_format = surface->stencil_internal_format;
    }
    if (hal_format == HAL_PIXEL_FORMAT_RGBA_8888 ||
        hal_format == HAL_PIXEL_FORMAT_RGBX_8888) {
        
        internal_format = GL_RGBA8;
        format = GL_RGBA;
        pixel_type = GL_UNSIGNED_BYTE;
        // real_image->pixel_type = GL_UNSIGNED_INT_8_8_8_8_REV;
        // row_byte_len = width * 4;
    }
    //    else if (hal_format == HAL_PIXEL_FORMAT_BGRA_8888)
    //    {
    //        // printf("EGLImage with g_buffer_id %llx need format
    //        BGRA_8888!!!\n", (uint64_t)g_buffer_id); internal_format =
    //        GL_RGBA8; format = GL_BGRA; pixel_type = GL_UNSIGNED_INT_8_8_8_8;
    //        // row_byte_len = width * 4;
    //    }
    else if (hal_format == HAL_PIXEL_FORMAT_RGB_888) {
        internal_format = GL_RGB8;
        format = GL_RGB;
        
        pixel_type = GL_UNSIGNED_BYTE;
        // row_byte_len = width * 3;
    } else if (hal_format == HAL_PIXEL_FORMAT_RGB_565) {
        
        internal_format = GL_RGB565;
        format = GL_RGB;
        pixel_type = GL_UNSIGNED_SHORT_5_6_5;
        // row_byte_len = width * 2;
    } else {
        internal_format = GL_RGBA8;
        format = GL_RGBA;
        pixel_type = GL_UNSIGNED_INT;
        // row_byte_len = width * 4;
        printf("error! unknown EGLImage format %d!!!\n", hal_format);
    }

    return create_gbuffer(width, height, sampler_num, format, pixel_type,
                          internal_format, depth_internal_format,
                          stencil_internal_format, gbuffer_id);
}

GraphicBuffer *GraphicBuffer::create_gbuffer(
    int width, int height, int sampler_num, int format, int pixel_type,
    int internal_format, int depth_internal_format, int stencil_internal_format,
    uint64_t gbuffer_id) {
    
    auto *gbuffer = new GraphicBuffer();

    gbuffer->writing_ok_event = create_event(0, 0);
    gbuffer->remain_life_time = MAX_WINDOW_LIFE_TIME;

    gbuffer->usage_type = GBUFFER_TYPE_WINDOW;
    gbuffer->gbuffer_id = gbuffer_id;

    GLuint pre_vbo = 0;
    GLuint pre_texture = 0;
    GLuint pre_fbo_draw = 0;
    GLuint pre_fbo_read = 0;
    GLuint pre_rbo = 0;
    GLuint pre_unpack_buffer = 0;

    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *)&pre_vbo);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&pre_texture);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint *)&pre_rbo);

    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint *)&pre_fbo_draw);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, (GLint *)&pre_fbo_read);

    glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, (GLint *)&pre_unpack_buffer);

    glGenTextures(1, &(gbuffer->data_texture));
    glGenRenderbuffers(1, &(gbuffer->rbo_depth));
    glGenRenderbuffers(1, &(gbuffer->rbo_stencil));

    if (sampler_num > 1) {
        glGenRenderbuffers(1, &(gbuffer->sampler_rbo));
    }

    glBindTexture(GL_TEXTURE_2D, gbuffer->data_texture);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifdef ENABLE_OPENGL_DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("error when creating gbuffer1 init error %x\n", error);
    }
#endif

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format,
                 pixel_type, nullptr);

#ifdef ENABLE_OPENGL_DEBUG
    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf(
            "error when creating gbuffer1 %x width %d height %d format %x "
            "pixel_type %x \n",
            error, width, height, format, pixel_type);
    }
#endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    static int max_sampler_num = -1;
    if (max_sampler_num == -1) {
        glGetInternalformativ(GL_RENDERBUFFER, GL_RGB, GL_SAMPLES, 1,
                              &max_sampler_num);
    }

    if (max_sampler_num < sampler_num) {
        ALOGD("over large sampler num %d max %d\n", sampler_num,
              max_sampler_num);
        sampler_num = max_sampler_num;
    }

    if (sampler_num > 1) {
        glBindRenderbuffer(GL_RENDERBUFFER, gbuffer->sampler_rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampler_num,
                                         internal_format, width, height);
    }

    if (depth_internal_format != 0) {
        
        glBindRenderbuffer(GL_RENDERBUFFER, gbuffer->rbo_depth);
        if (sampler_num > 1) {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampler_num,
                                             depth_internal_format, width,
                                             height);
        } else {
            glRenderbufferStorage(GL_RENDERBUFFER, depth_internal_format, width,
                                  height);
        }
    }

    
    if (stencil_internal_format != 0 &&
        depth_internal_format != GL_DEPTH24_STENCIL8) {
        glBindRenderbuffer(GL_RENDERBUFFER, gbuffer->rbo_stencil);
        if (sampler_num > 1) {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampler_num,
                                             stencil_internal_format, width,
                                             height);
        } else {
            glRenderbufferStorage(GL_RENDERBUFFER, stencil_internal_format,
                                  width, height);
        }
    }

#ifdef ENABLE_OPENGL_DEBUG
    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("error when creating gbuffer2 %x\n", error);
    }
#endif

    glBindTexture(GL_TEXTURE_2D, pre_texture);
    glBindBuffer(GL_ARRAY_BUFFER, pre_vbo);
    glBindRenderbuffer(GL_RENDERBUFFER, pre_rbo);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pre_unpack_buffer);

    gbuffer->format = format;
    gbuffer->pixel_type = pixel_type;
    gbuffer->internal_format = internal_format;
    // gbuffer->row_byte_len = row_byte_len;
    gbuffer->depth_internal_format = depth_internal_format;
    gbuffer->stencil_internal_format = stencil_internal_format;

    gbuffer->width = width;
    gbuffer->height = height;
    gbuffer->sampler_num = sampler_num;

    // printf("create gbuffer texture %d width %d height %d format %d\n",
    // gbuffer->data_texture,gbuffer->width, gbuffer->height, gbuffer->format);

    return gbuffer;
}

GraphicBuffer *GraphicBuffer::create_gbuffer_from_surface(EglSurface *surface) {
    return create_gbuffer(
        surface->width, surface->height, surface->sampler_num, surface->format,
        surface->pixel_type, surface->internal_format,
        surface->depth_internal_format, surface->stencil_internal_format, 0);
}

void GraphicBuffer::connect_gbuffer_to_surface(GraphicBuffer *gbuffer,
                                               EglSurface *surface) {
    gbuffer->data_fbo = surface->data_fbo[surface->now_fbo_loc];
    gbuffer->sampler_fbo = surface->sampler_fbo[surface->now_fbo_loc];
    surface->gbuffer = gbuffer;

    if (gbuffer->has_connected_fbo == 1 &&
        gbuffer->data_texture ==
            surface->connect_texture[surface->now_fbo_loc]) {
        if (surface->type == WINDOW_SURFACE) {
            surface->now_fbo_loc = (surface->now_fbo_loc + 1) % 3;
        }
        return;
    }
    // printf("connect surface %llx fbo %d to gbuffer %llx\n", surface,
    // surface->data_fbo[surface->now_fbo_loc], gbuffer->gbuffer_id);

    // glEnable(GL_MULTISAMPLE);

    if (surface->sampler_num > 1) {
        
        //  glfwWindowHint(GLFW_SAMPLES, d_buffer->config->samples_per_pixel);
        glEnable(GL_MULTISAMPLE);
    } else {
        glDisable(GL_MULTISAMPLE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, surface->data_fbo[surface->now_fbo_loc]);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           gbuffer->data_texture, 0);
    ALOGD(
        "glFramebufferTexture2D surface %llx fbo %d gbuffer %llx texture %d\n",
        surface, surface->data_fbo[surface->now_fbo_loc], gbuffer->gbuffer_id,
        gbuffer->data_texture);

    surface->connect_texture[surface->now_fbo_loc] = gbuffer->data_texture;
    gbuffer->has_connected_fbo = 1;

    if (surface->sampler_num > 1 && gbuffer->sampler_num > 1) {
        glBindFramebuffer(GL_FRAMEBUFFER,
                          surface->sampler_fbo[surface->now_fbo_loc]);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, gbuffer->sampler_rbo);
    }

    
    if (surface->depth_internal_format == GL_DEPTH24_STENCIL8) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, gbuffer->rbo_depth);
    } else if (surface->depth_internal_format != 0) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, gbuffer->rbo_depth);
    }

    
    if (surface->stencil_internal_format != 0 &&
        surface->depth_internal_format != GL_DEPTH24_STENCIL8) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, gbuffer->rbo_stencil);
    }
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        ALOGE("error! Framebuffer is not complete! status is %x error is %x ",
              status, glGetError());
        ALOGD(
            "foramt %x pixel_type %x internal_format %x depth_internal_format "
            "%x stencil_internal_format %x\n",
            surface->format, surface->pixel_type, surface->internal_format,
            surface->depth_internal_format, surface->stencil_internal_format);
    }

    if (surface->type == WINDOW_SURFACE) {
        surface->now_fbo_loc = (surface->now_fbo_loc + 1) % 3;
    }
}

GraphicBuffer *GraphicBuffer::create_gbuffer_with_context(int width, int height,
                                                          int hal_format,
                                                          void *t_context,
                                                          EGLContext ctx,
                                                          uint64_t gbuffer_id) {
    ThreadContext *thread_context = (ThreadContext *)t_context;
    ProcessContext *process_context = thread_context->process_context.get();

    //    GLContext *opengl_context = (GLContext
    //    *)g_hash_table_lookup(process_context->context_map,
    //    GUINT_TO_POINTER(ctx));
    GLContext *opengl_context = process_context->gl_context_map[ctx];
    if (opengl_context == NULL) {
        printf("error! create gbuffer with null context\n");
        return NULL;
    }

    if (opengl_context != thread_context->opengl_context) {
        
        printf("create gbuffer with different context!\n");
        if (opengl_context->independent_mode == 1) {
            //            glfwMakeContextCurrent((GLFWwindow
            //            *)opengl_context->window);
            ALOGE(
                "glfwMakeContextCurrent((GLFWwindow "
                "*)opengl_context->window);");
        } else {
            EglContext::native_make_current(opengl_context->native_context);
        }
    }

    GraphicBuffer *gbuffer =
        create_gbuffer_from_hal(width, height, hal_format, NULL, gbuffer_id);

    if (opengl_context != thread_context->opengl_context) {
        if (thread_context->opengl_context == NULL) {
            EglContext::native_make_current(NULL);
        } else {
            if (thread_context->opengl_context->independent_mode == 1) {
                //                glfwMakeContextCurrent((GLFWwindow *)NULL);
                ALOGE("glfwMakeContextCurrent((GLFWwindow *)NULL);");
            } else {
                EglContext::native_make_current(NULL);
            }
        }
    }

    return gbuffer;
}

void GraphicBuffer::reverse_gbuffer(GraphicBuffer *gbuffer) {
    if (gbuffer->reverse_rbo == 0) {
        GLuint pre_rbo = 0;

        glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint *)&pre_rbo);

        glGenRenderbuffers(1, &(gbuffer->reverse_rbo));
        glBindRenderbuffer(GL_RENDERBUFFER, gbuffer->reverse_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, gbuffer->internal_format,
                              gbuffer->width, gbuffer->height);

        glBindRenderbuffer(GL_RENDERBUFFER, pre_rbo);
    }

    GLuint pre_fbo_draw = 0;
    GLuint pre_fbo_read = 0;

    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint *)&pre_fbo_draw);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, (GLint *)&pre_fbo_read);

    
    GLuint temp_fbo;
    glGenFramebuffers(1, &temp_fbo);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, temp_fbo);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, gbuffer->reverse_rbo);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, gbuffer->data_fbo);

    glBlitFramebuffer(0, 0, gbuffer->width, gbuffer->height, 0, gbuffer->height,
                      gbuffer->width, 0, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer->data_fbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, temp_fbo);

    glBlitFramebuffer(0, 0, gbuffer->width, gbuffer->height, 0, 0,
                      gbuffer->width, gbuffer->height, GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);

    glDeleteFramebuffers(1, &temp_fbo);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, pre_fbo_read);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pre_fbo_draw);
}

void GraphicBuffer::destroy_gbuffer(GraphicBuffer *gbuffer) {
    // printf("destroy gbuffer %llx ptr %llx\n", gbuffer->gbuffer_id, gbuffer);
    if (gbuffer->data_texture != 0) {
        glDeleteTextures(1, &(gbuffer->data_texture));
    }
    
    // if(gbuffer->data_fbo!=0)
    // {
    //     glDeleteFramebuffers(1, &(gbuffer->data_fbo));
    // }

    // if(gbuffer->sampler_fbo!=0)
    // {
    //     glDeleteFramebuffers(1, &(gbuffer->sampler_fbo));
    // }
    if (gbuffer->sampler_rbo != 0) {
        glDeleteRenderbuffers(1, &(gbuffer->sampler_rbo));
    }
    if (gbuffer->rbo_depth != 0) {
        glDeleteRenderbuffers(1, &(gbuffer->rbo_depth));
    }
    if (gbuffer->rbo_stencil != 0) {
        glDeleteRenderbuffers(1, &(gbuffer->rbo_stencil));
    }

    if (gbuffer->data_sync != 0) {
        glDeleteSync(gbuffer->data_sync);
    }

    if (gbuffer->delete_sync != 0) {
        glDeleteSync(gbuffer->delete_sync);
    }
    delete_event(gbuffer->writing_ok_event);

    delete gbuffer;
}

void GraphicBuffer::gbuffer_map_destroy(GraphicBuffer *data) {
    auto *gbuffer = (GraphicBuffer *)data;
    if (gbuffer == nullptr) return;

    ALOGD(
        "destroy map gbuffer %llx type %d ptr %llx width %d height %d format "
        "%x type %d\n",
        gbuffer->gbuffer_id, gbuffer->usage_type, (uint64_t)gbuffer,
        gbuffer->width, gbuffer->height, gbuffer->internal_format,
        gbuffer->usage_type);

    
    if (gbuffer->usage_type == GBUFFER_TYPE_TEXTURE) {
        if (gbuffer->data_sync != NULL) {
            MainRenderer::get_instance()->send_window_message(
                MAIN_DESTROY_ONE_SYNC, gbuffer->data_sync);
            // glDeleteSync(gbuffer->data_sync);
        }
        if (gbuffer->delete_sync != NULL) {
            MainRenderer::get_instance()->send_window_message(
                MAIN_DESTROY_ONE_SYNC, gbuffer->delete_sync);
            // glDeleteSync(gbuffer->delete_sync);
        }
        MainRenderer::get_instance()->set_global_gbuffer_type(
            gbuffer->gbuffer_id, GBUFFER_TYPE_NONE);
        delete gbuffer;
    } else {
        SPIN_LOCK(gbuffer->lock);
        gbuffer->remain_life_time =
            (gbuffer->usage_type == GBUFFER_TYPE_BITMAP ? MAX_BITMAP_LIFE_TIME
                                                        : MAX_WINDOW_LIFE_TIME);
        gbuffer->is_using = 0;
        if (gbuffer->is_dying == 0) {
            gbuffer->is_dying = 1;
            MainRenderer::get_instance()->send_window_message(
                MAIN_DESTROY_GBUFFER, gbuffer);
        }
        SPIN_UNLOCK(gbuffer->lock);
    }
    // printf("send destroy gbuffer %llx message\n",gbuffer->gbuffer_id);
}

void GraphicBuffer::prepare_to_die() {
    SPIN_LOCK(lock);
    remain_life_time = MAX_WINDOW_LIFE_TIME;
    if (is_using == 0 && is_dying == 0) {
        is_dying = 1;
        MainRenderer::get_instance()->send_window_message(MAIN_DESTROY_GBUFFER,
                                                          this);
    }
    SPIN_UNLOCK(lock);
}

void GraphicBuffer::do_not_die() {
    SPIN_LOCK(lock);
    remain_life_time = MAX_WINDOW_LIFE_TIME;
    if (is_using == 0 && is_dying == 1) {
        is_dying = 0;
        MainRenderer::get_instance()->send_window_message(MAIN_CANCEL_GBUFFER,
                                                          this);
    }
    SPIN_UNLOCK(lock);
}
