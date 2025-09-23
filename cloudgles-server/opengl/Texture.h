#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>

#define INPUT_TYPE_NONE 0
#define INPUT_TYPE_FBO 1
#define INPUT_TYPE_TEXTURE 2

typedef struct PixelStoreStatus {
    // Data alignment standard
    int unpack_alignment;
    int pack_alignment;

    // Number of pixels in each row of the image
    // When it is 0, the number of pixels is the width of the image
    int unpack_row_length;
    int pack_row_length;

    // Number of rows to skip when reading from the image
    int unpack_skip_rows;
    int pack_skip_rows;

    // Number of pixels to skip for each row when reading from the image
    int unpack_skip_pixels;
    int pack_skip_pixels;

    // Number of images to skip when reading from all the images
    int unpack_skip_images;

    // Height of the image
    int unpack_image_height;
} PixelStoreStatus;

typedef struct FormatBits {
    int red_bits = 0;
    int green_bits = 0;
    int blue_bits = 0;
    int alpha_bits = 0;
    int depth_bits = 0;
    int stencil_bits = 0;
} FormatBits;

class TextureInfo {
   public:
    GLsizei width[6];
    GLsizei height[6];
    GLsizei depth[6];
    GLint crop_rect[4];
    GLenum format[6];
    GLint level[6];

    GLint samples[6];
    FormatBits format_bits[6];

    GLboolean fixedsamplelocations[6];

    GLenum type;

    GLint is_compressed[6];

    GLint buffer_binding[6];
    GLintptr buffer_offset[6];
    GLsizeiptr buffer_size[6];
    GLint immutable[6];

    GLint depth_stencil_mode;

    GLint mag_filter;
    GLint min_filter;
    GLint wrap_r;
    GLint wrap_s;
    GLint wrap_t;
    // GLint compare_func;
    // GLint compare_mode;
    GLint swizzle_r;
    GLint swizzle_g;
    GLint swizzle_b;
    GLint swizzle_a;
    GLfloat min_lod;
    GLfloat max_lod;
    GLint max_level;
    GLint base_level;

    GLint compare_mode;
    GLint compare_func;

    TextureInfo();
    static TextureInfo *get_texture_info(void *context, GLenum target);
    static FormatBits *get_format_bits(GLint texture, GLenum target);
};

typedef struct ImageInfo {
    GLboolean layered;
    GLuint texture_name;
    GLint level;
    GLint layer;
    GLenum access;
    GLenum format;
} ImageInfo;

typedef struct EGLImageInfo {
    GLenum target;

    GLuint share_texture_id;
    EGLContext share_context;

    int width;
    int height;
    int format;
    int stride;

    uint64_t usage;

    // GLuint texture_id;
    // GLuint fbo_id;

    int gbuffer_type;
    // int input_type;
    int need_data_back;

    int need_destroy;
    int using_cnt;

    int need_send_data;

    // In EGL_GL_TEXTURE_2D, the gbuffer_id is the unique identifier of the texture, 
    // which is generated from the address of the opengl_context at that time + share_texture_id
    uint64_t gbuffer_id;
    void *graphic_buffer;

} EGLImageInfo;

void internal_format_to_format_bits(GLint internalformat, GLint format,
                                    GLint type, FormatBits *format_bits);

void upload_graphic_buffer(void *context, EGLImageInfo *image_info);

void download_graphic_buffer(void *context, EGLImageInfo *image_info);

void try_destroy_EGLImage(void *context, EGLImageInfo *image_info);

// void copy_graphic_buffer(void *context, const void *pixels, int start, int
// len);

void d_glPixelStorei(void *context, GLenum pname, GLint param);

uint64_t gl_pixel_data_size(void *context, GLsizei width, GLsizei height,
                            GLenum format, GLenum type, int pack,
                            int *start_loc, int *len);

uint64_t gl_pixel_data_3d_size(void *context, GLsizei width, GLsizei height,
                               GLsizei depth, GLenum format, GLenum type,
                               int pack, int *start_loc, int *len);

void d_glTexStorage2D_special(void *context, GLenum target, GLsizei levels,
                              GLenum internalformat, GLsizei width,
                              GLsizei height);

void d_glTexParameterfv_special(void *context, GLenum target, GLenum pname,
                                const GLfloat *params);

void d_glTexParameteriv_special(void *context, GLenum target, GLenum pname,
                                const GLint *params);

void d_glGetTexParameterfv_special(void *context, GLenum target, GLenum pname,
                                   GLfloat *params);

void d_glGetTexParameteriv_special(void *context, GLenum target, GLenum pname,
                                   GLint *params);

void d_glEGLImageTargetTexture2DOES_special(void *context, GLenum target,
                                            GLeglImageOES image);

bool tex_parameter_should_set(void *context, GLenum target, GLenum pname,
                              GLint param);

bool tex_parameter_should_set(void *context, GLenum target, GLenum pname,
                              GLfloat param);

GLint get_image_info(void *context, GLuint unit, GLenum target);

void d_glBindImageTexture_special(void *context, GLuint unit, GLuint texture,
                                  GLint level, GLboolean layered, GLint layer,
                                  GLenum access, GLenum format);

void d_glTexBufferRange_special(void *context, GLenum target,
                                GLenum internalformat, GLuint buffer,
                                GLintptr offset, GLsizeiptr size);

void d_glTexBuffer_special(void *context, GLenum target, GLenum internalformat,
                           GLuint buffer);

void d_glGetTexLevelParameteriv_special(void *context, GLenum target,
                                        GLint level, GLenum pname,
                                        GLint *params);

void d_glGetTexLevelParameterfv_special(void *context, GLenum target,
                                        GLint level, GLenum pname,
                                        GLfloat *params);

void d_glTexStorage2DMultisample_special(void *context, GLenum target,
                                         GLsizei samples, GLenum internalformat,
                                         GLsizei width, GLsizei height,
                                         GLboolean fixedsamplelocations);

void d_glTexStorage3D_special(void *context, GLenum target, GLsizei levels,
                              GLenum internalformat, GLsizei width,
                              GLsizei height, GLsizei depth);

#endif  // GL_TEXTURE_H
