#include "opengl/Program.h"
#define LOG_TAG "GLImpl"

#include <GLES3/gl32.h>

#include <cstdlib>

#include "ErrorHandler.h"
#include "Framebuffer.h"
#include "GLDef.h"
#include "GLv1.h"
#include "Log.h"
#include "Resource.h"
#include "State.h"
#include "Texture.h"
#include "ThreadContext.h"

/******* file '1-1' *******/

GLenum d_glClientWaitSync(void *context, GLsync sync, GLbitfield flags,
                          GLuint64 timeout) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->sync_resource()->has_id((GLuint)(unsigned long long)sync)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return GL_WAIT_FAILED;
    }
    /*TODO GL_INVALID_VALUE is generated if flags contains any unsupported
     * flag.*/
    if (flags != 0 && !(flags & GL_SYNC_FLUSH_COMMANDS_BIT)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return GL_WAIT_FAILED;
    }
    GLenum ret = GL_ALREADY_SIGNALED;
    if (((GLThreadContext *)context)->context_share_thread_ids->size() > 1) {
        ret = r_glClientWaitSync(context, sync, flags, timeout);
    } else {
        r_glWaitSync(context, sync, 0, GL_TIMEOUT_IGNORED);
    }

    return ret;
}

GLint d_glTestInt1(void *context, GLint a, GLuint b) {
    return r_glTestInt1(context, a, b);
}

GLuint d_glTestInt2(void *context, GLint a, GLuint b) {
    return r_glTestInt2(context, a, b);
}

GLint64 d_glTestInt3(void *context, GLint64 a, GLuint64 b) {
    return r_glTestInt3(context, a, b);
}

GLuint64 d_glTestInt4(void *context, GLint64 a, GLuint64 b) {
    return r_glTestInt4(context, a, b);
}

GLfloat d_glTestInt5(void *context, GLint a, GLuint b) {
    return r_glTestInt5(context, a, b);
}

GLdouble d_glTestInt6(void *context, GLint a, GLuint b) {
    return r_glTestInt6(context, a, b);
}

void d_glTestPointer1(void *context, GLint a, const GLint *b) {
    r_glTestPointer1(context, a, b);
}

void d_glTestPointer2(void *context, GLint a, const GLint *b, GLint *c) {
    r_glTestPointer2(context, a, b, c);
}

GLint d_glTestPointer4(void *context, GLint a, const GLint *b, GLint *c) {
    return r_glTestPointer4(context, a, b, c);
}

void d_glTestString(void *context, GLint a, GLint count,
                    const GLchar *const *strings, GLint buf_len,
                    GLchar *char_buf) {
    r_glTestString(context, a, count, strings, buf_len, char_buf);
}

/******* file '1-1-1' *******/

/******* file '1-1-2' *******/

GLenum d_glGetError(void *context) {
    GLenum ret = get_gl_error(context);
    if (ret != GL_NO_ERROR) ALOGE("GL Error is 0x%x", ret);
    return ret;
    // return GL_NO_ERROR;
}

const GLubyte *d_glGetString(void *context, GLenum name) {
    const GLubyte *ret = d_glGetString_special(context, name);
    ALOGD("glGetString name %x: %s", name, ret);
    return ret;
}

const GLubyte *d_glGetStringi(void *context, GLenum name, GLuint index) {
    return d_glGetStringi_special(context, name, index);
}

GLenum d_glCheckFramebufferStatus(void *context, GLenum target) {
    {
        if (target != GL_DRAW_FRAMEBUFFER && target != GL_READ_FRAMEBUFFER &&
            target != GL_FRAMEBUFFER) {
            set_gl_error(context, GL_INVALID_ENUM);
            return 0;
        }
    }
    // return r_glCheckFramebufferStatus(context, target);
    return GL_FRAMEBUFFER_COMPLETE_OES;
}

GLbitfield d_glQueryMatrixxOES(void *context, GLfixed *mantissa,
                               GLint *exponent) {
    return r_glQueryMatrixxOES(context, mantissa, exponent);
}

void d_glGetFramebufferAttachmentParameteriv(void *context, GLenum target,
                                             GLenum attachment, GLenum pname,
                                             GLint *params) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_DRAW_FRAMEBUFFER,
                     GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER)
    // if (attachment != GL_DEPTH_ATTACHMENT && attachment !=
    // GL_STENCIL_ATTACHMENT && attachment != GL_DEPTH_STENCIL_ATTACHMENT)
    // {
    //     int check = attachment - GL_COLOR_ATTACHMENT0;
    //     CHECK_NEGATIVE(__LINE__, check, GL_INVALID_ENUM)
    //     CHECK_INT_MAX(__LINE__, check, GL_MAX_COLOR_ATTACHMENTS,
    //     GL_INVALID_ENUM)
    // }
    // have to call r_glGetFramebufferAttachmentParameteriv with
    // GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE first to check other error
    set_host_error(context);
    //    GLint result;
    //    r_glGetFramebufferAttachmentParameteriv(context, target,
    //    attachment,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,&result);
    //    if(result!=GL_NONE){
    //        switch (result) {
    //            case GL_RENDERBUFFER: CHECK_VALID_ENUM(__LINE__, pname, 9,
    //            GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
    //            GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
    //            GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
    //                                                   GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
    //                                                   GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
    //                                                   GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
    //                                                   GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE,
    //                                                   GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
    //                                                   GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME)
    //                                                   break;
    //            case GL_TEXTURE: CHECK_VALID_ENUM(__LINE__, pname , 13,
    //            GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
    //            GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,
    //            GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE ,
    //                                              GL_FRAMEBUFFER_ATTACHMENT_LAYERED,
    //                                              GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER
    //                                              ,GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
    //                                              GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
    //                                              GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
    //                                              GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
    //                                              GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE,
    //                                              GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING)
    //                                              break;
    //            default: CHECK_VALID_ENUM(__LINE__, pname,
    //            8,GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
    //            GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
    //            GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
    //                                      GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
    //                                      GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
    //                                      GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
    //                                      GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE,
    //                                      GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING)
    //        }
    //    }else
    //    if(pname!=GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME){set_gl_error(context,GL_INVALID_OPERATION);return;}
    CHECK_VALID_ENUM(__LINE__, pname, 14, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
                     GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
                     GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
                     GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
                     GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
                     GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
                     GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE,
                     GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
                     GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                     GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                     GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,
                     GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE,
                     GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER,
                     GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
    /*TODO GL_INVALID_OPERATION is generated if attachment is
     * GL_DEPTH_STENCIL_ATTACHMENT and different objects are bound to the depth
     * and stencil attachment points of target.*/
    if (attachment == GL_BACK) {
        attachment = GL_COLOR_ATTACHMENT0;
    }
    if (attachment == GL_DEPTH) {
        attachment = GL_DEPTH_ATTACHMENT;
    }
    if (attachment == GL_STENCIL) {
        attachment = GL_STENCIL_ATTACHMENT;
    }

    r_glGetFramebufferAttachmentParameteriv(context, target, attachment, pname,
                                            params);
}

void d_glGetProgramInfoLog(void *context, GLuint program, GLsizei bufSize,
                           GLsizei *length, GLchar *infoLog) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    r_glGetProgramInfoLog(context, program, bufSize, length, infoLog);
}

void d_glGetRenderbufferParameteriv(void *context, GLenum target, GLenum pname,
                                    GLint *params) {
    CHECK_VALID_ENUM(__LINE__, pname, 10, GL_RENDERBUFFER_WIDTH,
                     GL_RENDERBUFFER_HEIGHT, GL_RENDERBUFFER_INTERNAL_FORMAT,
                     GL_RENDERBUFFER_RED_SIZE, GL_RENDERBUFFER_GREEN_SIZE,
                     GL_RENDERBUFFER_BLUE_SIZE, GL_RENDERBUFFER_ALPHA_SIZE,
                     GL_RENDERBUFFER_DEPTH_SIZE, GL_RENDERBUFFER_STENCIL_SIZE,
                     GL_RENDERBUFFER_SAMPLES)
    r_glGetRenderbufferParameteriv(context, target, pname, params);
}

void d_glGetShaderInfoLog(void *context, GLuint shader, GLsizei bufSize,
                          GLsizei *length, GLchar *infoLog) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, shader, shader, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if shader is not a shader object.*/
    r_glGetShaderInfoLog(context, shader, bufSize, length, infoLog);
}

void d_glGetShaderPrecisionFormat(void *context, GLenum shadertype,
                                  GLenum precisiontype, GLint *range,
                                  GLint *precision) {
    CHECK_VALID_ENUM(__LINE__, shadertype, 2, GL_VERTEX_SHADER,
                     GL_FRAGMENT_SHADER)
    CHECK_VALID_ENUM(__LINE__, precisiontype, 6, GL_LOW_FLOAT, GL_MEDIUM_FLOAT,
                     GL_HIGH_FLOAT, GL_LOW_INT, GL_MEDIUM_INT, GL_HIGH_INT)

    d_glGetShaderPrecisionFormat_special(context, shadertype, precisiontype,
                                         range, precision);
}

void d_glGetShaderSource(void *context, GLuint shader, GLsizei bufSize,
                         GLsizei *length, GLchar *source) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, shader, shader, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if shader is not a shader object*/
    r_glGetShaderSource(context, shader, bufSize, length, source);
}

void d_glGetTexParameterfv(void *context, GLenum target, GLenum pname,
                           GLfloat *params) {
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_TEXTURE_2D, GL_TEXTURE_3D,
                     GL_TEXTURE_2D_ARRAY, GL_TEXTURE_EXTERNAL_OES,
                     GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
                     GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY)
    CHECK_VALID_ENUM(
        __LINE__, pname, 18, GL_DEPTH_STENCIL_TEXTURE_MODE,
        GL_TEXTURE_BASE_LEVEL, GL_TEXTURE_COMPARE_FUNC, GL_TEXTURE_COMPARE_MODE,
        GL_TEXTURE_IMMUTABLE_FORMAT, GL_TEXTURE_IMMUTABLE_LEVELS,
        GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MAX_LEVEL, GL_TEXTURE_MAX_LOD,
        GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MIN_LOD, GL_TEXTURE_SWIZZLE_R,
        GL_TEXTURE_SWIZZLE_G, GL_TEXTURE_SWIZZLE_B, GL_TEXTURE_SWIZZLE_A,
        GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R)
    d_glGetTexParameterfv_special(context, target, pname, params);
}

void d_glGetTexParameteriv(void *context, GLenum target, GLenum pname,
                           GLint *params) {
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_TEXTURE_2D, GL_TEXTURE_3D,
                     GL_TEXTURE_2D_ARRAY, GL_TEXTURE_EXTERNAL_OES,
                     GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
                     GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY)
    CHECK_VALID_ENUM(
        __LINE__, pname, 18, GL_DEPTH_STENCIL_TEXTURE_MODE,
        GL_TEXTURE_BASE_LEVEL, GL_TEXTURE_COMPARE_FUNC, GL_TEXTURE_COMPARE_MODE,
        GL_TEXTURE_IMMUTABLE_FORMAT, GL_TEXTURE_IMMUTABLE_LEVELS,
        GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MAX_LEVEL, GL_TEXTURE_MAX_LOD,
        GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MIN_LOD, GL_TEXTURE_SWIZZLE_R,
        GL_TEXTURE_SWIZZLE_G, GL_TEXTURE_SWIZZLE_B, GL_TEXTURE_SWIZZLE_A,
        GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R)
    d_glGetTexParameteriv_special(context, target, pname, params);
}

void d_glGetQueryiv(void *context, GLenum target, GLenum pname, GLint *params) {
    ALOGD("glGetQueryiv target %x pname %x", target, pname);
    CHECK_VALID_ENUM(__LINE__, target, 4, GL_ANY_SAMPLES_PASSED,
                     GL_ANY_SAMPLES_PASSED_CONSERVATIVE,
                     GL_PRIMITIVES_GENERATED,
                     GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN)
    CHECK_VALID_ENUM(__LINE__, pname, 1, GL_CURRENT_QUERY)
    r_glGetQueryiv(context, target, pname, params);
}

void d_glGetQueryObjectuiv(void *context, GLuint id, GLenum pname,
                           GLuint *params) {
    ALOGD("glGetQueryObjectiv id %u pname %x", id, pname);
    CHECK_VALID_ENUM(__LINE__, pname, 2, GL_QUERY_RESULT,
                     GL_QUERY_RESULT_AVAILABLE)
    CHECK_RESOURCE_ID(__LINE__, query, id, GL_INVALID_OPERATION)
    GLThreadContext *opengl_context = (GLThreadContext *)context;

    if (opengl_context->query_type_map.find(id) ==
        opengl_context->query_type_map.end()) {
        // *params = 0;
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    GLenum target = opengl_context->query_type_map[id];

    if (target == GL_ANY_SAMPLES_PASSED ||
        target == GL_ANY_SAMPLES_PASSED_CONSERVATIVE) {
        *params = 1;
        return;
    }
    /*TODO　GL_INVALID_OPERATION is generated if id is the name of a currently
     * active query object.*/
    ALOGE("glGetQueryObjectuiv from remote_controller target %x", pname);
    r_glGetQueryObjectuiv(context, id, pname, params);
}

void d_glGetTransformFeedbackVarying(void *context, GLuint program,
                                     GLuint index, GLsizei bufSize,
                                     GLsizei *length, GLsizei *size,
                                     GLenum *type, GLchar *name) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    CHECK_PROGRAM_INFO(__LINE__, index, GL_TRANSFORM_FEEDBACK_VARYINGS,
                       GL_INVALID_VALUE)
    CHECK_PROGRAM_LINKED
    r_glGetTransformFeedbackVarying(context, program, index, bufSize, length,
                                    size, type, name);
}

void d_glGetActiveUniformsiv(void *context, GLuint program,
                             GLsizei uniformCount, const GLuint *uniformIndices,
                             GLenum pname, GLint *params) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    // CHECK_PROGRAM_INFO(__LINE__, uniformCount, GL_ACTIVE_UNIFORMS,
    // GL_INVALID_VALUE)
    GLint para;
    d_glGetProgramiv_special(context, program, GL_ACTIVE_UNIFORMS, &para);
    if (uniformCount > para) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    CHECK_VALID_ENUM(__LINE__, pname, 8, GL_UNIFORM_TYPE, GL_UNIFORM_SIZE,
                     GL_UNIFORM_NAME_LENGTH, GL_UNIFORM_BLOCK_INDEX,
                     GL_UNIFORM_OFFSET, GL_UNIFORM_ARRAY_STRIDE,
                     GL_UNIFORM_MATRIX_STRIDE, GL_UNIFORM_IS_ROW_MAJOR)
    d_glGetActiveUniformsiv_special(context, program, uniformCount, uniformIndices,
                            pname, params);
}

void d_glGetActiveUniformBlockiv(void *context, GLuint program,
                                 GLuint uniformBlockIndex, GLenum pname,
                                 GLint *params) {
    CHECK_PROGRAM_INFO(__LINE__, uniformBlockIndex, GL_ACTIVE_UNIFORM_BLOCKS,
                       GL_INVALID_VALUE)
    CHECK_VALID_ENUM(__LINE__, pname, 7, GL_UNIFORM_BLOCK_BINDING,
                     GL_UNIFORM_BLOCK_DATA_SIZE, GL_UNIFORM_BLOCK_NAME_LENGTH,
                     GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,
                     GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,
                     GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER,
                     GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER);
    CHECK_PROGRAM_LINKED

    d_glGetActiveUniformBlockiv_special(context, program, uniformBlockIndex,
                                        pname, params);
}

void d_glGetActiveUniformBlockName(void *context, GLuint program,
                                   GLuint uniformBlockIndex, GLsizei bufSize,
                                   GLsizei *length, GLchar *uniformBlockName) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_PROGRAM_LINKED
    CHECK_PROGRAM_INFO(__LINE__, uniformBlockIndex, GL_ACTIVE_UNIFORM_BLOCKS,
                       GL_INVALID_VALUE)

    d_glGetActiveUniformBlockName_special(context, program, uniformBlockIndex,
                                          bufSize, length, uniformBlockName);
}

void d_glGetSamplerParameteriv(void *context, GLuint sampler, GLenum pname,
                               GLint *params) {
    CHECK_RESOURCE_ID(__LINE__, sampler, sampler, GL_INVALID_OPERATION)
    CHECK_VALID_ENUM(__LINE__, pname, 9, GL_TEXTURE_MAG_FILTER,
                     GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MIN_LOD,
                     GL_TEXTURE_MAX_LOD, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T,
                     GL_TEXTURE_WRAP_R, GL_TEXTURE_COMPARE_MODE,
                     GL_TEXTURE_COMPARE_FUNC)
    r_glGetSamplerParameteriv(context, sampler, pname, params);
}

void d_glGetSamplerParameterfv(void *context, GLuint sampler, GLenum pname,
                               GLfloat *params) {
    CHECK_RESOURCE_ID(__LINE__, sampler, sampler, GL_INVALID_OPERATION)
    CHECK_VALID_ENUM(__LINE__, pname, 9, GL_TEXTURE_MAG_FILTER,
                     GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MIN_LOD,
                     GL_TEXTURE_MAX_LOD, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T,
                     GL_TEXTURE_WRAP_R, GL_TEXTURE_COMPARE_MODE,
                     GL_TEXTURE_COMPARE_FUNC)
    r_glGetSamplerParameterfv(context, sampler, pname, params);
}

void d_glGetProgramBinary(void *context, GLuint program, GLsizei bufSize,
                          GLsizei *length, GLenum *binaryFormat, void *binary) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if bufSize is less than the size of
     * GL_PROGRAM_BINARY_LENGTH for program*/
    CHECK_PROGRAM_LINKED

#ifdef USE_LOCAL_COMPILER
    GLsizei real_length = 0;
    GLenum real_binary_format = 0;
    void *real_binary = malloc(bufSize);
    r_glGetProgramBinary(context, program, bufSize, &real_length,
                         &real_binary_format, real_binary);
    ((GLThreadContext *)context)
        ->glsl_compiler->get_program_binary(
            program, bufSize, length, binaryFormat, binary, real_binary_format,
            real_binary, real_length);
#else
    d_glGetProgramBinary_special(context, program, bufSize, length,
                                 binaryFormat, binary);
#endif

    ALOGD("glGetProgramBinary format %u", *binaryFormat);
}

void d_glGetProgramBinaryOES(void *context, GLuint program, GLsizei bufSize,
                             GLsizei *length, GLenum *binaryFormat,
                             void *binary) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if bufSize is less than the size of
     * GL_PROGRAM_BINARY_LENGTH for program*/
    CHECK_PROGRAM_LINKED
    d_glGetProgramBinary(context, program, bufSize, length, binaryFormat,
                         binary);
    ALOGD("glGetProgramBinary format %u", *binaryFormat);
}

void d_glGetInternalformativ(void *context, GLenum target,
                             GLenum internalformat, GLenum pname, GLsizei count,
                             GLint *params) {
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    CHECK_VALID_ENUM(__LINE__, pname, 2, GL_SAMPLES, GL_NUM_SAMPLE_COUNTS)
    /*TODO　GL_INVALID_ENUM is generated if internalformat is not color-,
     * depth-, or stencil-renderable.*/
    // CHECK_VALID_ENUM(__LINE__, target, 3, GL_RENDERBUFFER,
    // GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
    r_glGetInternalformativ(context, target, internalformat, pname, count,
                            params);
}

void d_glGetClipPlanexOES(void *context, GLenum plane, GLfixed *equation) {
    r_glGetClipPlanexOES(context, plane, equation);
}

void d_glGetFixedvOES(void *context, GLenum pname, GLfixed *params) {
    r_glGetFixedvOES(context, pname, params);
}

void d_glGetTexEnvxvOES(void *context, GLenum target, GLenum pname,
                        GLfixed *params) {
    r_glGetTexEnvxvOES(context, target, pname, params);
}

void d_glGetTexParameterxvOES(void *context, GLenum target, GLenum pname,
                              GLfixed *params) {
    d_glGetTexParameterxvOES_special(context, target, pname, params);
}

void d_glGetLightxvOES(void *context, GLenum light, GLenum pname,
                       GLfixed *params) {
    r_glGetLightxvOES(context, light, pname, params);
}

void d_glGetMaterialxvOES(void *context, GLenum face, GLenum pname,
                          GLfixed *params) {
    r_glGetMaterialxvOES(context, face, pname, params);
}

void d_glGetTexGenxvOES(void *context, GLenum coord, GLenum pname,
                        GLfixed *params) {
    r_glGetTexGenxvOES(context, coord, pname, params);
}

void d_glGetFramebufferParameteriv(void *context, GLenum target, GLenum pname,
                                   GLint *params) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_READ_FRAMEBUFFER,
                     GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER)
    CHECK_VALID_ENUM(__LINE__, pname, 4, GL_FRAMEBUFFER_DEFAULT_WIDTH,
                     GL_FRAMEBUFFER_DEFAULT_HEIGHT,
                     GL_FRAMEBUFFER_DEFAULT_SAMPLES,
                     GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS);
    GLint fbo;
    GLenum tar_binding = target == GL_READ_FRAMEBUFFER
                             ? GL_READ_FRAMEBUFFER_BINDING
                             : GL_DRAW_FRAMEBUFFER_BINDING;
    d_glGetIntegerv_special(context, tar_binding, &fbo);
    CHECK_ZERO(__LINE__, fbo, GL_INVALID_OPERATION)
    r_glGetFramebufferParameteriv(context, target, pname, params);
}

void d_glGetProgramInterfaceiv(void *context, GLuint program,
                               GLenum programInterface, GLenum pname,
                               GLint *params) {
    CHECK_VALID_ENUM(__LINE__, programInterface, 9, GL_UNIFORM,
                     GL_UNIFORM_BLOCK, GL_ATOMIC_COUNTER_BUFFER,
                     GL_PROGRAM_INPUT, GL_PROGRAM_OUTPUT,
                     GL_TRANSFORM_FEEDBACK_VARYING, GL_BUFFER_VARIABLE,
                     GL_SHADER_STORAGE_BLOCK, GL_TRANSFORM_FEEDBACK_BUFFER);
    CHECK_VALID_ENUM(__LINE__, pname, 3, GL_ACTIVE_RESOURCES,
                     GL_MAX_NAME_LENGTH, GL_MAX_NUM_ACTIVE_VARIABLES)
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    if (pname == GL_MAX_NAME_LENGTH &&
        (programInterface == GL_ATOMIC_COUNTER_BUFFER ||
         programInterface == GL_TRANSFORM_FEEDBACK_BUFFER)) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    if (pname == GL_MAX_NUM_ACTIVE_VARIABLES) {
        CHECK_VALID_ENUM_WITH_ERROR(
            __LINE__, programInterface, 4, GL_INVALID_OPERATION,
            GL_UNIFORM_BLOCK, GL_SHADER_STORAGE_BLOCK, GL_ATOMIC_COUNTER_BUFFER,
            GL_TRANSFORM_FEEDBACK_BUFFER)
    }
    d_glGetProgramInterfaceiv_special(context, program, programInterface, pname,
                                      params);
}

