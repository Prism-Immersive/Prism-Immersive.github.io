#define LOG_TAG "GLFramebuffer"
#include "Framebuffer.h"

#include <cstring>

#include "GLDef.h"
#include "Log.h"
#include "ThreadContext.h"

RenderBufferInfo *RenderBufferInfo::get_rb_info() {
    auto *thread_context = (ThreadContext *)egl_thread_context;
    auto *context = thread_context->opengl_context;
    GLuint rb_handle = context->context_binding_status.render_buffer;
    if (rb_handle == 0) {
        return nullptr;
    }

    return get_rb_info(rb_handle);
}

RenderBufferInfo *RenderBufferInfo::get_rb_info(GLuint renderbuffer) {
    auto *thread_context = (ThreadContext *)egl_thread_context;
    auto *context = thread_context->opengl_context;

    auto rb_info_iter = context->render_buffer_map->find(renderbuffer);
    RenderBufferInfo *rb_info_ptr = nullptr;
    if (rb_info_iter == context->render_buffer_map->end()) {
        auto rb_info = std::make_unique<RenderBufferInfo>();
        rb_info_ptr = rb_info.get();
        (*context->render_buffer_map)[renderbuffer] = std::move(rb_info);
    } else {
        rb_info_ptr = rb_info_iter->second.get();
    }
    return rb_info_ptr;
}

FormatBits *RenderBufferInfo::get_format_bits(GLuint renderbuffer) {
    auto *rb_info = get_rb_info(renderbuffer);
    if (rb_info == nullptr) return nullptr;
    return &rb_info->format_bits;
}

void d_glRenderbufferStorage_special(void *context, GLenum target,
                                     GLenum internalformat, GLsizei width,
                                     GLsizei height) {
    auto *rb_info = RenderBufferInfo::get_rb_info();
    if (rb_info == nullptr) return;
    rb_info->width = width;
    rb_info->height = height;
    rb_info->internalformat = internalformat;
    internal_format_to_format_bits(internalformat, GL_RGBA, GL_UNSIGNED_BYTE,
                                   &(rb_info->format_bits));
    r_glRenderbufferStorage(context, target, internalformat, width, height);
}

void d_glRenderbufferStorageMultisample_special(void *context, GLenum target,
                                                GLsizei samples,
                                                GLenum internalformat,
                                                GLsizei width, GLsizei height) {
    auto *rb_info = RenderBufferInfo::get_rb_info();
    if (rb_info == nullptr) return;
    rb_info->width = width;
    rb_info->height = height;
    rb_info->samples = samples;
    rb_info->internalformat = internalformat;
    internal_format_to_format_bits(internalformat, GL_RGBA, GL_UNSIGNED_BYTE,
                                   &(rb_info->format_bits));
    r_glRenderbufferStorageMultisample(context, target, samples, internalformat,
                                       width, height);
}

void FrameBufferInfo::attach(GLuint handle, GLenum type, GLenum target,
                             GLint level) {
    if (type != GL_DEPTH_STENCIL_ATTACHMENT)
        attachments[type_to_attachment_idx(type)] = {.handle = handle,
                                                     .type = type,
                                                     .target = target,
                                                     .level = level,
                                                     .is_attached = true};
    else {
        attachments[type_to_attachment_idx(GL_DEPTH_ATTACHMENT)] = {
            .handle = handle,
            .type = GL_DEPTH_ATTACHMENT,
            .target = target,
            .level = level,
            .is_attached = true};
        attachments[type_to_attachment_idx(GL_STENCIL_ATTACHMENT)] = {
            .handle = handle,
            .type = GL_STENCIL_ATTACHMENT,
            .target = target,
            .level = level,
            .is_attached = true};
    }
}

void FrameBufferInfo::detach(GLenum type) {
    if (type != GL_DEPTH_STENCIL_ATTACHMENT)
        attachments[type_to_attachment_idx(type)].is_attached = false;
    else {
        attachments[type_to_attachment_idx(GL_DEPTH_ATTACHMENT)].is_attached =
            false;
        attachments[type_to_attachment_idx(GL_STENCIL_ATTACHMENT)].is_attached =
            false;
    }
}

