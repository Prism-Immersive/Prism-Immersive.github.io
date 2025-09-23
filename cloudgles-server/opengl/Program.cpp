
#define LOG_TAG "GLSLProgram"
#include "Program.h"

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#include <dlfcn.h>
#include <unistd.h>
#include <vndksupport/linker.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>

#include "GLDef.h"
#include "GLUtils.h"
#include "Log.h"
#include "ProgramData/Attribute_generated.h"
#include "ProgramData/BufferVariable_generated.h"
#include "ProgramData/UniformBlock_generated.h"
#include "ProgramData/Uniform_generated.h"
#include "ThreadContext.h"
#include "opengl/State.h"

GLSLProgram::GLSLProgram(GLuint id) : program_id(id) {}

void GLSLProgram::create(void *context, GLuint program_id) {
    auto *opengl_context = (GLThreadContext *)context;
    std::unordered_map<GLuint, GLSLProgram *> *program_info_map =
        opengl_context->program_info_map;

    GLSLProgram *program = (*program_info_map)[program_id];
    if (program == nullptr) {
        program = new GLSLProgram(program_id);
        (*program_info_map)[program_id] = program;
    }

#ifdef USE_LOCAL_COMPILER
    program->mock_program_id = opengl_context->glsl_compiler->create_program();
    if (program->mock_program_id == 0) {
        ALOGE("%s: Create mock program failed, error is %x", __FUNCTION__,
              opengl_context->glsl_compiler->glv2.glGetError());
    }
#endif
}

void GLSLProgram::create_shader_program(void *context, GLuint program_id,
                                        GLenum type, GLsizei count,
                                        const GLchar *const *strings) {
    auto *opengl_context = (GLThreadContext *)context;
    std::unordered_map<GLuint, GLSLProgram *> *program_info_map =
        opengl_context->program_info_map;

    GLSLProgram *program = (*program_info_map)[program_id];
    if (program == nullptr) {
        program = new GLSLProgram(program_id);
        (*program_info_map)[program_id] = program;
    }

#ifdef USE_LOCAL_COMPILER
    program->mock_program_id =
        opengl_context->glsl_compiler->create_shader_programv(type, count,
                                                              strings);
    if (program->mock_program_id == 0) {
        ALOGE("%s: Create mock program failed, error is %x", __FUNCTION__,
              opengl_context->glsl_compiler->glv2.glGetError());
    }
#endif
}

void GLSLProgram::destroy(void *context, GLuint program_id) {
    auto *opengl_context = (GLThreadContext *)context;
    if (opengl_context->program_info_map->find(program_id) !=
        opengl_context->program_info_map->end()) {
        GLSLProgram *delete_program =
            (*opengl_context->program_info_map)[program_id];
        opengl_context->program_info_map->erase(program_id);

#ifdef USE_LOCAL_COMPILER
        if (delete_program->mock_program_id != 0)
            opengl_context->glsl_compiler->delete_program(
                delete_program->mock_program_id);
#endif
        delete delete_program;
    }
}

void GLSLProgram::attach_shader(void *context, GLuint shader) {
    attach_shaders[shader] = shader;
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (res_ctx->shader_is_external()->has_id(shader)) {
        is_external = 1;
        ALOGD("set program %d is external", program);
    }
}

void GLSLProgram::get_attached_shaders(void *context, GLsizei maxCount,
                                       GLsizei *count, GLuint *shaders) {
    int temp_count = 0;
    for (const auto &[key, value] : attach_shaders) {
        shaders[temp_count] = key;
        temp_count += 1;
        if (temp_count >= maxCount) {
            break;
        }
    }
    *count = temp_count;
}

bool GLSLProgram::detach_shader(void *context, GLuint shader) {
    if (attach_shaders.find(shader) == attach_shaders.end()) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return false;
    }

    attach_shaders.erase(shader);
    return true;
}

void GLSLProgram::set_external(int external) { is_external = external; }

void GLSLProgram::set_program_binary(const void *binary, GLsizei length) {
    program_binary.resize(length);
    memcpy(program_binary.data(), binary, length);
}

bool GLSLProgram::get_program_binary(void *context, void *binary,
                                     GLsizei bufSize, GLsizei *length,
                                     GLenum *binaryFormat) {
    if (program_binary.empty() || binary_format == 0) {
        return false;
    }

    if (bufSize < static_cast<GLsizei>(program_binary.size())) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return false;
    }

    if (binary != nullptr) {
        memcpy(binary, program_binary.data(), program_binary.size());
    }

    if (length != nullptr) {
        *length = static_cast<GLsizei>(program_binary.size());
    }

    if (binaryFormat != nullptr) {
        *binaryFormat = binary_format;
    }

    return true;
}

void GLSLProgram::uniform_block_binding(void *context, GLuint uniformBlockIndex,
                                        GLuint uniformBlockBinding) {
    if (!is_init) init(context);
    if (num_uniform_blocks <= uniformBlockIndex) {
        ALOGE("glUniformBlockBinding error index %d binding %d",
              uniformBlockIndex, uniformBlockBinding);
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (uniform_blocks.blocks[uniformBlockIndex]->buffer_binding == -1 ||
        uniform_blocks.blocks[uniformBlockIndex]->buffer_binding !=
            uniformBlockBinding) {
        uniform_blocks.blocks[uniformBlockIndex]->buffer_binding =
            uniformBlockBinding;
        r_glUniformBlockBinding(context, program_id, uniformBlockIndex,
                                uniformBlockBinding);
    }
}

void GLSLProgram::get_programiv(void *context, GLenum pname, GLint *params) {
    if (!is_init && pname != GL_LINK_STATUS) init(context);
    switch (pname) {
        case GL_ACTIVE_UNIFORMS:
            *params = num_uniforms;
            break;
        case GL_ACTIVE_ATTRIBUTES:
            *params = num_attribs;
            break;
        case GL_ACTIVE_UNIFORM_BLOCKS:
            *params = num_uniform_blocks;
            break;
        case GL_TRANSFORM_FEEDBACK_VARYINGS:
            *params = transform_feedback_varyings;
            break;
        case GL_ATTACHED_SHADERS:
            *params = static_cast<int>(attach_shaders.size());
            break;
        case GL_LINK_STATUS:
            *params = 1;
            break;
        case GL_ACTIVE_ATTRIBUTE_MAX_LENGTH:
            *params = max_attrib_name_len;
            break;
        case GL_ACTIVE_UNIFORM_MAX_LENGTH:
            *params = max_uniform_name_len;
            break;
        case GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH:
            *params = max_uniform_block_name_len;
            break;
        case GL_COMPUTE_WORK_GROUP_SIZE:
            params[0] = compute_work_group_size[0];
            params[1] = compute_work_group_size[1];
            params[2] = compute_work_group_size[2];
            break;
        case GL_PROGRAM_BINARY_LENGTH:
            *params = binary_length;
            break;
        default:
            r_glGetProgramiv(context, program_id, pname, params);
            ALOGE("get programiv from host pname %x data %d", pname, *params);
            break;
    }
}

void GLSLProgram::get_program_interfaceiv(void *context,
                                          GLenum programInterface, GLenum pname,
                                          GLint *params) {
    if (!is_init) init(context);
    int64_t interface_index =
        std::find(program_interfaces, program_interfaces + INTERFACE_SIZE,
                  programInterface) -
        program_interfaces;
    interface_index = interface_index < INTERFACE_SIZE ? interface_index : -1;

    int64_t pname_index =
        std::find(program_interface_pnames,
                  program_interface_pnames + INTERFACE_PNAME_SIZE, pname) -
        program_interface_pnames;
    pname_index = pname_index < INTERFACE_PNAME_SIZE ? pname_index : -1;

    if (interface_index == -1 || pname_index == -1) {
        r_glGetProgramInterfaceiv(context, program_id, programInterface, pname,
                                  params);
        return;
    }

    *params = program_interface_data[interface_index * INTERFACE_PNAME_SIZE +
                                     pname_index];
}

GLuint GLSLProgram::get_program_resource_index(void *context,
                                               GLenum programInterface,
                                               const GLchar *name) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM: {
            auto uniform =
                std::find_if(uniforms.uniforms.begin(), uniforms.uniforms.end(),
                             [name](const std::unique_ptr<UniformT> &up) {
                                 return up->name == name;
                             });
            if (uniform == uniforms.uniforms.end()) {
                ALOGE("Cannot get uniform %s index", name);
                return GL_INVALID_INDEX;
            }
            return (*uniform)->index;
        }
        case GL_UNIFORM_BLOCK: {
            auto uniform_block = std::find_if(
                uniform_blocks.blocks.begin(), uniform_blocks.blocks.end(),
                [name](const std::unique_ptr<UniformBlockT> &ubp) {
                    return ubp->name == name;
                });
            if (uniform_block == uniform_blocks.blocks.end()) {
                ALOGE("Cannot get uniform %s index", name);
                return GL_INVALID_INDEX;
            }
            return (*uniform_block)->index;
        }
        case GL_PROGRAM_INPUT: {
            auto attrib =
                std::find_if(attributes.attrs.begin(), attributes.attrs.end(),
                             [name](const std::unique_ptr<AttributeT> &ap) {
                                 return ap->name == name;
                             });
            if (attrib == attributes.attrs.end()) {
                ALOGE("Cannot get attribute %s index", name);
                return GL_INVALID_INDEX;
            }
            return (*attrib)->index;
        }
        case GL_PROGRAM_OUTPUT: {
            auto output = std::find_if(
                program_outputs.outputs.begin(), program_outputs.outputs.end(),
                [name](const std::unique_ptr<ProgramOutputT> &op) {
                    return op->name == name;
                });
            if (output == program_outputs.outputs.end()) {
                ALOGE("Cannot get output %s index", name);
                return GL_INVALID_INDEX;
            }
            return (*output)->index;
        }
        case GL_TRANSFORM_FEEDBACK_VARYING: {
            auto varying = std::find_if(
                varyings.varyings.begin(), varyings.varyings.end(),
                [name](const std::unique_ptr<TransformFeedbackVaryingT> &vp) {
                    return vp->name == name;
                });
            if (varying == varyings.varyings.end()) {
                ALOGE("Cannot get varying %s index", name);
                return GL_INVALID_INDEX;
            }
            return (*varying)->index;
        }
        case GL_BUFFER_VARIABLE: {
            auto buffer_variable = std::find_if(
                buffer_variables.variables.begin(),
                buffer_variables.variables.end(),
                [name](const std::unique_ptr<BufferVariableT> &bp) {
                    return bp->name == name;
                });
            if (buffer_variable == buffer_variables.variables.end()) {
                ALOGE("Cannot get buffer variable %s index", name);
                return GL_INVALID_INDEX;
            }
            return (*buffer_variable)->index;
        }
        case GL_SHADER_STORAGE_BLOCK: {
            auto storage_block = std::find_if(
                storage_blocks.blocks.begin(), storage_blocks.blocks.end(),
                [name](const std::unique_ptr<ShaderStorageBlockT> &sbp) {
                    return sbp->name == name;
                });
            if (storage_block == storage_blocks.blocks.end()) {
                ALOGE("Cannot get storage block %s index", name);
                return GL_INVALID_INDEX;
            }
            return (*storage_block)->index;
        }
        default:
            ALOGE("Cannot get index for program interface %d",
                  programInterface);
            return -1;
    }
}

