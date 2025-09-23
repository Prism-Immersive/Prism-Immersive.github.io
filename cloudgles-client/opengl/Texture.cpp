#include "Texture.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

#include "Context.h"
#include "Log.h"
#include "Resource.h"
#include "egl/Context.h"
#include "renderer/MainRenderer.h"
#include "renderer/RenderThread.h"

#define LOG_TAG "Texture"

void prepare_unpack_texture(void *context, void *guest_mem, int start_loc,
                            int end_loc) {
    GLContext *opengl_context = (GLContext *)context;
    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);
    BufferStatus *status = &(bound_buffer->buffer_status);
    GLint async_texture = bound_buffer->async_unpack_texture_buffer;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, async_texture);
    status->host_pixel_unpack_buffer = async_texture;

    
    glBufferData(GL_PIXEL_UNPACK_BUFFER, end_loc, NULL, GL_STREAM_DRAW);

    GLubyte *map_pointer = (GLubyte *)glMapBufferRange(
        GL_PIXEL_UNPACK_BUFFER, start_loc, end_loc - start_loc,
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    memcpy(map_pointer, (char *)guest_mem + start_loc, end_loc - start_loc);

    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    ALOGD("unpack texture start %d end %d\n", start_loc, end_loc);
}

void Texture::d_glTexImage2D_without_bound(void *context, GLenum target,
                                           GLint level, GLint internalformat,
                                           GLsizei width, GLsizei height,
                                           GLint border, GLenum format,
                                           GLenum type, GLint buf_len,
                                           const void *pixels) {
    void *guest_mem = (void *)pixels;

    GLContext *opengl_context = (GLContext *)context;
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);

    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    if (guest_mem == nullptr) {
        if (status->host_pixel_unpack_buffer != 0) {
            status->host_pixel_unpack_buffer = 0;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        glTexImage2D(target, level, internalformat, width, height, border,
                     format, type, NULL);
        return;
    }

    int start_loc = 0, end_loc = buf_len;

    prepare_unpack_texture(context, guest_mem, start_loc, end_loc);

    glTexImage2D(target, level, internalformat, width, height, border, format,
                 type, NULL);
}

void Texture::d_glTexImage2D_with_bound(void *context, GLenum target,
                                        GLint level, GLint internalformat,
                                        GLsizei width, GLsizei height,
                                        GLint border, GLenum format,
                                        GLenum type, GLintptr pixels) {
    GLState::buffer_binding_status_sync(context, GL_PIXEL_UNPACK_BUFFER);
    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);

    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    glTexImage2D(target, level, internalformat, width, height, border, format,
                 type, (void *)pixels);
}

void Texture::d_glTexSubImage2D_without_bound(void *context, GLenum target,
                                              GLint level, GLint xoffset,
                                              GLint yoffset, GLsizei width,
                                              GLsizei height, GLenum format,
                                              GLenum type, GLint buf_len,
                                              const void *pixels) {
    void *guest_mem = (void *)pixels;

    GLContext *opengl_context = (GLContext *)context;
    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);

    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    TextureBindingStatus *texture_status =
        &(opengl_context->texture_binding_status);
    BufferStatus *buffer_status =
        &(opengl_context->bound_buffer_status.buffer_status);

    if (guest_mem == nullptr) {
        if (buffer_status->host_pixel_unpack_buffer != 0) {
            buffer_status->host_pixel_unpack_buffer = 0;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }
        ALOGD(
            "error! glTexSubImage2D get NULL data! target %x level %d xoffset "
            "%d yoffset %d width %d height %d format %x type %x buf_len %d",
            target, level, xoffset, yoffset, (int)width, (int)height, format,
            type, buf_len);

        glTexSubImage2D(target, level, xoffset, yoffset, width, height, format,
                        type, NULL);
        return;
    }

    // Pixel_Store_Status *status=&(((GLContext
    // *)context)->pixel_store_status);

    int start_loc = 0, end_loc = buf_len;
    // gl_pixel_data_loc(status,width,height,format,type,0,&start_loc,&end_loc);

    prepare_unpack_texture(context, guest_mem, start_loc, end_loc);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        if (target == GL_TEXTURE_CUBE_MAP_POSITIVE_X ||
        //            target == GL_TEXTURE_CUBE_MAP_POSITIVE_Y ||
        //            target == GL_TEXTURE_CUBE_MAP_POSITIVE_Z ||
        //            target == GL_TEXTURE_CUBE_MAP_NEGATIVE_X ||
        //            target == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ||
        //            target == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
        //        {
        //            texture_binding_status_sync(context, target);
        //            glTexSubImage2D(target, level, xoffset, yoffset, width,
        //            height, format, type, 0);
        //        }
        //        else
        //        {
        //            glTextureSubImage2D(bind_texture, level, xoffset, yoffset,
        //            width, height, format, type, 0);
        //        }

        ALOGE("DSA not supported.");
    } else {
        if (target == GL_TEXTURE_EXTERNAL_OES) {
            if (texture_status->host_current_active_texture != 0) {
                glActiveTexture(GL_TEXTURE0);
            }
            glBindTexture(GL_TEXTURE_2D,
                          texture_status->current_texture_external);
            // printf("%s external texture
            // %u\n",__FUNCTION__,opengl_context->current_texture_external);
            glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width,
                            height, format, type, 0);
            glBindTexture(GL_TEXTURE_2D,
                          texture_status->host_current_texture_2D[0]);
            if (texture_status->host_current_active_texture != 0) {
                glActiveTexture(texture_status->host_current_active_texture +
                                GL_TEXTURE0);
            }
        } else {
            glTexSubImage2D(target, level, xoffset, yoffset, width, height,
                            format, type, 0);
        }

        // glBindBuffer(GL_PIXEL_UNPACK_BUFFER,
        // buffer_status->host_pixel_unpack_buffer);
    }

    GraphicBuffer *gbuffer = NULL;
    if (target == GL_TEXTURE_2D) {
        gbuffer = texture_status->current_2D_gbuffer;
    } else if (target == GL_TEXTURE_EXTERNAL_OES) {
        gbuffer = texture_status->current_external_gbuffer;
    }
    if (gbuffer != NULL && gbuffer->usage_type == GBUFFER_TYPE_NATIVE) {
        MainRenderer::get_instance()->set_global_gbuffer_type(
            gbuffer->gbuffer_id, GBUFFER_TYPE_BITMAP);
        gbuffer->usage_type = GBUFFER_TYPE_BITMAP;
        gbuffer->remain_life_time = MAX_BITMAP_LIFE_TIME;
    }
}