void FrameBufferInfo::get_format_bits(GLenum pname, GLint *data) {
    auto *thread_context = (ThreadContext *)egl_thread_context;
    auto *surface = thread_context->surface_draw;
    Attachment *color_attachment = nullptr;
    Attachment *depth_attachment =
        attachments + type_to_attachment_idx(GL_DEPTH_ATTACHMENT);
    depth_attachment =
        depth_attachment->is_attached ? depth_attachment : nullptr;
    Attachment *stencil_attachment =
        attachments + type_to_attachment_idx(GL_STENCIL_ATTACHMENT);
    stencil_attachment =
        stencil_attachment->is_attached ? stencil_attachment : nullptr;
    *data = 0;

    for (int i = 0; i < NUM_ATTACHMENTS; i++) {
        if (attachments[i].is_attached &&
            attachments[i].type != GL_DEPTH_ATTACHMENT &&
            attachments[i].type != GL_STENCIL_ATTACHMENT) {
            color_attachment = attachments + i;
            break;
        }
    }

    FormatBits *format_bits = nullptr;

    if (pname == GL_RED_BITS || pname == GL_GREEN_BITS ||
        pname == GL_BLUE_BITS || pname == GL_ALPHA_BITS) {
        if (color_attachment != nullptr) {
            format_bits = get_format_bits_internal(color_attachment);
        } else {
            format_bits = &default_format_bits;
        }

        switch (pname) {
            case GL_RED_BITS:
                if (format_bits == &default_format_bits && surface != nullptr) {
                    egl_default_display->get_config_attr(
                        reinterpret_cast<EGLConfig>(surface->config_id),
                        EGL_RED_SIZE, &(format_bits->red_bits));
                }
                *data = format_bits->red_bits;
                return;
            case GL_GREEN_BITS:
                if (format_bits == &default_format_bits && surface != nullptr) {
                    egl_default_display->get_config_attr(
                        reinterpret_cast<EGLConfig>(surface->config_id),
                        EGL_GREEN_SIZE, &(format_bits->green_bits));
                }
                *data = format_bits->green_bits;
                return;
            case GL_BLUE_BITS:
                if (format_bits == &default_format_bits && surface != nullptr) {
                    egl_default_display->get_config_attr(
                        reinterpret_cast<EGLConfig>(surface->config_id),
                        EGL_BLUE_SIZE, &(format_bits->blue_bits));
                }
                *data = format_bits->blue_bits;
                return;
            case GL_ALPHA_BITS:
                if (format_bits == &default_format_bits && surface != nullptr) {
                    egl_default_display->get_config_attr(
                        reinterpret_cast<EGLConfig>(surface->config_id),
                        EGL_ALPHA_SIZE, &(format_bits->alpha_bits));
                }
                *data = format_bits->alpha_bits;
                return;
            default:
                return;
        }
    }

    if (pname == GL_DEPTH_BITS) {
        if (depth_attachment != nullptr) {
            format_bits = get_format_bits_internal(depth_attachment);
        } else {
            format_bits = &default_format_bits;
        }

        if (format_bits == &default_format_bits && surface != nullptr) {
            egl_default_display->get_config_attr(
                reinterpret_cast<EGLConfig>(surface->config_id), EGL_DEPTH_SIZE,
                &(format_bits->depth_bits));
        }
        *data = format_bits->depth_bits;
        return;
    }

    if (pname == GL_STENCIL_BITS) {
        if (stencil_attachment != nullptr) {
            format_bits = get_format_bits_internal(stencil_attachment);
        } else {
            format_bits = &default_format_bits;
        }

        if (format_bits == &default_format_bits && surface != nullptr) {
            egl_default_display->get_config_attr(
                reinterpret_cast<EGLConfig>(surface->config_id),
                EGL_STENCIL_SIZE, &(format_bits->stencil_bits));
        }
        *data = format_bits->stencil_bits;
        return;
    }
}

FormatBits *FrameBufferInfo::get_format_bits_internal(Attachment *attachment) {
    FormatBits *format_bits = nullptr;
    if (attachment->target == GL_RENDERBUFFER) {
        format_bits = RenderBufferInfo::get_format_bits(attachment->handle);
    } else {
        format_bits = TextureInfo::get_format_bits(attachment->handle,
                                                    attachment->target);
    }

    if (format_bits == nullptr) {
        format_bits = &default_format_bits;
    }

    return format_bits;
}

FrameBufferInfo *FrameBufferInfo::get_fb_info(GLenum target) {
    auto *thread_context = (ThreadContext *)egl_thread_context;
    auto *opengl_context = thread_context->opengl_context;
    GLuint fb_handle = 0;
    if (target == GL_READ_FRAMEBUFFER) {
        fb_handle = opengl_context->context_binding_status.read_frame_buffer;
    } else {
        fb_handle = opengl_context->context_binding_status.draw_frame_buffer;
    }

    auto fb_info_iter = opengl_context->frame_buffer_map->find(fb_handle);
    FrameBufferInfo *fb_info_ptr = nullptr;
    if (fb_info_iter == opengl_context->frame_buffer_map->end()) {
        auto fb_info = std::make_unique<FrameBufferInfo>();
        fb_info_ptr = fb_info.get();
        (*opengl_context->frame_buffer_map)[fb_handle] = std::move(fb_info);
    } else {
        fb_info_ptr = fb_info_iter->second.get();
    }
    return fb_info_ptr;
}