GLint GLSLProgram::get_program_resource_location(void *context,
                                                 GLenum programInterface,
                                                 const GLchar *name) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM: {
            auto uniform =
                std::find_if(uniforms.uniforms.begin(), uniforms.uniforms.end(),
                             [name](const std::unique_ptr<UniformT> &up) {
                                 return up->name == name;
                             });
            if (uniform == uniforms.uniforms.end()) {
                // Try with [0] suffix
                ALOGE("Cannot get uniform %s location", name);
                return -1;
            }
            return (*uniform)->location;
        }
        case GL_PROGRAM_INPUT: {
            auto attrib =
                std::find_if(attributes.attrs.begin(), attributes.attrs.end(),
                             [name](const std::unique_ptr<AttributeT> &ap) {
                                 return ap->name == name;
                             });
            if (attrib == attributes.attrs.end()) {
                ALOGE("Cannot get attribute %s location", name);
                return -1;
            }
            return (*attrib)->location;
        }
        case GL_PROGRAM_OUTPUT: {
            auto output = std::find_if(
                program_outputs.outputs.begin(), program_outputs.outputs.end(),
                [name](const std::unique_ptr<ProgramOutputT> &op) {
                    return op->name == name;
                });
            if (output == program_outputs.outputs.end()) {
                ALOGE("Cannot get output %s location", name);
                return -1;
            }
            return (*output)->location;
        }
        default:
            ALOGE("Cannot get location for program interface %d",
                  programInterface);
            return -1;
    }
}

void GLSLProgram::get_program_resource_name(void *context,
                                            GLenum programInterface,
                                            GLuint index, GLsizei bufSize,
                                            GLsizei *length, GLchar *name) {
    if (!is_init) init(context);
    int name_len = 0;
    switch (programInterface) {
        case GL_UNIFORM: {
            if (index >= num_uniforms) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            name_len =
                static_cast<int>(uniforms.uniforms[index]->name.size()) + 1;
            if (name_len > bufSize) {
                name_len = bufSize;
            }
            strncpy(name, uniforms.uniforms[index]->name.c_str(), name_len);
        } break;
        case GL_UNIFORM_BLOCK: {
            if (index >= num_uniform_blocks) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            name_len =
                static_cast<int>(uniform_blocks.blocks[index]->name.size()) + 1;
            if (name_len > bufSize) {
                name_len = bufSize;
            }
            strncpy(name, uniform_blocks.blocks[index]->name.c_str(), name_len);
        } break;
        case GL_PROGRAM_INPUT: {
            if (index >= num_attribs) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            name_len =
                static_cast<int>(attributes.attrs[index]->name.size()) + 1;
            if (name_len > bufSize) {
                name_len = bufSize;
            }
            strncpy(name, attributes.attrs[index]->name.c_str(), name_len);
        } break;
        case GL_PROGRAM_OUTPUT: {
            if (index >= program_outputs.outputs.size()) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            name_len =
                static_cast<int>(program_outputs.outputs[index]->name.size()) +
                1;
            if (name_len > bufSize) {
                name_len = bufSize;
            }
            strncpy(name, program_outputs.outputs[index]->name.c_str(),
                    name_len);
        } break;
        case GL_TRANSFORM_FEEDBACK_VARYING: {
            if (index >= varyings.varyings.size()) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            name_len =
                static_cast<int>(varyings.varyings[index]->name.size()) + 1;
            if (name_len > bufSize) {
                name_len = bufSize;
            }
            strncpy(name, varyings.varyings[index]->name.c_str(), name_len);
        } break;
        case GL_BUFFER_VARIABLE: {
            if (index >= buffer_variables.variables.size()) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            name_len = static_cast<int>(
                           buffer_variables.variables[index]->name.size()) +
                       1;
            strncpy(name, buffer_variables.variables[index]->name.c_str(),
                    name_len);
            if (length != nullptr) {
                *length = name_len - 1;
            }
        } break;
        case GL_SHADER_STORAGE_BLOCK: {
            if (index >= storage_blocks.blocks.size()) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            name_len =
                static_cast<int>(storage_blocks.blocks[index]->name.size()) + 1;
            if (name_len > bufSize) {
                name_len = bufSize;
            }
            strncpy(name, storage_blocks.blocks[index]->name.c_str(), name_len);
        } break;
        default:
            ALOGE("Cannot get name for program interface %d", programInterface);
            break;
    }

    if (length != nullptr && name_len != 0) {
        // Excluding the null terminator
        *length = name_len - 1;
    }
}