void Texture::d_glTexSubImage2D_with_bound(void *context, GLenum target,
                                           GLint level, GLint xoffset,
                                           GLint yoffset, GLsizei width,
                                           GLsizei height, GLenum format,
                                           GLenum type, GLintptr pixels) {
    GLContext *opengl_context = (GLContext *)context;

    TextureBindingStatus *texture_status =
        &(opengl_context->texture_binding_status);
    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);

    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    GLState::buffer_binding_status_sync(context, GL_PIXEL_UNPACK_BUFFER);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glTextureSubImage2D(bind_texture, level, xoffset, yoffset,
        //        width, height, format, type, (void *)pixels);
        ALOGE("DSA not supported.");
    } else {
        if (target == GL_TEXTURE_EXTERNAL_OES) {
            if (texture_status->host_current_active_texture != 0) {
                glActiveTexture(GL_TEXTURE0);
            }
            glBindTexture(GL_TEXTURE_2D,
                          texture_status->current_texture_external);
            // printf("%s external texture
            // %u\n",__FUNCTION__,opengl_context->current_texture_external);
            glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width,
                            height, format, type, (void *)pixels);
            glBindTexture(GL_TEXTURE_2D,
                          texture_status->host_current_texture_2D[0]);
            if (texture_status->host_current_active_texture != 0) {
                glActiveTexture(texture_status->host_current_active_texture +
                                GL_TEXTURE0);
            }
        } else {
            glTexSubImage2D(target, level, xoffset, yoffset, width, height,
                            format, type, (void *)pixels);
        }
    }

    GraphicBuffer *gbuffer = NULL;
    if (target == GL_TEXTURE_2D) {
        gbuffer = texture_status->current_2D_gbuffer;
    } else if (target == GL_TEXTURE_EXTERNAL_OES) {
        gbuffer = texture_status->current_external_gbuffer;
    }
    if (gbuffer != NULL && gbuffer->usage_type == GBUFFER_TYPE_NATIVE) {
        MainRenderer::get_instance()->set_global_gbuffer_type(
            gbuffer->gbuffer_id, GBUFFER_TYPE_BITMAP);
        gbuffer->usage_type = GBUFFER_TYPE_BITMAP;
    }
}

void Texture::d_glTexImage3D_without_bound(void *context, GLenum target,
                                           GLint level, GLint internalformat,
                                           GLsizei width, GLsizei height,
                                           GLsizei depth, GLint border,
                                           GLenum format, GLenum type,
                                           GLint buf_len, const void *pixels) {
    void *guest_mem = (void *)pixels;

    GLContext *opengl_context = (GLContext *)context;

    // if(DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    // {
    //     texture_binding_status_sync(context, target);
    // }
    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);

    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    //    if (guest_mem->all_len == 0)
    if (guest_mem == nullptr) {
        // pixels=NULL
        BufferStatus *buffer_status =
            &(opengl_context->bound_buffer_status.buffer_status);

        if (buffer_status->host_pixel_unpack_buffer != 0) {
            buffer_status->host_pixel_unpack_buffer = 0;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                       MainRenderer::DSA_ENABLE != 0)) {
            //            glTextureImage3DEXT(bind_texture, target, level,
            //            internalformat, width, height, depth, border, format,
            //            type, NULL);
            ALOGE("DSA not supported.");
        } else {
            glTexImage3D(target, level, internalformat, width, height, depth,
                         border, format, type, NULL);
        }
        return;
    }

    int start_loc = 0, end_loc = buf_len;

    ALOGD("pixel start loc %d end loc %d\n", start_loc, end_loc);

    prepare_unpack_texture(context, guest_mem, start_loc, end_loc);

    
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glTextureImage3DEXT(bind_texture, target, level,
        //        internalformat, width, height, depth, border, format, type,
        //        0);
        ALOGE("DSA not supported.");
    } else {
        glTexImage3D(target, level, internalformat, width, height, depth,
                     border, format, type, 0);
    }

    // GLContext *opengl_context = (GLContext *)context;
    // BufferStatus *status =
    // &(opengl_context->bound_buffer_status.buffer_status);

    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, status->host_pixel_unpack_buffer);
}

