#include "Memory.h"

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
#include "renderer/MainRenderer.h"

#define LOG_TAG "Memory"

GLuint Memory::get_guest_buffer_binding_id(void *context, GLenum target) {
    // GLuint buffer_id;

    GLContext *opengl_context = (GLContext *)context;
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    switch (target) {
        case GL_ARRAY_BUFFER:
            return status->guest_array_buffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            return status->guest_element_array_buffer;
            break;
        case GL_COPY_READ_BUFFER:
            return status->guest_copy_read_buffer;
            break;
        case GL_COPY_WRITE_BUFFER:
            return status->guest_copy_write_buffer;
            break;
        case GL_PIXEL_PACK_BUFFER:
            return status->guest_pixel_pack_buffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER:
            return status->guest_pixel_unpack_buffer;
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            return status->guest_transform_feedback_buffer;
            break;
        case GL_UNIFORM_BUFFER:
            return status->guest_uniform_buffer;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            return status->guest_atomic_counter_buffer;
            break;
        case GL_DISPATCH_INDIRECT_BUFFER:
            return status->guest_dispatch_indirect_buffer;
            break;
        case GL_DRAW_INDIRECT_BUFFER:
            return status->guest_draw_indirect_buffer;
            break;
        case GL_SHADER_STORAGE_BUFFER:
            return status->guest_shader_storage_buffer;
        default:
            ALOGD("get_guest_buffer_binding_id error target %x\n", target);
            return 0;
    }
    return 0;
}