void GLSLProgram::get_program_resourceiv(void *context, GLenum programInterface,
                                         GLuint index, GLsizei propCount,
                                         const GLenum *props, GLsizei bufSize,
                                         GLsizei *length, GLint *params) {
    int write_count = 0;
    int remaining = bufSize;

    for (int i = 0; i < propCount; i++) {
        if (remaining <= 0) break;
        switch (props[i]) {
            case GL_NAME_LENGTH:
                get_name_length(context, programInterface, index,
                                params + write_count);
                write_count++;
                break;
            case GL_TYPE:
                get_type(context, programInterface, index,
                         params + write_count);
                write_count++;
                break;
            case GL_ARRAY_SIZE:
                get_array_size(context, programInterface, index,
                               params + write_count);
                write_count++;
                break;
            case GL_OFFSET:
                get_offset(context, programInterface, index,
                           params + write_count);
                write_count++;
                break;
            case GL_BLOCK_INDEX:
                get_block_index(context, programInterface, index,
                                params + write_count);
                write_count++;
                break;
            case GL_ARRAY_STRIDE:
                get_array_stride(context, programInterface, index,
                                 params + write_count);
                write_count++;
                break;
            case GL_MATRIX_STRIDE:
                get_matrix_stride(context, programInterface, index,
                                  params + write_count);
                write_count++;
                break;
            case GL_IS_ROW_MAJOR:
                get_is_row_major(context, programInterface, index,
                                 params + write_count);
                write_count++;
                break;
            case GL_ATOMIC_COUNTER_BUFFER_INDEX:
                get_atomic_counter_buffer_index(context, programInterface,
                                                index, params + write_count);
                write_count++;
                break;
            case GL_BUFFER_BINDING:
                get_buffer_binding(context, programInterface, index,
                                   params + write_count);
                write_count++;
                break;
            case GL_BUFFER_DATA_SIZE:
                get_buffer_data_size(context, programInterface, index,
                                     params + write_count);
                write_count++;
                break;
            case GL_NUM_ACTIVE_VARIABLES:
                get_num_active_variables(context, programInterface, index,
                                         params + write_count);
                write_count++;
                break;
            case GL_ACTIVE_VARIABLES:
                write_count +=
                    get_active_variables(context, programInterface, index,
                                         remaining, params + write_count);
                break;
            case GL_REFERENCED_BY_VERTEX_SHADER:
                get_ref_by_vertex_shader(context, programInterface, index,
                                         params + write_count);
                write_count++;
                break;
            case GL_REFERENCED_BY_FRAGMENT_SHADER:
                get_ref_by_frag_shader(context, programInterface, index,
                                       params + write_count);
                write_count++;
                break;
            case GL_REFERENCED_BY_COMPUTE_SHADER:
                get_ref_by_compute_shader(context, programInterface, index,
                                          params + write_count);
                write_count++;
                break;
            case GL_REFERENCED_BY_TESS_CONTROL_SHADER:
                get_ref_by_tess_control_shader(context, programInterface, index,
                                               params + write_count);
                write_count++;
                break;
            case GL_REFERENCED_BY_TESS_EVALUATION_SHADER:
                get_ref_by_tess_eval_shader(context, programInterface, index,
                                            params + write_count);
                write_count++;
                break;
            case GL_REFERENCED_BY_GEOMETRY_SHADER:
                get_ref_by_geometry_shader(context, programInterface, index,
                                           params + write_count);
                write_count++;
                break;
            case GL_TOP_LEVEL_ARRAY_SIZE:
                get_top_level_array_size(context, programInterface, index,
                                         params + write_count);
                write_count++;
                break;
            case GL_TOP_LEVEL_ARRAY_STRIDE:
                get_top_level_array_stride(context, programInterface, index,
                                           params + write_count);
                write_count++;
                break;
            case GL_LOCATION:
                get_location(context, programInterface, index,
                             params + write_count);
                write_count++;
                break;
            case GL_LOCATION_INDEX_EXT:
                get_location_index(context, programInterface, index,
                                   params + write_count);
                write_count++;
                break;
            default:
                ALOGE(
                    "Cannot get resourceiv for program interface %d with prop "
                    "%d",
                    programInterface, props[i]);
                break;
        }
        remaining -= write_count;
    }

    if (length != nullptr) {
        *length = write_count;
    }
}

void GLSLProgram::get_name_length(void *context, GLenum programInterface,
                                  GLuint index, GLsizei *length) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *length =
                static_cast<int>(uniforms.uniforms[index]->name.size()) + 1;
            break;
        case GL_UNIFORM_BLOCK:
            *length =
                static_cast<int>(uniform_blocks.blocks[index]->name.size()) + 1;
            break;
        case GL_PROGRAM_INPUT:
            *length =
                static_cast<int>(attributes.attrs[index]->name.size()) + 1;
            break;
        case GL_PROGRAM_OUTPUT:
            *length =
                static_cast<int>(program_outputs.outputs[index]->name.size()) +
                1;
            break;
        case GL_TRANSFORM_FEEDBACK_VARYING:
            *length =
                static_cast<int>(varyings.varyings[index]->name.size()) + 1;
            break;
        case GL_BUFFER_VARIABLE:
            *length = static_cast<int>(
                          buffer_variables.variables[index]->name.size()) +
                      1;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *length =
                static_cast<int>(storage_blocks.blocks[index]->name.size()) + 1;
            break;
        default:
            ALOGE("Cannot get name length for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_type(void *context, GLenum programInterface, GLuint index,
                           GLint *type) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *type = uniforms.uniforms[index]->type;
            break;
        case GL_PROGRAM_INPUT:
            *type = attributes.attrs[index]->type;
            break;
        case GL_PROGRAM_OUTPUT:
            *type = program_outputs.outputs[index]->type;
            break;
        case GL_TRANSFORM_FEEDBACK_VARYING:
            *type = varyings.varyings[index]->type;
            break;
        case GL_BUFFER_VARIABLE:
            *type = buffer_variables.variables[index]->type;
            break;
        default:
            ALOGE("Cannot get type for program interface %d", programInterface);
            break;
    }
}

void GLSLProgram::get_array_size(void *context, GLenum programInterface,
                                 GLuint index, GLint *arraySize) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *arraySize = uniforms.uniforms[index]->size;
            break;
        case GL_BUFFER_VARIABLE:
            *arraySize = buffer_variables.variables[index]->size;
            break;
        case GL_PROGRAM_INPUT:
            *arraySize = attributes.attrs[index]->size;
            break;
        case GL_PROGRAM_OUTPUT:
            *arraySize = program_outputs.outputs[index]->size;
            break;
        case GL_TRANSFORM_FEEDBACK_VARYING:
            *arraySize = varyings.varyings[index]->size;
            break;
        default:
            ALOGE("Cannot get array size for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_offset(void *context, GLenum programInterface,
                             GLuint index, GLint *offset) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *offset = uniforms.uniforms[index]->offset;
            break;
        case GL_BUFFER_VARIABLE:
            *offset = buffer_variables.variables[index]->offset;
            break;
        case GL_TRANSFORM_FEEDBACK_VARYING:
            *offset = varyings.varyings[index]->offset;
            break;
        default:
            ALOGE("Cannot get offset for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_block_index(void *context, GLenum programInterface,
                                  GLuint index, GLint *blockIndex) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *blockIndex = uniforms.uniforms[index]->block_index;
            break;
        case GL_BUFFER_VARIABLE:
            *blockIndex = buffer_variables.variables[index]->block_index;
            break;
        default:
            ALOGE("Cannot get block index for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_array_stride(void *context, GLenum programInterface,
                                   GLuint index, GLint *arrayStride) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *arrayStride = uniforms.uniforms[index]->array_stride;
            break;
        case GL_BUFFER_VARIABLE:
            *arrayStride = buffer_variables.variables[index]->array_stride;
            break;
        default:
            ALOGE("Cannot get array stride for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_matrix_stride(void *context, GLenum programInterface,
                                    GLuint index, GLint *matrixStride) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *matrixStride = uniforms.uniforms[index]->matrix_stride;
            break;
        case GL_BUFFER_VARIABLE:
            *matrixStride = buffer_variables.variables[index]->matrix_stride;
            break;
        default:
            ALOGE("Cannot get matrix stride for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_is_row_major(void *context, GLenum programInterface,
                                   GLuint index, GLint *isRowMajor) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *isRowMajor = uniforms.uniforms[index]->is_row_major;
            break;
        case GL_BUFFER_VARIABLE:
            *isRowMajor = buffer_variables.variables[index]->is_row_major;
            break;
        default:
            ALOGE("Cannot get is row major for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_atomic_counter_buffer_index(void *context,
                                                  GLenum programInterface,
                                                  GLuint index,
                                                  GLint *bufferIndex) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *bufferIndex =
                uniforms.uniforms[index]->atomic_counter_buffer_index;
            break;
        default:
            ALOGE(
                "Cannot get atomic counter buffer index for program interface "
                "%d",
                programInterface);
            break;
    }
}

void GLSLProgram::get_buffer_binding(void *context, GLenum programInterface,
                                     GLuint index, GLint *bufferBinding) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM_BLOCK:
            *bufferBinding = uniform_blocks.blocks[index]->buffer_binding;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *bufferBinding = counter_buffers.buffers[index]->buffer_binding;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *bufferBinding = storage_blocks.blocks[index]->buffer_binding;
            break;
        default:
            ALOGE("Cannot get buffer binding for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_buffer_data_size(void *context, GLenum programInterface,
                                       GLuint index, GLint *bufferDataSize) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM_BLOCK:
            *bufferDataSize = uniform_blocks.blocks[index]->buffer_data_size;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *bufferDataSize = counter_buffers.buffers[index]->buffer_data_size;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *bufferDataSize = storage_blocks.blocks[index]->buffer_data_size;
            break;
        default:
            ALOGE("Cannot get buffer data size for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_is_per_patch(void *context, GLenum programInterface,
                                   GLuint index, GLint *isPerPatch) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_PROGRAM_INPUT:
            *isPerPatch = attributes.attrs[index]->is_per_patch;
            break;
        case GL_PROGRAM_OUTPUT:
            *isPerPatch = program_outputs.outputs[index]->is_per_patch;
            break;
        default:
            ALOGE("Cannot get is per patch for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_num_active_variables(void *context,
                                           GLenum programInterface,
                                           GLuint index,
                                           GLint *numActiveVariables) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM_BLOCK:
            *numActiveVariables = uniforms.uniforms[index]->size;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *numActiveVariables =
                counter_buffers.buffers[index]->num_active_variables;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *numActiveVariables =
                storage_blocks.blocks[index]->num_active_variables;
            break;
        default:
            ALOGE("Cannot get num active variables for program interface %d",
                  programInterface);
            break;
    }
}