void d_glGetProgramResourceName(void *context, GLuint program,
                                GLenum programInterface, GLuint index,
                                GLsizei bufSize, GLsizei *length,
                                GLchar *name) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_VALID_ENUM(__LINE__, programInterface, 7, GL_UNIFORM,
                     GL_UNIFORM_BLOCK, GL_PROGRAM_INPUT, GL_PROGRAM_OUTPUT,
                     GL_TRANSFORM_FEEDBACK_VARYING, GL_BUFFER_VARIABLE,
                     GL_SHADER_STORAGE_BLOCK)
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    // have to call d_glGetProgramInterfaceiv get the active resouces
    //    GLint active_num;
    //    d_glGetProgramInterfaceiv(context, program, programInterface,
    //    GL_ACTIVE_RESOURCES,&active_num);
    //    if(index>=active_num){set_gl_error(context,GL_INVALID_VALUE);return;}
    set_host_error(context);
    if (programInterface == GL_ATOMIC_COUNTER_BUFFER ||
        programInterface == GL_TRANSFORM_FEEDBACK_BUFFER) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    d_glGetProgramResourceName_special(context, program, programInterface, index,
                               bufSize, length, name);
}

void d_glGetProgramResourceiv(void *context, GLuint program,
                              GLenum programInterface, GLuint index,
                              GLsizei propCount, const GLenum *props,
                              GLsizei bufSize, GLsizei *length, GLint *params) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    CHECK_ZERO(__LINE__, propCount, GL_INVALID_VALUE)
    for (int i = 0; i < propCount; i++) {
        switch (props[i]) {
            case GL_NAME_LENGTH:
                if (programInterface == GL_ATOMIC_COUNTER_BUFFER) {
                    set_gl_error(context, GL_INVALID_ENUM);
                    return;
                }
                break;
            case GL_TYPE:
                CHECK_VALID_ENUM(__LINE__, programInterface, 5, GL_UNIFORM,
                                 GL_PROGRAM_INPUT, GL_PROGRAM_OUTPUT,
                                 GL_TRANSFORM_FEEDBACK_VARYING,
                                 GL_BUFFER_VARIABLE)
                break;
            case GL_ARRAY_SIZE:
                CHECK_VALID_ENUM(__LINE__, programInterface, 5, GL_UNIFORM,
                                 GL_BUFFER_VARIABLE, GL_PROGRAM_INPUT,
                                 GL_PROGRAM_OUTPUT,
                                 GL_TRANSFORM_FEEDBACK_VARYING)
                break;
            case GL_OFFSET:
                CHECK_VALID_ENUM(__LINE__, programInterface, 3, GL_UNIFORM,
                                 GL_BUFFER_VARIABLE,
                                 GL_TRANSFORM_FEEDBACK_VARYING)
                break;
            case GL_BLOCK_INDEX:
            case GL_ARRAY_STRIDE:
            case GL_MATRIX_STRIDE:
            case GL_IS_ROW_MAJOR:
                CHECK_VALID_ENUM(__LINE__, programInterface, 2, GL_UNIFORM,
                                 GL_BUFFER_VARIABLE)
                break;
            case GL_ATOMIC_COUNTER_BUFFER_INDEX:
                CHECK_VALID_ENUM(__LINE__, programInterface, 1, GL_UNIFORM)
                break;
            case GL_BUFFER_BINDING:
            case GL_BUFFER_DATA_SIZE:
            case GL_NUM_ACTIVE_VARIABLES:
            case GL_ACTIVE_VARIABLES:
                CHECK_VALID_ENUM(__LINE__, programInterface, 3,
                                 GL_UNIFORM_BLOCK, GL_ATOMIC_COUNTER_BUFFER,
                                 GL_SHADER_STORAGE_BLOCK)
                break;
            case GL_REFERENCED_BY_VERTEX_SHADER:
            case GL_REFERENCED_BY_FRAGMENT_SHADER:
            case GL_REFERENCED_BY_COMPUTE_SHADER:
                CHECK_VALID_ENUM(__LINE__, programInterface, 7, GL_UNIFORM,
                                 GL_UNIFORM_BLOCK, GL_BUFFER,
                                 GL_SHADER_STORAGE_BLOCK, GL_BUFFER_VARIABLE,
                                 GL_PROGRAM_INPUT, GL_PROGRAM_OUTPUT)
                break;
            case GL_TOP_LEVEL_ARRAY_SIZE:
            case GL_TOP_LEVEL_ARRAY_STRIDE:
                CHECK_VALID_ENUM(__LINE__, programInterface, 1,
                                 GL_BUFFER_VARIABLE)
                break;
            case GL_LOCATION:
                CHECK_VALID_ENUM(__LINE__, programInterface, 3, GL_UNIFORM,
                                 GL_PROGRAM_INPUT, GL_PROGRAM_OUTPUT)
                break;
            case GL_PROGRAM_OUTPUT:
                CHECK_VALID_ENUM(__LINE__, programInterface, 1,
                                 GL_PROGRAM_OUTPUT)
                break;
                // case GL_LOCATION_COMPONENT: CHECK_VALID_ENUM(__LINE__,
                // programInterface, 2, GL_PROGRAM_INPUT, GL_PROGRAM_OUTPUT) not
                // found
        }
    }
    r_glGetProgramResourceiv(context, program, programInterface, index,
                             propCount, props, bufSize, length, params);
}

void d_glGetProgramPipelineiv(void *context, GLuint pipeline, GLenum pname,
                              GLint *params) {
    if (pipeline != 0) {
        CHECK_RESOURCE_ID(__LINE__, program_pipeline, pipeline,
                          GL_INVALID_OPERATION);
        return;
    }
    CHECK_VALID_ENUM(__LINE__, pname, 9, GL_ACTIVE_PROGRAM, GL_VERTEX_SHADER,
                     GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER, GL_INFO_LOG_LENGTH,
                     GL_VALIDATE_STATUS, GL_TESS_CONTROL_SHADER,
                     GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER)
    r_glGetProgramPipelineiv(context, pipeline, pname, params);
}

void d_glGetProgramPipelineInfoLog(void *context, GLuint pipeline,
                                   GLsizei bufSize, GLsizei *length,
                                   GLchar *infoLog) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, program_pipeline, pipeline,
                      GL_INVALID_OPERATION)

    r_glGetProgramPipelineInfoLog(context, pipeline, bufSize, length, infoLog);
}

void d_glGetMultisamplefv(void *context, GLenum pname, GLuint index,
                          GLfloat *val) {
    CHECK_VALID_ENUM(__LINE__, pname, 1, GL_SAMPLE_POSITION)
    CHECK_INT_MAX(__LINE__, index, GL_SAMPLES, GL_INVALID_VALUE)
    r_glGetMultisamplefv(context, pname, index, val);
}

void d_glGetTexLevelParameteriv(void *context, GLenum target, GLint level,
                                GLenum pname, GLint *params) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, target, 14, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY,
        GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_2D_MULTISAMPLE,
        GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_ARRAY,
        GL_TEXTURE_BUFFER);
    CHECK_VALID_ENUM(
        __LINE__, pname, 22, GL_TEXTURE_WIDTH, GL_TEXTURE_HEIGHT,
        GL_TEXTURE_DEPTH, GL_TEXTURE_INTERNAL_FORMAT, GL_TEXTURE_SAMPLES,
        GL_TEXTURE_FIXED_SAMPLE_LOCATIONS, GL_TEXTURE_RED_SIZE,
        GL_TEXTURE_GREEN_SIZE, GL_TEXTURE_BLUE_SIZE, GL_TEXTURE_ALPHA_SIZE,
        GL_TEXTURE_DEPTH_SIZE, GL_TEXTURE_STENCIL_SIZE, GL_TEXTURE_SHARED_SIZE,
        GL_TEXTURE_RED_TYPE, GL_TEXTURE_GREEN_TYPE, GL_TEXTURE_BLUE_TYPE,
        GL_TEXTURE_ALPHA_TYPE, GL_TEXTURE_DEPTH_TYPE, GL_TEXTURE_COMPRESSED,
        GL_TEXTURE_BUFFER_DATA_STORE_BINDING, GL_TEXTURE_BUFFER_OFFSET,
        GL_TEXTURE_BUFFER_SIZE)
    CHECK_NEGATIVE(__LINE__, level, GL_INVALID_VALUE)
    if (target == GL_TEXTURE_2D || target == GL_TEXTURE_2D_ARRAY ||
        target == GL_TEXTURE_3D) {
        GLint max;
        GLenum tar = target == GL_TEXTURE_3D ? GL_MAX_3D_TEXTURE_SIZE
                                             : GL_MAX_TEXTURE_SIZE;
        d_glGetIntegerv_special(context, tar, &max);
        if (level > log2(max)) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
    }
    if (target == GL_TEXTURE_2D_MULTISAMPLE && level != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    d_glGetTexLevelParameteriv_special(context, target, level, pname, params);
}

void d_glGetTexLevelParameterfv(void *context, GLenum target, GLint level,
                                GLenum pname, GLfloat *params) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, target, 14, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY,
        GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_2D_MULTISAMPLE,
        GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_ARRAY,
        GL_TEXTURE_BUFFER)
    CHECK_VALID_ENUM(
        __LINE__, pname, 22, GL_TEXTURE_WIDTH, GL_TEXTURE_HEIGHT,
        GL_TEXTURE_DEPTH, GL_TEXTURE_INTERNAL_FORMAT, GL_TEXTURE_SAMPLES,
        GL_TEXTURE_FIXED_SAMPLE_LOCATIONS, GL_TEXTURE_RED_SIZE,
        GL_TEXTURE_GREEN_SIZE, GL_TEXTURE_BLUE_SIZE, GL_TEXTURE_ALPHA_SIZE,
        GL_TEXTURE_DEPTH_SIZE, GL_TEXTURE_STENCIL_SIZE, GL_TEXTURE_SHARED_SIZE,
        GL_TEXTURE_RED_TYPE, GL_TEXTURE_GREEN_TYPE, GL_TEXTURE_BLUE_TYPE,
        GL_TEXTURE_ALPHA_TYPE, GL_TEXTURE_DEPTH_TYPE, GL_TEXTURE_COMPRESSED,
        GL_TEXTURE_BUFFER_DATA_STORE_BINDING, GL_TEXTURE_BUFFER_OFFSET,
        GL_TEXTURE_BUFFER_SIZE)
    CHECK_NEGATIVE(__LINE__, level, GL_INVALID_VALUE)
    if (target == GL_TEXTURE_2D || target == GL_TEXTURE_2D_ARRAY ||
        target == GL_TEXTURE_3D) {
        GLint max;
        GLenum tar = target == GL_TEXTURE_3D ? GL_MAX_3D_TEXTURE_SIZE
                                             : GL_MAX_TEXTURE_SIZE;
        d_glGetIntegerv_special(context, tar, &max);
        if (level > log2(max)) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
    }
    if (target == GL_TEXTURE_2D_MULTISAMPLE ||
        target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY ||
        target == GL_TEXTURE_BUFFER) {
        if (level != 0) {
            set_gl_error(context, GL_INVALID_VALUE);
            return;
        }
    }
    d_glGetTexLevelParameterfv_special(context, target, level, pname, params);
}

void d_glGetSynciv(void *context, GLsync sync, GLenum pname, GLsizei bufSize,
                   GLsizei *length, GLint *values) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, sync, (GLuint)(unsigned long long)sync,
                      GL_INVALID_VALUE)
    CHECK_VALID_ENUM(__LINE__, pname, 4, GL_OBJECT_TYPE, GL_SYNC_STATUS,
                     GL_SYNC_CONDITION, GL_SYNC_FLAGS)
    if (pname == GL_SYNC_STATUS) {
        if (((GLThreadContext *)context)->context_share_thread_ids->size() <=
            1) {
            if (length != 0) {
                *length = 1;
            }
            if (values != 0) {
                *values = GL_SIGNALED;
            }
            return;
        }
    }
    ALOGE("get sync iv pname %x", pname);
    r_glGetSynciv(context, sync, pname, bufSize, length, values);
}

GLint d_glGetAttribLocation(void *context, GLuint program, const GLchar *name) {
    // ALOGD("glGetAttribLocation program %u name %s",program,name);
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->program_resource()->has_id(program)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return 0;
    }
    
    
    // (strlen(name) >= 3 && strncmp("gl_", name, 3) == 0)
    // {
    //     set_gl_error(context, GL_INVALID_OPERATION);
    //     return -1;
    // }
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    if (((GLThreadContext *)context)->program_info_map->find(program) ==
        ((GLThreadContext *)context)->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return 0;
    }
    // return r_glGetAttribLocation(context, program, name);
    return d_glGetAttribLocation_special(context, program, name);
}

GLint d_glGetUniformLocation(void *context, GLuint program,
                             const GLchar *name) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->program_resource()->has_id(program)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return -1;
    }
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    if (((GLThreadContext *)context)->program_info_map->find(program) ==
        ((GLThreadContext *)context)->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return -1;
    }
    // return r_glGetUniformLocation(context, program, name);
    return d_glGetUniformLocation_special(context, program, name);
}

GLint d_glGetFragDataLocation(void *context, GLuint program,
                              const GLchar *name) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->program_resource()->has_id(program)) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return -1;
    }
    return r_glGetFragDataLocation(context, program, name);
}

GLuint d_glGetUniformBlockIndex(void *context, GLuint program,
                                const GLchar *uniformBlockName) {
    if (((GLThreadContext *)context)->program_info_map->find(program) ==
        ((GLThreadContext *)context)->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return 0;
    }

    return d_glGetUniformBlockIndex_special(context, program, uniformBlockName);
}

GLuint d_glGetProgramResourceIndex(void *context, GLuint program,
                                   GLenum programInterface,
                                   const GLchar *name) {
    if (programInterface != GL_UNIFORM &&
        programInterface != GL_UNIFORM_BLOCK &&
        programInterface != GL_PROGRAM_INPUT &&
        programInterface != GL_PROGRAM_OUTPUT &&
        programInterface != GL_TRANSFORM_FEEDBACK_VARYING &&
        programInterface != GL_BUFFER_VARIABLE &&
        programInterface != GL_SHADER_STORAGE_BLOCK) {
        set_gl_error(context, GL_INVALID_ENUM);
        return GL_INVALID_INDEX;
    }
    if (programInterface == GL_ATOMIC_COUNTER_BUFFER ||
        programInterface == GL_TRANSFORM_FEEDBACK_BUFFER) {
        set_gl_error(context, GL_INVALID_ENUM);
        return GL_INVALID_INDEX;
    }
    /*TODO Although not an error, GL_INVALID_INDEX is returned if name is not
     * the name of a resource within the interface identified by
     * programInterface.*/
    return d_glGetProgramResourceIndex_special(context, program, programInterface,
                                       name);
}

GLint d_glGetProgramResourceLocation(void *context, GLuint program,
                                     GLenum programInterface,
                                     const GLchar *name) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (!res_ctx->program_resource()->has_id(program)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return -1;
    }
    if (programInterface != GL_UNIFORM &&
        programInterface != GL_PROGRAM_INPUT &&
        programInterface != GL_PROGRAM_OUTPUT &&
        programInterface != GL_TRANSFORM_FEEDBACK_BUFFER) {
        set_gl_error(context, GL_INVALID_ENUM);
        return -1;
    }
    if (((GLThreadContext *)context)->program_info_map->find(program) ==
        ((GLThreadContext *)context)->program_info_map->end()) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return -1;
    }
    return d_glGetProgramResourceLocation_special(context, program, programInterface,
                                          name);
}

void d_glGetActiveAttrib(void *context, GLuint program, GLuint index,
                         GLsizei bufSize, GLsizei *length, GLint *size,
                         GLenum *type, GLchar *name) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    CHECK_PROGRAM_INFO(__LINE__, index, GL_ACTIVE_ATTRIBUTES, GL_INVALID_VALUE)

    // r_glGetActiveAttrib(context, program, index, bufSize, length, size, type,
    // name);
    d_glGetActiveAttrib_special(context, program, index, bufSize, length, size,
                                type, name);
}

void d_glGetActiveUniform(void *context, GLuint program, GLuint index,
                          GLsizei bufSize, GLsizei *length, GLint *size,
                          GLenum *type, GLchar *name) {
    CHECK_NEGATIVE(__LINE__, bufSize, GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    CHECK_PROGRAM_INFO(__LINE__, index, GL_ACTIVE_UNIFORMS, GL_INVALID_VALUE)

    // r_glGetActiveUniform(context, program, index, bufSize, length, size,
    // type, name);
    d_glGetActiveUniform_special(context, program, index, bufSize, length, size,
                                 type, name);
}

void d_glGetAttachedShaders(void *context, GLuint program, GLsizei maxCount,
                            GLsizei *count, GLuint *shaders) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    CHECK_NEGATIVE(__LINE__, maxCount, GL_INVALID_VALUE)

    d_glGetAttachedShaders_special(context, program, maxCount, count, shaders);
}

void d_glGetProgramiv(void *context, GLuint program, GLenum pname,
                      GLint *params) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program does not refer to a
     * program object.*/
    CHECK_VALID_ENUM(
        __LINE__, pname, 25, GL_ACTIVE_ATOMIC_COUNTER_BUFFERS,
        GL_ACTIVE_ATTRIBUTES, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
        GL_ACTIVE_UNIFORMS, GL_ACTIVE_UNIFORM_BLOCKS,
        GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, GL_ACTIVE_UNIFORM_MAX_LENGTH,
        GL_ATTACHED_SHADERS, GL_COMPUTE_WORK_GROUP_SIZE, GL_DELETE_STATUS,
        GL_GEOMETRY_SHADER_INVOCATIONS, GL_INFO_LOG_LENGTH, GL_LINK_STATUS,
        GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_PROGRAM_SEPARABLE,
        GL_TESS_CONTROL_OUTPUT_VERTICES, GL_TESS_GEN_MODE,
        GL_TESS_GEN_POINT_MODE, GL_TESS_GEN_SPACING, GL_TESS_GEN_VERTEX_ORDER,
        GL_TRANSFORM_FEEDBACK_BUFFER_MODE, GL_TRANSFORM_FEEDBACK_VARYINGS,
        GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, GL_VALIDATE_STATUS,
        GL_PROGRAM_BINARY_LENGTH);
    // r_glGetProgramiv(context, program, pname, params);
    d_glGetProgramiv_special(context, program, pname, params);
}

void d_glGetShaderiv(void *context, GLuint shader, GLenum pname,
                     GLint *params) {
    CHECK_RESOURCE_ID(__LINE__, shader, shader, GL_INVALID_VALUE)
    /*TODO　GL_INVALID_OPERATION is generated if shader does not refer to a
     * shader object.*/
    CHECK_VALID_ENUM(__LINE__, pname, 5, GL_SHADER_TYPE, GL_DELETE_STATUS,
                     GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH,
                     GL_SHADER_SOURCE_LENGTH)
    d_glGetShaderiv_special(context, shader, pname, params);
}

void d_glGetUniformfv(void *context, GLuint program, GLint location,
                      GLfloat *params) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    CHECK_PROGRAM_LINKED
    /*TODO GL_INVALID_OPERATION is generated if location does not correspond to
     * a valid uniform variable location for the specified program object.*/
    /*TODO GL_INVALID_OPERATION is generated by glGetnUniform if the buffer size
     * required to store the requested data is greater than bufSize.*/

    if (GLSLProgram::can_get_uniform(context, program, location,
                                     (char *)params)) {
        return;
    }

    r_glGetUniformfv(context, program, location, params);
}

void d_glGetUniformiv(void *context, GLuint program, GLint location,
                      GLint *params) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    CHECK_PROGRAM_LINKED
    /*TODO GL_INVALID_OPERATION is generated if location does not correspond to
     * a valid uniform variable location for the specified program object.*/
    /*TODO GL_INVALID_OPERATION is generated by glGetnUniform if the buffer size
     * required to store the requested data is greater than bufSize.*/

    if (GLSLProgram::can_get_uniform(context, program, location,
                                     (char *)params)) {
        return;
    }

    r_glGetUniformiv(context, program, location, params);
}

void d_glGetUniformuiv(void *context, GLuint program, GLint location,
                       GLuint *params) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    CHECK_PROGRAM_LINKED
    /*TODO GL_INVALID_OPERATION is generated if location does not correspond to
     * a valid uniform variable location for the specified program object.*/
    /*TODO GL_INVALID_OPERATION is generated by glGetnUniform if the buffer size
     * required to store the requested data is greater than bufSize.*/

    if (GLSLProgram::can_get_uniform(context, program, location,
                                     (char *)params)) {
        return;
    }

    r_glGetUniformuiv(context, program, location, params);
}

void d_glGetUniformIndices(void *context, GLuint program, GLsizei uniformCount,
                           const GLchar *const *uniformNames,
                           GLuint *uniformIndices) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    d_glGetUniformIndices_special(context, program, uniformCount, uniformNames,
                          uniformIndices);
}

void d_glGetBufferParameteriv(void *context, GLenum target, GLenum pname,
                              GLint *params) {
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER,
                     GL_COPY_WRITE_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
                     GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER,
                     GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER);
    GLint buffer_binding;
    buffer_binding = get_bound_buffer(context, target);
    CHECK_ZERO(__LINE__, buffer_binding, GL_INVALID_OPERATION)
    if (pname == GL_BUFFER_MAP_LENGTH || pname == GL_BUFFER_MAP_OFFSET) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    d_glGetBufferParameteriv_special(context, target, pname, params);
}

void d_glGetBufferParameteri64v(void *context, GLenum target, GLenum pname,
                                GLint64 *params) {
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER,
                     GL_COPY_WRITE_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
                     GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER,
                     GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER);
    GLint buffer_binding;
    buffer_binding = get_bound_buffer(context, target);
    CHECK_ZERO(__LINE__, buffer_binding, GL_INVALID_OPERATION)
    if (pname == GL_BUFFER_ACCESS_FLAGS || pname == GL_BUFFER_MAPPED ||
        pname == GL_BUFFER_USAGE) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    d_glGetBufferParameteri64v_special(context, target, pname, params);
}

void d_glGetBooleanv(void *context, GLenum pname, GLboolean *data) {
    d_glGetBooleanv_special(context, pname, data);
}

