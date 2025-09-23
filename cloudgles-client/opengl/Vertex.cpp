#include "Vertex.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

#include <algorithm>

#include "Helper.h"
#include "Log.h"
#include "Resource.h"
#include "State.h"
#include "egl/Surface.h"
#include "renderer/MainRenderer.h"

#define LOG_TAG "Vertex"

GLint Vertex::set_vertex_attrib_data(void *context, GLuint index, GLuint offset,
                                     GLuint length, const void *pointer) {
    BoundBuffer *bound_buffer = &(((GLContext *)context)->bound_buffer_status);
    // BufferStatus *status = bound_buffer->buffer_status;
    AttribPoint *point_data = bound_buffer->attrib_point;

    GLuint max_len = offset + length;

    unsigned char *map_pointer = NULL;

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        

        //        if (max_len > point_data->buffer_len[index])
        //        {
        
        //
        //            int alloc_size = max_len * BUFFER_MULTIPLY_FACTOR;
        //            if (alloc_size < 1024)
        //            {
        //                alloc_size = 1024;
        //            }
        //
        
        //            glNamedBufferData(point_data->buffer_object[index],
        //            alloc_size, NULL, GL_STREAM_DRAW);
        //            point_data->buffer_len[index] = alloc_size;
        //            map_pointer =
        //            glMapNamedBufferRange(point_data->buffer_object[index],
        //            offset, length,
        //                                                GL_MAP_WRITE_BIT |
        //                                                GL_MAP_FLUSH_EXPLICIT_BIT);
        //
        //            guest_write((Guest_Mem *)pointer, map_pointer, 0, length);
        //            glFlushMappedNamedBufferRange(point_data->buffer_object[index],
        //            0, length);
        //
        //            point_data->buffer_loc[index] = 0;
        //            point_data->remain_buffer_len[index] = max_len * 2 -
        //            max_len;
        //        }
        //        else if (length > point_data->remain_buffer_len[index])
        //        {
        //            map_pointer =
        //            glMapNamedBufferRange(point_data->buffer_object[index], 0,
        //            point_data->buffer_len[index],
        //                                                GL_MAP_WRITE_BIT |
        //                                                GL_MAP_FLUSH_EXPLICIT_BIT
        //                                                |
        //                                                GL_MAP_INVALIDATE_BUFFER_BIT);
        //
        
        //            guest_write((Guest_Mem *)pointer, map_pointer + offset, 0,
        //            length);
        //
        //            glFlushMappedNamedBufferRange(point_data->buffer_object[index],
        //            offset, length);
        //
        //            point_data->buffer_loc[index] = 0;
        //            point_data->remain_buffer_len[index] =
        //            point_data->buffer_len[index] - max_len;
        //
        //            // glBufferData(GL_ARRAY_BUFFER,
        //            point_data->buffer_len[index], NULL, GL_STREAM_DRAW);
        //            // map_pointer=glMapBufferRange(GL_ARRAY_BUFFER, offset,
        //            length,
        //            //     GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
        //
        //            // guest_write((Guest_Mem *)pointer,map_pointer,0,length);
        //            // glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, length);
        //
        //            // point_data->buffer_loc[index]=0;
        //            // point_data->remain_buffer_len[index]=max_len*2-max_len;
        //        }
        //        else
        //        {
        //            map_pointer =
        //            glMapNamedBufferRange(point_data->buffer_object[index],
        //                                                point_data->buffer_len[index]
        //                                                -
        //                                                point_data->remain_buffer_len[index],
        //                                                length,
        //                                                GL_MAP_WRITE_BIT |
        //                                                GL_MAP_FLUSH_EXPLICIT_BIT
        //                                                |
        //                                                GL_MAP_UNSYNCHRONIZED_BIT);
        //
        //            guest_write((Guest_Mem *)pointer, map_pointer, 0, length);
        //
        //            glFlushMappedNamedBufferRange(point_data->buffer_object[index],
        //            0, length);
        //
        //            point_data->buffer_loc[index] =
        //            point_data->buffer_len[index] -
        //            point_data->remain_buffer_len[index] - offset;
        //            point_data->remain_buffer_len[index] -= length;
        //        }
        //
        //        ALOGD("attrib point object %d loc %d %d index %d offset %d len
        //        %d\n", point_data->buffer_object[index],
        //        point_data->buffer_loc[index], point_data->buffer_loc[index] +
        //        length, index, offset, length);
        //
        //        glUnmapNamedBuffer(point_data->buffer_object[index]);
        ALOGE("DSA not supported.");
    } else {
        GLint padding =
            -std::min(point_data->buffer_len[index] -
                          point_data->remain_buffer_len[index] - (GLint)offset,
                      0);
        glBindBuffer(GL_ARRAY_BUFFER, point_data->buffer_object[index]);

        

        if (max_len > point_data->buffer_len[index]) {
            

            int alloc_size = max_len * BUFFER_MULTIPLY_FACTOR;
            if (alloc_size < 1024) {
                alloc_size = 1024;
            }

            
            glBufferData(GL_ARRAY_BUFFER, alloc_size, NULL, GL_STREAM_DRAW);
            point_data->buffer_len[index] = alloc_size;
            map_pointer = (unsigned char *)glMapBufferRange(
                GL_ARRAY_BUFFER, offset, length,
                GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
            // TODO: guest_write
            //            guest_write((Guest_Mem *)pointer, map_pointer, 0,
            //            length);
            memcpy(map_pointer, pointer, length);
            glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, length);

            point_data->buffer_loc[index] = 0;
            point_data->remain_buffer_len[index] = alloc_size - max_len;
        } else if (padding + length > point_data->remain_buffer_len[index]) {
            map_pointer = (unsigned char *)glMapBufferRange(
                GL_ARRAY_BUFFER, 0, point_data->buffer_len[index],
                GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT |
                    GL_MAP_INVALIDATE_BUFFER_BIT);

            
            // TODO: guest_write
            //            guest_write((Guest_Mem *)pointer, map_pointer +
            //            offset, 0, length);
            memcpy(map_pointer + offset, pointer, length);

            glFlushMappedBufferRange(GL_ARRAY_BUFFER, offset, length);

            point_data->buffer_loc[index] = 0;
            point_data->remain_buffer_len[index] =
                point_data->buffer_len[index] - max_len;

            // glBufferData(GL_ARRAY_BUFFER, point_data->buffer_len[index],
            // NULL, GL_STREAM_DRAW);
            // map_pointer=glMapBufferRange(GL_ARRAY_BUFFER, offset, length,
            //     GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

            // guest_write((Guest_Mem *)pointer,map_pointer,0,length);
            // glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, length);

            // point_data->buffer_loc[index]=0;
            // point_data->remain_buffer_len[index]=max_len*2-max_len;
        } else {
            map_pointer = (unsigned char *)glMapBufferRange(
                GL_ARRAY_BUFFER,
                point_data->buffer_len[index] -
                    point_data->remain_buffer_len[index],
                length + padding,
                GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT |
                    GL_MAP_UNSYNCHRONIZED_BIT);
            // TODO: guest_write
            //            guest_write((Guest_Mem *)pointer, map_pointer, 0,
            //            length);
            memcpy(map_pointer, pointer, length + padding);

            glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, length);

            point_data->buffer_loc[index] =
                point_data->buffer_len[index] -
                point_data->remain_buffer_len[index] + padding - offset;
            point_data->remain_buffer_len[index] -= length + padding;
        }

        ALOGD("attrib point loc %d %d index %d offset %d len %d\n",
              point_data->buffer_loc[index],
              point_data->buffer_loc[index] + length, index, offset, length);

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    return point_data->buffer_loc[index];
}