int GLSLProgram::get_active_variables(void *context, GLenum programInterface,
                                      GLuint index, GLint bufSize,
                                      GLint *activeVariables) {
    if (!is_init) init(context);
    size_t count = 0;
    switch (programInterface) {
        case GL_UNIFORM_BLOCK:
            count =
                bufSize > uniform_blocks.blocks[index]->active_variables.size()
                    ? uniform_blocks.blocks[index]->active_variables.size()
                    : bufSize;
            memcpy(activeVariables,
                   uniform_blocks.blocks[index]->active_variables.data(),
                   count * sizeof(GLint));
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            count =
                bufSize >
                        counter_buffers.buffers[index]->active_variables.size()
                    ? counter_buffers.buffers[index]->active_variables.size()
                    : bufSize;
            memcpy(activeVariables,
                   counter_buffers.buffers[index]->active_variables.data(),
                   count * sizeof(GLint));
            break;
        case GL_SHADER_STORAGE_BLOCK:
            count =
                bufSize > storage_blocks.blocks[index]->active_variables.size()
                    ? storage_blocks.blocks[index]->active_variables.size()
                    : bufSize;
            memcpy(activeVariables,
                   storage_blocks.blocks[index]->active_variables.data(),
                   count * sizeof(GLint));
            break;
        default:
            ALOGE("Cannot get active variables for program interface %d",
                  programInterface);
            break;
    }
    return static_cast<int>(count);
}

void GLSLProgram::get_ref_by_vertex_shader(void *context,
                                           GLenum programInterface,
                                           GLuint index,
                                           GLint *refByVertexShader) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *refByVertexShader = uniforms.uniforms[index]->ref_by_vertex_shader;
            break;
        case GL_UNIFORM_BLOCK:
            *refByVertexShader =
                uniform_blocks.blocks[index]->ref_by_vertex_shader;
            break;
        case GL_PROGRAM_INPUT:
            *refByVertexShader = attributes.attrs[index]->ref_by_vertex_shader;
            break;
        case GL_PROGRAM_OUTPUT:
            *refByVertexShader =
                program_outputs.outputs[index]->ref_by_vertex_shader;
            break;
        case GL_BUFFER_VARIABLE:
            *refByVertexShader =
                buffer_variables.variables[index]->ref_by_vertex_shader;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *refByVertexShader =
                storage_blocks.blocks[index]->ref_by_vertex_shader;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *refByVertexShader =
                counter_buffers.buffers[index]->ref_by_vertex_shader;
            break;
        default:
            ALOGE("Cannot get ref by vertex shader for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_ref_by_frag_shader(void *context, GLenum programInterface,
                                         GLuint index, GLint *refByFragShader) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *refByFragShader = uniforms.uniforms[index]->ref_by_frag_shader;
            break;
        case GL_UNIFORM_BLOCK:
            *refByFragShader = uniform_blocks.blocks[index]->ref_by_frag_shader;
            break;
        case GL_PROGRAM_INPUT:
            *refByFragShader = attributes.attrs[index]->ref_by_frag_shader;
            break;
        case GL_PROGRAM_OUTPUT:
            *refByFragShader =
                program_outputs.outputs[index]->ref_by_frag_shader;
            break;
        case GL_BUFFER_VARIABLE:
            *refByFragShader =
                buffer_variables.variables[index]->ref_by_frag_shader;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *refByFragShader = storage_blocks.blocks[index]->ref_by_frag_shader;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *refByFragShader =
                counter_buffers.buffers[index]->ref_by_frag_shader;
            break;
        default:
            ALOGE("Cannot get ref by frag shader for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_ref_by_compute_shader(void *context,
                                            GLenum programInterface,
                                            GLuint index,
                                            GLint *refByComputeShader) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *refByComputeShader =
                uniforms.uniforms[index]->ref_by_compute_shader;
            break;
        case GL_UNIFORM_BLOCK:
            *refByComputeShader =
                uniform_blocks.blocks[index]->ref_by_compute_shader;
            break;
        case GL_PROGRAM_INPUT:
            *refByComputeShader =
                attributes.attrs[index]->ref_by_compute_shader;
            break;
        case GL_PROGRAM_OUTPUT:
            *refByComputeShader =
                program_outputs.outputs[index]->ref_by_compute_shader;
            break;
        case GL_BUFFER_VARIABLE:
            *refByComputeShader =
                buffer_variables.variables[index]->ref_by_compute_shader;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *refByComputeShader =
                storage_blocks.blocks[index]->ref_by_compute_shader;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *refByComputeShader =
                counter_buffers.buffers[index]->ref_by_compute_shader;
            break;
        default:
            ALOGE("Cannot get ref by compute shader for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_ref_by_tess_control_shader(
    void *context, GLenum programInterface, GLuint index,
    GLint *refByTessControlShader) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *refByTessControlShader =
                uniforms.uniforms[index]->ref_by_tess_control_shader;
            break;
        case GL_UNIFORM_BLOCK:
            *refByTessControlShader =
                uniform_blocks.blocks[index]->ref_by_tess_control_shader;
            break;
        case GL_PROGRAM_INPUT:
            *refByTessControlShader =
                attributes.attrs[index]->ref_by_tess_control_shader;
            break;
        case GL_PROGRAM_OUTPUT:
            *refByTessControlShader =
                program_outputs.outputs[index]->ref_by_tess_control_shader;
            break;
        case GL_BUFFER_VARIABLE:
            *refByTessControlShader =
                buffer_variables.variables[index]->ref_by_tess_control_shader;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *refByTessControlShader =
                storage_blocks.blocks[index]->ref_by_tess_control_shader;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *refByTessControlShader =
                counter_buffers.buffers[index]->ref_by_tess_control_shader;
            break;
        default:
            ALOGE(
                "Cannot get ref by tess control shader for program interface "
                "%d",
                programInterface);
            break;
    }
}
void GLSLProgram::get_ref_by_tess_eval_shader(void *context,
                                              GLenum programInterface,
                                              GLuint index,
                                              GLint *refByTessEvalShader) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *refByTessEvalShader =
                uniforms.uniforms[index]->ref_by_tess_eval_shader;
            break;
        case GL_UNIFORM_BLOCK:
            *refByTessEvalShader =
                uniform_blocks.blocks[index]->ref_by_tess_eval_shader;
            break;
        case GL_PROGRAM_INPUT:
            *refByTessEvalShader =
                attributes.attrs[index]->ref_by_tess_eval_shader;
            break;
        case GL_PROGRAM_OUTPUT:
            *refByTessEvalShader =
                program_outputs.outputs[index]->ref_by_tess_eval_shader;
            break;
        case GL_BUFFER_VARIABLE:
            *refByTessEvalShader =
                buffer_variables.variables[index]->ref_by_tess_eval_shader;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *refByTessEvalShader =
                storage_blocks.blocks[index]->ref_by_tess_eval_shader;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *refByTessEvalShader =
                counter_buffers.buffers[index]->ref_by_tess_eval_shader;
            break;
        default:
            ALOGE("Cannot get ref by tess eval shader for program interface %d",
                  programInterface);
            break;
    }
}
void GLSLProgram::get_ref_by_geometry_shader(void *context,
                                             GLenum programInterface,
                                             GLuint index,
                                             GLint *refByGeometryShader) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *refByGeometryShader =
                uniforms.uniforms[index]->ref_by_geometry_shader;
            break;
        case GL_UNIFORM_BLOCK:
            *refByGeometryShader =
                uniform_blocks.blocks[index]->ref_by_geometry_shader;
            break;
        case GL_PROGRAM_INPUT:
            *refByGeometryShader =
                attributes.attrs[index]->ref_by_geometry_shader;
            break;
        case GL_PROGRAM_OUTPUT:
            *refByGeometryShader =
                program_outputs.outputs[index]->ref_by_geometry_shader;
            break;
        case GL_BUFFER_VARIABLE:
            *refByGeometryShader =
                buffer_variables.variables[index]->ref_by_geometry_shader;
            break;
        case GL_SHADER_STORAGE_BLOCK:
            *refByGeometryShader =
                storage_blocks.blocks[index]->ref_by_geometry_shader;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            *refByGeometryShader =
                counter_buffers.buffers[index]->ref_by_geometry_shader;
            break;
        default:
            ALOGE("Cannot get ref by geometry shader for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_top_level_array_size(void *context,
                                           GLenum programInterface,
                                           GLuint index,
                                           GLint *topLevelArraySize) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_BUFFER_VARIABLE:
            *topLevelArraySize =
                buffer_variables.variables[index]->top_level_array_size;
            break;
        default:
            ALOGE("Cannot get top level array size for program interface %d",
                  programInterface);
            break;
    }
}
void GLSLProgram::get_top_level_array_stride(void *context,
                                             GLenum programInterface,
                                             GLuint index,
                                             GLint *topLevelArrayStride) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_BUFFER_VARIABLE:
            *topLevelArrayStride =
                buffer_variables.variables[index]->top_level_array_stride;
            break;
        default:
            ALOGE("Cannot get top level array stride for program interface %d",
                  programInterface);
            break;
    }
}
void GLSLProgram::get_location(void *context, GLenum programInterface,
                               GLuint index, GLint *location) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_UNIFORM:
            *location = uniforms.uniforms[index]->location;
            break;
        case GL_PROGRAM_INPUT:
            *location = attributes.attrs[index]->location;
            break;
        case GL_PROGRAM_OUTPUT:
            *location = program_outputs.outputs[index]->location;
            break;
        default:
            ALOGE("Cannot get location for program interface %d",
                  programInterface);
            break;
    }
}