void Texture::d_glTexImage3D_with_bound(void *context, GLenum target,
                                        GLint level, GLint internalformat,
                                        GLsizei width, GLsizei height,
                                        GLsizei depth, GLint border,
                                        GLenum format, GLenum type,
                                        GLintptr pixels) {
    GLState::buffer_binding_status_sync(context, GL_PIXEL_UNPACK_BUFFER);
    // if(DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    // {
    //     texture_binding_status_sync(context, target);
    // }
    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);
    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glTextureImage3DEXT(bind_texture, target, level,
        //        internalformat, width, height, depth, border, format, type,
        //        (void *)pixels);
        ALOGE("DSA not supported.");
    } else {
        glTexImage3D(target, level, internalformat, width, height, depth,
                     border, format, type, (void *)pixels);
    }
}

void Texture::d_glTexSubImage3D_without_bound(
    void *context, GLenum target, GLint level, GLint xoffset, GLint yoffset,
    GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format,
    GLenum type, GLint buf_len, const void *pixels) {
    void *guest_mem = (void *)pixels;

    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);
    GLContext *opengl_context = (GLContext *)context;
    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    //    if (guest_mem->all_len == 0)
    if (guest_mem == nullptr) {
        // pixels=NULL
        BufferStatus *buffer_status =
            &(opengl_context->bound_buffer_status.buffer_status);
        if (buffer_status->host_pixel_unpack_buffer != 0) {
            buffer_status->host_pixel_unpack_buffer = 0;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                       MainRenderer::DSA_ENABLE != 0)) {
            //            glTextureSubImage3D(bind_texture, level, xoffset,
            //            yoffset, zoffset, width, height, depth, format, type,
            //            NULL);
            ALOGE("DSA not supported.");
        } else {
            glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width,
                            height, depth, format, type, NULL);
        }
        return;
    }

    int start_loc = 0, end_loc = buf_len;

    prepare_unpack_texture(context, guest_mem, start_loc, end_loc);

    
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glTextureSubImage3D(bind_texture, level, xoffset, yoffset,
        //        zoffset, width, height, depth, format, type, 0);
        ALOGE("DSA not supported.");
    } else {
        glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height,
                        depth, format, type, 0);
    }
    // GLContext *opengl_context = (GLContext *)context;
    // BufferStatus *status =
    // &(opengl_context->bound_buffer_status.buffer_status);

    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, status->host_pixel_unpack_buffer);
}

void Texture::d_glTexSubImage3D_with_bound(void *context, GLenum target,
                                           GLint level, GLint xoffset,
                                           GLint yoffset, GLint zoffset,
                                           GLsizei width, GLsizei height,
                                           GLsizei depth, GLenum format,
                                           GLenum type, GLintptr pixels) {
    GLState::buffer_binding_status_sync(context, GL_PIXEL_UNPACK_BUFFER);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLuint bind_texture =
        //        GLState::get_guest_binding_texture(context, target); if
        //        (bind_texture == 0)
        //        {
        //            ALOGD("error! %s with texture 0 target %llx\n",
        //            __FUNCTION__, (uint64_t)context); return;
        //        }
        //        glTextureSubImage3D(bind_texture, level, xoffset, yoffset,
        //        zoffset, width, height, depth, format, type, (void *)pixels);
        ALOGE("DSA not supported.");
    } else {
        glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height,
                        depth, format, type, (void *)pixels);
    }
}

void Texture::d_glCompressedTexImage3D_without_bound(
    void *context, GLenum target, GLint level, GLenum internalformat,
    GLsizei width, GLsizei height, GLsizei depth, GLint border,
    GLsizei imageSize, const void *data) {
    void *guest_mem = (void *)data;

    // printf("compress texture format %x\n",internalformat);
    GLContext *opengl_context = (GLContext *)context;
    // if(DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    // {
    //     texture_binding_status_sync(context, target);
    // }

    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);
    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    //    if (guest_mem->all_len == 0)
    if (guest_mem == nullptr) {
        BufferStatus *buffer_status =
            &(opengl_context->bound_buffer_status.buffer_status);
        // pixels=NULL
        if (buffer_status->host_pixel_unpack_buffer != 0) {
            buffer_status->host_pixel_unpack_buffer = 0;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                       MainRenderer::DSA_ENABLE != 0)) {
            //            glCompressedTextureImage3DEXT(bind_texture, target,
            //            level, internalformat, width, height, depth, border,
            //            imageSize, NULL);
            ALOGE("DSA not supported.");
        } else {
            glCompressedTexImage3D(target, level, internalformat, width, height,
                                   depth, border, imageSize, NULL);
        }
        return;
    }

    // Pixel_Store_Status *status=&(((GLContext
    // *)context)->pixel_store_status);
    prepare_unpack_texture(context, guest_mem, 0, imageSize);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glCompressedTextureImage3DEXT(bind_texture, target, level,
        //        internalformat, width, height, depth, border, imageSize, 0);
        ALOGE("DSA not supported.");
    } else {
        glCompressedTexImage3D(target, level, internalformat, width, height,
                               depth, border, imageSize, 0);
    }

    // GLContext *opengl_context = (GLContext *)context;
    // BufferStatus *status =
    // &(opengl_context->bound_buffer_status.buffer_status);

    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, status->host_pixel_unpack_buffer);
}

