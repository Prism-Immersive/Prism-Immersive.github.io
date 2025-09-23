#ifndef GL_V1_H
#define GL_V1_H

#include <GLES/gl.h>

class GLv1 {
   public:
    static void d_glTexEnvf_special(void *context, GLenum target, GLenum pname,
                                    GLfloat param);
    static void d_glTexEnvi_special(void *context, GLenum target, GLenum pname,
                                    GLint param);
    static void d_glTexEnvx_special(void *context, GLenum target, GLenum pname,
                                    GLfixed param);
    static void d_glTexParameterx_special(void *context, GLenum target,
                                          GLenum pname, GLint param);
    static void d_glShadeModel_special(void *context, GLenum mode);
    static void d_glDrawTexiOES_special(void *context, GLint x, GLint y,
                                        GLint z, GLint width, GLint height,
                                        GLfloat left_x, GLfloat right_x,
                                        GLfloat bottom_y, GLfloat top_y);
    static void prepare_draw_texi(void);
    static GLuint draw_texi_program;
    static GLint draw_texi_texture_id_loc;
};

#endif  // GL_V1_H
