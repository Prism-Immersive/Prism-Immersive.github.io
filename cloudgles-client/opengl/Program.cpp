#include "Program.h"

#include <EGL/egl.h>
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

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include "Context.h"
#include "Log.h"
#include "ProgramData/AtomicCounterBuffer_generated.h"
#include "ProgramData/Attribute_generated.h"
#include "ProgramData/BufferVariable_generated.h"
#include "ProgramData/ProgramOutput_generated.h"
#include "ProgramData/ShaderStorageBlock_generated.h"
#include "ProgramData/TransformFeedbackVarying_generated.h"
#include "ProgramData/UniformBlock_generated.h"
#include "ProgramData/Uniform_generated.h"
#include "Resource.h"

#define LOG_TAG "GLProgram"

std::unordered_map<GLuint, GLuint> GLProgram::program_is_external_map{};
std::unordered_map<GLuint, GLchar *> GLProgram::program_data_map{};

void GLProgram::d_glLinkProgram_special(void *context, GLuint program) {
    glLinkProgram(program);
}

/**
 * Get the specific uniform information
 * @param program The program id
 * @param num_uniforms The number of uniforms
 * @param fbb The flatbuffer builder
 */
void GLProgram::init_uniform_data(GLuint program, int num_uniforms,
                                  int max_name_len, int *has_image,
                                  flatbuffers::FlatBufferBuilder &fbb) {
    UniformsT uniforms;
    int detected_id = num_uniforms;
    *has_image = 0;
    std::string query_buf;
    std::unique_ptr<char[]> name = std::make_unique<char[]>(max_name_len);

    const GLenum props[] = {
        GL_OFFSET,
        GL_BLOCK_INDEX,
        GL_ARRAY_STRIDE,
        GL_MATRIX_STRIDE,
        GL_IS_ROW_MAJOR,
        GL_ATOMIC_COUNTER_BUFFER_INDEX,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
    };
    constexpr int prop_count = sizeof(props) / sizeof(GLenum);
    int params[prop_count] = {0};
    for (int i = 0; i < num_uniforms; i++) {
        std::unique_ptr<UniformT> uniform = std::make_unique<UniformT>();
        glGetActiveUniform(program, i, max_name_len, nullptr, &uniform->size,
                           reinterpret_cast<GLenum *>(&uniform->type),
                           name.get());
        uniform->name = name.get();
        uniform->location = glGetUniformLocation(program, name.get());

        if (strstr(name.get(), "has_EGL_image_external") != NULL) {
            program_is_external_map[program] = 1;
            *has_image = 1;
            continue;
        }
        uniform->index = i - *has_image;

        glGetProgramResourceiv(program, GL_UNIFORM, i, prop_count, props,
                               prop_count, nullptr, params);
        uniform->offset = params[0];
        uniform->block_index = params[1];
        uniform->array_stride = params[2];
        uniform->matrix_stride = params[3];
        uniform->is_row_major = params[4];
        uniform->atomic_counter_buffer_index = params[5];
        uniform->ref_by_vertex_shader = params[6];
        uniform->ref_by_frag_shader = params[7];
        uniform->ref_by_compute_shader = params[8];
        uniform->ref_by_tess_control_shader = params[9];
        uniform->ref_by_tess_eval_shader = params[10];
        uniform->ref_by_geometry_shader = params[11];

        ALOGD("uniform |%d %d| |%s|\n", location, type, name_buf);

        // If the name ends with "[0]", it is an array
        if (uniform->name.length() > 3 &&
            uniform->name.substr(uniform->name.length() - 3) == "[0]" &&
            uniform->size > 1) {
            std::string base_name =
                uniform->name.substr(0, uniform->name.length() - 3);
            for (int j = 1; j < uniform->size; j++) {
                std::unique_ptr<UniformT> detected_uniform =
                    std::make_unique<UniformT>(*uniform);
                query_buf = base_name + "[" + std::to_string(j) + "]";
                detected_uniform->location =
                    glGetUniformLocation(program, query_buf.c_str());
                detected_uniform->index = detected_id - *has_image;
                detected_uniform->name = query_buf;
                detected_id++;
                uniforms.uniforms.push_back(std::move(detected_uniform));
            }
        }

        uniforms.uniforms.push_back(std::move(uniform));
    }

    // Sort uniform array by index
    std::sort(
        uniforms.uniforms.begin(), uniforms.uniforms.end(),
        [](const std::unique_ptr<UniformT> &a,
           const std::unique_ptr<UniformT> &b) { return a->index < b->index; });

    fbb.Finish(Uniforms::Pack(fbb, &uniforms));
}

