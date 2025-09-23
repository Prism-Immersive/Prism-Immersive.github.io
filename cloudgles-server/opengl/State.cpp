#define LOG_TAG "GLState"
#include "State.h"

#include <GLES3/gl32.h>

#include "ErrorHandler.h"
#include "GLDef.h"
#include "Log.h"
#include "ThreadContext.h"
#include "opengl/Memory.h"

BoundBuffer::BoundBuffer() {
    
    ebo_mm_num = 4000;
    now_vao = 0;

    ebo_mm = new GLuint[4000 + 1];
    memset(ebo_mm, 0, sizeof(GLuint) * (4000 + 1));
    for (unsigned int i = 2; i < 4000 + 1; i++) {
        ebo_mm[i] = ((i & (i - 1)) == 0) ? ebo_mm[i - 1] + 1 : ebo_mm[i - 1];
    }

    buffer_status = new BufferStatus;
    memset(buffer_status, 0, sizeof(BufferStatus));

    attrib_point = new AttribPoint;
    memset(attrib_point, 0, sizeof(AttribPoint));
    for (int i = 0; i < MAX_VERTEX_ATTRIBS_NUM; i++) {
        attrib_point->invoke_type[i] = GL_FLOAT;
        attrib_point->type[i] = GL_FLOAT;
        attrib_point->size[i] = 4;
        attrib_point->bindingindex[i] = i;
    }
    for (unsigned int &i : attrib_point->vertex_binding_stride) {
        i = 16;
    }
    vao_point_data[0] = attrib_point;
}

BoundBuffer::~BoundBuffer() {
    delete[] ebo_mm;
    delete buffer_status;

    auto vao_iterator = vao_point_data.begin();
    while (vao_iterator != vao_point_data.end()) {
        delete vao_iterator->second;
        vao_iterator++;
    }

    auto ebo_iterator = ebo_buffer.begin();
    while (ebo_iterator != ebo_buffer.end()) {
        ElementArrayBuffer *ebo_data = ebo_iterator->second;
        if (ebo_data != nullptr) {
            if (ebo_data->dp_max != nullptr) {
                for (int i = 1; i <= ebo_mm[ebo_data->dp_num]; i++) {
                    delete[] ebo_data->dp_max[i];
                    delete[] ebo_data->dp_min[i];
                }
                delete[] ebo_data->dp_max;
                delete[] ebo_data->dp_min;
            }
            delete[] ebo_data;
        }
        ebo_iterator++;
    }
}

/**
 * @brief 
 *
 * @param context 
 * @param target buffer
 * @return GLuint buffer
 */
GLuint get_bound_buffer(void *context, const GLenum &target) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    switch (target) {
        case GL_ARRAY_BUFFER:
            return status->array_buffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            return status->element_array_buffer;
            break;
        case GL_COPY_READ_BUFFER:
            return status->copy_read_buffer;
            break;
        case GL_COPY_WRITE_BUFFER:
            return status->copy_write_buffer;
            break;
        case GL_PIXEL_PACK_BUFFER:
            return status->pixel_pack_buffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER:
            return status->pixel_unpack_buffer;
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            return status->transform_feedback_buffer;
            break;
        case GL_UNIFORM_BUFFER:
            return status->uniform_buffer;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            return status->atomic_counter_buffer;
            break;
        case GL_DISPATCH_INDIRECT_BUFFER:
            return status->dispatch_indirect_buffer;
            break;
        case GL_DRAW_INDIRECT_BUFFER:
            return status->draw_indirect_buffer;
            break;
        case GL_SHADER_STORAGE_BUFFER:
            return status->shader_storage_buffer;
            break;
        case GL_TEXTURE_BUFFER:
            return status->texture_buffer;
        default:
            ALOGE("%s target %x error", __FUNCTION__, target);
            break;
    }
    return 0;
}

GLuint get_index_binding_buffer(void *context, const GLenum &target,
                                GLuint index) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);

    switch (target) {
        case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING:
            return bind_status->feedback_buffer_binding_index[index];
        case GL_UNIFORM_BUFFER_BINDING:
            return bind_status->uniform_buffer_binding_index[index];
        case GL_ATOMIC_COUNTER_BUFFER_BINDING:
            return bind_status->atomic_counter_buffer_binding_index[index];
        case GL_SHADER_STORAGE_BUFFER_BINDING:
            return bind_status->shader_storage_buffer_binding_index[index];
        default:
            ALOGE("%s target %x error", __FUNCTION__, target);
            return 0;
    }
}

GLuint get_binging_status(void *context, const GLenum &target) {
    BindingStatus *binding_status =
        &(((GLThreadContext *)context)->context_binding_status);
    switch (target) {
        case GL_DRAW_FRAMEBUFFER_BINDING:
            return binding_status->draw_frame_buffer;
        case GL_READ_FRAMEBUFFER_BINDING:
            return binding_status->read_frame_buffer;
        case GL_RENDERBUFFER_BINDING:
            return binding_status->render_buffer;
        case GL_TEXTURE_BINDING_3D:
            return binding_status
                ->texture_binding[binding_status->active_texture]
                .texture_binding_3d;
        case GL_TEXTURE_BINDING_2D:
            return binding_status
                ->texture_binding[binding_status->active_texture]
                .texture_binding_2d;
        case GL_TEXTURE_BINDING_2D_ARRAY:
            return binding_status
                ->texture_binding[binding_status->active_texture]
                .texture_binding_2d_array;
        case GL_TEXTURE_BINDING_CUBE_MAP:
            return binding_status
                ->texture_binding[binding_status->active_texture]
                .texture_binding_cube_map;
        case GL_TEXTURE_BINDING_2D_MULTISAMPLE:
            return binding_status
                ->texture_binding[binding_status->active_texture]
                .texture_binding_2d_multisample;
        case GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY:
            return binding_status
                ->texture_binding[binding_status->active_texture]
                .texture_binding_2d_multisample_array;
        case GL_TEXTURE_BINDING_CUBE_MAP_ARRAY:
            return binding_status
                ->texture_binding[binding_status->active_texture]
                .texture_binding_cube_map_array;
        case GL_TEXTURE_BINDING_BUFFER:
            return binding_status
                ->texture_binding[binding_status->active_texture]
                .texture_binding_texture_buffer;
        case GL_TEXTURE_BINDING_EXTERNAL_OES:
            return binding_status->texture_binding[0].texture_binding_external;
        case GL_ACTIVE_TEXTURE:
            return binding_status->active_texture + GL_TEXTURE0;
        case GL_SAMPLER_BINDING:  // return the current binding sampler by
                                  // texture id binding
            return binding_status
                ->sampler_binding_2d[binding_status->active_texture];
        case GL_TRANSFORM_FEEDBACK_BINDING:
            return binding_status->transform_feedback_binding;
        case GL_DRAW_BUFFER0:
            return binding_status->drawbuffers[0];
        case GL_DRAW_BUFFER1:
            return binding_status->drawbuffers[1];
        case GL_DRAW_BUFFER2:
            return binding_status->drawbuffers[2];
        case GL_DRAW_BUFFER3:
            return binding_status->drawbuffers[3];
        case GL_DRAW_BUFFER4:
            return binding_status->drawbuffers[4];
        case GL_DRAW_BUFFER5:
            return binding_status->drawbuffers[5];
        case GL_DRAW_BUFFER6:
            return binding_status->drawbuffers[6];
        case GL_DRAW_BUFFER7:
            return binding_status->drawbuffers[7];
        case GL_DRAW_BUFFER8:
            return binding_status->drawbuffers[8];
        case GL_DRAW_BUFFER9:
            return binding_status->drawbuffers[9];
        case GL_DRAW_BUFFER10:
            return binding_status->drawbuffers[10];
        case GL_DRAW_BUFFER11:
            return binding_status->drawbuffers[11];
        case GL_DRAW_BUFFER12:
            return binding_status->drawbuffers[12];
        case GL_DRAW_BUFFER13:
            return binding_status->drawbuffers[13];
        case GL_DRAW_BUFFER14:
            return binding_status->drawbuffers[14];
        case GL_DRAW_BUFFER15:
            return binding_status->drawbuffers[15];
        case GL_READ_BUFFER:
            return binding_status->read_buffer;
        case GL_CURRENT_PROGRAM:
            return binding_status->current_program;
        case GL_FRAGMENT_SHADER_DERIVATIVE_HINT:
            return binding_status->fragment_shader_derivative_hint;
        case GL_GENERATE_MIPMAP_HINT:
            return binding_status->generate_mipmap_hint;
        default:
            ALOGE("%s target %x error", __FUNCTION__, target);
            return 0;
    }
}

