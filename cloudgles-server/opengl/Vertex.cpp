#define LOG_TAG "GLVertex"
#include "Vertex.h"

#include "ErrorHandler.h"
#include "GLDef.h"
#include "GLUtils.h"
#include "Log.h"
#include "ThreadContext.h"
#include "opengl/Memory.h"

/**
 * @brief 
 *
 * @param context
 * @param data
 * @param min_index
 * @param max_index
 */
void get_max_min_point_index_direct(void *context, const void *data,
                                    GLsizei count, GLenum type,
                                    GLuint *min_index, GLuint *max_index) {
    
    GLuint min_temp = 0xffffffff;
    GLuint max_temp = 0;
    for (int i = 0; i < count; i++) {
        GLuint t;
        switch (type) {
            case GL_UNSIGNED_BYTE:
                t = (GLuint) * ((GLubyte *)data + i);
                break;
            case GL_UNSIGNED_SHORT:
                t = (GLuint) * ((GLushort *)data + i);
                break;
            case GL_UNSIGNED_INT:
                t = (GLuint) * ((GLuint *)data + i);
                break;
        }
        if (t < min_temp) {
            min_temp = t;
        }
        if (t > max_temp) {
            max_temp = t;
        }
    }
    *min_index = min_temp;
    *max_index = max_temp;
    return;
}

/**
 * @brief
 * 
 *
 * @param context
 * @param count
 * @param type
 * @param indices
 * @param instancecount
 */
void send_point_attrib_data_indices(void *context, GLsizei count, GLenum type,
                                    const void *indices, GLint instancecount) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;

    AttribPoint *point_data = bound_buffer->attrib_point;
    GLuint min_index;
    GLuint max_index;

    
    
    int flag = 0;
    int point_loc = 0;
    for (int i = 0; i < point_data->max_index + 1; i++) {
        if (point_data->location[i] == 1 && point_data->enabled[i] == 1) {
            flag = 1;
            point_loc = i;
            break;
        }
    }
    if (flag == 0) {
        return;
    }

    ALOGD(
        "send_point_attrib_data_indices count %d type %x instancecount %d loc "
        "%d",
        count, type, instancecount, point_loc);

    
    
    if (get_bound_buffer(context, GL_ELEMENT_ARRAY_BUFFER) == 0) {
        
        

        get_max_min_point_index_direct(context, indices, count, type,
                                       &min_index, &max_index);
    } else {
        

        
        
        get_max_min_point_index_rmq(context, (uint64_t)indices, count, type,
                                    &min_index, &max_index);
    }
    ALOGD("send attrib data min %d max %d", min_index, max_index);
    send_point_attrib_data(context, min_index, max_index, instancecount);
}

