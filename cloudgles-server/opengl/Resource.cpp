#define LOG_TAG "GLResource"
#include "Resource.h"

#include <GLES3/gl32.h>
#include <unistd.h>

#include "GLDef.h"
#include "Log.h"
#include "Program.h"
#include "ThreadContext.h"
#include "Utils.h"

ResourceCreator::ResourceCreator(int size) {
    max_id = 0;
    bitmap_size = (size / 8) * 8;
    resource_id_bimap = new unsigned char[size / 8];
    memset(resource_id_bimap, 0, sizeof(unsigned char) * (size / 8));
}

ResourceCreator::ResourceCreator() {
    max_id = 0;
    bitmap_size = (128 / 8) * 8;
    resource_id_bimap = new unsigned char[128 / 8];
    memset(resource_id_bimap, 0, sizeof(unsigned char) * (128 / 8));
}

ResourceCreator::~ResourceCreator() { delete[] resource_id_bimap; }

void ResourceCreator::set_id(unsigned int id) {
    if (id >= this->bitmap_size) {
        unsigned char *temp = this->resource_id_bimap;
        this->resource_id_bimap = new unsigned char[id * 2 / 8];
        memcpy(this->resource_id_bimap, temp, this->bitmap_size / 8);
        memset(this->resource_id_bimap + this->bitmap_size / 8, 0,
               id * 2 / 8 - this->bitmap_size / 8);
        this->bitmap_size = (id * 2 / 8) * 8;
        delete[] temp;
    }
    this->resource_id_bimap[id / 8] |= (1u << (id % 8));
}

bool ResourceCreator::unset_id(unsigned int id) {
    if (id >= this->bitmap_size) {
        return 0;
    }
    bool ret = (this->resource_id_bimap[id / 8] & (1u << (id % 8)));
    this->resource_id_bimap[id / 8] &= (~(1u << (id % 8)));
    return ret;
}

unsigned int ResourceCreator::get_max_id() { return this->bitmap_size; }

unsigned int ResourceCreator::create_id() {
    unsigned int ret_id;

    SPIN_LOCK(this->locker);
    // Delay recycling until there are 32 ids waiting to be recycled to avoid
    // asynchronous deletion of shared context resources, resulting in incorrect
    // resource id mapping on the host side
    if (this->delete_id_queue.size() < 32) {
        this->max_id++;
        while (this->has_id(this->max_id)) {
            this->max_id++;
        }
        this->set_id(this->max_id);
        ret_id = this->max_id;
    } else {
        unsigned int id = this->delete_id_queue.front();
        this->delete_id_queue.pop();
        this->set_id(id);
        ret_id = id;
    }
    SPIN_UNLOCK(this->locker);

    return ret_id;
}

void ResourceCreator::delete_id(unsigned int id) {
    if (id == 0 || id >= this->bitmap_size || !has_id(id)) {
        return;
    }
    SPIN_LOCK(this->locker);
    this->resource_id_bimap[id / 8] &= (~(1u << (id % 8)));
    this->delete_id_queue.push(id);
    SPIN_UNLOCK(this->locker);
}

bool ResourceCreator::has_id(unsigned int id) {
    if (id == 0) {
        return true;
    }
    if (id >= this->bitmap_size) {
        return false;
    }
    return this->resource_id_bimap[id / 8] & (1u << (id % 8));
}

void create_resource_context(ResourceContext *cur_context,
                             ResourceContext *share_context) {
    if (share_context != nullptr) {
        cur_context->share_resources = share_context->share_resources;
    } else {
        cur_context->share_resources = new ShareResource;
        cur_context->share_resources->has_create = 0;
    }
    cur_context->exclusive_resources = new ExclusiveResource;
}

void set_share_context(ResourceContext *cur_context,
                       ResourceContext *share_context) {
    if (share_context != nullptr && cur_context != nullptr) {
        delete cur_context->share_resources;
        cur_context->share_resources = share_context->share_resources;
    } else {
        return;
    }
}

/******* file '2-1-1' *******/

void d_glGenBuffers(void *context, GLsizei n, GLuint *buffers) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    res_ctx->share_resources->has_create = 1;
    for (int i = 0; i < n; i++) {
        buffers[i] = res_ctx->buffer_resource()->create_id();
        ALOGD("glGenbuffer %d context %llx", buffers[i], (long long)context);
    }

    r_glGenBuffers(context, n, buffers);
}

