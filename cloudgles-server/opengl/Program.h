#ifndef PROGRAM_H
#define PROGRAM_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ProgramData/AtomicCounterBuffer_generated.h"
#include "ProgramData/Attribute_generated.h"
#include "ProgramData/BufferVariable_generated.h"
#include "ProgramData/ProgramOutput_generated.h"
#include "ProgramData/ShaderStorageBlock_generated.h"
#include "ProgramData/TransformFeedbackVarying_generated.h"
#include "ProgramData/UniformBlock_generated.h"
#include "ProgramData/Uniform_generated.h"
#include "flatbuffers/flatbuffers.h"
#include "opengl/GLDef.h"

class GLSLProgram {
   public:
    explicit GLSLProgram(GLuint id);
    ~GLSLProgram() = default;

    static void create(void *context, GLuint program_id);
    static void create_shader_program(void *context, GLuint program_id,
                                      GLenum type, GLsizei count,
                                      const GLchar *const *strings);
    static void destroy(void *context, GLuint program_id);

    static int replace_external_shader_source(char *string, int len);
    static void replace_standard_derivatives_shader_source(char *string,
                                                           int len);

    bool init(void *context);
    bool is_initialized() { return is_init; }
    void attach_shader(void *context, GLuint shader);
    void get_attached_shaders(void *context, GLsizei maxCount, GLsizei *count,
                              GLuint *shaders);
    bool detach_shader(void *context, GLuint shader);
    void set_external(int external);
    void set_program_binary(const void *binary, GLsizei length);
    bool get_program_binary(void *context, void *binary, GLsizei bufSize,
                            GLsizei *length, GLenum *binaryFormat);

    // Delay init in the following functions where program data is needed

    static size_t get_program_uniform_size(void *context, GLuint program,
                                           GLint location);

    static size_t get_uniform_block_para_size(void *context, GLuint program,
                                              GLuint uniformBlockIndex,
                                              GLenum pname);

    static bool uniform_is_change(void *context, GLint program, GLint location,
                                  char *check_data, int size);

    static bool can_get_uniform(void *context, GLint program, GLint location,
                                char *get_data);
    /*---------------- Program query ----------------*/
    
    void get_programiv(void *context, GLenum pname, GLint *params);

    void get_program_interfaceiv(void *context, GLenum programInterface,
                                 GLenum pname, GLint *params);
    GLuint get_program_resource_index(void *context, GLenum programInterface,
                                      const GLchar *name);
    GLint get_program_resource_location(void *context, GLenum programInterface,
                                        const GLchar *name);
    void get_program_resource_name(void *context, GLenum programInterface,
                                   GLuint index, GLsizei bufSize,
                                   GLsizei *length, GLchar *name);
    void get_program_resourceiv(void *context, GLenum programInterface, GLuint index,
                               GLsizei propCount, const GLenum *props,
                               GLsizei bufSize, GLsizei *length,
                               GLint *params);
    void get_name_length(void *context, GLenum programInterface, GLuint index,
                         GLsizei *length);
    void get_type(void *context, GLenum programInterface, GLuint index,
                  GLint *type);
    void get_array_size(void *context, GLenum programInterface, GLuint index,
                        GLint *arraySize);
    void get_offset(void *context, GLenum programInterface, GLuint index,
                    GLint *offset);
    void get_block_index(void *context, GLenum programInterface, GLuint index,
                         GLint *blockIndex);
    void get_array_stride(void *context, GLenum programInterface, GLuint index,
                          GLint *arrayStride);
    void get_matrix_stride(void *context, GLenum programInterface, GLuint index,
                           GLint *matrixStride);
    void get_is_row_major(void *context, GLenum programInterface, GLuint index,
                          GLint *isRowMajor);
    void get_atomic_counter_buffer_index(void *context, GLenum programInterface,
                                         GLuint index, GLint *bufferIndex);
    void get_buffer_binding(void *context, GLenum programInterface, GLuint index,
                            GLint *bufferBinding);
    void get_buffer_data_size(void *context, GLenum programInterface,
                              GLuint index, GLint *bufferDataSize);
    void get_is_per_patch(void *context, GLenum programInterface, GLuint index,
                          GLint *isPerPatch);
    void get_num_active_variables(void *context, GLenum programInterface, GLuint index,
                                  GLint *numActiveVariables);
    int get_active_variables(void *context, GLenum programInterface, GLuint index, GLint bufSize, GLint *activeVariables);
    void get_ref_by_vertex_shader(void *context, GLenum programInterface, GLuint index, GLint *refByVertexShader);
    void get_ref_by_frag_shader(void *context, GLenum programInterface, GLuint index, GLint *refByFragShader);
    void get_ref_by_compute_shader(void *context, GLenum programInterface, GLuint index, GLint *refByComputeShader);
    void get_ref_by_tess_control_shader(void *context, GLenum programInterface, GLuint index, GLint *refByTessControlShader);
    void get_ref_by_tess_eval_shader(void *context, GLenum programInterface, GLuint index, GLint *refByTessEvalShader);
    void get_ref_by_geometry_shader(void *context, GLenum programInterface, GLuint index, GLint *refByGeometryShader);
    void get_top_level_array_size(void *context, GLenum programInterface, GLuint index, GLint *topLevelArraySize);
    void get_top_level_array_stride(void *context, GLenum programInterface, GLuint index, GLint *topLevelArrayStride);
    void get_location(void *context, GLenum programInterface, GLuint index, GLint *location);
    void get_location_index(void *context, GLenum programInterface, GLuint index, GLint *locationIndex);