void d_glGetBooleani_v(void *context, GLenum target, GLuint index,
                       GLboolean *data) {
    r_glGetBooleani_v(context, target, index, data);
}

void d_glGetFloatv(void *context, GLenum pname, GLfloat *data) {
    d_glGetFloatv_special(context, pname, data);
}

void d_glGetIntegerv(void *context, GLenum pname, GLint *data) {
    d_glGetIntegerv_special(context, pname, data);
}

void d_glGetIntegeri_v(void *context, GLenum target, GLuint index,
                       GLint *data) {
    d_glGetIntegeri_v_special(context, target, index, data);
}

void d_glGetInteger64v(void *context, GLenum pname, GLint64 *data) {
    d_glGetInteger64v_special(context, pname, data);
}

void d_glGetInteger64i_v(void *context, GLenum target, GLuint index,
                         GLint64 *data) {
    d_glGetInteger64i_v_special(context, target, index, data);
}

GLint d_glTestPointer3(void *context, GLint a, const GLint *b, GLint *c) {
    return r_glTestPointer3(context, a, b, c);
}

/******* file '2-1' *******/

void d_glFlush(void *context) {
    r_glFlush(context);
    
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    if (((GLThreadContext *)context)->context_share_thread_ids->size() > 1 &&
        res_ctx->share_resources->has_create == 1) {
        ALOGE("glFlush sync");
        res_ctx->share_resources->has_create = 0;
        r_glSync(context);
    }
}

void d_glFinish(void *context) {
    r_glFinish(context);
    
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);

    if (((GLThreadContext *)context)->context_share_thread_ids->size() > 1 &&
        res_ctx->share_resources->has_create == 1) {
        ALOGE("glFinish sync");
        res_ctx->share_resources->has_create = 0;
        r_glSync(context);
    }
}

void d_glBeginQuery(void *context, GLenum target, GLuint id) {
    ALOGD("glBeginQuery target %x id %u context %llx", target, id,
          (long long)context);
    CHECK_VALID_ENUM(__LINE__, target, 4, GL_ANY_SAMPLES_PASSED,
                     GL_ANY_SAMPLES_PASSED_CONSERVATIVE,
                     GL_PRIMITIVES_GENERATED,
                     GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN)
    CHECK_ZERO(__LINE__, id, GL_INVALID_OPERATION)
    CHECK_RESOURCE_ID(__LINE__, query, id, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if glBeginQuery is executed while a
     * query object of the same target is already active. Note:
     * GL_ANY_SAMPLES_PASSED and GL_ANY_SAMPLES_PASSED_CONSERVATIVE alias to the
     * same target for the purposes of this error.*/
    /*TODO GL_INVALID_OPERATION is generated if id refers to an existing query
     * object whose type does not does not match target.*/
    GLThreadContext *opengl_context = (GLThreadContext *)context;
    opengl_context->query_type_map[id] = target;
    r_glBeginQuery(context, target, id);
}

void d_glEndQuery(void *context, GLenum target) {
    ALOGD("glEndQuery target %x context %llx", target, (long long)context);
    /*TODO GL_INVALID_OPERATION is generated if glEndQuery is executed when a
     * query object of the same target is not active.*/
    CHECK_VALID_ENUM(__LINE__, target, 4, GL_ANY_SAMPLES_PASSED,
                     GL_ANY_SAMPLES_PASSED_CONSERVATIVE,
                     GL_PRIMITIVES_GENERATED,
                     GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN)
    r_glEndQuery(context, target);
}

void d_glViewport(void *context, GLint x, GLint y, GLsizei width,
                  GLsizei height) {
    CHECK_NEGATIVE(__LINE__, width < 0 || height, GL_INVALID_VALUE)
    ALOGD("glviewport %d %d %lld %lld", x, y, (long long)width,
          (long long)height);
    r_glViewport(context, x, y, width, height);
}

void d_glTexStorage2D(void *context, GLenum target, GLsizei levels,
                      GLenum internalformat, GLsizei width, GLsizei height) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    /*TODO GL_INVALID_OPERATION is generated if the texture object curently
     * bound to target already has GL_TEXTURE_IMMUTABLE_FORMAT set to GL_TRUE.*/
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP,
                     GL_TEXTURE_EXTERNAL_OES)
    CHECK_VALID_ENUM(
        __LINE__, internalformat, 94, GL_R8, GL_R8_SNORM, GL_R16F, GL_R32F,
        GL_ETC1_RGB8_OES, GL_R8UI, GL_R8I, GL_R16UI, GL_R16I, GL_R32UI, GL_R32I,
        GL_RG8, GL_RG8_SNORM, GL_RG16F, GL_RG32F, GL_RG8UI, GL_RG8I, GL_RG16UI,
        GL_RG16I, GL_RG32UI, GL_RG32I, GL_RGB8, GL_SRGB8, GL_RGB565,
        GL_RGB8_SNORM, GL_R11F_G11F_B10F, GL_RGB9_E5, GL_RGB16F, GL_RGB32F,
        GL_RGB8UI, GL_RGB8I, GL_RGB16UI, GL_RGB16I, GL_RGB32UI, GL_RGB32I,
        GL_RGBA8, GL_SRGB8_ALPHA8, GL_RGBA8_SNORM, GL_RGB5_A1, GL_RGBA4,
        GL_RGB10_A2, GL_RGBA16F, GL_RGBA32F, GL_RGBA8UI, GL_RGBA8I,
        GL_RGB10_A2UI, GL_RGBA16UI, GL_RGBA16I, GL_RGBA32I, GL_RGBA32UI,
        GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT32F, GL_DEPTH24_STENCIL8, GL_DEPTH32F_STENCIL8,
        GL_COMPRESSED_R11_EAC, GL_COMPRESSED_SIGNED_R11_EAC,
        GL_COMPRESSED_RG11_EAC, GL_COMPRESSED_SIGNED_RG11_EAC,
        GL_COMPRESSED_RGB8_ETC2, GL_COMPRESSED_SRGB8_ETC2,
        GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
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
    GLint tex;
    GLenum tar_binding = target == GL_TEXTURE_2D ? GL_TEXTURE_BINDING_2D
                                                 : GL_TEXTURE_BINDING_CUBE_MAP;
    if (target == GL_TEXTURE_EXTERNAL_OES) {
        tar_binding = GL_TEXTURE_BINDING_EXTERNAL_OES;
    }
    d_glGetIntegerv_special(context, tar_binding, &tex);
    CHECK_ZERO(__LINE__, tex, GL_INVALID_OPERATION)
    if (levels < 1 || width < 1 || height < 1) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (levels > log2(std::max(width, height)) + 1) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    d_glTexStorage2D_special(context, target, levels, internalformat, width,
                             height);
}

void d_glTexStorage3D(void *context, GLenum target, GLsizei levels,
                      GLenum internalformat, GLsizei width, GLsizei height,
                      GLsizei depth) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    /*TODO GL_INVALID_OPERATION is generated if the texture object curently
     * bound to target already has GL_TEXTURE_IMMUTABLE_FORMAT set to GL_TRUE.*/
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY,
                     GL_TEXTURE_CUBE_MAP_ARRAY)
    CHECK_VALID_ENUM(
        __LINE__, internalformat, 94, GL_R8, GL_R8_SNORM, GL_R16F, GL_R32F,
        GL_ETC1_RGB8_OES, GL_R8UI, GL_R8I, GL_R16UI, GL_R16I, GL_R32UI, GL_R32I,
        GL_RG8, GL_RG8_SNORM, GL_RG16F, GL_RG32F, GL_RG8UI, GL_RG8I, GL_RG16UI,
        GL_RG16I, GL_RG32UI, GL_RG32I, GL_RGB8, GL_SRGB8, GL_RGB565,
        GL_RGB8_SNORM, GL_R11F_G11F_B10F, GL_RGB9_E5, GL_RGB16F, GL_RGB32F,
        GL_RGB8UI, GL_RGB8I, GL_RGB16UI, GL_RGB16I, GL_RGB32UI, GL_RGB32I,
        GL_RGBA8, GL_SRGB8_ALPHA8, GL_RGBA8_SNORM, GL_RGB5_A1, GL_RGBA4,
        GL_RGB10_A2, GL_RGBA16F, GL_RGBA32F, GL_RGBA8UI, GL_RGBA8I,
        GL_RGB10_A2UI, GL_RGBA16UI, GL_RGBA16I, GL_RGBA32I, GL_RGBA32UI,
        GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT32F, GL_DEPTH24_STENCIL8, GL_DEPTH32F_STENCIL8,
        GL_COMPRESSED_R11_EAC, GL_COMPRESSED_SIGNED_R11_EAC,
        GL_COMPRESSED_RG11_EAC, GL_COMPRESSED_SIGNED_RG11_EAC,
        GL_COMPRESSED_RGB8_ETC2, GL_COMPRESSED_SRGB8_ETC2,
        GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2,
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
    GLint tex;
    GLenum tar_binding = target == GL_TEXTURE_3D ? GL_TEXTURE_BINDING_3D
                                                 : GL_TEXTURE_BINDING_2D_ARRAY;
    d_glGetIntegerv_special(context, tar_binding, &tex);
    CHECK_ZERO(__LINE__, tex, GL_INVALID_OPERATION)
    if (levels < 1 || width < 1 || height < 1) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    // if (target == GL_TEXTURE_2D_ARRAY && (width != height || depth % 6 != 0))
    // {
    //     set_gl_error(context, GL_INVALID_VALUE);
    //     return;
    // }
    if (target == GL_TEXTURE_3D &&
        levels > log2(std::max(width, std::max(height, depth))) + 1) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    if (target == GL_TEXTURE_2D_ARRAY &&
        levels > log2(std::max(width, height)) + 1) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    // ALOGD("glTexStorage3D target %x levels %d internalformat %x width %d
    // height %d depth %d",target, (int)levels, internalformat, (int)width,
    // (int)height, (int)depth);
    d_glTexStorage3D_special(context, target, levels, internalformat, width,
                             height, depth);
}

// void d_glTexImage2D_with_bound(void *context, GLenum target, GLint level,
// GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum
// format, GLenum type, GLintptr pixels)
//{
//
//     r_glTexImage2D_with_bound(context, target, level, internalformat, width,
//     height, border, format, type, pixels);
// }
//
// void d_glTexSubImage2D_with_bound(void *context, GLenum target, GLint level,
// GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format,
// GLenum type, GLintptr pixels)
//{
//
//     r_glTexSubImage2D_with_bound(context, target, level, xoffset, yoffset,
//     width, height, format, type, pixels);
// }
//
// void d_glTexImage3D_with_bound(void *context, GLenum target, GLint level,
// GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint
// border, GLenum format, GLenum type, GLintptr pixels)
//{
//
//     r_glTexImage3D_with_bound(context, target, level, internalformat, width,
//     height, depth, border, format, type, pixels);
// }
//
// void d_glTexSubImage3D_with_bound(void *context, GLenum target, GLint level,
// GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height,
// GLsizei depth, GLenum format, GLenum type, GLintptr pixels)
//{
//
//     r_glTexSubImage3D_with_bound(context, target, level, xoffset, yoffset,
//     zoffset, width, height, depth, format, type, pixels);
// }
//
// void d_glReadPixels_with_bound(void *context, GLint x, GLint y, GLsizei
// width, GLsizei height, GLenum format, GLenum type, GLintptr pixels)
//{
//
//     r_glReadPixels_with_bound(context, x, y, width, height, format, type,
//     pixels);
// }
//
// void d_glCompressedTexImage2D_with_bound(void *context, GLenum target, GLint
// level, GLenum internalformat, GLsizei width, GLsizei height, GLint border,
// GLsizei imageSize, GLintptr data)
//{
//     CHECK_VALID_ENUM(__LINE__, internalformat, 7, GL_TEXTURE_2D,
//     GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
//                      GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
//                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
//                      GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
//                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
//     /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with
//     the format, dimensions, and contents of the specified compressed image
//     data.*/ if(border!=0){set_gl_error(context, GL_INVALID_VALUE); return;}
//     /*TODO GL_INVALID_OPERATION is generated if parameter combinations are
//     not supported by the specific compressed internal format as specified in
//     the specific texture compression extension*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's
//     data store is currently mapped*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be
//     unpacked from the buffer object such that the memory reads required would
//     exceed the data store size.*/
//
//     {
//         if (imageSize < 0)
//         {
//             set_gl_error(context, GL_INVALID_VALUE);
//             return;
//         }
//     }
//     r_glCompressedTexImage2D_with_bound(context, target, level,
//     internalformat, width, height, border, imageSize, data);
// }
//
// void d_glCompressedTexSubImage2D_with_bound(void *context, GLenum target,
// GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
// GLenum format, GLsizei imageSize, GLintptr data)
//{
//     CHECK_VALID_ENUM(__LINE__, target, 7, GL_TEXTURE_2D,
//     GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
//                      GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
//                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
//                      GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
//                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
//     /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with
//     the format, dimensions, and contents of the specified compressed image
//     data.*/
//     /*TODO GL_INVALID_OPERATION is generated if parameter combinations are
//     not supported by the specific compressed internal format as specified in
//     the specific texture compression extension.*/
//     /*TODO For ETC2/EAC images GL_INVALID_OPERATION is generated if width is
//     not a multiple of four, and width + xoffset is not equal to the width of
//     the texture level; if height is not a multiple of four, and height +
//     yoffset is not equal to the height of the texture level; or if xoffset or
//     yoffset is not a multiple of four.*/
//     /*TODO For ASTC images GL_INVALID_OPERATION is generated if width is not
//     a multiple of the block width, and width + xoffset is not equal to the
//     width of the texture level; if height is not a multiple of the block
//     height, and height + yoffset is not equal to the height of the texture
//     level; or if xoffset or yoffset is not a multiple of the corresponding
//     block dimension.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's
//     data store is currently mapped.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be
//     unpacked from the buffer object such that the memory reads required would
//     exceed the data store size.*/
//
//     {
//         if (imageSize < 0)
//         {
//             set_gl_error(context, GL_INVALID_VALUE);
//             return;
//         }
//     }
//     r_glCompressedTexSubImage2D_with_bound(context, target, level, xoffset,
//     yoffset, width, height, format, imageSize, data);
// }
//
// void d_glCompressedTexImage3D_with_bound(void *context, GLenum target, GLint
// level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth,
// GLint border, GLsizei imageSize, GLintptr data)
//{
//     CHECK_VALID_ENUM(__LINE__, target, 2, GL_TEXTURE_3D,
//     GL_TEXTURE_2D_ARRAY);
//     /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with
//     the format, dimensions, and contents of the specified compressed image
//     data.*/ if(border!=0){set_gl_error(context, GL_INVALID_VALUE); return;}
//     /*TODO GL_INVALID_OPERATION is generated if parameter combinations are
//     not supported by the specific compressed internal format as specified in
//     the specific texture compression extension. The ETC2/EAC texture
//     compression algorithm supports only two-dimensional images. If
//     internalformat is an ETC2/EAC format, glCompressedTexImage3D will
//     generate a GL_INVALID_OPERATION error if target is not
//     GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY. If internalformat is an
//     ASTC format, glCompressedTexImage3D will generate an INVALID_OPERATION
//     error if target is not GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY
//     unless GL_OES_texture_compression_astc is supported.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's
//     data store is currently mapped*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be
//     unpacked from the buffer object such that the memory reads required would
//     exceed the data store size.*/
//
//     {
//         if (imageSize < 0)
//         {
//             set_gl_error(context, GL_INVALID_VALUE);
//             return;
//         }
//     }
//     r_glCompressedTexImage3D_with_bound(context, target, level,
//     internalformat, width, height, depth, border, imageSize, data);
// }
//
// void d_glCompressedTexSubImage3D_with_bound(void *context, GLenum target,
// GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width,
// GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, GLintptr
// data)
//{
//     CHECK_VALID_ENUM(__LINE__, target,2, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY);
//     /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with
//     the format, dimensions, and contents of the specified compressed image
//     data.*/
//     /*TODO GL_INVALID_OPERATION is generated if parameter combinations are
//     not supported by the specific compressed internal format as specified in
//     the specific texture compression extension. For ETC2/EAC images
//     GL_INVALID_OPERATION is generated if width is not a multiple of four, and
//     width + xoffset is not equal to the width of the texture level; if height
//     is not a multiple of four, and height + yoffset is not equal to the
//     height of the texture level; or if xoffset or yoffset is not a multiple
//     of four. The ETC2/EAC texture compression algorithm supports only
//     two-dimensional images. If format is an ETC2/EAC format,
//     glCompressedTexSubImage3D will generate an GL_INVALID_OPERATION error if
//     target is not GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY.*/
//     /*TODO For ASTC images GL_INVALID_OPERATION is generated if width is not
//     a multiple of the block width, and width + xoffset is not equal to the
//     width of the texture level; if height is not a multiple of the block
//     height, and height + yoffset is not equal to the height of the texture
//     level; or if xoffset or yoffset is not a multiple of the corresponding
//     block dimension. If format is an ASTC format, glCompressedTexSubImage3D
//     will generate an GL_INVALID_OPERATION error if target is not
//     GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY, unless
//     GL_OES_texture_compression_astc is supported.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's
//     data store is currently mapped.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be
//     unpacked from the buffer object such that the memory reads required would
//     exceed the data store size.*/
//     r_glCompressedTexSubImage3D_with_bound(context, target, level, xoffset,
//     yoffset, zoffset, width, height, depth, format, imageSize, data);
// }

void d_glCopyTexImage2D(void *context, GLenum target, GLint level,
                        GLenum internalformat, GLint x, GLint y, GLsizei width,
                        GLsizei height, GLint border) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, target, 8, GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_EXTERNAL_OES);
    CHECK_NEGATIVE(__LINE__, level < 0 || height < 0 || width, GL_INVALID_VALUE)
    GLint max_size;
    d_glGetIntegerv_special(context, GL_MAX_TEXTURE_SIZE, &max_size);
    if (level > log2(max_size)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    if (height > max_size || width > max_size) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    if (border != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    CHECK_VALID_ENUM(__LINE__, internalformat, 34, GL_ALPHA, GL_LUMINANCE,
                     GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA, GL_R8, GL_RG8,
                     GL_RGB565, GL_RGB8, GL_RGBA4, GL_RGB5_A1, GL_RGBA8,
                     GL_RGB10_A2, GL_SRGB8, GL_SRGB8_ALPHA8, GL_R8I, GL_R8UI,
                     GL_R16I, GL_R16UI, GL_R32I, GL_R32UI, GL_RG8I, GL_RG8UI,
                     GL_RG16I, GL_RG16UI, GL_RG32I, GL_RG32UI, GL_RGBA8I,
                     GL_RGBA8UI, GL_RGB10_A2UI, GL_RGBA16I, GL_RGBA16UI,
                     GL_RGBA32I, GL_RGBA32UI)
    r_glCopyTexImage2D(context, target, level, internalformat, x, y, width,
                       height, border);
}

void d_glCopyTexSubImage2D(void *context, GLenum target, GLint level,
                           GLint xoffset, GLint yoffset, GLint x, GLint y,
                           GLsizei width, GLsizei height) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, target, 8, GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_EXTERNAL_OES);
    /*TODO GL_INVALID_OPERATION is generated if the texture array has not been
     * defined by a previous glTexImage2D, glCopyTexImage2D, or glTexStorage2D
     * operation.*/
    CHECK_NEGATIVE(__LINE__, level < 0 || height < 0 || width, GL_INVALID_VALUE)
    GLint max_size;
    d_glGetIntegerv_special(context, GL_MAX_TEXTURE_SIZE, &max_size);
    if (level > log2(max_size)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    if (height > max_size || width > max_size) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    r_glCopyTexSubImage2D(context, target, level, xoffset, yoffset, x, y, width,
                          height);
}

void d_glCopyTexSubImage3D(void *context, GLenum target, GLint level,
                           GLint xoffset, GLint yoffset, GLint zoffset, GLint x,
                           GLint y, GLsizei width, GLsizei height) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY,
                     GL_TEXTURE_CUBE_MAP_ARRAY);
    /*TODO GL_INVALID_OPERATION is generated if the texture array has not been
     * defined by a previous glTexImage3D or glTexStorage3D operation.*/
    CHECK_NEGATIVE(__LINE__, level < 0 || width < 0 || height, GL_INVALID_VALUE)
    GLint max_size;
    d_glGetIntegerv_special(context, GL_MAX_TEXTURE_SIZE, &max_size);
    if (level > log2(max_size)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    if (height > max_size || width > max_size) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    r_glCopyTexSubImage3D(context, target, level, xoffset, yoffset, zoffset, x,
                          y, width, height);
}

// void d_glVertexAttribPointer_with_bound(void *context, GLuint index, GLint
// size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr pointer)
//{
//
//     r_glVertexAttribPointer_with_bound(context, index, size, type,
//     normalized, stride, pointer);
// }
//
// void d_glVertexAttribPointer_offset(void *context, GLuint index, GLuint size,
// GLenum type, GLboolean normalized, GLsizei stride, GLuint index_father,
// GLintptr offset)
//{
//
//     r_glVertexAttribPointer_offset(context, index, size, type, normalized,
//     stride, index_father, offset);
// }

// void d_glMapBufferRange_write(void *context, GLenum target, GLintptr offset,
// GLsizeiptr length, GLbitfield access)
//{
//
//     r_glMapBufferRange_write(context, target, offset, length, access);
// }

// void d_glUnmapBuffer_special(void *context, GLenum target)
//{
//
//     r_glUnmapBuffer_special(context, target);
// }