void GLSLProgram::get_location_index(void *context, GLenum programInterface,
                                     GLuint index, GLint *locationIndex) {
    if (!is_init) init(context);
    switch (programInterface) {
        case GL_PROGRAM_OUTPUT:
            *locationIndex = program_outputs.outputs[index]->location_index;
            break;
        default:
            ALOGE("Cannot get location index for program interface %d",
                  programInterface);
            break;
    }
}

bool GLSLProgram::init(void *context) {
    main_program_cache->get_program_data(program_binary.data(),
                                         program_binary.size(), &program_data);

    bool cache_miss = false;
    if (program_data.empty()) {
        cache_miss = true;
        int program_data_len = 0;
        r_glGetProgramDataSize(context, program_id, &program_data_len);
        if (program_data_len == 0) {
            return false;
        }
        program_data.resize(program_data_len);
        r_glGetProgramData(context, program_id, program_data_len,
                           program_data.data());
    }

    char *next = program_data.data();
    next = init_basic_info(next);
    next = init_uniform_info(next);
    next = init_attrib_info(next);
    next = init_uniform_block_info(next);
    next = init_program_output_info(next);
    next = init_transform_feedback_varying_info(next);
    next = init_shader_storage_block_info(next);
    next = init_buffer_variable_info(next);
    next = init_atomic_counter_buffer_info(next);
    next = init_program_interface_info(next);

    if (program_binary.empty()) {
        program_binary.resize(binary_length);
        r_glGetProgramBinary(context, program_id, binary_length, nullptr,
                             &binary_format, program_binary.data());
    }

    if (cache_miss)
        main_program_cache->put_program_data(
            program_binary.data(), program_binary.size(), program_data.data(),
            program_data.size());

    is_init = true;

    return true;
}

char *GLSLProgram::init_basic_info(char *data) {
    auto *program_basic_info = (int *)data;

    num_uniforms = program_basic_info[0];
    num_attribs = program_basic_info[1];
    num_uniform_blocks = program_basic_info[2];
    max_uniform_name_len = program_basic_info[3];
    max_attrib_name_len = program_basic_info[4];
    max_uniform_block_name_len = program_basic_info[5];
    transform_feedback_varyings = program_basic_info[6];
    compute_work_group_size[0] = program_basic_info[7];
    compute_work_group_size[1] = program_basic_info[8];
    compute_work_group_size[2] = program_basic_info[9];

#ifdef USE_LOCAL_COMPILER
    GLint mock_binary_length = 0;
    opengl_context->glsl_compiler->get_programiv(
        program, GL_PROGRAM_BINARY_LENGTH, &mock_binary_length);
    binary_length =
        int_ptr[14] > mock_binary_length ? int_ptr[14] : mock_binary_length;
#else
    binary_length = program_basic_info[10];
#endif

    return data + 11 * sizeof(int);
}

char *GLSLProgram::init_uniform_info(char *data) {
    int size = *(int *)data;
    data += sizeof(int);
    GetUniforms(data)->UnPackTo(&uniforms);
    uniform_detected_num = static_cast<int>(uniforms.uniforms.size());
    for (int i = 0; i < uniform_detected_num; i++) {
        if (uniforms.uniforms[i]->location > max_uniform_location) {
            max_uniform_location = uniforms.uniforms[i]->location;
        }
        uniform_location_data[uniforms.uniforms[i]->location] =
            std::make_unique<char[]>(gl_sizeof(uniforms.uniforms[i]->type));
        memset(uniform_location_data[uniforms.uniforms[i]->location].get(), 0,
               gl_sizeof(uniforms.uniforms[i]->type));
        uniform_location_index[uniforms.uniforms[i]->location] = i;
    }

    return data + size;
}

char *GLSLProgram::init_attrib_info(char *data) {
    int size = *(int *)data;
    data += sizeof(int);
    GetAttributes(data)->UnPackTo(&attributes);
    for (int i = 0; i < attributes.attrs.size(); i++) {
        if (attributes.attrs[i]->location > max_attrib_location) {
            max_attrib_location = attributes.attrs[i]->location;
        }
    }

    return data + size;
}

char *GLSLProgram::init_uniform_block_info(char *data) {
    int size = *(int *)data;
    data += sizeof(int);
    GetUniformBlocks(data)->UnPackTo(&uniform_blocks);

    return data + size;
}

char *GLSLProgram::init_program_output_info(char *data) {
    int size = *(int *)data;
    data += sizeof(int);
    GetProgramOutputs(data)->UnPackTo(&program_outputs);

    return data + size;
}

char *GLSLProgram::init_transform_feedback_varying_info(char *data) {
    int size = *(int *)data;
    data += sizeof(int);
    GetTransformFeedbackVaryings(data)->UnPackTo(&varyings);

    return data + size;
}

char *GLSLProgram::init_shader_storage_block_info(char *data) {
    int size = *(int *)data;
    data += sizeof(int);
    GetShaderStorageBlocks(data)->UnPackTo(&storage_blocks);

    return data + size;
}

char *GLSLProgram::init_buffer_variable_info(char *data) {
    int size = *(int *)data;
    data += sizeof(int);
    GetBufferVariables(data)->UnPackTo(&buffer_variables);

    return data + size;
}

char *GLSLProgram::init_atomic_counter_buffer_info(char *data) {
    int size = *(int *)data;
    data += sizeof(int);
    GetAtomicCounterBuffers(data)->UnPackTo(&counter_buffers);

    return data + size;
}

char *GLSLProgram::init_program_interface_info(char *data) {
    memcpy(program_interface_data, data,
           INTERFACE_SIZE * INTERFACE_PNAME_SIZE * sizeof(int));
    return data + INTERFACE_SIZE * INTERFACE_PNAME_SIZE * sizeof(int);
}

/**
 * @brief Used by glGetActiveUniformBlockiv to determine the space of params
 * If pname is GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, params will be an array
 * of int of size GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS
 *
 * @param context
 * @param program
 * @param uniformBlockIndex
 * @param pname
 * @return size_t
 */
size_t GLSLProgram::get_uniform_block_para_size(void *context, GLuint program,
                                                GLuint uniformBlockIndex,
                                                GLenum pname) {
    auto *opengl_context = (GLThreadContext *)context;
    std::unordered_map<GLuint, GLSLProgram *> *program_info_map =
        opengl_context->program_info_map;
    if (pname == GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES &&
        program_info_map->find(program) != program_info_map->end()) {
        GLSLProgram *target_program = (*program_info_map)[program];
        if (!target_program->is_initialized()) target_program->init(context);
        if (target_program == nullptr ||
            uniformBlockIndex >= target_program->num_uniform_blocks) {
            return 1;
        } else {
            return target_program->uniform_blocks.blocks[uniformBlockIndex]
                ->size;
        }
    }
    return gl_pname_size(pname);
}

void GLSLProgram::replace_standard_derivatives_shader_source(char *string,
                                                             int len) {
    static const char STR_HASH_EXTENSION[] = "#extension";
    static const char STR_VERSION[] = "#version";

    static const char STR_GL_OES_STANDARD_DERIVATIVES_EXTERNAL[] =
        "GL_OES_standard_derivatives";

    if (string == nullptr) {
        return;
    }
    if (len == 0) {
        len = strlen(string);
    }

    char *c = string;
    c = strstr(c, STR_HASH_EXTENSION);
    while (c != nullptr && c - string < len) {
        char *start = c;
        c += sizeof(STR_HASH_EXTENSION) - 1;
        while (isspace(*c) && *c != '\0' && c - string + 28 < len) {
            c++;
        }

        bool has_standard_derivatives_external =
            !strncmp(c, STR_GL_OES_STANDARD_DERIVATIVES_EXTERNAL,
                     sizeof(STR_GL_OES_STANDARD_DERIVATIVES_EXTERNAL) - 1);

        if (has_standard_derivatives_external) {
            // #extension statements are terminated by end of line
            c = start;
            while (*c != '\0' && *c != '\r' && *c != '\n' && c - string < len) {
                *c++ = ' ';
            }
        }
    }

    c = string;

    return;
}

