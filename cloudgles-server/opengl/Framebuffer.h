#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define NUM_ATTACHMENTS 35

#include "Texture.h"

typedef struct Attachment {
    GLuint handle = 0;
    GLenum type = 0;
    GLenum target = 0;
    GLint level = 0;
    bool is_attached = false;
} Attachment;

inline int type_to_attachment_idx(GLenum type) {
    if (type == GL_COLOR_ATTACHMENT0)
        return 0;
    else if (type == GL_DEPTH_ATTACHMENT)
        return 1;
    else if (type == GL_STENCIL_ATTACHMENT)
        return 2;
    else if (type >= GL_COLOR_ATTACHMENT1 && type <= GL_COLOR_ATTACHMENT31) {
        return type - GL_COLOR_ATTACHMENT1 + 3;
    } else {
        return -1;
    }
}

class RenderBufferInfo {
   public:
    static RenderBufferInfo *get_rb_info();
    static RenderBufferInfo *get_rb_info(GLuint renderbuffer);
    static FormatBits *get_format_bits(GLuint renderbuffer);

    GLsizei width = 0;
    GLsizei height = 0;
    GLsizei samples = 0;
    GLenum internalformat = GL_RGBA4;
    FormatBits format_bits;
};

class FrameBufferInfo {
   public:
    void attach(GLuint handle, GLenum type, GLenum target = 0, GLint level = 0);

    void detach(GLenum type);

    void get_format_bits(GLenum pname, GLint *data);

    static FrameBufferInfo *get_fb_info(GLenum target);

    static void detach_texture(GLuint texture);
    static void detach_renderbuffer(GLuint renderbuffer);

   private:
    FormatBits default_format_bits;
    Attachment attachments[NUM_ATTACHMENTS];

    FormatBits *get_format_bits_internal(Attachment *attachment);
};

void d_glFramebufferTexture2D_special(void *context, GLenum target,
                                      GLenum attachment, GLenum textarget,
                                      GLuint texture, GLint level);

void d_glFramebufferTextureLayer_special(void *context, GLenum target,
                                         GLenum attachment, GLuint texture,
                                         GLint level, GLint layer);

void d_glFramebufferRenderbuffer_special(void *context, GLenum target,
                                         GLenum attachment,
                                         GLenum renderbuffertarget,
                                         GLuint renderbuffer);

void d_glRenderbufferStorage_special(void *context, GLenum target,
                                     GLenum internalformat, GLsizei width,
                                     GLsizei height);

void d_glRenderbufferStorageMultisample_special(void *context, GLenum target,
                                                GLsizei samples,
                                                GLenum internalformat,
                                                GLsizei width, GLsizei height);

#endif  // FRAMEBUFFER_H