void d_glWaitSync(void *context, GLsync sync, GLbitfield flags,
                  GLuint64 timeout) {
    CHECK_RESOURCE_ID(__LINE__, sync, (GLuint)(unsigned long long)sync,
                      GL_INVALID_OPERATION)
    if (flags != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    CHECK_VALID_ENUM_WITH_ERROR(__LINE__, timeout, 1, GL_INVALID_VALUE,
                                GL_TIMEOUT_IGNORED)
    ALOGD("glWaitSync %lld flag %d timeout %llu", (long long)sync, flags,
          (unsigned long long)timeout);
    r_glWaitSync(context, sync, flags, timeout);
}

void d_glShaderBinary(void *context, GLsizei count, const GLuint *shaders,
                      GLenum binaryFormat, const void *binary, GLsizei length) {
    /*TODO GL_INVALID_OPERATION is generated if more than one of the handles in
     * shaders refers to the same shader object.*/
    GLint num = 1;
    d_glGetIntegerv(context, GL_NUM_SHADER_BINARY_FORMATS, &num);
    GLint *formats = new GLint[num];
    d_glGetIntegerv(context, GL_SHADER_BINARY_FORMATS, formats);
    // if binaryFormat is not one of the valid formats for the implementation
    if (std::find(formats, formats + num, binaryFormat) == formats + num) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    /*TODO GL_INVALID_VALUE is generated if the data pointed to by binary does
     * not match the format specified by binaryFormat.*/
    r_glShaderBinary(context, count, shaders, binaryFormat, binary, length);
    delete[] formats;
}

void d_glProgramBinary(void *context, GLuint program, GLenum binaryFormat,
                       const void *binary, GLsizei length) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    GLint num = 1;
    d_glGetIntegerv_special(context, GL_NUM_PROGRAM_BINARY_FORMATS, &num);
    auto *binary_formats = new GLint[num];
    d_glGetIntegerv_special(context, GL_PROGRAM_BINARY_FORMATS, binary_formats);

#ifndef USE_LOCAL_COMPILER
    if (std::find(binary_formats, binary_formats + num, binaryFormat) ==
        binary_formats + num) {
        delete[] binary_formats;
        ALOGE("error format %x", binaryFormat);
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
#endif

    d_glProgramBinary_special(context, program, binaryFormat, binary, length);
    delete[] binary_formats;
}

void d_glProgramBinaryOES(void *context, GLuint program, GLenum binaryFormat,
                          const void *binary, GLsizei length) {
    d_glProgramBinary(context, program, binaryFormat, binary, length);
}

void d_glDrawBuffers(void *context, GLsizei n, const GLenum *bufs) {
    /*TODO GL_INVALID_OPERATION is generated if the GL is bound to the default
     * framebuffer and n is not 1, or if the value in bufs is one of the
     * GL_COLOR_ATTACHMENTn tokens.*/
    /*TODO GL_INVALID_OPERATION is generated if the GL is bound to a framebuffer
     * object and the ith buffer listed in bufs is anything other than GL_NONE
     * or GL_COLOR_ATTACHMENTSi.*/
    static GLint max_draw_buf = 0;
    if (max_draw_buf == 0) {
        d_glGetIntegerv_special(context, GL_MAX_DRAW_BUFFERS, &max_draw_buf);
    }
    if (n < 0 || n > max_draw_buf) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    static GLint max_color_attach = 0;
    if (max_color_attach == 0) {
        d_glGetIntegerv_special(context, GL_MAX_COLOR_ATTACHMENTS,
                                &max_color_attach);
    }
    // and the acceptable value of bufs is GL_BACK or GL_NONE and the range of
    // GL_COLOR_ATTACHMENT

    GLint fbo;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);

    fbo = bind_status->draw_frame_buffer;

    if (fbo == 0) {
        if (n != 1) {
            set_gl_error(context, GL_INVALID_OPERATION);
            return;
        }  // n must be 1
        if (bufs[0] >= GL_COLOR_ATTACHMENT0 &&
            bufs[0] <= GL_COLOR_ATTACHMENT0 + max_color_attach) {
            set_gl_error(context, GL_INVALID_OPERATION);
            return;
        }
        bind_status->drawbuffers[0] = bufs[0];
        GLenum new_bufs[1];
        
        new_bufs[0] = GL_COLOR_ATTACHMENT0;
        d_glDrawBuffers_special(context, 1, new_bufs);
    } else {
        for (int i = 0; i < n && i < 16; i++) {
            if (bufs[i] != GL_NONE) {
                if (unlikely(bufs[i] < GL_COLOR_ATTACHMENT0 ||
                             bufs[i] >
                                 GL_COLOR_ATTACHMENT0 + max_color_attach)) {
                    set_gl_error(context, GL_INVALID_ENUM);
                    return;
                }
            }
            bind_status->drawbuffers[i] = bufs[i];
        }
        d_glDrawBuffers_special(context, n, bufs);
    }
}

// void d_glDrawArrays_origin(void *context, GLenum mode, GLint first, GLsizei
// count)
//{
//
//     r_glDrawArrays_origin(context, mode, first, count);
// }
//
// void d_glDrawArraysInstanced_origin(void *context, GLenum mode, GLint first,
// GLsizei count, GLsizei instancecount)
//{
//
//     r_glDrawArraysInstanced_origin(context, mode, first, count,
//     instancecount);
// }
//
// void d_glDrawElementsInstanced_with_bound(void *context, GLenum mode, GLsizei
// count, GLenum type, GLsizeiptr indices, GLsizei instancecount)
//{
//
//     r_glDrawElementsInstanced_with_bound(context, mode, count, type, indices,
//     instancecount);
// }

// void d_glDrawElements_with_bound(void *context, GLenum mode, GLsizei count,
// GLenum type, GLsizeiptr indices)
//{
//
//     r_glDrawElements_with_bound(context, mode, count, type, indices);
// }
//
// void d_glDrawRangeElements_with_bound(void *context, GLenum mode, GLuint
// start, GLuint end, GLsizei count, GLenum type, GLsizeiptr indices)
//{
//
//     r_glDrawRangeElements_with_bound(context, mode, start, end, count, type,
//     indices);
// }

void d_glTestIntAsyn(void *context, GLint a, GLuint b, GLfloat c, GLdouble d) {
    r_glTestIntAsyn(context, a, b, c, d);
}

void d_glPrintfAsyn(void *context, GLint a, GLuint size, GLdouble c,
                    const GLchar *out_string) {
    r_glPrintfAsyn(context, a, size, c, out_string);
}

void d_glEGLImageTargetTexture2DOES(void *context, GLenum target,
                                    GLeglImageOES image) {
    if (target != GL_TEXTURE_2D && target != GL_TEXTURE_EXTERNAL_OES &&
        target != EGL_GL_TEXTURE_2D) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    d_glEGLImageTargetTexture2DOES_special(context, target, image);
}

void d_glEGLImageTargetRenderbufferStorageOES(void *context, GLenum target,
                                              GLeglImageOES image) {
    //    r_glEGLImageTargetRenderbufferStorageOES(context, target, image);
    

    ALOGE("d_glEGLImageTargetRenderbufferStorageOES target %x image %llx",
          target, (long long)image);
    return;
}

/******* file '2-1-2' *******/

void d_glLinkProgram(void *context, GLuint program) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    GLboolean active;
    d_glGetBooleanv_special(context, GL_TRANSFORM_FEEDBACK_ACTIVE, &active);
    GLint current_pro;
    d_glGetIntegerv_special(context, GL_CURRENT_PROGRAM, &current_pro);
    if (program == (GLuint)current_pro && active) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    d_glLinkProgram_special(context, program);
}

void d_glShaderSource(void *context, GLuint shader, GLsizei count,
                      const GLchar *const *string, const GLint *length) {
    CHECK_RESOURCE_ID(__LINE__, shader, shader, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if shader is not a shader object.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)

    d_glShaderSource_special(context, shader, count, string, length);
}

//
// void d_glPixelStorei_origin(void *context, GLenum pname, GLint param)
//{
//
//    r_glPixelStorei_origin(context, pname, param);
//}
//
// void d_glDisableVertexAttribArray_origin(void *context, GLuint index)
//{
//    CHECK_INT_MAX(__LINE__, index,GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE);
//    r_glDisableVertexAttribArray_origin(context, index);
//}

// void d_glEnableVertexAttribArray_origin(void *context, GLuint index)
//{
//
//     r_glEnableVertexAttribArray_origin(context, index);
// }

// void d_glReadBuffer_special(void *context, GLenum src)
//{
//
//     r_glReadBuffer_special(context, src);
// }

// void d_glVertexAttribDivisor_origin(void *context, GLuint index, GLuint
// divisor)
//{
//
//     r_glVertexAttribDivisor_origin(context, index, divisor);
// }

// void d_glShaderSource_origin(void *context, GLuint shader, GLsizei count,
// const GLint *length, const GLchar *const *string)
//{
//
//     r_glShaderSource_origin(context, shader, count, length, string);
// }
//
// void d_glVertexAttribIPointer_with_bound(void *context, GLuint index, GLint
// size, GLenum type, GLsizei stride, GLintptr pointer)
//{
//
//     r_glVertexAttribIPointer_with_bound(context, index, size, type, stride,
//     pointer);
// }
//
// void d_glVertexAttribIPointer_offset(void *context, GLuint index, GLint size,
// GLenum type, GLsizei stride, GLuint index_father, GLintptr offset)
//{
//
//     r_glVertexAttribIPointer_offset(context, index, size, type, stride,
//     index_father, offset);
// }

void d_glBindVertexArray(void *context, GLuint array) {
    
    // if (array != 0)
    // {
    //     CHECK_RESOURCE_ID(__LINE__, vertex_array, array,
    //     GL_INVALID_OPERATION);
    // }
    // ALOGD("bind vao %u",array);
    d_glBindVertexArray_special(context, array);
}

void d_glBindBuffer(void *context, GLenum target, GLuint buffer) {
    CHECK_VALID_ENUM(
        __LINE__, target, 13, GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER,
        GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER,
        GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
        GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_SHADER_STORAGE_BUFFER,
        GL_TEXTURE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER);
    d_glBindBuffer_special(context, target, buffer);
}

void d_glBeginTransformFeedback(void *context, GLenum primitiveMode) {
    GLboolean active;
    d_glGetBooleanv_special(context, GL_TRANSFORM_FEEDBACK_ACTIVE, &active);
    if (active) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    CHECK_VALID_ENUM(__LINE__, primitiveMode, 3, GL_POINTS, GL_LINES,
                     GL_TRIANGLES);
    /*TODO GL_INVALID_OPERATION is generated by glDrawArrays and other drawing
     * commands if transform feedback is active and mode is not compatible with
     * primitiveMode as described above.*/
    /*TODO GL_INVALID_OPERATION is generated by glBeginTransformFeedback if any
     * binding point used in transform feedback mode does not have a buffer
     * object bound. In interleaved mode, only the first buffer object binding
     * point is ever written to.*/
    /*TODO GL_INVALID_OPERATION is generated by glBeginTransformFeedback if no
     * binding points would be used, either because no program object is active
     * of because the active program object has specified no varying variables
     * to record.*/
    d_glBeginTransformFeedback_special(context, primitiveMode);
}

void d_glEndTransformFeedback(void *context) {
    GLboolean active;
    d_glGetBooleanv_special(context, GL_TRANSFORM_FEEDBACK_ACTIVE, &active);
    if (!active) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    /*TODO GL_INVALID_OPERATION is generated by glDrawArrays and other drawing
     * commands if transform feedback is active and mode is not compatible with
     * primitiveMode as described above.*/
    d_glEndTransformFeedback_special(context);
}

void d_glPauseTransformFeedback(void *context) {
    GLboolean active, paused;
    d_glGetBooleanv_special(context, GL_TRANSFORM_FEEDBACK_PAUSED, &paused);
    d_glGetBooleanv_special(context, GL_TRANSFORM_FEEDBACK_ACTIVE, &active);
    if (!active || paused) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    d_glPauseTransformFeedback_special(context);
}

void d_glResumeTransformFeedback(void *context) {
    GLboolean active, paused;
    d_glGetBooleanv_special(context, GL_TRANSFORM_FEEDBACK_PAUSED, &paused);
    d_glGetBooleanv_special(context, GL_TRANSFORM_FEEDBACK_ACTIVE, &active);
    if (!active || !paused) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    d_glResumeTransformFeedback_special(context);
}

void d_glBindBufferRange(void *context, GLenum target, GLuint index,
                         GLuint buffer, GLintptr offset, GLsizeiptr size) {
    CHECK_NEGATIVE(__LINE__, size, GL_INVALID_VALUE)

    if (target == GL_TRANSFORM_FEEDBACK_BUFFER) {
        static GLint max_transform_feedback_attribs = 0;
        if (max_transform_feedback_attribs == 0) {
            d_glGetIntegerv_special(context,
                                    GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,
                                    &max_transform_feedback_attribs);
        }
        if (unlikely(index >= max_transform_feedback_attribs || size % 4 != 0 ||
                     offset % 4 != 0)) {
            set_gl_error(context, GL_INVALID_VALUE);
        }
    } else if (target == GL_UNIFORM_BUFFER) {
        static GLint max_uniform_bindings = 0;
        static GLint uniform_offset = 0;
        if (max_uniform_bindings == 0) {
            d_glGetIntegerv_special(context, GL_MAX_UNIFORM_BUFFER_BINDINGS,
                                    &max_uniform_bindings);
        }
        if (uniform_offset == 0) {
            d_glGetIntegerv_special(context, GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
                                    &uniform_offset);
        }
        if (unlikely(index >= max_uniform_bindings ||
                     offset % uniform_offset != 0)) {
            set_gl_error(context, GL_INVALID_VALUE);
        }
    }

    // CHECK_VALID_ENUM(__LINE__, target, 4, GL_ATOMIC_COUNTER_BUFFER,
    // GL_SHADER_STORAGE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER,
    // GL_UNIFORM_BUFFER); GLenum check_max_target; switch (target)
    // {
    // case GL_ATOMIC_COUNTER_BUFFER:
    //     check_max_target = GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS;
    //     break;
    // case GL_SHADER_STORAGE_BUFFER:
    //     check_max_target = GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS;
    //     break;
    // case GL_TRANSFORM_FEEDBACK_BUFFER:
    //     check_max_target = GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS;
    //     break;
    // case GL_UNIFORM_BUFFER:
    //     check_max_target = GL_MAX_UNIFORM_BUFFER_BINDINGS;
    //     break;
    // }
    // CHECK_INT_MAX(__LINE__, index, check_max_target, GL_INVALID_VALUE)
    // CHECK_ZERO(__LINE__, buffer, GL_INVALID_VALUE);
    // if ((target == GL_ATOMIC_COUNTER_BUFFER || target ==
    // GL_TRANSFORM_FEEDBACK_BUFFER) && (size % 4 != 0 || offset % 4 != 0))
    // {
    //     set_gl_error(context, GL_INVALID_VALUE);
    //     return;
    // }
    // if (target == GL_UNIFORM_BUFFER || target == GL_SHADER_STORAGE_BUFFER)
    // {
    //     GLint alignment;
    //     GLenum tar = target == GL_UNIFORM_BUFFER ?
    //     GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT :
    //     GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT; d_glGetIntegerv(context,
    //     tar, &alignment); if (offset % alignment != 0)
    //     {
    //         set_gl_error(context, GL_INVALID_VALUE);
    //         return;
    //     }
    // }
    d_glBindBufferRange_special(context, target, index, buffer, offset, size);
}

void d_glBindBufferBase(void *context, GLenum target, GLuint index,
                        GLuint buffer) {
    // CHECK_VALID_ENUM(__LINE__, target, 4, GL_ATOMIC_COUNTER_BUFFER,
    // GL_SHADER_STORAGE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER,
    // GL_UNIFORM_BUFFER); GLenum check_max_target; switch (target)
    // {
    // case GL_ATOMIC_COUNTER_BUFFER:
    //     check_max_target = GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS;
    //     break;
    // case GL_SHADER_STORAGE_BUFFER:
    //     check_max_target = GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS;
    //     break;
    // case GL_TRANSFORM_FEEDBACK_BUFFER:
    //     check_max_target = GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS;
    //     break;
    // case GL_UNIFORM_BUFFER:
    //     check_max_target = GL_MAX_UNIFORM_BUFFER_BINDINGS;
    //     break;
    // }
    // CHECK_INT_MAX(__LINE__, index, check_max_target, GL_INVALID_VALUE)

    CHECK_VALID_ENUM(__LINE__, target, 4, GL_TRANSFORM_FEEDBACK_BUFFER,
                     GL_UNIFORM_BUFFER, GL_ATOMIC_COUNTER_BUFFER,
                     GL_SHADER_STORAGE_BUFFER);

    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(target, GL_TRANSFORM_FEEDBACK_BUFFER, index,
                           max_transform_feedback_separate_attribs,
                           GL_INVALID_VALUE)
    CHECK_TARGET_INDEX_MAX(target, GL_UNIFORM_BUFFER, index,
                           max_uniform_buffer_bindings, GL_INVALID_VALUE)

    d_glBindBufferBase_special(context, target, index, buffer);
}

void d_glBindTexture(void *context, GLenum target, GLuint texture) {
    CHECK_VALID_ENUM(__LINE__, target, 9, GL_TEXTURE_EXTERNAL_OES,
                     GL_TEXTURE_2D, GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
                     GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_3D,
                     GL_TEXTURE_2D_ARRAY, GL_TEXTURE_CUBE_MAP,
                     GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_BUFFER)
    /*TODO GL_INVALID_OPERATION is generated if texture was previously created
     * with a target that doesn't match that of target*/
    d_glBindTexture_special(context, target, texture);
}

void d_glBindRenderbuffer(void *context, GLenum target, GLuint renderbuffer) {
    CHECK_VALID_ENUM(__LINE__, target, 1, GL_RENDERBUFFER)
    d_glBindRenderbuffer_special(context, target, renderbuffer);
}

void d_glBindSampler(void *context, GLuint unit, GLuint sampler) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, unit, max_combined_texture_image_units,
                           GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, unit, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
    // GL_INVALID_VALUE)
    CHECK_RESOURCE_ID(__LINE__, sampler, sampler, GL_INVALID_OPERATION)
    d_glBindSampler_special(context, unit, sampler);
}

// void d_glBindFramebuffer(void *context, GLenum target, GLuint framebuffer)
//{
//     CHECK_VALID_ENUM(__LINE__, target, 3, GL_DRAW_FRAMEBUFFER,
//     GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER); r_glBindFramebuffer(context,
//     target, framebuffer);
// }

void d_glBindProgramPipeline(void *context, GLuint pipeline) {
    if (pipeline != 0) {
        CHECK_RESOURCE_ID(__LINE__, program_pipeline, pipeline,
                          GL_INVALID_OPERATION)
    }
    r_glBindProgramPipeline(context, pipeline);
}

void d_glBindTransformFeedback(void *context, GLenum target,
                               GLuint feedback_id) {
    CHECK_VALID_ENUM(__LINE__, target, 1, GL_TRANSFORM_FEEDBACK)
    CHECK_TRANSFORM_FEEDBACK_ACTIVE_BUT_NOT_PAUSED(GL_INVALID_OPERATION)
    if (feedback_id != 0) {
        CHECK_RESOURCE_ID(__LINE__, transform_feedback, feedback_id,
                          GL_INVALID_OPERATION)
    }
    d_glBindTransformFeedback_special(context, target, feedback_id);
}

void d_glActiveTexture(void *context, GLenum texture) {
    static int static_max_image = 0;
    if (static_max_image == 0) {
        d_glGetIntegerv(context, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                        &static_max_image);
    }
    if (texture >= GL_TEXTURE0 && texture < GL_TEXTURE0 + static_max_image) {
        d_glActiveTexture_special(context, texture);
    } else {
        ALOGD("activeTexture error texture %x max size %d", texture,
              static_max_image);
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
}

void d_glAttachShader(void *context, GLuint program, GLuint shader) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE);
    CHECK_RESOURCE_ID_CONTINUELY(__LINE__, shader, shader, GL_INVALID_VALUE);
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    /*TODO GL_INVALID_OPERATION is generated if shader is not a shader object.*/
    /*TODO GL_INVALID_OPERATION is generated if shader is already attached to
     * program.*/
    /*TODO GL_INVALID_OPERATION is generated if a shader of the same type as
     * shader is already attached to program.*/
    d_glAttachShader_special(context, program, shader);
}

void d_glBlendColor(void *context, GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat alpha) {
    r_glBlendColor(context, red, green, blue, alpha);
}

void d_glBlendEquation(void *context, GLenum mode) {
    CHECK_VALID_ENUM(__LINE__, mode, 5, GL_FUNC_ADD, GL_FUNC_SUBTRACT,
                     GL_FUNC_REVERSE_SUBTRACT, GL_MAX, GL_MIN);
    r_glBlendEquation(context, mode);
}

void d_glBlendEquationSeparate(void *context, GLenum modeRGB,
                               GLenum modeAlpha) {
    CHECK_VALID_ENUM(__LINE__, modeRGB, 5, GL_FUNC_ADD, GL_FUNC_SUBTRACT,
                     GL_FUNC_REVERSE_SUBTRACT, GL_MAX, GL_MIN);
    CHECK_VALID_ENUM(__LINE__, modeAlpha, 5, GL_FUNC_ADD, GL_FUNC_SUBTRACT,
                     GL_FUNC_REVERSE_SUBTRACT, GL_MAX, GL_MIN);
    r_glBlendEquationSeparate(context, modeRGB, modeAlpha);
}

