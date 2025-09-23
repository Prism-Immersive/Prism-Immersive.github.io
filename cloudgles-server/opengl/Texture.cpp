#define LOG_TAG "GLTexture"
#include "Texture.h"

#include <GLES3/gl32.h>
#include <android/native_window.h>

#include "ErrorHandler.h"
#include "GLDef.h"
#include "GLUtils.h"
#include "Log.h"
#include "Memory.h"
#include "State.h"
#include "ThreadContext.h"
#include "egl/EglDef.h"

TextureInfo::TextureInfo() {
    memset(width, 0, sizeof(GLsizei) * 6);
    memset(height, 0, sizeof(GLsizei) * 6);
    memset(depth, 0, sizeof(GLsizei) * 6);
    memset(format, 0, sizeof(GLenum) * 6);
    memset(level, 0, sizeof(GLint) * 6);
    memset(level, 0, sizeof(GLint) * 6);
    memset(samples, 0, sizeof(GLint) * 6);
    memset(fixedsamplelocations, 0, sizeof(GLboolean) * 6);
    memset(is_compressed, 0, sizeof(GLint) * 6);

    memset(buffer_offset, 0, sizeof(GLintptr) * 6);
    memset(buffer_size, 0, sizeof(GLsizeiptr) * 6);
    memset(buffer_binding, 0, sizeof(GLint) * 6);

    memset(immutable, 0, sizeof(GLint) * 6);

    crop_rect[0] = 0;
    crop_rect[1] = 0;
    crop_rect[2] = 0;
    crop_rect[3] = 0;

    type = 0;

    mag_filter = GL_LINEAR;
    min_filter = GL_NEAREST_MIPMAP_LINEAR;
    wrap_r = GL_REPEAT;
    wrap_s = GL_REPEAT;
    wrap_t = GL_REPEAT;
    swizzle_r = GL_RED;
    swizzle_g = GL_GREEN;
    swizzle_b = GL_BLUE;
    swizzle_a = GL_ALPHA;
    min_lod = -1000;
    max_lod = 1000;
    max_level = 1000;
    base_level = 0;

    compare_mode = -1;
    compare_func = -1;
}

void d_glPixelStorei(void *context, GLenum pname, GLint param) {
    CHECK_VALID_ENUM(__LINE__, pname, 10, GL_PACK_ROW_LENGTH,
                     GL_PACK_SKIP_PIXELS, GL_PACK_SKIP_ROWS, GL_PACK_ALIGNMENT,
                     GL_UNPACK_ROW_LENGTH, GL_UNPACK_IMAGE_HEIGHT,
                     GL_UNPACK_SKIP_PIXELS, GL_UNPACK_SKIP_ROWS,
                     GL_UNPACK_SKIP_IMAGES, GL_UNPACK_ALIGNMENT)
    ALOGD("glPixelStorei pname %x param %d", pname, param);
    if (pname == GL_UNPACK_ALIGNMENT || pname == GL_PACK_ALIGNMENT) {
        if (param != 1 && param != 2 && param != 4 && param != 8) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
    } else {
        CHECK_NEGATIVE(__LINE__, param, GL_INVALID_VALUE)
    }

    PixelStoreStatus *pixel_store =
        &(((GLThreadContext *)context)->pixel_store_status);
    int skip_flag = 0;
    switch (pname) {
        case GL_UNPACK_ALIGNMENT:
            pixel_store->unpack_alignment = param;
            break;
        case GL_PACK_ALIGNMENT:
            pixel_store->pack_alignment = param;
            break;
        case GL_UNPACK_ROW_LENGTH:
            pixel_store->unpack_row_length = param;
            break;
        case GL_UNPACK_IMAGE_HEIGHT:
            pixel_store->unpack_image_height = param;
            break;
        case GL_UNPACK_SKIP_PIXELS:
            skip_flag = 1;
            pixel_store->unpack_skip_pixels = param;
            break;
        case GL_UNPACK_SKIP_ROWS:
            skip_flag = 1;
            pixel_store->unpack_skip_rows = param;
            break;
        case GL_UNPACK_SKIP_IMAGES:
            skip_flag = 1;
            pixel_store->unpack_skip_images = param;
            break;
        case GL_PACK_ROW_LENGTH:
            pixel_store->pack_row_length = param;
            break;
        case GL_PACK_SKIP_PIXELS:
            skip_flag = 1;
            pixel_store->pack_skip_pixels = param;
            break;
        case GL_PACK_SKIP_ROWS:
            skip_flag = 1;
            pixel_store->pack_skip_rows = param;
            break;
        default:
            ALOGE("glPixelStorei error pname %x", pname);
            return;
    }
    if (!skip_flag) {
        r_glPixelStorei_origin(context, pname, param);
    }

    return;
}

uint64_t gl_pixel_data_size(void *context, GLsizei width, GLsizei height,
                            GLenum format, GLenum type, int pack,
                            int *start_loc, int *len) {
    return gl_pixel_data_3d_size(context, width, height, 1, format, type, pack,
                                 start_loc, len);
}

uint64_t gl_pixel_data_3d_size(void *context, GLsizei width, GLsizei height,
                               GLsizei depth, GLenum format, GLenum type,
                               int pack, int *start_loc, int *len) {
    
    
    
    
    
    
    
    //
    
    PixelStoreStatus *status =
        &(((GLThreadContext *)context)->pixel_store_status);
    int align = 4;
    int row_length = 0;
    int skip_rows = 0;
    int skip_pixels = 0;
    int skip_images = 0;
    int image_height = 0;

    GLsizei real_width;
    GLsizei real_height;

    
    if (pack) {
        align = status->pack_alignment;
        row_length = status->pack_row_length;
        skip_rows = status->pack_skip_rows;
        skip_pixels = status->pack_skip_pixels;
    } else {
        align = status->unpack_alignment;
        row_length = status->unpack_row_length;
        skip_rows = status->unpack_skip_rows;
        skip_pixels = status->unpack_skip_pixels;

        skip_images = status->unpack_skip_images;
        image_height = status->unpack_image_height;
    }

    
    real_width = row_length == 0 ? width : row_length;
    real_height = image_height == 0 ? height : image_height;

    
    GLsizei pixel_size = pixel_size_calc(format, type);
    
    GLsizei width_size = pixel_size * real_width;
    
    width_size = (width_size + align - 1) & (~(align - 1));

    
    *start_loc = (skip_images * real_height + skip_rows) * width_size +
                 skip_pixels * pixel_size;

    
    
    *len = real_height * width_size * depth;

    return *len;
}

static void type_to_bits(GLenum type, int *red, int *green, int *blue,
                         int *alpha) {
    *red = 0;
    *green = 0;
    *blue = 0;
    *alpha = 0;
    switch (type) {
        case GL_UNSIGNED_BYTE:
            *red = 8;
            *green = 8;
            *blue = 8;
            break;
        case GL_BYTE:
            *red = 8;
            *green = 8;
            *blue = 8;
            break;
        case GL_UNSIGNED_SHORT:
            *red = 16;
            *green = 16;
            *blue = 16;
            break;
        case GL_SHORT:
            *red = 16;
            *green = 16;
            *blue = 16;
            break;
        case GL_UNSIGNED_INT:
            *red = 32;
            *green = 32;
            *blue = 32;
            break;
        case GL_INT:
            *red = 32;
            *green = 32;
            *blue = 32;
            break;
        case GL_HALF_FLOAT:
            *red = 16;
            *green = 16;
            *blue = 16;
            break;
        case GL_FLOAT:
            *red = 32;
            *green = 32;
            *blue = 32;
            break;
        case GL_UNSIGNED_SHORT_5_6_5:
            *red = 5;
            *green = 6;
            *blue = 5;
            break;
        case GL_UNSIGNED_SHORT_4_4_4_4:
            *red = 4;
            *green = 4;
            *blue = 4;
            *alpha = 4;
            break;
        case GL_UNSIGNED_SHORT_5_5_5_1:
            *red = 5;
            *green = 5;
            *blue = 5;
            *alpha = 1;
            break;
        case GL_UNSIGNED_INT_2_10_10_10_REV:
            *red = 10;
            *green = 10;
            *blue = 10;
            *alpha = 2;
            break;
        case GL_UNSIGNED_INT_10F_11F_11F_REV:
            *red = 11;
            *green = 11;
            *blue = 10;
            break;
        case GL_UNSIGNED_INT_5_9_9_9_REV:
            *red = 9;
            *green = 9;
            *blue = 9;
            *alpha = 5;
            break;
        default:
            ALOGE("type_to_bits error unknown type %x", type);
            break;
    }
}

void internal_format_to_format_bits(GLint internalformat, GLint format,
                                    GLint type, FormatBits *format_bits) {
    int red_bit = 0;
    int green_bit = 0;
    int blue_bit = 0;
    int alpha_bit = 0;
    int depth_bit = 0;
    int stencil_bit = 0;

    if (internalformat == GL_DEPTH_COMPONENT16) {
        depth_bit = 16;
    } else if (internalformat == GL_DEPTH_COMPONENT24) {
        depth_bit = 24;
    } else if (internalformat == GL_DEPTH_COMPONENT32F) {
        depth_bit = 32;
    } else if (internalformat == GL_DEPTH24_STENCIL8) {
        depth_bit = 24;
        stencil_bit = 8;
    } else if (internalformat == GL_DEPTH32F_STENCIL8) {
        depth_bit = 32;
        stencil_bit = 8;
    } else {
        switch (internalformat) {
            case GL_RGB: {
                if (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT_5_6_5)
                    type_to_bits(type, &red_bit, &green_bit, &blue_bit,
                                 &alpha_bit);
                break;
            }
            case GL_RGBA: {
                if (type == GL_UNSIGNED_BYTE ||
                    type == GL_UNSIGNED_SHORT_4_4_4_4 ||
                    type == GL_UNSIGNED_SHORT_5_5_5_1)
                    type_to_bits(type, &red_bit, &green_bit, &blue_bit,
                                 &alpha_bit);
                break;
            }
            case GL_LUMINANCE_ALPHA: {
                if (type == GL_UNSIGNED_BYTE)
                    type_to_bits(type, &red_bit, &green_bit, &blue_bit,
                                 &alpha_bit);
                break;
            }
            case GL_LUMINANCE: {
                if (type == GL_UNSIGNED_BYTE)
                    type_to_bits(type, &red_bit, &green_bit, &blue_bit,
                                 &alpha_bit);
                break;
            }
            case GL_ALPHA: {
                if (type == GL_UNSIGNED_BYTE)
                    type_to_bits(type, &red_bit, &green_bit, &blue_bit,
                                 &alpha_bit);
                break;
            }
            case GL_R8:
                red_bit = 8;
                break;
            case GL_R8_SNORM:
                red_bit = 8;
                break;
            case GL_R16F:
                red_bit = 16;
                break;
            case GL_R32F:
                red_bit = 32;
                break;
            case GL_R8UI:
            case GL_R8I:
                red_bit = 8;
                break;
            case GL_R16UI:
            case GL_R16I:
                red_bit = 16;
                break;
            case GL_R32UI:
            case GL_R32I:
                red_bit = 32;
                break;

            case GL_RG8:
                red_bit = 8;
                green_bit = 8;
                break;
            case GL_RG8_SNORM:
                red_bit = 8;
                green_bit = 8;
                break;
            case GL_RG16F:
                red_bit = 16;
                green_bit = 16;
                break;
            case GL_RG32F:
                red_bit = 32;
                green_bit = 32;
                break;
            case GL_RG8UI:
            case GL_RG8I:
                red_bit = 8;
                green_bit = 8;
                break;
            case GL_RG16UI:
            case GL_RG16I:
                red_bit = 16;
                green_bit = 16;
                break;
            case GL_RG32UI:
            case GL_RG32I:
                red_bit = 32;
                green_bit = 32;
                break;

            case GL_RGB8:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                break;
            case GL_SRGB8:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                break;
            case GL_RGB565:
                red_bit = 5;
                green_bit = 6;
                blue_bit = 5;
                break;
            case GL_RGB8_SNORM:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                break;
            case GL_R11F_G11F_B10F:
                red_bit = 11;
                green_bit = 11;
                blue_bit = 10;
                break;
            case GL_RGB9_E5:
                red_bit = 9;
                green_bit = 9;
                blue_bit = 9;
                break;
            case GL_RGB16F:
                red_bit = 16;
                green_bit = 16;
                blue_bit = 16;
                break;
            case GL_RGB32F:
                red_bit = 32;
                green_bit = 32;
                blue_bit = 32;
                break;
            case GL_RGB8UI:
            case GL_RGB8I:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                break;
            case GL_RGB16UI:
            case GL_RGB16I:
                red_bit = 16;
                green_bit = 16;
                blue_bit = 16;
                break;
            case GL_RGB32UI:
            case GL_RGB32I:
                red_bit = 32;
                green_bit = 32;
                blue_bit = 32;
                break;

            case GL_RGBA8:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                alpha_bit = 8;
                break;
            case GL_SRGB8_ALPHA8:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                alpha_bit = 8;
                break;
            case GL_RGBA8_SNORM:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                alpha_bit = 8;
                break;
            case GL_RGB5_A1:
                red_bit = 5;
                green_bit = 5;
                blue_bit = 5;
                alpha_bit = 1;
                break;
            case GL_RGBA4:
                red_bit = 4;
                green_bit = 4;
                blue_bit = 4;
                alpha_bit = 4;
                break;
            case GL_RGB10_A2:
                red_bit = 10;
                green_bit = 10;
                blue_bit = 10;
                alpha_bit = 2;
                break;
            case GL_RGBA16F:
                red_bit = 16;
                green_bit = 16;
                blue_bit = 16;
                alpha_bit = 16;
                break;
            case GL_RGBA32F:
                red_bit = 32;
                green_bit = 32;
                blue_bit = 32;
                alpha_bit = 32;
                break;
            case GL_RGBA8UI:
            case GL_RGBA8I:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                alpha_bit = 8;
                break;
            case GL_RGB10_A2UI:
                red_bit = 10;
                green_bit = 10;
                blue_bit = 10;
                alpha_bit = 2;
                break;
            case GL_RGBA16UI:
            case GL_RGBA16I:
                red_bit = 16;
                green_bit = 16;
                blue_bit = 16;
                alpha_bit = 16;
                break;
            case GL_RGBA32UI:
            case GL_RGBA32I:
                red_bit = 32;
                green_bit = 32;
                blue_bit = 32;
                alpha_bit = 32;
                break;

            case GL_COMPRESSED_R11_EAC:
                red_bit = 11;
                break;
            case GL_COMPRESSED_SIGNED_R11_EAC:
                red_bit = 11;
                break;
            case GL_COMPRESSED_RG11_EAC:
                red_bit = 11;
                green_bit = 11;
                break;
            case GL_COMPRESSED_SIGNED_RG11_EAC:
                red_bit = 11;
                green_bit = 11;
                break;
            case GL_COMPRESSED_RGB8_ETC2:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                break;
            case GL_COMPRESSED_SRGB8_ETC2:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                break;
            case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                alpha_bit = 1;
                break;
            case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                alpha_bit = 1;
                break;
            case GL_COMPRESSED_RGBA8_ETC2_EAC:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                alpha_bit = 8;
                break;
            case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
                red_bit = 8;
                green_bit = 8;
                blue_bit = 8;
                alpha_bit = 8;
                break;
            case GL_COMPRESSED_RGBA_ASTC_4x4:
            case GL_COMPRESSED_RGBA_ASTC_5x4:
            case GL_COMPRESSED_RGBA_ASTC_5x5:
            case GL_COMPRESSED_RGBA_ASTC_6x5:
            case GL_COMPRESSED_RGBA_ASTC_6x6:
            case GL_COMPRESSED_RGBA_ASTC_8x5:
            case GL_COMPRESSED_RGBA_ASTC_8x6:
            case GL_COMPRESSED_RGBA_ASTC_8x8:
            case GL_COMPRESSED_RGBA_ASTC_10x5:
            case GL_COMPRESSED_RGBA_ASTC_10x6:
            case GL_COMPRESSED_RGBA_ASTC_10x8:
            case GL_COMPRESSED_RGBA_ASTC_10x10:
            case GL_COMPRESSED_RGBA_ASTC_12x10:
            case GL_COMPRESSED_RGBA_ASTC_12x12:
                red_bit = 6;
                green_bit = 6;
                blue_bit = 6;
                alpha_bit = 8;
                break;

            default:
                ALOGE(
                    "internal_format_to_color_bits error unknown "
                    "internalformat %x",
                    internalformat);
                break;
        }
    }

    format_bits->red_bits = red_bit;
    format_bits->green_bits = green_bit;
    format_bits->blue_bits = blue_bit;
    format_bits->alpha_bits = alpha_bit;
    format_bits->depth_bits = depth_bit;
    format_bits->stencil_bits = stencil_bit;
}

