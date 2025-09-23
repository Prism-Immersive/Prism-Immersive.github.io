#ifndef GL_MEMORY_H
#define GL_MEMORY_H

#include <GLES/gl.h>

class Memory {
   public:
    static void d_glBufferData_custom(void *context, GLenum target,
                                      GLsizeiptr size, const void *data,
                                      GLenum usage);
    static void d_glBufferSubData_custom(void *context, GLenum target,
                                         GLintptr offset, GLsizeiptr size,
                                         const void *data);

    static void d_glMapBufferRange_read(void *context, GLenum target,
                                        GLintptr offset, GLsizeiptr length,
                                        GLbitfield access, void *mem_buf);
    static void d_glMapBufferRange_write(void *context, GLenum target,
                                         GLintptr offset, GLsizeiptr length,
                                         GLbitfield access);
    static GLboolean d_glUnmapBuffer_special(void *context, GLenum target);

    static void d_glFlushMappedBufferRange_special(void *context, GLenum target,
                                                   GLintptr offset,
                                                   GLsizeiptr length,
                                                   const void *data);

   private:
    static GLuint get_guest_buffer_binding_id(void *context, GLenum target);
};

#endif  // GL_MEMORY_H