int GLSLProgram::replace_external_shader_source(char *string, int len) {
    static const char STR_HASH_EXTENSION[] = "#extension";
    static const char STR_SPECIAL_UNIFORM[] =
        "uniform int has_EGL_image_external;";

    static const char STR_GL_OES_EGL_IMAGE_EXTERNAL[] =
        "GL_OES_EGL_image_external";
    static const char STR_GL_OES_EGL_IMAGE_EXTERNAL_ESSL3[] =
        "GL_OES_EGL_image_external_essl3";
    static const char STR_SAMPLER_EXTERNAL_OES[] = "samplerExternalOES";
    static const char STR_SAMPLER2D_SPACE[] = "         sampler2D";

    char *c = string;
    int has_uniform = 0;
    int has_external = 0;
    while ((c = strstr(c, STR_HASH_EXTENSION))) {
        char *start = c;
        c += sizeof(STR_HASH_EXTENSION) - 1;
        while (isspace(*c) && *c != '\0') {
            c++;
        }

        bool has_base_image_external =
            !strncmp(c, STR_GL_OES_EGL_IMAGE_EXTERNAL,
                     sizeof(STR_GL_OES_EGL_IMAGE_EXTERNAL) - 1);
        bool has_essl3_image_external =
            !strncmp(c, STR_GL_OES_EGL_IMAGE_EXTERNAL_ESSL3,
                     sizeof(STR_GL_OES_EGL_IMAGE_EXTERNAL_ESSL3) - 1);

        if (has_base_image_external || has_essl3_image_external) {
            ALOGD("shader source has external:\n%s\n", string);
            has_external = 1;
            // #extension statements are terminated by end of line
            c = start;
            if (has_uniform == 0) {
                has_uniform = 1;
                memcpy(c, STR_SPECIAL_UNIFORM, sizeof(STR_SPECIAL_UNIFORM) - 1);
                c += sizeof(STR_SPECIAL_UNIFORM) - 1;
            }
            while (*c != '\0' && *c != '\r' && *c != '\n') {
                *c++ = ' ';
            }
        }
    }

    c = string;

    while ((c = strstr(c, STR_SAMPLER_EXTERNAL_OES))) {
        memcpy(c, STR_SAMPLER2D_SPACE, sizeof(STR_SAMPLER2D_SPACE) - 1);
    }
    return has_external;
}

bool GLSLProgram::uniform_is_change(void *context, GLint program,
                                    GLint location, char *check_data,
                                    int size) {
    auto *opengl_context = (GLThreadContext *)context;
    std::unordered_map<GLuint, GLSLProgram *> *program_info_map =
        opengl_context->program_info_map;

    if (program_info_map->find(program) != program_info_map->end()) {
        GLSLProgram *info = (*program_info_map)[program];
        if (!info->is_initialized()) info->init(context);
        if (location < 0 || location > info->max_uniform_location) {
            ALOGE("GLSLProgram::uniform_is_change get error location %d max %d",
                  location, info->max_uniform_location);
            return true;
        }

        if (info->uniform_location_data[location] != nullptr) {
            bool has_change = false;
            for (int i = 0; i < size; i++) {
                if (info->uniform_location_data[location][i] != check_data[i]) {
                    has_change = true;
                    info->uniform_location_data[location][i] = check_data[i];
                }
            }

            if (!has_change) {
                return false;
            }
        }
    }
    return true;
}

bool GLSLProgram::can_get_uniform(void *context, GLint program, GLint location,
                                  char *get_data) {
    auto *opengl_context = (GLThreadContext *)context;
    std::unordered_map<GLuint, GLSLProgram *> *program_info_map =
        opengl_context->program_info_map;

    if (program_info_map->find(program) != program_info_map->end()) {
        GLSLProgram *info = (*program_info_map)[program];
        if (!info->is_initialized()) info->init(context);
        if (location < 0 || location > info->max_uniform_location) {
            return false;
        }
        if (info->uniform_location_data[location] != nullptr &&
            info->uniform_location_index[location] >= 0 &&
            info->uniform_location_index[location] < info->num_uniforms) {
            int data_size = gl_sizeof(
                info->uniforms.uniforms[info->uniform_location_index[location]]
                    ->type);
            memcpy(get_data, info->uniform_location_data[location].get(),
                   data_size);
            return true;
        }
    }
    return false;
}

/**
 * @brief
 * Obtain the size of a specific uniform variable at the location in the program
 * Return 0 if there is no such location or the program has not been linked
 *
 * @param context
 * @param program
 * @param location
 * @return size_t
 */
size_t GLSLProgram::get_program_uniform_size(void *context, GLuint program,
                                             GLint location) {
    auto *opengl_context = (GLThreadContext *)context;
    std::unordered_map<GLuint, GLSLProgram *> *program_info_map =
        opengl_context->program_info_map;

    if (program_info_map->find(program) != program_info_map->end()) {
        GLSLProgram *info = (*program_info_map)[program];
        if (!info->is_initialized()) info->init(context);
        if (location < 0 || location > info->max_uniform_location) {
            return 4;
        }
        int index = info->uniform_location_index[location];
        if (index < 0 || index >= info->uniform_detected_num) {
            return 4;
        }
        GLenum type = info->uniforms.uniforms[index]->type;
        return gl_sizeof(type);
    }
    // Default return 4, because this pointer is of type int or float
    return 4;
}

void GLSLShader::create(void *context, GLuint shader_id, GLenum type) {
    auto *opengl_context = (GLThreadContext *)context;

    if (opengl_context->shader_map->find(shader_id) ==
        opengl_context->shader_map->end()) {
        auto shader = std::make_unique<GLSLShader>();
        shader->shader_type = type;
        (*opengl_context->shader_map)[shader_id] = std::move(shader);
    }

#ifdef USE_LOCAL_COMPILER
    int mock_shader_id = opengl_context->glsl_compiler->create_shader(type);
    (*opengl_context->shader_map)[shader_id]->mock_shader_id = mock_shader_id;
    if (mock_shader_id == 0) {
        ALOGE("Create mock shader failed, error is %x",
              opengl_context->glsl_compiler->glv2.glGetError());
    }
#endif
}

void GLSLShader::destroy(void *context, GLuint shader_id) {
    auto *opengl_context = (GLThreadContext *)context;
    if (opengl_context->shader_map->find(shader_id) !=
        opengl_context->shader_map->end()) {
#ifdef USE_LOCAL_COMPILER
        GLuint mock_shader_id =
            (*opengl_context->shader_map)[shader_id]->mock_shader_id;
        if (mock_shader_id != 0) {
            opengl_context->glsl_compiler->delete_shader(mock_shader_id);
        }
#endif
        opengl_context->shader_map->erase(shader_id);
    }
}

void d_glUseProgram_special(void *context, GLuint program) {
    auto *opengl_context = (GLThreadContext *)context;

    BindingStatus *bind_status = &opengl_context->context_binding_status;
    if (bind_status->current_program == program) return;
    bind_status->current_program = program;

#ifdef USE_LOCAL_COMPILER
    opengl_context->glsl_compiler->use_program(program);
#endif

    r_glUseProgram(context, program);
}

void d_glGetAttachedShaders_special(void *context, GLuint program,
                                    GLsizei maxCount, GLsizei *count,
                                    GLuint *shaders) {
    GLSLProgram *now_program =
        ((GLThreadContext *)context)->program_info_map[0][program];
    if (count == nullptr || shaders == nullptr) {
        return;
    }

    if (now_program == nullptr) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    now_program->get_attached_shaders(context, maxCount, count, shaders);
}

void d_glDetachShader_special(void *context, GLuint program, GLuint shader) {
    auto *opengl_context = (GLThreadContext *)context;

    GLSLProgram *program_info = (*opengl_context->program_info_map)[program];
    if (program_info == nullptr) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    if (!program_info->detach_shader(context, shader)) {
        return;
    }

#ifdef USE_LOCAL_COMPILER
    opengl_context->glsl_compiler->detach_shader(program, shader);
#endif

    r_glDetachShader(context, program, shader);
}

void d_glAttachShader_special(void *context, GLuint program, GLuint shader) {
    auto *opengl_context = (GLThreadContext *)context;
    GLSLProgram *program_info = (*opengl_context->program_info_map)[program];
    if (program_info == nullptr) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    program_info->attach_shader(context, shader);

#ifdef USE_LOCAL_COMPILER
    opengl_context->glsl_compiler->attach_shader(program, shader);
#endif

    r_glAttachShader(context, program, shader);
}