void d_glBlendFunc(void *context, GLenum sfactor, GLenum dfactor) {
    CHECK_VALID_ENUM(
        __LINE__, sfactor, 14, GL_ZERO, GL_ONE, GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    CHECK_VALID_ENUM(
        __LINE__, dfactor, 14, GL_ZERO, GL_ONE, GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
    r_glBlendFunc(context, sfactor, dfactor);
}

void d_glBlendFuncSeparate(void *context, GLenum sfactorRGB, GLenum dfactorRGB,
                           GLenum sfactorAlpha, GLenum dfactorAlpha) {
    CHECK_VALID_ENUM(
        __LINE__, sfactorRGB, 15, GL_ZERO, GL_ONE, GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE);
    CHECK_VALID_ENUM(
        __LINE__, dfactorRGB, 15, GL_ZERO, GL_ONE, GL_SRC_COLOR,
        GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR,
        GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE);
    r_glBlendFuncSeparate(context, sfactorRGB, dfactorRGB, sfactorAlpha,
                          dfactorAlpha);
}

void d_glClear(void *context, GLbitfield mask) {
    CHECK_BUFFER_BITS(mask, 3, GL_INVALID_VALUE, GL_COLOR_BUFFER_BIT,
                      GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT)
    r_glClear(context, mask);
}

void d_glClearColor(void *context, GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat alpha) {
    r_glClearColor(context, red, green, blue, alpha);
}

void d_glClearDepthf(void *context, GLfloat d) { r_glClearDepthf(context, d); }

void d_glClearStencil(void *context, GLint s) { r_glClearStencil(context, s); }

void d_glColorMask(void *context, GLboolean red, GLboolean green,
                   GLboolean blue, GLboolean alpha) {
    r_glColorMask(context, red, green, blue, alpha);
}

void d_glCompileShader(void *context, GLuint shader) {
    CHECK_RESOURCE_ID(__LINE__, shader, shader, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if shader is not a shader object.*/
#ifdef USE_LOCAL_COMPILER
    ((GLThreadContext *)context)->glsl_compiler->compile_shader(shader);
#endif
    (*((GLThreadContext *)context)->shader_map)[shader]->compile_status = GL_TRUE;
    r_glCompileShader(context, shader);
}

void d_glCullFace(void *context, GLenum mode) {
    CHECK_VALID_ENUM(__LINE__, mode, 3, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK);
    r_glCullFace(context, mode);
}

void d_glDepthFunc(void *context, GLenum func) {
    CHECK_VALID_ENUM(__LINE__, func, 8, GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL,
                     GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS);
    r_glDepthFunc(context, func);
}

void d_glDepthMask(void *context, GLboolean flag) {
    r_glDepthMask(context, flag);
}

void d_glDepthRangef(void *context, GLfloat n, GLfloat f) {
    r_glDepthRangef(context, n, f);
}

void d_glDetachShader(void *context, GLuint program, GLuint shader) {
    CHECK_RESOURCE_ID(__LINE__, shader, shader, GL_INVALID_VALUE);
    CHECK_RESOURCE_ID_CONTINUELY(__LINE__, program, program,
                                 GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if shader is not a shader object.*/

    d_glDetachShader_special(context, program, shader);
}

void d_glDisable(void *context, GLenum cap) {
    CHECK_VALID_ENUM(__LINE__, cap, 15, GL_TEXTURE_2D, GL_BLEND, GL_CULL_FACE,
                     GL_DEPTH_TEST, GL_DITHER, GL_POLYGON_OFFSET_FILL,
                     GL_PRIMITIVE_RESTART_FIXED_INDEX, GL_RASTERIZER_DISCARD,
                     GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_COVERAGE,
                     GL_SAMPLE_MASK, GL_SCISSOR_TEST, GL_STENCIL_TEST,
                     GL_DEBUG_OUTPUT, GL_DEBUG_OUTPUT_SYNCHRONOUS)
    d_glDisable_special(context, cap);
}

void d_glEnable(void *context, GLenum cap) {
    CHECK_VALID_ENUM(__LINE__, cap, 15, GL_TEXTURE_2D, GL_BLEND, GL_CULL_FACE,
                     GL_DEPTH_TEST, GL_DITHER, GL_POLYGON_OFFSET_FILL,
                     GL_PRIMITIVE_RESTART_FIXED_INDEX, GL_RASTERIZER_DISCARD,
                     GL_SAMPLE_ALPHA_TO_COVERAGE, GL_SAMPLE_COVERAGE,
                     GL_SAMPLE_MASK, GL_SCISSOR_TEST, GL_STENCIL_TEST,
                     GL_DEBUG_OUTPUT, GL_DEBUG_OUTPUT_SYNCHRONOUS)
    d_glEnable_special(context, cap);
}

void d_glFramebufferRenderbuffer(void *context, GLenum target,
                                 GLenum attachment, GLenum renderbuffertarget,
                                 GLuint renderbuffer) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_DRAW_FRAMEBUFFER,
                     GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER);
    CHECK_VALID_ENUM(__LINE__, renderbuffertarget, 1, GL_RENDERBUFFER);
    GLint fbo;
    GLenum tar = target == GL_READ_FRAMEBUFFER ? GL_READ_FRAMEBUFFER_BINDING
                                               : GL_DRAW_FRAMEBUFFER_BINDING;
    d_glGetIntegerv_special(context, tar, &fbo);
    CHECK_ZERO(__LINE__, fbo, GL_INVALID_OPERATION)
    d_glFramebufferRenderbuffer_special(context, target, attachment,
                                        renderbuffertarget, renderbuffer);
}

void d_glFramebufferTexture2D(void *context, GLenum target, GLenum attachment,
                              GLenum textarget, GLuint texture, GLint level) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_DRAW_FRAMEBUFFER,
                     GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER);
    if (attachment != GL_DEPTH_ATTACHMENT &&
        attachment != GL_STENCIL_ATTACHMENT &&
        attachment != GL_DEPTH_STENCIL_ATTACHMENT) {
        StaticContextValues *static_values =
            (((GLThreadContext *)context)->static_values);
        GLint max_color_attachments = static_values->max_color_attachments;
        if (attachment < GL_COLOR_ATTACHMENT0 ||
            attachment > GL_COLOR_ATTACHMENT0 + max_color_attachments) {
            set_gl_error(context, GL_INVALID_ENUM);
        }

        // int check = attachment - GL_COLOR_ATTACHMENT0;
        // CHECK_NEGATIVE(__LINE__, check, GL_INVALID_ENUM)
        // CHECK_INT_MAX(__LINE__, check, GL_MAX_COLOR_ATTACHMENTS,
        // GL_INVALID_ENUM)
    }
    GLint fbo;

    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);

    fbo = target == GL_READ_FRAMEBUFFER ? bind_status->read_frame_buffer
                                        : bind_status->draw_frame_buffer;

    // d_glGetIntegerv(context, tar, &fbo);
    CHECK_ZERO(__LINE__, fbo, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if textarget and texture are not
     * compatible*/

    d_glFramebufferTexture2D_special(context, target, attachment, textarget,
                                     texture, level);
}

void d_glFrontFace(void *context, GLenum mode) {
    CHECK_VALID_ENUM(__LINE__, mode, 2, GL_CW, GL_CCW);
    r_glFrontFace(context, mode);
}

void d_glGenerateMipmap(void *context, GLenum target) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(__LINE__, target, 6, GL_TEXTURE_2D,
                     GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_3D,
                     GL_TEXTURE_2D_ARRAY, GL_TEXTURE_CUBE_MAP,
                     GL_TEXTURE_CUBE_MAP_ARRAY);
    /*TODO GL_INVALID_OPERATION is generated if target is GL_TEXTURE_CUBE_MAP or
     * GL_TEXTURE_CUBE_MAP_ARRAY and the texture bound to target is not cube or
     * cube array complete.*/
    /*TODO GL_INVALID_OPERATION is generated if the level base array is stored
     * in a compressed internal format.*/
    /*TODO GL_INVALID_OPERATION is generated if the level base array was not
     * specified with an unsized internal format or a sized internal format that
     * is both color-renderable and texture-filterable.*/

    r_glGenerateMipmap(context, target);
}

void d_glHint(void *context, GLenum target, GLenum mode) {
    CHECK_VALID_ENUM(__LINE__, target, 2, GL_FRAGMENT_SHADER_DERIVATIVE_HINT,
                     GL_GENERATE_MIPMAP_HINT)
    CHECK_VALID_ENUM(__LINE__, mode, 3, GL_FASTEST, GL_NICEST, GL_DONT_CARE)
    d_glHint_special(context, target, mode);
}

void d_glLineWidth(void *context, GLfloat width) {
    CHECK_ZERO(__LINE__, width, GL_INVALID_VALUE)
    CHECK_NEGATIVE(__LINE__, width, GL_INVALID_VALUE)
    r_glLineWidth(context, width);
}

void d_glPolygonOffset(void *context, GLfloat factor, GLfloat units) {
    r_glPolygonOffset(context, factor, units);
}

void d_glReleaseShaderCompiler(void *context) {
    r_glReleaseShaderCompiler(context);
}

void d_glRenderbufferStorage(void *context, GLenum target,
                             GLenum internalformat, GLsizei width,
                             GLsizei height) {
    CHECK_VALID_ENUM(__LINE__, target, 1, GL_RENDERBUFFER)
    CHECK_NEGATIVE(__LINE__, width < 0 || height, GL_INVALID_VALUE)
    GLint max;
    d_glGetIntegerv_special(context, GL_MAX_RENDERBUFFER_SIZE, &max);
    if (width > max || height > max) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    CHECK_VALID_ENUM(
        __LINE__, internalformat, 37, GL_R8, GL_R8UI, GL_R8I, GL_R16UI, GL_R16I,
        GL_R32UI, GL_R32I, GL_RG8, GL_RG8UI, GL_RG8I, GL_RG16UI, GL_RG16I,
        GL_RG32UI, GL_RG32UI, GL_RGBA16F, GL_RGB16F, GL_RG32I, GL_RGB8,
        GL_RGB565, GL_RGBA8, GL_SRGB8_ALPHA8, GL_RGB5_A1, GL_RGBA4, GL_RGB10_A2,
        GL_RGBA8UI, GL_RGBA8I, GL_RGB10_A2UI, GL_RGBA16UI, GL_RGBA16I,
        GL_RGBA32I, GL_RGBA32UI, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT32F, GL_DEPTH24_STENCIL8, GL_DEPTH32F_STENCIL8,
        GL_STENCIL_INDEX8)
    /*TODO GL_OUT_OF_MEMORY is generated if the GL is unable to create a data
     * store of the requested size.*/
    d_glRenderbufferStorage_special(context, target, internalformat, width,
                                    height);
}

void d_glSampleCoverage(void *context, GLfloat value, GLboolean invert) {
    r_glSampleCoverage(context, value, invert);
}

void d_glScissor(void *context, GLint x, GLint y, GLsizei width,
                 GLsizei height) {
    CHECK_NEGATIVE(__LINE__, width < 0 || height, GL_INVALID_VALUE)
    ALOGD("glScissor %d %d %d %d", x, y, (int)width, (int)height);
    r_glScissor(context, x, y, width, height);
}

void d_glStencilFunc(void *context, GLenum func, GLint ref, GLuint mask) {
    CHECK_VALID_ENUM(__LINE__, func, 8, GL_NEVER, GL_LESS, GL_LEQUAL,
                     GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS)
    r_glStencilFunc(context, func, ref, mask);
}

void d_glStencilFuncSeparate(void *context, GLenum face, GLenum func, GLint ref,
                             GLuint mask) {
    CHECK_VALID_ENUM(__LINE__, func, 8, GL_NEVER, GL_LESS, GL_LEQUAL,
                     GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS)
    r_glStencilFuncSeparate(context, face, func, ref, mask);
}

void d_glStencilMask(void *context, GLuint mask) {
    r_glStencilMask(context, mask);
}

void d_glStencilMaskSeparate(void *context, GLenum face, GLuint mask) {
    CHECK_VALID_ENUM(__LINE__, face, 3, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK)
    r_glStencilMaskSeparate(context, face, mask);
}

void d_glStencilOp(void *context, GLenum fail, GLenum zfail, GLenum zpass) {
    CHECK_VALID_ENUM(__LINE__, fail, 8, GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR,
                     GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT)
    CHECK_VALID_ENUM(__LINE__, zfail, 8, GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR,
                     GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT)
    CHECK_VALID_ENUM(__LINE__, zpass, 8, GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR,
                     GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT)
    r_glStencilOp(context, fail, zfail, zpass);
}

void d_glStencilOpSeparate(void *context, GLenum face, GLenum sfail,
                           GLenum dpfail, GLenum dppass) {
    CHECK_VALID_ENUM(__LINE__, face, 3, GL_FRONT, GL_BACK, GL_FRONT_AND_BACK)
    CHECK_VALID_ENUM(__LINE__, sfail, 8, GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR,
                     GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT)
    CHECK_VALID_ENUM(__LINE__, dpfail, 8, GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR,
                     GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT)
    CHECK_VALID_ENUM(__LINE__, dppass, 8, GL_KEEP, GL_ZERO, GL_REPLACE, GL_INCR,
                     GL_INCR_WRAP, GL_DECR, GL_DECR_WRAP, GL_INVERT)
    r_glStencilOpSeparate(context, face, sfail, dpfail, dppass);
}

void d_glTexParameterf(void *context, GLenum target, GLenum pname,
                       GLfloat param) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, pname, 16, GL_DEPTH_STENCIL_TEXTURE_MODE,
        GL_TEXTURE_BASE_LEVEL, GL_TEXTURE_COMPARE_FUNC, GL_TEXTURE_COMPARE_MODE,
        GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_LOD,
        GL_TEXTURE_MAX_LOD, GL_TEXTURE_MAX_LEVEL, GL_TEXTURE_SWIZZLE_R,
        GL_TEXTURE_SWIZZLE_G, GL_TEXTURE_SWIZZLE_B, GL_TEXTURE_SWIZZLE_A,
        GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R)
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_TEXTURE_2D,
                     GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_3D,
                     GL_TEXTURE_2D_ARRAY, GL_TEXTURE_2D_MULTISAMPLE,
                     GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_CUBE_MAP,
                     GL_TEXTURE_CUBE_MAP_ARRAY)
    switch (pname) {
        case GL_DEPTH_STENCIL_TEXTURE_MODE:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_DEPTH_COMPONENT,
                             GL_STENCIL_INDEX)
            break;
        case GL_TEXTURE_COMPARE_FUNC:
            CHECK_VALID_ENUM(__LINE__, param, 8, GL_LEQUAL, GL_GEQUAL, GL_LESS,
                             GL_GREATER, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS,
                             GL_NEVER)
            break;
        case GL_TEXTURE_COMPARE_MODE:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_COMPARE_REF_TO_TEXTURE,
                             GL_NONE)
            break;
        case GL_TEXTURE_MIN_FILTER:
            CHECK_VALID_ENUM(__LINE__, param, 6, GL_NEAREST, GL_LINEAR,
                             GL_NEAREST_MIPMAP_NEAREST,
                             GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
                             GL_LINEAR_MIPMAP_LINEAR)
            break;
        case GL_TEXTURE_MAG_FILTER:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_NEAREST, GL_LINEAR)
            break;
        case GL_TEXTURE_SWIZZLE_R:
        case GL_TEXTURE_SWIZZLE_G:
        case GL_TEXTURE_SWIZZLE_B:
        case GL_TEXTURE_SWIZZLE_A:
            CHECK_VALID_ENUM(__LINE__, param, 6, GL_RED, GL_GREEN, GL_BLUE,
                             GL_ALPHA, GL_ZERO, GL_ONE)
            break;
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
            CHECK_VALID_ENUM(__LINE__, param, 3, GL_CLAMP_TO_EDGE,
                             GL_MIRRORED_REPEAT, GL_REPEAT)
            break;
    }
    if (pname == GL_TEXTURE_BASE_LEVEL || pname == GL_TEXTURE_MAX_LEVEL) {
        CHECK_NEGATIVE(__LINE__, param, GL_INVALID_VALUE)
    }
    if ((target == GL_TEXTURE_2D_MULTISAMPLE ||
         target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY) &&
        pname == GL_TEXTURE_BASE_LEVEL && param != 0) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }

    if ((target == GL_TEXTURE_2D || target == GL_TEXTURE_EXTERNAL_OES) &&
        !tex_parameter_should_set(context, target, pname, param)) {
        return;
    }

    r_glTexParameterf(context, target, pname, param);
}

void d_glTexParameteri(void *context, GLenum target, GLenum pname,
                       GLint param) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, pname, 16, GL_DEPTH_STENCIL_TEXTURE_MODE,
        GL_TEXTURE_BASE_LEVEL, GL_TEXTURE_COMPARE_FUNC, GL_TEXTURE_COMPARE_MODE,
        GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_LOD,
        GL_TEXTURE_MAX_LOD, GL_TEXTURE_MAX_LEVEL, GL_TEXTURE_SWIZZLE_R,
        GL_TEXTURE_SWIZZLE_G, GL_TEXTURE_SWIZZLE_B, GL_TEXTURE_SWIZZLE_A,
        GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R)
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_TEXTURE_2D,
                     GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_3D,
                     GL_TEXTURE_2D_ARRAY, GL_TEXTURE_2D_MULTISAMPLE,
                     GL_TEXTURE_2D_MULTISAMPLE_ARRAY, GL_TEXTURE_CUBE_MAP,
                     GL_TEXTURE_CUBE_MAP_ARRAY)
    switch (pname) {
        case GL_DEPTH_STENCIL_TEXTURE_MODE:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_DEPTH_COMPONENT,
                             GL_STENCIL_INDEX)
            break;
        case GL_TEXTURE_COMPARE_FUNC:
            CHECK_VALID_ENUM(__LINE__, param, 8, GL_LEQUAL, GL_GEQUAL, GL_LESS,
                             GL_GREATER, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS,
                             GL_NEVER)
            break;
        case GL_TEXTURE_COMPARE_MODE:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_COMPARE_REF_TO_TEXTURE,
                             GL_NONE)
            break;
        case GL_TEXTURE_MIN_FILTER:
            CHECK_VALID_ENUM(__LINE__, param, 6, GL_NEAREST, GL_LINEAR,
                             GL_NEAREST_MIPMAP_NEAREST,
                             GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
                             GL_LINEAR_MIPMAP_LINEAR)
            break;
        case GL_TEXTURE_MAG_FILTER:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_NEAREST, GL_LINEAR)
            break;
        case GL_TEXTURE_SWIZZLE_R:
        case GL_TEXTURE_SWIZZLE_G:
        case GL_TEXTURE_SWIZZLE_B:
        case GL_TEXTURE_SWIZZLE_A:
            CHECK_VALID_ENUM(__LINE__, param, 6, GL_RED, GL_GREEN, GL_BLUE,
                             GL_ALPHA, GL_ZERO, GL_ONE)
            break;
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
            CHECK_VALID_ENUM(__LINE__, param, 3, GL_CLAMP_TO_EDGE,
                             GL_MIRRORED_REPEAT, GL_REPEAT)
            break;
    }
    if (pname == GL_TEXTURE_BASE_LEVEL || pname == GL_TEXTURE_MAX_LEVEL) {
        CHECK_NEGATIVE(__LINE__, param, GL_INVALID_VALUE)
    }
    if ((target == GL_TEXTURE_2D_MULTISAMPLE ||
         target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY) &&
        pname == GL_TEXTURE_BASE_LEVEL && param != 0) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    ALOGD("glTexParameteri target %x pname %x param %d", target, pname, param);

    if ((target == GL_TEXTURE_2D || target == GL_TEXTURE_EXTERNAL_OES) &&
        !tex_parameter_should_set(context, target, pname, param)) {
        return;
    }
    r_glTexParameteri(context, target, pname, param);
}

void d_glUniform1f(void *context, GLint location, GLfloat v0) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)

    // GLint current_pro;
    // d_glGetIntegerv(context, GL_CURRENT_PROGRAM, &current_pro);
    // CHECK_ZERO(__LINE__, current_pro, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/
    GLfloat value[1];
    value[0] = v0;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 1)) {
        return;
    }

    r_glUniform1f(context, location, v0);
}

void d_glUniform1i(void *context, GLint location, GLint v0) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/
    // GLSLProgram *now_program = ((GLThreadContext
    // *)context)->program_info_map[current_program]; char *name=NULL; if
    // (now_program != NULL)
    // {
    //     for(int i=0;i<now_program->uniform_gl_num;i++)
    //     {
    //         name = now_program->uniform_index_name[i];
    //         if(now_program->uniform_name_location[name]==location)
    //         {
    //             ALOGD("glUniform1i %s v0:%d", name, v0);
    //             break;
    //         }
    //     }
    // }

    // ALOGD("glUniform1i loc %d %d",location, v0);

    GLint value[1];
    value[0] = v0;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLint) * 1)) {
        return;
    }

    r_glUniform1i(context, location, v0);
}

void d_glUniform2f(void *context, GLint location, GLfloat v0, GLfloat v1) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLfloat value[2];
    value[0] = v0;
    value[1] = v1;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 2)) {
        return;
    }

    r_glUniform2f(context, location, v0, v1);
}

void d_glUniform2i(void *context, GLint location, GLint v0, GLint v1) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLint value[2];
    value[0] = v0;
    value[1] = v1;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLint) * 2)) {
        return;
    }

    r_glUniform2i(context, location, v0, v1);
}

void d_glUniform3f(void *context, GLint location, GLfloat v0, GLfloat v1,
                   GLfloat v2) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLfloat value[3];
    value[0] = v0;
    value[1] = v1;
    value[2] = v2;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 3)) {
        return;
    }

    r_glUniform3f(context, location, v0, v1, v2);
}

void d_glUniform3i(void *context, GLint location, GLint v0, GLint v1,
                   GLint v2) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLint value[3];
    value[0] = v0;
    value[1] = v1;
    value[2] = v2;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLint) * 3)) {
        return;
    }

    r_glUniform3i(context, location, v0, v1, v2);
}

void d_glUniform4f(void *context, GLint location, GLfloat v0, GLfloat v1,
                   GLfloat v2, GLfloat v3) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLfloat value[4];
    value[0] = v0;
    value[1] = v1;
    value[2] = v2;
    value[3] = v3;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 4)) {
        return;
    }

    r_glUniform4f(context, location, v0, v1, v2, v3);
}

void d_glUniform4i(void *context, GLint location, GLint v0, GLint v1, GLint v2,
                   GLint v3) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLint value[4];
    value[0] = v0;
    value[1] = v1;
    value[2] = v2;
    value[3] = v3;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLint) * 4)) {
        return;
    }

    r_glUniform4i(context, location, v0, v1, v2, v3);
}

void d_glUseProgram(void *context, GLuint program) {
    ResourceContext *res_ctx =
        &(((GLThreadContext *)context)->resource_context);
    if (program != 0) {
        CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    }
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    /*TODO GL_INVALID_OPERATION is generated if program could not be made part
     * of current state.*/

    CHECK_TRANSFORM_FEEDBACK_ACTIVE_BUT_NOT_PAUSED(GL_INVALID_OPERATION)
    d_glUseProgram_special(context, program);
}

void d_glValidateProgram(void *context, GLuint program) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/
    r_glValidateProgram(context, program);
}

void d_glVertexAttrib1f(void *context, GLuint index, GLfloat x) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttrib1f(context, index, x);
}

void d_glVertexAttrib2f(void *context, GLuint index, GLfloat x, GLfloat y) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttrib2f(context, index, x, y);
}

void d_glVertexAttrib3f(void *context, GLuint index, GLfloat x, GLfloat y,
                        GLfloat z) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttrib3f(context, index, x, y, z);
}

void d_glVertexAttrib4f(void *context, GLuint index, GLfloat x, GLfloat y,
                        GLfloat z, GLfloat w) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttrib4f(context, index, x, y, z, w);
}