void FrameBufferInfo::detach_texture(GLuint texture) {
    auto *fb_write_info = FrameBufferInfo::get_fb_info(GL_FRAMEBUFFER);
    auto *fb_read_info = FrameBufferInfo::get_fb_info(GL_READ_FRAMEBUFFER);

    if (fb_write_info != nullptr) {
        for (int i = 0; i < NUM_ATTACHMENTS; i++) {
            if (fb_write_info->attachments[i].handle == texture) {
                fb_write_info->detach(fb_write_info->attachments[i].type);
            }
        }
    }

    if (fb_read_info != nullptr && fb_read_info != fb_write_info) {
        for (int i = 0; i < NUM_ATTACHMENTS; i++) {
            if (fb_read_info->attachments[i].handle == texture) {
                fb_read_info->detach(fb_read_info->attachments[i].type);
            }
        }
    }
}

void FrameBufferInfo::detach_renderbuffer(GLuint renderbuffer) {
    auto *fb_write_info = FrameBufferInfo::get_fb_info(GL_FRAMEBUFFER);
    auto *fb_read_info = FrameBufferInfo::get_fb_info(GL_READ_FRAMEBUFFER);

    if (fb_write_info != nullptr) {
        for (int i = 0; i < NUM_ATTACHMENTS; i++) {
            if (fb_write_info->attachments[i].handle == renderbuffer) {
                fb_write_info->detach(fb_write_info->attachments[i].type);
            }
        }
    }

    if (fb_read_info != nullptr && fb_read_info != fb_write_info) {
        for (int i = 0; i < NUM_ATTACHMENTS; i++) {
            if (fb_read_info->attachments[i].handle == renderbuffer) {
                fb_read_info->detach(fb_read_info->attachments[i].type);
            }
        }
    }
}

void d_glFramebufferTexture2D_special(void *context, GLenum target,
                                      GLenum attachment, GLenum textarget,
                                      GLuint texture, GLint level) {
    auto *opengl_context = (GLThreadContext *)context;

    auto *fb_info = FrameBufferInfo::get_fb_info(target);

    if (texture != 0)
        fb_info->attach(texture, attachment, textarget, level);
    else
        fb_info->detach(attachment);

    ResourceContext *res_ctx = &(opengl_context->resource_context);
    if (texture != 0 && res_ctx->texture_has_gbuffer()->has_id(texture)) {
        EGLImageInfo *image_info =
            opengl_context->texture_gbuffer_map[0][texture];
        if (image_info == NULL) {
            ALOGE("error! get null image_info whth texture %d", texture);
        }

        global_gbuffer_types[image_info->gbuffer_id] = GBUFFER_TYPE_FBO;
        image_info->gbuffer_type = GBUFFER_TYPE_FBO;
        image_info->need_data_back = 1;
    }

    r_glFramebufferTexture2D(context, target, attachment, textarget, texture,
                             level);
    return;
}

void d_glFramebufferTextureLayer_special(void *context, GLenum target,
                                         GLenum attachment, GLuint texture,
                                         GLint level, GLint layer) {
    auto *opengl_context = (GLThreadContext *)context;

    auto *fb_info = FrameBufferInfo::get_fb_info(target);

    if (texture != 0)
        fb_info->attach(texture, attachment, GL_TEXTURE_2D_ARRAY, level);
    else
        fb_info->detach(attachment);

    r_glFramebufferTextureLayer(context, target, attachment, texture, level,
                                layer);
}

void d_glFramebufferRenderbuffer_special(void *context, GLenum target,
                                         GLenum attachment,
                                         GLenum renderbuffertarget,
                                         GLuint renderbuffer) {
    auto *opengl_context = (GLThreadContext *)context;

    auto *fb_info = FrameBufferInfo::get_fb_info(target);

    if (renderbuffer != 0)
        fb_info->attach(renderbuffer, attachment, renderbuffertarget);
    else
        fb_info->detach(attachment);

    r_glFramebufferRenderbuffer(context, target, attachment, renderbuffertarget,
                                renderbuffer);
}