const GLubyte *get_static_string(void *context, const GLenum &target,
                                 GLint idx) {
    StaticContextValues *static_status =
        (((GLThreadContext *)context)->static_values);
    // GLubyte **data_addr;
    // GLubyte buffer[1024];

    switch (target) {
        case GL_VENDOR:
            return (const GLubyte *)static_status->vendor;
            break;
        case GL_VERSION:
            return (const GLubyte *)static_status->version;
            break;
        case GL_RENDERER:
            return (const GLubyte *)static_status->renderer;
            break;
        case GL_SHADING_LANGUAGE_VERSION:
            return (const GLubyte *)static_status->shading_language_version;
            break;
        case GL_EXTENSIONS:
            // if idx is given setting it to target element of the array else
            // return the whole array
            if (idx == -1) {
                return (const GLubyte *)static_status->extensions_gles2;
            } else if (idx < static_status->num_extensions) {
                return (const GLubyte *)static_status->extensions[idx];
            } else {
                return NULL;
            }

            break;
        default:
            ALOGE("%s target %x error", __FUNCTION__, target);
            break;
    }
    return NULL;
}

GLint get_pixel_storei(void *context, const GLenum &pname) {
    PixelStoreStatus *pixel_store =
        &(((GLThreadContext *)context)->pixel_store_status);
    switch (pname) {
        case GL_UNPACK_ALIGNMENT:
            return pixel_store->unpack_alignment;
        case GL_PACK_ALIGNMENT:
            return pixel_store->pack_alignment;
        case GL_UNPACK_ROW_LENGTH:
            return pixel_store->unpack_row_length;
        case GL_UNPACK_IMAGE_HEIGHT:
            return pixel_store->unpack_image_height;
        case GL_UNPACK_SKIP_PIXELS:
            return pixel_store->unpack_skip_pixels;
        case GL_UNPACK_SKIP_ROWS:
            return pixel_store->unpack_skip_rows;
        case GL_UNPACK_SKIP_IMAGES:
            return pixel_store->unpack_skip_images;
        case GL_PACK_ROW_LENGTH:
            return pixel_store->pack_row_length;
        case GL_PACK_SKIP_PIXELS:
            return pixel_store->pack_skip_pixels;
        case GL_PACK_SKIP_ROWS:
            return pixel_store->pack_skip_rows;
        default:
            ALOGE("%s pname %x error", __FUNCTION__, pname);
            return 0;
    }
}

void d_glBindFramebuffer(void *context, GLenum target, GLuint framebuffer) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_DRAW_FRAMEBUFFER,
                     GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER);
    GLThreadContext *opengl_context = (GLThreadContext *)context;
    BindingStatus *binding_status = &(opengl_context->context_binding_status);

    GLuint pre_read_fbo = binding_status->read_frame_buffer;
    GLuint pre_draw_fbo = binding_status->draw_frame_buffer;

    int released_fbo = 0;
    int has_change = 0;
    ALOGD("bindframebuffer target %x fbo %u", target, framebuffer);
    
    if (pre_draw_fbo != framebuffer && target != GL_READ_FRAMEBUFFER) {
        binding_status->draw_frame_buffer = framebuffer;
        has_change = 1;
    }

    if (pre_read_fbo != framebuffer && target != GL_DRAW_FRAMEBUFFER) {
        binding_status->read_frame_buffer = framebuffer;
        has_change = 1;
    }

    // need
    if (has_change == 0) {
        return;
    }

    r_glBindFramebuffer(context, target, framebuffer);
}

/**
 * @brief 
 *
 * @param context 
 * @param target 
 * @param id buffer id
 */
void d_glBindBuffer_special(void *context, GLenum target, GLuint buffer) {
    ALOGD("bind buffer target %x %d context %llx", target, buffer,
          (long long)context);

    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->buffer_resource()->has_id(buffer)) {
        if (buffer > res_ctx->buffer_resource()->get_max_id() * 2 &&
            buffer > 10000) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
        res_ctx->buffer_resource()->set_id(buffer);
    }
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    switch (target) {
        case GL_ARRAY_BUFFER:
            if (status->array_buffer == buffer) {
                return;
            }
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            if (status->element_array_buffer == buffer) return;
            break;
        case GL_COPY_READ_BUFFER:
            if (status->copy_read_buffer == buffer) return;
            break;
        case GL_COPY_WRITE_BUFFER:
            if (status->copy_write_buffer == buffer) return;
            break;
        case GL_PIXEL_PACK_BUFFER:
            if (status->pixel_pack_buffer == buffer) return;
            break;
        case GL_PIXEL_UNPACK_BUFFER:
            if (status->pixel_unpack_buffer == buffer) return;
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            if (status->transform_feedback_buffer == buffer) return;
            break;
        case GL_UNIFORM_BUFFER:
            if (status->uniform_buffer == buffer) return;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            if (status->atomic_counter_buffer == buffer) return;
            break;
        case GL_DISPATCH_INDIRECT_BUFFER:
            if (status->dispatch_indirect_buffer == buffer) return;
            break;
        case GL_DRAW_INDIRECT_BUFFER:
            if (status->draw_indirect_buffer == buffer) return;
            break;
        case GL_SHADER_STORAGE_BUFFER:
            if (status->shader_storage_buffer == buffer) return;
            break;
        case GL_TEXTURE_BUFFER:
            if (status->texture_buffer == buffer) return;
            break;
    }

    r_glBindBuffer_origin(context, target, buffer);

    
    if (buffer != 0) {
        bound_buffer->buffer_type[buffer] = target;
    }
    switch (target) {
        case GL_ARRAY_BUFFER:
            status->array_buffer = buffer;
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            status->element_array_buffer = buffer;
            bound_buffer->attrib_point->element_array_buffer_binding =
                status->element_array_buffer;
            break;
        case GL_COPY_READ_BUFFER:
            status->copy_read_buffer = buffer;
            break;
        case GL_COPY_WRITE_BUFFER:
            status->copy_write_buffer = buffer;
            break;
        case GL_PIXEL_PACK_BUFFER:
            status->pixel_pack_buffer = buffer;
            break;
        case GL_PIXEL_UNPACK_BUFFER:
            status->pixel_unpack_buffer = buffer;
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            status->transform_feedback_buffer = buffer;
            break;
        case GL_UNIFORM_BUFFER:
            status->uniform_buffer = buffer;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            status->atomic_counter_buffer = buffer;
            break;
        case GL_DISPATCH_INDIRECT_BUFFER:
            status->dispatch_indirect_buffer = buffer;
            break;
        case GL_DRAW_INDIRECT_BUFFER:
            status->draw_indirect_buffer = buffer;
            break;
        case GL_SHADER_STORAGE_BUFFER:
            status->shader_storage_buffer = buffer;
            break;
        case GL_TEXTURE_BUFFER:
            status->texture_buffer = buffer;
            break;
        default:
            ALOGE("%s target %x error", __FUNCTION__, target);
            break;
    }
}

void d_glBindSampler_special(void *context, GLuint unit, GLuint sampler_id) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    // saving binding unit and corresponding sampler to the struct
    if (bind_status->sampler_binding_2d[unit] == sampler_id) return;
    bind_status->sampler_binding_2d[unit] = sampler_id;
    r_glBindSampler(context, unit, sampler_id);
}

void d_glDrawBuffers_special(void *context, GLsizei n, const GLenum *bufs) {
    r_glDrawBuffers(context, n, bufs);
}

void d_glHint_special(void *context, const GLenum &target, GLenum mode) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    if (target == GL_FRAGMENT_SHADER_DERIVATIVE_HINT) {
        bind_status->fragment_shader_derivative_hint = mode;
    } else {
        bind_status->generate_mipmap_hint = mode;
    }
    r_glHint(context, target, mode);
}

void d_glBindTransformFeedback_special(void *context, const GLenum &target,
                                       GLuint feedback_id) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    if (bind_status->transform_feedback_binding == feedback_id) return;

    bind_status->transform_feedback_binding = feedback_id;
    r_glBindTransformFeedback(context, target, feedback_id);
}

void d_glActiveTexture_special(void *context, const GLenum &target) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    if (bind_status->active_texture == target - GL_TEXTURE0) {
        return;
    }
    bind_status->active_texture = target - GL_TEXTURE0;
    ALOGD("active texture %x", target);
    r_glActiveTexture(context, target);
}