void d_glShaderSource_special(void *context, GLuint shader, GLsizei count,
                              const GLchar *const *string,
                              const GLint *length) {
    int is_external = 0;
    int total_length = 0;

#ifdef USE_LOCAL_COMPILER
    GLSLCompiler *compiler = ((GLThreadContext *)context)->glsl_compiler.get();
#endif

    if (length == nullptr && count > 0) {
        GLint *temp_length = new GLint[count];
        for (int i = 0; i < count; i++) {
            temp_length[i] = strlen(string[i]);
            is_external |= GLSLProgram::replace_external_shader_source(
                (char *)string[i], temp_length[i]);
            GLSLProgram::replace_standard_derivatives_shader_source(
                (char *)string[i], temp_length[i]);
            total_length += temp_length[i];
        }

#ifdef USE_LOCAL_COMPILER
        compiler->shader_source(shader, count, string, length);
#endif
        // Note that the order of len and string is reversed, so that we can
        // easily determine the length of string
        r_glShaderSource_origin(context, shader, count,
                                (const GLint *)temp_length, string);
        delete[] temp_length;
    } else {
        is_external |=
            GLSLProgram::replace_external_shader_source((char *)string[0], 0);
        GLSLProgram::replace_standard_derivatives_shader_source(
            (char *)string[0], 0);
        total_length = length[0];

#ifdef USE_LOCAL_COMPILER
        compiler->shader_source(shader, count, string, length);
#endif
        r_glShaderSource_origin(context, shader, count, length, string);
    }

    (*((GLThreadContext *)context)->shader_map)[shader]->shader_source_length =
        total_length;

    if (is_external == 1) {
        GLThreadContext *opengl_context = (GLThreadContext *)context;
        ResourceContext *res_ctx = &(opengl_context->resource_context);
        res_ctx->shader_is_external()->set_id(shader);
    }
}

/**
 * @brief
 * Save some information of shader program when linking shader program
 *
 * @param context
 * @param program Program ID
 */
void d_glLinkProgram_special(void *context, GLuint program) {
#ifdef USE_LOCAL_COMPILER
    GLSLCompiler *compiler = ((GLThreadContext *)context)->glsl_compiler.get();
    compiler->link_program(program);
#endif

    r_glLinkProgram_special(context, program);
    GLSLProgram *program_info =
        (*((GLThreadContext *)context)->program_info_map)[program];
    if (!program_info->is_initialized()) program_info->init(context);
    ALOGD("%llx link program %u", (long long)context, program);
}

void d_glProgramBinary_special(void *context, GLuint program,
                               GLenum binaryFormat, const void *binary,
                               GLsizei length) {
#ifdef USE_LOCAL_COMPILER
    GLSLCompiler *compiler = ((GLThreadContext *)context)->glsl_compiler.get();
    std::string real_binary;
    uint64_t real_binary_length = 0;
    GLenum real_binary_format = 0;
    compiler->program_binary(program, binaryFormat, binary, length,
                             &real_binary_format, &real_binary,
                             &real_binary_length);
    if (real_binary_length == 0 && real_binary.empty()) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }

    int program_data_len = 0;
    ALOGD("%llx program binary %u format %x", (long long)context, program,
          binaryFormat);

    r_glProgramBinary_special(context, program, real_binary_format,
                              real_binary.data(), real_binary_length,
                              &program_data_len);
#else
    r_glProgramBinary_special(context, program, binaryFormat, binary, length);
    GLSLProgram *program_info =
        (*((GLThreadContext *)context)->program_info_map)[program];
    program_info->set_program_binary(binary, length);
    if (!program_info->is_initialized()) program_info->init(context);
#endif
}

void d_glGetProgramBinary_special(void *context, GLuint program,
                                  GLsizei bufSize, GLsizei *length,
                                  GLenum *binaryFormat, void *binary) {
    GLSLProgram *program_info =
        (*((GLThreadContext *)context)->program_info_map)[program];
    if (program_info->get_program_binary(context, binary, bufSize, length,
                                         binaryFormat))
        return;

    GLsizei real_length = 0;
    r_glGetProgramBinary(context, program, bufSize, &real_length, binaryFormat,
                         binary);
    if (length != nullptr) {
        *length = real_length;
    }
    program_info->set_program_binary(binary, real_length);
}

void d_glGetShaderiv_special(void *context, GLuint shader, GLenum pname,
                             GLint *params) {
    auto *opengl_context = (GLThreadContext *)context;
    GLSLShader *shader_info = nullptr;
    if (opengl_context->shader_map->find(shader) !=
        opengl_context->shader_map->end()) {
        shader_info = (*opengl_context->shader_map)[shader].get();
    }
    if (shader_info == nullptr && pname != GL_DELETE_STATUS) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    switch (pname) {
        case GL_SHADER_TYPE:
            *params = shader_info->shader_type;
            break;
        case GL_DELETE_STATUS:
            *params = shader_info == nullptr ? GL_TRUE : GL_FALSE;
            break;
        case GL_COMPILE_STATUS:
            *params = shader_info->compile_status;
            break;
        case GL_INFO_LOG_LENGTH:
            *params = 0 /* No info is good info */;
            break;
        case GL_SHADER_SOURCE_LENGTH:
            *params = shader_info->shader_source_length;
            break;
        default:
            set_gl_error(context, GL_INVALID_ENUM);
            break;
    }
}

void d_glGetShaderPrecisionFormat_special(void *context, GLenum shadertype,
                                          GLenum precisiontype, GLint *range,
                                          GLint *precision) {
    int row = shadertype == GL_VERTEX_SHADER ? 0 : 1;
    GLenum precision_types[6] = {GL_LOW_FLOAT, GL_MEDIUM_FLOAT, GL_HIGH_FLOAT,
                                 GL_LOW_INT,   GL_MEDIUM_INT,   GL_HIGH_INT};
    int64_t col =
        std::find(precision_types, precision_types + 6, precisiontype) -
        precision_types;
    range[0] = process_static_context_values
                   ->shader_precision_ranges[row * 6 * 2 + col * 2];
    range[1] = process_static_context_values
                   ->shader_precision_ranges[row * 6 * 2 + col * 2 + 1];
    *precision =
        process_static_context_values->shader_precisions[row * 6 + col];
}

void d_glGetProgramiv_special(void *context, GLuint program, GLenum pname,
                              GLint *params) {
    if (((GLThreadContext *)context)->program_info_map->find(program) ==
        ((GLThreadContext *)context)->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    GLSLProgram *now_program =
        ((GLThreadContext *)context)->program_info_map[0][program];

    if (now_program == nullptr) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    now_program->get_programiv(context, pname, params);
}

void d_glGetProgramInterfaceiv_special(void *context, GLuint program,
                                       GLenum programInterface, GLenum pname,
                                       GLint *params) {
    auto *opengl_context = (GLThreadContext *)context;
    if (opengl_context->program_info_map->find(program) ==
        opengl_context->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    (*opengl_context->program_info_map)[program]->get_program_interfaceiv(
        context, programInterface, pname, params);

#ifdef REMOTE_PROGRAM_TEST
    int test_param = -1;
    r_glGetProgramInterfaceiv(context, program, programInterface, pname,
                              &test_param);
    if (test_param != *params) {
        ALOGE("REMOTE TEST FAIL! GetProgramInterfaceiv %d %d %d %d", program,
              programInterface, pname, *params);
    }
#endif
}

GLuint d_glGetProgramResourceIndex_special(void *context, GLuint program,
                                           GLenum programInterface,
                                           const GLchar *name) {
    auto *opengl_context = (GLThreadContext *)context;
    if (opengl_context->program_info_map->find(program) ==
        opengl_context->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_VALUE);
        return GL_INVALID_INDEX;
    }

    GLuint index =
        (*opengl_context->program_info_map)[program]
            ->get_program_resource_index(context, programInterface, name);
    if (index == GL_INVALID_INDEX) {
        // Try with [0] suffix
        index = (*opengl_context->program_info_map)[program]
                    ->get_program_resource_index(
                        context, programInterface,
                        (std::string(name) + "[0]").c_str());
    }
#ifndef REMOTE_PROGRAM_TEST
    return index;
#else
    GLuint test_index =
        r_glGetProgramResourceIndex(context, program, programInterface, name);
    if (test_index != index) {
        ALOGE("REMOTE TEST FAIL! GetProgramResourceIndex %d %d %s %ud", program,
              programInterface, name, index);
    }
    return index;
#endif
}

GLint d_glGetProgramResourceLocation_special(void *context, GLuint program,
                                             GLenum programInterface,
                                             const GLchar *name) {
    auto *opengl_context = (GLThreadContext *)context;
    if (opengl_context->program_info_map->find(program) ==
        opengl_context->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_VALUE);
        return -1;
    }

    int location =
        (*opengl_context->program_info_map)[program]
            ->get_program_resource_location(context, programInterface, name);
    if (location == -1) {
        // Try with [0] suffix
        location = (*opengl_context->program_info_map)[program]
                       ->get_program_resource_location(
                           context, programInterface,
                           (std::string(name) + "[0]").c_str());
    }
#ifndef REMOTE_PROGRAM_TEST
    return location;
#else
    int test_location = r_glGetProgramResourceLocation(context, program,
                                                       programInterface, name);
    if (test_location != location) {
        ALOGE("REMOTE TEST FAIL! GetProgramResourceLocation %d %d %s %d",
              program, programInterface, name, location);
    }
    return location;
#endif
}