void get_max_min_point_index_rmq(void *context, uint64_t offset, GLsizei count,
                                 GLenum type, GLuint *min_index,
                                 GLuint *max_index) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    ElementArrayBuffer *ebo_data =
        bound_buffer
            ->ebo_buffer[bound_buffer->buffer_status->element_array_buffer];
    if (ebo_data == NULL) {
        *max_index = 0;
        *min_index = 0;
        ALOGE("error! ebo_data is NULL ebo id %u!",
              bound_buffer->buffer_status->element_array_buffer);
        return;
    }
    ebo_data->type = type;

    uint64_t index_offset = offset;
    int not_align_flag = 0;

    switch (ebo_data->type) {
        case GL_UNSIGNED_BYTE:
            index_offset = offset / sizeof(GLubyte);
            if (offset % sizeof(GLubyte) != 0) {
                not_align_flag = 1;
            }
            break;
        case GL_UNSIGNED_SHORT:
            index_offset = offset / sizeof(GLushort);
            if (offset % sizeof(GLushort) != 0) {
                not_align_flag = 1;
            }
            break;
        case GL_UNSIGNED_INT:
            index_offset = offset / sizeof(GLuint);
            if (offset % sizeof(GLuint) != 0) {
                not_align_flag = 1;
            }
            break;
    }

    
    
    char *no_align_data = NULL;
    size_t no_align_size = 0;
    if (not_align_flag == 1) {
        index_offset = 0;
        no_align_data = ebo_data->data;
        ebo_data->data = ebo_data->data + offset;
        no_align_size = ebo_data->size;
        ebo_data->size = ebo_data->size - offset;
    }

    if (ebo_data->has_update) {
        
        ebo_data->has_update = false;
        GLuint real_size = ebo_data->size / gl_sizeof(type);
        
        if (real_size > bound_buffer->ebo_mm_num ||
            bound_buffer->ebo_mm == NULL) {
            GLuint t_size = real_size;
            
            if (t_size < bound_buffer->ebo_mm_num * 2) {
                t_size = bound_buffer->ebo_mm_num * 2;
            } else {
                t_size += bound_buffer->ebo_mm_num;
            }

            GLuint *temp = new GLuint[t_size + 1];
            for (int i = 1; i < t_size + 1; i++) {
                if (i < bound_buffer->ebo_mm_num + 1) {
                    temp[i] = bound_buffer->ebo_mm[i];
                } else {
                    temp[i] =
                        ((i & (i - 1)) == 0) ? temp[i - 1] + 1 : temp[i - 1];
                }
            }
            bound_buffer->ebo_mm_num = t_size;
            delete[] bound_buffer->ebo_mm;
            bound_buffer->ebo_mm = temp;
        }
        int max_log_n = bound_buffer->ebo_mm[real_size];
        if (ebo_data->dp_num < real_size) {
            
            
            if (ebo_data->dp_max != NULL) {
                for (int i = 1; i <= bound_buffer->ebo_mm[ebo_data->dp_num];
                     i++) {
                    delete[] ebo_data->dp_max[i];
                    delete[] ebo_data->dp_min[i];
                }
                delete[] ebo_data->dp_max;
                delete[] ebo_data->dp_min;
            }

            
            ebo_data->dp_max = new GLuint *[max_log_n + 1];
            ebo_data->dp_min = new GLuint *[max_log_n + 1];
            ebo_data->dp_num = real_size;

            for (int i = 0; i <= max_log_n; i++) {
                ebo_data->dp_max[i] = new GLuint[real_size + 5];
                ebo_data->dp_min[i] = new GLuint[real_size + 5];
            }
        }

        for (int i = 1; i <= real_size; i++) {
            GLuint t;
            switch (ebo_data->type) {
                case GL_UNSIGNED_BYTE:
                    t = (GLuint) * ((GLubyte *)ebo_data->data + i - 1);
                    break;
                case GL_UNSIGNED_SHORT:
                    t = (GLuint) * ((GLushort *)ebo_data->data + i - 1);
                    break;
                case GL_UNSIGNED_INT:
                    t = (GLuint) * ((GLuint *)ebo_data->data + i - 1);
                    break;
            }
            ebo_data->dp_max[0][i] = t;
            ebo_data->dp_min[0][i] = t;
        }
        for (int j = 1; j <= max_log_n; j++) {
            for (int i = 1; i + (1 << j) - 1 <= real_size; i++) {
                ebo_data->dp_max[j][i] =
                    std::max(ebo_data->dp_max[j - 1][i],
                             ebo_data->dp_max[j - 1][i + (1 << (j - 1))]);
                ebo_data->dp_min[j][i] =
                    std::min(ebo_data->dp_min[j - 1][i],
                             ebo_data->dp_min[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    
    int k = bound_buffer->ebo_mm[count + 1];
    *max_index =
        std::max(ebo_data->dp_max[k][index_offset + 1],
                 ebo_data->dp_max[k][(index_offset + count) - (1 << k) + 1]);
    *min_index =
        std::min(ebo_data->dp_min[k][index_offset + 1],
                 ebo_data->dp_min[k][(index_offset + count) - (1 << k) + 1]);

    if (not_align_flag == 1) {
        ebo_data->data = no_align_data;
        ebo_data->size = no_align_size;
    }

    ALOGD("calc ebo_data min max %u %u", *min_index, *max_index);
    
    //  GLuint temp_min_index;
    //  GLuint temp_max_index;
    //  get_max_min_point_index_direct(context, (void *)(ebo_data->data +
    //  offset), count, type,
    //                                 &temp_min_index, &temp_max_index);

    // assert(temp_max_index == *max_index);
    // assert(temp_min_index == *min_index);

    return;
}

void send_point_attrib_data(void *context, GLuint min_index, GLuint max_index,
                            GLint instancecount) {
    

    

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;

    AttribPoint *point_data = bound_buffer->attrib_point;

    if (point_data->has_update) {
        
        
        point_data->has_update = 0;
        for (int i = 0; i < point_data->max_index + 1; i++) {
            if (point_data->location[i] == 0 || point_data->enabled[i] == 0) {
                continue;
            }

            GLuint father_index = i;
            const char *min_pointer = point_data->data[i];
            const char *min_pointer_sub_stride =
                point_data->data[i] - point_data->stride[i];
            for (int j = 0; j < point_data->max_index + 1; j++) {
                if (point_data->location[j] == 1 &&
                    point_data->enabled[j] == 1 && j != i) {
                    
                    if (point_data->data[j] < min_pointer &&
                        point_data->data[j] > min_pointer_sub_stride) {
                        
                        min_pointer = point_data->data[j];
                        min_pointer_sub_stride =
                            point_data->data[j] - point_data->stride[j];
                        father_index = j;
                    }
                }
            }

            point_data->data_index_father[i] = father_index;
        }
    }

    

    int send_index[MAX_VERTEX_ATTRIBS_NUM];
    memset(send_index, 0, sizeof(send_index));
    for (int i = 0; i < point_data->max_index + 1; i++) {
        if (point_data->location[i] == 0 || point_data->enabled[i] == 0) {
            continue;
        }
        GLuint father_index = point_data->data_index_father[i];

        

        if (send_index[father_index] == 0) {
            

            
            uint64_t end_loc =
                point_data->stride[father_index] * (max_index + 1);
            uint64_t start_loc = point_data->stride[father_index] * min_index;

            
            if (instancecount >= 0) {
                int divisor = point_data->divisor[father_index];
                uint64_t father_instance_end_loc = end_loc;
                if (divisor) {
                    
                    father_instance_end_loc =
                        point_data->stride[father_index] *
                        (int)((instancecount + divisor - 1) / divisor);
                }
                
                divisor = point_data->divisor[i];
                uint64_t instance_end_loc = end_loc;
                if (divisor) {
                    
                    instance_end_loc =
                        point_data->stride[i] *
                        (int)((instancecount + divisor - 1) / divisor);
                }
                
                
                end_loc = std::max(father_instance_end_loc, instance_end_loc);
            }
            if (point_data->stride[father_index] != point_data->stride[i]) {
                ALOGE(
                    "error! father stride not equal father %d %lld me %d %lld",
                    father_index, (long long)point_data->stride[father_index],
                    i, (long long)point_data->stride[i]);
            }
            if (point_data->divisor[i] != point_data->divisor[father_index]) {
                ALOGE(
                    "error! father divisor not equal father %d %lld me %d %lld",
                    father_index, (long long)point_data->divisor[father_index],
                    i, (long long)point_data->divisor[i]);
            }

            
            if (point_data->invoke_type[father_index] == GL_FLOAT) {
                ALOGD(
                    "glVertexAttribPointer_without_bound %u %d %x %d %llu %llu "
                    "%llu %llu",
                    father_index, point_data->size[father_index],
                    point_data->type[father_index],
                    (int)point_data->normalized[father_index],
                    (unsigned long long)point_data->stride[father_index],
                    (unsigned long long)start_loc,
                    (unsigned long long)end_loc - start_loc,
                    (unsigned long long)point_data->data[father_index] +
                        start_loc);
                r_glVertexAttribPointer_without_bound(
                    context, father_index, point_data->size[father_index],
                    point_data->type[father_index],
                    point_data->normalized[father_index],
                    point_data->stride[father_index], start_loc,
                    end_loc - start_loc,
                    point_data->data[father_index] + start_loc);
            } else {
                r_glVertexAttribIPointer_without_bound(
                    context, father_index, point_data->size[father_index],
                    point_data->type[father_index],
                    point_data->stride[father_index], start_loc,
                    end_loc - start_loc,
                    point_data->data[father_index] + start_loc);
            }
            send_index[father_index] = 1;
        }

        if (send_index[i] == 0) {
            
            if (point_data->invoke_type[i] == GL_FLOAT) {
                
                if (point_data->enabled[i]) {
                    ALOGD(
                        "glVertexAttribPointer offset %d %d %x %d %llu %u %llu",
                        i, point_data->size[i], point_data->type[i],
                        (int)point_data->normalized[i],
                        (unsigned long long)point_data->stride[i], father_index,
                        (unsigned long long)(point_data->data[i] -
                                             point_data->data[father_index]));
                    r_glVertexAttribPointer_offset(
                        context, i, point_data->size[i], point_data->type[i],
                        point_data->normalized[i], point_data->stride[i],
                        father_index,
                        (GLintptr)(point_data->data[i] -
                                   point_data->data[father_index]));
                }
            } else {
                if (point_data->enabled[i]) {
                    ALOGD("glVertexAttribIPointer offset %d %d %x %llu %u %llu",
                          i, point_data->size[i], point_data->type[i],
                          (unsigned long long)point_data->stride[i],
                          father_index,
                          (unsigned long long)(point_data->data[i] -
                                               point_data->data[father_index]));
                    r_glVertexAttribIPointer_offset(
                        context, i, point_data->size[i], point_data->type[i],
                        point_data->stride[i], father_index,
                        (GLintptr)(point_data->data[i] -
                                   point_data->data[father_index]));
                }
            }
            send_index[i] = 1;
        }
    }
}

void send_gbuffer_data(void *context) {
    GLThreadContext *opengl_context = (GLThreadContext *)context;
    BindingStatus *binding_status = &(opengl_context->context_binding_status);
    return;

    if (binding_status->gbuffer_2D_binding != NULL &&
        binding_status->gbuffer_2D_binding->gbuffer_type ==
            GBUFFER_TYPE_NATIVE &&
        binding_status->gbuffer_2D_binding->need_send_data == 1) {
        binding_status->gbuffer_2D_binding->need_send_data = 0;
        upload_graphic_buffer(opengl_context,
                              binding_status->gbuffer_2D_binding);
    }

    if (binding_status->gbuffer_external_binding != NULL &&
        binding_status->gbuffer_external_binding->gbuffer_type ==
            GBUFFER_TYPE_NATIVE &&
        binding_status->gbuffer_external_binding->need_send_data == 1) {
        binding_status->gbuffer_external_binding->need_send_data = 0;

        BindingStatus *bind_status = &(opengl_context->context_binding_status);
        GLSLProgram *now_program =
            opengl_context->program_info_map[0][bind_status->current_program];

        ALOGE("send_buffer_data with now_program %d (%d)",
              bind_status->current_program, now_program->is_external);
        if (now_program != NULL && now_program->is_external == 1) {
            upload_graphic_buffer(opengl_context,
                                  binding_status->gbuffer_external_binding);
        }
    }
}

void d_glGenVertexArrays_special(void *context, GLsizei n, GLuint *arrays) {
    r_glGenVertexArrays(context, n, arrays);
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    for (int i = 0; i < n; i++) {
        ALOGD("gen vao %u", arrays[i]);
        AttribPoint *point_data;
        point_data = new AttribPoint;
        memset(point_data, 0, sizeof(AttribPoint));
        for (int i = 0; i < MAX_VERTEX_ATTRIBS_NUM; i++) {
            point_data->invoke_type[i] = GL_FLOAT;
            point_data->type[i] = GL_FLOAT;
            point_data->size[i] = 4;
            point_data->bindingindex[i] = i;
        }
        for (int i = 0; i < MAX_VERTEX_ATTRIB_BINDINGS_NUM; i++) {
            point_data->vertex_binding_stride[i] = 16;
        }
        bound_buffer->vao_point_data[arrays[i]] = point_data;
    }
}

void d_glBindVertexArray_special(void *context, GLuint array) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->vertex_array_resource()->has_id(array)) {
        
        if (array > res_ctx->vertex_array_resource()->get_max_id() * 2 &&
            array > 10000) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
        res_ctx->vertex_array_resource()->set_id(array);
        
        d_glGenVertexArrays_special(context, 1, &array);
        ALOGD("gen vao when glBindVertexArray %u", array);
    }

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;

    ALOGD("glBindVertexArray %u", array);

    bound_buffer->attrib_point->element_array_buffer_binding =
        bound_buffer->buffer_status->element_array_buffer;

    if (bound_buffer->attrib_point != bound_buffer->vao_point_data[array]) {
       ((GLThreadContext *)context)->buffer_ptr->buffer_layout_maps.clear(); 
    }
    bound_buffer->attrib_point = bound_buffer->vao_point_data[array];

    bound_buffer->buffer_status->element_array_buffer =
        bound_buffer->attrib_point->element_array_buffer_binding;

    if (bound_buffer->now_vao != array) {
        bound_buffer->now_vao = array;
        r_glBindVertexArray_special(context, array);
    }
}

/**
 * @brief 
 *
 * @param context
 * @param id
 */
void d_glDeleteVertexArrays_special(void *context, GLsizei n,
                                    const GLuint *arrays) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;

    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    for (int i = 0; i < n; i++) {
        if (arrays[i] == 0) {
            
            continue;
        }
        if (bound_buffer->buffer_status->vertex_array_buffer == arrays[i]) {
            

            bound_buffer->attrib_point = bound_buffer->vao_point_data[0];
            bound_buffer->buffer_status->vertex_array_buffer =
                bound_buffer->attrib_point->element_array_buffer_binding;
            // ALOGD("delete vao back to 0 vao, vbo ebo %u
            // %u",bound_buffer->buffer_status->array_buffer,bound_buffer->buffer_status->element_array_buffer);
        }

        if (bound_buffer->vao_point_data[arrays[i]] != NULL) {
            
            delete bound_buffer->vao_point_data[arrays[i]];
        }
        bound_buffer->vao_point_data.erase(arrays[i]);
    }

    r_glDeleteVertexArrays_origin(context, n, arrays);
}

void d_glGenVertexArraysOES(void *context, GLsizei n, GLuint *arrays) {
    d_glGenVertexArrays(context, n, arrays);
}

void d_glBindVertexArrayOES(void *context, GLuint array) {
    d_glBindVertexArray_special(context, array);
}

void d_glDeleteVertexArraysOES(void *context, GLsizei n, const GLuint *arrays) {
    d_glDeleteVertexArrays(context, n, arrays);
}

void d_glDrawArrays(void *context, GLenum mode, GLint first, GLsizei count) {
    CHECK_VALID_ENUM(__LINE__, mode, 7, GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
                     GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
                     GL_TRIANGLES);
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to an enabled array and the buffer object's data store is currently
     * mapped*/
    // CHECK_DRAW_FRAMEBUFFER_COMPLETE(GL_INVALID_FRAMEBUFFER_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if recording the vertices of a
     * primitive to the buffer objects being used for transform feedback
     * purposes would result in either exceeding the limits of any buffer
     * object\u2019s size, or in exceeding the end position offset + size - 1,
     * as set by glBindBufferRange.*/
    /*TODO GL_INVALID_OPERATION is generated if a geometry shader is active and
     * mode is incompatible with the input primitive type of the geometry shader
     * in the currently installed program object.*/

    send_gbuffer_data(context);

    ALOGD("glDrawArrays mode %x first %d count %d", mode, first, count);
    send_point_attrib_data(context, first, first + count - 1, -1);
    VertexMemLayout::draw_arrays(context, count);
    r_glDrawArrays_origin(context, mode, first, count);
}

void d_glDrawArraysInstanced(void *context, GLenum mode, GLint first,
                             GLsizei count, GLsizei instancecount) {
    CHECK_VALID_ENUM(__LINE__, mode, 7, GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
                     GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    CHECK_NEGATIVE(__LINE__, instancecount, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to an enabled array and the buffer object's data store is currently
     * mapped.*/
    CHECK_DRAW_FRAMEBUFFER_COMPLETE(GL_INVALID_FRAMEBUFFER_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if recording the vertices of a
     * primitive to the buffer objects being used for transform feedback
     * purposes would result in either exceeding the limits of any buffer
     * object\u2019s size, or in exceeding the end position offset + size - 1,
     * as set by glBindBufferRange.*/
    /*TODO GL_INVALID_OPERATION is generated if a geometry shader is active and
     * mode is incompatible with the input primitive type of the geometry shader
     * in the currently installed program object.*/

    send_gbuffer_data(context);

    send_point_attrib_data(context, first, first + count - 1, instancecount);
    VertexMemLayout::draw_arrays(context, count);
    r_glDrawArraysInstanced_origin(context, mode, first, count, instancecount);
}

void d_glDrawElements(void *context, GLenum mode, GLsizei count, GLenum type,
                      const void *indices) {
    CHECK_VALID_ENUM(__LINE__, mode, 7, GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
                     GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to an enabled array or the element array and the buffer object's
     * data store is currently mapped.*/
    CHECK_DRAW_FRAMEBUFFER_COMPLETE(GL_INVALID_FRAMEBUFFER_OPERATION)
    CHECK_TRANSFORM_FEEDBACK_ACTIVE_BUT_NOT_PAUSED(GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if a geometry shader is active and
     * mode is incompatible with the input primitive type of the geometry shader
     * in the currently installed program object.*/

    send_gbuffer_data(context);

    send_point_attrib_data_indices(context, count, type, indices, -1);
    
    if (get_bound_buffer(context, GL_ELEMENT_ARRAY_BUFFER) == 0) {
        ALOGD(
            "glDrawElements_without_bound mode %x count %d type %x indices "
            "%llx",
            mode, (int)count, type, (long long)indices);
        
        r_glDrawElements_without_bound(context, mode, count, type, indices);
    } else {
        ALOGD("glDrawElements_bound mode %x count %d type %x indices %llx",
              mode, (int)count, type, (long long)indices);
        VertexMemLayout::draw_elements(context, count, type, indices);
        r_glDrawElements_with_bound(context, mode, count, type,
                                    (GLsizeiptr)indices);
    }
}

void d_glDrawRangeElements(void *context, GLenum mode, GLuint start, GLuint end,
                           GLsizei count, GLenum type, const void *indices) {
    

    CHECK_VALID_ENUM(__LINE__, mode, 7, GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
                     GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    if (end < start) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to an enabled array or the element array and the buffer object's
     * data store is currently mapped.*/
    CHECK_DRAW_FRAMEBUFFER_COMPLETE(GL_INVALID_FRAMEBUFFER_OPERATION)
    CHECK_TRANSFORM_FEEDBACK_ACTIVE_BUT_NOT_PAUSED(GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if a geometry shader is active and
     * mode is incompatible with the input primitive type of the geometry shader
     * in the currently installed program object.*/

    send_gbuffer_data(context);

    send_point_attrib_data_indices(context, count, type, indices, -1);
    if (get_bound_buffer(context, GL_ELEMENT_ARRAY_BUFFER) == 0) {
        ALOGD(
            "glDrawRangeElements_without_bound mode %x count %d type %x "
            "indices %llx start %d end %d",
            mode, (int)count, type, (long long)indices, start, end);
        r_glDrawRangeElements_without_bound(context, mode, start, end, count,
                                            type, indices);
    } else {
        ALOGD(
            "glDrawRangeElements_with_bound mode %x count %d type %x indices "
            "%llx start %d end %d",
            mode, (int)count, type, (long long)indices, start, end);
        VertexMemLayout::draw_elements(context, count, type, indices);
        r_glDrawRangeElements_with_bound(context, mode, start, end, count, type,
                                         (GLsizeiptr)indices);
    }
}

void d_glDrawElementsInstanced(void *context, GLenum mode, GLsizei count,
                               GLenum type, const void *indices,
                               GLsizei instancecount) {
    CHECK_VALID_ENUM(__LINE__, mode, 7, GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
                     GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES)
    CHECK_NEGATIVE(__LINE__, instancecount < 0 || count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to an enabled array or the element array and the buffer object's
     * data store is currently mapped.*/
    CHECK_DRAW_FRAMEBUFFER_COMPLETE(GL_INVALID_FRAMEBUFFER_OPERATION)
    CHECK_TRANSFORM_FEEDBACK_ACTIVE_BUT_NOT_PAUSED(GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if a geometry shader is active and
     * mode is incompatible with the input primitive type of the geometry shader
     * in the currently installed program object.*/

    send_gbuffer_data(context);

    send_point_attrib_data_indices(context, count, type, indices,
                                   instancecount);
    if (get_bound_buffer(context, GL_ELEMENT_ARRAY_BUFFER) == 0) {
        r_glDrawElementsInstanced_without_bound(context, mode, count, type,
                                                indices, instancecount);
    } else {
        VertexMemLayout::draw_elements(context, count, type, indices);
        r_glDrawElementsInstanced_with_bound(
            context, mode, count, type, (GLsizeiptr)indices, instancecount);
    }
}

void init_vertex_attrib_offset(void *context, GLuint index, GLint size,
                               GLenum type, GLboolean normalized,
                               GLsizei stride, GLenum invoke_type,
                               const void *pointer) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    if (index > MAX_VERTEX_ATTRIBS_NUM) {
        ALOGE("%s Error! index %d > MAX_VERTEX_ATTRIBS_NUM", __FUNCTION__,
              index);
        return;
    }

    AttribPoint *point_data = bound_buffer->attrib_point;
    if (point_data == nullptr) {
        point_data = new AttribPoint;
        bound_buffer->vao_point_data[status->vertex_array_buffer] = point_data;
        bound_buffer->attrib_point = point_data;
        memset(point_data, 0, sizeof(AttribPoint));
        for (int i = 0; i < MAX_VERTEX_ATTRIBS_NUM; i++) {
            point_data->invoke_type[i] = GL_FLOAT;
            point_data->type[i] = GL_FLOAT;
            point_data->size[i] = 4;
            point_data->bindingindex[i] = i;
        }
        for (int i = 0; i < MAX_VERTEX_ATTRIB_BINDINGS_NUM; i++) {
            point_data->vertex_binding_stride[i] = 16;
        }
        ALOGD("create vao point data %u", status->vertex_array_buffer);
    }

    point_data->location[index] = 0;

    point_data->array_buffer_binding[index] =
        bound_buffer->buffer_status->array_buffer;

    point_data->size[index] = size;
    point_data->type[index] = type;
    point_data->normalized[index] = normalized;

    point_data->stride[index] = stride;
    if (stride == 0) {
        point_data->stride[index] =
            point_data->size[index] * gl_sizeof(point_data->type[index]);
        if (point_data->type[index] == GL_INT_2_10_10_10_REV ||
            point_data->type[index] == GL_UNSIGNED_INT_2_10_10_10_REV) {
            point_data->stride[index] /= 4;
        }
    }
    point_data->data[index] = (char *)pointer;
    point_data->invoke_type[index] = invoke_type;
    point_data->has_update = GL_TRUE;

    VertexMemLayout *vertex_mem_layout =
        VertexMemLayout::get_vertex_mem_layout_with_id(
            context, bound_buffer->buffer_status->array_buffer);
    vertex_mem_layout->set_layout(point_data, index);
}

void init_vertex_attrib_pointer(void *context, GLuint index, GLint size,
                                GLenum type, GLboolean normalized,
                                GLsizei stride, GLenum invoke_type,
                                const void *pointer) {
    
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    if (index > MAX_VERTEX_ATTRIBS_NUM) {
        // error
        return;
    }

    AttribPoint *point_data = bound_buffer->attrib_point;
    if (point_data == NULL) {
        point_data = new AttribPoint;
        bound_buffer->vao_point_data[status->vertex_array_buffer] = point_data;
        bound_buffer->attrib_point = point_data;
        memset(point_data, 0, sizeof(AttribPoint));
        for (int i = 0; i < MAX_VERTEX_ATTRIBS_NUM; i++) {
            point_data->invoke_type[i] = GL_FLOAT;
            point_data->type[i] = GL_FLOAT;
            point_data->size[i] = 4;
            point_data->bindingindex[i] = i;
        }
        for (int i = 0; i < MAX_VERTEX_ATTRIB_BINDINGS_NUM; i++) {
            point_data->vertex_binding_stride[i] = 16;
        }
    }

    //    point_data->data_has_change[index]=GL_FALSE;

    //    int has_change=0;
    if (point_data->data[index] != (char *)pointer) {
        point_data->data[index] = (char *)pointer;
        //        point_data->data_has_change[index]=GL_TRUE;
        point_data->has_update = 1;
    }

    
    //    if(point_data->data_has_change[index]==GL_TRUE){
    //        for (int i = 0; i < MAX_VERTEX_ATTRIBS_NUM; i++) {
    //            if (point_data->location[i] == 0 || i == index ) {
    //                continue;
    //            }
    //            point_data->data_dis[i][index] = llabs((uint64_t)((char *)
    //            point_data->data[i] - (char *) pointer));
    //            point_data->data_dis[index][i] =
    //            point_data->data_dis[i][index];
    //
    
    //            if (point_data->data_dis[i][index] < stride &&
    //            point_data->stride[i] == stride) {
    
    //                if (point_data->data[i] > pointer) {
    //                    point_data->data_index_father[i] = index;
    //                } else {
    //                    if (point_data->data_index_father[i] == i) {
    
    //                        point_data->data_index_father[index] = i;
    //                    }
    //                }
    //            }
    //        }
    //    }

    if (point_data->location[index] != 1) {
        point_data->location[index] = 1;
        point_data->has_update = 1;
    }
    if (point_data->size[index] != size) {
        point_data->size[index] = size;
        point_data->has_update = 1;
    }
    if (point_data->type[index] != type) {
        point_data->type[index] = type;
        point_data->has_update = 1;
    }
    if (point_data->normalized[index] != normalized) {
        point_data->normalized[index] = normalized;
        point_data->has_update = 1;
    }

    
    if (stride == 0) {
        stride = point_data->size[index] * gl_sizeof(point_data->type[index]);
        if (point_data->type[index] == GL_INT_2_10_10_10_REV ||
            point_data->type[index] == GL_UNSIGNED_INT_2_10_10_10_REV) {
            stride /= 4;
        }
    }
    if (point_data->stride[index] != stride) {
        point_data->stride[index] = stride;
        point_data->has_update = 1;
    }

    if (point_data->invoke_type[index] != invoke_type) {
        point_data->invoke_type[index] = invoke_type;
        point_data->has_update = 1;
    }

    if (index > point_data->max_index) {
        point_data->max_index = index;
    }

    return;
}

void d_glVertexAttribPointer(void *context, GLuint index, GLint size,
                             GLenum type, GLboolean normalized, GLsizei stride,
                             const void *pointer) {
    ALOGD("d_glVertexAttribPointer %u %d %x %d %d pointer %llx", index, size,
          type, normalized, stride, (long long)pointer);
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    if (size != 1 && size != 2 && size != 3 && size != 4) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    CHECK_VALID_ENUM(__LINE__, type, 12, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
                     GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_HALF_FLOAT,
                     GL_FLOAT, GL_FIXED, GL_HALF_FLOAT_OES,
                     GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV)
    CHECK_NEGATIVE(__LINE__, stride, GL_INVALID_VALUE)
    if (size != 4 && (type == GL_INT_2_10_10_10_REV ||
                      type == GL_UNSIGNED_INT_2_10_10_10_REV)) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    AttribPoint *point_data = bound_buffer->attrib_point;

    ALOGD(
        "glVertexAttribPointer index %d(enable %d) size %d stride %d pointer "
        "%llu",
        index, point_data->enabled[index], size, stride,
        (unsigned long long)pointer);
    if (get_bound_buffer(context, GL_ARRAY_BUFFER) == 0) {
        
        init_vertex_attrib_pointer(context, index, size, type, normalized,
                                   stride, GL_FLOAT, pointer);
    } else {
        init_vertex_attrib_offset(context, index, size, type, normalized,
                                  stride, GL_FLOAT, pointer);
        
        r_glVertexAttribPointer_with_bound(
            context, index, size, type, normalized, stride, (GLintptr)pointer);
    }
}

void d_glVertexAttribIPointer(void *context, GLuint index, GLint size,
                              GLenum type, GLsizei stride,
                              const void *pointer) {
    ALOGD("d_glVertexAttribIPointer %u %d %x %d pointer %llx", index, size,
          type, stride, (long long)pointer);

    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    if (size != 1 && size != 2 && size != 3 && size != 4) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    CHECK_VALID_ENUM(__LINE__, type, 6, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT,
                     GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT)
    CHECK_NEGATIVE(__LINE__, stride, GL_INVALID_VALUE)

    
    if (type == GL_HALF_FLOAT || type == GL_FLOAT || type == GL_FIXED ||
        type == GL_INT_2_10_10_10_REV ||
        type == GL_UNSIGNED_INT_2_10_10_10_REV) {
        
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    if (get_bound_buffer(context, GL_ARRAY_BUFFER) == 0) {
        
        init_vertex_attrib_pointer(context, index, size, type, GL_FALSE, stride,
                                   GL_INT, pointer);
    } else {
        init_vertex_attrib_offset(context, index, size, type, GL_FALSE, stride,
                                  GL_INT, pointer);
        
        r_glVertexAttribIPointer_with_bound(context, index, size, type, stride,
                                            (GLintptr)pointer);
    }
}

void d_glGetVertexAttribPointerv(void *context, GLuint index, GLenum pname,
                                 void **pointer) {
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    CHECK_VALID_ENUM(__LINE__, pname, 1, GL_VERTEX_ATTRIB_ARRAY_POINTER)

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    AttribPoint *point_data = bound_buffer->attrib_point;
    *pointer = (void *)point_data->data[index];

    // if(get_bound_buffer(context, GL_ARRAY_BUFFER) == 0){
    //     BoundBuffer *bound_buffer = ((GLThreadContext
    //     *)context)->context_binding_status.bound_buffer; Buffer_Status
    //     *status = bound_buffer->buffer_status; AttribPoint *point_data =
    //     bound_buffer->vao_point_data[status->vertex_array_buffer];
    //     *pointer=(void *)point_data->data[index];
    // }else{
    //     d_glGetVertexAttribPointerv_with_bound(context,index,pname,(GLintptr
    //     *)pointer);
    // }
}

int get_vertex_attrib_float(void *context, GLuint index, GLenum pname,
                            GLfloat *param) {
    GLint param_int;
    int ret = get_vertex_attrib(context, index, pname, &param_int);
    if (ret == 1) {
        *param = (GLfloat)param_int;
    }
    return ret;
}

int get_vertex_attrib(void *context, GLuint index, GLenum pname, GLint *param) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    AttribPoint *point_data = bound_buffer->attrib_point;
    switch (pname) {
        
        case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
            *((GLint *)param) = (GLint)point_data->enabled[index];
            break;
        case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
            *((GLuint *)param) =
                (GLuint)point_data->array_buffer_binding[index];
            break;
        case GL_VERTEX_ATTRIB_ARRAY_INTEGER:
            if (point_data->invoke_type[index] == GL_INT) {
                *((GLint *)param) = 1;
            } else {
                *((GLint *)param) = 0;
            }
            break;
        case GL_VERTEX_ATTRIB_ARRAY_SIZE:
            *((GLint *)param) = (GLint)point_data->size[index];
            break;
        case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
            *((GLint *)param) = (GLint)point_data->stride[index];
            break;
        case GL_VERTEX_ATTRIB_ARRAY_TYPE:
            *((GLenum *)param) = (GLenum)point_data->type[index];
            break;
        case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED:
            *((GLint *)param) = (GLint)point_data->normalized[index];
            break;
        case GL_VERTEX_ATTRIB_ARRAY_DIVISOR:
            *((GLuint *)param) = (GLuint)point_data->divisor[index];
            break;

            
        case GL_CURRENT_VERTEX_ATTRIB:
            return 0;
            break;
        case GL_VERTEX_ATTRIB_BINDING:
            *((GLuint *)param) = (GLuint)point_data->bindingindex[index];
            break;
        case GL_VERTEX_ATTRIB_RELATIVE_OFFSET:
            *((GLuint *)param) = (GLuint)point_data->relativeoffset[index];
            break;
        default:
            
            ALOGE("get_vertex_attrib error pname %x", pname);
            return 0;
            break;
    }
    return 1;
}

GLintptr get_vertex_binding_status(void *context, GLenum target, GLuint index) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    AttribPoint *point_data = bound_buffer->attrib_point;
    switch (target) {
        case GL_VERTEX_BINDING_BUFFER:
            return (GLintptr)point_data->vertex_binding_buffer[index];
            break;
        case GL_VERTEX_BINDING_DIVISOR:
            return (GLintptr)point_data->vertex_binding_divisor[index];
            break;
        case GL_VERTEX_BINDING_STRIDE:
            return (GLintptr)point_data->vertex_binding_stride[index];
            break;
        case GL_VERTEX_BINDING_OFFSET:
            return point_data->vertex_binding_offset[index];
            break;
        default:
            ALOGE("get_vertex_binding_status error target %x", target);
            break;
    }
    return (GLintptr)0;
}

void d_glGetVertexAttribIiv(void *context, GLuint index, GLenum pname,
                            GLint *params) {
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    CHECK_VALID_ENUM(
        __LINE__, pname, 10, GL_CURRENT_VERTEX_ATTRIB,
        GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, GL_VERTEX_ATTRIB_ARRAY_ENABLED,
        GL_VERTEX_ATTRIB_ARRAY_SIZE, GL_VERTEX_ATTRIB_ARRAY_STRIDE,
        GL_VERTEX_ATTRIB_ARRAY_TYPE, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
        GL_VERTEX_ATTRIB_ARRAY_INTEGER, GL_VERTEX_ATTRIB_ARRAY_DIVISOR,
        GL_VERTEX_ATTRIB_BINDING)

    if (!get_vertex_attrib(context, index, pname, params)) {
        r_glGetVertexAttribIiv(context, index, pname, params);
    }
}

void d_glGetVertexAttribIuiv(void *context, GLuint index, GLenum pname,
                             GLuint *params) {
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    CHECK_VALID_ENUM(
        __LINE__, pname, 10, GL_CURRENT_VERTEX_ATTRIB,
        GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, GL_VERTEX_ATTRIB_ARRAY_ENABLED,
        GL_VERTEX_ATTRIB_ARRAY_SIZE, GL_VERTEX_ATTRIB_ARRAY_STRIDE,
        GL_VERTEX_ATTRIB_ARRAY_TYPE, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
        GL_VERTEX_ATTRIB_ARRAY_INTEGER, GL_VERTEX_ATTRIB_ARRAY_DIVISOR,
        GL_VERTEX_ATTRIB_BINDING)

    if (!get_vertex_attrib(context, index, pname, (GLint *)params)) {
        r_glGetVertexAttribIuiv(context, index, pname, params);
    }
}

void d_glGetVertexAttribfv(void *context, GLuint index, GLenum pname,
                           GLfloat *params) {
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    CHECK_VALID_ENUM(
        __LINE__, pname, 10, GL_CURRENT_VERTEX_ATTRIB,
        GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, GL_VERTEX_ATTRIB_ARRAY_ENABLED,
        GL_VERTEX_ATTRIB_ARRAY_SIZE, GL_VERTEX_ATTRIB_ARRAY_STRIDE,
        GL_VERTEX_ATTRIB_ARRAY_TYPE, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
        GL_VERTEX_ATTRIB_ARRAY_INTEGER, GL_VERTEX_ATTRIB_ARRAY_DIVISOR,
        GL_VERTEX_ATTRIB_BINDING)

    if (!get_vertex_attrib_float(context, index, pname, params)) {
        r_glGetVertexAttribfv(context, index, pname, params);
    }
}

void d_glGetVertexAttribiv(void *context, GLuint index, GLenum pname,
                           GLint *params) {
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    CHECK_VALID_ENUM(
        __LINE__, pname, 10, GL_CURRENT_VERTEX_ATTRIB,
        GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, GL_VERTEX_ATTRIB_ARRAY_ENABLED,
        GL_VERTEX_ATTRIB_ARRAY_SIZE, GL_VERTEX_ATTRIB_ARRAY_STRIDE,
        GL_VERTEX_ATTRIB_ARRAY_TYPE, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED,
        GL_VERTEX_ATTRIB_ARRAY_INTEGER, GL_VERTEX_ATTRIB_ARRAY_DIVISOR,
        GL_VERTEX_ATTRIB_BINDING)

    if (!get_vertex_attrib(context, index, pname, params)) {
        r_glGetVertexAttribiv(context, index, pname, params);
    }
}

void d_glDisableVertexAttribArray(void *context, GLuint index) {
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE);

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    AttribPoint *point_data = bound_buffer->attrib_point;
    if (point_data->enabled[index] == 0) {
        return;
    }
    point_data->enabled[index] = 0;
    ALOGD("glDisableVertexAttribArray %u", index);
    r_glDisableVertexAttribArray_origin(context, index);
}

void d_glEnableVertexAttribArray(void *context, GLuint index) {
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE);

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    AttribPoint *point_data = bound_buffer->attrib_point;
    if (point_data->enabled[index] == 1) {
        return;
    }
    point_data->enabled[index] = 1;
    ALOGD("glEnableVertexAttribArray %u", index);
    r_glEnableVertexAttribArray_origin(context, index);
}

void d_glVertexAttribDivisor(void *context, GLuint index, GLuint divisor) {
    CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;

    AttribPoint *point_data = bound_buffer->attrib_point;

    point_data->divisor[index] = divisor;
    r_glVertexAttribDivisor_origin(context, index, divisor);
}




void d_glBindVertexBuffer_special(void *context, GLuint bindingindex,
                                  GLuint buffer, GLintptr offset,
                                  GLsizei stride) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;

    AttribPoint *point_data = bound_buffer->attrib_point;

    point_data->vertex_binding_buffer[bindingindex] = buffer;
    point_data->vertex_binding_offset[bindingindex] = offset;
    point_data->vertex_binding_stride[bindingindex] = stride;

    r_glBindVertexBuffer(context, bindingindex, buffer, offset, stride);
}

void d_glVertexAttribFormat_special(void *context, GLuint attribindex,
                                    GLint size, GLenum type,
                                    GLboolean normalized,
                                    GLuint relativeoffset) {
    CHECK_INT_MAX(__LINE__, attribindex, GL_MAX_VERTEX_ATTRIBS,
                  GL_INVALID_VALUE)
    // GL_INVALID_OPERATION is generated if no vertex array object is bound.

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;

    AttribPoint *point_data = bound_buffer->attrib_point;

    // point_data->has_update = GL_FALSE;
    point_data->location[attribindex] = 0;
    point_data->size[attribindex] = size;
    point_data->type[attribindex] = type;
    point_data->normalized[attribindex] = normalized;
    point_data->relativeoffset[attribindex] = relativeoffset;
    point_data->invoke_type[attribindex] = GL_FLOAT;

    r_glVertexAttribFormat(context, attribindex, size, type, normalized,
                           relativeoffset);
}

void d_glVertexAttribIFormat_special(void *context, GLuint attribindex,
                                     GLint size, GLenum type,
                                     GLuint relativeoffset) {
    CHECK_INT_MAX(__LINE__, attribindex, GL_MAX_VERTEX_ATTRIBS,
                  GL_INVALID_VALUE)
    // GL_INVALID_OPERATION is generated if no vertex array object is bound.

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;

    AttribPoint *point_data = bound_buffer->attrib_point;

    // point_data->has_update = GL_FALSE;
    point_data->location[attribindex] = 0;
    point_data->size[attribindex] = size;
    point_data->type[attribindex] = type;
    point_data->normalized[attribindex] = GL_FALSE;
    ;
    point_data->relativeoffset[attribindex] = relativeoffset;
    point_data->invoke_type[attribindex] = GL_INT;

    r_glVertexAttribIFormat(context, attribindex, size, type, relativeoffset);
}

void d_glVertexAttribBinding_special(void *context, GLuint attribindex,
                                     GLuint bindingindex) {
    CHECK_INT_MAX(__LINE__, attribindex, GL_MAX_VERTEX_ATTRIBS,
                  GL_INVALID_VALUE);
    CHECK_INT_MAX(__LINE__, bindingindex, GL_MAX_VERTEX_ATTRIB_BINDINGS,
                  GL_INVALID_VALUE)
    // GL_INVALID_OPERATION is generated if no vertex array object is bound.

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;

    AttribPoint *point_data = bound_buffer->attrib_point;

    point_data->bindingindex[attribindex] = bindingindex;

    r_glVertexAttribBinding(context, attribindex, bindingindex);
}

void d_glVertexBindingDivisor_special(void *context, GLuint bindingindex,
                                      GLuint divisor) {
    CHECK_INT_MAX(__LINE__, bindingindex, GL_MAX_VERTEX_ATTRIB_BINDINGS,
                  GL_INVALID_VALUE)
    // GL_INVALID_OPERATION is generated if no vertex array object is bound.

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;

    AttribPoint *point_data = bound_buffer->attrib_point;

    point_data->vertex_binding_divisor[bindingindex] = divisor;

    r_glVertexBindingDivisor(context, bindingindex, divisor);
}

void d_glDrawArraysIndirect_special(void *context, GLenum mode,
                                    const void *indirect) {
    send_gbuffer_data(context);

    GLuint current_indirect_buffer =
        get_bound_buffer(context, GL_DRAW_INDIRECT_BUFFER);
    if (current_indirect_buffer == 0) {
        const DrawArraysIndirectCommand *cmd =
            (const DrawArraysIndirectCommand *)indirect;
        send_point_attrib_data(context, cmd->first, cmd->first + cmd->count - 1,
                               cmd->primCount);
        r_glDrawArraysIndirect_without_bound(context, mode, indirect);
    } else {
        r_glDrawArraysIndirect_with_bound(context, mode, (GLintptr)indirect);
    }
}

void d_glDrawElementsIndirect_special(void *context, GLenum mode, GLenum type,
                                      const void *indirect) {
    send_gbuffer_data(context);

    GLuint current_indirect_buffer =
        get_bound_buffer(context, GL_DRAW_INDIRECT_BUFFER);
    if (current_indirect_buffer == 0) {
        const DrawElementsIndirectCommand *cmd =
            (const DrawElementsIndirectCommand *)indirect;
        send_point_attrib_data_indices(
            context, cmd->count, type,
            (void *)(cmd->firstIndex * gl_sizeof(type)), cmd->instanceCount);
        r_glDrawElementsIndirect_without_bound(context, mode, type, indirect);
    } else {
        r_glDrawElementsIndirect_with_bound(context, mode, type,
                                            (GLintptr)indirect);
    }
}