void d_glBindTexture_special(void *context, const GLenum &target, GLuint id) {
    GLThreadContext *opengl_context = (GLThreadContext *)context;
    ResourceContext *res_ctx = &(opengl_context->resource_context);
    if (!res_ctx->texture_resource()->has_id(id)) {
        if (id > res_ctx->texture_resource()->get_max_id() * 2 && id > 10000) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
    }

    BindingStatus *binding_status = &(opengl_context->context_binding_status);
    ALOGD("context %llx bindtexture target %x id %u",
          (unsigned long long)context, target, id);
    GLuint pre_texture_id = 0;

    switch (target) {
        case GL_TEXTURE_2D:
            pre_texture_id =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d;
            ALOGD("context %llx bindtexture id %u pre id %u",
                  (unsigned long long)context, id, pre_texture_id);
            if (id != 0 && res_ctx->texture_has_gbuffer()->has_id(id)) {
                binding_status->gbuffer_2D_binding =
                    opengl_context->texture_gbuffer_map[0][id];
            } else {
                binding_status->gbuffer_2D_binding = NULL;
            }
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_2d = id;
            break;
        case GL_TEXTURE_CUBE_MAP:
            pre_texture_id =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_cube_map;
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_cube_map = id;
            break;
        case GL_TEXTURE_3D:
            pre_texture_id =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_3d;
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_3d = id;
            break;
        case GL_TEXTURE_2D_ARRAY:
            pre_texture_id =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_array;
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_2d_array = id;
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            pre_texture_id =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_multisample;
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_2d_multisample = id;
            break;
        case GL_TEXTURE_EXTERNAL_OES:
            pre_texture_id =
                binding_status->texture_binding[0].texture_binding_external;
            if (id != 0 && res_ctx->texture_has_gbuffer()->has_id(id)) {
                binding_status->gbuffer_external_binding =
                    opengl_context->texture_gbuffer_map[0][id];
            } else {
                ALOGE("bind external with null gbuffer %d",
                      (int)(res_ctx->texture_has_gbuffer()->has_id(id)));
                binding_status->gbuffer_external_binding = NULL;
            }
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[0].texture_binding_external = id;
            break;
        case GL_TEXTURE_BUFFER:
            pre_texture_id =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_texture_buffer;
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_texture_buffer = id;
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            pre_texture_id =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_multisample_array;
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_2d_multisample_array = id;
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            pre_texture_id =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_cube_map_array;
            if (pre_texture_id == id) {
                return;
            }
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_cube_map_array = id;
            break;
        default:
            ALOGE("%s target %x error", __FUNCTION__, target);
            break;
    }

    r_glBindTexture(context, target, id);
}

void d_glBindBufferBase_special(void *context, GLenum target, GLuint index,
                                GLuint buffer) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->buffer_resource()->has_id(buffer)) {
        if (buffer > res_ctx->buffer_resource()->get_max_id() * 2 &&
            buffer > 10000) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
        res_ctx->buffer_resource()->set_id(buffer);
    }
    
    // GL_MAX_UNIFORM_BUFFER_BINDINGS
    if (index > 1000) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);

    
    if (buffer != 0) {
        bound_buffer->buffer_type[buffer] = target;
    }

    GLuint *buffer_index_max_ptr = NULL;
    GLuint **buffer_index_ptr = NULL;
    switch (target) {
        case GL_TRANSFORM_FEEDBACK_BUFFER: {
            status->transform_feedback_buffer = buffer;
            buffer_index_max_ptr = &(bind_status->feedback_max_index);
            buffer_index_ptr = &(bind_status->feedback_buffer_binding_index);
            break;
        }
        case GL_UNIFORM_BUFFER: {
            status->uniform_buffer = buffer;
            buffer_index_max_ptr = &(bind_status->uniform_max_index);
            buffer_index_ptr = &(bind_status->uniform_buffer_binding_index);
            break;
        }
        case GL_ATOMIC_COUNTER_BUFFER: {
            status->atomic_counter_buffer = buffer;
            buffer_index_max_ptr = &(bind_status->atomic_counter_max_index);
            buffer_index_ptr =
                &(bind_status->atomic_counter_buffer_binding_index);
            break;
        }
        case GL_SHADER_STORAGE_BUFFER: {
            status->shader_storage_buffer = buffer;
            buffer_index_max_ptr = &(bind_status->shader_storage_max_index);
            buffer_index_ptr =
                &(bind_status->shader_storage_buffer_binding_index);
            break;
        }

        default:
            ALOGE("%s target %x error", __FUNCTION__, target);
            break;
    }

    if (buffer_index_max_ptr != NULL && buffer_index_ptr != NULL) {
        if (index >= *buffer_index_max_ptr) {
            GLuint *temp = new GLuint[(index * 2 > 100 ? index * 2 : 100)];
            if (*buffer_index_ptr != NULL) {
                memcpy(temp, *buffer_index_ptr,
                       *buffer_index_max_ptr * sizeof(GLuint));
                memset(temp + *buffer_index_max_ptr, 0,
                       ((index * 2 > 100 ? index * 2 : 100) -
                        *buffer_index_max_ptr) *
                           sizeof(GLuint));
                delete *buffer_index_ptr;
            } else {
                memset(temp, 0,
                       (index * 2 > 100 ? index * 2 : 100) * sizeof(GLuint));
            }
            *buffer_index_max_ptr = (index * 2 > 100 ? index * 2 : 100);
            *buffer_index_ptr = temp;
        }
        (*buffer_index_ptr)[index] = buffer;
    }

    r_glBindBufferBase(context, target, index, buffer);
}

void d_glBindBufferRange_special(void *context, GLenum target, GLuint index,
                                 GLuint buffer, GLintptr offset,
                                 GLsizeiptr size) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->buffer_resource()->has_id(buffer)) {
        if (buffer > res_ctx->buffer_resource()->get_max_id() * 2 &&
            buffer > 10000) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
        res_ctx->buffer_resource()->set_id(buffer);
    }

    ALOGD("glBindBufferRange target %x id %u index %u offset %lld size %lld",
          target, buffer, index, (long long)offset, (long long)size);

    
    // GL_MAX_UNIFORM_BUFFER_BINDINGS
    //  if (index > 1000)
    //  {
    //      set_gl_error(context, GL_INVALID_VALUE);
    //      return;
    //  }

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);

    
    if (buffer != 0) {
        bound_buffer->buffer_type[buffer] = target;
    }
    switch (target) {
        case GL_TRANSFORM_FEEDBACK_BUFFER: {
            status->transform_feedback_buffer = buffer;

            if (index >= bind_status->feedback_max_index) {
                GLuint *temp = new GLuint[(index * 2 > 100 ? index * 2 : 100)];
                if (bind_status->feedback_buffer_binding_index != NULL) {
                    memcpy(temp, bind_status->feedback_buffer_binding_index,
                           bind_status->feedback_max_index * sizeof(GLuint));
                    memset(temp + bind_status->feedback_max_index, 0,
                           ((index * 2 > 100 ? index * 2 : 100) -
                            bind_status->feedback_max_index) *
                               sizeof(GLuint));
                    delete bind_status->feedback_buffer_binding_index;
                } else {
                    memset(
                        temp, 0,
                        (index * 2 > 100 ? index * 2 : 100) * sizeof(GLuint));
                }
                bind_status->feedback_max_index =
                    (index * 2 > 100 ? index * 2 : 100);
                bind_status->feedback_buffer_binding_index = temp;
            }
            bind_status->feedback_buffer_binding_index[index] = buffer;
            break;
        }
        case GL_UNIFORM_BUFFER: {
            status->uniform_buffer = buffer;

            if (index >= bind_status->uniform_max_index) {
                GLuint *temp = new GLuint[(index * 2 > 100 ? index * 2 : 100)];
                if (bind_status->uniform_buffer_binding_index != NULL) {
                    memcpy(temp, bind_status->uniform_buffer_binding_index,
                           bind_status->uniform_max_index * sizeof(GLuint));
                    memset(temp + bind_status->uniform_max_index, 0,
                           ((index * 2 > 100 ? index * 2 : 100) -
                            bind_status->uniform_max_index) *
                               sizeof(GLuint));
                    delete bind_status->uniform_buffer_binding_index;
                } else {
                    memset(
                        temp, 0,
                        (index * 2 > 100 ? index * 2 : 100) * sizeof(GLuint));
                }
                bind_status->uniform_max_index =
                    (index * 2 > 100 ? index * 2 : 100);
                bind_status->uniform_buffer_binding_index = temp;
            }
            bind_status->uniform_buffer_binding_index[index] = buffer;
            break;
        }
        case GL_SHADER_STORAGE_BUFFER:
        case GL_ATOMIC_COUNTER_BUFFER:
            break;
        default:
            ALOGE("glBindBufferRange error target %x", target);
            break;
    }

    r_glBindBufferRange(context, target, index, buffer, offset, size);
}

void d_glPauseTransformFeedback_special(void *context) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    bind_status->transform_feedback_paused = GL_TRUE;
    r_glPauseTransformFeedback(context);
}

void d_glResumeTransformFeedback_special(void *context) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    bind_status->transform_feedback_paused = GL_FALSE;
    r_glResumeTransformFeedback(context);
}

void d_glBeginTransformFeedback_special(void *context, GLenum primitiveMode) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    bind_status->transform_feedback_active = GL_TRUE;
    r_glBeginTransformFeedback(context, primitiveMode);
}

void d_glEndTransformFeedback_special(void *context) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    bind_status->transform_feedback_active = GL_FALSE;
    r_glEndTransformFeedback(context);
}