void GLProgram::init_attribute_data(GLuint program, int num_attributes,
                                    int max_name_len,
                                    flatbuffers::FlatBufferBuilder &fbb) {
    AttributesT attributes;
    std::unique_ptr<char[]> name = std::make_unique<char[]>(max_name_len);

    const GLenum props[] = {
        GL_IS_PER_PATCH,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
    };
    constexpr int prop_count = sizeof(props) / sizeof(GLenum);
    int params[prop_count] = {0};
    for (int i = 0; i < num_attributes; i++) {
        std::unique_ptr<AttributeT> attribute = std::make_unique<AttributeT>();
        glGetActiveAttrib(program, i, max_name_len, nullptr, &attribute->size,
                          reinterpret_cast<GLenum *>(&attribute->type),
                          name.get());
        attribute->index = i;
        attribute->name = name.get();
        attribute->location = glGetAttribLocation(program, name.get());

        glGetProgramResourceiv(program, GL_PROGRAM_INPUT, i, prop_count, props,
                               prop_count, nullptr, params);
        attribute->is_per_patch = params[0];
        attribute->ref_by_vertex_shader = params[1];
        attribute->ref_by_frag_shader = params[2];
        attribute->ref_by_compute_shader = params[3];
        attribute->ref_by_tess_control_shader = params[4];
        attribute->ref_by_tess_eval_shader = params[5];
        attribute->ref_by_geometry_shader = params[6];

        attributes.attrs.push_back(std::move(attribute));

        ALOGD("attrib |%d %d| |%s|\n", location, type, name_buf);
    }

    fbb.Finish(Attributes::Pack(fbb, &attributes));
}

void GLProgram::init_uniform_block_data(GLuint program, int num_uniform_blocks,
                                        int max_name_len,
                                        flatbuffers::FlatBufferBuilder &fbb) {
    UniformBlocksT uniform_blocks;
    std::unique_ptr<char[]> name = std::make_unique<char[]>(max_name_len);

    const GLenum props[] = {
        GL_NUM_ACTIVE_VARIABLES,
        GL_BUFFER_BINDING,
        GL_BUFFER_DATA_SIZE,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
    };
    constexpr int prop_count = sizeof(props) / sizeof(GLenum);
    int params[prop_count] = {0};
    for (int i = 0; i < num_uniform_blocks; i++) {
        std::unique_ptr<UniformBlockT> uniform_block =
            std::make_unique<UniformBlockT>();
        glGetActiveUniformBlockName(program, i, max_name_len, nullptr,
                                    name.get());
        int length = 0;
        glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, i, prop_count, props,
                               prop_count, &length, params);
        if (length != prop_count) {
            ALOGE("Get uniform block info failed\n");
            continue;
        }

        uniform_block->index = i;
        uniform_block->name = name.get();
        uniform_block->size = params[0];
        uniform_block->buffer_binding = params[1];
        uniform_block->buffer_data_size = params[2];
        uniform_block->ref_by_vertex_shader = params[3];
        uniform_block->ref_by_frag_shader = params[4];
        uniform_block->ref_by_compute_shader = params[5];
        uniform_block->ref_by_tess_control_shader = params[6];
        uniform_block->ref_by_tess_eval_shader = params[7];
        uniform_block->ref_by_geometry_shader = params[8];

        GLenum active_variable_prop = GL_ACTIVE_VARIABLES;
        uniform_block->active_variables.resize(uniform_block->size);
        glGetProgramResourceiv(program, GL_UNIFORM_BLOCK, i, 1, &active_variable_prop,
                               uniform_block->size, nullptr, uniform_block->active_variables.data());

        ALOGD("uniform block |%d %d| |%s| index %d\n", block_active_uniforms,
              size, name_buf, i);

        uniform_blocks.blocks.push_back(std::move(uniform_block));
    }

    fbb.Finish(UniformBlocks::Pack(fbb, &uniform_blocks));
}