void d_glBlitFramebuffer(void *context, GLint srcX0, GLint srcY0, GLint srcX1,
                         GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1,
                         GLint dstY1, GLbitfield mask, GLenum filter) {
    CHECK_BUFFER_BITS(mask, 3, GL_INVALID_VALUE, GL_COLOR_BUFFER_BIT,
                      GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT)
    CHECK_VALID_ENUM(__LINE__, filter, 2, GL_LINEAR, GL_NEAREST);
    if (filter != GL_NEAREST &&
        (mask & GL_DEPTH_BUFFER_BIT || mask & GL_STENCIL_BUFFER_BIT)) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    /*TODO GL_INVALID_OPERATION is generated if the source and destination
     * buffers are identical*/
    /*TODO GL_INVALID_FRAMEBUFFER_OPERATION is generated if the objects bound to
     * GL_DRAW_FRAMEBUFFER_BINDING or GL_READ_FRAMEBUFFER_BINDING are not
     * framebuffer complete*/
    /*TODO GL_INVALID_OPERATION is generated if mask contains
     * GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT and the source and
     * destination depth and stencil formats do not match*/
    /*TODO GL_INVALID_OPERATION is generated if filter is GL_LINEAR and the read
     * buffer contains integer data.*/
    /*TODO GL_INVALID_OPERATION is generated if GL_SAMPLE_BUFFERS for the read
     * buffer is greater than zero and the formats of draw and read buffers are
     * not identical, or the source and destination rectangles are not defined
     * with the same (X0, Y0) and (X1, Y1) bounds.*/
    //    GLint sample_bufer;
    //    glGetIntegerv(GL_SAMPLE_BUFFERS,&sample_bufer);
    //    if(sample_bufer>0){set_gl_error(context, GL_INVALID_OPERATION);
    //    return;}
    if (mask & GL_COLOR_BUFFER_BIT) {
        /*TODO GL_INVALID_OPERATION is generated if mask contains
        GL_COLOR_BUFFER_BIT and any of the following conditions hold: The read
        buffer contains fixed-point or floating-point values and any draw buffer
        contains neither fixed-point nor floating-point values. The read buffer
        contains unsigned integer values and any draw buffer does not contain
        unsigned integer values. The read buffer contains signed integer values
        and any draw buffer does not contain signed integer values.
     */
    }
    r_glBlitFramebuffer(context, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0,
                        dstX1, dstY1, mask, filter);
}

void d_glRenderbufferStorageMultisample(void *context, GLenum target,
                                        GLsizei samples, GLenum internalformat,
                                        GLsizei width, GLsizei height) {
    CHECK_VALID_ENUM(__LINE__, target, 1, GL_RENDERBUFFER)
    /*TODO GL_INVALID_OPERATION is generated if samples is greater than the
     * maximum number of samples supported for internalformat.*/
    CHECK_VALID_ENUM(
        __LINE__, internalformat, 36, GL_R8, GL_R8UI, GL_R8I, GL_R16UI, GL_R16I,
        GL_R32UI, GL_R32I, GL_RG8, GL_RG8UI, GL_RG8I, GL_RG16UI, GL_RG16I,
        GL_RG32UI, GL_RGBA16F, GL_RGB16F, GL_RG32I, GL_RGB8, GL_RGB565,
        GL_RGBA8, GL_SRGB8_ALPHA8, GL_RGB5_A1, GL_RGBA4, GL_RGB10_A2,
        GL_RGBA8UI, GL_RGBA8I, GL_RGB10_A2UI, GL_RGBA16UI, GL_RGBA16I,
        GL_RGBA32I, GL_RGBA32UI, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT32F, GL_DEPTH24_STENCIL8, GL_DEPTH32F_STENCIL8,
        GL_STENCIL_INDEX8);
    if (samples > 0) {
        CHECK_VALID_ENUM_WITH_ERROR(
            __LINE__, internalformat, 17, GL_INVALID_OPERATION, GL_R8, GL_RG8,
            GL_RGB8, GL_RGB565, GL_RGBA8, GL_SRGB8_ALPHA8, GL_RGBA16F,
            GL_RGB16F, GL_RGB5_A1, GL_RGBA4, GL_RGB10_A2, GL_DEPTH_COMPONENT16,
            GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32F, GL_DEPTH24_STENCIL8,
            GL_DEPTH32F_STENCIL8, GL_STENCIL_INDEX8)
    }
    CHECK_NEGATIVE(__LINE__, width < 0 || height, GL_INVALID_VALUE)
    GLint max;
    d_glGetIntegerv_special(context, GL_MAX_RENDERBUFFER_SIZE, &max);
    if (width > max || height > max) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    /*TODO GL_OUT_OF_MEMORY is generated if the GL is unable to create a data
     * store of the requested size.*/
    d_glRenderbufferStorageMultisample_special(context, target, samples,
                                               internalformat, width, height);
}

void d_glFramebufferTextureLayer(void *context, GLenum target,
                                 GLenum attachment, GLuint texture, GLint level,
                                 GLint layer) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_DRAW_FRAMEBUFFER,
                     GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER);
    if (attachment != GL_DEPTH_ATTACHMENT &&
        attachment != GL_STENCIL_ATTACHMENT &&
        attachment != GL_DEPTH_STENCIL_ATTACHMENT) {
        int check = attachment - GL_COLOR_ATTACHMENT0;
        CHECK_NEGATIVE(__LINE__, check, GL_INVALID_ENUM)
        CHECK_INT_MAX(__LINE__, check, GL_MAX_COLOR_ATTACHMENTS,
                      GL_INVALID_ENUM)
    }
    if (texture != 0) {
        CHECK_RESOURCE_ID(__LINE__, texture, texture, GL_INVALID_VALUE);
        CHECK_NEGATIVE(__LINE__, level, GL_INVALID_VALUE)
        /*TODO layer is greater than the value of GL_MAX_3D_TEXTURE_SIZE minus
         * one for a 3D texture or greater than the value of
         * GL_MAX_ARRAY_TEXTURE_LAYERS minus one for a 2D array or 2D
         * multisample array texture.*/
        /*TODO GL_INVALID_OPERATION is generated if texture is not zero or the
         * name of an existing three-dimensional, two-dimensional array, cube
         * map array, or two-dimensional multisample array texture.*/
    }
    GLint fbo;
    GLenum tar = target == GL_READ_FRAMEBUFFER ? GL_READ_FRAMEBUFFER_BINDING
                                               : GL_DRAW_FRAMEBUFFER_BINDING;
    d_glGetIntegerv_special(context, tar, &fbo);
    CHECK_ZERO(__LINE__, fbo, GL_INVALID_OPERATION)
    d_glFramebufferTextureLayer_special(context, target, attachment, texture,
                                        level, layer);
}

void d_glVertexAttribI4i(void *context, GLuint index, GLint x, GLint y, GLint z,
                         GLint w) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttribI4i(context, index, x, y, z, w);
}

void d_glVertexAttribI4ui(void *context, GLuint index, GLuint x, GLuint y,
                          GLuint z, GLuint w) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttribI4ui(context, index, x, y, z, w);
}

void d_glUniform1ui(void *context, GLint location, GLuint v0) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLuint value[1];
    value[0] = v0;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLuint) * 1)) {
        return;
    }

    r_glUniform1ui(context, location, v0);
}

void d_glUniform2ui(void *context, GLint location, GLuint v0, GLuint v1) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLuint value[2];
    value[0] = v0;
    value[1] = v1;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLuint) * 2)) {
        return;
    }

    r_glUniform2ui(context, location, v0, v1);
}

void d_glUniform3ui(void *context, GLint location, GLuint v0, GLuint v1,
                    GLuint v2) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLuint value[3];
    value[0] = v0;
    value[1] = v1;
    value[2] = v2;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLuint) * 3)) {
        return;
    }

    r_glUniform3ui(context, location, v0, v1, v2);
}

void d_glUniform4ui(void *context, GLint location, GLuint v0, GLuint v1,
                    GLuint v2, GLuint v3) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    GLuint value[4];
    value[0] = v0;
    value[1] = v1;
    value[2] = v2;
    value[3] = v3;
    if (!GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLuint) * 4)) {
        return;
    }

    r_glUniform4ui(context, location, v0, v1, v2, v3);
}

void d_glClearBufferfi(void *context, GLenum buffer, GLint drawbuffer,
                       GLfloat depth, GLint stencil) {
    CHECK_VALID_ENUM(__LINE__, buffer, 1, GL_DEPTH_STENCIL);
    if (drawbuffer != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    r_glClearBufferfi(context, buffer, drawbuffer, depth, stencil);
}

void d_glCopyBufferSubData(void *context, GLenum readTarget, GLenum writeTarget,
                           GLintptr readOffset, GLintptr writeOffset,
                           GLsizeiptr size) {
    CHECK_NEGATIVE(__LINE__, writeOffset < 0 || readOffset < 0 || size,
                   GL_INVALID_VALUE)
    /*TODO GL_INVALID_VALUE is generated if readoffset + size exceeds the size
     * of the buffer object bound to readtarget or if writeoffset + size exceeds
     * the size of the buffer object bound to writetarget.*/
    /*TODO GL_INVALID_VALUE is generated if the same buffer object is bound to
     * both readtarget and writetarget and the ranges [readoffset, readoffset +
     * size) and [writeoffset, writeoffset + size) overlap.*/
    for (int i = 0; i < 2; i++) {
        GLint buffer_binding;
        buffer_binding =
            get_bound_buffer(context, i == 0 ? readTarget : writeTarget);
        CHECK_ZERO(__LINE__, buffer_binding, GL_INVALID_OPERATION)
    }

    /*TODO GL_INVALID_OPERATION is generated if the buffer object bound to
     * either readtarget or writetarget is mapped.*/
    r_glCopyBufferSubData(context, readTarget, writeTarget, readOffset,
                          writeOffset, size);
}

void d_glUniformBlockBinding(void *context, GLuint program,
                             GLuint uniformBlockIndex,
                             GLuint uniformBlockBinding) {
    static GLint max_uniform_bindings = 0;
    if (max_uniform_bindings == 0) {
        d_glGetIntegerv_special(context, GL_MAX_UNIFORM_BUFFER_BINDINGS,
                                &max_uniform_bindings);
    }
    if (unlikely(uniformBlockBinding >= max_uniform_bindings)) {
        set_gl_error(context, GL_INVALID_VALUE);
    }
    d_glUniformBlockBinding_special(context, program, uniformBlockIndex,
                                    uniformBlockBinding);
}

void d_glSamplerParameteri(void *context, GLuint sampler, GLenum pname,
                           GLint param) {
    CHECK_RESOURCE_ID(__LINE__, sampler, sampler, GL_INVALID_OPERATION)
    if (pname == GL_TEXTURE_BORDER_COLOR) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    switch (pname) {
        case GL_TEXTURE_MIN_FILTER:
            CHECK_VALID_ENUM(__LINE__, param, 6, GL_NEAREST, GL_LINEAR,
                             GL_NEAREST_MIPMAP_NEAREST,
                             GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
                             GL_LINEAR_MIPMAP_LINEAR)
            break;
        case GL_TEXTURE_MAG_FILTER:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_NEAREST, GL_LINEAR)
            break;
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
            CHECK_VALID_ENUM(__LINE__, param, 3, GL_CLAMP_TO_EDGE,
                             GL_MIRRORED_REPEAT, GL_REPEAT)
            break;
        case GL_TEXTURE_COMPARE_MODE:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_COMPARE_REF_TO_TEXTURE,
                             GL_NONE)
            break;
        case GL_TEXTURE_COMPARE_FUNC:
            CHECK_VALID_ENUM(__LINE__, param, 8, GL_LEQUAL, GL_GEQUAL, GL_LESS,
                             GL_GREATER, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS,
                             GL_NEVER)
            break;
    }
    r_glSamplerParameteri(context, sampler, pname, param);
}

void d_glSamplerParameterf(void *context, GLuint sampler, GLenum pname,
                           GLfloat param) {
    CHECK_RESOURCE_ID(__LINE__, sampler, sampler, GL_INVALID_OPERATION)
    if (pname == GL_TEXTURE_BORDER_COLOR) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }
    switch (pname) {
        case GL_TEXTURE_MIN_FILTER:
            CHECK_VALID_ENUM(__LINE__, param, 6, GL_NEAREST, GL_LINEAR,
                             GL_NEAREST_MIPMAP_NEAREST,
                             GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
                             GL_LINEAR_MIPMAP_LINEAR)
            break;
        case GL_TEXTURE_MAG_FILTER:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_NEAREST, GL_LINEAR)
            break;
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
            CHECK_VALID_ENUM(__LINE__, param, 3, GL_CLAMP_TO_EDGE,
                             GL_MIRRORED_REPEAT, GL_REPEAT)
            break;
        case GL_TEXTURE_COMPARE_MODE:
            CHECK_VALID_ENUM(__LINE__, param, 2, GL_COMPARE_REF_TO_TEXTURE,
                             GL_NONE)
            break;
        case GL_TEXTURE_COMPARE_FUNC:
            CHECK_VALID_ENUM(__LINE__, param, 8, GL_LEQUAL, GL_GEQUAL, GL_LESS,
                             GL_GREATER, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS,
                             GL_NEVER)
            break;
    }
    r_glSamplerParameterf(context, sampler, pname, param);
}

void d_glProgramParameteri(void *context, GLuint program, GLenum pname,
                           GLint value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    CHECK_VALID_ENUM(__LINE__, pname, 2, GL_PROGRAM_BINARY_RETRIEVABLE_HINT,
                     GL_PROGRAM_SEPARABLE)
    CHECK_VALID_ENUM_WITH_ERROR(__LINE__, value, 2, GL_INVALID_VALUE, GL_TRUE,
                                GL_FALSE)
    r_glProgramParameteri(context, program, pname, value);
}

void d_glAlphaFuncxOES(void *context, GLenum func, GLfixed ref) {
    r_glAlphaFuncxOES(context, func, ref);
}

void d_glClearColorxOES(void *context, GLfixed red, GLfixed green, GLfixed blue,
                        GLfixed alpha) {
    r_glClearColorxOES(context, red, green, blue, alpha);
}

void d_glClearDepthxOES(void *context, GLfixed depth) {
    r_glClearDepthxOES(context, depth);
}

void d_glColor4xOES(void *context, GLfixed red, GLfixed green, GLfixed blue,
                    GLfixed alpha) {
    r_glColor4xOES(context, red, green, blue, alpha);
}

void d_glDepthRangexOES(void *context, GLfixed n, GLfixed f) {
    r_glDepthRangexOES(context, n, f);
}

void d_glFogxOES(void *context, GLenum pname, GLfixed param) {
    r_glFogxOES(context, pname, param);
}

void d_glFrustumxOES(void *context, GLfixed l, GLfixed r, GLfixed b, GLfixed t,
                     GLfixed n, GLfixed f) {
    r_glFrustumxOES(context, l, r, b, t, n, f);
}

void d_glLightModelxOES(void *context, GLenum pname, GLfixed param) {
    r_glLightModelxOES(context, pname, param);
}

void d_glLightxOES(void *context, GLenum light, GLenum pname, GLfixed param) {
    r_glLightxOES(context, light, pname, param);
}

void d_glLineWidthxOES(void *context, GLfixed width) {
    r_glLineWidthxOES(context, width);
}

void d_glMaterialxOES(void *context, GLenum face, GLenum pname, GLfixed param) {
    r_glMaterialxOES(context, face, pname, param);
}

void d_glMultiTexCoord4xOES(void *context, GLenum texture, GLfixed s, GLfixed t,
                            GLfixed r, GLfixed q) {
    r_glMultiTexCoord4xOES(context, texture, s, t, r, q);
}

void d_glNormal3xOES(void *context, GLfixed nx, GLfixed ny, GLfixed nz) {
    r_glNormal3xOES(context, nx, ny, nz);
}

void d_glOrthoxOES(void *context, GLfixed l, GLfixed r, GLfixed b, GLfixed t,
                   GLfixed n, GLfixed f) {
    r_glOrthoxOES(context, l, r, b, t, n, f);
}

void d_glPointSizexOES(void *context, GLfixed size) {
    r_glPointSizexOES(context, size);
}

void d_glPolygonOffsetxOES(void *context, GLfixed factor, GLfixed units) {
    r_glPolygonOffsetxOES(context, factor, units);
}

void d_glRotatexOES(void *context, GLfixed angle, GLfixed x, GLfixed y,
                    GLfixed z) {
    r_glRotatexOES(context, angle, x, y, z);
}

void d_glScalexOES(void *context, GLfixed x, GLfixed y, GLfixed z) {
    r_glScalexOES(context, x, y, z);
}

void d_glTexEnvxOES(void *context, GLenum target, GLenum pname, GLfixed param) {
    r_glTexEnvxOES(context, target, pname, param);
}

void d_glTranslatexOES(void *context, GLfixed x, GLfixed y, GLfixed z) {
    r_glTranslatexOES(context, x, y, z);
}

void d_glPointParameterxOES(void *context, GLenum pname, GLfixed param) {
    r_glPointParameterxOES(context, pname, param);
}

void d_glSampleCoveragexOES(void *context, GLclampx value, GLboolean invert) {
    r_glSampleCoveragexOES(context, value, invert);
}

void d_glTexGenxOES(void *context, GLenum coord, GLenum pname, GLfixed param) {
    r_glTexGenxOES(context, coord, pname, param);
}

void d_glClearDepthfOES(void *context, GLclampf depth) {
    r_glClearDepthfOES(context, depth);
}

void d_glDepthRangefOES(void *context, GLclampf n, GLclampf f) {
    r_glDepthRangefOES(context, n, f);
}

void d_glFrustumfOES(void *context, GLfloat l, GLfloat r, GLfloat b, GLfloat t,
                     GLfloat n, GLfloat f) {
    r_glFrustumfOES(context, l, r, b, t, n, f);
}

void d_glOrthofOES(void *context, GLfloat l, GLfloat r, GLfloat b, GLfloat t,
                   GLfloat n, GLfloat f) {
    r_glOrthofOES(context, l, r, b, t, n, f);
}

void d_glRenderbufferStorageMultisampleEXT(void *context, GLenum target,
                                           GLsizei samples,
                                           GLenum internalformat, GLsizei width,
                                           GLsizei height) {
    r_glRenderbufferStorageMultisampleEXT(context, target, samples,
                                          internalformat, width, height);
}

void d_glUseProgramStages(void *context, GLuint pipeline, GLbitfield stages,
                          GLuint program) {
    if (stages != GL_ALL_SHADER_BITS) {
        CHECK_BUFFER_BITS(stages, 3, GL_INVALID_VALUE, GL_VERTEX_SHADER_BIT,
                          GL_FRAGMENT_SHADER_BIT, GL_COMPUTE_SHADER_BIT)
    }
    /*TODO GL_INVALID_OPERATION is generated if program refers to a program
     * object that was not linked with its GL_PROGRAM_SEPARABLE status set.*/
    CHECK_PROGRAM_LINKED
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    r_glUseProgramStages(context, pipeline, stages, program);
}

void d_glActiveShaderProgram(void *context, GLuint pipeline, GLuint program) {
    CHECK_RESOURCE_ID(__LINE__, program_pipeline, pipeline,
                      GL_INVALID_OPERATION);
    CHECK_PROGRAM_LINKED;
    r_glActiveShaderProgram(context, pipeline, program);
}

void d_glProgramUniform1i(void *context, GLuint program, GLint location,
                          GLint v0) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform1i(context, program, location, v0);
}

void d_glProgramUniform2i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform2i(context, program, location, v0, v1);
}

void d_glProgramUniform3i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1, GLint v2) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform3i(context, program, location, v0, v1, v2);
}

void d_glProgramUniform4i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1, GLint v2, GLint v3) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform4i(context, program, location, v0, v1, v2, v3);
}

void d_glProgramUniform1ui(void *context, GLuint program, GLint location,
                           GLuint v0) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform1ui(context, program, location, v0);
}

void d_glProgramUniform2ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform2ui(context, program, location, v0, v1);
}

void d_glProgramUniform3ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1, GLuint v2) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform3ui(context, program, location, v0, v1, v2);
}

void d_glProgramUniform4ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform4ui(context, program, location, v0, v1, v2, v3);
}

void d_glProgramUniform1f(void *context, GLuint program, GLint location,
                          GLfloat v0) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform1f(context, program, location, v0);
}

void d_glProgramUniform2f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform2f(context, program, location, v0, v1);
}

void d_glProgramUniform3f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1, GLfloat v2) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform3f(context, program, location, v0, v1, v2);
}

void d_glProgramUniform4f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    r_glProgramUniform4f(context, program, location, v0, v1, v2, v3);
}

void d_glTransformFeedbackVaryings(void *context, GLuint program, GLsizei count,
                                   const GLchar *const *varyings,
                                   GLenum bufferMode) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE)
    if (bufferMode == GL_SEPARATE_ATTRIBS) {
        CHECK_INT_MAX(__LINE__, count,
                      GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,
                      GL_INVALID_VALUE)
    }
    r_glTransformFeedbackVaryings(context, program, count, varyings,
                                  bufferMode);
}

void d_glTexParameterfv(void *context, GLenum target, GLenum pname,
                        const GLfloat *params) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, pname, 16, GL_DEPTH_STENCIL_TEXTURE_MODE,
        GL_TEXTURE_BASE_LEVEL, GL_TEXTURE_COMPARE_FUNC, GL_TEXTURE_COMPARE_MODE,
        GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_LOD,
        GL_TEXTURE_MAX_LOD, GL_TEXTURE_MAX_LEVEL, GL_TEXTURE_SWIZZLE_R,
        GL_TEXTURE_SWIZZLE_G, GL_TEXTURE_SWIZZLE_B, GL_TEXTURE_SWIZZLE_A,
        GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R,
        GL_TEXTURE_BORDER_COLOR)
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_TEXTURE_2D, GL_TEXTURE_3D,
                     GL_TEXTURE_2D_ARRAY, GL_TEXTURE_EXTERNAL_OES,
                     GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
                     GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY)
    /*TODO GL_INVALID_ENUM is generated if the target is
     * GL_TEXTURE_2D_MULTISAMPLE or GL_TEXTURE_2D_MULTISAMPLE_ARRAY, and pname
     * is any of the sampler states.*/
    //    int arr_len = *(&params + 1) - params;
    //    for(int i=0;i<arr_len;i++){
    //    switch (pname) {
    //        case GL_DEPTH_STENCIL_TEXTURE_MODE: CHECK_VALID_ENUM(__LINE__,
    //        params[i], 2, GL_DEPTH_COMPONENT, GL_STENCIL_INDEX) break; case
    //        GL_TEXTURE_COMPARE_FUNC: CHECK_VALID_ENUM(__LINE__, params[i], 8,
    //        GL_LEQUAL, GL_GEQUAL, GL_LESS, GL_GREATER, GL_EQUAL, GL_NOTEQUAL,
    //        GL_ALWAYS, GL_NEVER) break; case GL_TEXTURE_COMPARE_MODE:
    //        CHECK_VALID_ENUM(__LINE__, params[i], 2,
    //        GL_COMPARE_REF_TO_TEXTURE, GL_NONE)break; case
    //        GL_TEXTURE_MIN_FILTER: CHECK_VALID_ENUM(__LINE__, params[i], 6,
    //        GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST,
    //        GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
    //        GL_LINEAR_MIPMAP_LINEAR)break; case GL_TEXTURE_MAG_FILTER:
    //        CHECK_VALID_ENUM(__LINE__, params[i], 2, GL_NEAREST,
    //        GL_LINEAR)break; case GL_TEXTURE_SWIZZLE_R: case
    //        GL_TEXTURE_SWIZZLE_G: case GL_TEXTURE_SWIZZLE_B: case
    //        GL_TEXTURE_SWIZZLE_A: CHECK_VALID_ENUM(__LINE__, params[i], 6,
    //        GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_ZERO, GL_ONE)break; case
    //        GL_TEXTURE_WRAP_S: case GL_TEXTURE_WRAP_T: case GL_TEXTURE_WRAP_R:
    //        CHECK_VALID_ENUM(__LINE__, params[i], 3, GL_CLAMP_TO_EDGE,
    //        GL_MIRRORED_REPEAT, GL_REPEAT)break;
    //    }
    //    }
    if ((target == GL_TEXTURE_2D || target == GL_TEXTURE_EXTERNAL_OES) &&
        !tex_parameter_should_set(context, target, pname, params[0])) {
        return;
    }
    d_glTexParameterfv_special(context, target, pname, params);
}