void d_glDeleteBuffers_special(void *context, GLsizei n,
                               const GLuint *buffers) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;

    BufferStatus *buffer_status = bound_buffer->buffer_status;
    BufferMapped *buffer_ptr = ((GLThreadContext *)context)->buffer_ptr;

    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    for (int i = 0; i < n; i++) {
        GLuint id = buffers[i];
        if (id == 0) {
            continue;
        }

        MapResult *map_res = get_map_result_with_id(context, id);
        if (map_res != NULL) {
            set_map_result_with_id(context, id, NULL);
            ALOGD("deletebuffer id %d", id);

            if (map_res->dynamic_data != NULL) {
                ALOGD("deletebuffer with mapped dynamic buffer\n");
                delete[] map_res->dynamic_data;
            } else if (map_res->map_data != NULL) {
                ALOGE("deletebuffer with mapped buffer\n");
                delete[] map_res->map_data;
            }
            delete map_res;
            // r_glBindBuffer_origin(context,type, pre_id);
        }

        if (buffer_ptr->buffer_layout_maps.find(id) !=
            buffer_ptr->buffer_layout_maps.end()) {
            buffer_ptr->buffer_layout_maps.erase(id);
        }

        GLenum type = bound_buffer->buffer_type[id];

        switch (type) {
            case GL_ELEMENT_ARRAY_BUFFER:
                
                
                {
                    if (bound_buffer->ebo_buffer.find(id) !=
                        bound_buffer->ebo_buffer.end()) {
                        ElementArrayBuffer *ebo_data =
                            bound_buffer->ebo_buffer[id];
                        if (ebo_data != NULL) {
                            // delete[] ebo_data->data;

                            if (ebo_data->dp_max != NULL) {
                                for (int i = 0;
                                     i <=
                                     bound_buffer->ebo_mm[ebo_data->dp_num];
                                     i++) {
                                    delete[] ebo_data->dp_max[i];
                                    delete[] ebo_data->dp_min[i];
                                }
                                delete[] ebo_data->dp_max;
                                delete[] ebo_data->dp_min;
                            }
                            delete ebo_data;
                        }
                        
                        bound_buffer->ebo_buffer.erase(id);
                    }
                    break;
                }
            case GL_ARRAY_BUFFER:
                // vbo
                break;
        }

        if (buffers[i] == buffer_status->array_buffer) {
            buffer_status->array_buffer = 0;
        }
        if (buffers[i] == buffer_status->element_array_buffer) {
            buffer_status->element_array_buffer = 0;
        }
        if (buffers[i] == buffer_status->copy_read_buffer) {
            buffer_status->copy_read_buffer = 0;
        }
        if (buffers[i] == buffer_status->copy_write_buffer) {
            buffer_status->copy_write_buffer = 0;
        }
        if (buffers[i] == buffer_status->pixel_pack_buffer) {
            buffer_status->pixel_pack_buffer = 0;
        }
        if (buffers[i] == buffer_status->pixel_unpack_buffer) {
            buffer_status->pixel_unpack_buffer = 0;
        }
        if (buffers[i] == buffer_status->transform_feedback_buffer) {
            buffer_status->transform_feedback_buffer = 0;
        }
        if (buffers[i] == buffer_status->uniform_buffer) {
            buffer_status->uniform_buffer = 0;
        }
        if (buffers[i] == buffer_status->atomic_counter_buffer) {
            buffer_status->atomic_counter_buffer = 0;
        }
        if (buffers[i] == buffer_status->dispatch_indirect_buffer) {
            buffer_status->dispatch_indirect_buffer = 0;
        }
        if (buffers[i] == buffer_status->draw_indirect_buffer) {
            buffer_status->draw_indirect_buffer = 0;
        }
        if (buffers[i] == buffer_status->shader_storage_buffer) {
            buffer_status->shader_storage_buffer = 0;
        }
        if (buffers[i] == buffer_status->texture_buffer) {
            buffer_status->texture_buffer = 0;
        }

        bound_buffer->buffer_type.erase(id);
    }

    r_glDeleteBuffers_origin(context, n, buffers);
}

void d_glEnable_special(void *context, GLenum cap) {
    EnableStatus *status = &(((GLThreadContext *)context)->enable_status);

    switch (cap) {
        case GL_CULL_FACE:
            if (status->cull_face == GL_TRUE) {
                return;
            }
            status->cull_face = GL_TRUE;
            break;
        case GL_POLYGON_OFFSET_FILL:
            if (status->polygon_offset_fill == GL_TRUE) {
                return;
            }
            status->polygon_offset_fill = GL_TRUE;
            break;
        case GL_SCISSOR_TEST:
            if (status->scissor_test == GL_TRUE) {
                return;
            }
            status->scissor_test = GL_TRUE;
            break;
        case GL_SAMPLE_COVERAGE:
            if (status->sample_coverage == GL_TRUE) {
                return;
            }
            status->sample_coverage = GL_TRUE;
            break;
        case GL_SAMPLE_ALPHA_TO_COVERAGE:
            if (status->sample_alpha_to_coverage == GL_TRUE) {
                return;
            }
            status->sample_alpha_to_coverage = GL_TRUE;
            break;
        case GL_STENCIL_TEST:
            if (status->stencil_test == GL_TRUE) {
                return;
            }
            status->stencil_test = GL_TRUE;
            break;
        case GL_DEPTH_TEST:
            if (status->depth_test == GL_TRUE) {
                return;
            }
            status->depth_test = GL_TRUE;
            break;
        case GL_BLEND:
            if (status->blend == GL_TRUE) {
                return;
            }
            status->blend = GL_TRUE;
            break;
        case GL_PRIMITIVE_RESTART_FIXED_INDEX:
            if (status->primitive_resstart_fixed_index == GL_TRUE) {
                return;
            }
            status->primitive_resstart_fixed_index = GL_TRUE;
            break;
        case GL_RASTERIZER_DISCARD:
            if (status->rasterizer_discard == GL_TRUE) {
                return;
            }
            status->rasterizer_discard = GL_TRUE;
            break;
        case GL_DITHER:
            if (status->dither == GL_TRUE) {
                return;
            }
            status->dither = GL_TRUE;
            break;
        case GL_TEXTURE_2D:
            if (status->texture_2d == GL_TRUE) {
                return;
            }
            status->texture_2d = GL_TRUE;
            break;
        default:
            ALOGE("%s cap %x error", __FUNCTION__, cap);
            break;
    }
    ALOGD("%s cap %x", __FUNCTION__, cap);

    r_glEnable(context, cap);
}

void d_glDisable_special(void *context, GLenum cap) {
    EnableStatus *status = &(((GLThreadContext *)context)->enable_status);

    switch (cap) {
        case GL_CULL_FACE:
            if (status->cull_face == GL_FALSE) {
                return;
            }
            status->cull_face = GL_FALSE;
            break;
        case GL_POLYGON_OFFSET_FILL:
            if (status->polygon_offset_fill == GL_FALSE) {
                return;
            }
            status->polygon_offset_fill = GL_FALSE;
            break;
        case GL_SCISSOR_TEST:
            if (status->scissor_test == GL_FALSE) {
                return;
            }
            status->scissor_test = GL_FALSE;
            break;
        case GL_SAMPLE_COVERAGE:
            if (status->sample_coverage == GL_FALSE) {
                return;
            }
            status->sample_coverage = GL_FALSE;
            break;
        case GL_SAMPLE_ALPHA_TO_COVERAGE:
            if (status->sample_alpha_to_coverage == GL_FALSE) {
                return;
            }
            status->sample_alpha_to_coverage = GL_FALSE;
            break;
        case GL_STENCIL_TEST:
            if (status->stencil_test == GL_FALSE) {
                return;
            }
            status->stencil_test = GL_FALSE;
            break;
        case GL_DEPTH_TEST:
            if (status->depth_test == GL_FALSE) {
                return;
            }
            status->depth_test = GL_FALSE;
            break;
        case GL_BLEND:
            if (status->blend == GL_FALSE) {
                return;
            }
            status->blend = GL_FALSE;
            break;
        case GL_PRIMITIVE_RESTART_FIXED_INDEX:
            if (status->primitive_resstart_fixed_index == GL_FALSE) {
                return;
            }
            status->primitive_resstart_fixed_index = GL_FALSE;
            break;
        case GL_RASTERIZER_DISCARD:
            if (status->rasterizer_discard == GL_FALSE) {
                return;
            }
            status->rasterizer_discard = GL_FALSE;
            break;
        case GL_DITHER:
            if (status->dither == GL_FALSE) {
                return;
            }
            status->dither = GL_FALSE;
            break;
        case GL_TEXTURE_2D:
            if (status->texture_2d == GL_FALSE) {
                return;
            }
            status->texture_2d = GL_FALSE;
            break;
        default:
            ALOGE("%s target %x error", __FUNCTION__, cap);
            break;
    }
    r_glDisable(context, cap);
}