void Texture::d_glCompressedTexImage3D_with_bound(
    void *context, GLenum target, GLint level, GLenum internalformat,
    GLsizei width, GLsizei height, GLsizei depth, GLint border,
    GLsizei imageSize, GLintptr data) {
    // printf("compress texture format %x\n",internalformat);
    GLState::buffer_binding_status_sync(context, GL_PIXEL_UNPACK_BUFFER);
    // if(DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    // {
    //     texture_binding_status_sync(context, target);
    // }

    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);
    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glCompressedTextureImage3DEXT(bind_texture, target, level,
        //        internalformat, width, height, depth, border, imageSize, (void
        //        *)data);
        ALOGE("DSA not supported.");
    } else {
        glCompressedTexImage3D(target, level, internalformat, width, height,
                               depth, border, imageSize, (void *)data);
    }
}

void Texture::d_glCompressedTexSubImage3D_without_bound(
    void *context, GLenum target, GLint level, GLint xoffset, GLint yoffset,
    GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format,
    GLsizei imageSize, const void *data) {
    void *guest_mem = (void *)data;

    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);
    GLContext *opengl_context = (GLContext *)context;
    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    //    if (guest_mem->all_len == 0)
    if (guest_mem == nullptr) {
        BufferStatus *buffer_status =
            &(opengl_context->bound_buffer_status.buffer_status);
        // pixels=NULL
        if (buffer_status->host_pixel_unpack_buffer != 0) {
            buffer_status->host_pixel_unpack_buffer = 0;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }
        if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                       MainRenderer::DSA_ENABLE != 0)) {
            //            glCompressedTextureSubImage3D(bind_texture, level,
            //            xoffset, yoffset, zoffset, width, height, depth,
            //            format, imageSize, NULL);
            ALOGE("DSA not supported.");
        } else {
            glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset,
                                      width, height, depth, format, imageSize,
                                      NULL);
        }
        return;
    }

    // Pixel_Store_Status *status=&(((GLContext
    // *)context)->pixel_store_status);
    prepare_unpack_texture(context, guest_mem, 0, imageSize);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glCompressedTextureSubImage3D(bind_texture, level, xoffset,
        //        yoffset, zoffset, width, height, depth, format, imageSize, 0);
        ALOGE("DSA not supported.");
    } else {
        glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset,
                                  width, height, depth, format, imageSize, 0);
    }

    // GLContext *opengl_context = (GLContext *)context;
    // BufferStatus *status =
    // &(opengl_context->bound_buffer_status.buffer_status);

    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, status->host_pixel_unpack_buffer);
}

void Texture::d_glCompressedTexSubImage3D_with_bound(
    void *context, GLenum target, GLint level, GLint xoffset, GLint yoffset,
    GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format,
    GLsizei imageSize, GLintptr data) {
    GLState::buffer_binding_status_sync(context, GL_PIXEL_UNPACK_BUFFER);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLuint bind_texture =
        //        GLState::get_guest_binding_texture(context, target); if
        //        (bind_texture == 0)
        //        {
        //            ALOGD("error! %s with texture 0 target %llx\n",
        //            __FUNCTION__, (uint64_t)context); return;
        //        }
        //        glCompressedTextureSubImage3D(bind_texture, level, xoffset,
        //        yoffset, zoffset, width, height, depth, format, imageSize,
        //        (void *)data);
        ALOGE("DSA not supported.");
    } else {
        glCompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset,
                                  width, height, depth, format, imageSize,
                                  (void *)data);
    }
}

void Texture::d_glCompressedTexImage2D_without_bound(
    void *context, GLenum target, GLint level, GLenum internalformat,
    GLsizei width, GLsizei height, GLint border, GLsizei imageSize,
    const void *data) {
    void *guest_mem = (void *)data;

    // printf("compress texture format %x\n",internalformat);
    GLContext *opengl_context = (GLContext *)context;

    // if(DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    // {
    //     texture_binding_status_sync(context, target);
    // }

    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);
    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    //    if (guest_mem->all_len == 0)
    if (guest_mem == nullptr) {
        BufferStatus *buffer_status =
            &(opengl_context->bound_buffer_status.buffer_status);
        // pixels=NULL
        if (buffer_status->host_pixel_unpack_buffer != 0) {
            buffer_status->host_pixel_unpack_buffer = 0;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                       MainRenderer::DSA_ENABLE != 0)) {
            //            glCompressedTextureImage2DEXT(bind_texture, target,
            //            level, internalformat, width, height, border,
            //            imageSize, NULL);
            ALOGE("DSA not supported.");
        } else {
            glCompressedTexImage2D(target, level, internalformat, width, height,
                                   border, imageSize, NULL);
        }

        return;
    }
    // Pixel_Store_Status *status=&(((GLContext
    // *)context)->pixel_store_status);
    prepare_unpack_texture(context, guest_mem, 0, imageSize);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glCompressedTextureImage2DEXT(bind_texture, target, level,
        //        internalformat, width, height, border, imageSize, 0);
        ALOGE("DSA not supported.");
    } else {
        glCompressedTexImage2D(target, level, internalformat, width, height,
                               border, imageSize, 0);
    }

    // GLContext *opengl_context = (GLContext *)context;
    // BufferStatus *status =
    // &(opengl_context->bound_buffer_status.buffer_status);

    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, status->host_pixel_unpack_buffer);
}