void d_glTexParameteriv(void *context, GLenum target, GLenum pname,
                        const GLint *params) {
    // if (target == GL_TEXTURE_EXTERNAL_OES)
    // {
    //     target = GL_TEXTURE_2D;
    // }
    CHECK_VALID_ENUM(
        __LINE__, pname, 17, GL_DEPTH_STENCIL_TEXTURE_MODE,
        GL_TEXTURE_BASE_LEVEL, GL_TEXTURE_COMPARE_FUNC, GL_TEXTURE_COMPARE_MODE,
        GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_MIN_LOD,
        GL_TEXTURE_MAX_LOD, GL_TEXTURE_MAX_LEVEL, GL_TEXTURE_SWIZZLE_R,
        GL_TEXTURE_SWIZZLE_G, GL_TEXTURE_SWIZZLE_B, GL_TEXTURE_SWIZZLE_A,
        GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R,
        GL_TEXTURE_CROP_RECT_OES)
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_TEXTURE_2D, GL_TEXTURE_3D,
                     GL_TEXTURE_2D_ARRAY, GL_TEXTURE_EXTERNAL_OES,
                     GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
                     GL_TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP_ARRAY)
    //    int arr_len =  sizeof(params)/sizeof(params[0]);
    //
    //    for(int i=0;i<arr_len;i++){
    //        switch (pname) {
    //            case GL_DEPTH_STENCIL_TEXTURE_MODE: CHECK_VALID_ENUM(__LINE__,
    //            params[i], 2, GL_DEPTH_COMPONENT, GL_STENCIL_INDEX)break; case
    //            GL_TEXTURE_COMPARE_FUNC: CHECK_VALID_ENUM(__LINE__, params[i],
    //            8, GL_LEQUAL, GL_GEQUAL, GL_LESS, GL_GREATER, GL_EQUAL,
    //            GL_NOTEQUAL, GL_ALWAYS, GL_NEVER)break; case
    //            GL_TEXTURE_COMPARE_MODE: CHECK_VALID_ENUM(__LINE__, params[i],
    //            2, GL_COMPARE_REF_TO_TEXTURE, GL_NONE)break; case
    //            GL_TEXTURE_MIN_FILTER: CHECK_VALID_ENUM(__LINE__, params[i],
    //            6, GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST,
    //            GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR,
    //            GL_LINEAR_MIPMAP_LINEAR)break; case GL_TEXTURE_MAG_FILTER:
    //            CHECK_VALID_ENUM(__LINE__, params[i], 2, GL_NEAREST,
    //            GL_LINEAR)break; case GL_TEXTURE_SWIZZLE_R: case
    //            GL_TEXTURE_SWIZZLE_G: case GL_TEXTURE_SWIZZLE_B: case
    //            GL_TEXTURE_SWIZZLE_A: CHECK_VALID_ENUM(__LINE__, params[i], 6,
    //            GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_ZERO, GL_ONE)break;
    //            case GL_TEXTURE_WRAP_S:
    //            case GL_TEXTURE_WRAP_T:
    //            case GL_TEXTURE_WRAP_R: CHECK_VALID_ENUM(__LINE__, params[i],
    //            3, GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, GL_REPEAT)break;
    //        }
    //    }

    if ((target == GL_TEXTURE_2D || target == GL_TEXTURE_EXTERNAL_OES) &&
        !tex_parameter_should_set(context, target, pname, params[0])) {
        return;
    }
    d_glTexParameteriv_special(context, target, pname, params);
}

void d_glUniform1fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 1)) {
        return;
    }

    r_glUniform1fv(context, location, count, value);
}

void d_glUniform1iv(void *context, GLint location, GLsizei count,
                    const GLint *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLint) * 1)) {
        return;
    }

    r_glUniform1iv(context, location, count, value);
}

void d_glUniform2fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 2)) {
        return;
    }

    r_glUniform2fv(context, location, count, value);
}

void d_glUniform2iv(void *context, GLint location, GLsizei count,
                    const GLint *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLint) * 2)) {
        return;
    }

    r_glUniform2iv(context, location, count, value);
}

void d_glUniform3fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 3)) {
        return;
    }

    r_glUniform3fv(context, location, count, value);
}

void d_glUniform3iv(void *context, GLint location, GLsizei count,
                    const GLint *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLint) * 3)) {
        return;
    }

    r_glUniform3iv(context, location, count, value);
}

void d_glUniform4fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 4)) {
        return;
    }

    r_glUniform4fv(context, location, count, value);
}

void d_glUniform4iv(void *context, GLint location, GLsizei count,
                    const GLint *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLint) * 4)) {
        return;
    }

    r_glUniform4iv(context, location, count, value);
}

void d_glVertexAttrib1fv(void *context, GLuint index, const GLfloat *v) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttrib1fv(context, index, v);
}

void d_glVertexAttrib2fv(void *context, GLuint index, const GLfloat *v) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttrib2fv(context, index, v);
}

void d_glVertexAttrib3fv(void *context, GLuint index, const GLfloat *v) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttrib3fv(context, index, v);
}

void d_glVertexAttrib4fv(void *context, GLuint index, const GLfloat *v) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttrib4fv(context, index, v);
}

void d_glUniformMatrix2fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 4)) {
        return;
    }

    r_glUniformMatrix2fv(context, location, count, transpose, value);
}

void d_glUniformMatrix3fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 9)) {
        return;
    }

    r_glUniformMatrix3fv(context, location, count, transpose, value);
}

void d_glUniformMatrix4fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 16)) {
        return;
    }

    r_glUniformMatrix4fv(context, location, count, transpose, value);
}

void d_glUniformMatrix2x3fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 6)) {
        return;
    }

    r_glUniformMatrix2x3fv(context, location, count, transpose, value);
}

void d_glUniformMatrix3x2fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 6)) {
        return;
    }

    r_glUniformMatrix3x2fv(context, location, count, transpose, value);
}

void d_glUniformMatrix2x4fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 8)) {
        return;
    }

    r_glUniformMatrix2x4fv(context, location, count, transpose, value);
}

void d_glUniformMatrix4x2fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 8)) {
        return;
    }

    r_glUniformMatrix4x2fv(context, location, count, transpose, value);
}

void d_glUniformMatrix3x4fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 12)) {
        return;
    }

    r_glUniformMatrix3x4fv(context, location, count, transpose, value);
}

void d_glUniformMatrix4x3fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLfloat) * 12)) {
        return;
    }

    r_glUniformMatrix4x3fv(context, location, count, transpose, value);
}

void d_glVertexAttribI4iv(void *context, GLuint index, const GLint *v) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttribI4iv(context, index, v);
}

void d_glVertexAttribI4uiv(void *context, GLuint index, const GLuint *v) {
    StaticContextValues *static_values =
        (((GLThreadContext *)context)->static_values);
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE)
    r_glVertexAttribI4uiv(context, index, v);
}

void d_glUniform1uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLuint) * 1)) {
        return;
    }

    r_glUniform1uiv(context, location, count, value);
}

void d_glUniform2uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLuint) * 2)) {
        return;
    }

    r_glUniform2uiv(context, location, count, value);
}

void d_glUniform3uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLuint) * 3)) {
        return;
    }

    r_glUniform3uiv(context, location, count, value);
}

void d_glUniform4uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value) {
    GLint current_program = 0;
    BindingStatus *bind_status =
        &(((GLThreadContext *)context)->context_binding_status);
    current_program = bind_status->current_program;
    CHECK_ZERO(__LINE__, current_program, GL_INVALID_OPERATION)
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the glUniform
     * command.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed or unsigned
     * integer variants of this function is used to load a uniform variable of
     * type float, vec2, vec3, vec4, or an array of these, or if one of the
     * floating-point variants of this function is used to load a uniform
     * variable of type int, ivec2, ivec3, ivec4, unsigned int, uvec2, uvec3,
     * uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the signed integer
     * variants of this function is used to load a uniform variable of type
     * unsigned int, uvec2, uvec3, uvec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if one of the unsigned integer
     * variants of this function is used to load a uniform variable of type int,
     * ivec2, ivec3, ivec4, or an array of these.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for the current program object and location is not equal to
     * -1.*/
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/
    /*TODO GL_INVALID_OPERATION is generated if a sampler is loaded using a
     * command other than glUniform1i and glUniform1iv.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, current_program, location,
                                        (char *)value, sizeof(GLuint) * 4)) {
        return;
    }

    r_glUniform4uiv(context, location, count, value);
}

void d_glClearBufferiv(void *context, GLenum buffer, GLint drawbuffer,
                       const GLint *value) {
    CHECK_VALID_ENUM(__LINE__, buffer, 2, GL_COLOR, GL_STENCIL);
    if (buffer == GL_COLOR) {
        StaticContextValues *static_values =
            (((GLThreadContext *)context)->static_values);
        CHECK_TARGET_INDEX_MAX(1, 1, drawbuffer, max_draw_buffers,
                               GL_INVALID_VALUE)
        // CHECK_INT_MAX(__LINE__, drawbuffer, GL_MAX_DRAW_BUFFERS,
        // GL_INVALID_VALUE)
    }
    if (buffer == GL_STENCIL && drawbuffer != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    r_glClearBufferiv(context, buffer, drawbuffer, value);
}

void d_glClearBufferuiv(void *context, GLenum buffer, GLint drawbuffer,
                        const GLuint *value) {
    CHECK_VALID_ENUM(__LINE__, buffer, 1, GL_COLOR);
    if (buffer == GL_COLOR) {
        StaticContextValues *static_values =
            (((GLThreadContext *)context)->static_values);
        CHECK_TARGET_INDEX_MAX(1, 1, drawbuffer, max_draw_buffers,
                               GL_INVALID_VALUE)
        // CHECK_INT_MAX(__LINE__, drawbuffer, GL_MAX_DRAW_BUFFERS,
        // GL_INVALID_VALUE)
    }
    r_glClearBufferuiv(context, buffer, drawbuffer, value);
}

void d_glClearBufferfv(void *context, GLenum buffer, GLint drawbuffer,
                       const GLfloat *value) {
    CHECK_VALID_ENUM(__LINE__, buffer, 2, GL_COLOR, GL_DEPTH);
    if (buffer == GL_COLOR) {
        StaticContextValues *static_values =
            (((GLThreadContext *)context)->static_values);
        CHECK_TARGET_INDEX_MAX(1, 1, drawbuffer, max_draw_buffers,
                               GL_INVALID_VALUE)
        // CHECK_INT_MAX(__LINE__, drawbuffer, GL_MAX_DRAW_BUFFERS,
        // GL_INVALID_VALUE)
    }
    if (buffer == GL_DEPTH && drawbuffer != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }
    r_glClearBufferfv(context, buffer, drawbuffer, value);
}

void d_glSamplerParameteriv(void *context, GLuint sampler, GLenum pname,
                            const GLint *param) {
    CHECK_RESOURCE_ID(__LINE__, sampler, sampler, GL_INVALID_OPERATION)
    //    int arr_len = *(&param + 1) - param;
    //    for(int i=0;i<arr_len;i++) {
    //        switch (pname) {
    //            case GL_TEXTURE_MIN_FILTER:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 6, GL_NEAREST,
    //                GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST,
    //                                 GL_LINEAR_MIPMAP_NEAREST,
    //                                 GL_NEAREST_MIPMAP_LINEAR,
    //                                 GL_LINEAR_MIPMAP_LINEAR)break;
    //            case GL_TEXTURE_MAG_FILTER:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 2, GL_NEAREST,
    //                GL_LINEAR)break;
    //            case GL_TEXTURE_WRAP_S:
    //            case GL_TEXTURE_WRAP_T:
    //            case GL_TEXTURE_WRAP_R:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 3, GL_CLAMP_TO_EDGE,
    //                GL_MIRRORED_REPEAT, GL_REPEAT)break;
    //            case GL_TEXTURE_COMPARE_MODE:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 2,
    //                GL_COMPARE_REF_TO_TEXTURE, GL_NONE)break;
    //            case GL_TEXTURE_COMPARE_FUNC:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 8, GL_LEQUAL,
    //                GL_GEQUAL, GL_LESS, GL_GREATER, GL_EQUAL,
    //                                 GL_NOTEQUAL, GL_ALWAYS, GL_NEVER)break;
    //        }
    //    }
    r_glSamplerParameteriv(context, sampler, pname, param);
}

void d_glSamplerParameterfv(void *context, GLuint sampler, GLenum pname,
                            const GLfloat *param) {
    CHECK_RESOURCE_ID(__LINE__, sampler, sampler, GL_INVALID_OPERATION)
    //    int arr_len = *(&param + 1) - param;
    //    for(int i=0;i<arr_len;i++) {
    //        switch (pname) {
    //            case GL_TEXTURE_MIN_FILTER:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 6, GL_NEAREST,
    //                GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST,
    //                                 GL_LINEAR_MIPMAP_NEAREST,
    //                                 GL_NEAREST_MIPMAP_LINEAR,
    //                                 GL_LINEAR_MIPMAP_LINEAR)break;
    //            case GL_TEXTURE_MAG_FILTER:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 2, GL_NEAREST,
    //                GL_LINEAR)break;
    //            case GL_TEXTURE_WRAP_S:
    //            case GL_TEXTURE_WRAP_T:
    //            case GL_TEXTURE_WRAP_R:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 3, GL_CLAMP_TO_EDGE,
    //                GL_MIRRORED_REPEAT, GL_REPEAT)break;
    //            case GL_TEXTURE_COMPARE_MODE:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 2,
    //                GL_COMPARE_REF_TO_TEXTURE, GL_NONE)break;
    //            case GL_TEXTURE_COMPARE_FUNC:
    //                CHECK_VALID_ENUM(__LINE__, param[i], 8, GL_LEQUAL,
    //                GL_GEQUAL, GL_LESS, GL_GREATER, GL_EQUAL,
    //                                 GL_NOTEQUAL, GL_ALWAYS, GL_NEVER)break;
    //        }
    //    }
    r_glSamplerParameterfv(context, sampler, pname, param);
}

void d_glInvalidateFramebuffer(void *context, GLenum target,
                               GLsizei numAttachments,
                               const GLenum *attachments) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_DRAW_FRAMEBUFFER,
                     GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER)

    // static GLint max_color_attachments=0;
    // if(max_color_attachments==0)
    // {
    //     StaticContextValues *static_values = &(((GLThreadContext
    //     *)context)->static_values); max_color_attachments =
    //     static_values->max_color_attachments;
    // }
    // for (int i = 0; i < numAttachments; i++)
    // {
    //     if (attachments[i] != GL_DEPTH_ATTACHMENT && attachments[i] !=
    //     GL_STENCIL_ATTACHMENT && attachments[i] !=
    //     GL_DEPTH_STENCIL_ATTACHMENT)
    //     {
    //         GLint check = attachments[i] - GL_COLOR_ATTACHMENT0;
    //         CHECK_NEGATIVE(__LINE__, check, GL_INVALID_OPERATION)
    //         CHECK_INT_MAX(__LINE__, check, GL_MAX_COLOR_ATTACHMENTS,
    //         GL_INVALID_OPERATION)
    //     }
    // }
    r_glInvalidateFramebuffer(context, target, numAttachments, attachments);
}

void d_glInvalidateSubFramebuffer(void *context, GLenum target,
                                  GLsizei numAttachments,
                                  const GLenum *attachments, GLint x, GLint y,
                                  GLsizei width, GLsizei height) {
    CHECK_VALID_ENUM(__LINE__, target, 3, GL_DRAW_FRAMEBUFFER,
                     GL_READ_FRAMEBUFFER, GL_FRAMEBUFFER)
    // for (int i = 0; i < numAttachments; i++)
    // {
    //     if (attachments[i] != GL_DEPTH_ATTACHMENT && attachments[i] !=
    //     GL_STENCIL_ATTACHMENT && attachments[i] !=
    //     GL_DEPTH_STENCIL_ATTACHMENT)
    //     {
    //         GLint check = attachments[i] - GL_COLOR_ATTACHMENT0;
    //         CHECK_NEGATIVE(__LINE__, check, GL_INVALID_OPERATION)
    //         CHECK_INT_MAX(__LINE__, check, GL_MAX_COLOR_ATTACHMENTS,
    //         GL_INVALID_OPERATION)
    //     }
    // }
    r_glInvalidateSubFramebuffer(context, target, numAttachments, attachments,
                                 x, y, width, height);
}

void d_glClipPlanexOES(void *context, GLenum plane, const GLfixed *equation) {
    r_glClipPlanexOES(context, plane, equation);
}

void d_glFogxvOES(void *context, GLenum pname, const GLfixed *param) {
    r_glFogxvOES(context, pname, param);
}

void d_glLightModelxvOES(void *context, GLenum pname, const GLfixed *param) {
    r_glLightModelxvOES(context, pname, param);
}

void d_glLightxvOES(void *context, GLenum light, GLenum pname,
                    const GLfixed *params) {
    r_glLightxvOES(context, light, pname, params);
}

void d_glLoadMatrixxOES(void *context, const GLfixed *m) {
    r_glLoadMatrixxOES(context, m);
}

void d_glMaterialxvOES(void *context, GLenum face, GLenum pname,
                       const GLfixed *param) {
    r_glMaterialxvOES(context, face, pname, param);
}

void d_glMultMatrixxOES(void *context, const GLfixed *m) {
    r_glMultMatrixxOES(context, m);
}

void d_glPointParameterxvOES(void *context, GLenum pname,
                             const GLfixed *params) {
    r_glPointParameterxvOES(context, pname, params);
}

void d_glTexEnvxvOES(void *context, GLenum target, GLenum pname,
                     const GLfixed *params) {
    r_glTexEnvxvOES(context, target, pname, params);
}

void d_glClipPlanefOES(void *context, GLenum plane, const GLfloat *equation) {
    r_glClipPlanefOES(context, plane, equation);
}

void d_glTexGenxvOES(void *context, GLenum coord, GLenum pname,
                     const GLfixed *params) {
    r_glTexGenxvOES(context, coord, pname, params);
}

void d_glProgramUniform1iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLint) * 1)) {
        return;
    }

    r_glProgramUniform1iv(context, program, location, count, value);
}

void d_glProgramUniform2iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLint) * 2)) {
        return;
    }

    r_glProgramUniform2iv(context, program, location, count, value);
}

void d_glProgramUniform3iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLint) * 3)) {
        return;
    }

    r_glProgramUniform3iv(context, program, location, count, value);
}

void d_glProgramUniform4iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLint) * 4)) {
        return;
    }

    r_glProgramUniform4iv(context, program, location, count, value);
}

void d_glProgramUniform1uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLuint) * 1)) {
        return;
    }

    r_glProgramUniform1uiv(context, program, location, count, value);
}

void d_glProgramUniform2uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLuint) * 2)) {
        return;
    }

    r_glProgramUniform2uiv(context, program, location, count, value);
}

void d_glProgramUniform3uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLuint) * 3)) {
        return;
    }

    r_glProgramUniform3uiv(context, program, location, count, value);
}

void d_glProgramUniform4uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLuint) * 4)) {
        return;
    }

    r_glProgramUniform4uiv(context, program, location, count, value);
}

void d_glProgramUniform1fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 1)) {
        return;
    }

    r_glProgramUniform1fv(context, program, location, count, value);
}

void d_glProgramUniform2fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 2)) {
        return;
    }

    r_glProgramUniform2fv(context, program, location, count, value);
}

void d_glProgramUniform3fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 3)) {
        return;
    }

    r_glProgramUniform3fv(context, program, location, count, value);
}

void d_glProgramUniform4fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 4)) {
        return;
    }

    r_glProgramUniform4fv(context, program, location, count, value);
}

void d_glProgramUniformMatrix2fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 4)) {
        return;
    }

    r_glProgramUniformMatrix2fv(context, program, location, count, transpose,
                                value);
}

void d_glProgramUniformMatrix3fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 9)) {
        return;
    }

    r_glProgramUniformMatrix3fv(context, program, location, count, transpose,
                                value);
}

void d_glProgramUniformMatrix4fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 16)) {
        return;
    }

    r_glProgramUniformMatrix4fv(context, program, location, count, transpose,
                                value);
}

void d_glProgramUniformMatrix2x3fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 6)) {
        return;
    }

    r_glProgramUniformMatrix2x3fv(context, program, location, count, transpose,
                                  value);
}

void d_glProgramUniformMatrix3x2fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 6)) {
        return;
    }

    r_glProgramUniformMatrix3x2fv(context, program, location, count, transpose,
                                  value);
}

void d_glProgramUniformMatrix2x4fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 8)) {
        return;
    }

    r_glProgramUniformMatrix2x4fv(context, program, location, count, transpose,
                                  value);
}

void d_glProgramUniformMatrix4x2fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 8)) {
        return;
    }

    r_glProgramUniformMatrix4x2fv(context, program, location, count, transpose,
                                  value);
}

void d_glProgramUniformMatrix3x4fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 12)) {
        return;
    }

    r_glProgramUniformMatrix3x4fv(context, program, location, count, transpose,
                                  value);
}

void d_glProgramUniformMatrix4x3fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value) {
    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_OPERATION)
    /*TODO GL_INVALID_OPERATION is generated if the size of the uniform variable
     * declared in the shader does not match the size indicated by the
     * glProgramUniform command.*/
    /*TODO GL_INVALID_OPERATION is generated if location is an invalid uniform
     * location for program and location is not equal to -1.*/
    CHECK_NEGATIVE(__LINE__, count, GL_INVALID_VALUE)
    /*TODO GL_INVALID_OPERATION is generated if count is greater than 1 and the
     * indicated uniform variable is not an array variable.*/

    if (count == 1 &&
        !GLSLProgram::uniform_is_change(context, program, location,
                                        (char *)value, sizeof(GLfloat) * 12)) {
        return;
    }

    r_glProgramUniformMatrix4x3fv(context, program, location, count, transpose,
                                  value);
}