void Vertex::d_glVertexAttribPointer_without_bound(
    void *context, GLuint index, GLint size, GLenum type, GLboolean normalized,
    GLsizei stride, GLuint offset, GLuint length, const void *pointer) {
    GLContext *opengl_context = (GLContext *)context;
    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);
    BufferStatus *status = &(bound_buffer->buffer_status);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLint loc = set_vertex_attrib_data(context, index, offset,
        //        length, pointer);
        //
        //        AttribPoint *point_data = bound_buffer->attrib_point;
        //
        //        ALOGD("d_glVertexAttribPointer_without_bound vao %d %d obj %d
        //        index %u size %d type %x normalized %d stride %d offset %u
        //        length %d\n", status->guest_vao, status->host_vao,
        //                       point_data->buffer_object[index], index, size,
        //                       type, normalized, stride, offset, length);
        //
        //        glVertexArrayVertexAttribOffsetEXT(status->guest_vao,
        //        point_data->buffer_object[index], index, size, type,
        //        normalized, stride, (GLintptr)loc);
    } else {
        GLint vbo = status->host_array_buffer;
        // if(status->guest_array_buffer != 0)
        // {
        //     glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
        //     printf("error! %s vbo guest %d host %d but now
        //     %d\n",__FUNCTION__, status->guest_array_buffer,
        //     status->host_array_buffer, vbo);
        // }

        GLint loc =
            set_vertex_attrib_data(context, index, offset, length, pointer);

        ALOGD(
            "d_glVertexAttribPointer_without_bound index %u size %d type %x "
            "normalized %d stride %d offset %u length %d origin vbo %d\n",
            index, size, type, normalized, stride, offset, length, vbo);
        glVertexAttribPointer(index, size, type, normalized, stride,
                              (void *)(uint64_t)loc);

        glBindBuffer(GL_ARRAY_BUFFER, status->host_array_buffer);
    }

    return;
}