void d_glGetIntegerv_special(void *context, GLenum pname, GLint *data) {
    auto *opengl_context = (GLThreadContext *)context;
    StaticContextValues *static_status =
        (((GLThreadContext *)context)->static_values);

    FrameBufferInfo *fb_info = nullptr;
    int data_bit = 0;

    switch (pname) {
        case GL_MAJOR_VERSION:
            *data = static_status->major_version;
            break;
        case GL_MINOR_VERSION:
            *data = static_status->minor_version;
            break;
        case GL_SUBPIXEL_BITS:
            *data = static_status->subpixel_bits;
            break;
        case GL_MAX_3D_TEXTURE_SIZE:
            *data = static_status->max_3d_texture_size;
            break;
        case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
            *data = static_status->max_combined_texture_image_units;
            break;
        case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
            *data = static_status->max_cube_map_texture_size;
            break;
        case GL_MAX_ELEMENTS_VERTICES:
            *data = static_status->max_elements_vertices;
            break;
        case GL_MAX_ELEMENTS_INDICES:
            *data = static_status->max_elements_indices;
            break;
        case GL_MAX_FRAGMENT_UNIFORM_COMPONENTS:
            *data = static_status->max_fragment_uniform_components;
            break;
        case GL_MAX_RENDERBUFFER_SIZE:
            *data = static_status->max_renderbuffer_size;
            break;
        case GL_MAX_SAMPLES:
            *data = static_status->max_samples;
            break;
        case GL_MAX_TEXTURE_SIZE:
            *data = static_status->max_texture_size;
            break;
        case GL_MAX_VERTEX_ATTRIBS:
            *data = static_status->max_vertex_attribs;
            break;
        case GL_MAX_VERTEX_UNIFORM_COMPONENTS:
            *data = static_status->max_vertex_uniform_components;
            break;
        case GL_MAX_IMAGE_UNITS:
            *data = static_status->max_image_units;
            break;
        case GL_MAX_VERTEX_ATTRIB_BINDINGS:
            *data = static_status->max_vertex_attrib_bindings;
            break;
        case GL_MAX_COMPUTE_UNIFORM_BLOCKS:
            *data = static_status->max_computer_uniform_blocks;
            break;
        case GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS:
            *data = static_status->max_computer_texture_image_units;
            break;
        case GL_MAX_COMPUTE_IMAGE_UNIFORMS:
            *data = static_status->max_computer_image_uniforms;
            break;
        case GL_MAX_COMPUTE_SHARED_MEMORY_SIZE:
            *data = static_status->max_computer_sharde_memory_size;
            break;
        case GL_MAX_COMPUTE_UNIFORM_COMPONENTS:
            *data = static_status->max_computer_uniform_components;
            break;
        case GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS:
            *data = static_status->max_computer_atomic_counter_buffers;
            break;
        case GL_MAX_COMPUTE_ATOMIC_COUNTERS:
            *data = static_status->max_computer_atomic_counters;
            break;
        case GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS:
            *data = static_status->max_combined_compute_uniform_components;
            break;
        case GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS:
            *data = static_status->max_computer_work_group_invocations;
            break;
        case GL_MAX_UNIFORM_LOCATIONS:
            *data = static_status->max_uniform_locations;
            break;
        case GL_MAX_FRAMEBUFFER_WIDTH:
            *data = static_status->max_framebuffer_width;
            break;
        case GL_MAX_FRAMEBUFFER_HEIGHT:
            *data = static_status->max_framebuffer_height;
            break;
        case GL_MAX_FRAMEBUFFER_SAMPLES:
            *data = static_status->max_framebuffer_samples;
            break;
        case GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS:
            *data = static_status->max_vertex_atomic_counter_buffers;
            break;
        case GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS:
            *data = static_status->max_fragment_atomic_counter_buffers;
            break;
        case GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS:
            *data = static_status->max_combined_atomic_counter_buffers;
            break;
        case GL_MAX_FRAGMENT_ATOMIC_COUNTERS:
            *data = static_status->max_fragment_atomic_counters;
            break;
        case GL_MAX_COMBINED_ATOMIC_COUNTERS:
            *data = static_status->max_combined_atomic_counters;
            break;
        case GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE:
            *data = static_status->max_atomic_counter_buffer_size;
            break;
        case GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS:
            *data = static_status->max_atomic_counter_buffer_bindings;
            break;
        case GL_MAX_VERTEX_IMAGE_UNIFORMS:
            *data = static_status->max_vertex_image_uniforms;
            break;
        case GL_MAX_FRAGMENT_IMAGE_UNIFORMS:
            *data = static_status->max_fragment_image_uniforms;
            break;
        case GL_MAX_COMBINED_IMAGE_UNIFORMS:
            *data = static_status->max_combined_image_uniforms;
            break;
        case GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS:
            *data = static_status->max_vertex_shader_storage_blocks;
            break;
        case GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS:
            *data = static_status->max_fragment_shader_storage_blocks;
            break;
        case GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS:
            *data = static_status->max_compute_shader_storage_blocks;
            break;
        case GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS:
            *data = static_status->max_combined_shader_storage_blocks;
            break;
        case GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS:
            *data = static_status->max_shader_storage_buffer_bindings;
            break;
        case GL_MAX_SHADER_STORAGE_BLOCK_SIZE:
            *data = static_status->max_shader_storage_block_size;
            break;
        case GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES:
            *data = static_status->max_combined_shader_output_resources;
            break;
        case GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET:
            *data = static_status->min_program_texture_gather_offset;
            break;
        case GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET:
            *data = static_status->max_program_texture_gather_offset;
            break;
        case GL_MAX_SAMPLE_MASK_WORDS:
            *data = static_status->max_sample_mask_words;
            break;
        case GL_MAX_COLOR_TEXTURE_SAMPLES:
            *data = static_status->max_color_texture_samples;
            break;
        case GL_MAX_DEPTH_TEXTURE_SAMPLES:
            *data = static_status->max_depth_texture_samples;
            break;
        case GL_MAX_INTEGER_SAMPLES:
            *data = static_status->max_integer_samples;
            break;
        case GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET:
            *data = static_status->max_vertex_attrib_relative_offset;
            break;
        case GL_MAX_VERTEX_ATTRIB_STRIDE:
            *data = static_status->max_vertex_attrib_stride;
            break;

        case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
            *data = static_status->max_vertex_texture_image_units;
            break;
        case GL_MAX_VERTEX_UNIFORM_VECTORS:
            *data = static_status->max_vertex_uniform_vectors;
            break;
        case GL_MAX_VIEWPORT_DIMS:
            *data = static_status->max_viewport_dims[0];
            *(data + 1) = static_status->max_viewport_dims[1];
            break;
        case GL_MAX_ARRAY_TEXTURE_LAYERS:
            *data = static_status->max_array_texture_layers;
            break;
        case GL_MAX_COLOR_ATTACHMENTS:
            *data = static_status->max_color_attachments;
            break;
        case GL_MAX_COMBINED_UNIFORM_BLOCKS:
            *data = static_status->max_combined_uniform_blocks;
            break;
        case GL_MAX_DRAW_BUFFERS:
            *data = static_status->max_draw_buffers;
            break;
        case GL_MAX_FRAGMENT_INPUT_COMPONENTS:
            *data = static_status->max_fragment_input_components;
            break;
        case GL_MAX_FRAGMENT_UNIFORM_BLOCKS:
            *data = static_status->max_fragment_uniform_blocks;
            break;
        case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
            *data = static_status->max_fragment_uniform_vectors;
            break;
        case GL_MAX_PROGRAM_TEXEL_OFFSET:
            *data = static_status->max_program_texel_offset;
            break;
        case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS:
            *data = static_status->max_transform_feedback_separate_attribs;
            break;
        case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS:
            *data = static_status->max_transform_feedback_separate_components;
            break;
        case GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS:
            *data =
                static_status->max_transform_feedback_interleaved_components;
            break;
        case GL_MAX_UNIFORM_BUFFER_BINDINGS:
            *data = static_status->max_uniform_buffer_bindings;
            break;
        // case GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS:
        //     *data = static_status->max_atomic_counter_buffer_bindings;
        //     break;
        // case GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS:
        //     *data = static_status->max_shader_storage_buffer_bindings;
        //     break;
        case GL_MAX_VARYING_VECTORS:
            *data = static_status->max_varying_vectors;
            break;
        case GL_MAX_VARYING_COMPONENTS:
            *data = static_status->max_varying_components;
            break;
        case GL_MAX_VERTEX_OUTPUT_COMPONENTS:
            *data = static_status->max_vertex_output_components;
            break;
        case GL_MAX_VERTEX_UNIFORM_BLOCKS:
            *data = static_status->max_vertex_uniform_blocks;
            break;
        case GL_MIN_PROGRAM_TEXEL_OFFSET:
            *data = static_status->min_program_texel_offset;
            break;
        case GL_NUM_SHADER_BINARY_FORMATS:
            *data = static_status->num_shader_binary_formats;
            break;
        case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
            *data = static_status->num_compressed_texture_formats;
            break;
        case GL_NUM_PROGRAM_BINARY_FORMATS:
            *data = static_status->num_program_binary_formats;
            break;
        case GL_NUM_EXTENSIONS:
            *data = static_status->num_extensions;
            break;

        case GL_MAX_ELEMENT_INDEX:
            *data = (GLint)static_status->max_element_index;
            break;
        case GL_MAX_SERVER_WAIT_TIMEOUT:
            *data = (GLint)static_status->max_server_wait_timeout;
            break;
        case GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS:
            *data =
                (GLint)static_status->max_combined_vertex_uniform_components;
            break;
        case GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS:
            *data =
                (GLint)static_status->max_combined_fragment_uniform_components;
            break;
        case GL_MAX_UNIFORM_BLOCK_SIZE:
            *data = (GLint)static_status->max_uniform_block_size;
            break;
        case GL_MAX_TEXTURE_IMAGE_UNITS:
            *data = (GLint)static_status->texture_image_units;
            break;
        case GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT:
            *data = (GLint)static_status->uniform_buffer_offset_alignment;
            break;
        case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:
            *data = (GLint)static_status->max_texture_anisotropy;
            break;

        case GL_VERTEX_ARRAY_BINDING:
            *data = ((GLThreadContext *)context)
                        ->context_binding_status.bound_buffer->now_vao;
            break;

        case GL_PROGRAM_BINARY_FORMATS:
            memcpy(data, static_status->program_binary_formats,
                   static_status->num_program_binary_formats * sizeof(GLint));
            break;
        case GL_SHADER_BINARY_FORMATS:
            memcpy(data, static_status->shader_binary_formats,
                   static_status->num_shader_binary_formats * sizeof(GLint));
            break;
        case GL_COMPRESSED_TEXTURE_FORMATS:
            memcpy(
                data, static_status->compressed_texture_formats,
                static_status->num_compressed_texture_formats * sizeof(GLint));
            break;

        case GL_ARRAY_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_ARRAY_BUFFER);
            break;
        case GL_ELEMENT_ARRAY_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_ELEMENT_ARRAY_BUFFER);
            break;
        case GL_COPY_READ_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_COPY_READ_BUFFER);
            break;
        case GL_COPY_WRITE_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_COPY_WRITE_BUFFER);
            break;
        case GL_PIXEL_PACK_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_PIXEL_PACK_BUFFER);
            break;
        case GL_PIXEL_UNPACK_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER);
            break;

            
            
        case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_TRANSFORM_FEEDBACK_BUFFER);
            break;
        case GL_UNIFORM_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_UNIFORM_BUFFER);
            break;

        case GL_ATOMIC_COUNTER_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_ATOMIC_COUNTER_BUFFER);
            break;
        case GL_DISPATCH_INDIRECT_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_DISPATCH_INDIRECT_BUFFER);
            break;
        case GL_DRAW_INDIRECT_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_DRAW_INDIRECT_BUFFER);
            break;
        case GL_SHADER_STORAGE_BUFFER_BINDING:
            *data = get_bound_buffer(context, GL_SHADER_STORAGE_BUFFER);
            break;

            
        case GL_DRAW_FRAMEBUFFER_BINDING:
            *data = get_binging_status(context, GL_DRAW_FRAMEBUFFER_BINDING);
            break;
        case GL_READ_FRAMEBUFFER_BINDING:
            *data = get_binging_status(context, GL_READ_FRAMEBUFFER_BINDING);
            break;
        case GL_RENDERBUFFER_BINDING:
            *data = get_binging_status(context, GL_RENDERBUFFER_BINDING);
            break;
        case GL_TEXTURE_BINDING_EXTERNAL_OES:
            *data = get_binging_status(context, pname);
            break;
        case GL_TEXTURE_BINDING_2D:
            *data = get_binging_status(context, pname);
            break;
        case GL_TEXTURE_BINDING_3D:
            *data = get_binging_status(context, pname);
            break;
        case GL_TEXTURE_BINDING_2D_ARRAY:
            *data = get_binging_status(context, pname);
            break;
        case GL_TEXTURE_BINDING_2D_MULTISAMPLE:
            *data = get_binging_status(context, pname);
            break;
        case GL_TEXTURE_BINDING_CUBE_MAP:
            *data = get_binging_status(context, pname);
            break;
        case GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY:
            *data = get_binging_status(context, pname);
            break;
        case GL_TEXTURE_BINDING_CUBE_MAP_ARRAY:
            *data = get_binging_status(context, pname);
            break;
        case GL_TEXTURE_BINDING_BUFFER:
            *data = get_binging_status(context, pname);
            break;
        case GL_SAMPLER_BINDING:
            *data = get_binging_status(context, pname);
            break;
        case GL_TRANSFORM_FEEDBACK_BINDING:
            *data = get_binging_status(context, pname);
            break;
        case GL_ACTIVE_TEXTURE:  // glActiveTexture.
            *data = get_binging_status(context, pname);
            break;
        case GL_UNPACK_IMAGE_HEIGHT:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_UNPACK_SKIP_IMAGES:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_UNPACK_ROW_LENGTH:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_UNPACK_SKIP_ROWS:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_UNPACK_SKIP_PIXELS:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_UNPACK_ALIGNMENT:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_PACK_ROW_LENGTH:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_PACK_SKIP_ROWS:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_PACK_SKIP_PIXELS:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
        case GL_PACK_ALIGNMENT:  // glPixelStorei.
            *data = get_pixel_storei(context, pname);
            break;
            // _____________ OTHERS EDITABLE STATUS BINDING TO OPENGL ES CONTEXT
            
        case GL_VIEWPORT:  
            r_glGetIntegerv(context, GL_VIEWPORT, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_CULL_FACE_MODE:  // glCullFace
            r_glGetIntegerv(context, GL_CULL_FACE_MODE, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_FRONT_FACE:  // glFrontFace.
            r_glGetIntegerv(context, GL_FRONT_FACE, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_WRITEMASK:  // glStencilMask. glStencilMaskSeparate.
                                    // @QQQ got -1? should be at least 1
            r_glGetIntegerv(context, GL_STENCIL_WRITEMASK, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_BACK_WRITEMASK:  // glStencilMaskSeparate. @QQQ got -1?
                                         // should be at least 1
            r_glGetIntegerv(context, GL_STENCIL_BACK_WRITEMASK, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_CLEAR_VALUE:  // glClearStencil.
            r_glGetIntegerv(context, GL_STENCIL_CLEAR_VALUE, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_SCISSOR_BOX:  // glScissor.
            r_glGetIntegerv(context, GL_SCISSOR_BOX, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_FUNC:  // glStencilFunc. glStencilFuncSeparate.
            r_glGetIntegerv(context, GL_STENCIL_FUNC, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_VALUE_MASK:  // glStencilFunc. glStencilFuncSeparate.
                                     // @QQQ got -1??
            r_glGetIntegerv(context, GL_STENCIL_VALUE_MASK, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_REF:  // glStencilFunc. glStencilFuncSeparate.
            r_glGetIntegerv(context, GL_STENCIL_REF, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_FAIL:  // glStencilOp. glStencilOpSeparate.
            r_glGetIntegerv(context, GL_STENCIL_FAIL, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_PASS_DEPTH_FAIL:  // glStencilOp. glStencilOpSeparate.
            r_glGetIntegerv(context, GL_STENCIL_PASS_DEPTH_FAIL, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_PASS_DEPTH_PASS:  // glStencilOp. glStencilOpSeparate.
            r_glGetIntegerv(context, GL_STENCIL_PASS_DEPTH_PASS, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_BACK_FUNC:  // glStencilOpSeparate.
            r_glGetIntegerv(context, GL_STENCIL_BACK_FUNC, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_BACK_VALUE_MASK:  // glStencilFuncSeparate. @QQQ got -1
                                          // same with computer
            r_glGetIntegerv(context, GL_STENCIL_BACK_VALUE_MASK, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_BACK_REF:  // glStencilFuncSeparate.
            r_glGetIntegerv(context, GL_STENCIL_BACK_REF, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_BACK_FAIL:  // glStencilOpSeparate.
            r_glGetIntegerv(context, GL_STENCIL_BACK_FAIL, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_BACK_PASS_DEPTH_FAIL:  // glStencilOpSeparate.
            r_glGetIntegerv(context, GL_STENCIL_BACK_PASS_DEPTH_FAIL, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_STENCIL_BACK_PASS_DEPTH_PASS:  // glStencilOpSeparate
            r_glGetIntegerv(context, GL_STENCIL_BACK_PASS_DEPTH_PASS, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_DEPTH_FUNC:  // glDepthFunc.
            r_glGetIntegerv(context, GL_DEPTH_FUNC, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_BLEND_SRC_ALPHA:  //  glBlendFunc glBlendFuncSeparate.
            r_glGetIntegerv(context, GL_BLEND_SRC_ALPHA, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_BLEND_SRC_RGB:  //  glBlendFunc glBlendFuncSeparate.
            r_glGetIntegerv(context, GL_BLEND_SRC_RGB, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_BLEND_DST_RGB:  //  glBlendFunc glBlendFuncSeparate.
            r_glGetIntegerv(context, GL_BLEND_DST_RGB, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_BLEND_DST_ALPHA:  //  glBlendFunc glBlendFuncSeparate.
            r_glGetIntegerv(context, GL_BLEND_DST_ALPHA, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_BLEND_EQUATION_RGB:  // glBlendEquationSeparate.
            r_glGetIntegerv(context, GL_BLEND_EQUATION_RGB, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_BLEND_EQUATION_ALPHA:  // glBlendEquationSeparate.
            r_glGetIntegerv(context, GL_BLEND_EQUATION_ALPHA, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_DRAW_BUFFER0:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER0);
            break;
        case GL_DRAW_BUFFER1:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER1);
            break;
        case GL_DRAW_BUFFER2:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER2);
            break;
        case GL_DRAW_BUFFER3:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER3);
            break;
        case GL_DRAW_BUFFER4:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER4);
            break;
        case GL_DRAW_BUFFER5:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER5);
            break;
        case GL_DRAW_BUFFER6:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER6);
            break;
        case GL_DRAW_BUFFER7:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER7);
            break;
        case GL_DRAW_BUFFER8:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER8);
            break;
        case GL_DRAW_BUFFER9:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER9);
            break;
        case GL_DRAW_BUFFER10:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER10);
            break;
        case GL_DRAW_BUFFER11:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER11);
            break;
        case GL_DRAW_BUFFER12:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER12);
            break;
        case GL_DRAW_BUFFER13:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER13);
            break;
        case GL_DRAW_BUFFER14:  //
            *data = get_binging_status(context, GL_DRAW_BUFFER14);
            break;
        case GL_DRAW_BUFFER15:  // glDrawBuffers
            *data = get_binging_status(context, GL_DRAW_BUFFER15);
            break;
        case GL_READ_BUFFER:  // glReadPixels.
            *data = get_binging_status(context, GL_READ_BUFFER);
            break;
        case GL_CURRENT_PROGRAM:  // glUseProgram.
            *data = get_binging_status(context, GL_CURRENT_PROGRAM);
            break;
        case GL_GENERATE_MIPMAP_HINT:  // glHint.
            *data = get_binging_status(context, GL_GENERATE_MIPMAP_HINT);
            break;
        case GL_FRAGMENT_SHADER_DERIVATIVE_HINT:  // glHint.
            *data =
                get_binging_status(context, GL_FRAGMENT_SHADER_DERIVATIVE_HINT);
            break;
            //____________ STATIC STATUS _____________
            
        case GL_ALPHA_BITS:
            fb_info = FrameBufferInfo::get_fb_info(GL_FRAMEBUFFER);
            fb_info->get_format_bits(pname, data);
            break;
        case GL_BLUE_BITS:
            fb_info = FrameBufferInfo::get_fb_info(GL_FRAMEBUFFER);
            fb_info->get_format_bits(pname, data);
            break;
        case GL_DEPTH_BITS:
            fb_info = FrameBufferInfo::get_fb_info(GL_FRAMEBUFFER);
            fb_info->get_format_bits(pname, data);
            break;
        case GL_GREEN_BITS:
            fb_info = FrameBufferInfo::get_fb_info(GL_FRAMEBUFFER);
            fb_info->get_format_bits(pname, data);
            break;
        case GL_RED_BITS:
            fb_info = FrameBufferInfo::get_fb_info(GL_FRAMEBUFFER);
            fb_info->get_format_bits(pname, data);
            break;
        case GL_STENCIL_BITS:
            fb_info = FrameBufferInfo::get_fb_info(GL_FRAMEBUFFER);
            fb_info->get_format_bits(pname, data);
            break;
        default:
            // r_glGetIntegerv(context, pname, data);
            ALOGE("%s unknow pname %x data %d", __FUNCTION__, pname, *data);
            break;
    }
    int ret_data = *data;
    ALOGD("getintegerv pname %x data %d", pname, ret_data);
}

void d_glGetInteger64v_special(void *context, GLenum pname, GLint64 *data) {
    StaticContextValues *static_status =
        (((GLThreadContext *)context)->static_values);

    switch (pname) {
            //____________ STATIC STATUS _____________
        
        case GL_MAX_ELEMENT_INDEX:
            *data = static_status->max_element_index;
            break;
        case GL_MAX_SERVER_WAIT_TIMEOUT:
            *data = static_status->max_server_wait_timeout;
            break;
        case GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS:
            *data = static_status->max_combined_vertex_uniform_components;
            break;
        case GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS:
            *data = static_status->max_combined_fragment_uniform_components;
            break;
        case GL_MAX_UNIFORM_BLOCK_SIZE:
            *data = static_status->max_uniform_block_size;
            break;
        default:
            r_glGetInteger64v(context, pname, data);
            ALOGE("%s unknow pname %x data %lld", __FUNCTION__, pname,
                  (long long)*data);
            break;
    }
}

void d_glGetFloatv_special(void *context, GLenum pname, GLfloat *data) {
    StaticContextValues *static_status =
        (((GLThreadContext *)context)->static_values);
    switch (pname) {
        //____________ STATIC STATUS _____________
        
        case GL_ALIASED_LINE_WIDTH_RANGE:  // first time got the 10.0 1.0 right
                                           // answer
            memcpy(data, static_status->aliased_line_width_range,
                   2 * sizeof(GLfloat));
            break;
        case GL_ALIASED_POINT_SIZE_RANGE:
            memcpy(data, static_status->aliased_point_size_range,
                   2 * sizeof(GLfloat));
            break;
        case GL_MAX_TEXTURE_LOD_BIAS:
            *data = static_status->max_texture_log_bias;
            break;
            // _____________ OTHERS EDITABLE STATUS BINDING TO OPENGL ES CONTEXT
            
        case GL_DEPTH_RANGE:  // glBindBuffer, glBindBufferBase, and
                              // glBindBufferRange.
            r_glGetFloatv(context, GL_DEPTH_RANGE, data);
            break;
        case GL_LINE_WIDTH:  // glLineWidth
            r_glGetFloatv(context, GL_LINE_WIDTH, data);
            break;
        case GL_POLYGON_OFFSET_FACTOR:  // glPolygonOffset.
            r_glGetFloatv(context, GL_POLYGON_OFFSET_FACTOR, data);
            break;
        case GL_POLYGON_OFFSET_UNITS:  // glPolygonOffset.
            r_glGetFloatv(context, GL_POLYGON_OFFSET_UNITS, data);
            break;
        case GL_SAMPLE_COVERAGE_VALUE:  // glSampleCoverage.
            r_glGetFloatv(context, GL_SAMPLE_COVERAGE_VALUE, data);
            break;
        case GL_COLOR_CLEAR_VALUE:  //  glClearColor.
            r_glGetFloatv(context, GL_COLOR_CLEAR_VALUE, data);
            break;
        case GL_DEPTH_CLEAR_VALUE:  // glClearDepthf.
            r_glGetFloatv(context, GL_DEPTH_CLEAR_VALUE, data);
            break;
        case GL_BLEND_COLOR:  // glBlendColor.
            r_glGetFloatv(context, GL_BLEND_COLOR, data);
            break;
        case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:
            *data = (GLfloat)static_status->max_texture_anisotropy;
            break;
        default:
            r_glGetFloatv(context, pname, data);
            ALOGE("%s unknow pname %x data %f", __FUNCTION__, pname, *data);
            break;
    }
}

void d_glGetBooleanv_special(void *context, GLenum pname, GLboolean *data) {
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);

    EnableStatus *enable_status =
        &(((GLThreadContext *)context)->enable_status);

    switch (pname) {
        // _____________ OTHERS EDITABLE STATUS BINDING TO OPENGL ES CONTEXT
        
        case GL_SAMPLE_COVERAGE_INVERT:  // glSampleCoverage.
            r_glGetBooleanv(context, GL_SAMPLE_COVERAGE_INVERT, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_COLOR_WRITEMASK:  // glColorMask.
            r_glGetBooleanv(context, GL_SAMPLE_COVERAGE_INVERT, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_DEPTH_WRITEMASK:  // glDepthMask.
            r_glGetBooleanv(context, GL_DEPTH_WRITEMASK, data);
            ALOGE("%s get remote_controller pname %x data %d", __FUNCTION__,
                  pname, *data);
            break;
        case GL_TRANSFORM_FEEDBACK_PAUSED:  // glPauseTransformFeedback.
            *data = bind_status->transform_feedback_paused;
            break;
        case GL_TRANSFORM_FEEDBACK_ACTIVE:  // glBeginTransformFeedback and
                                            // glResumeTransformFeedback.
            *data = bind_status->transform_feedback_active;
            break;

        case GL_CULL_FACE:
            *data = enable_status->cull_face;
            break;
        case GL_POLYGON_OFFSET_FILL:
            *data = enable_status->polygon_offset_fill;
            break;

        case GL_SCISSOR_TEST:
            *data = enable_status->scissor_test;
            break;

        case GL_SAMPLE_COVERAGE:
            *data = enable_status->sample_coverage;
            break;

        case GL_SAMPLE_ALPHA_TO_COVERAGE:
            *data = enable_status->sample_alpha_to_coverage;
            break;

        case GL_STENCIL_TEST:
            *data = enable_status->stencil_test;
            break;

        case GL_DEPTH_TEST:
            *data = enable_status->depth_test;
            break;

        case GL_BLEND:
            *data = enable_status->blend;
            break;

        case GL_PRIMITIVE_RESTART_FIXED_INDEX:
            *data = enable_status->primitive_resstart_fixed_index;
            break;

        case GL_RASTERIZER_DISCARD:
            *data = enable_status->rasterizer_discard;
            break;

        case GL_DITHER:
            *data = enable_status->dither;
            break;

        default:
            r_glGetBooleanv(context, pname, data);
            ALOGE("%s unknow pname %x data %d", __FUNCTION__, pname,
                  (int)*data);
            break;
    }
}

void d_glGetIntegeri_v_special(void *context, GLenum target, GLuint index,
                               GLint *data) {
    StaticContextValues *static_status =
        (((GLThreadContext *)context)->static_values);
    switch (target) {
        case GL_MAX_COMPUTE_WORK_GROUP_COUNT:
            if (index >= 3) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            *data = static_status->max_computer_work_group_count[index];
            break;
        case GL_MAX_COMPUTE_WORK_GROUP_SIZE:
            if (index >= 3) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            *data = static_status->max_computer_work_group_size[index];
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING:
        case GL_UNIFORM_BUFFER_BINDING:
        case GL_ATOMIC_COUNTER_BUFFER_BINDING:
        case GL_SHADER_STORAGE_BUFFER_BINDING:
            *data = get_index_binding_buffer(context, target, index);
            break;
        case GL_VERTEX_BINDING_BUFFER:
        case GL_VERTEX_BINDING_DIVISOR:
        case GL_VERTEX_BINDING_STRIDE:
            *data = (GLuint)get_vertex_binding_status(context, target, index);
            break;
        case GL_IMAGE_BINDING_NAME:
        case GL_IMAGE_BINDING_LEVEL:
        case GL_IMAGE_BINDING_LAYERED:
        case GL_IMAGE_BINDING_LAYER:
        case GL_IMAGE_BINDING_ACCESS:
        case GL_IMAGE_BINDING_FORMAT:
            *data = get_image_info(context, index, target);
            break;
        case GL_VERTEX_BINDING_OFFSET:
            *data = (GLint)get_vertex_binding_status(context, target, index);
            break;
        default:
            ALOGE("%s unknow target %x", __FUNCTION__, target);
            r_glGetIntegeri_v(context, target, index, data);
            break;
    }
}

void d_glGetInteger64i_v_special(void *context, GLenum target, GLuint index,
                                 GLint64 *data) {
    switch (target) {
        // _____________ OTHERS EDITABLE STATUS BINDING TO OPENGL ES CONTEXT
        
        case GL_TRANSFORM_FEEDBACK_BUFFER_START:  // glBindBufferRange.
            ALOGE("%s get remote_controller target %x data %lld", __FUNCTION__,
                  target, (long long)*data);
            r_glGetInteger64i_v(context, target, index, data);
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER_SIZE:  // glBindBufferRange.
            r_glGetInteger64i_v(context, target, index, data);
            ALOGE("%s get remote_controller target %x data %lld", __FUNCTION__,
                  target, (long long)*data);
            break;
        case GL_UNIFORM_BUFFER_START:  // glBindBufferRange.
            r_glGetInteger64i_v(context, target, index, data);
            ALOGE("%s get remote_controller target %x data %lld", __FUNCTION__,
                  target, (long long)*data);
            break;
        case GL_UNIFORM_BUFFER_SIZE:  // glBindBufferRange.
            r_glGetInteger64i_v(context, target, index, data);
            ALOGE("%s get remote_controller target %x data %lld", __FUNCTION__,
                  target, (long long)*data);
            break;
        case GL_VERTEX_BINDING_OFFSET:
            *data = get_vertex_binding_status(context, target, index);
            break;
        default:
            r_glGetInteger64i_v(context, target, index, data);
            ALOGE("%s unknow target %x data %lld", __FUNCTION__, target,
                  (long long)*data);
            break;
    }
}

const GLubyte *d_glGetStringi_special(void *context, GLenum name,
                                      GLuint index) {
    if (name != GL_EXTENSIONS) {  // only accept gl_extentions
        ALOGE("glGetStringi error");

        set_gl_error(context, GL_INVALID_ENUM);
        return NULL;
    }
    //    r_glGetString_special(context,name,index,buffer)

    const GLubyte *ret_string = get_static_string(context, name, index);
    ALOGD("glGetStringi Target is 0x%x index %d : %s", name, index, ret_string);
    return ret_string;
}

const GLubyte *d_glGetString_special(void *context, GLenum name) {
    const GLubyte *ret = get_static_string(context, name);
    ALOGD("glGetString 0x%x: %s", name, ret);
    return ret;
}

void set_gl_error_origin(void *context, GLenum error, const char *fun_name) {
    GLThreadContext *opengl_context = (GLThreadContext *)context;

    switch (error) {
        case GL_INVALID_ENUM:
            ALOGE("glError set GL_INVALID_ENUM %s", fun_name);
            opengl_context->error_status |= (((uint64_t)1) << 0);
            break;
        case GL_INVALID_OPERATION:
            ALOGE("glError set GL_INVALID_OPERATION %s", fun_name);
            opengl_context->error_status |= (((uint64_t)1) << 1);
            break;
        case GL_INVALID_VALUE:
            ALOGE("glError set GL_INVALID_VALUE %s", fun_name);
            opengl_context->error_status |= (((uint64_t)1) << 2);
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            ALOGE("glError set GL_INVALID_FRAMEBUFFER_OPERATION %s", fun_name);
            opengl_context->error_status |= (((uint64_t)1) << 3);
            break;
    }
}

GLenum get_gl_error(void *context) {
    GLThreadContext *opengl_context = (GLThreadContext *)context;
    if (opengl_context->error_status != 0) {
        if (((opengl_context->error_status) & (1 << 0)) != 0) {
            opengl_context->error_status &= ~(((uint64_t)1) << 0);
            return GL_INVALID_ENUM;
        }
        if (((opengl_context->error_status) & (1 << 1)) != 0) {
            opengl_context->error_status &= ~(((uint64_t)1) << 1);
            return GL_INVALID_OPERATION;
        }
        if (((opengl_context->error_status) & (1 << 2)) != 0) {
            opengl_context->error_status &= ~(((uint64_t)1) << 2);
            return GL_INVALID_VALUE;
        }
        if (((opengl_context->error_status) & (1 << 3)) != 0) {
            opengl_context->error_status &= ~(((uint64_t)1) << 3);
            return GL_INVALID_FRAMEBUFFER_OPERATION;
        }
    }
    // GLenum error = r_glGetError(context);
    // if(error != GL_NO_ERROR){
    //     ALOGE("get host error code %x",error);
    // }

    return GL_NO_ERROR;
}

void set_host_error(void *context) {}

void d_glGetBufferParameteriv_special(void *context, GLenum target,
                                      GLenum pname, GLint *params) {
    MapResult *map_res = get_map_result_with_target(context, target);
    if (map_res == NULL) {
        ALOGE("target %x pname %x", target, pname);
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    switch (pname) {
        case GL_BUFFER_ACCESS_FLAGS:
            *params = map_res->access;
            break;
        case GL_BUFFER_MAPPED:
            if (map_res->map_data != NULL) {
                *params = GL_TRUE;
            } else {
                *params = GL_FALSE;
            }
            break;
        case GL_BUFFER_SIZE:
            *params = (GLint)map_res->total_len;
            if (map_res->total_len >= 2147483648) {
                *params = 2147483647;
            }
            break;
        case GL_BUFFER_USAGE:
            *params = map_res->usage;
            break;
        default:
            ALOGE("%s error pname %x target %x", __FUNCTION__, pname, target);
            set_gl_error(context, GL_INVALID_ENUM);
            break;
    }
    ALOGD("%s target %x pname %x params %d", __FUNCTION__, target, pname,
          *params);
}

void d_glGetBufferParameteri64v_special(void *context, GLenum target,
                                        GLenum pname, GLint64 *params) {
    MapResult const *map_res = get_map_result_with_target(context, target);
    if (map_res == nullptr) {
        ALOGE("target %x pname %x", target, pname);
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    switch (pname) {
        case GL_BUFFER_MAP_LENGTH:
            *params = map_res->length;
            break;
        case GL_BUFFER_MAP_OFFSET:
            *params = map_res->offset;
            break;
        case GL_BUFFER_SIZE:
            *params = map_res->total_len;
            if (map_res->total_len >= 2147483648) {
                *params = 2147483647;
            }
            break;
        default:
            ALOGE("%s error pname %x target %x", __FUNCTION__, pname, target);
            set_gl_error(context, GL_INVALID_ENUM);
            break;
    }
    ALOGD("%s target %x pname %x params %lld", __FUNCTION__, target, pname,
          (long long)*params);
}

void d_glBindRenderbuffer_special(void *context, GLenum target,
                                  GLuint renderbuffer) {
    (void)target;
    auto *opengl_context = (GLThreadContext *)context;
    opengl_context->context_binding_status.render_buffer = renderbuffer;
    r_glBindRenderbuffer(context, target, renderbuffer);
}