void GLProgram::init_program_output_data(GLuint program,
                                         int num_program_outputs,
                                         flatbuffers::FlatBufferBuilder &fbb) {
    ProgramOutputsT program_outputs;
    int max_name_len = 0;
    for (int i = 0; i < num_program_outputs; i++) {
        GLint len = 0;
        GLenum name_len_enum = GL_NAME_LENGTH;
        glGetProgramResourceiv(program, GL_PROGRAM_OUTPUT, i, 1, &name_len_enum,
                               1, NULL, &len);
        max_name_len = std::max(max_name_len, len);
    }
    std::unique_ptr<char[]> name = std::make_unique<char[]>(max_name_len);

    const GLenum props[] = {
        GL_ARRAY_SIZE,
        GL_TYPE,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
        GL_IS_PER_PATCH,
        GL_LOCATION,
        GL_LOCATION_INDEX_EXT,
    };
    constexpr int prop_count = sizeof(props) / sizeof(GLenum);
    int params[prop_count] = {0};
    for (int i = 0; i < num_program_outputs; i++) {
        std::unique_ptr<ProgramOutputT> program_output =
            std::make_unique<ProgramOutputT>();
        glGetProgramResourceName(program, GL_PROGRAM_OUTPUT, i, max_name_len,
                                 nullptr, name.get());
        glGetProgramResourceiv(program, GL_PROGRAM_OUTPUT, i, prop_count, props,
                               prop_count, nullptr, params);
        program_output->index = i;
        program_output->name = name.get();
        program_output->size = params[0];
        program_output->type = params[1];
        program_output->ref_by_vertex_shader = params[2];
        program_output->ref_by_frag_shader = params[3];
        program_output->ref_by_compute_shader = params[4];
        program_output->ref_by_tess_control_shader = params[5];
        program_output->ref_by_tess_eval_shader = params[6];
        program_output->ref_by_geometry_shader = params[7];
        program_output->is_per_patch = params[8];
        program_output->location = params[9];
        program_output->location_index = params[10];

        program_outputs.outputs.push_back(std::move(program_output));
    }

    fbb.Finish(ProgramOutputs::Pack(fbb, &program_outputs));
}

void GLProgram::init_buffer_variable_data(GLuint program,
                                          int num_buffer_variables,
                                          flatbuffers::FlatBufferBuilder &fbb) {
    BufferVariablesT buffer_variables;
    int max_name_len = 0;
    for (int i = 0; i < num_buffer_variables; i++) {
        GLint len = 0;
        GLenum name_len_enum = GL_NAME_LENGTH;
        glGetProgramResourceiv(program, GL_BUFFER_VARIABLE, i, 1,
                               &name_len_enum, 1, NULL, &len);
        max_name_len = std::max(max_name_len, len);
    }
    std::unique_ptr<char[]> name = std::make_unique<char[]>(max_name_len);

    GLenum props[] = {
        GL_TYPE,
        GL_ARRAY_SIZE,
        GL_OFFSET,
        GL_BLOCK_INDEX,
        GL_ARRAY_STRIDE,
        GL_MATRIX_STRIDE,
        GL_IS_ROW_MAJOR,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
        GL_TOP_LEVEL_ARRAY_SIZE,
        GL_TOP_LEVEL_ARRAY_STRIDE,
    };
    constexpr int prop_count = sizeof(props) / sizeof(GLenum);
    int params[prop_count] = {0};
    for (int i = 0; i < num_buffer_variables; i++) {
        std::unique_ptr<BufferVariableT> buffer_variable =
            std::make_unique<BufferVariableT>();
        glGetProgramResourceName(program, GL_BUFFER_VARIABLE, i, max_name_len,
                                 nullptr, name.get());
        glGetProgramResourceiv(program, GL_BUFFER_VARIABLE, i, prop_count,
                               props, prop_count, nullptr, params);
        buffer_variable->index = i;
        buffer_variable->name = name.get();
        buffer_variable->type = params[0];
        buffer_variable->size = params[1];
        buffer_variable->offset = params[2];
        buffer_variable->block_index = params[3];
        buffer_variable->array_stride = params[4];
        buffer_variable->matrix_stride = params[5];
        buffer_variable->is_row_major = params[6];
        buffer_variable->ref_by_vertex_shader = params[7];
        buffer_variable->ref_by_frag_shader = params[8];
        buffer_variable->ref_by_compute_shader = params[9];
        buffer_variable->ref_by_tess_control_shader = params[10];
        buffer_variable->ref_by_tess_eval_shader = params[11];
        buffer_variable->ref_by_geometry_shader = params[12];
        buffer_variable->top_level_array_size = params[13];
        buffer_variable->top_level_array_stride = params[14];

        buffer_variables.variables.push_back(std::move(buffer_variable));
    }

    fbb.Finish(BufferVariables::Pack(fbb, &buffer_variables));
}

