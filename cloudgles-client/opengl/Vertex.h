#ifndef GL_VERTEX_H
#define GL_VERTEX_H

#include <GLES/gl.h>

#define BUFFER_MULTIPLY_FACTOR 10

class Vertex {
   public:
    static GLint set_vertex_attrib_data(void *context, GLuint index,
                                        GLuint offset, GLuint length,
                                        const void *pointer);

    static void d_glVertexAttribPointer_without_bound(
        void *context, GLuint index, GLint size, GLenum type,
        GLboolean normalized, GLsizei stride, GLuint offset, GLuint length,
        const void *pointer);
    static void d_glVertexAttribIPointer_without_bound(
        void *context, GLuint index, GLint size, GLenum type, GLsizei stride,
        GLuint offset, GLuint length, const void *pointer);

    static void d_glVertexAttribPointer_offset(void *context, GLuint index,
                                               GLuint size, GLenum type,
                                               GLboolean normalized,
                                               GLsizei stride,
                                               GLuint index_father,
                                               GLintptr offset);

    static void d_glVertexAttribIPointer_offset(void *context, GLuint index,
                                                GLint size, GLenum type,
                                                GLsizei stride,
                                                GLuint index_father,
                                                GLintptr offset);

    static void d_glVertexAttribPointer_with_bound(void *context, GLuint index,
                                                   GLint size, GLenum type,
                                                   GLboolean normalized,
                                                   GLsizei stride,
                                                   GLintptr pointer);
    static void d_glVertexAttribIPointer_with_bound(void *context, GLuint index,
                                                    GLint size, GLenum type,
                                                    GLsizei stride,
                                                    GLintptr pointer);

    // void d_glGenVertexArrays_origin(void *context, GLsizei n, GLuint
    // *arrays); void d_glDeleteVertexArrays_origin(void *context, GLsizei n,
    // const GLuint *arrays); void d_glBindVertexArray_special(void *context,
    // GLuint array);

    static void d_glVertexAttribDivisor_origin(void *context, GLuint index,
                                               GLuint divisor);

    static void d_glDisableVertexAttribArray_origin(void *context,
                                                    GLuint index);
    static void d_glEnableVertexAttribArray_origin(void *context, GLuint index);

    static void d_glDrawArrays_origin(void *context, GLenum mode, GLint first,
                                      GLsizei count);
    static void d_glDrawArraysInstanced_origin(void *context, GLenum mode,
                                               GLint first, GLsizei count,
                                               GLsizei instancecount);

    static void d_glDrawElements_with_bound(void *context, GLenum mode,
                                            GLsizei count, GLenum type,
                                            GLsizeiptr indices);

    static GLint set_indices_data(void *context, void *pointer, GLint length);

    static void d_glDrawElements_without_bound(void *context, GLenum mode,
                                               GLsizei count, GLenum type,
                                               const void *indices);

    static void d_glDrawElementsInstanced_without_bound(
        void *context, GLenum mode, GLsizei count, GLenum type,
        const void *indices, GLsizei instancecount);
    static void d_glDrawElementsInstanced_with_bound(void *context, GLenum mode,
                                                     GLsizei count, GLenum type,
                                                     GLsizeiptr indices,
                                                     GLsizei instancecount);

    static void d_glDrawRangeElements_with_bound(void *context, GLenum mode,
                                                 GLuint start, GLuint end,
                                                 GLsizei count, GLenum type,
                                                 GLsizeiptr indices);
    static void d_glDrawRangeElements_without_bound(void *context, GLenum mode,
                                                    GLuint start, GLuint end,
                                                    GLsizei count, GLenum type,
                                                    const void *indices);

    static void d_glVertexBindingDivisor_special(void *context,
                                                 GLuint bindingindex,
                                                 GLuint divisor);

    static void d_glDrawArraysIndirect_with_bound(void *context, GLenum mode,
                                                  GLintptr indirect);
    static void d_glDrawArraysIndirect_without_bound(void *context, GLenum mode,
                                                     const void *indirect);

    static void d_glDrawElementsIndirect_with_bound(void *context, GLenum mode,
                                                    GLenum type,
                                                    GLintptr indirect);
    static void d_glDrawElementsIndirect_without_bound(void *context,
                                                       GLenum mode, GLenum type,
                                                       const void *indirect);
};

#endif  // GL_VERTEX_H