    void uniform_block_binding(void *context, GLuint uniformBlockIndex,
                               GLuint uniformBlockBinding);

    int is_external = 0;

   private:
    char *init_basic_info(char *data);
    char *init_uniform_info(char *data);
    char *init_attrib_info(char *data);
    char *init_uniform_block_info(char *data);
    char *init_program_output_info(char *data);
    char *init_transform_feedback_varying_info(char *data);
    char *init_shader_storage_block_info(char *data);
    char *init_buffer_variable_info(char *data);
    char *init_atomic_counter_buffer_info(char *data);
    char *init_program_interface_info(char *data);

    bool is_init = false;

    // Basic program info
    int num_uniforms = 0;
    int num_attribs = 0;
    int num_uniform_blocks = 0;
    int max_uniform_name_len = 0;
    int max_attrib_name_len = 0;
    int max_uniform_block_name_len = 0;
    int transform_feedback_varyings = 0;
    int compute_work_group_size[3] = {0, 0, 0};
    int binary_length = 0;

    // Deduced program info
    int uniform_detected_num = 0;
    int max_uniform_location = -1;
    int max_attrib_location = -1;

    std::unordered_map<GLint, std::unique_ptr<char[]>> uniform_location_data;
    std::unordered_map<GLint, GLint> uniform_location_index;

    // Uniform info
    UniformsT uniforms;

    // Attribute info
    AttributesT attributes;

    // Uniform block info
    UniformBlocksT uniform_blocks;

    // Program Output info
    ProgramOutputsT program_outputs;

    // Transform Feedback Varying info
    TransformFeedbackVaryingsT varyings;

    // Shader Storage Block info
    ShaderStorageBlocksT storage_blocks;

    // Buffer Variable info
    BufferVariablesT buffer_variables;

    // Atomic Counter Buffer info
    AtomicCounterBuffersT counter_buffers;

#define INTERFACE_SIZE 9
#define INTERFACE_PNAME_SIZE 3
    GLenum program_interfaces[INTERFACE_SIZE] = {GL_UNIFORM,
                                                 GL_UNIFORM_BLOCK,
                                                 GL_ATOMIC_COUNTER_BUFFER,
                                                 GL_PROGRAM_INPUT,
                                                 GL_PROGRAM_OUTPUT,
                                                 GL_TRANSFORM_FEEDBACK_VARYING,
                                                 GL_BUFFER_VARIABLE,
                                                 GL_SHADER_STORAGE_BLOCK,
                                                 GL_TRANSFORM_FEEDBACK_BUFFER};
    GLenum program_interface_pnames[INTERFACE_PNAME_SIZE] = {
        GL_ACTIVE_RESOURCES, GL_MAX_NAME_LENGTH, GL_MAX_NUM_ACTIVE_VARIABLES};
    int program_interface_data[INTERFACE_SIZE * INTERFACE_PNAME_SIZE] = {0};

    std::unordered_map<GLuint, GLuint> attach_shaders;
    GLenum binary_format = 0;
#ifdef USE_LOCAL_COMPILER
    GLuint mock_program_id = 0;
#endif
    std::string program_binary;

    GLuint program_id;
    std::string program_data;
};