void GLProgram::init_transform_feedback_varying_data(
    GLuint program, int num_transform_feedback_varyings,
    flatbuffers::FlatBufferBuilder &fbb) {
    TransformFeedbackVaryingsT transform_feedback_varyings;
    int max_name_len = 0;
    for (int i = 0; i < num_transform_feedback_varyings; i++) {
        GLint len = 0;
        GLenum name_len_enum = GL_NAME_LENGTH;
        glGetProgramResourceiv(program, GL_TRANSFORM_FEEDBACK_VARYING, i, 1,
                               &name_len_enum, 1, NULL, &len);
        max_name_len = std::max(max_name_len, len);
    }
    std::unique_ptr<char[]> name = std::make_unique<char[]>(max_name_len);

    const GLenum props[] = {
        GL_TYPE,
        GL_ARRAY_SIZE,
        GL_OFFSET,
    };
    constexpr int prop_count = sizeof(props) / sizeof(GLenum);
    int params[prop_count] = {0};
    for (int i = 0; i < num_transform_feedback_varyings; i++) {
        std::unique_ptr<TransformFeedbackVaryingT> transform_feedback_varying =
            std::make_unique<TransformFeedbackVaryingT>();
        glGetProgramResourceName(program, GL_TRANSFORM_FEEDBACK_VARYING, i,
                                 max_name_len, nullptr, name.get());
        glGetProgramResourceiv(program, GL_TRANSFORM_FEEDBACK_VARYING, i,
                               prop_count, props, prop_count, nullptr, params);
        transform_feedback_varying->index = i;
        transform_feedback_varying->name = name.get();
        transform_feedback_varying->type = params[0];
        transform_feedback_varying->size = params[1];
        transform_feedback_varying->offset = params[2];
        transform_feedback_varying->location = glGetProgramResourceLocation(
            program, GL_TRANSFORM_FEEDBACK_VARYING, name.get());

        transform_feedback_varyings.varyings.push_back(
            std::move(transform_feedback_varying));
    }

    fbb.Finish(
        TransformFeedbackVaryings::Pack(fbb, &transform_feedback_varyings));
}

void GLProgram::init_shader_storage_block_data(
    GLuint program, int num_shader_storage_blocks,
    flatbuffers::FlatBufferBuilder &fbb) {
    ShaderStorageBlocksT shader_storage_blocks;
    int max_name_len = 0;
    for (int i = 0; i < num_shader_storage_blocks; i++) {
        GLint len = 0;
        GLenum name_len_enum = GL_NAME_LENGTH;
        glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, i, 1,
                               &name_len_enum, 1, NULL, &len);
        max_name_len = std::max(max_name_len, len);
    }
    std::unique_ptr<char[]> name = std::make_unique<char[]>(max_name_len);

    GLenum props[] = {
        GL_BUFFER_BINDING,
        GL_BUFFER_DATA_SIZE,
        GL_NUM_ACTIVE_VARIABLES,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
    };
    constexpr int prop_count = sizeof(props) / sizeof(GLenum);
    int params[prop_count] = {0};

    for (int i = 0; i < num_shader_storage_blocks; i++) {
        std::unique_ptr<ShaderStorageBlockT> shader_storage_block =
            std::make_unique<ShaderStorageBlockT>();
        glGetProgramResourceName(program, GL_SHADER_STORAGE_BLOCK, i,
                                 max_name_len, nullptr, name.get());
        glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, i, prop_count,
                               props, prop_count, nullptr, params);
        shader_storage_block->index = i;
        shader_storage_block->name = name.get();
        shader_storage_block->buffer_binding = params[0];
        shader_storage_block->buffer_data_size = params[1];
        shader_storage_block->num_active_variables = params[2];
        shader_storage_block->ref_by_vertex_shader = params[3];
        shader_storage_block->ref_by_frag_shader = params[4];
        shader_storage_block->ref_by_compute_shader = params[5];
        shader_storage_block->ref_by_tess_control_shader = params[6];
        shader_storage_block->ref_by_tess_eval_shader = params[7];
        shader_storage_block->ref_by_geometry_shader = params[8];

        GLenum active_variable_prop = GL_ACTIVE_VARIABLES;
        shader_storage_block->active_variables.resize(shader_storage_block->num_active_variables);
        glGetProgramResourceiv(program, GL_SHADER_STORAGE_BLOCK, i, 1, &active_variable_prop,
                               shader_storage_block->num_active_variables, nullptr, shader_storage_block->active_variables.data());

        shader_storage_blocks.blocks.push_back(std::move(shader_storage_block));
    }

    fbb.Finish(ShaderStorageBlocks::Pack(fbb, &shader_storage_blocks));
}

