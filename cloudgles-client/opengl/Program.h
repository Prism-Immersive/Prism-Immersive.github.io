#ifndef GL_PROGRAM_H
#define GL_PROGRAM_H

#include <GLES/gl.h>

#include <unordered_map>

#include "flatbuffers/flatbuffers.h"

class GLProgram {
   public:
    static std::unordered_map<GLuint, GLuint> program_is_external_map;
    static std::unordered_map<GLuint, GLchar *> program_data_map;

    static void init_uniform_data(GLuint program, int num_uniforms,
                                  int max_name_len, int *has_image,
                                  flatbuffers::FlatBufferBuilder &fbb);
    static void init_attribute_data(GLuint program, int num_attributes,
                                    int max_name_len,
                                    flatbuffers::FlatBufferBuilder &fbb);
    static void init_uniform_block_data(GLuint program, int num_uniform_blocks,
                                       int max_name_len,
                                       flatbuffers::FlatBufferBuilder &fbb);
    static void init_program_output_data(GLuint program, int num_program_outputs,
                                       flatbuffers::FlatBufferBuilder &fbb);
    static void init_buffer_variable_data(GLuint program, int num_buffer_variables,
                                        flatbuffers::FlatBufferBuilder &fbb);
    static void init_transform_feedback_varying_data(GLuint program,
                                                     int num_transform_feedback_varyings,
                                                     flatbuffers::FlatBufferBuilder &fbb);
    static void init_shader_storage_block_data(GLuint program, int num_shader_storage_blocks,
                                              flatbuffers::FlatBufferBuilder &fbb);
    static void init_atomic_counter_buffer_data(GLuint program, int num_atomic_counter_buffers,
                                              flatbuffers::FlatBufferBuilder &fbb);
    static int init_program_data(GLuint program);

    static void d_glLinkProgram_special(void *context, GLuint program);
    static void d_glProgramBinary_special(void *context, GLuint program,
                                          GLenum binaryFormat,
                                          const void *binary, GLsizei length);
    static void d_glGetProgramDataSize(void *context, GLuint program,
                                       GLsizei *data_len);
    static void d_glGetProgramData(void *context, GLuint program, int buf_len,
                                   void *program_data);
    static void d_glShaderSource_special(void *context, GLuint shader,
                                         GLsizei count, GLint *length,
                                         GLchar **string);
    static void d_glUseProgram_special(void *context, GLuint program);
    static void d_glCompileShader_special(void *context, GLuint id);

    static void g_program_data_destroy(GLchar *data);
};

#endif  // GL_PROGRAM_H