void d_glBindAttribLocation(void *context, GLuint program, GLuint index,
                            const GLchar *name) {
    GLThreadContext *opengl_context = (GLThreadContext *)context;
    StaticContextValues *static_values = opengl_context->static_values;
    CHECK_TARGET_INDEX_MAX(1, 1, index, max_vertex_attribs, GL_INVALID_VALUE)
    // CHECK_INT_MAX(__LINE__, index, GL_MAX_VERTEX_ATTRIBS, GL_INVALID_VALUE);
    // if (strlen(name) >= 3 && strncmp("gl_", name, 3) == 0)
    // {
    //     set_gl_error(context, GL_INVALID_OPERATION);
    //     return;
    // }

    CHECK_RESOURCE_ID(__LINE__, program, program, GL_INVALID_VALUE);
    /*TODO GL_INVALID_OPERATION is generated if program is not a program
     * object.*/

#ifdef USE_LOCAL_COMPILER
    opengl_context->glsl_compiler->bind_attrib_location(program, index, name);
#endif
    r_glBindAttribLocation(context, program, index, name);
}

void d_glTexEnvf(void *context, GLenum target, GLenum pname, GLfloat param) {
    r_glTexEnvf(context, target, pname, param);
}

void d_glTexEnvi(void *context, GLenum target, GLenum pname, GLint param) {
    r_glTexEnvi(context, target, pname, param);
}

void d_glTexEnvx(void *context, GLenum target, GLenum pname, GLfixed param) {
    r_glTexEnvx(context, target, pname, param);
}

void d_glTexParameterx(void *context, GLenum target, GLenum pname,
                       GLint param) {
    if ((target == GL_TEXTURE_2D || target == GL_TEXTURE_EXTERNAL_OES) &&
        !tex_parameter_should_set(context, target, pname, param)) {
        return;
    }
    r_glTexParameterx(context, target, pname, param);
}

void d_glShadeModel(void *context, GLenum mode) {
    r_glShadeModel(context, mode);
}

void d_glDrawTexiOES(void *context, GLint x, GLint y, GLint z, GLint width,
                     GLint height) {
    d_glDrawTexiOES_special(context, x, y, z, width, height);
}

/******* file '2-2' *******/

// void d_glVertexAttribIPointer_without_bound(void *context, GLuint index,
// GLint size, GLenum type, GLsizei stride, GLuint offset, GLsizei length, const
// void *pointer)
//{
//
//     r_glVertexAttribIPointer_without_bound(context, index, size, type,
//     stride, offset, length, pointer);
// }
//
// void d_glVertexAttribPointer_without_bound(void *context, GLuint index, GLint
// size, GLenum type, GLboolean normalized, GLsizei stride, GLuint offset,
// GLuint length, const void *pointer)
//{
//
//     r_glVertexAttribPointer_without_bound(context, index, size, type,
//     normalized, stride, offset, length, pointer);
// }
//
// void d_glDrawElements_without_bound(void *context, GLenum mode, GLsizei
// count, GLenum type, const void *indices)
//{
//
//     r_glDrawElements_without_bound(context, mode, count, type, indices);
// }
//
// void d_glDrawElementsInstanced_without_bound(void *context, GLenum mode,
// GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
//{
//
//     r_glDrawElementsInstanced_without_bound(context, mode, count, type,
//     indices, instancecount);
// }
//
// void d_glDrawRangeElements_without_bound(void *context, GLenum mode, GLuint
// start, GLuint end, GLsizei count, GLenum type, const void *indices)
//{
//
//     r_glDrawRangeElements_without_bound(context, mode, start, end, count,
//     type, indices);
// }

// void d_glFlushMappedBufferRange_special(void *context, GLenum target,
// GLintptr offset, GLsizeiptr length, const void *data)
//{
//
//     r_glFlushMappedBufferRange_special(context, target, offset, length,
//     data);
// }

// void d_glBufferData_custom(void *context, GLenum target, GLsizeiptr size,
// const void *data, GLenum usage)
//{
//     CHECK_VALID_ENUM(__LINE__, target, 12, GL_ARRAY_BUFFER,
//     GL_ATOMIC_COUNTER_BUFFER, GL_COPY_READ_BUFFER,
//                      GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER,
//                      GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
//                      GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER,
//                      GL_SHADER_STORAGE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER,
//                      GL_UNIFORM_BUFFER);
//     CHECK_VALID_ENUM(__LINE__, usage, 9, GL_STREAM_DRAW, GL_STREAM_READ,
//     GL_STREAM_COPY,
//                      GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY,
//                      GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, GL_DYNAMIC_COPY);
//     if(size<0){set_gl_error(context, GL_INVALID_VALUE); return;}
//     /*TODO　GL_INVALID_OPERATION is generated if the reserved buffer object
//     name 0 is bound to target.*/
//     /*TODO　GL_OUT_OF_MEMORY is generated if the GL is unable to create a
//     data store with the specified size*/ r_glBufferData_custom(context,
//     target, size, data, usage);
// }

// void d_glBufferSubData_custom(void *context, GLenum target, GLintptr offset,
// GLsizeiptr size, const void *data)
//{
//     CHECK_VALID_ENUM(__LINE__, target, 12, GL_ARRAY_BUFFER,
//     GL_ATOMIC_COUNTER_BUFFER, GL_COPY_READ_BUFFER,
//                      GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER,
//                      GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
//                      GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER,
//                      GL_SHADER_STORAGE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER,
//                      GL_UNIFORM_BUFFER);
//     /*TODO GL_INVALID_VALUE is generated if offset or size is negative, or if
//     together they define a region of memory that extends beyond the buffer
//     object's allocated data store.*/
//     /*TODO GL_INVALID_OPERATION is generated if the reserved buffer object
//     name 0 is bound to target.*/
//     /*TODO GL_INVALID_OPERATION is generated if the buffer object being
//     updated is mapped.*/ r_glBufferSubData_custom(context, target, offset,
//     size, data);
// }
//
// void d_glCompressedTexImage2D_without_bound(void *context, GLenum target,
// GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint
// border, GLsizei imageSize, const void *data)
//{
//     CHECK_VALID_ENUM(__LINE__, internalformat, 7, GL_TEXTURE_2D,
//     GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
//                      GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
//                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
//                      GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
//                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
//     /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with
//     the format, dimensions, and contents of the specified compressed image
//     data.*/ if(border!=0){set_gl_error(context, GL_INVALID_VALUE); return;}
//     /*TODO GL_INVALID_OPERATION is generated if parameter combinations are
//     not supported by the specific compressed internal format as specified in
//     the specific texture compression extension*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's
//     data store is currently mapped*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be
//     unpacked from the buffer object such that the memory reads required would
//     exceed the data store size.*/
//
//     {
//         if (imageSize < 0)
//         {
//             set_gl_error(context, GL_INVALID_VALUE);
//             return;
//         }
//     }
//     r_glCompressedTexImage2D_without_bound(context, target, level,
//     internalformat, width, height, border, imageSize, data);
// }
//
// void d_glCompressedTexSubImage2D_without_bound(void *context, GLenum target,
// GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
// GLenum format, GLsizei imageSize, const void *data)
//{
//     CHECK_VALID_ENUM(__LINE__, target, 7, GL_TEXTURE_2D,
//     GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
//                      GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
//                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
//                      GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
//                      GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
//     /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with
//     the format, dimensions, and contents of the specified compressed image
//     data.*/
//     /*TODO GL_INVALID_OPERATION is generated if parameter combinations are
//     not supported by the specific compressed internal format as specified in
//     the specific texture compression extension.*/
//     /*TODO For ETC2/EAC images GL_INVALID_OPERATION is generated if width is
//     not a multiple of four, and width + xoffset is not equal to the width of
//     the texture level; if height is not a multiple of four, and height +
//     yoffset is not equal to the height of the texture level; or if xoffset or
//     yoffset is not a multiple of four.*/
//     /*TODO For ASTC images GL_INVALID_OPERATION is generated if width is not
//     a multiple of the block width, and width + xoffset is not equal to the
//     width of the texture level; if height is not a multiple of the block
//     height, and height + yoffset is not equal to the height of the texture
//     level; or if xoffset or yoffset is not a multiple of the corresponding
//     block dimension.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's
//     data store is currently mapped.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be
//     unpacked from the buffer object such that the memory reads required would
//     exceed the data store size.*/
//
//     {
//         if (imageSize < 0)
//         {
//             set_gl_error(context, GL_INVALID_VALUE);
//             return;
//         }
//     }
//     r_glCompressedTexSubImage2D_without_bound(context, target, level,
//     xoffset, yoffset, width, height, format, imageSize, data);
// }
//
// void d_glCompressedTexImage3D_without_bound(void *context, GLenum target,
// GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei
// depth, GLint border, GLsizei imageSize, const void *data)
//{
//     CHECK_VALID_ENUM(__LINE__, target, 2, GL_TEXTURE_3D,
//     GL_TEXTURE_2D_ARRAY);
//     /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with
//     the format, dimensions, and contents of the specified compressed image
//     data.*/ if(border!=0){set_gl_error(context, GL_INVALID_VALUE); return;}
//     /*TODO GL_INVALID_OPERATION is generated if parameter combinations are
//     not supported by the specific compressed internal format as specified in
//     the specific texture compression extension. The ETC2/EAC texture
//     compression algorithm supports only two-dimensional images. If
//     internalformat is an ETC2/EAC format, glCompressedTexImage3D will
//     generate a GL_INVALID_OPERATION error if target is not
//     GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY. If internalformat is an
//     ASTC format, glCompressedTexImage3D will generate an INVALID_OPERATION
//     error if target is not GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY
//     unless GL_OES_texture_compression_astc is supported.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's
//     data store is currently mapped*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be
//     unpacked from the buffer object such that the memory reads required would
//     exceed the data store size.*/
//
//     {
//         if (imageSize < 0)
//         {
//             set_gl_error(context, GL_INVALID_VALUE);
//             return;
//         }
//     }
//     r_glCompressedTexImage3D_without_bound(context, target, level,
//     internalformat, width, height, depth, border, imageSize, data);
// }

// void d_glCompressedTexSubImage3D_without_bound(void *context, GLenum target,
// GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width,
// GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void
// *data)
//{
//     CHECK_VALID_ENUM(__LINE__, target,2, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY);
//     /*TODO GL_INVALID_VALUE is generated if imageSize is not consistent with
//     the format, dimensions, and contents of the specified compressed image
//     data.*/
//     /*TODO GL_INVALID_OPERATION is generated if parameter combinations are
//     not supported by the specific compressed internal format as specified in
//     the specific texture compression extension. For ETC2/EAC images
//     GL_INVALID_OPERATION is generated if width is not a multiple of four, and
//     width + xoffset is not equal to the width of the texture level; if height
//     is not a multiple of four, and height + yoffset is not equal to the
//     height of the texture level; or if xoffset or yoffset is not a multiple
//     of four. The ETC2/EAC texture compression algorithm supports only
//     two-dimensional images. If format is an ETC2/EAC format,
//     glCompressedTexSubImage3D will generate an GL_INVALID_OPERATION error if
//     target is not GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY.*/
//     /*TODO For ASTC images GL_INVALID_OPERATION is generated if width is not
//     a multiple of the block width, and width + xoffset is not equal to the
//     width of the texture level; if height is not a multiple of the block
//     height, and height + yoffset is not equal to the height of the texture
//     level; or if xoffset or yoffset is not a multiple of the corresponding
//     block dimension. If format is an ASTC format, glCompressedTexSubImage3D
//     will generate an GL_INVALID_OPERATION error if target is not
//     GL_TEXTURE_2D_ARRAY or GL_TEXTURE_CUBE_MAP_ARRAY, unless
//     GL_OES_texture_compression_astc is supported.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the buffer object's
//     data store is currently mapped.*/
//     /*TODO GL_INVALID_OPERATION is generated if a non-zero buffer object name
//     is bound to the GL_PIXEL_UNPACK_BUFFER target and the data would be
//     unpacked from the buffer object such that the memory reads required would
//     exceed the data store size.*/
//     {
//         if (imageSize < 0)
//         {
//             set_gl_error(context, GL_INVALID_VALUE);
//             return;
//         }
//     }
//     r_glCompressedTexSubImage3D_without_bound(context, target, level,
//     xoffset, yoffset, zoffset, width, height, depth, format, imageSize,
//     data);
// }

// void d_glTexImage2D_without_bound(void *context, GLenum target, GLint level,
// GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum
// format, GLenum type, GLint buf_len, const void *pixels)
//{
//
//     r_glTexImage2D_without_bound(context, target, level, internalformat,
//     width, height, border, format, type, buf_len, pixels);
// }
//
// void d_glTexImage3D_without_bound(void *context, GLenum target, GLint level,
// GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint
// border, GLenum format, GLenum type, GLint buf_len, const void *pixels)
//{
//
//     r_glTexImage3D_without_bound(context, target, level, internalformat,
//     width, height, depth, border, format, type, buf_len, pixels);
// }

// void d_glTexSubImage2D_without_bound(void *context, GLenum target, GLint
// level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum
// format, GLenum type, GLint buf_len, const void *pixels)
//{
//
//     r_glTexSubImage2D_without_bound(context, target, level, xoffset, yoffset,
//     width, height, format, type, buf_len, pixels);
// }
//
// void d_glTexSubImage3D_without_bound(void *context, GLenum target, GLint
// level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei
// height, GLsizei depth, GLenum format, GLenum type, GLint buf_len, const void
// *pixels)
//{
//
//     r_glTexSubImage3D_without_bound(context, target, level, xoffset, yoffset,
//     zoffset, width, height, depth, format, type, buf_len, pixels);
// }

void d_glPrintf(void *context, GLint buf_len, const GLchar *out_string) {
    r_glPrintf(context, buf_len, out_string);
}

void d_glBindImageTexture(void *context, GLuint unit, GLuint texture,
                          GLint level, GLboolean layered, GLint layer,
                          GLenum access, GLenum format) {
    d_glBindImageTexture_special(context, unit, texture, level, layered, layer,
                                 access, format);
}

void d_glBindVertexBuffer(void *context, GLuint bindingindex, GLuint buffer,
                          GLintptr offset, GLsizei stride) {
    CHECK_NEGATIVE(__LINE__, offset < 0 || stride < 0 || buffer == 0,
                   GL_INVALID_VALUE)
    CHECK_INT_MAX(__LINE__, bindingindex, GL_MAX_VERTEX_ATTRIB_BINDINGS,
                  GL_INVALID_VALUE)
    CHECK_INT_MAX(__LINE__, stride, GL_MAX_VERTEX_ATTRIB_STRIDE,
                  GL_INVALID_VALUE)
    // GL_INVALID_OPERATION is generated if no vertex array object is bound.

    d_glBindVertexBuffer_special(context, bindingindex, buffer, offset, stride);
}

void d_glVertexAttribFormat(void *context, GLuint attribindex, GLint size,
                            GLenum type, GLboolean normalized,
                            GLuint relativeoffset) {
    d_glVertexAttribFormat_special(context, attribindex, size, type, normalized,
                                   relativeoffset);
}

void d_glVertexAttribIFormat(void *context, GLuint attribindex, GLint size,
                             GLenum type, GLuint relativeoffset) {
    d_glVertexAttribIFormat_special(context, attribindex, size, type,
                                    relativeoffset);
}

void d_glVertexAttribBinding(void *context, GLuint attribindex,
                             GLuint bindingindex) {
    d_glVertexAttribBinding_special(context, attribindex, bindingindex);
}

void d_glVertexBindingDivisor(void *context, GLuint bindingindex,
                              GLuint divisor) {
    d_glVertexBindingDivisor_special(context, bindingindex, divisor);
}

void d_glDispatchCompute(void *context, GLuint num_groups_x,
                         GLuint num_groups_y, GLuint num_groups_z) {
    r_glDispatchCompute(context, num_groups_x, num_groups_y, num_groups_z);
}

void d_glDispatchComputeIndirect(void *context, GLintptr indirect) {
    r_glDispatchComputeIndirect(context, indirect);
}

void d_glMemoryBarrier(void *context, GLbitfield barriers) {
    r_glMemoryBarrier(context, barriers);
}

void d_glMemoryBarrierByRegion(void *context, GLbitfield barriers) {
    r_glMemoryBarrierByRegion(context, barriers);
}

void d_glFramebufferParameteri(void *context, GLenum target, GLenum pname,
                               GLint param) {
    
    r_glFramebufferParameteri(context, target, pname, param);
}

void d_glSampleMaski(void *context, GLuint maskNumber, GLbitfield mask) {
    r_glSampleMaski(context, maskNumber, mask);
}

void d_glTexStorage2DMultisample(void *context, GLenum target, GLsizei samples,
                                 GLenum internalformat, GLsizei width,
                                 GLsizei height,
                                 GLboolean fixedsamplelocations) {
    d_glTexStorage2DMultisample_special(context, target, samples,
                                        internalformat, width, height,
                                        fixedsamplelocations);
}

void d_glValidateProgramPipeline(void *context, GLuint pipeline) {
    r_glValidateProgramPipeline(context, pipeline);
}

void d_glDrawArraysIndirect(void *context, GLenum mode, const void *indirect) {
    CHECK_VALID_ENUM(__LINE__, mode, 7, GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
                     GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
                     GL_TRIANGLES);
    d_glDrawArraysIndirect_special(context, mode, indirect);
}

void d_glDrawElementsIndirect(void *context, GLenum mode, GLenum type,
                              const void *indirect) {
    CHECK_VALID_ENUM(__LINE__, mode, 12, GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP,
                     GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY,
                     GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES,
                     GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY,
                     GL_PATCHES);
    /*TODO GL_INVALID_OPERATION is generated if the command would source data
     * beyond the end of any bound buffer object.*/
    /*TODO GL_INVALID_OPERATION is generated if a geometry shader is active and
     * mode is incompatible with the input primitive type of the geometry shader
     * in the currently installed program object.*/
    if (((uint64_t)indirect) % 4 != 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    d_glDrawElementsIndirect_special(context, mode, type, indirect);
}

void d_glDiscardFramebufferEXT(void *context, GLenum target,
                               GLsizei numAttachments,
                               const GLenum *attachments) {
    r_glDiscardFramebufferEXT(context, target, numAttachments, attachments);
}

void d_glTexBuffer(void *context, GLenum target, GLenum internalformat,
                   GLuint buffer) {
    if (target != GL_TEXTURE_BUFFER) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }

    // @todo
    // GL_INVALID_ENUM is generated if internalformat is not one of the sized
    // internal formats described above.

    d_glTexBuffer_special(context, target, internalformat, buffer);
}

void d_glTexBufferRange(void *context, GLenum target, GLenum internalformat,
                        GLuint buffer, GLintptr offset, GLsizeiptr size) {
    if (target != GL_TEXTURE_BUFFER) {
        set_gl_error(context, GL_INVALID_ENUM);
        return;
    }

    // @todo
    // GL_INVALID_ENUM is generated if internalformat is not one of the sized
    // internal formats described above. GL_INVALID_VALUE is generated if offset
    // is not an integer multiple of the value of
    // GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT.

    d_glTexBufferRange_special(context, target, internalformat, buffer, offset,
                               size);
}

void d_glTexBufferOES(void *context, GLenum target, GLenum internalformat,
                      GLuint buffer) {
    d_glTexBuffer(context, target, internalformat, buffer);
}

void d_glTexBufferRangeOES(void *context, GLenum target, GLenum internalformat,
                           GLuint buffer, GLintptr offset, GLsizeiptr size) {
    d_glTexBufferRange(context, target, internalformat, buffer, offset, size);
}

void d_glTexBufferEXT(void *context, GLenum target, GLenum internalformat,
                      GLuint buffer) {
    d_glTexBuffer(context, target, internalformat, buffer);
}

void d_glTexBufferRangeEXT(void *context, GLenum target, GLenum internalformat,
                           GLuint buffer, GLintptr offset, GLsizeiptr size) {
    d_glTexBufferRange(context, target, internalformat, buffer, offset, size);
}

void d_glColorMaski(void *context, GLuint buf, GLboolean red, GLboolean green,
                    GLboolean blue, GLboolean alpha) {
    //@todo error check
    //@todo status save
    if (buf == GL_BACK) {
        buf = GL_COLOR_ATTACHMENT0;
    }
    r_glColorMaski(context, buf, red, green, blue, alpha);
}

void d_glColorMaskiEXT(void *context, GLuint buf, GLboolean red,
                       GLboolean green, GLboolean blue, GLboolean alpha) {
    d_glColorMaski(context, buf, red, green, blue, alpha);
}

void d_glColorMaskiOES(void *context, GLuint buf, GLboolean red,
                       GLboolean green, GLboolean blue, GLboolean alpha) {
    d_glColorMaski(context, buf, red, green, blue, alpha);
}

void d_glBlendFuncSeparatei(void *context, GLuint buf, GLenum srcRGB,
                            GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {
    //@todo error check
    //@todo status save
    if (buf == GL_BACK) {
        buf = GL_COLOR_ATTACHMENT0;
    }
    r_glBlendFuncSeparatei(context, buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void d_glBlendFuncSeparateiEXT(void *context, GLuint buf, GLenum srcRGB,
                               GLenum dstRGB, GLenum srcAlpha,
                               GLenum dstAlpha) {
    d_glBlendFuncSeparatei(context, buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void d_glBlendFuncSeparateiOES(void *context, GLuint buf, GLenum srcRGB,
                               GLenum dstRGB, GLenum srcAlpha,
                               GLenum dstAlpha) {
    d_glBlendFuncSeparatei(context, buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void d_glBlendEquationSeparatei(void *context, GLuint buf, GLenum modeRGB,
                                GLenum modeAlpha) {
    //@todo error check
    //@todo status save
    if (buf == GL_BACK) {
        buf = GL_COLOR_ATTACHMENT0;
    }
    r_glBlendEquationSeparatei(context, buf, modeRGB, modeAlpha);
}

void d_glBlendEquationSeparateiEXT(void *context, GLuint buf, GLenum modeRGB,
                                   GLenum modeAlpha) {
    d_glBlendEquationSeparatei(context, buf, modeRGB, modeAlpha);
}

void d_glBlendEquationSeparateiOES(void *context, GLuint buf, GLenum modeRGB,
                                   GLenum modeAlpha) {
    d_glBlendEquationSeparatei(context, buf, modeRGB, modeAlpha);
}

/*TODO glBlendBarrier not implemented*/    // es3.2
/*TODO glCreateProgram not found*/         // glv3_resource.cpp
/*TODO glCreateShader not found*/          // glv3_resource.cpp
/*TODO glCreateShaderProgramv not found*/  // glv3_resource.cpp
/*TODO glIsBuffer not implemented*/        // glv3_resource.cpp