void GLProgram::init_atomic_counter_buffer_data(
    GLuint program, int num_atomic_counter_buffers,
    flatbuffers::FlatBufferBuilder &fbb) {
    AtomicCounterBuffersT atomic_counter_buffers;

    GLenum props[] = {
        GL_BUFFER_BINDING,
        GL_BUFFER_DATA_SIZE,
        GL_NUM_ACTIVE_VARIABLES,
        GL_REFERENCED_BY_VERTEX_SHADER,
        GL_REFERENCED_BY_FRAGMENT_SHADER,
        GL_REFERENCED_BY_COMPUTE_SHADER,
        GL_REFERENCED_BY_TESS_CONTROL_SHADER,
        GL_REFERENCED_BY_TESS_EVALUATION_SHADER,
        GL_REFERENCED_BY_GEOMETRY_SHADER,
    };
    constexpr int prop_count = sizeof(props) / sizeof(GLenum);
    int params[prop_count] = {0};

    for (int i = 0; i < num_atomic_counter_buffers; i++) {
        std::unique_ptr<AtomicCounterBufferT> atomic_counter_buffer =
            std::make_unique<AtomicCounterBufferT>();
        glGetProgramResourceiv(program, GL_ATOMIC_COUNTER_BUFFER, i, prop_count,
                               props, prop_count, nullptr, params);
        atomic_counter_buffer->index = i;
        atomic_counter_buffer->buffer_binding = params[0];
        atomic_counter_buffer->buffer_data_size = params[1];
        atomic_counter_buffer->num_active_variables = params[2];
        atomic_counter_buffer->ref_by_vertex_shader = params[3];
        atomic_counter_buffer->ref_by_frag_shader = params[4];
        atomic_counter_buffer->ref_by_compute_shader = params[5];
        atomic_counter_buffer->ref_by_tess_control_shader = params[6];
        atomic_counter_buffer->ref_by_tess_eval_shader = params[7];
        atomic_counter_buffer->ref_by_geometry_shader = params[8];

        GLenum active_variable_prop = GL_ACTIVE_VARIABLES;
        atomic_counter_buffer->active_variables.resize(atomic_counter_buffer->num_active_variables);
        glGetProgramResourceiv(program, GL_ATOMIC_COUNTER_BUFFER, i, 1, &active_variable_prop,
                               atomic_counter_buffer->num_active_variables, nullptr, atomic_counter_buffer->active_variables.data());

        atomic_counter_buffers.buffers.push_back(
            std::move(atomic_counter_buffer));
    }

    fbb.Finish(AtomicCounterBuffers::Pack(fbb, &atomic_counter_buffers));
}

