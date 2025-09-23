#ifndef GL_HELPER_H
#define GL_HELPER_H

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "renderer/MainRenderer.h"

class GLHelper {
   public:
    static size_t gl_pname_size(GLenum pname);
    static void prepare_integer_value(Static_Context_Values *s_values);
    static size_t gl_sizeof(GLenum type);
    static void d_glPrintf(void *context, GLint buf_len,
                           const GLchar *out_string);

    static GLint glTestInt1(GLint a, GLuint b);
    static GLuint glTestInt2(GLint a, GLuint b);

    static GLint64 glTestInt3(GLint64 a, GLuint64 b);
    static GLuint64 glTestInt4(GLint64 a, GLuint64 b);

    static GLfloat glTestInt5(GLint a, GLuint b);
    static GLdouble glTestInt6(GLint a, GLuint b);

    static void glTestPointer1(GLint a, const GLint *b);

    static void glTestPointer2(GLint a, const GLint *b, GLint *c);

    static GLint d_glTestPointer3(void *context, GLint a, const GLint *b,
                                  GLint *c);

    static GLint glTestPointer4(GLint a, const GLint *b, GLint *c);

    static void glTestString(GLint a, GLint count, const GLchar *const *strings,
                             GLint buf_len, GLchar *char_buf);

    static void glTestIntAsyn(GLint a, GLuint b, GLfloat c, GLdouble d);
    static void glPrintfAsyn(GLint a, GLuint size, GLdouble c,
                             const GLchar *out_string);

    static GLuint load_shader(GLenum type, const char *shaderSrc);
    static int main_window_opengl_prepare(GLuint *program, GLuint *VAO);
};

#endif  // GL_HELPER_H