void Vertex::d_glVertexAttribIPointer_without_bound(
    void *context, GLuint index, GLint size, GLenum type, GLsizei stride,
    GLuint offset, GLuint length, const void *pointer) {
    GLContext *opengl_context = (GLContext *)context;
    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);
    BufferStatus *status = &(bound_buffer->buffer_status);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLint loc = set_vertex_attrib_data(context, index, offset,
        //        length, pointer);
        //
        //        ALOGD("d_glVertexAttribIPointer_without_bound index %u size %d
        //        type %x stride %d offset %u length %d\n", index, size, type,
        //        stride, offset, length);
        //
        //        AttribPoint *point_data = bound_buffer->attrib_point;
        //
        //        glVertexArrayVertexAttribIOffsetEXT(status->guest_vao,
        //        point_data->buffer_object[index], index, size, type, stride,
        //        (GLintptr)loc);
        ALOGE("DSA not supported.");
    } else {
        // if(status->guest_array_buffer != 0)
        // {
        //     GLint vbo;
        //     glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
        //     printf("error! %s vbo guest %d host %d but now
        //     %d\n",__FUNCTION__, status->guest_array_buffer,
        //     status->host_array_buffer, vbo);
        // }

        GLint loc =
            set_vertex_attrib_data(context, index, offset, length, pointer);

        glVertexAttribIPointer(index, size, type, stride,
                               (const void *)(uint64_t)loc);

        glBindBuffer(GL_ARRAY_BUFFER, status->host_array_buffer);
    }

    return;
}