class GLSLShader {
   public:
    static void create(void *context, GLuint shader_id, GLenum type);
    static void destroy(void *context, GLuint shader_id);

    GLint shader_type = 0;
    GLint compile_status = 0;
    GLint shader_source_length = 0;
    GLuint mock_shader_id = 0;
};

/*---------------- Program/shader configurations and compilations
 * ----------------*/
void d_glUseProgram_special(void *context, GLuint program);
void d_glDetachShader_special(void *context, GLuint program, GLuint shader);

void d_glAttachShader_special(void *context, GLuint program, GLuint shader);

void d_glGetAttachedShaders_special(void *context, GLuint program,
                                    GLsizei maxCount, GLsizei *count,
                                    GLuint *shaders);

void d_glShaderSource_special(void *context, GLuint shader, GLsizei count,
                              const GLchar *const *string, const GLint *length);

void d_glLinkProgram_special(void *context, GLuint program);

void d_glProgramBinary_special(void *context, GLuint program,
                               GLenum binaryFormat, const void *binary,
                               GLsizei length);

void d_glGetProgramBinary_special(void *context, GLuint program,
                                  GLsizei bufSize, GLsizei *length,
                                  GLenum *binaryFormat, void *binary);

/*---------------- Shader query ----------------*/
void d_glGetShaderiv_special(void *context, GLuint shader, GLenum pname,
                             GLint *params);
void d_glGetShaderPrecisionFormat_special(void *context, GLenum shadertype,
                                          GLenum precisiontype, GLint *range,
                                          GLint *precision);

/*---------------- Program query ----------------*/
void d_glGetProgramiv_special(void *context, GLuint program, GLenum pname,
                              GLint *params);

void d_glGetProgramInterfaceiv_special(void *context, GLuint program,
                                       GLenum programInterface, GLenum pname,
                                       GLint *params);

GLuint d_glGetProgramResourceIndex_special(void *context, GLuint program,
                                           GLenum programInterface,
                                           const GLchar *name);

GLint d_glGetProgramResourceLocation_special(void *context, GLuint program,
                                             GLenum programInterface,
                                             const GLchar *name);

void d_glGetProgramResourceName_special(void *context, GLuint program,
                                        GLenum programInterface, GLuint index,
                                        GLsizei bufSize, GLsizei *length,
                                        GLchar *name);

void d_glGetProgramResourceiv_special(void *context, GLuint program,
                                      GLenum programInterface, GLuint index,
                                      GLsizei propCount, const GLenum *props,
                                      GLsizei bufSize, GLsizei *length,
                                      GLint *params);

/*---------------- Attributes query ----------------*/
void d_glGetActiveAttrib_special(void *context, GLuint program, GLuint index,
                                 GLsizei bufSize, GLsizei *length, GLint *size,
                                 GLenum *type, GLchar *name);
GLint d_glGetAttribLocation_special(void *context, GLuint program,
                                    const GLchar *name);

/*---------------- Uniform query ----------------*/
void d_glGetActiveUniform_special(void *context, GLuint program, GLuint index,
                                  GLsizei bufSize, GLsizei *length, GLint *size,
                                  GLenum *type, GLchar *name);
void d_glGetUniformIndices_special(void *context, GLuint program,
                                   GLsizei uniformCount,
                                   const GLchar *const *uniformNames,
                                   GLuint *uniformIndices);
GLint d_glGetUniformLocation_special(void *context, GLuint program,
                                     const GLchar *name);

void d_glGetActiveUniformsiv_special(void *context, GLuint program,
                                     GLsizei uniformCount,
                                     const GLuint *uniformIndices, GLenum pname,
                                     GLint *params);

/*---------------- Uniform block query ----------------*/
GLuint d_glGetUniformBlockIndex_special(void *context, GLuint program,
                                        const char *uniformBlockName);

void d_glUniformBlockBinding_special(void *context, GLuint program,
                                     GLuint uniformBlockIndex,
                                     GLuint uniformBlockBinding);

void d_glGetActiveUniformBlockName_special(void *context, GLuint program,
                                           GLuint uniformBlockIndex,
                                           GLsizei bufSize, GLsizei *length,
                                           GLchar *uniformBlockName);

void d_glGetActiveUniformBlockiv_special(void *context, GLuint program,
                                         GLuint uniformBlockIndex, GLenum pname,
                                         GLint *params);

#endif  // PROGRAM_H