void d_glGenRenderbuffers(void *context, GLsizei n, GLuint *renderbuffers) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    res_ctx->share_resources->has_create = 1;

    for (int i = 0; i < n; i++) {
        renderbuffers[i] = res_ctx->render_buffer_resource()->create_id();
    }

    r_glGenRenderbuffers(context, n, renderbuffers);
}

void d_glGenTextures(void *context, GLsizei n, GLuint *textures) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    res_ctx->share_resources->has_create = 1;

    for (int i = 0; i < n; i++) {
        textures[i] = res_ctx->texture_resource()->create_id();
    }

    r_glGenTextures(context, n, textures);
}

void d_glGenSamplers(void *context, GLsizei count, GLuint *samplers) {
    if (count < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    res_ctx->share_resources->has_create = 1;

    for (int i = 0; i < count; i++) {
        samplers[i] = res_ctx->sampler_resource()->create_id();
    }

    r_glGenSamplers(context, count, samplers);
}

GLuint d_glCreateProgram(void *context) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    GLuint program = res_ctx->program_resource()->create_id();
    res_ctx->share_resources->has_create = 1;

    GLSLProgram::create(context, program);

    ALOGD("%llx create program %u", (long long)context, program);

    r_glCreateProgram(context, program);
    return program;
}

GLuint d_glCreateShader(void *context, GLenum type) {
    if (type != GL_COMPUTE_SHADER && type != GL_VERTEX_SHADER &&
        type != GL_FRAGMENT_SHADER && type != GL_GEOMETRY_SHADER) {
        set_gl_error(context, GL_INVALID_ENUM);
        return 0;
    }

    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    res_ctx->share_resources->has_create = 1;

    GLuint shader = res_ctx->shader_resource()->create_id();
    GLSLShader::create(context, shader, type);

    r_glCreateShader(context, type, shader);
    return shader;
}

GLsync d_glFenceSync(void *context, GLenum condition, GLbitfield flags) {
    if (condition != GL_SYNC_GPU_COMMANDS_COMPLETE) {
        set_gl_error(context, GL_INVALID_ENUM);
        return 0;
    }
    if (flags != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return 0;
    }
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    res_ctx->share_resources->has_create = 1;

    GLsync sync =
        (GLsync)(unsigned long long)res_ctx->sync_resource()->create_id();

    ALOGD("glFenceSync %lld", (long long)sync);
    r_glFenceSync(context, condition, flags, sync);

    return sync;
}

GLuint d_glCreateShaderProgramv(void *context, GLenum type, GLsizei count,
                                const GLchar *const *strings) {
    if (type != GL_COMPUTE_SHADER && type != GL_VERTEX_SHADER &&
        type != GL_FRAGMENT_SHADER) {
        set_gl_error(context, GL_INVALID_ENUM);
        return 0;
    }
    if (count < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return 0;
    }

    auto *opengl_context = (GLThreadContext *)context;
    ResourceContext *res_ctx = &(opengl_context->resource_context);
    res_ctx->share_resources->has_create = 1;

    GLuint program = res_ctx->program_resource()->create_id();
    GLSLProgram::create(context, program);

    int is_external = 0;
    for (int i = 0; i < count; i++) {
        GLSLProgram::replace_standard_derivatives_shader_source(
            (char *)(strings[i]), 0);
        is_external |= GLSLProgram::replace_external_shader_source(
            (char *)(strings[i]), 0);
    }

    GLSLProgram::create_shader_program(context, program, type, count, strings);
    r_glCreateShaderProgramv(context, type, count, strings, program);
    GLSLProgram *program_info = (*opengl_context->program_info_map)[program];
    program_info->is_external = is_external;
    if (!program_info->is_initialized()) program_info->init(context);

    return program;
}

void d_glGenFramebuffers(void *context, GLsizei n, GLuint *framebuffers) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    for (int i = 0; i < n; i++) {
        framebuffers[i] = res_ctx->frame_buffer_resource()->create_id();
    }

    r_glGenFramebuffers(context, n, framebuffers);
}

void d_glGenProgramPipelines(void *context, GLsizei n, GLuint *pipelines) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    for (int i = 0; i < n; i++) {
        pipelines[i] = res_ctx->program_pipeline_resource()->create_id();
    }

    r_glGenProgramPipelines(context, n, pipelines);
}

void d_glGenTransformFeedbacks(void *context, GLsizei n, GLuint *ids) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    for (int i = 0; i < n; i++) {
        ids[i] = res_ctx->transform_feedback_resource()->create_id();
    }

    r_glGenTransformFeedbacks(context, n, ids);
}