int GLProgram::init_program_data(GLuint program) {
    GLint link_status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);

    if (link_status == 0) {
        GLint infoLen = 0;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

        ALOGD("GL_INFO_LOG_LENGTH %d\n", infoLen);

        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog);
            ALOGE("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }

        return 0;
    }

    int binary_len = 0;
    glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &binary_len);

    // The number of active uniforms and attributes
    GLint uniform_gl_num = 0;
    GLint attrib_gl_num = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniform_gl_num);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attrib_gl_num);

    // The maximum length of the uniform and attribute names
    GLint max_uniform_gl_name_len = 0;
    GLint max_attrib_gl_name_len = 0;
    GLint max_uniform_block_name_len = 0;

    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                   &max_uniform_gl_name_len);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                   &max_attrib_gl_name_len);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,
                   &max_uniform_block_name_len);

    // The number of uniform blocks and transform feedback varyings
    GLint uniform_blocks_num = 0;
    GLint transform_feedback_varyings = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_blocks_num);
    glGetProgramiv(program, GL_TRANSFORM_FEEDBACK_VARYINGS,
                   &transform_feedback_varyings);

    GLint group_size[3] = {0, 0, 0};

    GLuint shader_ids[10];
    GLsizei shader_cnt = 0;
    glGetAttachedShaders(program, 10, &shader_cnt, shader_ids);
    // Compute shader is the only shader in its attached program
    if (shader_cnt == 1) {
        GLint shader_type;
        glGetShaderiv(shader_ids[0], GL_SHADER_TYPE, &shader_type);
        if (shader_type == GL_COMPUTE_SHADER) {
            glGetProgramiv(program, GL_COMPUTE_WORK_GROUP_SIZE, group_size);
        }
    }

    int max_name_len_all = max_uniform_gl_name_len > max_attrib_gl_name_len
                               ? max_uniform_gl_name_len
                               : max_attrib_gl_name_len;
    max_name_len_all = max_name_len_all > max_uniform_block_name_len
                           ? max_name_len_all
                           : max_uniform_block_name_len;
    max_name_len_all += 1;

    int buf_len = 11 * sizeof(int);
    int write_offset = buf_len;
    char *program_data = (char *)malloc(buf_len);
    int *program_basic_info = (int *)program_data;
    program_basic_info[0] = uniform_gl_num;
    program_basic_info[1] = attrib_gl_num;
    program_basic_info[2] = uniform_blocks_num;
    program_basic_info[3] = max_uniform_gl_name_len;
    program_basic_info[4] = max_attrib_gl_name_len;
    program_basic_info[5] = max_uniform_block_name_len;
    program_basic_info[6] = transform_feedback_varyings;
    program_basic_info[7] = group_size[0];
    program_basic_info[8] = group_size[1];
    program_basic_info[9] = group_size[2];
    program_basic_info[10] = binary_len;

    flatbuffers::FlatBufferBuilder fbb;
    int has_image = 0;
    init_uniform_data(program, uniform_gl_num, max_name_len_all, &has_image,
                      fbb);  // uniform
    *((int *)program_data) -= has_image;
    int fbb_size = fbb.GetSize();
    buf_len += sizeof(int) + fbb_size;
    program_data = (GLchar *)realloc(program_data, buf_len);
    *((int *)(program_data + write_offset)) = fbb_size;
    write_offset += sizeof(int);
    memcpy(program_data + write_offset, fbb.GetBufferPointer(), fbb_size);
    write_offset += fbb_size;

    fbb.Reset();
    init_attribute_data(program, attrib_gl_num, max_name_len_all,
                        fbb);  // attribute
    fbb_size = fbb.GetSize();
    buf_len += sizeof(int) + fbb_size;
    program_data = (GLchar *)realloc(program_data, buf_len);
    *((int *)(program_data + write_offset)) = fbb_size;
    write_offset += sizeof(int);
    memcpy(program_data + write_offset, fbb.GetBufferPointer(), fbb_size);
    write_offset += fbb_size;

    fbb.Reset();
    init_uniform_block_data(program, uniform_blocks_num, max_name_len_all,
                            fbb);  // uniform block
    fbb_size = fbb.GetSize();
    buf_len += sizeof(int) + fbb_size;
    program_data = (GLchar *)realloc(program_data, buf_len);
    *((int *)(program_data + write_offset)) = fbb_size;
    write_offset += sizeof(int);
    memcpy(program_data + write_offset, fbb.GetBufferPointer(), fbb_size);
    write_offset += fbb_size;

    fbb.Reset();
    int num_program_outputs = 0;
    glGetProgramInterfaceiv(program, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES,
                            &num_program_outputs);
    init_program_output_data(program, num_program_outputs,
                             fbb);  // program output
    fbb_size = fbb.GetSize();
    buf_len += sizeof(int) + fbb_size;
    program_data = (GLchar *)realloc(program_data, buf_len);
    *((int *)(program_data + write_offset)) = fbb_size;
    write_offset += sizeof(int);
    memcpy(program_data + write_offset, fbb.GetBufferPointer(), fbb_size);
    write_offset += fbb_size;

    fbb.Reset();
    int num_transform_feedback_varyings = 0;
    glGetProgramInterfaceiv(program, GL_TRANSFORM_FEEDBACK_VARYING,
                            GL_ACTIVE_RESOURCES,
                            &num_transform_feedback_varyings);
    init_transform_feedback_varying_data(program,
                                         num_transform_feedback_varyings,
                                         fbb);  // transform feedback varying
    fbb_size = fbb.GetSize();
    buf_len += sizeof(int) + fbb_size;
    program_data = (GLchar *)realloc(program_data, buf_len);
    *((int *)(program_data + write_offset)) = fbb_size;
    write_offset += sizeof(int);
    memcpy(program_data + write_offset, fbb.GetBufferPointer(), fbb_size);
    write_offset += fbb_size;

    fbb.Reset();
    int num_shader_storage_blocks = 0;
    glGetProgramInterfaceiv(program, GL_SHADER_STORAGE_BLOCK,
                            GL_ACTIVE_RESOURCES, &num_shader_storage_blocks);
    init_shader_storage_block_data(program, num_shader_storage_blocks,
                                   fbb);  // shader storage block
    fbb_size = fbb.GetSize();
    buf_len += sizeof(int) + fbb_size;
    program_data = (GLchar *)realloc(program_data, buf_len);
    *((int *)(program_data + write_offset)) = fbb_size;
    write_offset += sizeof(int);
    memcpy(program_data + write_offset, fbb.GetBufferPointer(), fbb_size);
    write_offset += fbb_size;

    fbb.Reset();
    int num_buffer_variables = 0;
    glGetProgramInterfaceiv(program, GL_BUFFER_VARIABLE, GL_ACTIVE_RESOURCES,
                            &num_buffer_variables);
    init_buffer_variable_data(program, num_buffer_variables,
                              fbb);  // buffer variable
    fbb_size = fbb.GetSize();
    buf_len += sizeof(int) + fbb_size;
    program_data = (GLchar *)realloc(program_data, buf_len);
    *((int *)(program_data + write_offset)) = fbb_size;
    write_offset += sizeof(int);
    memcpy(program_data + write_offset, fbb.GetBufferPointer(), fbb_size);
    write_offset += fbb_size;

    fbb.Reset();
    int num_atomic_counter_buffers = 0;
    glGetProgramInterfaceiv(program, GL_ATOMIC_COUNTER_BUFFER,
                            GL_ACTIVE_RESOURCES, &num_atomic_counter_buffers);
    init_atomic_counter_buffer_data(program, num_atomic_counter_buffers,
                                    fbb);  // atomic counter buffer
    fbb_size = fbb.GetSize();
    buf_len += sizeof(int) + fbb_size;
    program_data = (GLchar *)realloc(program_data, buf_len);
    *((int *)(program_data + write_offset)) = fbb_size;
    write_offset += sizeof(int);
    memcpy(program_data + write_offset, fbb.GetBufferPointer(), fbb_size);
    write_offset += fbb_size;