void Vertex::d_glVertexAttribPointer_offset(void *context, GLuint index,
                                            GLuint size, GLenum type,
                                            GLboolean normalized,
                                            GLsizei stride, GLuint index_father,
                                            GLintptr offset) {
    BoundBuffer *bound_buffer = &(((GLContext *)context)->bound_buffer_status);
    AttribPoint *point_data = bound_buffer->attrib_point;
    BufferStatus *status = &(bound_buffer->buffer_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        ALOGD("pointer offset vao %d %d obj %d  index %u size %d type
        //        %x stride %d offset %u real offset %d\n",
        //                       status->guest_vao, status->host_vao,
        //                       point_data->buffer_object[index], index, size,
        //                       type, stride, offset, offset +
        //                       point_data->buffer_loc[index_father]);
        //
        //        glVertexArrayVertexAttribOffsetEXT(status->guest_vao,
        //        point_data->buffer_object[index_father], index, size, type,
        //        normalized, stride, (GLintptr)(offset +
        //        point_data->buffer_loc[index_father]));
        ALOGE("DSA not supported.");
    } else {
        // if(status->guest_array_buffer != 0)
        // {
        //     GLint vbo;
        //     glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
        //     printf("error! %s vbo guest %d host %d but now
        //     %d\n",__FUNCTION__, status->guest_array_buffer,
        //     status->host_array_buffer, vbo);
        // }

        glBindBuffer(GL_ARRAY_BUFFER, point_data->buffer_object[index_father]);

        ALOGD("pointer offset %lld\n",
              offset + point_data->buffer_loc[index_father]);
        glVertexAttribPointer(
            index, size, type, normalized, stride,
            (const void *)(offset + point_data->buffer_loc[index_father]));

        glBindBuffer(GL_ARRAY_BUFFER, status->host_array_buffer);
    }

    return;
}

void Vertex::d_glVertexAttribIPointer_offset(void *context, GLuint index,
                                             GLint size, GLenum type,
                                             GLsizei stride,
                                             GLuint index_father,
                                             GLintptr offset) {
    BoundBuffer *bound_buffer = &(((GLContext *)context)->bound_buffer_status);
    // BufferStatus *status = bound_buffer->buffer_status;
    AttribPoint *point_data = bound_buffer->attrib_point;

    BufferStatus *status = &(bound_buffer->buffer_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        glVertexArrayVertexAttribIOffsetEXT(status->guest_vao,
        //        point_data->buffer_object[index_father], index, size, type,
        //        stride, (GLintptr)(offset +
        //        point_data->buffer_loc[index_father]));
        ALOGE("DSA not supported.");
    } else {
        // if(status->guest_array_buffer != 0)
        // {
        //     GLint vbo;
        //     glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
        //     printf("error! %s vbo guest %d host %d but now
        //     %d\n",__FUNCTION__, status->guest_array_buffer,
        //     status->host_array_buffer, vbo);
        // }

        glBindBuffer(GL_ARRAY_BUFFER, point_data->buffer_object[index_father]);

        ALOGD("pointer offset %lld\n",
              offset + point_data->buffer_loc[index_father]);

        glVertexAttribIPointer(
            index, size, type, stride,
            (const void *)(offset + point_data->buffer_loc[index_father]));

        glBindBuffer(GL_ARRAY_BUFFER, status->host_array_buffer);
    }

    return;
}

void Vertex::d_glVertexAttribPointer_with_bound(void *context, GLuint index,
                                                GLint size, GLenum type,
                                                GLboolean normalized,
                                                GLsizei stride,
                                                GLintptr pointer) {
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        BoundBuffer *bound_buffer = &(((GLContext
        //        *)context)->bound_buffer_status); BufferStatus *status =
        //        &(bound_buffer->buffer_status);
        //
        //        glVertexArrayVertexAttribOffsetEXT(status->guest_vao,
        //        status->guest_array_buffer, index, size, type, normalized,
        //        stride, (GLintptr)pointer);
        ALOGE("DSA not supported.");
    } else {
        GLuint ebo = 0;
        GLuint vbo = 0;
#ifdef STD_DEBUG_LOG_GLOBAL_ON
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
#endif
        ALOGD(
            "%llx d_glVertexAttribPointer_with_bound index %u size %d type %x "
            "normalized %d stride %d pointer %llx ebo %d vbo %d\n",
            (uint64_t)context, index, size, type, normalized, stride, pointer,
            ebo, vbo);

        glVertexAttribPointer(index, size, type, normalized, stride,
                              (void *)pointer);
    }
    return;
}

void Vertex::d_glVertexAttribIPointer_with_bound(void *context, GLuint index,
                                                 GLint size, GLenum type,
                                                 GLsizei stride,
                                                 GLintptr pointer) {
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        BoundBuffer *bound_buffer = &(((GLContext
        //        *)context)->bound_buffer_status); BufferStatus *status =
        //        &(bound_buffer->buffer_status);
        //
        //        glVertexArrayVertexAttribIOffsetEXT(status->guest_vao,
        //        status->guest_array_buffer, index, size, type, stride,
        //        (GLintptr)pointer);
        ALOGE("DSA not supported.");
    } else {
        GLuint ebo = 0;
        GLuint vbo = 0;
#ifdef STD_DEBUG_LOG_GLOBAL_ON
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
#endif
        ALOGD(
            "%llx d_glVertexAttribIPointer_with_bound index %u size %d type %x "
            "stride %d pointer %llx ebo %d vbo %d\n",
            (uint64_t)context, index, size, type, stride, pointer, ebo, vbo);

        glVertexAttribIPointer(index, size, type, stride, (void *)pointer);
    }

    return;
}

void Vertex::d_glVertexAttribDivisor_origin(void *context, GLuint index,
                                            GLuint divisor) {
    
    ALOGD("%llx glVertexAttribDivisor %u %u\n", (uint64_t)context, index,
          divisor);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        BoundBuffer *bound_buffer = &(((GLContext
        //        *)context)->bound_buffer_status); BufferStatus *status =
        //        &(bound_buffer->buffer_status);
        //        glVertexArrayVertexAttribDivisorEXT(status->guest_vao, index,
        //        divisor);
        ALOGE("DSA not supported.");
    } else {
        glVertexAttribDivisor(index, divisor);
    }
}


//  void d_glGetVertexAttribPointerv_with_bound(void *context, GLuint index,
//  GLenum pname, GLintptr *pointer)
//  {
//      aaaaa
//  }

void Vertex::d_glDisableVertexAttribArray_origin(void *context, GLuint index) {
    
    ALOGD("%llx glDisableVertexAttribArray %u\n", (uint64_t)context, index);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        BoundBuffer *bound_buffer = &(((GLContext
        //        *)context)->bound_buffer_status); BufferStatus *status =
        //        &(bound_buffer->buffer_status);
        //        glDisableVertexArrayAttribEXT(status->guest_vao, index);
        ALOGE("DSA not supported.");
    } else {
        glDisableVertexAttribArray(index);
    }
}
void Vertex::d_glEnableVertexAttribArray_origin(void *context, GLuint index) {
    
    ALOGD("%llx glEnableVertexAttribArray %u\n", (uint64_t)context, index);
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        BoundBuffer *bound_buffer = &(((GLContext
        //        *)context)->bound_buffer_status); BufferStatus *status =
        //        &(bound_buffer->buffer_status);
        //        glEnableVertexArrayAttribEXT(status->guest_vao, index);
        ALOGE("DSA not supported.");
    } else {
        glEnableVertexAttribArray(index);
    }
}

void Vertex::d_glDrawArrays_origin(void *context, GLenum mode, GLint first,
                                   GLsizei count) {
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

#ifdef STD_DEBUG_LOG_GLOBAL_ON
    GLuint cu_vao = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &cu_vao);
    ALOGD("%llx glDrawArrays mode %x first %d count %d vao %d\n",
          (uint64_t)context, mode, first, count, cu_vao);
#endif
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external; ALOGD("use external
        //            texture %d\n", status->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawArrays(mode, first, count);
        //        ALOGD("glDrawArrays end\n");
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);

            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
            // printf("use external texture %d\n",
            // opengl_context->current_texture_external);
        }

        glDrawArrays(mode, first, count);

        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}

void Vertex::d_glDrawArraysInstanced_origin(void *context, GLenum mode,
                                            GLint first, GLsizei count,
                                            GLsizei instancecount) {
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawArraysInstanced(mode, first, count, instancecount);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
            // printf("use external texture %d\n",
            // opengl_context->current_texture_external);
        }
        glDrawArraysInstanced(mode, first, count, instancecount);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}

void Vertex::d_glDrawElements_with_bound(void *context, GLenum mode,
                                         GLsizei count, GLenum type,
                                         GLsizeiptr indices) {
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //        // buffer_binding_status_sync(context,
        //        GL_ELEMENT_ARRAY_BUFFER); if
        //        (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawElements(mode, count, type, (void *)indices);
        ALOGE("DSA not supported.");
    } else {
        GLuint ebo = 0;
        GLuint vbo = 0;
#ifdef STD_DEBUG_LOG_GLOBAL_ON
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
#endif
        ALOGD("drawElements %x %d %x %lx vbo %u ebo %u\n", mode, (int)count,
              type, indices, vbo, ebo);

        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
            // printf("use external texture %d\n",
            // opengl_context->current_texture_external);
        }
        // GLuint pre_array;
        // glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &pre_array);
        // printf("pre array vao %d\n",pre_array);
        // glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &pre_array);
        // printf("pre array ebo %d\n",pre_array);

        glDrawElements(mode, count, type, (void *)indices);
        //        glFlush();
        //        MainRenderer::get_instance()->send_window_message(MAIN_PAINT,
        //        opengl_context->cur_draw_surface->gbuffer);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}

GLint Vertex::set_indices_data(void *context, void *pointer, GLint length) {
    BoundBuffer *bound_buffer = &(((GLContext *)context)->bound_buffer_status);
    // BufferStatus *status = bound_buffer->buffer_status;
    AttribPoint *point_data = bound_buffer->attrib_point;

    GLint buffer_loc = 0;
    unsigned char *map_pointer = NULL;

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        // BufferStatus *status = &(bound_buffer->buffer_status);
        //
        //        if (length > point_data->indices_buffer_len)
        //        {
        
        //
        //            int alloc_size = length * BUFFER_MULTIPLY_FACTOR;
        //            if (alloc_size < 1024)
        //            {
        //                alloc_size = 1024;
        //            }
        //
        
        //            glNamedBufferData(point_data->indices_buffer_object,
        //            alloc_size, NULL, GL_STREAM_DRAW);
        //            point_data->indices_buffer_len = alloc_size;
        //            map_pointer =
        //            glMapNamedBufferRange(point_data->indices_buffer_object,
        //            0, length,
        //                                                GL_MAP_WRITE_BIT |
        //                                                GL_MAP_FLUSH_EXPLICIT_BIT);
        //
        //            guest_write((Guest_Mem *)pointer, map_pointer, 0, length);
        //            glFlushMappedNamedBufferRange(point_data->indices_buffer_object,
        //            0, length);
        //
        //            point_data->remain_indices_buffer_len =
        //            point_data->indices_buffer_len - length; buffer_loc = 0;
        //        }
        //        else if (length > point_data->remain_indices_buffer_len)
        //        {
        //            map_pointer =
        //            glMapNamedBufferRange(point_data->indices_buffer_object,
        //            0, point_data->indices_buffer_len,
        //                                                GL_MAP_WRITE_BIT |
        //                                                GL_MAP_FLUSH_EXPLICIT_BIT
        //                                                |
        //                                                GL_MAP_INVALIDATE_BUFFER_BIT);
        //
        
        //            guest_write((Guest_Mem *)pointer, map_pointer, 0, length);
        //
        //            glFlushMappedNamedBufferRange(point_data->indices_buffer_object,
        //            0, length);
        //
        //            point_data->remain_indices_buffer_len =
        //            point_data->indices_buffer_len - length; buffer_loc = 0;
        //        }
        //        else
        //        {
        //            map_pointer =
        //            glMapNamedBufferRange(point_data->indices_buffer_object,
        //                                                point_data->indices_buffer_len
        //                                                -
        //                                                point_data->remain_indices_buffer_len,
        //                                                length,
        //                                                GL_MAP_WRITE_BIT |
        //                                                GL_MAP_FLUSH_EXPLICIT_BIT
        //                                                |
        //                                                GL_MAP_UNSYNCHRONIZED_BIT);
        //
        //            guest_write((Guest_Mem *)pointer, map_pointer, 0, length);
        //
        //            glFlushMappedNamedBufferRange(point_data->indices_buffer_object,
        //            0, length);
        //
        //            buffer_loc = point_data->indices_buffer_len -
        //            point_data->remain_indices_buffer_len;
        //            point_data->remain_indices_buffer_len -= length;
        //        }
        //
        //        ALOGD("indices loc %d %d\n", buffer_loc, buffer_loc + length);
        //
        //        glUnmapNamedBuffer(point_data->indices_buffer_object);
        ALOGE("DSA not supported.");
    } else {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     point_data->indices_buffer_object);
        if (length > point_data->indices_buffer_len) {
            

            int alloc_size = length * BUFFER_MULTIPLY_FACTOR;
            if (alloc_size < 1024) {
                alloc_size = 1024;
            }

            
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, alloc_size, NULL,
                         GL_STREAM_DRAW);
            point_data->indices_buffer_len = alloc_size;
            map_pointer = (unsigned char *)glMapBufferRange(
                GL_ELEMENT_ARRAY_BUFFER, 0, length,
                GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
            // TODO: guest_write
            //            guest_write((Guest_Mem *)pointer, map_pointer, 0,
            //            length);
            memcpy(map_pointer, pointer, length);
            glFlushMappedBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, length);

            point_data->remain_indices_buffer_len =
                point_data->indices_buffer_len - length;
            buffer_loc = 0;
        } else if (length > point_data->remain_indices_buffer_len) {
            map_pointer = (unsigned char *)glMapBufferRange(
                GL_ELEMENT_ARRAY_BUFFER, 0, point_data->indices_buffer_len,
                GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT |
                    GL_MAP_INVALIDATE_BUFFER_BIT);
            // TODO: guest_write
            
            //            guest_write((Guest_Mem *)pointer, map_pointer, 0,
            //            length);
            memcpy(map_pointer, pointer, length);

            glFlushMappedBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, length);

            point_data->remain_indices_buffer_len =
                point_data->indices_buffer_len - length;
            buffer_loc = 0;
        } else {
            map_pointer = (unsigned char *)glMapBufferRange(
                GL_ELEMENT_ARRAY_BUFFER,
                point_data->indices_buffer_len -
                    point_data->remain_indices_buffer_len,
                length,
                GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT |
                    GL_MAP_UNSYNCHRONIZED_BIT);
            // TODO: guest_write
            //            guest_write((Guest_Mem *)pointer, map_pointer, 0,
            //            length);
            memcpy(map_pointer, pointer, length);

            glFlushMappedBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, length);

            buffer_loc = point_data->indices_buffer_len -
                         point_data->remain_indices_buffer_len;
            point_data->remain_indices_buffer_len -= length;

            // glBufferData(GL_ARRAY_BUFFER, point_data->indices_buffer_len,
            // NULL, GL_STREAM_DRAW);
            // map_pointer=glMapBufferRange(GL_ARRAY_BUFFER, 0, length,
            //     GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

            // guest_write((Guest_Mem *)pointer,map_pointer,0,length);
            // glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, length);

            // point_data->remain_indices_buffer_len=point_data->indices_buffer_len-length;
            // buffer_loc=0;
        }

        ALOGD("indices loc %d %d\n", buffer_loc, buffer_loc + length);

        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    }

    return buffer_loc;
}

void Vertex::d_glDrawElements_without_bound(void *context, GLenum mode,
                                            GLsizei count, GLenum type,
                                            const void *indices) {
    int len = count * GLHelper::gl_sizeof(type);

    ALOGD("drawElements without %x %d %x %lx len %d\n", mode, (int)count, type,
          indices, len);

    GLState::vao_binding_status_sync(context);

    GLint buffer_loc = set_indices_data(context, (void *)indices, len);

    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawElements(mode, count, type, (const void
        //        *)(uint64_t)buffer_loc);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
            // printf("use external texture %d\n",
            // opengl_context->current_texture_external);
        }

        glDrawElements(mode, count, type, (const void *)(uint64_t)buffer_loc);

        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    // // @todo
    
    // if(len<40000){
    //     glDrawElements(mode,count,type,indices);
    // }else{
    //     set_attrib_point_index(context,indices,len);

    //     glDrawElements(mode,count,type,0);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // }
}

void Vertex::d_glDrawElementsInstanced_without_bound(void *context, GLenum mode,
                                                     GLsizei count, GLenum type,
                                                     const void *indices,
                                                     GLsizei instancecount) {
    int len = count * GLHelper::gl_sizeof(type);

    GLState::vao_binding_status_sync(context);

    GLint buffer_loc = set_indices_data(context, (void *)indices, len);

    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawElementsInstanced(mode, count, type, (const void
        //        *)(uint64_t)buffer_loc, instancecount);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
            // printf("use external texture %d\n",
            // opengl_context->current_texture_external);
        }

        glDrawElementsInstanced(mode, count, type,
                                (const void *)(uint64_t)buffer_loc,
                                instancecount);

        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // // @todo
    
    // if(len<40000){
    //     glDrawElementsInstanced(mode,count,type,indices,instancecount);
    // }else{
    //     set_attrib_point_index(context,indices,len);
    //     glDrawElementsInstanced(mode,count,type,0,instancecount);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // }
}

void Vertex::d_glDrawElementsInstanced_with_bound(void *context, GLenum mode,
                                                  GLsizei count, GLenum type,
                                                  GLsizeiptr indices,
                                                  GLsizei instancecount) {
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //        // buffer_binding_status_sync(context,
        //        GL_ELEMENT_ARRAY_BUFFER); if
        //        (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawElementsInstanced(mode, count, type, (void *)indices,
        //        instancecount);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
            // printf("use external texture %d\n",
            // opengl_context->current_texture_external);
        }
        glDrawElementsInstanced(mode, count, type, (void *)indices,
                                instancecount);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}

void Vertex::d_glDrawRangeElements_with_bound(void *context, GLenum mode,
                                              GLuint start, GLuint end,
                                              GLsizei count, GLenum type,
                                              GLsizeiptr indices) {
    

    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //        // buffer_binding_status_sync(context,
        //        GL_ELEMENT_ARRAY_BUFFER); if
        //        (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawRangeElements(mode, start, end, count, type, (void
        //        *)indices);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
            // printf("use external texture %d\n",
            // opengl_context->current_texture_external);
        }
        glDrawRangeElements(mode, start, end, count, type, (void *)indices);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}

void Vertex::d_glDrawRangeElements_without_bound(void *context, GLenum mode,
                                                 GLuint start, GLuint end,
                                                 GLsizei count, GLenum type,
                                                 const void *indices) {
    int len = count * GLHelper::gl_sizeof(type);

    GLState::vao_binding_status_sync(context);

    GLint buffer_loc = set_indices_data(context, (void *)indices, len);
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawRangeElements(mode, start, end, count, type, (const void
        //        *)(uint64_t)buffer_loc);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
            // printf("use external texture %d\n",
            // opengl_context->current_texture_external);
        }
        glDrawRangeElements(mode, start, end, count, type,
                            (const void *)(uint64_t)buffer_loc);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // // @todo
    
    // if(len<40000){
    //     glDrawRangeElements(mode, start, end, count, type, indices);
    // }else{
    //     set_attrib_point_index(context,indices,len);

    //     glDrawRangeElements(mode, start, end, count, type, 0);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    // }
}

void Vertex::d_glVertexBindingDivisor_special(void *context,
                                              GLuint bindingindex,
                                              GLuint divisor) {
    glVertexBindingDivisor(bindingindex, divisor);
}

void Vertex::d_glDrawArraysIndirect_with_bound(void *context, GLenum mode,
                                               GLintptr indirect) {
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //
        //        buffer_binding_status_sync(context, GL_DRAW_INDIRECT_BUFFER);
        //
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawArraysIndirect(mode, (void *)indirect);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
        }
        glDrawArraysIndirect(mode, (void *)indirect);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}

void Vertex::d_glDrawArraysIndirect_without_bound(void *context, GLenum mode,
                                                  const void *indirect) {
    
    
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //        buffer_binding_status_sync(context, GL_DRAW_INDIRECT_BUFFER);
        //
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawArraysIndirect(mode, indirect);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
        }
        glDrawArraysIndirect(mode, indirect);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}

void Vertex::d_glDrawElementsIndirect_with_bound(void *context, GLenum mode,
                                                 GLenum type,
                                                 GLintptr indirect) {
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //
        //        buffer_binding_status_sync(context, GL_DRAW_INDIRECT_BUFFER);
        //
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawElementsIndirect(mode, type, (void *)indirect);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
        }
        glDrawElementsIndirect(mode, type, (void *)indirect);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}

void Vertex::d_glDrawElementsIndirect_without_bound(void *context, GLenum mode,
                                                    GLenum type,
                                                    const void *indirect) {
    
    
    GLContext *opengl_context = (GLContext *)context;
    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLState::vao_binding_status_sync(context);
        //
        //        buffer_binding_status_sync(context, GL_DRAW_INDIRECT_BUFFER);
        //
        //        if (opengl_context->is_using_external_program == 1)
        //        {
        //            glBindTextureUnit(0, status->current_texture_external);
        //            status->host_current_texture_2D[0] =
        //            status->current_texture_external;
        //
        //            // status->host_current_texture_unit[0] =
        //            status->current_texture_external;
        //            // printf("use external texture %d\n",
        //            opengl_context->current_texture_external);
        //        }
        //        else
        //        {
        //            texture_unit_status_sync(context, -1);
        //        }
        //
        //        glDrawElementsIndirect(mode, type, indirect);
        ALOGE("DSA not supported.");
    } else {
        if (opengl_context->is_using_external_program == 1) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, status->current_texture_external);
        }
        glDrawElementsIndirect(mode, type, indirect);
        if (opengl_context->is_using_external_program == 1) {
            glBindTexture(GL_TEXTURE_2D, status->host_current_texture_2D[0]);
            glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
        }
    }
}