void Texture::d_glCompressedTexImage2D_with_bound(
    void *context, GLenum target, GLint level, GLenum internalformat,
    GLsizei width, GLsizei height, GLint border, GLsizei imageSize,
    GLintptr data) {
    // printf("compress texture format %x\n",internalformat);
    GLState::buffer_binding_status_sync(context, GL_PIXEL_UNPACK_BUFFER);
    // if(DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    // {
    //     texture_binding_status_sync(context, target);
    // }

    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);
    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glCompressedTextureImage2DEXT(bind_texture, target, level,
        //        internalformat, width, height, border, imageSize, (void
        //        *)data);
        ALOGE("DSA not supported.");
    } else {
        glCompressedTexImage2D(target, level, internalformat, width, height,
                               border, imageSize, (void *)data);
    }
}

void Texture::d_glCompressedTexSubImage2D_without_bound(
    void *context, GLenum target, GLint level, GLint xoffset, GLint yoffset,
    GLsizei width, GLsizei height, GLenum format, GLsizei imageSize,
    const void *data) {
    void *guest_mem = (void *)data;

    GLuint bind_texture = GLState::get_guest_binding_texture(context, target);
    GLContext *opengl_context = (GLContext *)context;
    if (bind_texture == 0) {
        ALOGD("error! %s with texture 0 target %llx\n", __FUNCTION__,
              (uint64_t)context);
        return;
    }

    //    if (guest_mem->all_len == 0)
    if (guest_mem == nullptr) {
        // pixels=NULL
        BufferStatus *buffer_status =
            &(opengl_context->bound_buffer_status.buffer_status);
        if (buffer_status->host_pixel_unpack_buffer != 0) {
            buffer_status->host_pixel_unpack_buffer = 0;
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }
        if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                       MainRenderer::DSA_ENABLE != 0)) {
            //            glCompressedTextureSubImage2D(bind_texture, level,
            //            xoffset, yoffset, width, height, format, imageSize,
            //            NULL);
            ALOGE("DSA not supported.");
        } else {
            glCompressedTexSubImage2D(target, level, xoffset, yoffset, width,
                                      height, format, imageSize, NULL);
        }
        return;
    }

    prepare_unpack_texture(context, guest_mem, 0, imageSize);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        if (target == GL_TEXTURE_CUBE_MAP_POSITIVE_X ||
        //            target == GL_TEXTURE_CUBE_MAP_POSITIVE_Y ||
        //            target == GL_TEXTURE_CUBE_MAP_POSITIVE_Z ||
        //            target == GL_TEXTURE_CUBE_MAP_NEGATIVE_X ||
        //            target == GL_TEXTURE_CUBE_MAP_NEGATIVE_Y ||
        //            target == GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
        //        {
        //            texture_binding_status_sync(context, target);
        //            glCompressedTexSubImage2D(target, level, xoffset, yoffset,
        //            width, height, format, imageSize, 0);
        //        }
        //        else
        //        {
        //            glCompressedTextureSubImage2D(bind_texture, level,
        //            xoffset, yoffset, width, height, format, imageSize, 0);
        //        }
        ALOGE("DSA not supported.");
    } else {
        glCompressedTexSubImage2D(target, level, xoffset, yoffset, width,
                                  height, format, imageSize, 0);
    }

    // GLContext *opengl_context = (GLContext *)context;
    // BufferStatus *status =
    // &(opengl_context->bound_buffer_status.buffer_status);

    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, status->host_pixel_unpack_buffer);
}

void Texture::d_glCompressedTexSubImage2D_with_bound(
    void *context, GLenum target, GLint level, GLint xoffset, GLint yoffset,
    GLsizei width, GLsizei height, GLenum format, GLsizei imageSize,
    GLintptr data) {
    GLState::buffer_binding_status_sync(context, GL_PIXEL_UNPACK_BUFFER);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLuint bind_texture =
        //        GLState::get_guest_binding_texture(context, target); if
        //        (bind_texture == 0)
        //        {
        //            ALOGD("error! %s with texture 0 target %llx\n",
        //            __FUNCTION__, (uint64_t)context); return;
        //        }
        //
        //        glCompressedTextureSubImage2D(bind_texture, level, xoffset,
        //        yoffset, width, height, format, imageSize, (void *)data);
        ALOGE("DSA not supported.");
    } else {
        glCompressedTexSubImage2D(target, level, xoffset, yoffset, width,
                                  height, format, imageSize, (void *)data);
    }
}

