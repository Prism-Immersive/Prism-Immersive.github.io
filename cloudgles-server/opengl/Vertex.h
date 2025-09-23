#ifndef GL_VERTEX_H
#define GL_VERTEX_H

#include <GLES3/gl3.h>
#include <stdint.h>

#include <cstdint>

#include "GLES/gl.h"
#include "GLES2/gl2.h"

#define MAX_VERTEX_ATTRIBS_NUM 32
// Minimum value is 16. Set to 32 here, and adjust according to the value on the
// host side later
#define MAX_VERTEX_ATTRIB_BINDINGS_NUM 32

typedef struct AttribPoint {
    GLboolean has_update;

    // The pointer to the vertex data if using pointer mode
    GLboolean location[MAX_VERTEX_ATTRIBS_NUM];

    // Common attributes of the vertex array
    GLboolean normalized[MAX_VERTEX_ATTRIBS_NUM];
    GLuint size[MAX_VERTEX_ATTRIBS_NUM];
    GLenum type[MAX_VERTEX_ATTRIBS_NUM];
    GLuint stride[MAX_VERTEX_ATTRIBS_NUM];
    GLuint relativeoffset[MAX_VERTEX_ATTRIBS_NUM];

    GLuint array_buffer_binding[MAX_VERTEX_ATTRIBS_NUM];

    GLuint element_array_buffer_binding;

    // Whether the vertex array is enabled
    GLboolean enabled[MAX_VERTEX_ATTRIBS_NUM];

    GLuint divisor[MAX_VERTEX_ATTRIBS_NUM];
    GLenum invoke_type[MAX_VERTEX_ATTRIBS_NUM];

    // TODO: Note that the vertex data may change during the draw, so the data
    // needs to be copied each time We can optimize this when the data is a
    // structure array
    const char *data[MAX_VERTEX_ATTRIBS_NUM];

    GLuint bindingindex[MAX_VERTEX_ATTRIBS_NUM];

    GLuint vertex_binding_buffer[MAX_VERTEX_ATTRIB_BINDINGS_NUM];
    GLintptr vertex_binding_offset[MAX_VERTEX_ATTRIB_BINDINGS_NUM];
    GLuint vertex_binding_stride[MAX_VERTEX_ATTRIB_BINDINGS_NUM];
    GLuint vertex_binding_divisor[MAX_VERTEX_ATTRIB_BINDINGS_NUM];

    GLuint max_index;

    // Indicate with which attribute this attribute pointer is in the same array
    GLint data_index_father[MAX_VERTEX_ATTRIBS_NUM];

} AttribPoint;

typedef struct ElementArrayBuffer {
    char *data;
    size_t size;
    GLenum type;

    // ST algorithm for RMQ (Range Minimum Query)
    GLboolean has_update;

    GLuint **dp_max;
    GLuint **dp_min;
    GLuint dp_num;
} ElementArrayBuffer;

typedef struct {
    uint32_t count;
    uint32_t primCount;
    uint32_t first;
    uint32_t reserved;
} DrawArraysIndirectCommand;

typedef struct {
    uint32_t count;
    uint32_t instanceCount;
    uint32_t firstIndex;
    int baseVertex;
    uint32_t reservedMustBeZero;
} DrawElementsIndirectCommand;

void get_max_min_point_index_direct(void *context, const void *data,
                                    GLsizei count, GLenum type,
                                    GLuint *min_index, GLuint *max_index);

void send_point_attrib_data_indices(void *context, GLsizei count, GLenum type,
                                    const void *indices, GLint instancecount);

void get_max_min_point_index_rmq(void *context, uint64_t offset, GLsizei count,
                                 GLenum type, GLuint *min_index,
                                 GLuint *max_index);

void send_point_attrib_data(void *context, GLuint min_index, GLuint max_index,
                            GLint instancecount);

void d_glGenVertexArrays_special(void *context, GLsizei n, GLuint *arrays);

void d_glBindVertexArray_special(void *context, GLuint array);

void d_glDeleteVertexArrays_special(void *context, GLsizei n,
                                    const GLuint *arrays);

void init_vertex_attrib_offset(void *context, GLuint index, GLint size,
                               GLenum type, GLboolean normalized,
                               GLsizei stride, GLenum invoke_type,
                               const void *pointer);

void init_vertex_attrib_pointer(void *context, GLuint index, GLint size,
                                GLenum type, GLboolean normalized,
                                GLsizei stride, GLenum invoke_type,
                                const void *pointer);

int get_vertex_attrib(void *context, GLuint index, GLenum pname, GLint *param);

int get_vertex_attrib_float(void *context, GLuint index, GLenum pname,
                            GLfloat *param);

GLintptr get_vertex_binding_status(void *context, GLenum target, GLuint index);

void send_gbuffer_data(void *context);

void d_glBindVertexBuffer_special(void *context, GLuint bindingindex,
                                  GLuint buffer, GLintptr offset,
                                  GLsizei stride);

void d_glVertexAttribFormat_special(void *context, GLuint attribindex,
                                    GLint size, GLenum type,
                                    GLboolean normalized,
                                    GLuint relativeoffset);

void d_glVertexAttribIFormat_special(void *context, GLuint attribindex,
                                     GLint size, GLenum type,
                                     GLuint relativeoffset);

void d_glVertexAttribBinding_special(void *context, GLuint attribindex,
                                     GLuint bindingindex);

void d_glVertexBindingDivisor_special(void *context, GLuint bindingindex,
                                      GLuint divisor);

void d_glDrawArraysIndirect_special(void *context, GLenum mode,
                                    const void *indirect);

void d_glDrawElementsIndirect_special(void *context, GLenum mode, GLenum type,
                                      const void *indirect);

#endif  // GL_VERTEX_H