void Memory::d_glBufferData_custom(void *context, GLenum target,
                                   GLsizeiptr size, const void *data,
                                   GLenum usage) {
    // todo
    
    //    Guest_Mem *guest_mem = (Guest_Mem *)data;
    //    Scatter_Data *s_data = guest_mem->scatter_data;
    GLuint bind_buffer = GLState::get_guest_binding_buffer(context, target);

    ALOGD("%llx %s target %x bind_buffer %d size %lld usage %x real size %d\n",
          (uint64_t)context, __FUNCTION__, target, bind_buffer, size, usage,
          -1);

    if (size == 0) {
        return;
    }

    //    if (guest_mem->all_len == 0)
    //    {
    //        express_printf("glBufferData null\n");
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            glNamedBufferData(bind_buffer, size, NULL, usage);
    //        }
    //        else
    //        {
    //            glBufferData(target, size, NULL, usage);
    //        }
    //        return;
    //    }

    
    glBufferData(target, size, data, usage);

    //    if (size == s_data[0].len)
    //    {
    
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            glNamedBufferData(bind_buffer, size, s_data[0].data, usage);
    //        }
    //        else
    //        {
    //            glBufferData(target, size, s_data[0].data, usage);
    //        }
    //        uint32_t crc = 0;
    //        // for(int i=0;i<size;i++)
    //        // {
    //        //     crc = updateCRC32(s_data[0].data[i],crc);
    //        // }
    //
    //        express_printf("glBufferData direct %d crc %x\n", size, crc);
    //        // float *temp=(float *)s_data[0].data;
    //        // for(int i=0;i<size/4;i++){
    //        //     express_printf("%f ",temp[i]);
    //        // }
    //        // express_printf("\n");
    //    }
    //    else
    //    {
    
    //
    //        // char *temp=g_malloc(size);
    //        // host_guest_buffer_exchange(s_data, temp, 0, size, 1);
    //        // glBufferData(target, size,temp, usage);
    //
    //        // g_free(temp);
    //
    //        // return;
    //
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            glNamedBufferData(bind_buffer, size, NULL, usage);
    //        }
    //        else
    //        {
    //            glBufferData(target, size, NULL, usage);
    //        }
    //
    //        GLubyte *map_pointer = NULL;
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            map_pointer = glMapNamedBufferRange(bind_buffer, 0, size,
    //            GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
    //        }
    //        else
    //        {
    //            map_pointer = glMapBufferRange(target, 0, size,
    //            GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
    //        }
    //        host_guest_buffer_exchange(s_data, map_pointer, 0, size, 1);
    //
    //        express_printf("glBufferData indirect %d\n", size);
    //        // float *temp=g_malloc(size);
    //        // host_guest_buffer_exchange(s_data, temp, 0, size, 1);
    //        // for(int i=0;i<size/4;i++){
    //        //     express_printf("%f ",temp[i]);
    //        // }
    //        // express_printf("\n");
    //        // glBufferData(target, size,temp, usage);
    //
    //        // g_free(temp);
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            glUnmapNamedBuffer(bind_buffer);
    //        }
    //        else
    //        {
    //            glUnmapBuffer(target);
    //        }
    //    }
}
void Memory::d_glBufferSubData_custom(void *context, GLenum target,
                                      GLintptr offset, GLsizeiptr size,
                                      const void *data) {
    // todo
    
    //    Guest_Mem *guest_mem = (Guest_Mem *)data;
    //    Scatter_Data *s_data = guest_mem->scatter_data;
    GLuint bind_buffer = GLState::get_guest_binding_buffer(context, target);
    if (bind_buffer == 0) {
        ALOGD("d_glBufferSubData_custom target %x\n", target);
    }

    //    if (guest_mem->all_len == 0)
    //    {
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            glNamedBufferSubData(bind_buffer, offset, size, NULL);
    //        }
    //        else
    //        {
    //            glBufferSubData(target, offset, size, NULL);
    //        }
    //        return;
    //    }

    
    glBufferSubData(target, offset, size, data);

    //    if (size == s_data[0].len)
    //    {
    
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            glNamedBufferSubData(bind_buffer, offset, size,
    //            s_data[0].data);
    //        }
    //        else
    //        {
    //            glBufferSubData(target, offset, size, s_data[0].data);
    //        }
    //    }
    //    else
    //    {
    //        // char *temp=g_malloc(size);
    //        // host_guest_buffer_exchange(s_data, temp, 0, size, 1);
    //        // glBufferSubData(target, offset, size, temp);
    //
    //        // g_free(temp);
    //
    //        // return;
    //
    
    //        GLubyte *map_pointer = NULL;
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            map_pointer = glMapNamedBufferRange(bind_buffer, offset, size,
    //            GL_MAP_WRITE_BIT);
    //        }
    //        else
    //        {
    //            map_pointer = glMapBufferRange(target, offset, size,
    //            GL_MAP_WRITE_BIT);
    //        }
    //
    //        host_guest_buffer_exchange(s_data, map_pointer, 0, size, 1);
    //
    //        if (DSA_LIKELY(host_opengl_version >= 45 && DSA_enable != 0))
    //        {
    //            glUnmapNamedBuffer(bind_buffer);
    //        }
    //        else
    //        {
    //            glUnmapBuffer(target);
    //        }
    //    }

    // GLint error = glGetError();
    // if(error!=0)
    // {
    //     GLint all_size = 0;

    //     glGetNamedBufferParameteriv(bind_buffer, GL_BUFFER_SIZE, &all_size);
    //     printf("glBufferSubData context %llx error %x target %x buffer %d
    //     offset %d size %d all size %d\n", (uint64_t)context, error, target,
    //     bind_buffer, (int)offset, (int)size, (int)all_size);
    // }
}

void Memory::d_glMapBufferRange_read(void *context, GLenum target,
                                     GLintptr offset, GLsizeiptr length,
                                     GLbitfield access, void *mem_buf) {
    d_glMapBufferRange_write(context, target, offset, length, access);

    
    if (access & GL_MAP_READ_BIT) {
        //        GHashTable *buffer_map = ((GLContext *)context)->buffer_map;
        //        MappedBuffer *map_res = g_hash_table_lookup(buffer_map,
        //        (gpointer)((((guint64)target) << 32) +
        //        get_guest_buffer_binding_id(context, target)));
        MappedBuffer *map_res =
            ((GLContext *)context)
                ->buffer_map[(
                    uint64_t)((((uint64_t)target) << 32) +
                              get_guest_buffer_binding_id(context, target))];
        memcpy(mem_buf, (void *)map_res->data, length);
        // host_guest_buffer_exchange(map_res->guest_data, map_res->data,
        // 0, length, 0);
    }
}

void Memory::d_glMapBufferRange_write(void *context, GLenum target,
                                      GLintptr offset, GLsizeiptr length,
                                      GLbitfield access) {
    // GLint size = 0;

    // glGetBufferParameteriv(target, GL_BUFFER_SIZE, &size);
    // ALOGD("mapbufferrange target %x offset %d length %d end %d buffer id %d
    // buffer size %d access
    // %x\n",(int)target,(int)offset,(int)length,(int)offset+(int)length,buffer,size,(int)access);

    GLuint bind_buffer = GLState::get_guest_binding_buffer(context, target);

    GLubyte *map_pointer = NULL;

    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        map_pointer = glMapNamedBufferRange(bind_buffer, offset,
        //        length, access);
        ALOGE("DSA not supported.");
    } else {
        map_pointer =
            (GLubyte *)glMapBufferRange(target, offset, length, access);
    }

    
    //     GHashTable *buffer_map = ((GLContext *)context)->buffer_map;
    //     MappedBuffer *map_res = g_hash_table_lookup(buffer_map,
    //     (gpointer)((((guint64)target) << 32) +
    //     get_guest_buffer_binding_id(context, target)));
    MappedBuffer *map_res =
        ((GLContext *)context)
            ->buffer_map[(
                uint64_t)((((uint64_t)target) << 32) +
                          get_guest_buffer_binding_id(context, target))];
    if (map_res == NULL) {
        map_res = (MappedBuffer *)malloc(sizeof(MappedBuffer));
        memset(map_res, 0, sizeof(MappedBuffer));
        //        g_hash_table_insert(buffer_map, (gpointer)((((guint64)target)
        //        << 32) + get_guest_buffer_binding_id(context, target)),
        //        (gpointer)map_res);
        ((GLContext *)context)
            ->buffer_map[(uint64_t)((((uint64_t)target) << 32) +
                                    get_guest_buffer_binding_id(
                                        context, target))] = map_res;
    } else {
        
        //@todo
        
        ALOGD("error! map_res is not NULL!\n");
    }

    map_res->access = access;
    map_res->data = map_pointer;
    map_res->map_len = length;
    map_res->target = target;

    
    
}

GLboolean Memory::d_glUnmapBuffer_special(void *context, GLenum target) {
    //    GHashTable *buffer_map = ((GLContext *)context)->buffer_map;
    //    MappedBuffer *map_res = g_hash_table_lookup(buffer_map,
    //    (gpointer)((((guint64)target) << 32) +
    //    get_guest_buffer_binding_id(context, target)));
    MappedBuffer *map_res =
        ((GLContext *)context)
            ->buffer_map[(
                uint64_t)((((uint64_t)target) << 32) +
                          get_guest_buffer_binding_id(context, target))];
    if (map_res == NULL) {
        ALOGD("error! unmap get NULL map_res!\n");
        return GL_FALSE;
    }

    ALOGD("unmap target %x\n", target);

    
    GLboolean ret = GL_TRUE;
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLuint bind_buffer =
        //        GLState::get_guest_binding_buffer(context, target); ret =
        //        glUnmapNamedBuffer(bind_buffer);
        ALOGE("DSA not supported.");
    } else {
        ret = glUnmapBuffer(target);
    }

    //    g_hash_table_remove(buffer_map, (gpointer)((((guint64)target) << 32) +
    //    get_guest_buffer_binding_id(context, target)));
    ((GLContext *)context)
        ->buffer_map.erase(
            (uint64_t)((((uint64_t)target) << 32) +
                       get_guest_buffer_binding_id(context, target)));

    return ret;
}

void Memory::d_glFlushMappedBufferRange_special(void *context, GLenum target,
                                                GLintptr offset,
                                                GLsizeiptr length,
                                                const void *data) {
    //    GHashTable *buffer_map = ((GLContext *)context)->buffer_map;
    //    MappedBuffer *map_res = g_hash_table_lookup(buffer_map,
    //    (gpointer)((((guint64)target) << 32) +
    //    get_guest_buffer_binding_id(context, target)));
    MappedBuffer *map_res =
        ((GLContext *)context)
            ->buffer_map[(
                uint64_t)((((uint64_t)target) << 32) +
                          get_guest_buffer_binding_id(context, target))];

    if (map_res == NULL) {
        map_res = (MappedBuffer *)malloc(sizeof(MappedBuffer));
        memset(map_res, 0, sizeof(MappedBuffer));
        //        g_hash_table_insert(buffer_map, (gpointer)((((guint64)target)
        //        << 32) + get_guest_buffer_binding_id(context, target)),
        //        (gpointer)map_res);
        ((GLContext *)context)
            ->buffer_map[(uint64_t)((((uint64_t)target) << 32) +
                                    get_guest_buffer_binding_id(
                                        context, target))] = map_res;
        ALOGD("error! flush data get NULL map_res!\n");
        return;
    }
    if (map_res->data == NULL) {
        ALOGD("error! host data get NULL!\n");
        return;
    }
    if (map_res->access & GL_MAP_WRITE_BIT) {
        memcpy(map_res->data + offset, data, length);

        uint32_t crc = 0;
        // for(int i=0;i<length;i++)
        // {
        //     crc = updateCRC32((map_res->data + offset)[i],crc);
        // }

        ALOGD(
            "flush mapbufferrange target %x offset %d length %d access %x crc "
            "%x\n",
            (int)target, (int)offset, (int)length, (int)map_res->access, crc);
        if ((map_res->access & GL_MAP_FLUSH_EXPLICIT_BIT)) {
            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                //                GLuint bind_buffer =
                //                GLState::get_guest_binding_buffer(context,
                //                target);
                //                glFlushMappedNamedBufferRange(bind_buffer,
                //                offset, length);
                ALOGE("DSA not supported.");
            } else {
                glFlushMappedBufferRange(target, offset, length);
            }
        }
    }
}