void Texture::d_glReadPixels_without_bound(void *context, GLint x, GLint y,
                                           GLsizei width, GLsizei height,
                                           GLenum format, GLenum type,
                                           GLint buf_len, void *pixels) {
    void *guest_mem = (void *)pixels;

    //    if (unlikely(guest_mem->all_len == 0))
    if (unlikely(guest_mem == nullptr)) {
        // pixels=NULL
        //  glReadPixels(x, y, width, height, format, type, NULL);
        return;
    }

    int start_loc = 0, end_loc = buf_len;

    GLContext *opengl_context = (GLContext *)context;

    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);

    BufferStatus *status = &(bound_buffer->buffer_status);

    GLint asyn_texture = bound_buffer->async_pack_texture_buffer;
    glBindBuffer(GL_PIXEL_PACK_BUFFER, asyn_texture);
    status->host_pixel_pack_buffer = asyn_texture;

    
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glNamedBufferData(asyn_texture, end_loc, NULL,
        //        GL_STREAM_READ);
        ALOGE("DSA not supported.");
    } else {
        glBufferData(GL_PIXEL_PACK_BUFFER, end_loc, NULL, GL_STREAM_READ);
    }
    glReadPixels(x, y, width, height, format, type, 0);

    
    GLubyte *map_pointer = NULL;
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        map_pointer = glMapNamedBufferRange(asyn_texture, start_loc,
        //        end_loc - start_loc, GL_MAP_READ_BIT);
        ALOGE("DSA not supported.");
    } else {
        map_pointer =
            (GLubyte *)glMapBufferRange(GL_PIXEL_PACK_BUFFER, start_loc,
                                        end_loc - start_loc, GL_MAP_READ_BIT);
    }

    // char print_chars[1000];
    // int print_loc = 0;
    // for(int i=0;i<end_loc-start_loc && i<100;i++)
    // {
    //     print_loc += sprintf(print_chars+print_loc,
    //     "%.2x",(int)map_pointer[i]);
    // }
    // printf("glreadpixels %s\n",print_chars);
    // TODO: guest_read
    //    guest_read(guest_mem, map_pointer, 0, end_loc - start_loc);
    memcpy(guest_mem, map_pointer, end_loc - start_loc);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glUnmapNamedBuffer(asyn_texture);
        ALOGE("DSA not supported.");
    } else {
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    }

    // status->host_pixel_pack_buffer = asyn_texture;

    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, status->host_pixel_pack_buffer);
}

void Texture::d_glReadPixels_with_bound(void *context, GLint x, GLint y,
                                        GLsizei width, GLsizei height,
                                        GLenum format, GLenum type,
                                        GLintptr pixels) {
    GLState::buffer_binding_status_sync(context, GL_PIXEL_PACK_BUFFER);
    // GLuint pack = 0;
    // glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &pack);
    // printf("glReadPixels x %d y %d width %d height %d format %x type %x
    // pixels %llx pack %d\n", x, y, (int)width, (int)height, format, type,
    // pixels, pack);
    glReadPixels(x, y, width, height, format, type, (void *)pixels);
}

void Texture::d_glReadBuffer_special(void *context, GLenum src) {
    glReadBuffer(src);
}