void d_glGetProgramResourceName_special(void *context, GLuint program,
                                        GLenum programInterface, GLuint index,
                                        GLsizei bufSize, GLsizei *length,
                                        GLchar *name) {
    auto *opengl_context = (GLThreadContext *)context;
    if (opengl_context->program_info_map->find(program) ==
        opengl_context->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    (*opengl_context->program_info_map)[program]->get_program_resource_name(
        context, programInterface, index, bufSize, length, name);
#ifdef REMOTE_PROGRAM_TEST
    auto test_name = std::make_unique<char[]>(bufSize);
    int test_length = 0;
    r_glGetProgramResourceName(context, program, programInterface, index,
                               bufSize, &test_length, test_name.get());
    if (strcmp(test_name.get(), name) != 0 ||
        (length != nullptr && *length != test_length)) {
        ALOGE("REMOTE TEST FAIL! GetProgramResourceName %d %d %d %s %s",
              program, programInterface, index, name, test_name.get());
    }
#endif
}

void d_glGetProgramResourceiv_special(void *context, GLuint program,
                                      GLenum programInterface, GLuint index,
                                      GLsizei propCount, const GLenum *props,
                                      GLsizei bufSize, GLsizei *length,
                                      GLint *params) {
    auto *opengl_context = (GLThreadContext *)context;
    if (opengl_context->program_info_map->find(program) ==
        opengl_context->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    (*opengl_context->program_info_map)[program]->get_program_resourceiv(
        context, programInterface, index, propCount, props, bufSize, length,
        params);
#ifdef REMOTE_PROGRAM_TEST
    auto test_params = std::make_unique<int[]>(bufSize);
    int test_length = 0;
    r_glGetProgramResourceiv(context, program, programInterface, index,
                             propCount, props, bufSize, &test_length,
                             test_params.get());
    if (memcmp(test_params.get(), params, test_length * sizeof(int)) != 0 ||
        (length != nullptr && *length != test_length)) {
        ALOGE("REMOTE TEST FAIL! GetProgramResourceiv %d %d %d %d %d", program,
              programInterface, index, *params, *test_params.get());
    }
#endif
}

void d_glGetActiveAttrib_special(void *context, GLuint program, GLuint index,
                                 GLsizei bufSize, GLsizei *length, GLint *size,
                                 GLenum *type, GLchar *name) {
    const GLenum props[] = {GL_ARRAY_SIZE, GL_TYPE};
    if (name != nullptr)
        d_glGetProgramResourceName_special(context, program, GL_PROGRAM_INPUT,
                                           index, bufSize, length, name);
    if (size != nullptr)
        d_glGetProgramResourceiv_special(context, program, GL_PROGRAM_INPUT,
                                         index, 1, &props[0], 1, nullptr, size);
    if (type != nullptr)
        d_glGetProgramResourceiv_special(context, program, GL_PROGRAM_INPUT,
                                         index, 1, &props[1], 1, nullptr,
                                         (int *)type);
}

GLint d_glGetAttribLocation_special(void *context, GLuint program,
                                    const GLchar *name) {
    return d_glGetProgramResourceLocation_special(context, program,
                                                  GL_PROGRAM_INPUT, name);
}

void d_glGetActiveUniform_special(void *context, GLuint program, GLuint index,
                                  GLsizei bufSize, GLsizei *length, GLint *size,
                                  GLenum *type, GLchar *name) {
    const GLenum props[] = {GL_ARRAY_SIZE, GL_TYPE};
    if (name != nullptr)
        d_glGetProgramResourceName_special(context, program, GL_UNIFORM, index,
                                           bufSize, length, name);
    if (size != nullptr)
        d_glGetProgramResourceiv_special(context, program, GL_UNIFORM, index, 1,
                                         &props[0], 1, nullptr, size);
    if (type != nullptr)
        d_glGetProgramResourceiv_special(context, program, GL_UNIFORM, index, 1,
                                         &props[1], 1, nullptr, (int *)type);
}

void d_glGetUniformIndices_special(void *context, GLuint program,
                                   GLsizei uniformCount,
                                   const GLchar *const *uniformNames,
                                   GLuint *uniformIndices) {
    for (int i = 0; i < uniformCount; i++) {
        uniformIndices[i] = d_glGetProgramResourceIndex_special(
            context, program, GL_UNIFORM, uniformNames[i]);
    }
}

GLint d_glGetUniformLocation_special(void *context, GLuint program,
                                     const GLchar *name) {
    return d_glGetProgramResourceLocation_special(context, program, GL_UNIFORM,
                                                  name);
}

void d_glGetActiveUniformsiv_special(void *context, GLuint program,
                                     GLsizei uniformCount,
                                     const GLuint *uniformIndices, GLenum pname,
                                     GLint *params) {
    GLenum prop;
    switch (pname) {
        case GL_UNIFORM_TYPE:
            prop = GL_TYPE;
            break;
        case GL_UNIFORM_SIZE:
            prop = GL_ARRAY_SIZE;
            break;
        case GL_UNIFORM_NAME_LENGTH:
            prop = GL_NAME_LENGTH;
            break;
        case GL_UNIFORM_BLOCK_INDEX:
            prop = GL_BLOCK_INDEX;
            break;
        case GL_UNIFORM_OFFSET:
            prop = GL_OFFSET;
            break;
        case GL_UNIFORM_ARRAY_STRIDE:
            prop = GL_ARRAY_STRIDE;
            break;
        case GL_UNIFORM_MATRIX_STRIDE:
            prop = GL_MATRIX_STRIDE;
            break;
        case GL_UNIFORM_IS_ROW_MAJOR:
            prop = GL_IS_ROW_MAJOR;
            break;
        default:
            set_gl_error(context, GL_INVALID_ENUM);
            return;
    }
    for (int i = 0; i < uniformCount; i++) {
        d_glGetProgramResourceiv_special(context, program, GL_UNIFORM,
                                         uniformIndices[i], 1, &prop, 1,
                                         nullptr, &params[i]);
    }
}

GLuint d_glGetUniformBlockIndex_special(void *context, GLuint program,
                                        const char *uniformBlockName) {
    return d_glGetProgramResourceIndex_special(
        context, program, GL_UNIFORM_BLOCK, uniformBlockName);
}

void d_glUniformBlockBinding_special(void *context, GLuint program,
                                     GLuint uniformBlockIndex,
                                     GLuint uniformBlockBinding) {
    auto *opengl_context = (GLThreadContext *)context;
    if (opengl_context->program_info_map->find(program) ==
        opengl_context->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    (*opengl_context->program_info_map)[program]->uniform_block_binding(
        context, uniformBlockIndex, uniformBlockBinding);
}

void d_glGetActiveUniformBlockName_special(void *context, GLuint program,
                                           GLuint uniformBlockIndex,
                                           GLsizei bufSize, GLsizei *length,
                                           GLchar *uniformBlockName) {
    d_glGetProgramResourceName_special(context, program, GL_UNIFORM_BLOCK,
                                       uniformBlockIndex, bufSize, length,
                                       uniformBlockName);
}

void d_glGetActiveUniformBlockiv_special(void *context, GLuint program,
                                         GLuint uniformBlockIndex, GLenum pname,
                                         GLint *params) {
    GLenum prop;
    int max_size = 1;
    switch (pname) {
        case GL_UNIFORM_BLOCK_BINDING:
            prop = GL_BUFFER_BINDING;
            break;
        case GL_UNIFORM_BLOCK_DATA_SIZE:
            prop = GL_BUFFER_DATA_SIZE;
            break;
        case GL_UNIFORM_BLOCK_NAME_LENGTH:
            prop = GL_NAME_LENGTH;
            break;
        case GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS:
            prop = GL_NUM_ACTIVE_VARIABLES;
            break;
        case GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES:
            prop = GL_ACTIVE_VARIABLES;
            d_glGetActiveUniformBlockiv_special(
                context, program, uniformBlockIndex,
                GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &max_size);
            break;
        case GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER:
            prop = GL_REFERENCED_BY_VERTEX_SHADER;
            break;
        case GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER:
            prop = GL_REFERENCED_BY_FRAGMENT_SHADER;
            break;
        default:
            set_gl_error(context, GL_INVALID_ENUM);
            return;
    }
    d_glGetProgramResourceiv_special(context, program, GL_UNIFORM_BLOCK,
                                     uniformBlockIndex, 1, &prop, max_size,
                                     nullptr, params);
}