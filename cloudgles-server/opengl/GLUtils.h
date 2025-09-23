#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>

/**
 * @brief Calculate the byte size of a pixel based on the pixel format and type
 *
 * @param format Pixel format
 * @param type Pixel type
 * @return int
 */
int pixel_size_calc(GLenum format, GLenum type);

/**
 * @brief Calculate the byte size of different data types of OpenGL
 *
 * @param type Data type
 * @return size_t
 */
unsigned long gl_sizeof(GLenum type);

size_t gl_pname_size(GLenum pname);

#endif // GL_UTILS_H