void Texture::d_glGraphicBufferData(void *t_context, EGLContext ctx,
                                    uint64_t gbuffer_id, int width, int height,
                                    int buf_len, int row_byte_len, int stride,
                                    const void *real_buffer) {
    

    ThreadContext *thread_context = (ThreadContext *)t_context;

    ProcessContext *process_context = thread_context->process_context.get();
    void *guest_mem = (void *)real_buffer;

    //    GraphicBuffer *gbuffer = (GraphicBuffer
    //    *)g_hash_table_lookup(process_context->gbuffer_map,
    //    GUINT_TO_POINTER(gbuffer_id));
    GraphicBuffer *gbuffer = process_context->gbuffer_map[gbuffer_id];

    if (gbuffer == NULL) {
        gbuffer = MainRenderer::get_instance()->get_gbuffer_from_global_map(
            gbuffer_id);
    }

    if (gbuffer == NULL || width != gbuffer->width ||
        height != gbuffer->height) {
        return;
    }

    GLContext *opengl_context = (GLContext *)thread_context->opengl_context;
    if (opengl_context == NULL) {
        //        opengl_context = (GLContext
        //        *)g_hash_table_lookup(process_context->context_map,
        //        GUINT_TO_POINTER(ctx));
        opengl_context = process_context->gl_context_map[ctx];
        if (opengl_context == NULL) {
            ALOGD("error! GraphicBufferData get null context!\n");
        } else {
            if (opengl_context->independent_mode == 1) {
                //                glfwMakeContextCurrent((GLFWwindow
                //                *)opengl_context->window);
                ALOGE(
                    "glfwMakeContextCurrent((GLFWwindow "
                    "*)opengl_context->window);");
            } else {
                EglContext::native_make_current(opengl_context->native_context);
            }
        }
    }

    int real_width = width;
    if (real_width % (stride) != 0) {
        real_width = (real_width / stride + 1) * stride;
    }

    int guest_row_byte_len = row_byte_len / width * real_width;

    // printf("GraphicBuffer data width %d height %d row_byte_len %d
    // guest_row_byte_len %d\n", egl_image->width, egl_image->height,
    // row_byte_len, guest_row_byte_len);

    if (row_byte_len * height > buf_len) {
        ALOGD(
            "error! GraphicBuffer Data len error! width %d height %d "
            "row_byte_len %d stride %d get len %d\n",
            width, height, row_byte_len, stride, buf_len);
        return;
    }

    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);
    GLint asyn_texture = bound_buffer->async_unpack_texture_buffer;
    BufferStatus *buffer_status =
        &(opengl_context->bound_buffer_status.buffer_status);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, asyn_texture);

    buffer_status->host_pixel_unpack_buffer = asyn_texture;

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        
        //         glNamedBufferData(asyn_texture, row_byte_len * height, NULL,
        //         GL_STREAM_DRAW);
        ALOGE("DSA not supported.");
    } else {
        
        glBufferData(GL_PIXEL_UNPACK_BUFFER, row_byte_len * height, NULL,
                     GL_STREAM_DRAW);
    }

    GLubyte *map_pointer = NULL;
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        map_pointer = glMapNamedBufferRange(asyn_texture, 0,
        //        row_byte_len * height, GL_MAP_WRITE_BIT |
        //        GL_MAP_INVALIDATE_BUFFER_BIT);
        ALOGE("DSA not supported.");
    } else {
        map_pointer = (GLubyte *)glMapBufferRange(
            GL_PIXEL_UNPACK_BUFFER, 0, row_byte_len * height,
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    }
    ALOGD(
        "glGraphicBufferData id %llx width %d height %d internal_format %x "
        "format %x row_byte_len %d buf_len %d\n",
        gbuffer->gbuffer_id, width, height, gbuffer->internal_format,
        gbuffer->format, row_byte_len, buf_len);

    
    
    if (guest_row_byte_len != row_byte_len) {
        for (int i = 0; i < height; i++) {
            // guest_write(guest_mem, map_pointer + (egl_image->height - i - 1)
            // * row_byte_len, i * guest_row_byte_len, row_byte_len);
            // TODO: guest_write
            //            guest_write(guest_mem, map_pointer + i * row_byte_len,
            //            i * guest_row_byte_len, row_byte_len);
            memcpy(map_pointer + i * row_byte_len,
                   (char *)guest_mem + i * guest_row_byte_len, row_byte_len);
        }
    } else {
        // TODO: guest_write
        //        guest_write(guest_mem, map_pointer, 0, row_byte_len * height);
        memcpy(map_pointer, guest_mem, row_byte_len * height);
    }

    
    //  guest_write(guest_mem, map_pointer, 0, buf_len);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glUnmapNamedBuffer(asyn_texture);
        
        //
        //        glTextureSubImage2D(gbuffer->data_texture, 0, 0, 0,
        //        gbuffer->width, gbuffer->height, gbuffer->format,
        //        gbuffer->pixel_type, NULL);
        ALOGE("DSA not supported.");
    } else {
        glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
        glBindTexture(GL_TEXTURE_2D, gbuffer->data_texture);
        

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, gbuffer->width, gbuffer->height,
                        gbuffer->format, gbuffer->pixel_type, NULL);

        

        TextureBindingStatus *texture_status =
            &(opengl_context->texture_binding_status);

        glBindTexture(GL_TEXTURE_2D,
                      texture_status->host_current_texture_2D
                          [texture_status->host_current_active_texture]);
    }

    if (gbuffer->data_sync != 0) {
        if (gbuffer->delete_sync != 0) {
            glDeleteSync(gbuffer->delete_sync);
        }

        gbuffer->delete_sync = gbuffer->data_sync;
        gbuffer->data_sync = 0;
    }

    gbuffer->data_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    if (thread_context->opengl_context == NULL) {
        if (opengl_context->independent_mode == 1) {
            //            glfwMakeContextCurrent((GLFWwindow *)NULL);
            ALOGE("glfwMakeContextCurrent((GLFWwindow *)NULL);");
        } else {
            EglContext::native_make_current(NULL);
        }
    }

    // printf("get graphic buffer from image %llx guest width %d height %d
    // format %x len %d texture %u\n", gbuffer_id, width, height,
    // gbuffer->format, buf_len, gbuffer->data_texture);
}