void d_glGenVertexArrays(void *context, GLsizei n, GLuint *arrays) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    for (int i = 0; i < n; i++) {
        arrays[i] = res_ctx->vertex_array_resource()->create_id();
    }

    d_glGenVertexArrays_special(context, n, arrays);
}

void d_glGenQueries(void *context, GLsizei n, GLuint *ids) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    for (int i = 0; i < n; i++) {
        ids[i] = res_ctx->query_resource()->create_id();
    }

    r_glGenQueries(context, n, ids);
}

void d_glDeleteBuffers(void *context, GLsizei n, const GLuint *buffers) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    auto *opengl_context = (GLThreadContext *)context;

    ResourceContext *res_ctx = &(opengl_context->resource_context);
    int should_delete = 0;
    for (int i = 0; i < n; i++) {
        if (buffers[i] != 0) {
            res_ctx->buffer_resource()->delete_id(buffers[i]);
            should_delete = 1;
        }
    }

    if (should_delete == 1) {
        d_glDeleteBuffers_special(context, n, buffers);
    }
}

void d_glDeleteRenderbuffers(void *context, GLsizei n,
                             const GLuint *renderbuffers) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    auto *opengl_context = (GLThreadContext *)context;

    BindingStatus *bind_status = &(opengl_context->context_binding_status);

    ResourceContext *res_ctx = &(opengl_context->resource_context);

    int should_delete = 0;
    for (int i = 0; i < n; i++) {
        if (renderbuffers[i] != 0) {
            res_ctx->render_buffer_resource()->delete_id(renderbuffers[i]);
            if (renderbuffers[i] == bind_status->render_buffer) {
                bind_status->render_buffer = 0;
            }
            should_delete = 1;
            FrameBufferInfo::detach_renderbuffer(renderbuffers[i]);
        }
    }
    if (should_delete == 1) {
        r_glDeleteRenderbuffers(context, n, renderbuffers);
    }
}

void d_glDeleteTextures(void *context, GLsizei n, const GLuint *textures) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    // ALOGD("GlDeleteTextures %d %u",n,textures[0]);

    auto *opengl_context = (GLThreadContext *)context;

    BindingStatus *binding_status = &(opengl_context->context_binding_status);

    ResourceContext *res_ctx = &(opengl_context->resource_context);

    int should_delete = 0;

    for (int i = 0; i < n; i++) {
        if (textures[i] != 0) {
            should_delete = 1;
            res_ctx->texture_resource()->delete_id(textures[i]);
            if (res_ctx->texture_has_gbuffer()->unset_id(textures[i])) {
                ALOGE("erase texture_gbuffer with texture id %d", textures[i]);
                EGLImageInfo *image_info =
                    opengl_context->texture_gbuffer_map[0][textures[i]];
                image_info->using_cnt--;
                try_destroy_EGLImage(context, image_info);
                opengl_context->texture_gbuffer_map[0].erase(textures[i]);
            }

            if (textures[i] ==
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d) {
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d = 0;
            }
            if (textures[i] ==
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_cube_map) {
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_cube_map = 0;
            }
            if (textures[i] ==
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_3d) {
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_3d = 0;
            }
            if (textures[i] ==
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_array) {
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_array = 0;
            }
            if (textures[i] ==
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_multisample) {
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_multisample = 0;
            }
            if (textures[i] ==
                binding_status->texture_binding[0].texture_binding_external) {
                binding_status->texture_binding[0].texture_binding_external = 0;
            }
            if (textures[i] ==
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_multisample_array) {
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_multisample_array = 0;
            }
            if (textures[i] ==
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_cube_map_array) {
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_cube_map_array = 0;
            }
            if (textures[i] ==
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_texture_buffer) {
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_texture_buffer = 0;
            }

            FrameBufferInfo::detach_texture(textures[i]);

            if (((GLThreadContext *)context)
                    ->texture_info_map->find(textures[i]) !=
                ((GLThreadContext *)context)->texture_info_map->end()) {
                TextureInfo *info = ((GLThreadContext *)context)
                                        ->texture_info_map[0][textures[i]];
                ((GLThreadContext *)context)
                    ->texture_info_map->erase(textures[i]);
                ALOGD("delete texture %u info %llx", textures[i],
                      (unsigned long long)info);
                delete info;
            }
        }
    }

    if (should_delete == 1) {
        r_glDeleteTextures(context, n, textures);
    }
}

