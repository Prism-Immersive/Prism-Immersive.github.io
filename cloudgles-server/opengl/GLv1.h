#ifndef GLV1_H
#define GLV1_H

#include <GLES/gl.h>

void d_glDrawTexiOES_special(void *context, GLint x, GLint y, GLint z,
                             GLint width, GLint height);

void d_glGetTexParameterxvOES_special(void *context, GLenum target,
                                      GLenum pname, GLfixed *params);

#endif  // GLV1_H