/**
 * @param context
 * @param target
 * @param level
 * @param internalformat
 * @param width
 * @param height
 * @param border
 * @param format
 * @param type
 * @param pixels is data pointer or offset depending on whether there is a bound
 * buffer
 */
void d_glTexImage2D(void *context, GLenum target, GLint level,
                    GLint internalformat, GLsizei width, GLsizei height,
                    GLint border, GLenum format, GLenum type,
                    const void *pixels) {
    CHECK_VALID_ENUM(
        __LINE__, target, 8, GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_2D,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
    if (target != GL_TEXTURE_2D && width != height) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    CHECK_VALID_ENUM(
        __LINE__, type, 16, GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT,
        GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT,
        GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4,
        GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_INT_2_10_10_10_REV,
        GL_UNSIGNED_INT_10F_11F_11F_REV, GL_UNSIGNED_INT_5_9_9_9_REV,
        GL_UNSIGNED_INT_24_8, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
    CHECK_NEGATIVE(__LINE__, width < 0 || level < 0 || height, GL_INVALID_VALUE)
    GLint max;
    d_glGetIntegerv(context, GL_MAX_TEXTURE_SIZE, &max);
    if (width > max || height > max) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (level > log2(max)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (border != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    CHECK_VALID_ENUM(
        __LINE__, internalformat, 61, GL_RGB, GL_RGBA, GL_LUMINANCE_ALPHA,
        GL_LUMINANCE, GL_ALPHA, GL_RED, GL_R8, GL_R8_SNORM, GL_R16F, GL_R32F,
        GL_R8UI, GL_R8I, GL_R16UI, GL_R16I, GL_R32UI, GL_R32I, GL_RG8,
        GL_RG8_SNORM, GL_RG16F, GL_RG32F, GL_RG8UI, GL_RG8I, GL_RG16UI,
        GL_RG16I, GL_RG32UI, GL_RG32I, GL_RGB8, GL_SRGB8, GL_RGB565,
        GL_RGB8_SNORM, GL_R11F_G11F_B10F, GL_RGB9_E5, GL_RGB16F, GL_RGB32F,
        GL_RGB8UI, GL_RGB8I, GL_RGB16UI, GL_RGB16I, GL_RGB32UI, GL_RGB32I,
        GL_RGBA8, GL_SRGB8_ALPHA8, GL_RGBA8_SNORM, GL_RGB5_A1, GL_RGBA4,
        GL_RGB10_A2, GL_RGBA16F, GL_RGBA32F, GL_RGBA8UI, GL_RGBA8I,
        GL_RGB10_A2UI, GL_RGBA16UI, GL_RGBA16I, GL_RGBA32I, GL_RGBA32UI,
        GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT32F, GL_DEPTH24_STENCIL8, GL_DEPTH32F_STENCIL8)
    switch (internalformat) {
        case GL_RGB:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 2, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_5_6_5)
            break;
        case GL_RGBA:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 3, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_4_4_4_4,
                                 GL_UNSIGNED_SHORT_5_5_5_1)
            break;
        case GL_LUMINANCE_ALPHA:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_LUMINANCE_ALPHA, 1,
                                 GL_UNSIGNED_BYTE)
            break;
        case GL_LUMINANCE:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_LUMINANCE, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_ALPHA:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_ALPHA, 1, GL_UNSIGNED_BYTE)
            break;

        case GL_RED:
        case GL_R8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_R8_SNORM:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED, 1, GL_BYTE)
            break;
        case GL_R16F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED, 2, GL_HALF_FLOAT, GL_FLOAT)
            break;
        case GL_R32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED, 1, GL_FLOAT)
            break;
        case GL_R8UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_R8I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_BYTE)
            break;
        case GL_R16UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_UNSIGNED_SHORT)
            break;
        case GL_R16I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_SHORT)
            break;
        case GL_R32UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_UNSIGNED_INT)
            break;
        case GL_R32I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_INT)
            break;
        case GL_RG8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RG8_SNORM:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG, 1, GL_BYTE)
            break;
        case GL_RG16F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG, 2, GL_HALF_FLOAT, GL_FLOAT)
            break;
        case GL_RG32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG, 1, GL_FLOAT)
            break;
        case GL_RG8UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RG8I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_BYTE)
            break;
        case GL_RG16UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_UNSIGNED_SHORT)
            break;
        case GL_RG16I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_SHORT)
            break;
        case GL_RG32UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_UNSIGNED_INT)
            break;
        case GL_RG32I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_INT)
            break;
        case GL_RGB8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_SRGB8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RGB565:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 2, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_5_6_5)
            break;
        case GL_RGB8_SNORM:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 1, GL_BYTE)
            break;
        case GL_R11F_G11F_B10F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 3,
                                 GL_UNSIGNED_INT_10F_11F_11F_REV, GL_HALF_FLOAT,
                                 GL_FLOAT)
            break;
        case GL_RGB9_E5:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 3,
                                 GL_UNSIGNED_INT_5_9_9_9_REV, GL_HALF_FLOAT,
                                 GL_FLOAT)
            break;
        case GL_RGB16F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 2, GL_HALF_FLOAT, GL_FLOAT)
            break;
        case GL_RGB32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 1, GL_FLOAT)
            break;
        case GL_RGB8UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RGB8I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_BYTE)
            break;
        case GL_RGB16UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_UNSIGNED_SHORT)
            break;
        case GL_RGB16I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_SHORT)
            break;
        case GL_RGB32UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_UNSIGNED_INT)
            break;
        case GL_RGB32I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_INT)
            break;
        case GL_RGBA8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_SRGB8_ALPHA8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RGBA8_SNORM:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1, GL_BYTE)
            break;
        case GL_RGB5_A1:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 3, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_5_5_5_1,
                                 GL_UNSIGNED_INT_2_10_10_10_REV)
            break;
        case GL_RGBA4:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 2, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_4_4_4_4)
            break;
        case GL_RGB10_A2:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1,
                                 GL_UNSIGNED_INT_2_10_10_10_REV)
            break;
        case GL_RGBA16F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 2, GL_HALF_FLOAT, GL_FLOAT)
            break;
        case GL_RGBA32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1, GL_FLOAT)
            break;
        case GL_RGBA8UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RGBA8I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_BYTE)
            break;
        case GL_RGB10_A2UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1,
                                 GL_UNSIGNED_INT_2_10_10_10_REV)
            break;
        case GL_RGBA16UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1,
                                 GL_UNSIGNED_SHORT)
            break;
        case GL_RGBA16I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_SHORT)
            break;
        case GL_RGBA32I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_INT)
            break;
        case GL_RGBA32UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_UNSIGNED_INT)
            break;

        case GL_DEPTH_COMPONENT:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_DEPTH_COMPONENT, 3,
                                 GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT,
                                 GL_UNSIGNED_INT)
            break;
        case GL_DEPTH_COMPONENT16:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_DEPTH_COMPONENT, 2,
                                 GL_UNSIGNED_SHORT, GL_UNSIGNED_INT)
            break;
        case GL_DEPTH_COMPONENT24:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_DEPTH_COMPONENT, 1,
                                 GL_UNSIGNED_INT)
            break;
        case GL_DEPTH_COMPONENT32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_DEPTH_COMPONENT, 1, GL_FLOAT)
            break;
        case GL_DEPTH24_STENCIL8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_DEPTH_STENCIL, 1,
                                 GL_UNSIGNED_INT_24_8)
            break;
        case GL_DEPTH32F_STENCIL8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_DEPTH_STENCIL, 1,
                                 GL_FLOAT_32_UNSIGNED_INT_24_8_REV)
            break;
    }

    int start;
    int buf_len;

    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    int index = 0;
    switch (target) {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 0;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 1;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 2;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 3;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 4;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 5;
            break;
        case GL_TEXTURE_2D:
        default:
            info->type = target;
            index = 0;
            break;
    }

    info->height[index] = height;
    info->width[index] = width;
    info->depth[index] = 0;
    info->format[index] = internalformat;
    info->level[index] = level;
    info->is_compressed[index] = GL_FALSE;

    internal_format_to_format_bits(internalformat, format, type,
                                   &info->format_bits[index]);

    gl_pixel_data_size(context, width, height, format, type, 0, &start,
                       &buf_len);

    if (get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER) == 0) {
        r_glPixelStorei_origin(context, GL_UNPACK_SKIP_PIXELS, 0);
        r_glTexImage2D_without_bound(context, target, level, internalformat,
                                     width, height, border, format, type,
                                     buf_len, ((char *)pixels + start));
    } else {
        r_glPixelStorei_origin(context, GL_UNPACK_SKIP_PIXELS,
                               start / pixel_size_calc(format, type));
        r_glTexImage2D_with_bound(context, target, level, internalformat, width,
                                  height, border, format, type,
                                  (GLintptr)pixels);
    }
}

void d_glTexStorage2D_special(void *context, GLenum target, GLsizei levels,
                              GLenum internalformat, GLsizei width,
                              GLsizei height) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    int index = 0;
    switch (target) {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 0;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 1;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 2;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 3;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 4;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 5;
            break;
        case GL_TEXTURE_2D:
        default:
            info->type = target;
            index = 0;
            break;
    }

    info->height[index] = height;
    info->width[index] = width;
    info->depth[index] = 0;
    info->format[index] = internalformat;
    info->level[index] = levels;
    info->is_compressed[index] = GL_FALSE;

    info->immutable[index] = GL_TRUE;

    if (internalformat == GL_ETC1_RGB8_OES) {
        internalformat = GL_COMPRESSED_RGB8_ETC2;
    }

    internal_format_to_format_bits(internalformat, 0, 0,
                                   &info->format_bits[index]);

    r_glTexStorage2D(context, target, levels, internalformat, width, height);
}