#define INTERFACE_SIZE 9
    GLenum program_interfaces[INTERFACE_SIZE] = {GL_UNIFORM,
                                                 GL_UNIFORM_BLOCK,
                                                 GL_ATOMIC_COUNTER_BUFFER,
                                                 GL_PROGRAM_INPUT,
                                                 GL_PROGRAM_OUTPUT,
                                                 GL_TRANSFORM_FEEDBACK_VARYING,
                                                 GL_BUFFER_VARIABLE,
                                                 GL_SHADER_STORAGE_BLOCK,
                                                 GL_TRANSFORM_FEEDBACK_BUFFER};
#define INTERFACE_PNAME_SIZE 3
    GLenum program_interface_pnames[INTERFACE_PNAME_SIZE] = {
        GL_ACTIVE_RESOURCES, GL_MAX_NAME_LENGTH, GL_MAX_NUM_ACTIVE_VARIABLES};
    GLint params[INTERFACE_SIZE * INTERFACE_PNAME_SIZE] = {0};
    for (int i = 0; i < INTERFACE_SIZE; i++) {
        for (int j = 0; j < INTERFACE_PNAME_SIZE; j++) {
            if (program_interface_pnames[j] == GL_MAX_NAME_LENGTH &&
                program_interfaces[i] == GL_ATOMIC_COUNTER_BUFFER)
                continue;
            if (program_interface_pnames[j] == GL_MAX_NUM_ACTIVE_VARIABLES &&
                (program_interfaces[i] != GL_UNIFORM_BLOCK &&
                 program_interfaces[i] != GL_ATOMIC_COUNTER_BUFFER &&
                 program_interfaces[i] != GL_SHADER_STORAGE_BLOCK))
                continue;
            glGetProgramInterfaceiv(program, program_interfaces[i],
                                    program_interface_pnames[j],
                                    &params[i * INTERFACE_PNAME_SIZE + j]);
            ALOGD("Program interface %d %d %d\n", interface, pname, param);
        }
    }

    buf_len += INTERFACE_SIZE * INTERFACE_PNAME_SIZE * sizeof(int);
    program_data = (GLchar *)realloc(program_data, buf_len);
    memcpy(program_data + write_offset, params,
           INTERFACE_SIZE * INTERFACE_PNAME_SIZE * sizeof(int));

    program_data_map[program] = program_data;

#ifdef ENABLE_OPENGL_DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        ALOGE("Error when create program\n");
    }
#endif
    return buf_len;
}