void d_glDeleteSamplers(void *context, GLsizei count, const GLuint *samplers) {
    if (count < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    auto *opengl_context = (GLThreadContext *)context;

    BindingStatus *bind_status = &(opengl_context->context_binding_status);

    ResourceContext *res_ctx = &(opengl_context->resource_context);
    int should_delete = 0;

    for (int i = 0; i < count; i++) {
        if (samplers[i] != 0) {
            should_delete = 1;
            res_ctx->sampler_resource()->delete_id(samplers[i]);
            for (int j = 0; j < opengl_context->static_values
                                    ->max_combined_texture_image_units;
                 j++) {
                if (samplers[i] == bind_status->sampler_binding_2d[j]) {
                    bind_status->sampler_binding_2d[j] = 0;
                }
            }
        }
    }
    if (should_delete == 1) {
        r_glDeleteSamplers(context, count, samplers);
    }
}

void d_glDeleteProgram(void *context, GLuint program) {
    if (program == 0) {
        return;
    }
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    res_ctx->program_resource()->delete_id(program);

    GLSLProgram::destroy(context, program);
    r_glDeleteProgram_origin(context, program);
}

void d_glDeleteShader(void *context, GLuint shader) {
    if (shader == 0) {
        return;
    }
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    res_ctx->shader_resource()->delete_id(shader);
    res_ctx->shader_is_external()->unset_id(shader);

    GLSLShader::destroy(context, shader);

    r_glDeleteShader(context, shader);
}

void d_glDeleteSync(void *context, GLsync sync) {
    if (sync == 0) {
        return;
    }
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    res_ctx->sync_resource()->delete_id((GLuint)(unsigned long long)sync);
    r_glDeleteSync(context, sync);

    
    
    //  if(((GLThreadContext *)context)->share_opengl_context!=NULL)
    //  {
    //      r_glSync(context);
    //  }
}

void d_glDeleteFramebuffers(void *context, GLsizei n,
                            const GLuint *framebuffers) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    auto *opengl_context = (GLThreadContext *)context;

    BindingStatus *bind_status = &(opengl_context->context_binding_status);

    ResourceContext *res_ctx = &(opengl_context->resource_context);

    int should_delete = 0;

    int draw_need_bind0 = 0;
    int read_need_bind0 = 0;

    for (int i = 0; i < n; i++) {
        if (framebuffers[i] != 0) {
            should_delete = 1;
            res_ctx->frame_buffer_resource()->delete_id(framebuffers[i]);
            if (framebuffers[i] == bind_status->draw_frame_buffer) {
                ALOGE("glDeleteframebuffer with draw bind %u", framebuffers[i]);
                bind_status->draw_frame_buffer = 0;
                draw_need_bind0 = 1;
            }
            if (framebuffers[i] == bind_status->read_frame_buffer) {
                ALOGE("glDeleteframebuffer with read bind %u", framebuffers[i]);
                bind_status->read_frame_buffer = 0;
                read_need_bind0 = 1;
            }
        }
    }
    if (should_delete == 1) {
        r_glDeleteFramebuffers(context, n, framebuffers);
    }

    if (draw_need_bind0 == 1 && read_need_bind0 == 1) {
        r_glBindFramebuffer(context, GL_FRAMEBUFFER, 0);
    } else if (draw_need_bind0 == 1) {
        r_glBindFramebuffer(context, GL_READ_FRAMEBUFFER, 0);
    } else if (read_need_bind0 == 0) {
        r_glBindFramebuffer(context, GL_DRAW_FRAMEBUFFER, 0);
    }
}

void d_glDeleteProgramPipelines(void *context, GLsizei n,
                                const GLuint *pipelines) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    auto *opengl_context = (GLThreadContext *)context;

    BindingStatus *bind_status = &(opengl_context->context_binding_status);

    ResourceContext *res_ctx = &(opengl_context->resource_context);

    int should_delete = 0;

    for (int i = 0; i < n; i++) {
        if (pipelines[i] != 0) {
            res_ctx->program_pipeline_resource()->delete_id(pipelines[i]);
            // if(pipelines[i]==bind_status->)
            // todo
            should_delete = 1;
        }
    }

    if (should_delete == 1) {
        r_glDeleteProgramPipelines(context, n, pipelines);
    }
}

void d_glDeleteTransformFeedbacks(void *context, GLsizei n, const GLuint *ids) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    auto *opengl_context = (GLThreadContext *)context;

    BindingStatus *bind_status = &(opengl_context->context_binding_status);

    ResourceContext *res_ctx = &(opengl_context->resource_context);

    int should_delete = 0;

    for (int i = 0; i < n; i++) {
        if (ids[i] != 0) {
            should_delete = 1;
            res_ctx->transform_feedback_resource()->delete_id(ids[i]);
            if (ids[i] == bind_status->transform_feedback_binding) {
                bind_status->transform_feedback_binding = 0;
            }
        }
    }

    if (should_delete == 1) {
        r_glDeleteTransformFeedbacks(context, n, ids);
    }
}