void Texture::d_glReadGraphicBuffer(void *r_context, EGLContext ctx,
                                    uint64_t gbuffer_id, int width, int height,
                                    int buf_len, int row_byte_len, int stride,
                                    void *real_buffer) {
    
    ThreadContext *thread_context = (ThreadContext *)r_context;

    ProcessContext *process_context = thread_context->process_context.get();
    void *guest_mem = (void *)real_buffer;

    //    GraphicBuffer *gbuffer = (GraphicBuffer
    //    *)g_hash_table_lookup(process_context->gbuffer_map,
    //    GUINT_TO_POINTER(gbuffer_id));
    GraphicBuffer *gbuffer = process_context->gbuffer_map[gbuffer_id];

    if (gbuffer == NULL) {
        gbuffer = MainRenderer::get_instance()->get_gbuffer_from_global_map(
            gbuffer_id);
    }

    if (gbuffer == NULL || width != gbuffer->width ||
        height != gbuffer->height) {
        ALOGD("error! guest require gbuffer data size error %d %d origin %d %d",
              width, height, gbuffer == NULL ? 0 : gbuffer->width,
              gbuffer == NULL ? 0 : gbuffer->height);
        return;
    }

    GLContext *opengl_context = (GLContext *)thread_context->opengl_context;
    if (opengl_context == NULL) {
        //        opengl_context = (GLContext
        //        *)g_hash_table_lookup(process_context->context_map,
        //        GUINT_TO_POINTER(ctx));
        opengl_context = process_context->gl_context_map[ctx];
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

    int real_width = width;
    if (real_width % (stride) != 0) {
        real_width = (real_width / stride + 1) * stride;
    }

    int guest_row_byte_len = row_byte_len / width * real_width;

    // printf("GraphicBuffer data width %d height %d row_byte_len %d
    // guest_row_byte_len %d\n", egl_image->width, egl_image->height,
    // row_byte_len, guest_row_byte_len);

    if (row_byte_len * height > buf_len) {
        ALOGD(
            "error! GraphicBuffer read Data len error! row %d height %d get "
            "len %d\n",
            row_byte_len, height, buf_len);
        return;
    }

    
    GLuint temp_buffer;
    glGenBuffers(1, &temp_buffer);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, temp_buffer);
    glBufferData(GL_PIXEL_PACK_BUFFER, row_byte_len * height, NULL,
                 GL_STREAM_READ);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glGetTextureImage(gbuffer->data_texture, 0, gbuffer->format,
        //        gbuffer->pixel_type, row_byte_len * gbuffer->height, 0);
        ALOGE("DSA not supported.");
    } else {
        GLint pre_texture = 0;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &pre_texture);

        glBindTexture(GL_TEXTURE_2D, gbuffer->data_texture);

        // TODO: use glReadPixels instead

        //        glGetTexImage(GL_TEXTURE_2D, 0, gbuffer->format,
        //        gbuffer->pixel_type, 0);

        if (pre_texture != gbuffer->data_texture) {
            glBindTexture(GL_TEXTURE_2D, pre_texture);
        }
    }

    GLint error = glGetError();
    if (error != 0) {
        ALOGD(
            "error %x when d_glReadGraphicBuffer width %d height %d "
            "internal_format %x format %x row_byte_len %d buf_len %d\n",
            error, width, height, gbuffer->internal_format, gbuffer->format,
            row_byte_len, buf_len);
    }

    ALOGD(
        "glReadGraphicBuffer id %llx width %d height %d internal_format %x "
        "format %x row_byte_len %d buf_len %d\n",
        gbuffer->gbuffer_id, width, height, gbuffer->internal_format,
        gbuffer->format, row_byte_len, buf_len);
    GLubyte *map_pointer = NULL;
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        map_pointer = glMapNamedBufferRange(temp_buffer, 0,
        //        row_byte_len * height, GL_MAP_READ_BIT);
        ALOGE("DSA not supported.");
    } else {
        map_pointer = (GLubyte *)glMapBufferRange(
            GL_PIXEL_PACK_BUFFER, 0, row_byte_len * height, GL_MAP_READ_BIT);
    }

    if (guest_row_byte_len != row_byte_len) {
        for (int i = 0; i < height; i++) {
            // TODO: guest_read
            // guest_write(guest_mem, map_pointer + (height - i - 1) *
            // row_byte_len, i * guest_row_byte_len, row_byte_len);
            //            guest_read(guest_mem, map_pointer + i * row_byte_len,
            //            i * guest_row_byte_len, row_byte_len);
            memcpy((char *)guest_mem + i * guest_row_byte_len,
                   map_pointer + i * row_byte_len, row_byte_len);
        }
    } else {
        // TODO: guest_read
        //        guest_read(guest_mem, map_pointer, 0, row_byte_len * height);
        memcpy(guest_mem, map_pointer, row_byte_len * height);
    }

    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    glDeleteBuffers(1, &temp_buffer);

    BufferStatus *buffer_status =
        &(opengl_context->bound_buffer_status.buffer_status);
    buffer_status->host_pixel_pack_buffer = 0;

    // glBindBuffer(GL_PIXEL_PACK_BUFFER, opengl_context->current_pack_buffer);

    if (thread_context->opengl_context == NULL) {
        EglContext::native_make_current(NULL);
    }

    // printf("send graphic buffer from image %llx guest width %d height %d
    // format %x len %d\n", g_buffer_id, egl_image->width, egl_image->height,
    // egl_image->format, buf_len);
}

void Texture::d_glFramebufferTexture2D_special(void *context, GLenum target,
                                               GLenum attachment,
                                               GLenum textarget,
                                               GLuint guest_texture,
                                               GLint level) {
    GLContext *opengl_context = (GLContext *)context;
    GLuint host_texture =
        (GLuint)GLResource::get_host_texture_id(opengl_context, guest_texture);

    char is_init =
        GLResource::set_host_texture_init(opengl_context, guest_texture);

    if (is_init == 2) {
        GraphicBuffer *gbuffer =
            GLResource::get_texture_gbuffer_ptr(context, guest_texture);
        if (gbuffer != NULL) {
            MainRenderer::get_instance()->set_global_gbuffer_type(
                gbuffer->gbuffer_id, GBUFFER_TYPE_FBO);
        }
        if (textarget == GL_TEXTURE_EXTERNAL_OES) {
            textarget = GL_TEXTURE_2D;
        }
    }

    glFramebufferTexture2D(target, attachment, textarget, host_texture, level);
}