void d_glTexSubImage2D(void *context, GLenum target, GLint level, GLint xoffset,
                       GLint yoffset, GLsizei width, GLsizei height,
                       GLenum format, GLenum type, const void *pixels) {
    CHECK_VALID_ENUM(
        __LINE__, target, 8, GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_2D,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
    CHECK_VALID_ENUM(__LINE__, format, 13, GL_RED, GL_RED_INTEGER, GL_RG,
                     GL_RG_INTEGER, GL_RGB, GL_RGB_INTEGER, GL_RGBA,
                     GL_RGBA_INTEGER, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL,
                     GL_LUMINANCE_ALPHA, GL_LUMINANCE, GL_ALPHA)
    CHECK_VALID_ENUM(
        __LINE__, type, 16, GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT,
        GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT,
        GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4,
        GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_INT_2_10_10_10_REV,
        GL_UNSIGNED_INT_10F_11F_11F_REV, GL_UNSIGNED_INT_5_9_9_9_REV,
        GL_UNSIGNED_INT_24_8, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
    CHECK_NEGATIVE(__LINE__, level < 0 || width < 0 || height, GL_INVALID_VALUE)
    GLint max;
    d_glGetIntegerv(context, GL_MAX_TEXTURE_SIZE, &max);
    if (level > log2(max)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    // /*TODO GL_INVALID_OPERATION is generated if the texture array has not
    // been defined by a previous glTexImage2D or glTexStorage2D operation.*/
    // /*TODO GL_INVALID_OPERATION is generated if the combination of
    // internalFormat of the previously specified texture array, format and type
    // is not valid. See glTexImage2D.*/
    // /*TODO GL_INVALID_VALUE is generated if xoffset < 0 , xoffset + width > w
    // , yoffset < 0 , or yoffset + height > h , where w is the
    // GL_TEXTURE_WIDTH, and h is the GL_TEXTURE_HEIGHT of the texture image
    // being modified.*/ GLint pix_buff; d_glGetIntegerv(context,
    // GL_PIXEL_UNPACK_BUFFER_BINDING, &pix_buff); if (pix_buff != 0)
    // {
    //     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
    //     name is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer
    //     object's data store is currently mapped.*/
    //     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
    //     name is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would
    //     be unpacked from the buffer object such that the memory reads
    //     required would exceed the data store size.*/
    //     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
    //     name is bound to the GL_PIXEL_UNPACK_BUFFER target and data is not
    //     evenly divisible into the number of bytes needed to store in memory a
    //     datum indicated by type.*/
    // }
    // assert(~(width == 30 && height==32));
    int start;
    int buf_len;
    gl_pixel_data_size(context, width, height, format, type, 0, &start,
                       &buf_len);

    ALOGD(
        "glTexSubImage2D target %x level %d xoffset %d yoffset %d width %d "
        "height %d format %x type %x start %d buf_len %d pixel %llx",
        target, level, xoffset, yoffset, width, height, format, type, start,
        buf_len, (unsigned long long)pixels);

    GLThreadContext *opengl_context = (GLThreadContext *)context;
    BindingStatus *binding_status = &(opengl_context->context_binding_status);

    if (target == GL_TEXTURE_2D) {
        if (binding_status->gbuffer_2D_binding != NULL) {
            EGLImageInfo *image_info = binding_status->gbuffer_2D_binding;
            global_gbuffer_types[image_info->gbuffer_id] = GBUFFER_TYPE_BITMAP;
            image_info->gbuffer_type = GBUFFER_TYPE_BITMAP;
            image_info->need_data_back = 1;
            ALOGD("set gbuffer %llx bitmap",
                  (unsigned long long)image_info->gbuffer_id);
        }
    } else if (target == GL_TEXTURE_EXTERNAL_OES) {
        if (binding_status->gbuffer_external_binding != NULL) {
            EGLImageInfo *image_info = binding_status->gbuffer_external_binding;
            global_gbuffer_types[image_info->gbuffer_id] = GBUFFER_TYPE_BITMAP;
            image_info->gbuffer_type = GBUFFER_TYPE_BITMAP;
            image_info->need_data_back = 1;
            ALOGD("set gbuffer %llx bitmap",
                  (unsigned long long)image_info->gbuffer_id);
        }
    }

    if (get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER) == 0) {
        r_glPixelStorei_origin(context, GL_UNPACK_SKIP_PIXELS, 0);
        r_glTexSubImage2D_without_bound(context, target, level, xoffset,
                                        yoffset, width, height, format, type,
                                        buf_len, ((char *)pixels + start));
    } else {
        r_glPixelStorei_origin(context, GL_UNPACK_SKIP_PIXELS,
                               start / pixel_size_calc(format, type));
        // if(target == GL_TEXTURE_EXTERNAL_OES)
        // {
        //     target = GL_TEXTURE_2D;
        // }
        r_glTexSubImage2D_with_bound(context, target, level, xoffset, yoffset,
                                     width, height, format, type,
                                     (GLintptr)pixels);
    }
}

void d_glTexImage3D(void *context, GLenum target, GLint level,
                    GLint internalformat, GLsizei width, GLsizei height,
                    GLsizei depth, GLint border, GLenum format, GLenum type,
                    const void *pixels) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY,
                     GL_TEXTURE_CUBE_MAP_ARRAY)
    CHECK_VALID_ENUM(
        __LINE__, internalformat, 56, GL_RGB, GL_RGBA, GL_LUMINANCE_ALPHA,
        GL_BGRA_EXT, GL_LUMINANCE, GL_ALPHA, GL_RED, GL_R8, GL_R8_SNORM,
        GL_R16F, GL_R32F, GL_R8UI, GL_R8I, GL_R16UI, GL_R16I, GL_R32UI, GL_R32I,
        GL_RG8, GL_RG8_SNORM, GL_RG16F, GL_RG32F, GL_RG8UI, GL_RG8I, GL_RG16UI,
        GL_RG16I, GL_RG32UI, GL_RG32I, GL_RGB8, GL_SRGB8, GL_RGB565,
        GL_RGB8_SNORM, GL_R11F_G11F_B10F, GL_RGB9_E5, GL_RGB16F, GL_RGB32F,
        GL_RGB8UI, GL_RGB8I, GL_RGB16UI, GL_RGB16I, GL_RGB32UI, GL_RGB32I,
        GL_RGBA8, GL_SRGB8_ALPHA8, GL_RGBA8_SNORM, GL_RGB5_A1, GL_RGBA4,
        GL_RGB10_A2, GL_RGBA16F, GL_RGBA32F, GL_RGBA8UI, GL_RGBA8I,
        GL_RGB10_A2UI, GL_RGBA16UI, GL_RGBA16I, GL_RGBA32I, GL_RGBA32UI);
    CHECK_VALID_ENUM(__LINE__, format, 12, GL_RGB, GL_RGBA, GL_LUMINANCE_ALPHA,
                     GL_LUMINANCE, GL_ALPHA, GL_RED, GL_BGRA_EXT,
                     GL_RED_INTEGER, GL_RG, GL_RG_INTEGER, GL_RGB_INTEGER,
                     GL_RGBA_INTEGER)
    CHECK_NEGATIVE(__LINE__, width < 0 || level < 0 || height, GL_INVALID_VALUE)
    GLint max;
    d_glGetIntegerv_special(context, GL_MAX_3D_TEXTURE_SIZE, &max);
    if (unlikely(width > max || height > max || depth > max)) {
        set_gl_error(context, 0x0501);
        return;
    }
    if (unlikely(level > log2(max))) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (unlikely(border != 0 && border != 1)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (unlikely(target == GL_TEXTURE_CUBE_MAP_ARRAY) &&
        (width != height || depth % 6 != 0)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (unlikely(target == GL_TEXTURE_3D && (format == GL_DEPTH_COMPONENT ||
                                             format == GL_DEPTH_STENCIL))) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    switch (internalformat) {
        case GL_RGB:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 2, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_5_6_5)
            break;
        case GL_RGBA:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 3, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_4_4_4_4,
                                 GL_UNSIGNED_SHORT_5_5_5_1)
            break;
        case GL_LUMINANCE_ALPHA:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_LUMINANCE_ALPHA, 1,
                                 GL_UNSIGNED_BYTE)
            break;
        case GL_LUMINANCE:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_LUMINANCE, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_ALPHA:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_ALPHA, 1, GL_UNSIGNED_BYTE)
            break;

        case GL_RED:
        case GL_R8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_R8_SNORM:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED, 1, GL_BYTE)
            break;
        case GL_R16F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED, 2, GL_HALF_FLOAT, GL_FLOAT)
            break;
        case GL_R32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED, 1, GL_FLOAT)
            break;
        case GL_R8UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_R8I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_BYTE)
            break;
        case GL_R16UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_UNSIGNED_SHORT)
            break;
        case GL_R16I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_SHORT)
            break;
        case GL_R32UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_UNSIGNED_INT)
            break;
        case GL_R32I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RED_INTEGER, 1, GL_INT)
            break;
        case GL_RG8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RG8_SNORM:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG, 1, GL_BYTE)
            break;
        case GL_RG16F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG, 2, GL_HALF_FLOAT, GL_FLOAT)
            break;
        case GL_RG32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG, 1, GL_FLOAT)
            break;
        case GL_RG8UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RG8I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_BYTE)
            break;
        case GL_RG16UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_UNSIGNED_SHORT)
            break;
        case GL_RG16I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_SHORT)
            break;
        case GL_RG32UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_UNSIGNED_INT)
            break;
        case GL_RG32I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RG_INTEGER, 1, GL_INT)
            break;
        case GL_RGB8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_SRGB8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RGB565:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 2, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_5_6_5)
            break;
        case GL_RGB8_SNORM:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 1, GL_BYTE)
            break;
        case GL_R11F_G11F_B10F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 3,
                                 GL_UNSIGNED_INT_10F_11F_11F_REV, GL_HALF_FLOAT,
                                 GL_FLOAT)
            break;
        case GL_RGB9_E5:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 3,
                                 GL_UNSIGNED_INT_5_9_9_9_REV, GL_HALF_FLOAT,
                                 GL_FLOAT)
            break;
        case GL_RGB16F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 2, GL_HALF_FLOAT, GL_FLOAT)
            break;
        case GL_RGB32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB, 1, GL_FLOAT)
            break;
        case GL_RGB8UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RGB8I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_BYTE)
            break;
        case GL_RGB16UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_UNSIGNED_SHORT)
            break;
        case GL_RGB16I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_SHORT)
            break;
        case GL_RGB32UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_UNSIGNED_INT)
            break;
        case GL_RGB32I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGB_INTEGER, 1, GL_INT)
            break;
        case GL_RGBA8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_SRGB8_ALPHA8:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RGBA8_SNORM:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1, GL_BYTE)
            break;
        case GL_RGB5_A1:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 3, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_5_5_5_1,
                                 GL_UNSIGNED_INT_2_10_10_10_REV)
            break;
        case GL_RGBA4:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 2, GL_UNSIGNED_BYTE,
                                 GL_UNSIGNED_SHORT_4_4_4_4)
            break;
        case GL_RGB10_A2:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1,
                                 GL_UNSIGNED_INT_2_10_10_10_REV)
            break;
        case GL_RGBA16F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 2, GL_HALF_FLOAT, GL_FLOAT)
            break;
        case GL_RGBA32F:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA, 1, GL_FLOAT)
            break;
        case GL_RGBA8UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_UNSIGNED_BYTE)
            break;
        case GL_RGBA8I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_BYTE)
            break;
        case GL_RGB10_A2UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1,
                                 GL_UNSIGNED_INT_2_10_10_10_REV)
            break;
        case GL_RGBA16UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1,
                                 GL_UNSIGNED_SHORT)
            break;
        case GL_RGBA16I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_SHORT)
            break;
        case GL_RGBA32I:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_INT)
            break;
        case GL_RGBA32UI:
            CHECK_TEXTURE_FORMAT(__LINE__, GL_RGBA_INTEGER, 1, GL_UNSIGNED_INT)
            break;
    }
    GLint pix_buff;
    d_glGetIntegerv_special(context, GL_PIXEL_UNPACK_BUFFER_BINDING, &pix_buff);
    if (pix_buff != 0) {
        /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
         * name is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer
         * object's data store is currently mapped.*/
        /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
         * name is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would
         * be unpacked from the buffer object such that the memory reads
         * required would exceed the data store size.*/
        /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
         * name is bound to the GL_PIXEL_UNPACK_BUFFER target and data is not
         * evenly divisible into the number of bytes needed to store in memory a
         * datum indicated by type.*/
    }

    int start;
    int buf_len;

    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    info->height[0] = height;
    info->width[0] = width;
    info->depth[0] = depth;
    info->format[0] = internalformat;
    info->level[0] = level;
    info->is_compressed[0] = GL_FALSE;
    info->type = target;

    internal_format_to_format_bits(internalformat, format, type,
                                   &info->format_bits[0]);

    gl_pixel_data_3d_size(context, width, height, depth, format, type, 0,
                          &start, &buf_len);
    if (get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER) == 0) {
        r_glPixelStorei_origin(context, GL_UNPACK_SKIP_PIXELS, 0);
        r_glTexImage3D_without_bound(context, target, level, internalformat,
                                     width, height, depth, border, format, type,
                                     buf_len, ((char *)pixels + start));
    } else {
        r_glPixelStorei_origin(context, GL_UNPACK_SKIP_PIXELS,
                               start / pixel_size_calc(format, type));
        r_glTexImage3D_with_bound(context, target, level, internalformat, width,
                                  height, depth, border, format, type,
                                  (GLintptr)pixels);
    }
}

void d_glTexSubImage3D(void *context, GLenum target, GLint level, GLint xoffset,
                       GLint yoffset, GLint zoffset, GLsizei width,
                       GLsizei height, GLsizei depth, GLenum format,
                       GLenum type, const void *pixels) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY,
                     GL_TEXTURE_CUBE_MAP_ARRAY)
    CHECK_VALID_ENUM(
        __LINE__, type, 16, GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT,
        GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT,
        GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4,
        GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_INT_2_10_10_10_REV,
        GL_UNSIGNED_INT_10F_11F_11F_REV, GL_UNSIGNED_INT_5_9_9_9_REV,
        GL_UNSIGNED_INT_24_8, GL_FLOAT_32_UNSIGNED_INT_24_8_REV);
    CHECK_VALID_ENUM(__LINE__, format, 15, GL_RED, GL_RED_INTEGER, GL_RG,
                     GL_BGRA_EXT, GL_RG_INTEGER, GL_RGB, GL_RGB_INTEGER,
                     GL_RGBA8, GL_RGBA, GL_RGBA_INTEGER, GL_DEPTH_COMPONENT,
                     GL_DEPTH_STENCIL, GL_LUMINANCE_ALPHA, GL_LUMINANCE,
                     GL_ALPHA)
    CHECK_NEGATIVE(__LINE__, depth < 0 || width < 0 || level < 0 || height,
                   GL_INVALID_VALUE)
    GLint max;
    d_glGetIntegerv_special(context, GL_MAX_3D_TEXTURE_SIZE, &max);
    if (level > log2(max)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    // GLint pix_buff;
    // d_glGetIntegerv(context, GL_PIXEL_UNPACK_BUFFER_BINDING, &pix_buff);
    // if (pix_buff != 0)
    // {
    //     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
    //     name is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer
    //     object's data store is currently mapped.*/
    //     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
    //     name is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would
    //     be unpacked from the buffer object such that the memory reads
    //     required would exceed the data store size.*/
    //     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object
    //     name is bound to the GL_PIXEL_UNPACK_BUFFER target and data is not
    //     evenly divisible into the number of bytes needed to store in memory a
    //     datum indicated by type.*/
    // }

    int start;
    int buf_len;
    gl_pixel_data_3d_size(context, width, height, depth, format, type, 0,
                          &start, &buf_len);
    if (get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER) == 0) {
        ALOGD(
            "glTexSubImage3D_without_bound target %x level %d offset %d "
            "yoffset %d zoffset %d width %d height %d depth %d format %x type "
            "%x pixels %llx",
            target, level, xoffset, yoffset, zoffset, (int)width, (int)height,
            (int)depth, format, type, (unsigned long long)pixels);
        r_glPixelStorei_origin(context, GL_UNPACK_SKIP_PIXELS, 0);
        r_glTexSubImage3D_without_bound(
            context, target, level, xoffset, yoffset, zoffset, width, height,
            depth, format, type, buf_len, ((char *)pixels + start));
    } else {
        ALOGD(
            "glTexSubImage3D_with_bound target %x level %d offset %d yoffset "
            "%d zoffset %d width %d height %d depth %d format %x type %x "
            "pixels %llx",
            target, level, xoffset, yoffset, zoffset, (int)width, (int)height,
            (int)depth, format, type, (unsigned long long)pixels);
        r_glPixelStorei_origin(context, GL_UNPACK_SKIP_PIXELS,
                               start / pixel_size_calc(format, type));
        r_glTexSubImage3D_with_bound(context, target, level, xoffset, yoffset,
                                     zoffset, width, height, depth, format,
                                     type, (GLintptr)pixels);
    }
}