void d_glDeleteVertexArrays(void *context, GLsizei n, const GLuint *arrays) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    auto *opengl_context = (GLThreadContext *)context;

    // Buffer_Status *buffer_status =
    // &(opengl_context->bound_buffer)->buffer_status; todo

    ResourceContext *res_ctx = &(opengl_context->resource_context);

    int should_delete = 0;

    for (int i = 0; i < n; i++) {
        if (arrays[i] != 0) {
            should_delete = 1;
            res_ctx->vertex_array_resource()->delete_id(arrays[i]);
        }
    }

    if (should_delete == 1) {
        d_glDeleteVertexArrays_special(context, n, arrays);
    }
}

void d_glDeleteQueries(void *context, GLsizei n, const GLuint *ids) {
    if (n < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    auto *opengl_context = (GLThreadContext *)context;

    BindingStatus *bind_status = &(opengl_context->context_binding_status);

    ResourceContext *res_ctx = &(opengl_context->resource_context);

    int should_delete = 0;

    for (int i = 0; i < n; i++) {
        if (ids[i] != 0) {
            should_delete = 1;
            res_ctx->query_resource()->delete_id(ids[i]);
            opengl_context->query_type_map.erase(ids[i]);
        }
    }

    if (should_delete == 1) {
        r_glDeleteQueries(context, n, ids);
    }
}

GLboolean d_glIsBuffer(void *context, GLuint buffer) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->buffer_resource()->has_id(buffer);
}

GLboolean d_glIsProgramPipeline(void *context, GLuint pipeline) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->program_pipeline_resource()->has_id(pipeline);
}

GLboolean d_glIsEnabled(void *context, GLenum cap) {
    EnableStatus *status = &(((GLThreadContext *)context)->enable_status);

    GLboolean ret = GL_FALSE;

    switch (cap) {
        case GL_CULL_FACE:
            ret = status->cull_face;
            break;
        case GL_POLYGON_OFFSET_FILL:
            ret = status->polygon_offset_fill;
            break;

        case GL_SCISSOR_TEST:
            ret = status->scissor_test;
            break;

        case GL_SAMPLE_COVERAGE:
            ret = status->sample_coverage;
            break;

        case GL_SAMPLE_ALPHA_TO_COVERAGE:
            ret = status->sample_alpha_to_coverage;
            break;

        case GL_STENCIL_TEST:
            ret = status->stencil_test;
            break;

        case GL_DEPTH_TEST:
            ret = status->depth_test;
            break;

        case GL_BLEND:
            ret = status->blend;
            break;

        case GL_PRIMITIVE_RESTART_FIXED_INDEX:
            ret = status->primitive_resstart_fixed_index;
            break;

        case GL_RASTERIZER_DISCARD:
            ret = status->rasterizer_discard;
            break;

        case GL_DITHER:
            ret = status->dither;
            break;

        case GL_TEXTURE_2D:
            ret = status->texture_2d;
            break;

        default:
            ALOGE("isEnable error cap %x", cap);
            set_gl_error(context, GL_INVALID_ENUM);
            return GL_FALSE;
    }

    return ret;
}

GLboolean d_glIsFramebuffer(void *context, GLuint framebuffer) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->frame_buffer_resource()->has_id(framebuffer);
}

GLboolean d_glIsProgram(void *context, GLuint program) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->program_resource()->has_id(program);
}

GLboolean d_glIsRenderbuffer(void *context, GLuint renderbuffer) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->render_buffer_resource()->has_id(renderbuffer);
}

GLboolean d_glIsShader(void *context, GLuint shader) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->shader_resource()->has_id(shader);
}

GLboolean d_glIsTexture(void *context, GLuint texture) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->texture_resource()->has_id(texture);
}

GLboolean d_glIsQuery(void *context, GLuint id) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->query_resource()->has_id(id);
}

GLboolean d_glIsVertexArray(void *context, GLuint array) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->vertex_array_resource()->has_id(array);
}

GLboolean d_glIsSampler(void *context, GLuint sampler) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->sampler_resource()->has_id(sampler);
}

GLboolean d_glIsTransformFeedback(void *context, GLuint id) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->transform_feedback_resource()->has_id(id);
}

GLboolean d_glIsSync(void *context, GLsync sync) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    return res_ctx->sync_resource()->has_id((GLuint)(unsigned long long)sync);
}