void GLProgram::d_glUseProgram_special(void *context, GLuint program) {
    GLContext *opengl_context = (GLContext *)context;

    if (program_is_external_map.find(program) !=
            program_is_external_map.end() &&
        (int)(uint64_t)program_is_external_map[program] == 1) {
        
        opengl_context->is_using_external_program = 1;
    } else {
        opengl_context->is_using_external_program = 0;
    }

    glUseProgram(program);
}

void GLProgram::d_glProgramBinary_special(void *context, GLuint program,
                                          GLenum binaryFormat,
                                          const void *binary, GLsizei length) {
    glProgramBinary(program, binaryFormat, binary, length);
    return;
}

void GLProgram::d_glGetProgramDataSize(void *context, GLuint program,
                                       GLsizei *data_len) {
    *data_len = init_program_data(program);
}

void GLProgram::d_glGetProgramData(void *context, GLuint program, int buf_len,
                                   void *program_data) {
    GLchar *save_program_data = program_data_map[program];

    if (save_program_data == NULL) {
        ALOGE("error! save_program_data NULL program %u", program);
        return;
    }
    memcpy(program_data, save_program_data, buf_len);

    program_data_map.erase(program);
    return;
}

void GLProgram::d_glShaderSource_special(void *context, GLuint shader,
                                         GLsizei count, GLint *length,
                                         GLchar **string) {
    glShaderSource(shader, count, (const GLchar *const *)string, length);
}

static int GLSL_VERSION_SIZE = 2;
static const char *GLSL_VERSION[] = {"300 es", "310 es"};

void change_GLSL_version(char *start, char *end, int try_cnt) {
    while (start + 3 < end && (*start < '0' || *start > '9')) {
        start++;
    }

    if (*start < '0' || *start > '9' || try_cnt >= GLSL_VERSION_SIZE) {
        return;
    }

    start[0] = GLSL_VERSION[try_cnt][0];
    start[1] = GLSL_VERSION[try_cnt][1];
    start[2] = GLSL_VERSION[try_cnt][2];
    start += 3;
    if (end - start >= 3) {
        start[0] = GLSL_VERSION[try_cnt][3];
        start[1] = GLSL_VERSION[try_cnt][4];
        start[2] = GLSL_VERSION[try_cnt][5];
        start += 3;
    }
    while (start != end) {
        *start = ' ';
        start++;
    }
    return;
}

void GLProgram::d_glCompileShader_special(void *context, GLuint guest_id) {
    GLuint host_id =
        (GLuint)GLResource::get_host_shader_id(context, (unsigned int)guest_id);

    glCompileShader(host_id);
    GLenum error = glGetError();

    if (error != GL_NO_ERROR) {
        ALOGE("glCompileShader %x guest %u host %u\n", error, guest_id,
              host_id);
    }

    GLint compiled;
    glGetShaderiv(host_id, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint source_len = 0;
        glGetShaderiv(host_id, GL_SHADER_SOURCE_LENGTH, &source_len);

        char *source = (char *)malloc(source_len + 1);
        source[source_len] = 0;

        glGetShaderSource(host_id, source_len, NULL, source);

        GLint infoLen = 0;
        glGetShaderiv(host_id, GL_INFO_LOG_LENGTH, &infoLen);

        char *info_log = (char *)malloc(infoLen);
        glGetShaderInfoLog(host_id, infoLen, NULL, info_log);

        char *string_loc = strstr(source, "#version");
        char *enter_loc = strstr(source, "\n");
        if (string_loc != NULL && string_loc + 8 < enter_loc &&
            enter_loc - string_loc - 8 < 100 &&
            string_loc - source <= source_len) {
            
            int try_cnt = 0;
            while (try_cnt < GLSL_VERSION_SIZE) {
                change_GLSL_version(string_loc + 8, enter_loc, try_cnt);

                glShaderSource(host_id, 1, (const char *const *)&source,
                               &source_len);
                glCompileShader(host_id);
                glGetShaderiv(host_id, GL_COMPILE_STATUS, &compiled);
                // ALOGD("try change(%d) source compiled %d:%s\n", try_cnt,
                // compiled, source);
                if (compiled) {
                    break;
                }
                try_cnt++;
            }
        }
        if (!compiled) {
            ALOGE("shader compile error! shader source:\n%s\nerror info:\n%s\n",
                  source, info_log);
        }

        free(source);
        free(info_log);
    }
}

void GLProgram::g_program_data_destroy(GLchar *data) { free(data); }