void d_glCompressedTexImage3D(void *context, GLenum target, GLint level,
                              GLenum internalformat, GLsizei width,
                              GLsizei height, GLsizei depth, GLint border,
                              GLsizei imageSize, const void *data) {
    CHECK_VALID_ENUM(
        __LINE__, internalformat, 39, GL_ETC1_RGB8_OES, GL_COMPRESSED_R11_EAC,
        GL_COMPRESSED_SIGNED_R11_EAC, GL_COMPRESSED_RG11_EAC,
        GL_COMPRESSED_SIGNED_RG11_EAC, GL_COMPRESSED_RGB8_ETC2,
        GL_COMPRESSED_SRGB8_ETC2, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
        GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
        GL_COMPRESSED_RGBA8_ETC2_EAC, GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,
        GL_COMPRESSED_RGBA_ASTC_4x4, GL_COMPRESSED_RGBA_ASTC_5x4,
        GL_COMPRESSED_RGBA_ASTC_5x5, GL_COMPRESSED_RGBA_ASTC_6x5,
        GL_COMPRESSED_RGBA_ASTC_6x6, GL_COMPRESSED_RGBA_ASTC_8x5,
        GL_COMPRESSED_RGBA_ASTC_8x6, GL_COMPRESSED_RGBA_ASTC_8x8,
        GL_COMPRESSED_RGBA_ASTC_10x5, GL_COMPRESSED_RGBA_ASTC_10x6,
        GL_COMPRESSED_RGBA_ASTC_10x8, GL_COMPRESSED_RGBA_ASTC_10x10,
        GL_COMPRESSED_RGBA_ASTC_12x10, GL_COMPRESSED_RGBA_ASTC_12x12,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12)
    /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with the
     * format, dimensions, and contents of the specified compressed image
     * data.*/
    GLfloat w = width, h = height;
    switch (internalformat) {
        case GL_COMPRESSED_R11_EAC:
        case GL_COMPRESSED_SIGNED_R11_EAC:
        case GL_COMPRESSED_RGB8_ETC2:
        case GL_COMPRESSED_SRGB8_ETC2:
        case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
        case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
            if (unlikely(imageSize > ceil(w / 4) * ceil(h / 4) * 8)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RG11_EAC:
        case GL_COMPRESSED_SIGNED_RG11_EAC:
        case GL_COMPRESSED_RGBA8_ETC2_EAC:
        case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
        case GL_COMPRESSED_RGBA_ASTC_4x4:
            if (unlikely(imageSize > ceil(w / 4) * ceil(h / 4) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_5x4:
            if (unlikely(imageSize > ceil(w / 5) * ceil(h / 4) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_5x5:
            if (unlikely(imageSize > ceil(w / 5) * ceil(h / 5) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_6x5:
            if (unlikely(imageSize > ceil(w / 6) * ceil(h / 5) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_6x6:
            if (unlikely(imageSize > ceil(w / 6) * ceil(h / 6) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x5:
            if (unlikely(imageSize > ceil(w / 8) * ceil(h / 5) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x6:
            if (unlikely(imageSize > ceil(w / 8) * ceil(h / 6) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x8:
            if (unlikely(imageSize > ceil(w / 8) * ceil(h / 8) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x5:
            if (unlikely(imageSize > ceil(w / 10) * ceil(h / 5) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x6:
            if (unlikely(imageSize > ceil(w / 10) * ceil(h / 6) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x8:
            if (unlikely(imageSize > ceil(w / 10) * ceil(h / 8) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x10:
            if (unlikely(imageSize > ceil(w / 10) * ceil(h / 10) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_12x10:
            if (unlikely(imageSize > ceil(w / 12) * ceil(h / 10) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_12x12:
            if (unlikely(imageSize > ceil(w / 12) * ceil(h / 12) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4:
            if (unlikely(imageSize > ceil(w / 4) * ceil(h / 4) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4:
            if (unlikely(imageSize > ceil(w / 5) * ceil(h / 4) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5:
            if (unlikely(imageSize > ceil(w / 5) * ceil(h / 5) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5:
            if (unlikely(imageSize > ceil(w / 6) * ceil(h / 5) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6:
            if (unlikely(imageSize > ceil(w / 6) * ceil(h / 6) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5:
            if (unlikely(imageSize > ceil(w / 8) * ceil(h / 5) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6:
            if (unlikely(imageSize > ceil(w / 8) * ceil(h / 6) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8:
            if (unlikely(imageSize > ceil(w / 8) * ceil(h / 8) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5:
            if (unlikely(imageSize > ceil(w / 10) * ceil(h / 5) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6:
            if (unlikely(imageSize > ceil(w / 10) * ceil(h / 6) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8:
            if (unlikely(imageSize > ceil(w / 10) * ceil(h / 8) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10:
            if (unlikely(imageSize > ceil(w / 10) * ceil(h / 10) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10:
            if (unlikely(imageSize > ceil(w / 12) * ceil(h / 10) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12:
            if (unlikely(imageSize > ceil(w / 12) * ceil(h / 12) * 16)) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
    }
    if (unlikely(border != 0)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (internalformat == GL_COMPRESSED_R11_EAC ||
        internalformat == GL_COMPRESSED_SIGNED_R11_EAC ||
        internalformat == GL_COMPRESSED_RG11_EAC ||
        internalformat == GL_COMPRESSED_SIGNED_RG11_EAC ||
        internalformat == GL_COMPRESSED_RGB8_ETC2 ||
        internalformat == GL_COMPRESSED_SRGB8_ETC2 ||
        internalformat == GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 ||
        internalformat == GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 ||
        internalformat == GL_COMPRESSED_RGBA8_ETC2_EAC ||
        internalformat == GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC) {
        if (unlikely(target == GL_TEXTURE_2D_ARRAY ||
                     target == GL_TEXTURE_CUBE_MAP_ARRAY)) {
            set_gl_error(context, GL_INVALID_OPERATION);
            return;
        }
    }
    /*TODO GL_INVALID_OPERATION is generated if parameter combinations are not
     * supported by the specific compressed internal format as specified in the
     * specific texture compression extension. The ETC2/EAC texture compression
     * algorithm supports only two-dimensional images. If internalformat is an
     * ETC2/EAC format, glCompressedTexImage3D will generate a
     * GL_INVALID_OPERATION error if target is not GL_TEXTURE_2D_ARRAY or
     * GL_TEXTURE_CUBE_MAP_ARRAY. If internalformat is an ASTC format,
     * glCompressedTexImage3D will generate an INVALID_OPERATION error if target
     * is not GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY unless
     * GL_OES_texture_compression_astc is supported.*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's data
     * store is currently mapped*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be unpacked
     * from the buffer object such that the memory reads required would exceed
     * the data store size.*/

    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    info->height[0] = height;
    info->width[0] = width;
    info->depth[0] = depth;
    info->format[0] = internalformat;
    info->level[0] = level;
    info->is_compressed[0] = GL_TRUE;
    info->type = target;

    if (internalformat == GL_ETC1_RGB8_OES) {
        internalformat = GL_COMPRESSED_RGB8_ETC2;
    }

    internal_format_to_format_bits(internalformat, 0, 0, &info->format_bits[0]);

    if (get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER) == 0) {
        r_glCompressedTexImage3D_without_bound(context, target, level,
                                               internalformat, width, height,
                                               depth, border, imageSize, data);
    } else {
        r_glCompressedTexImage3D_with_bound(
            context, target, level, internalformat, width, height, depth,
            border, imageSize, (GLintptr)data);
    }
}

void d_glCompressedTexSubImage3D(void *context, GLenum target, GLint level,
                                 GLint xoffset, GLint yoffset, GLint zoffset,
                                 GLsizei width, GLsizei height, GLsizei depth,
                                 GLenum format, GLsizei imageSize,
                                 const void *data) {
    CHECK_VALID_ENUM(
        __LINE__, format, 39, GL_ETC1_RGB8_OES, GL_COMPRESSED_R11_EAC,
        GL_COMPRESSED_SIGNED_R11_EAC, GL_COMPRESSED_RG11_EAC,
        GL_COMPRESSED_SIGNED_RG11_EAC, GL_COMPRESSED_RGB8_ETC2,
        GL_COMPRESSED_SRGB8_ETC2, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
        GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
        GL_COMPRESSED_RGBA8_ETC2_EAC, GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,
        GL_COMPRESSED_RGBA_ASTC_4x4, GL_COMPRESSED_RGBA_ASTC_5x4,
        GL_COMPRESSED_RGBA_ASTC_5x5, GL_COMPRESSED_RGBA_ASTC_6x5,
        GL_COMPRESSED_RGBA_ASTC_6x6, GL_COMPRESSED_RGBA_ASTC_8x5,
        GL_COMPRESSED_RGBA_ASTC_8x6, GL_COMPRESSED_RGBA_ASTC_8x8,
        GL_COMPRESSED_RGBA_ASTC_10x5, GL_COMPRESSED_RGBA_ASTC_10x6,
        GL_COMPRESSED_RGBA_ASTC_10x8, GL_COMPRESSED_RGBA_ASTC_10x10,
        GL_COMPRESSED_RGBA_ASTC_12x10, GL_COMPRESSED_RGBA_ASTC_12x12,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12)

    /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with the
     * format, dimensions, and contents of the specified compressed image
     * data.*/
    GLfloat w = width, h = height;
    switch (format) {
        case GL_COMPRESSED_R11_EAC:
        case GL_COMPRESSED_SIGNED_R11_EAC:
        case GL_COMPRESSED_RGB8_ETC2:
        case GL_COMPRESSED_SRGB8_ETC2:
        case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
        case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 8) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RG11_EAC:
        case GL_COMPRESSED_SIGNED_RG11_EAC:
        case GL_COMPRESSED_RGBA8_ETC2_EAC:
        case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
        case GL_COMPRESSED_RGBA_ASTC_4x4:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_5x4:
            if (imageSize > ceil(w / 5) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_5x5:
            if (imageSize > ceil(w / 5) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_6x5:
            if (imageSize > ceil(w / 6) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_6x6:
            if (imageSize > ceil(w / 6) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x5:
            if (imageSize > ceil(w / 8) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x6:
            if (imageSize > ceil(w / 8) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x8:
            if (imageSize > ceil(w / 8) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x5:
            if (imageSize > ceil(w / 10) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x6:
            if (imageSize > ceil(w / 10) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x8:
            if (imageSize > ceil(w / 10) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x10:
            if (imageSize > ceil(w / 10) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_12x10:
            if (imageSize > ceil(w / 12) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_12x12:
            if (imageSize > ceil(w / 12) * ceil(h / 12) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4:
            if (imageSize > ceil(w / 5) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5:
            if (imageSize > ceil(w / 5) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5:
            if (imageSize > ceil(w / 6) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6:
            if (imageSize > ceil(w / 6) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5:
            if (imageSize > ceil(w / 8) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6:
            if (imageSize > ceil(w / 8) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8:
            if (imageSize > ceil(w / 8) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5:
            if (imageSize > ceil(w / 10) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6:
            if (imageSize > ceil(w / 10) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8:
            if (imageSize > ceil(w / 10) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10:
            if (imageSize > ceil(w / 10) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10:
            if (imageSize > ceil(w / 12) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12:
            if (imageSize > ceil(w / 12) * ceil(h / 12) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
    }
    /*TODO GL_INVALID_OPERATION is generated if parameter combinations are not
     * supported by the specific compressed internal format as specified in the
     * specific texture compression extension. For ETC2/EAC images
     * GL_INVALID_OPERATION is generated if width is not a multiple of four, and
     * width + xoffset is not equal to the width of the texture level; if height
     * is not a multiple of four, and height + yoffset is not equal to the
     * height of the texture level; or if xoffset or yoffset is not a multiple
     * of four. The ETC2/EAC texture compression algorithm supports only
     * two-dimensional images. If format is an ETC2/EAC format,
     * glCompressedTexSubImage3D will generate an GL_INVALID_OPERATION error if
     * target is not GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY.*/
    /*TODO For ASTC images GL_INVALID_OPERATION is generated if width is not a
     * multiple of the block width, and width + xoffset is not equal to the
     * width of the texture level; if height is not a multiple of the block
     * height, and height + yoffset is not equal to the height of the texture
     * level; or if xoffset or yoffset is not a multiple of the corresponding
     * block dimension. If format is an ASTC format, glCompressedTexSubImage3D
     * will generate an GL_INVALID_OPERATION error if target is not
     * GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY, unless
     * GL_OES_texture_compression_astc is supported.*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's data
     * store is currently mapped.*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be unpacked
     * from the buffer object such that the memory reads required would exceed
     * the data store size.*/

    if (format == GL_ETC1_RGB8_OES) {
        format = GL_COMPRESSED_RGB8_ETC2;
    }

    if (get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER) == 0) {
        r_glCompressedTexSubImage3D_without_bound(
            context, target, level, xoffset, yoffset, zoffset, width, height,
            depth, format, imageSize, data);
    } else {
        r_glCompressedTexSubImage3D_with_bound(
            context, target, level, xoffset, yoffset, zoffset, width, height,
            depth, format, imageSize, (GLintptr)data);
    }
}

void d_glCompressedTexImage2D(void *context, GLenum target, GLint level,
                              GLenum internalformat, GLsizei width,
                              GLsizei height, GLint border, GLsizei imageSize,
                              const void *data) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, target, 7, GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
    CHECK_VALID_ENUM(
        __LINE__, internalformat, 39, GL_ETC1_RGB8_OES, GL_COMPRESSED_R11_EAC,
        GL_COMPRESSED_SIGNED_R11_EAC, GL_COMPRESSED_RG11_EAC,
        GL_COMPRESSED_SIGNED_RG11_EAC, GL_COMPRESSED_RGB8_ETC2,
        GL_COMPRESSED_SRGB8_ETC2, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
        GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
        GL_COMPRESSED_RGBA8_ETC2_EAC, GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,
        GL_COMPRESSED_RGBA_ASTC_4x4, GL_COMPRESSED_RGBA_ASTC_5x4,
        GL_COMPRESSED_RGBA_ASTC_5x5, GL_COMPRESSED_RGBA_ASTC_6x5,
        GL_COMPRESSED_RGBA_ASTC_6x6, GL_COMPRESSED_RGBA_ASTC_8x5,
        GL_COMPRESSED_RGBA_ASTC_8x6, GL_COMPRESSED_RGBA_ASTC_8x8,
        GL_COMPRESSED_RGBA_ASTC_10x5, GL_COMPRESSED_RGBA_ASTC_10x6,
        GL_COMPRESSED_RGBA_ASTC_10x8, GL_COMPRESSED_RGBA_ASTC_10x10,
        GL_COMPRESSED_RGBA_ASTC_12x10, GL_COMPRESSED_RGBA_ASTC_12x12,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12)

    /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with the
     * format, dimensions, and contents of the specified compressed image
     * data.*/
    GLfloat w = width, h = height;
    switch (internalformat) {
        case GL_COMPRESSED_R11_EAC:
        case GL_COMPRESSED_SIGNED_R11_EAC:
        case GL_COMPRESSED_RGB8_ETC2:
        case GL_COMPRESSED_SRGB8_ETC2:
        case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
        case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 8) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RG11_EAC:
        case GL_COMPRESSED_SIGNED_RG11_EAC:
        case GL_COMPRESSED_RGBA8_ETC2_EAC:
        case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
        case GL_COMPRESSED_RGBA_ASTC_4x4:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_5x4:
            if (imageSize > ceil(w / 5) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_5x5:
            if (imageSize > ceil(w / 5) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_6x5:
            if (imageSize > ceil(w / 6) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_6x6:
            if (imageSize > ceil(w / 6) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x5:
            if (imageSize > ceil(w / 8) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x6:
            if (imageSize > ceil(w / 8) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x8:
            if (imageSize > ceil(w / 8) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x5:
            if (imageSize > ceil(w / 10) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x6:
            if (imageSize > ceil(w / 10) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x8:
            if (imageSize > ceil(w / 10) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x10:
            if (imageSize > ceil(w / 10) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_12x10:
            if (imageSize > ceil(w / 12) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_12x12:
            if (imageSize > ceil(w / 12) * ceil(h / 12) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4:
            if (imageSize > ceil(w / 5) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5:
            if (imageSize > ceil(w / 5) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5:
            if (imageSize > ceil(w / 6) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6:
            if (imageSize > ceil(w / 6) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5:
            if (imageSize > ceil(w / 8) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6:
            if (imageSize > ceil(w / 8) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8:
            if (imageSize > ceil(w / 8) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5:
            if (imageSize > ceil(w / 10) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6:
            if (imageSize > ceil(w / 10) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8:
            if (imageSize > ceil(w / 10) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10:
            if (imageSize > ceil(w / 10) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10:
            if (imageSize > ceil(w / 12) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12:
            if (imageSize > ceil(w / 12) * ceil(h / 12) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
    }
    /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with the
     * format, dimensions, and contents of the specified compressed image
     * data.*/
    if (border != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    /*TODO GL_INVALID_OPERATION is generated if parameter combinations are not
     * supported by the specific compressed internal format as specified in the
     * specific texture compression extension*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's data
     * store is currently mapped*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be unpacked
     * from the buffer object such that the memory reads required would exceed
     * the data store size.*/

    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    int index = 0;
    switch (target) {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 0;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 1;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 2;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 3;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 4;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            info->type = GL_TEXTURE_CUBE_MAP;
            index = 5;
            break;
        case GL_TEXTURE_2D:
        default:
            info->type = target;
            index = 0;
            break;
    }

    info->height[index] = height;
    info->width[index] = width;
    info->depth[index] = 0;
    info->format[index] = internalformat;
    info->level[index] = level;
    info->is_compressed[index] = GL_TRUE;

    if (internalformat == GL_ETC1_RGB8_OES) {
        internalformat = GL_COMPRESSED_RGB8_ETC2;
    }

    internal_format_to_format_bits(internalformat, 0, 0, &info->format_bits[0]);

    if (get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER) == 0) {
        r_glCompressedTexImage2D_without_bound(context, target, level,
                                               internalformat, width, height,
                                               border, imageSize, data);
    } else {
        r_glCompressedTexImage2D_with_bound(context, target, level,
                                            internalformat, width, height,
                                            border, imageSize, (GLintptr)data);
    }
}

void d_glCompressedTexSubImage2D(void *context, GLenum target, GLint level,
                                 GLint xoffset, GLint yoffset, GLsizei width,
                                 GLsizei height, GLenum format,
                                 GLsizei imageSize, const void *data) {
    CHECK_VALID_ENUM(
        __LINE__, target, 7, GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
    CHECK_VALID_ENUM(
        __LINE__, format, 39, GL_ETC1_RGB8_OES, GL_COMPRESSED_R11_EAC,
        GL_COMPRESSED_SIGNED_R11_EAC, GL_COMPRESSED_RG11_EAC,
        GL_COMPRESSED_SIGNED_RG11_EAC, GL_COMPRESSED_RGB8_ETC2,
        GL_COMPRESSED_SRGB8_ETC2, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
        GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2,
        GL_COMPRESSED_RGBA8_ETC2_EAC, GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC,
        GL_COMPRESSED_RGBA_ASTC_4x4, GL_COMPRESSED_RGBA_ASTC_5x4,
        GL_COMPRESSED_RGBA_ASTC_5x5, GL_COMPRESSED_RGBA_ASTC_6x5,
        GL_COMPRESSED_RGBA_ASTC_6x6, GL_COMPRESSED_RGBA_ASTC_8x5,
        GL_COMPRESSED_RGBA_ASTC_8x6, GL_COMPRESSED_RGBA_ASTC_8x8,
        GL_COMPRESSED_RGBA_ASTC_10x5, GL_COMPRESSED_RGBA_ASTC_10x6,
        GL_COMPRESSED_RGBA_ASTC_10x8, GL_COMPRESSED_RGBA_ASTC_10x10,
        GL_COMPRESSED_RGBA_ASTC_12x10, GL_COMPRESSED_RGBA_ASTC_12x12,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10,
        GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12)

    /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with the
     * format, dimensions, and contents of the specified compressed image
     * data.*/
    GLfloat w = width, h = height;
    switch (format) {
        case GL_COMPRESSED_R11_EAC:
        case GL_COMPRESSED_SIGNED_R11_EAC:
        case GL_COMPRESSED_RGB8_ETC2:
        case GL_COMPRESSED_SRGB8_ETC2:
        case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:
        case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 8) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RG11_EAC:
        case GL_COMPRESSED_SIGNED_RG11_EAC:
        case GL_COMPRESSED_RGBA8_ETC2_EAC:
        case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
        case GL_COMPRESSED_RGBA_ASTC_4x4:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_5x4:
            if (imageSize > ceil(w / 5) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_5x5:
            if (imageSize > ceil(w / 5) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_6x5:
            if (imageSize > ceil(w / 6) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_6x6:
            if (imageSize > ceil(w / 6) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x5:
            if (imageSize > ceil(w / 8) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x6:
            if (imageSize > ceil(w / 8) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_8x8:
            if (imageSize > ceil(w / 8) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x5:
            if (imageSize > ceil(w / 10) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x6:
            if (imageSize > ceil(w / 10) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x8:
            if (imageSize > ceil(w / 10) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_10x10:
            if (imageSize > ceil(w / 10) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_12x10:
            if (imageSize > ceil(w / 12) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_RGBA_ASTC_12x12:
            if (imageSize > ceil(w / 12) * ceil(h / 12) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4:
            if (imageSize > ceil(w / 4) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4:
            if (imageSize > ceil(w / 5) * ceil(h / 4) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5:
            if (imageSize > ceil(w / 5) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5:
            if (imageSize > ceil(w / 6) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6:
            if (imageSize > ceil(w / 6) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5:
            if (imageSize > ceil(w / 8) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6:
            if (imageSize > ceil(w / 8) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8:
            if (imageSize > ceil(w / 8) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5:
            if (imageSize > ceil(w / 10) * ceil(h / 5) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6:
            if (imageSize > ceil(w / 10) * ceil(h / 6) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8:
            if (imageSize > ceil(w / 10) * ceil(h / 8) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10:
            if (imageSize > ceil(w / 10) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10:
            if (imageSize > ceil(w / 12) * ceil(h / 10) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12:
            if (imageSize > ceil(w / 12) * ceil(h / 12) * 16) {
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
            break;
    }
    /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with the
     * format, dimensions, and contents of the specified compressed image
     * data.*/
    /*TODO GL_INVALID_OPERATION is generated if parameter combinations are not
     * supported by the specific compressed internal format as specified in the
     * specific texture compression extension.*/
    /*TODO For ETC2/EAC images GL_INVALID_OPERATION is generated if width is not
     * a multiple of four, and width + xoffset is not equal to the width of the
     * texture level; if height is not a multiple of four, and height + yoffset
     * is not equal to the height of the texture level; or if xoffset or yoffset
     * is not a multiple of four.*/
    /*TODO For ASTC images GL_INVALID_OPERATION is generated if width is not a
     * multiple of the block width, and width + xoffset is not equal to the
     * width of the texture level; if height is not a multiple of the block
     * height, and height + yoffset is not equal to the height of the texture
     * level; or if xoffset or yoffset is not a multiple of the corresponding
     * block dimension.*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's data
     * store is currently mapped.*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be unpacked
     * from the buffer object such that the memory reads required would exceed
     * the data store size.*/

    if (format == GL_ETC1_RGB8_OES) {
        format = GL_COMPRESSED_RGB8_ETC2;
    }

    if (get_bound_buffer(context, GL_PIXEL_UNPACK_BUFFER) == 0) {
        r_glCompressedTexSubImage2D_without_bound(
            context, target, level, xoffset, yoffset, width, height, format,
            imageSize, data);
    } else {
        r_glCompressedTexSubImage2D_with_bound(context, target, level, xoffset,
                                               yoffset, width, height, format,
                                               imageSize, (GLintptr)data);
    }
}

void d_glReadPixels(void *context, GLint x, GLint y, GLsizei width,
                    GLsizei height, GLenum format, GLenum type, void *pixels) {
    
    // GLint imp_formats, imp_type;
    // d_glGetIntegerv(context, GL_IMPLEMENTATION_COLOR_READ_FORMAT,
    // &imp_formats); d_glGetIntegerv(context,
    // GL_IMPLEMENTATION_COLOR_READ_TYPE, &imp_type); CHECK_VALID_ENUM(__LINE__,
    // format, 3, GL_RGBA, imp_formats, GL_RGBA_INTEGER)
    // CHECK_VALID_ENUM(__LINE__, type, 6, GL_UNSIGNED_BYTE, GL_UNSIGNED_INT,
    // GL_UNSIGNED_INT_2_10_10_10_REV,
    //                  GL_INT, GL_FLOAT, imp_type)
    CHECK_NEGATIVE(__LINE__, width < 0 || height, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_PACK_BUFFER target and the buffer object's data
     * store is currently mapped.*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_PACK_BUFFER target and the data would be packed to
     * the buffer object such that the memory writes required would exceed the
     * data store size.*/
    /*TODO GL_INVALID_OPERATION is generated if GL_READ_BUFFER is GL_NONE or if
     * GL_READ_FRAMEBUFFER_BINDING is non-zero and the read buffer selects an
     * attachment that has no image attached.*/
    /*TODO GL_INVALID_OPERATION is generated if the readbuffer of the currently
     * bound framebuffer is a fixed point normalized surface and format and type
     * are neither GL_RGBA and GL_UNSIGNED_BYTE, respectively, nor the
     * format/type pair returned by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT
     * and GL_IMPLEMENTATION_COLOR_READ_TYPE.*/
    /*TODO GL_INVALID_OPERATION is generated if the readbuffer of the currently
     * bound framebuffer is a floating point surface and format and type are
     * neither GL_RGBA and GL_FLOAT, respectively, nor the format/type pair
     * returned by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT and
     * GL_IMPLEMENTATION_COLOR_READ_TYPE.*/
    /*TODO GL_INVALID_OPERATION is generated if the readbuffer of the currently
     * bound framebuffer is a signed integer surface and format and type are
     * neither GL_RGBA_INTEGER and GL_INT, respectively, nor the format/type
     * pair returned by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT and
     * GL_IMPLEMENTATION_COLOR_READ_TYPE.*/
    /*TODO GL_INVALID_OPERATION is generated if the readbuffer of the currently
     * bound framebuffer is an unsigned integer surface and format and type are
     * neither GL_RGBA_INTEGER and GL_UNSIGNED_INT, respectively, nor the
     * format/type pair returned by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT
     * and GL_IMPLEMENTATION_COLOR_READ_TYPE.*/
    CHECK_READ_FRAMEBUFFER_COMPLETE(GL_INVALID_FRAMEBUFFER_OPERATION)
    //    GLint fbo,samples;
    //    d_glGetIntegerv(context,GL_READ_FRAMEBUFFER_BINDING,&fbo);
    //    d_glGetIntegerv(context,GL_SAMPLE_BUFFERS,&samples);
    //    if(fbo_complete==GL_FRAMEBUFFER_COMPLETE&&fbo!=0&&samples>0){set_gl_error(context,GL_INVALID_OPERATION);return;}
    /*TODO GL_INVALID_OPERATION is generated by glReadnPixels if the buffer size
     * required to store the requested data is greater than bufSize.*/

    int start;
    int buf_len;
    gl_pixel_data_size(context, width, height, format, type, 1, &start,
                       &buf_len);
    // #define LOG_TAG "main_renderer custom"
    ALOGD("readpixels buffer len %d %d", start, buf_len);
    if (get_bound_buffer(context, GL_PIXEL_PACK_BUFFER) == 0) {
        r_glPixelStorei_origin(context, GL_PACK_SKIP_PIXELS, 0);
        r_glReadPixels_without_bound(context, x, y, width, height, format, type,
                                     buf_len, ((char *)pixels + start));
        ALOGD("readpixels without bound ok");
    } else {
        r_glPixelStorei_origin(context, GL_PACK_SKIP_PIXELS,
                               start / pixel_size_calc(format, type));
        r_glReadPixels_with_bound(context, x, y, width, height, format, type,
                                  (GLintptr)pixels);
        ALOGD("readpixels bound ok");
    }
}

void d_glReadnPixelsEXT(void *context, GLint x, GLint y, GLsizei width,
                        GLsizei height, GLenum format, GLenum type,
                        GLsizei bufSize, void *data) {
    CHECK_VALID_ENUM(
        __LINE__, type, 15, GL_UNSIGNED_BYTE, GL_BYTE, GL_UNSIGNED_SHORT,
        GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_HALF_FLOAT, GL_FLOAT,
        GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_4_4_4_4,
        GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_INT_2_10_10_10_REV,
        GL_UNSIGNED_INT_24_8, GL_UNSIGNED_INT_10F_11F_11F_REV,
        GL_UNSIGNED_INT_5_9_9_9_REV, GL_FLOAT_32_UNSIGNED_INT_24_8_REV)
    CHECK_NEGATIVE(__LINE__, width < 0 || height, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_PACK_BUFFER target and the buffer object's data
     * store is currently mapped.*/
    /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name is
     * bound to the GL_PIXEL_PACK_BUFFER target and the data would be packed to
     * the buffer object such that the memory writes required would exceed the
     * data store size.*/
    /*TODO GL_INVALID_OPERATION is generated if GL_READ_BUFFER is GL_NONE or if
     * GL_READ_FRAMEBUFFER_BINDING is non-zero and the read buffer selects an
     * attachment that has no image attached.*/
    /*TODO GL_INVALID_OPERATION is generated if the readbuffer of the currently
     * bound framebuffer is a fixed point normalized surface and format and type
     * are neither GL_RGBA and GL_UNSIGNED_BYTE, respectively, nor the
     * format/type pair returned by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT
     * and GL_IMPLEMENTATION_COLOR_READ_TYPE.*/
    /*TODO GL_INVALID_OPERATION is generated if the readbuffer of the currently
     * bound framebuffer is a floating point surface and format and type are
     * neither GL_RGBA and GL_FLOAT, respectively, nor the format/type pair
     * returned by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT and
     * GL_IMPLEMENTATION_COLOR_READ_TYPE.*/
    /*TODO GL_INVALID_OPERATION is generated if the readbuffer of the currently
     * bound framebuffer is a signed integer surface and format and type are
     * neither GL_RGBA_INTEGER and GL_INT, respectively, nor the format/type
     * pair returned by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT and
     * GL_IMPLEMENTATION_COLOR_READ_TYPE.*/
    /*TODO GL_INVALID_OPERATION is generated if the readbuffer of the currently
     * bound framebuffer is an unsigned integer surface and format and type are
     * neither GL_RGBA_INTEGER and GL_UNSIGNED_INT, respectively, nor the
     * format/type pair returned by querying GL_IMPLEMENTATION_COLOR_READ_FORMAT
     * and GL_IMPLEMENTATION_COLOR_READ_TYPE.*/
    CHECK_READ_FRAMEBUFFER_COMPLETE(GL_INVALID_FRAMEBUFFER_OPERATION)
    //    GLint fbo,samples;
    //    d_glGetIntegerv(context,GL_READ_FRAMEBUFFER_BINDING,&fbo);
    //    d_glGetIntegerv(context,GL_SAMPLE_BUFFERS,&samples);
    //    if(fbo_complete==GL_FRAMEBUFFER_COMPLETE&&fbo!=0&&samples>0){set_gl_error(context,GL_INVALID_OPERATION);return;}
    /*TODO GL_INVALID_OPERATION is generated by glReadnPixels if the buffer size
     * required to store the requested data is greater than bufSize.*/

    // todo set error if bufSize< pixeldatasize
    d_glReadPixels(context, x, y, width, height, format, type, data);
}

void d_glReadBuffer(void *context, GLenum src) {
    //    BoundBuffer *bound_buffer = &(((GLThreadContext *)
    //    context)->bound_buffer_status); GLuint fbo =
    //    bound_buffer->now_frame_buffer_object;

    GLint max;
    d_glGetIntegerv_special(context, GL_MAX_COLOR_ATTACHMENTS, &max);
    if (src != GL_BACK && src != GL_NONE &&
        !(src >= GL_COLOR_ATTACHMENT0 && src - GL_COLOR_ATTACHMENT0 < max)) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }

    BindingStatus *binding_status =
        &(((GLThreadContext *)context)->context_binding_status);
    GLuint fbo = binding_status->read_frame_buffer;

    if (fbo == 0 && src == GL_BACK) {
        r_glReadBuffer_special(context, GL_COLOR_ATTACHMENT0);
        binding_status->read_buffer = src;
    } else if (fbo == 0 && src != GL_NONE) {
        ALOGE("readbuffer fbo %d src %x\n", fbo, src);
        set_gl_error(context, GL_INVALID_OPERATION);
    } else if (fbo != 0 && src == GL_BACK) {
        ALOGE("readbuffer fbo %d src %x\n", fbo, src);
        set_gl_error(context, GL_INVALID_OPERATION);
    } else {
        r_glReadBuffer_special(context, src);
        binding_status->read_buffer = src;
    }
}

void d_glTexParameterfv_special(void *context, GLenum target, GLenum pname,
                                const GLfloat *params) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    switch (pname) {
        case GL_DEPTH_STENCIL_TEXTURE_MODE:
            if (info->depth_stencil_mode == (GLint)*params) {
                return;
            }
            info->depth_stencil_mode = (GLint)*params;
            break;
        // case GL_IMAGE_FORMAT_COMPATIBILITY_TYPE:
        case GL_TEXTURE_BASE_LEVEL:
            if (info->base_level == (GLint)*params) {
                return;
            }
            info->base_level = (GLint)*params;
            break;
        case GL_TEXTURE_COMPARE_FUNC:
            if (info->compare_func == (GLint)*params) {
                return;
            }
            info->compare_func = (GLint)*params;
            break;
        case GL_TEXTURE_COMPARE_MODE:
            if (info->compare_mode == (GLint)*params) {
                return;
            }
            info->compare_mode = (GLint)*params;
            break;
        // case GL_TEXTURE_IMMUTABLE_FORMAT:
        // case GL_TEXTURE_IMMUTABLE_LEVELS:
        case GL_TEXTURE_MAG_FILTER:
            if (info->mag_filter == (GLint)*params) {
                return;
            }
            info->mag_filter = (GLint)*params;
            break;
        case GL_TEXTURE_MAX_LEVEL:
            if (info->max_level == (GLint)*params) {
                return;
            }
            info->max_level = (GLint)*params;
            break;
        case GL_TEXTURE_MAX_LOD:
            if (info->max_lod == (GLint)*params) {
                return;
            }
            info->max_lod = (GLint)*params;
            break;
        case GL_TEXTURE_MIN_FILTER:
            if (info->min_filter == (GLint)*params) {
                return;
            }
            info->min_filter = (GLint)*params;
            break;
        case GL_TEXTURE_MIN_LOD:
            if (info->min_lod == (GLint)*params) {
                return;
            }
            info->min_lod = (GLint)*params;
            break;
        case GL_TEXTURE_SWIZZLE_R:
            if (info->swizzle_r == (GLint)*params) {
                return;
            }
            info->swizzle_r = (GLint)*params;
            break;
        case GL_TEXTURE_SWIZZLE_G:
            if (info->swizzle_g == (GLint)*params) {
                return;
            }
            info->swizzle_g = (GLint)*params;
            break;
        case GL_TEXTURE_SWIZZLE_B:
            if (info->swizzle_b == (GLint)*params) {
                return;
            }
            info->swizzle_b = (GLint)*params;
            break;
        case GL_TEXTURE_SWIZZLE_A:
            if (info->swizzle_a == (GLint)*params) {
                return;
            }
            info->swizzle_a = (GLint)*params;
            break;
        case GL_TEXTURE_WRAP_S:
            if (info->wrap_s == (GLint)*params) {
                return;
            }
            info->wrap_s = (GLint)*params;
            break;
        case GL_TEXTURE_WRAP_T:
            if (info->wrap_t == (GLint)*params) {
                return;
            }
            info->wrap_t = (GLint)*params;
            break;
        case GL_TEXTURE_WRAP_R:
            if (info->wrap_r == (GLint)*params) {
                return;
            }
            info->wrap_r = (GLint)*params;
            break;
            // case GL_TEXTURE_BORDER_COLOR:
    }

    if (pname == GL_TEXTURE_CROP_RECT_OES) {
        info->crop_rect[0] = (GLint)params[0];
        info->crop_rect[1] = (GLint)params[1];
        info->crop_rect[2] = (GLint)params[2];
        info->crop_rect[3] = (GLint)params[3];
    } else {
        r_glTexParameterfv(context, target, pname, params);
    }
}

void d_glTexParameteriv_special(void *context, GLenum target, GLenum pname,
                                const GLint *params) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    switch (pname) {
        case GL_DEPTH_STENCIL_TEXTURE_MODE:
            if (info->depth_stencil_mode == *params) {
                return;
            }
            info->depth_stencil_mode = *params;
            break;
        // case GL_IMAGE_FORMAT_COMPATIBILITY_TYPE:
        case GL_TEXTURE_BASE_LEVEL:
            if (info->base_level == *params) {
                return;
            }
            info->base_level = *params;
            break;
        case GL_TEXTURE_COMPARE_FUNC:
            if (info->compare_func == *params) {
                return;
            }
            info->compare_func = *params;
            break;
        case GL_TEXTURE_COMPARE_MODE:
            if (info->compare_mode == *params) {
                return;
            }
            info->compare_mode = *params;
            break;
        // case GL_TEXTURE_IMMUTABLE_FORMAT:
        // case GL_TEXTURE_IMMUTABLE_LEVELS:
        case GL_TEXTURE_MAG_FILTER:
            if (info->mag_filter == *params) {
                return;
            }
            info->mag_filter = *params;
            break;
        case GL_TEXTURE_MAX_LEVEL:
            if (info->max_level == *params) {
                return;
            }
            info->max_level = *params;
            break;
        case GL_TEXTURE_MAX_LOD:
            if (info->max_lod == *params) {
                return;
            }
            info->max_lod = *params;
            break;
        case GL_TEXTURE_MIN_FILTER:
            if (info->min_filter == *params) {
                return;
            }
            info->min_filter = *params;
            break;
        case GL_TEXTURE_MIN_LOD:
            if (info->min_lod == *params) {
                return;
            }
            info->min_lod = *params;
            break;
        case GL_TEXTURE_SWIZZLE_R:
            if (info->swizzle_r == *params) {
                return;
            }
            info->swizzle_r = *params;
            break;
        case GL_TEXTURE_SWIZZLE_G:
            if (info->swizzle_g == *params) {
                return;
            }
            info->swizzle_g = *params;
            break;
        case GL_TEXTURE_SWIZZLE_B:
            if (info->swizzle_b == *params) {
                return;
            }
            info->swizzle_b = *params;
            break;
        case GL_TEXTURE_SWIZZLE_A:
            if (info->swizzle_a == *params) {
                return;
            }
            info->swizzle_a = *params;
            break;
        case GL_TEXTURE_WRAP_S:
            if (info->wrap_s == *params) {
                return;
            }
            info->wrap_s = *params;
            break;
        case GL_TEXTURE_WRAP_T:
            if (info->wrap_t == *params) {
                return;
            }
            info->wrap_t = *params;
            break;
        case GL_TEXTURE_WRAP_R:
            if (info->wrap_r == *params) {
                return;
            }
            info->wrap_r = *params;
            break;
            // case GL_TEXTURE_BORDER_COLOR:
    }

    if (pname == GL_TEXTURE_CROP_RECT_OES) {
        info->crop_rect[0] = (GLint)params[0];
        info->crop_rect[1] = (GLint)params[1];
        info->crop_rect[2] = (GLint)params[2];
        info->crop_rect[3] = (GLint)params[3];
    } else {
        r_glTexParameteriv(context, target, pname, params);
    }
}

void d_glGetTexParameterfv_special(void *context, GLenum target, GLenum pname,
                                   GLfloat *params) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    switch (pname) {
        case GL_DEPTH_STENCIL_TEXTURE_MODE:
            *params = (GLfloat)info->depth_stencil_mode;
            return;
        // case GL_IMAGE_FORMAT_COMPATIBILITY_TYPE:
        case GL_TEXTURE_BASE_LEVEL:
            *params = (GLfloat)info->base_level;
            return;
        case GL_TEXTURE_COMPARE_FUNC:
            *params = (GLfloat)info->compare_func;
            return;
        case GL_TEXTURE_COMPARE_MODE:
            *params = (GLfloat)info->compare_mode;
            return;
        // case GL_TEXTURE_IMMUTABLE_FORMAT:
        // case GL_TEXTURE_IMMUTABLE_LEVELS:
        case GL_TEXTURE_MAG_FILTER:
            *params = (GLfloat)info->mag_filter;
            return;
        case GL_TEXTURE_MAX_LEVEL:
            *params = (GLfloat)info->max_level;
            return;
        case GL_TEXTURE_MAX_LOD:
            *params = (GLfloat)info->max_lod;
            return;
        case GL_TEXTURE_MIN_FILTER:
            *params = (GLfloat)info->min_filter;
            return;
        case GL_TEXTURE_MIN_LOD:
            *params = (GLfloat)info->min_lod;
            return;
        case GL_TEXTURE_SWIZZLE_R:
            *params = (GLfloat)info->swizzle_r;
            return;
        case GL_TEXTURE_SWIZZLE_G:
            *params = (GLfloat)info->swizzle_g;
            return;
        case GL_TEXTURE_SWIZZLE_B:
            *params = (GLfloat)info->swizzle_b;
            return;
        case GL_TEXTURE_SWIZZLE_A:
            *params = (GLfloat)info->swizzle_a;
            return;
        case GL_TEXTURE_WRAP_S:
            *params = (GLfloat)info->wrap_s;
            return;
        case GL_TEXTURE_WRAP_T:
            *params = (GLfloat)info->wrap_t;
            return;
        case GL_TEXTURE_WRAP_R:
            *params = (GLfloat)info->wrap_r;
            return;
            // case GL_TEXTURE_BORDER_COLOR:
    }

    if (pname == GL_TEXTURE_CROP_RECT_OES) {
        params[0] = (GLfloat)info->crop_rect[0];
        params[1] = (GLfloat)info->crop_rect[1];
        params[2] = (GLfloat)info->crop_rect[2];
        params[3] = (GLfloat)info->crop_rect[3];
    } else {
        r_glGetTexParameterfv(context, target, pname, params);
        ALOGE("%s get remote_controller target %x pname %x data %f",
              __FUNCTION__, target, pname, *params);
    }
}

void d_glGetTexParameteriv_special(void *context, GLenum target, GLenum pname,
                                   GLint *params) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    switch (pname) {
        case GL_DEPTH_STENCIL_TEXTURE_MODE:
            *params = info->depth_stencil_mode;
            return;
        // case GL_IMAGE_FORMAT_COMPATIBILITY_TYPE:
        case GL_TEXTURE_BASE_LEVEL:
            *params = info->base_level;
            return;
        case GL_TEXTURE_COMPARE_FUNC:
            *params = info->compare_func;
            return;
        case GL_TEXTURE_COMPARE_MODE:
            *params = info->compare_mode;
            return;
        // case GL_TEXTURE_IMMUTABLE_FORMAT:
        // case GL_TEXTURE_IMMUTABLE_LEVELS:
        case GL_TEXTURE_MAG_FILTER:
            *params = info->mag_filter;
            return;
        case GL_TEXTURE_MAX_LEVEL:
            *params = info->max_level;
            return;
        case GL_TEXTURE_MAX_LOD:
            *params = info->max_lod;
            return;
        case GL_TEXTURE_MIN_FILTER:
            *params = info->min_filter;
            return;
        case GL_TEXTURE_MIN_LOD:
            *params = info->min_lod;
            return;
        case GL_TEXTURE_SWIZZLE_R:
            *params = info->swizzle_r;
            return;
        case GL_TEXTURE_SWIZZLE_G:
            *params = info->swizzle_g;
            return;
        case GL_TEXTURE_SWIZZLE_B:
            *params = info->swizzle_b;
            return;
        case GL_TEXTURE_SWIZZLE_A:
            *params = info->swizzle_a;
            return;
        case GL_TEXTURE_WRAP_S:
            *params = info->wrap_s;
            return;
        case GL_TEXTURE_WRAP_T:
            *params = info->wrap_t;
            return;
        case GL_TEXTURE_WRAP_R:
            *params = info->wrap_r;
            return;
            // case GL_TEXTURE_BORDER_COLOR:
    }

    if (pname == GL_TEXTURE_CROP_RECT_OES) {
        params[0] = (GLint)info->crop_rect[0];
        params[1] = (GLint)info->crop_rect[1];
        params[2] = (GLint)info->crop_rect[2];
        params[3] = (GLint)info->crop_rect[3];
    } else if (pname == GL_TEXTURE_IMMUTABLE_FORMAT &&
               (target == GL_TEXTURE_2D || target == GL_TEXTURE_3D ||
                target == GL_TEXTURE_CUBE_MAP ||
                target == GL_TEXTURE_2D_ARRAY)) {
        if (info != nullptr) {
            *params = info->immutable[0];
            ALOGD("%s get local target %x pname %x info not null immutable %d",
                  __FUNCTION__, target, pname, info->immutable[0]);
        }
#ifdef LOCAL_NONE_WITH_REMOTE_CHECK
        GLint temp_params;
        r_glGetTexParameteriv(context, target, pname, &temp_params);
        if (*params != temp_params) {
            ALOGE(
                "%s get different local remote immutable target %x pname %x "
                "remote %d local %d",
                __FUNCTION__, target, pname, temp_params, *params);
        } else {
            ALOGE(
                "%s get same local remote immutable target %x pname %x remote "
                "%d local %d",
                __FUNCTION__, target, pname, temp_params, *params);
        }
#endif
    } else {
        r_glGetTexParameteriv(context, target, pname, params);
        ALOGE("%s get remote_controller target %x pname %x data %d",
              __FUNCTION__, target, pname, *params);
    }
}

// #include <utils/CallStack.h>
void d_glEGLImageTargetTexture2DOES_special(void *context, GLenum target,
                                            GLeglImageOES image) {
    // android::CallStack callstack;
    // callstack.update();
    // callstack.log("callstack-tag-display", ANDROID_LOG_DEBUG, "  ");
    GLThreadContext *opengl_context = ((GLThreadContext *)context);
    BindingStatus *binding_status = &(opengl_context->context_binding_status);

    GLuint now_texture =
        (target == GL_TEXTURE_2D)
            ? binding_status->texture_binding[binding_status->active_texture]
                  .texture_binding_2d
            : binding_status->texture_binding[0].texture_binding_external;

    EGLImageInfo *image_info = NULL;

    GLuint share_texture = 0;
    EGLContext share_context = NULL;
    uint64_t gbuffer_id = 0;

    if (image != NULL &&
        all_image_info.find((EGLImage)image) != all_image_info.end()) {
        image_info = all_image_info[(EGLImage)image];
        if (image_info == NULL) {
            ALOGE("glbindeglimage get null image_info with image %llx",
                  (long long)(EGLImage)image);
            return;
        }
        share_context = image_info->share_context;
        share_texture = image_info->share_texture_id;

        gbuffer_id = image_info->gbuffer_id;

        image_info->need_send_data = 1;

        ResourceContext *res_ctx = &(opengl_context->resource_context);

        if (res_ctx->texture_has_gbuffer()->has_id(now_texture)) {
            EGLImageInfo *old_image_info =
                opengl_context->texture_gbuffer_map[0][now_texture];
            if (old_image_info == image_info) {
                ALOGE("BindEGLimage same texture %d gbuffer_id %llx",
                      now_texture, (unsigned long long)gbuffer_id);
                return;
            } else {
                old_image_info->using_cnt--;
                try_destroy_EGLImage(context, old_image_info);
            }
        }

        res_ctx->texture_has_gbuffer()->set_id(now_texture);
        image_info->using_cnt++;
        opengl_context->texture_gbuffer_map[0][now_texture] = image_info;

        if (target == GL_TEXTURE_2D) {
            binding_status->gbuffer_2D_binding = image_info;
        } else {
            binding_status->gbuffer_external_binding = image_info;
        }

        

        ALOGE(
            "gbuffer %llx display should upload %lld usage %lld gbuffer_type "
            "%d has_id %d",
            (unsigned long long)image_info->gbuffer_id,
            (long long)(image_info->usage & 8192),
            (long long)(image_info->usage), image_info->gbuffer_type,
            res_ctx->texture_has_gbuffer()->has_id(now_texture));
        // if(((image_info->usage)&8192) != 0)
        // {
        //     upload_graphic_buffer(context,image_info);
        // }

        // if(image_info->gbuffer_type == GBUFFER_TYPE_NATIVE
        //     && opengl_context->context_share_thread_ids->size() > 1)
        // {
        //     upload_graphic_buffer(context, image_info);
        // }

        ALOGE(
            "glbindEGLImage target %x gbuffer_id %llx now_texture %d "
            "share_texture %d share_context %llx",
            target, (long long)gbuffer_id, now_texture, share_texture,
            (long long)share_context);

        r_glBindEGLImage(context, target, gbuffer_id, now_texture,
                         share_texture, share_context);
    }
    return;
}

void upload_graphic_buffer(void *context, EGLImageInfo *image_info) {
    if (image_info == NULL || image_info->graphic_buffer == NULL) {
        return;
    }

    //    android::GraphicBuffer *g_buffer = (android::GraphicBuffer
    //    *)image_info->graphic_buffer;

    void *real_buffer;

    int length_per_size = 4;
    int row_byte_length = 4 * image_info->width;

    if (image_info->format == WINDOW_FORMAT_RGBA_8888 ||
        image_info->format == WINDOW_FORMAT_RGBX_8888) {
        length_per_size = 4;
        row_byte_length = 4 * image_info->width;
    } else if (image_info->format == WINDOW_FORMAT_RGB_565) {
        length_per_size = 2;
        row_byte_length = 2 * image_info->width;
    } else {
        ALOGE("error! unknow GraphicBuffer format %x", image_info->format);
    }

    int real_width = image_info->width;

    if (real_width % (image_info->stride) != 0) {
        real_width = (real_width / image_info->stride + 1) * image_info->stride;
    }

    int buf_len = length_per_size * real_width * image_info->height;

    //    int status =
    //    g_buffer->lock(android::GraphicBuffer::USAGE_SW_READ_OFTEN,
    //    &real_buffer);

    //    ALOGE("guest send eglImage to host %llx status %d gbuffer_id %llx
    //    width %d height %d format %d stride %d", (unsigned long
    //    long)image_info, status, (unsigned long long)image_info->gbuffer_id,
    //    (int)image_info->width, (int)image_info->height, image_info->format,
    //    image_info->stride); if (status == 0)
    {
        r_glGraphicBufferData(context, image_info->share_context,
                              image_info->gbuffer_id, image_info->width,
                              image_info->height, buf_len, row_byte_length,
                              image_info->stride, real_buffer);
    }

    //    g_buffer->unlock();
}

void download_graphic_buffer(void *context, EGLImageInfo *image_info) {
    if (image_info == NULL || image_info->graphic_buffer == NULL) {
        return;
    }

#ifdef ANDROID_SYSTEM
    android::GraphicBuffer *g_buffer =
        (android::GraphicBuffer *)image_info->graphic_buffer;
#endif

    int length_per_size = 4;
    int row_byte_length = 4 * image_info->width;

    if (image_info->format == WINDOW_FORMAT_RGBA_8888 ||
        image_info->format == WINDOW_FORMAT_RGBX_8888) {
        length_per_size = 4;
        row_byte_length = 4 * image_info->width;
    } else if (image_info->format == WINDOW_FORMAT_RGB_565) {
        length_per_size = 2;
        row_byte_length = 2 * image_info->width;
    }

    void *real_buffer;

    int real_width = image_info->width;

    if (real_width % (image_info->stride) != 0) {
        real_width = (real_width / image_info->stride + 1) * image_info->stride;
    }

    int buf_len = length_per_size * real_width * image_info->height;

#ifdef ANDROID_SYSTEM
    int status = g_buffer->lock(android::GraphicBuffer::USAGE_SW_WRITE_OFTEN,
                                &real_buffer);

    ALOGE(
        "guest read eglImage %llx status %d from host gbuffer_id %llx width %d "
        "height %d format %d stride %d",
        (unsigned long long)image_info, status,
        (unsigned long long)image_info->gbuffer_id, (int)image_info->width,
        (int)image_info->height, image_info->format, image_info->stride);
#endif

#ifdef ANDROID_SYSTEM
    if (status == 0)
#endif
    {
        r_glReadGraphicBuffer(context, image_info->share_context,
                              image_info->gbuffer_id, image_info->width,
                              image_info->height, buf_len, row_byte_length,
                              image_info->stride, real_buffer);
    }
#ifdef ANDROID_SYSTEM
    g_buffer->unlock();
#endif
}

void try_destroy_EGLImage(void *context, EGLImageInfo *image_info) {
    if (image_info != NULL) {
        ALOGE("try_destroy_EGLImage gbuffer %llx type %d",
              (long long)(image_info->gbuffer_id), image_info->gbuffer_type);

        if (image_info->using_cnt > 0 || image_info->need_destroy == 0) {
            return;
        }

        struct timeval now;
        gettimeofday(&now, NULL);
        uint64_t now_time = (int64_t)now.tv_sec * 1000000 + now.tv_usec;
        global_gbuffer_died_times[image_info->gbuffer_id] = now_time;
        ALOGE("real destroy_EGLImage gbuffer %llx type %d",
              (long long)(image_info->gbuffer_id), image_info->gbuffer_type);

        r_eglDestroyImage(context, NULL, image_info->gbuffer_id);
        all_image_info.erase((EGLImage)(image_info->gbuffer_id));
        delete image_info;
        return;
    } else {
        return;
    }
}

bool tex_parameter_should_set(void *context, GLenum target, GLenum pname,
                              GLint param) {
    GLThreadContext *opengl_context = ((GLThreadContext *)context);
    // if(opengl_context->share_opengl_context!=NULL)
    // {
    //     return true;
    // }
    BindingStatus *binding_status = &(opengl_context->context_binding_status);
    GLuint now_texture = 0;
    if (target == GL_TEXTURE_2D) {
        now_texture =
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_2d;
    } else {
        now_texture =
            binding_status->texture_binding[0].texture_binding_external;
    }

    TextureInfo *info = NULL;

    if (opengl_context->texture_info_map->find(now_texture) !=
        opengl_context->texture_info_map->end()) {
        info = opengl_context->texture_info_map[0][now_texture];
    } else {
        info = new TextureInfo;
        opengl_context->texture_info_map[0][now_texture] = info;
    }
    ALOGD("%s texture info %llx", __FUNCTION__, (unsigned long long)info);

    switch (pname) {
        case GL_TEXTURE_BASE_LEVEL: {
            if (info->base_level == param) {
                return false;
            }
            info->base_level = param;
            break;
        }
        case GL_TEXTURE_MIN_FILTER: {
            if (info->min_filter == param) {
                return false;
            }
            info->min_filter = param;
            break;
        }
        case GL_TEXTURE_MAG_FILTER: {
            if (info->mag_filter == param) {
                return false;
            }
            info->mag_filter = param;
            break;
        }
        case GL_TEXTURE_MAX_LEVEL: {
            if (info->max_level == param) {
                return false;
            }
            info->max_level = param;
            break;
        }
        case GL_TEXTURE_SWIZZLE_R: {
            if (info->swizzle_r == param) {
                return false;
            }
            info->swizzle_r = param;
            break;
        }
        case GL_TEXTURE_SWIZZLE_G: {
            if (info->swizzle_g == param) {
                return false;
            }
            info->swizzle_g = param;
            break;
        }
        case GL_TEXTURE_SWIZZLE_B: {
            if (info->swizzle_b == param) {
                return false;
            }
            info->swizzle_b = param;
            break;
        }
        case GL_TEXTURE_SWIZZLE_A: {
            if (info->swizzle_a == param) {
                return false;
            }
            info->swizzle_a = param;
            break;
        }
        case GL_TEXTURE_WRAP_S: {
            if (info->wrap_s == param) {
                return false;
            }
            info->wrap_s = param;
            break;
        }
        case GL_TEXTURE_WRAP_T: {
            if (info->wrap_t == param) {
                return false;
            }
            info->wrap_t = param;
            break;
        }
        case GL_TEXTURE_WRAP_R: {
            if (info->wrap_r == param) {
                return false;
            }
            info->wrap_r = param;
            break;
        }
        case GL_TEXTURE_COMPARE_MODE: {
            if (info->compare_mode == param) {
                return false;
            }
            info->compare_mode = param;
            break;
        }
        case GL_TEXTURE_COMPARE_FUNC: {
            if (info->compare_func == param) {
                return false;
            }
            info->compare_func = param;
            break;
        }

        default:
            ALOGE("tex_parameter_should_set error pname %x", pname);
            break;
    }
    return true;
}

bool tex_parameter_should_set(void *context, GLenum target, GLenum pname,
                              GLfloat param) {
    GLThreadContext *opengl_context = ((GLThreadContext *)context);
    BindingStatus *binding_status = &(opengl_context->context_binding_status);
    GLuint now_texture =
        binding_status->texture_binding[binding_status->active_texture]
            .texture_binding_2d;
    TextureInfo *info = NULL;

    if (opengl_context->texture_info_map->find(now_texture) !=
        opengl_context->texture_info_map->end()) {
        info = opengl_context->texture_info_map[0][now_texture];
    } else {
        info = new TextureInfo;
        opengl_context->texture_info_map[0][now_texture] = info;
    }
    ALOGD("%s texture info %llx", __FUNCTION__, (unsigned long long)info);

    switch (pname) {
        case GL_TEXTURE_MAX_LOD: {
            if (info->max_lod == param) {
                return false;
            }
            info->max_lod = param;
            break;
        }
        case GL_TEXTURE_MIN_LOD: {
            if (info->min_lod == param) {
                return false;
            }
            info->min_lod = param;
            break;
        }
        default:
            return tex_parameter_should_set(context, target, pname,
                                            (GLint)param);
            // ALOGE("tex_parameter_should_set error pname %x",pname);
            // break;
    }
    return true;
}

GLint get_image_info(void *context, GLuint unit, GLenum target) {
    BindingStatus *binding_status =
        &(((GLThreadContext *)context)->context_binding_status);

    if (binding_status->image_info[unit].texture_name == 0) {
        return 0;
    }

    switch (target) {
        case GL_IMAGE_BINDING_NAME:
            return (GLint)binding_status->image_info[unit].texture_name;

        case GL_IMAGE_BINDING_LEVEL:
            return (GLint)binding_status->image_info[unit].level;

        case GL_IMAGE_BINDING_LAYERED:
            return (GLint)binding_status->image_info[unit].layered;

        case GL_IMAGE_BINDING_LAYER:
            return (GLint)binding_status->image_info[unit].layer;

        case GL_IMAGE_BINDING_ACCESS:
            return (GLint)binding_status->image_info[unit].access;

        case GL_IMAGE_BINDING_FORMAT:
            return (GLint)binding_status->image_info[unit].format;

        default:
            ALOGE("get_image_info error target %x", target);
            break;
    }
    return 0;
}

void d_glBindImageTexture_special(void *context, GLuint unit, GLuint texture,
                                  GLint level, GLboolean layered, GLint layer,
                                  GLenum access, GLenum format) {
    BindingStatus *binding_status =
        &(((GLThreadContext *)context)->context_binding_status);

    binding_status->image_info[unit].access = access;
    binding_status->image_info[unit].texture_name = texture;
    binding_status->image_info[unit].level = level;
    binding_status->image_info[unit].layered = layered;
    binding_status->image_info[unit].layer = layer;
    binding_status->image_info[unit].format = format;

    r_glBindImageTexture(context, unit, texture, level, layered, layer, access,
                         format);
}

void d_glTexBuffer_special(void *context, GLenum target, GLenum internalformat,
                           GLuint buffer) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    // @todo
    // GL_INVALID_OPERATION is generated by glTextureBuffer if texture is not
    // the name of an existing texture object. if(info->type == 0)
    // {
    //     set_gl_error(context, GL_INVALID_OPERATION);
    //     ALOGE("error! cannot find texture when %s target %x internalformat %x
    //     buffer %d", __FUNCTION__, target, internalformat, buffer); return;
    // }

    int index = 0;
    switch (target) {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            index = 0;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:;
            index = 1;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            index = 2;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            index = 3;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            index = 4;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            index = 5;
            break;
        case GL_TEXTURE_2D:
        default:
            index = 0;
            break;
    }

    if (info->format[index] == internalformat &&
        info->buffer_binding[index] == buffer) {
        return;
    }

    info->format[index] = internalformat;
    info->buffer_binding[index] = 0;
    info->buffer_offset[index] = 0;

    if (buffer == 0) {
        info->buffer_size[index] = 0;
    } else {
        MapResult *map_res = get_map_result_with_id(context, buffer);
        if (map_res == NULL) {
            set_gl_error(context, GL_INVALID_OPERATION);
            ALOGE(
                "error! cannot find buffer %d mapped when %s target %x "
                "internalformat %x",
                buffer, __FUNCTION__, target, internalformat);
            return;
        }
        info->buffer_size[index] = map_res->total_len;
    }

    info->type = target;

    r_glTexBuffer(context, target, internalformat, buffer);
}

void d_glTexBufferRange_special(void *context, GLenum target,
                                GLenum internalformat, GLuint buffer,
                                GLintptr offset, GLsizeiptr size) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    // @todo
    // GL_INVALID_OPERATION is generated by glTextureBuffer if texture is not
    // the name of an existing texture object. if(info->type == 0)
    // {
    //     set_gl_error(context, GL_INVALID_OPERATION);
    //     ALOGE("error! cannot find texture when %s target %x internalformat %x
    //     buffer %d", __FUNCTION__, target, internalformat, buffer); return;
    // }

    MapResult *map_res = get_map_result_with_id(context, buffer);
    if (map_res == NULL) {
        set_gl_error(context, GL_INVALID_OPERATION);
        ALOGE(
            "error! cannot find buffer %d mapped when %s target %x "
            "internalformat %x",
            buffer, __FUNCTION__, target, internalformat);
        return;
    }
    if (offset < 0 || size <= 0 ||
        (map_res != NULL && (offset + size) > map_res->total_len)) {
        set_gl_error(context, GL_INVALID_OPERATION);
        ALOGE(
            "error! %s target %x internalformat %x buffer %d offset %lld size "
            "%lld buffer_size %lld",
            __FUNCTION__, target, internalformat, buffer, (long long)offset,
            (long long)size, (long long)map_res->total_len);
        return;
    }

    int index = 0;
    switch (target) {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            index = 0;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:;
            index = 1;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            index = 2;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            index = 3;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            index = 4;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            index = 5;
            break;
        case GL_TEXTURE_2D:
        default:
            index = 0;
            break;
    }

    if (info->format[index] == internalformat &&
        info->buffer_binding[index] == buffer &&
        info->buffer_offset[index] == offset &&
        info->buffer_size[index] == (GLint)size) {
        return;
    }

    info->format[index] = internalformat;
    info->buffer_binding[index] = buffer;
    info->buffer_offset[index] = offset;
    info->buffer_size[index] = size;

    info->type = target;

    r_glTexBufferRange(context, target, internalformat, buffer, offset, size);
}

TextureInfo *TextureInfo::get_texture_info(void *context, GLenum target) {
    BindingStatus *binding_status =
        &(((GLThreadContext *)context)->context_binding_status);

    GLuint now_texture = 0;
    switch (target) {
        case GL_TEXTURE_2D:
            now_texture =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d;
            break;
        case GL_TEXTURE_2D_ARRAY:
            now_texture =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_array;
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            now_texture =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_multisample;
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            now_texture =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_2d_multisample_array;
            break;
        case GL_TEXTURE_3D:
            now_texture =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_3d;
            break;
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            now_texture =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_cube_map;
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            now_texture =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_cube_map_array;
            break;
        case GL_TEXTURE_EXTERNAL_OES:
            now_texture =
                binding_status->texture_binding[0].texture_binding_external;
            break;
        case GL_TEXTURE_BUFFER:
            now_texture =
                binding_status->texture_binding[binding_status->active_texture]
                    .texture_binding_texture_buffer;
            break;
        default:
            ALOGE("glTexImage2D error target %x", target);
            break;
    }

    TextureInfo *info = nullptr;
    if (((GLThreadContext *)context)->texture_info_map->find(now_texture) !=
        ((GLThreadContext *)context)->texture_info_map->end()) {
        info = (*((GLThreadContext *)context)->texture_info_map)[now_texture];
    } else {
        info = new TextureInfo;
        (*((GLThreadContext *)context)->texture_info_map)[now_texture] = info;
    }

    return info;
}

FormatBits *TextureInfo::get_format_bits(GLint texture, GLenum target) {
    auto *thread_context = (ThreadContext *)egl_thread_context;
    auto *context = thread_context->opengl_context;
    TextureInfo *info = nullptr;
    if (context->texture_info_map->find(texture) !=
        context->texture_info_map->end()) {
        info = (*context->texture_info_map)[texture];
    } else {
        info = new TextureInfo;
        (*context->texture_info_map)[texture] = info;
    }

    int index = 0;
    switch (target) {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            index = 0;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:;
            index = 1;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            index = 2;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            index = 3;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            index = 4;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            index = 5;
            break;
        default:
            index = 0;
            break;
    }

    return &info->format_bits[index];
}

void d_glGetTexLevelParameteriv_special(void *context, GLenum target,
                                        GLint level, GLenum pname,
                                        GLint *params) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    int index = 0;
    switch (target) {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            index = 0;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:;
            index = 1;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            index = 2;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            index = 3;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            index = 4;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            index = 5;
            break;
        case GL_TEXTURE_2D:
        default:
            index = 0;
            break;
    }

    switch (pname) {
        case GL_TEXTURE_WIDTH:
            *params = (info->width[index] >> level);
            break;
        case GL_TEXTURE_HEIGHT:
            *params = (info->height[index] >> level);
            break;
        case GL_TEXTURE_DEPTH:
            *params = info->depth[index];
            break;
        case GL_TEXTURE_INTERNAL_FORMAT:
            *params = info->format[index];
            break;
        case GL_TEXTURE_COMPRESSED:
            *params = info->is_compressed[index];
            break;
        case GL_TEXTURE_SAMPLES:
            *params = info->samples[index];
            break;
        // cannot find GL_FIXED_SAMPLE_LOCATIONS
        // case GL_FIXED_SAMPLE_LOCATIONS:
        //     *params = info->fixedsamplelocations;
        //     break;
        case GL_TEXTURE_BUFFER_DATA_STORE_BINDING:
            *params = info->buffer_binding[index];
            break;
        case GL_TEXTURE_BUFFER_OFFSET:
            *params = info->buffer_offset[index];
            break;
        case GL_TEXTURE_BUFFER_SIZE:
            *params = info->buffer_size[index];
            break;
        default:
            ALOGE("%s get remote_controller target %x level %d pname %x",
                  __FUNCTION__, target, level, pname);
            r_glGetTexLevelParameteriv(context, target, level, pname, params);
            break;
    }
}

void d_glGetTexLevelParameterfv_special(void *context, GLenum target,
                                        GLint level, GLenum pname,
                                        GLfloat *params) {
    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    int index = 0;
    switch (target) {
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
            index = 0;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:;
            index = 1;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
            index = 2;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
            index = 3;
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
            index = 4;
            break;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            index = 5;
            break;
        case GL_TEXTURE_2D:
        default:
            index = 0;
            break;
    }

    switch (pname) {
        case GL_TEXTURE_WIDTH:
            *params = (GLfloat)(info->width[index] >> level);
            break;
        case GL_TEXTURE_HEIGHT:
            *params = (GLfloat)(info->height[index] >> level);
            break;
        case GL_TEXTURE_DEPTH:
            *params = (GLfloat)info->depth[index];
            break;
        case GL_TEXTURE_INTERNAL_FORMAT:
            *params = (GLfloat)info->format[index];
            break;
        case GL_TEXTURE_COMPRESSED:
            *params = (GLfloat)info->is_compressed[index];
            break;
        case GL_TEXTURE_SAMPLES:
            *params = (GLfloat)info->samples[index];
            break;
        // cannot find GL_FIXED_SAMPLE_LOCATIONS
        // case GL_FIXED_SAMPLE_LOCATIONS:
        //     *params = (GLfloat)info->fixedsamplelocations;
        //     break;
        case GL_TEXTURE_BUFFER_DATA_STORE_BINDING:
            *params = (GLfloat)info->buffer_binding[index];
            break;
        case GL_TEXTURE_BUFFER_OFFSET:
            *params = (GLfloat)info->buffer_offset[index];
            break;
        case GL_TEXTURE_BUFFER_SIZE:
            *params = (GLfloat)info->buffer_size[index];
            break;
        default:
            ALOGE("%s get remote_controller target %x level %d pname %x",
                  __FUNCTION__, target, level, pname);
            r_glGetTexLevelParameterfv(context, target, level, pname, params);
            break;
    }
}

void d_glTexStorage2DMultisample_special(void *context, GLenum target,
                                         GLsizei samples, GLenum internalformat,
                                         GLsizei width, GLsizei height,
                                         GLboolean fixedsamplelocations) {
    BindingStatus *binding_status =
        &(((GLThreadContext *)context)->context_binding_status);

    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    info->height[0] = height;
    info->width[0] = width;
    info->depth[0] = 0;
    info->format[0] = internalformat;
    info->immutable[0] = GL_TRUE;
    info->is_compressed[0] = GL_FALSE;
    info->type = target;

    info->samples[0] = samples;
    info->fixedsamplelocations[0] = fixedsamplelocations;

    if (internalformat == GL_ETC1_RGB8_OES) {
        internalformat = GL_COMPRESSED_RGB8_ETC2;
    }
    r_glTexStorage2DMultisample(context, target, samples, internalformat, width,
                                height, fixedsamplelocations);
}

void d_glTexStorage3D_special(void *context, GLenum target, GLsizei levels,
                              GLenum internalformat, GLsizei width,
                              GLsizei height, GLsizei depth) {
    BindingStatus *binding_status =
        &(((GLThreadContext *)context)->context_binding_status);

    TextureInfo *info = TextureInfo::get_texture_info(context, target);
    ALOGD("%s texture info %llx", __FUNCTION__, (long long)info);

    info->height[0] = height;
    info->width[0] = width;
    info->depth[0] = depth;
    info->level[0] = levels;
    info->format[0] = internalformat;
    info->immutable[0] = GL_TRUE;
    info->is_compressed[0] = GL_FALSE;
    info->type = target;

    if (internalformat == GL_ETC1_RGB8_OES) {
        internalformat = GL_COMPRESSED_RGB8_ETC2;
    }
    r_glTexStorage3D(context, target, levels, internalformat, width, height,
                     depth);
}