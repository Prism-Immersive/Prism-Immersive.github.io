#ifndef GL_FUNC_LIST_H
#define GL_FUNC_LIST_H

#define VOID_DEFAULT return
#define RET_DEFAULT return 0

#define LIST_IMPL_GL_FUNCTIONS(X)                                              \
    X(GLenum, glClientWaitSync,                                                \
      (GLsync sync, GLbitfield flags, GLuint64 timeout), RET_DEFAULT,          \
      thread_ctx, sync, flags, timeout)                                        \
    X(void, glUniform2ui, (GLint location, GLuint v0, GLuint v1),              \
      VOID_DEFAULT, thread_ctx, location, v0, v1)                              \
    X(void, glClearDepthfOES, (GLclampf depth), VOID_DEFAULT, thread_ctx,      \
      depth)                                                                   \
    X(void, glUniformMatrix2x3fv,                                              \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glEnableVertexAttribArray, (GLuint index), VOID_DEFAULT,           \
      thread_ctx, index)                                                       \
    X(GLboolean, glIsQuery, (GLuint id), RET_DEFAULT, thread_ctx, id)          \
    X(void, glInvalidateSubFramebuffer,                                        \
      (GLenum target, GLsizei numAttachments, const GLenum *attachments,       \
       GLint x, GLint y, GLsizei width, GLsizei height),                       \
      VOID_DEFAULT, thread_ctx, target, numAttachments, attachments, x, y,     \
      width, height)                                                           \
    X(void, glGetIntegerv, (GLenum pname, GLint * data), VOID_DEFAULT,         \
      thread_ctx, pname, data)                                                 \
    X(void, glTexParameteri, (GLenum target, GLenum pname, GLint param),       \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glBindBufferBase, (GLenum target, GLuint index, GLuint buffer),    \
      VOID_DEFAULT, thread_ctx, target, index, buffer)                         \
    X(void, glGenQueries, (GLsizei n, GLuint * ids), VOID_DEFAULT, thread_ctx, \
      n, ids)                                                                  \
    X(void, glGetTransformFeedbackVarying,                                     \
      (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length,        \
       GLsizei * size, GLenum * type, GLchar * name),                          \
      VOID_DEFAULT, thread_ctx, program, index, bufSize, length, size, type,   \
      name)                                                                    \
    X(void, glDrawBuffers, (GLsizei n, const GLenum *bufs), VOID_DEFAULT,      \
      thread_ctx, n, bufs)                                                     \
    X(void, glVertexAttribIFormat,                                             \
      (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset),    \
      VOID_DEFAULT, thread_ctx, attribindex, size, type, relativeoffset)       \
    X(void, glGetActiveUniform,                                                \
      (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length,        \
       GLint * size, GLenum * type, GLchar * name),                            \
      VOID_DEFAULT, thread_ctx, program, index, bufSize, length, size, type,   \
      name)                                                                    \
    X(void, glColorMask,                                                       \
      (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha),       \
      VOID_DEFAULT, thread_ctx, red, green, blue, alpha)                       \
    X(void, glTexImage3D,                                                      \
      (GLenum target, GLint level, GLint internalformat, GLsizei width,        \
       GLsizei height, GLsizei depth, GLint border, GLenum format,             \
       GLenum type, const void *pixels),                                       \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, width, height,  \
      depth, border, format, type, pixels)                                     \
    X(void, glUniform4fv,                                                      \
      (GLint location, GLsizei count, const GLfloat *value), VOID_DEFAULT,     \
      thread_ctx, location, count, value)                                      \
    X(void, glDeleteSamplers, (GLsizei count, const GLuint *samplers),         \
      VOID_DEFAULT, thread_ctx, count, samplers)                               \
    X(void, glDeleteShader, (GLuint shader), VOID_DEFAULT, thread_ctx, shader) \
    X(void, glUniform4f,                                                       \
      (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3),        \
      VOID_DEFAULT, thread_ctx, location, v0, v1, v2, v3)                      \
    X(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count),           \
      VOID_DEFAULT, thread_ctx, mode, first, count)                            \
    X(void, glWaitSync, (GLsync sync, GLbitfield flags, GLuint64 timeout),     \
      VOID_DEFAULT, thread_ctx, sync, flags, timeout)                          \
    X(void, glPixelStorei, (GLenum pname, GLint param), VOID_DEFAULT,          \
      thread_ctx, pname, param)                                                \
    X(void, glGetShaderInfoLog,                                                \
      (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog),    \
      VOID_DEFAULT, thread_ctx, shader, bufSize, length, infoLog)              \
    X(void, glGetTexEnvxvOES, (GLenum target, GLenum pname, GLfixed * params), \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glGenVertexArraysOES, (GLsizei n, GLuint * arrays), VOID_DEFAULT,  \
      thread_ctx, n, arrays)                                                   \
    X(void, glProgramUniform3uiv,                                              \
      (GLuint program, GLint location, GLsizei count, const GLuint *value),    \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glSamplerParameteriv,                                              \
      (GLuint sampler, GLenum pname, const GLint *param), VOID_DEFAULT,        \
      thread_ctx, sampler, pname, param)                                       \
    X(void, glClearDepthxOES, (GLfixed depth), VOID_DEFAULT, thread_ctx,       \
      depth)                                                                   \
    X(void, glGetBooleanv, (GLenum pname, GLboolean * data), VOID_DEFAULT,     \
      thread_ctx, pname, data)                                                 \
    X(void, glClipPlanexOES, (GLenum plane, const GLfixed *equation),          \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glGenProgramPipelines, (GLsizei n, GLuint * pipelines),            \
      VOID_DEFAULT, thread_ctx, n, pipelines)                                  \
    X(void, glGetRenderbufferParameteriv,                                      \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(GLboolean, glIsRenderbuffer, (GLuint renderbuffer), RET_DEFAULT,         \
      thread_ctx, renderbuffer)                                                \
    X(void, glRenderbufferStorageMultisampleEXT,                               \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height),                                                        \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height)                                                                  \
    X(void, glUniform1f, (GLint location, GLfloat v0), VOID_DEFAULT,           \
      thread_ctx, location, v0)                                                \
    X(void, glStencilFunc, (GLenum func, GLint ref, GLuint mask),              \
      VOID_DEFAULT, thread_ctx, func, ref, mask)                               \
    X(GLboolean, glIsEnabled, (GLenum cap), RET_DEFAULT, thread_ctx, cap)      \
    X(void, glVertexAttribPointer,                                             \
      (GLuint index, GLint size, GLenum type, GLboolean normalized,            \
       GLsizei stride, const void *pointer),                                   \
      VOID_DEFAULT, thread_ctx, index, size, type, normalized, stride,         \
      pointer)                                                                 \
    X(void, glBindVertexBuffer,                                                \
      (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride),   \
      VOID_DEFAULT, thread_ctx, bindingindex, buffer, offset, stride)          \
    X(GLboolean, glIsShader, (GLuint shader), RET_DEFAULT, thread_ctx, shader) \
    X(void, glEndTransformFeedback, (void), VOID_DEFAULT, thread_ctx)          \
    X(void, glClearBufferfi,                                                   \
      (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil),         \
      VOID_DEFAULT, thread_ctx, buffer, drawbuffer, depth, stencil)            \
    X(void, glPolygonOffset, (GLfloat factor, GLfloat units), VOID_DEFAULT,    \
      thread_ctx, factor, units)                                               \
    X(void, glGetQueryiv, (GLenum target, GLenum pname, GLint * params),       \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glProgramUniform3ui,                                               \
      (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2),       \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2)                 \
    X(void, glVertexAttrib1f, (GLuint index, GLfloat x), VOID_DEFAULT,         \
      thread_ctx, index, x)                                                    \
    X(void, glVertexAttrib3fv, (GLuint index, const GLfloat *v), VOID_DEFAULT, \
      thread_ctx, index, v)                                                    \
    X(void, glVertexAttrib2f, (GLuint index, GLfloat x, GLfloat y),            \
      VOID_DEFAULT, thread_ctx, index, x, y)                                   \
    X(void, glRotatexOES, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z),    \
      VOID_DEFAULT, thread_ctx, angle, x, y, z)                                \
    X(void, glUniform4i,                                                       \
      (GLint location, GLint v0, GLint v1, GLint v2, GLint v3), VOID_DEFAULT,  \
      thread_ctx, location, v0, v1, v2, v3)                                    \
    X(void, glMultMatrixxOES, (const GLfixed *m), VOID_DEFAULT, thread_ctx, m) \
    X(void, glVertexAttribI4ui,                                                \
      (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w), VOID_DEFAULT,    \
      thread_ctx, index, x, y, z, w)                                           \
    X(void, glGetTexLevelParameterfv,                                          \
      (GLenum target, GLint level, GLenum pname, GLfloat * params),            \
      VOID_DEFAULT, thread_ctx, target, level, pname, params)                  \
    X(void, glBindTexture, (GLenum target, GLuint texture), VOID_DEFAULT,      \
      thread_ctx, target, texture)                                             \
    X(void, glProgramUniform4uiv,                                              \
      (GLuint program, GLint location, GLsizei count, const GLuint *value),    \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glTexEnvxvOES,                                                     \
      (GLenum target, GLenum pname, const GLfixed *params), VOID_DEFAULT,      \
      thread_ctx, target, pname, params)                                       \
    X(void, glTexParameterx, (GLenum target, GLenum pname, GLfixed param),     \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glClearColor,                                                      \
      (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glDeleteVertexArrays, (GLsizei n, const GLuint *arrays),           \
      VOID_DEFAULT, thread_ctx, n, arrays)                                     \
    X(void, glProgramUniform1uiv,                                              \
      (GLuint program, GLint location, GLsizei count, const GLuint *value),    \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glTexParameteriv,                                                  \
      (GLenum target, GLenum pname, const GLint *params), VOID_DEFAULT,        \
      thread_ctx, target, pname, params)                                       \
    X(void, glDrawElementsIndirect,                                            \
      (GLenum mode, GLenum type, const void *indirect), VOID_DEFAULT,          \
      thread_ctx, mode, type, indirect)                                        \
    X(GLboolean, glIsSync, (GLsync sync), RET_DEFAULT, thread_ctx, sync)       \
    X(void, glGetUniformiv, (GLuint program, GLint location, GLint * params),  \
      VOID_DEFAULT, thread_ctx, program, location, params)                     \
    X(void, glUniform1iv, (GLint location, GLsizei count, const GLint *value), \
      VOID_DEFAULT, thread_ctx, location, count, value)                        \
    X(void, glGenRenderbuffers, (GLsizei n, GLuint * renderbuffers),           \
      VOID_DEFAULT, thread_ctx, n, renderbuffers)                              \
    X(void, glVertexAttribDivisor, (GLuint index, GLuint divisor),             \
      VOID_DEFAULT, thread_ctx, index, divisor)                                \
    X(void, glTexParameterfv,                                                  \
      (GLenum target, GLenum pname, const GLfloat *params), VOID_DEFAULT,      \
      thread_ctx, target, pname, params)                                       \
    X(void, glGetProgramPipelineInfoLog,                                       \
      (GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog),  \
      VOID_DEFAULT, thread_ctx, pipeline, bufSize, length, infoLog)            \
    X(void, glTexSubImage2D,                                                   \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLsizei width, GLsizei height, GLenum format, GLenum type,              \
       const void *pixels),                                                    \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, width,        \
      height, format, type, pixels)                                            \
    X(void, glLinkProgram, (GLuint program), VOID_DEFAULT, thread_ctx,         \
      program)                                                                 \
    X(void, glBindImageTexture,                                                \
      (GLuint unit, GLuint texture, GLint level, GLboolean layered,            \
       GLint layer, GLenum access, GLenum format),                             \
      VOID_DEFAULT, thread_ctx, unit, texture, level, layered, layer, access,  \
      format)                                                                  \
    X(void, glLineWidth, (GLfloat width), VOID_DEFAULT, thread_ctx, width)     \
    X(void, glUniformMatrix3fv,                                                \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glProgramUniform2uiv,                                              \
      (GLuint program, GLint location, GLsizei count, const GLuint *value),    \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glTexStorage2D,                                                    \
      (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width,    \
       GLsizei height),                                                        \
      VOID_DEFAULT, thread_ctx, target, levels, internalformat, width, height) \
    X(void, glUniformMatrix2fv,                                                \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glUniform3fv,                                                      \
      (GLint location, GLsizei count, const GLfloat *value), VOID_DEFAULT,     \
      thread_ctx, location, count, value)                                      \
    X(void, glVertexAttribI4uiv, (GLuint index, const GLuint *v),              \
      VOID_DEFAULT, thread_ctx, index, v)                                      \
    X(void, glUniformMatrix4x2fv,                                              \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glUniform2uiv,                                                     \
      (GLint location, GLsizei count, const GLuint *value), VOID_DEFAULT,      \
      thread_ctx, location, count, value)                                      \
    X(void, glLightxOES, (GLenum light, GLenum pname, GLfixed param),          \
      VOID_DEFAULT, thread_ctx, light, pname, param)                           \
    X(void, glProgramUniform1i, (GLuint program, GLint location, GLint v0),    \
      VOID_DEFAULT, thread_ctx, program, location, v0)                         \
    X(void, glBeginTransformFeedback, (GLenum primitiveMode), VOID_DEFAULT,    \
      thread_ctx, primitiveMode)                                               \
    X(void, glTestPointer1, (GLint a, const GLint *b), VOID_DEFAULT,           \
      thread_ctx, a, b)                                                        \
    X(void, glTexParameterf, (GLenum target, GLenum pname, GLfloat param),     \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glRenderbufferStorage,                                             \
      (GLenum target, GLenum internalformat, GLsizei width, GLsizei height),   \
      VOID_DEFAULT, thread_ctx, target, internalformat, width, height)         \
    X(GLboolean, glIsSampler, (GLuint sampler), RET_DEFAULT, thread_ctx,       \
      sampler)                                                                 \
    X(GLsync, glFenceSync, (GLenum condition, GLbitfield flags), RET_DEFAULT,  \
      thread_ctx, condition, flags)                                            \
    X(void, glDispatchComputeIndirect, (GLintptr indirect), VOID_DEFAULT,      \
      thread_ctx, indirect)                                                    \
    X(void, glBlendColor,                                                      \
      (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(GLuint, glTestInt2, (GLint a, GLuint b), RET_DEFAULT, thread_ctx, a, b)  \
    X(GLuint, glGetUniformBlockIndex,                                          \
      (GLuint program, const GLchar *uniformBlockName), RET_DEFAULT,           \
      thread_ctx, program, uniformBlockName)                                   \
    X(void, glCompressedTexSubImage2D,                                         \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLsizei width, GLsizei height, GLenum format, GLsizei imageSize,        \
       const void *data),                                                      \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, width,        \
      height, format, imageSize, data)                                         \
    X(void, glGetSamplerParameterfv,                                           \
      (GLuint sampler, GLenum pname, GLfloat * params), VOID_DEFAULT,          \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glFinish, (void), VOID_DEFAULT, thread_ctx)                        \
    X(void, glCompressedTexSubImage3D,                                         \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLenum format, GLsizei imageSize, const void *data),                    \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset,      \
      width, height, depth, format, imageSize, data)                           \
    X(void, glLoadMatrixxOES, (const GLfixed *m), VOID_DEFAULT, thread_ctx, m) \
    X(void, glUniformMatrix3x4fv,                                              \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glFramebufferTextureLayer,                                         \
      (GLenum target, GLenum attachment, GLuint texture, GLint level,          \
       GLint layer),                                                           \
      VOID_DEFAULT, thread_ctx, target, attachment, texture, level, layer)     \
    X(void, glDeleteTransformFeedbacks, (GLsizei n, const GLuint *ids),        \
      VOID_DEFAULT, thread_ctx, n, ids)                                        \
    X(void, glGetInteger64i_v, (GLenum target, GLuint index, GLint64 * data),  \
      VOID_DEFAULT, thread_ctx, target, index, data)                           \
    X(void, glUniform2i, (GLint location, GLint v0, GLint v1), VOID_DEFAULT,   \
      thread_ctx, location, v0, v1)                                            \
    X(void, glSampleCoverage, (GLfloat value, GLboolean invert), VOID_DEFAULT, \
      thread_ctx, value, invert)                                               \
    X(void, glDeleteFramebuffers, (GLsizei n, const GLuint *framebuffers),     \
      VOID_DEFAULT, thread_ctx, n, framebuffers)                               \
    X(void, glUniformMatrix4fv,                                                \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glTexSubImage3D,                                                   \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLenum format, GLenum type, const void *pixels),                        \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset,      \
      width, height, depth, format, type, pixels)                              \
    X(void, glEnable, (GLenum cap), VOID_DEFAULT, thread_ctx, cap)             \
    X(void, glGetActiveUniformBlockiv,                                         \
      (GLuint program, GLuint uniformBlockIndex, GLenum pname,                 \
       GLint * params),                                                        \
      VOID_DEFAULT, thread_ctx, program, uniformBlockIndex, pname, params)     \
    X(void, glProgramUniform4i,                                                \
      (GLuint program, GLint location, GLint v0, GLint v1, GLint v2,           \
       GLint v3),                                                              \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2, v3)             \
    X(void, glUniform4ui,                                                      \
      (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3),            \
      VOID_DEFAULT, thread_ctx, location, v0, v1, v2, v3)                      \
    X(void, glStencilMaskSeparate, (GLenum face, GLuint mask), VOID_DEFAULT,   \
      thread_ctx, face, mask)                                                  \
    X(GLuint, glCreateShader, (GLenum type), RET_DEFAULT, thread_ctx, type)    \
    X(void, glDrawElements,                                                    \
      (GLenum mode, GLsizei count, GLenum type, const void *indices),          \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices)                    \
    X(void, glGetProgramBinary,                                                \
      (GLuint program, GLsizei bufSize, GLsizei * length,                      \
       GLenum * binaryFormat, void *binary),                                   \
      VOID_DEFAULT, thread_ctx, program, bufSize, length, binaryFormat,        \
      binary)                                                                  \
    X(void, glShaderSource,                                                    \
      (GLuint shader, GLsizei count, const GLchar *const *string,              \
       const GLint *length),                                                   \
      VOID_DEFAULT, thread_ctx, shader, count, string, length)                 \
    X(void, glGenTextures, (GLsizei n, GLuint * textures), VOID_DEFAULT,       \
      thread_ctx, n, textures)                                                 \
    X(void, glGetVertexAttribiv, (GLuint index, GLenum pname, GLint * params), \
      VOID_DEFAULT, thread_ctx, index, pname, params)                          \
    X(void, glProgramUniform1fv,                                               \
      (GLuint program, GLint location, GLsizei count, const GLfloat *value),   \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glProgramUniform3iv,                                               \
      (GLuint program, GLint location, GLsizei count, const GLint *value),     \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glStencilMask, (GLuint mask), VOID_DEFAULT, thread_ctx, mask)      \
    X(void, glGetFramebufferAttachmentParameteriv,                             \
      (GLenum target, GLenum attachment, GLenum pname, GLint * params),        \
      VOID_DEFAULT, thread_ctx, target, attachment, pname, params)             \
    X(void, glValidateProgram, (GLuint program), VOID_DEFAULT, thread_ctx,     \
      program)                                                                 \
    X(void, glProgramUniform2iv,                                               \
      (GLuint program, GLint location, GLsizei count, const GLint *value),     \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glGenVertexArrays, (GLsizei n, GLuint * arrays), VOID_DEFAULT,     \
      thread_ctx, n, arrays)                                                   \
    X(void, glClearDepthf, (GLfloat d), VOID_DEFAULT, thread_ctx, d)           \
    X(void, glClipPlanefOES, (GLenum plane, const GLfloat *equation),          \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glBufferSubData,                                                   \
      (GLenum target, GLintptr offset, GLsizeiptr size, const void *data),     \
      VOID_DEFAULT, thread_ctx, target, offset, size, data)                    \
    X(void, glDeleteVertexArraysOES, (GLsizei n, const GLuint *arrays),        \
      VOID_DEFAULT, thread_ctx, n, arrays)                                     \
    X(void, glHint, (GLenum target, GLenum mode), VOID_DEFAULT, thread_ctx,    \
      target, mode)                                                            \
    X(void, glTexGenxvOES,                                                     \
      (GLenum coord, GLenum pname, const GLfixed *params), VOID_DEFAULT,       \
      thread_ctx, coord, pname, params)                                        \
    X(void, glGetShaderPrecisionFormat,                                        \
      (GLenum shadertype, GLenum precisiontype, GLint * range,                 \
       GLint * precision),                                                     \
      VOID_DEFAULT, thread_ctx, shadertype, precisiontype, range, precision)   \
    X(void, glDeleteTextures, (GLsizei n, const GLuint *textures),             \
      VOID_DEFAULT, thread_ctx, n, textures)                                   \
    X(void, glFlush, (void), VOID_DEFAULT, thread_ctx)                         \
    X(void, glCopyTexSubImage2D,                                               \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x,      \
       GLint y, GLsizei width, GLsizei height),                                \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, x, y, width,  \
      height)                                                                  \
    X(GLboolean, glIsBuffer, (GLuint buffer), RET_DEFAULT, thread_ctx, buffer) \
    X(void, glProgramUniform2ui,                                               \
      (GLuint program, GLint location, GLuint v0, GLuint v1), VOID_DEFAULT,    \
      thread_ctx, program, location, v0, v1)                                   \
    X(void, glCompressedTexImage2D,                                            \
      (GLenum target, GLint level, GLenum internalformat, GLsizei width,       \
       GLsizei height, GLint border, GLsizei imageSize, const void *data),     \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, width, height,  \
      border, imageSize, data)                                                 \
    X(void, glProgramUniformMatrix3x4fv,                                       \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glPointSizexOES, (GLfixed size), VOID_DEFAULT, thread_ctx, size)   \
    X(void, glGetUniformuiv,                                                   \
      (GLuint program, GLint location, GLuint * params), VOID_DEFAULT,         \
      thread_ctx, program, location, params)                                   \
    X(void, glClearBufferiv,                                                   \
      (GLenum buffer, GLint drawbuffer, const GLint *value), VOID_DEFAULT,     \
      thread_ctx, buffer, drawbuffer, value)                                   \
    X(void, glTexStorage3D,                                                    \
      (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width,    \
       GLsizei height, GLsizei depth),                                         \
      VOID_DEFAULT, thread_ctx, target, levels, internalformat, width, height, \
      depth)                                                                   \
    X(void, glUniformMatrix2x4fv,                                              \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glGetTexParameteriv,                                               \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glShaderBinary,                                                    \
      (GLsizei count, const GLuint *shaders, GLenum binaryformat,              \
       const void *binary, GLsizei length),                                    \
      VOID_DEFAULT, thread_ctx, count, shaders, binaryformat, binary, length)  \
    X(void, glMaterialxOES, (GLenum face, GLenum pname, GLfixed param),        \
      VOID_DEFAULT, thread_ctx, face, pname, param)                            \
    X(void, glBlendEquationSeparate, (GLenum modeRGB, GLenum modeAlpha),       \
      VOID_DEFAULT, thread_ctx, modeRGB, modeAlpha)                            \
    X(void, glUniformMatrix4x3fv,                                              \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glUniform4iv, (GLint location, GLsizei count, const GLint *value), \
      VOID_DEFAULT, thread_ctx, location, count, value)                        \
    X(void, glAttachShader, (GLuint program, GLuint shader), VOID_DEFAULT,     \
      thread_ctx, program, shader)                                             \
    X(void, glGetSynciv,                                                       \
      (GLsync sync, GLenum pname, GLsizei count, GLsizei * length,             \
       GLint * values),                                                        \
      VOID_DEFAULT, thread_ctx, sync, pname, count, length, values)            \
    X(void, glUniformMatrix3x2fv,                                              \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glTexEnvx, (GLenum target, GLenum pname, GLfixed param),           \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glBlendFuncSeparate,                                               \
      (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha,              \
       GLenum dfactorAlpha),                                                   \
      VOID_DEFAULT, thread_ctx, sfactorRGB, dfactorRGB, sfactorAlpha,          \
      dfactorAlpha)                                                            \
    X(void, glDepthRangef, (GLfloat n, GLfloat f), VOID_DEFAULT, thread_ctx,   \
      n, f)                                                                    \
    X(void, glCompressedTexImage3D,                                            \
      (GLenum target, GLint level, GLenum internalformat, GLsizei width,       \
       GLsizei height, GLsizei depth, GLint border, GLsizei imageSize,         \
       const void *data),                                                      \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, width, height,  \
      depth, border, imageSize, data)                                          \
    X(void, glTexEnvxOES, (GLenum target, GLenum pname, GLfixed param),        \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glGetProgramInfoLog,                                               \
      (GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog),   \
      VOID_DEFAULT, thread_ctx, program, bufSize, length, infoLog)             \
    X(void, glSamplerParameteri, (GLuint sampler, GLenum pname, GLint param),  \
      VOID_DEFAULT, thread_ctx, sampler, pname, param)                         \
    X(void, glCopyBufferSubData,                                               \
      (GLenum readTarget, GLenum writeTarget, GLintptr readOffset,             \
       GLintptr writeOffset, GLsizeiptr size),                                 \
      VOID_DEFAULT, thread_ctx, readTarget, writeTarget, readOffset,           \
      writeOffset, size)                                                       \
    X(void, glClear, (GLbitfield mask), VOID_DEFAULT, thread_ctx, mask)        \
    X(GLboolean, glUnmapBuffer, (GLenum target), RET_DEFAULT, thread_ctx,      \
      target)                                                                  \
    X(void, glBeginQuery, (GLenum target, GLuint id), VOID_DEFAULT,            \
      thread_ctx, target, id)                                                  \
    X(void, glDrawArraysInstanced,                                             \
      (GLenum mode, GLint first, GLsizei count, GLsizei instancecount),        \
      VOID_DEFAULT, thread_ctx, mode, first, count, instancecount)             \
    X(void, glUniform4uiv,                                                     \
      (GLint location, GLsizei count, const GLuint *value), VOID_DEFAULT,      \
      thread_ctx, location, count, value)                                      \
    X(void, glProgramUniformMatrix3fv,                                         \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(GLboolean, glIsProgram, (GLuint program), RET_DEFAULT, thread_ctx,       \
      program)                                                                 \
    X(GLint, glGetAttribLocation, (GLuint program, const GLchar *name),        \
      RET_DEFAULT, thread_ctx, program, name)                                  \
    X(void, glGetBooleani_v, (GLenum target, GLuint index, GLboolean * data),  \
      VOID_DEFAULT, thread_ctx, target, index, data)                           \
    X(void, glGetVertexAttribIiv,                                              \
      (GLuint index, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx,  \
      index, pname, params)                                                    \
    X(void, glVertexAttrib4fv, (GLuint index, const GLfloat *v), VOID_DEFAULT, \
      thread_ctx, index, v)                                                    \
    X(GLboolean, glIsTransformFeedback, (GLuint id), RET_DEFAULT, thread_ctx,  \
      id)                                                                      \
    X(void, glSampleMaski, (GLuint maskNumber, GLbitfield mask), VOID_DEFAULT, \
      thread_ctx, maskNumber, mask)                                            \
    X(GLuint, glGetProgramResourceIndex,                                       \
      (GLuint program, GLenum programInterface, const GLchar *name),           \
      RET_DEFAULT, thread_ctx, program, programInterface, name)                \
    X(GLboolean, glIsTexture, (GLuint texture), RET_DEFAULT, thread_ctx,       \
      texture)                                                                 \
    X(void, glBindSampler, (GLuint unit, GLuint sampler), VOID_DEFAULT,        \
      thread_ctx, unit, sampler)                                               \
    X(void, glGetActiveAttrib,                                                 \
      (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length,        \
       GLint * size, GLenum * type, GLchar * name),                            \
      VOID_DEFAULT, thread_ctx, program, index, bufSize, length, size, type,   \
      name)                                                                    \
    X(void, glBindFramebuffer, (GLenum target, GLuint framebuffer),            \
      VOID_DEFAULT, thread_ctx, target, framebuffer)                           \
    X(void, glUniform3iv, (GLint location, GLsizei count, const GLint *value), \
      VOID_DEFAULT, thread_ctx, location, count, value)                        \
    X(void, glBindProgramPipeline, (GLuint pipeline), VOID_DEFAULT,            \
      thread_ctx, pipeline)                                                    \
    X(void, glVertexAttribIPointer,                                            \
      (GLuint index, GLint size, GLenum type, GLsizei stride,                  \
       const void *pointer),                                                   \
      VOID_DEFAULT, thread_ctx, index, size, type, stride, pointer)            \
    X(void, glUniform3f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2), \
      VOID_DEFAULT, thread_ctx, location, v0, v1, v2)                          \
    X(GLboolean, glIsFramebuffer, (GLuint framebuffer), RET_DEFAULT,           \
      thread_ctx, framebuffer)                                                 \
    X(void, glScissor, (GLint x, GLint y, GLsizei width, GLsizei height),      \
      VOID_DEFAULT, thread_ctx, x, y, width, height)                           \
    X(const GLubyte *, glGetString, (GLenum name), RET_DEFAULT, thread_ctx,    \
      name)                                                                    \
    X(void, glUseProgram, (GLuint program), VOID_DEFAULT, thread_ctx, program) \
    X(void, glGetProgramiv, (GLuint program, GLenum pname, GLint * params),    \
      VOID_DEFAULT, thread_ctx, program, pname, params)                        \
    X(void, glVertexAttribBinding, (GLuint attribindex, GLuint bindingindex),  \
      VOID_DEFAULT, thread_ctx, attribindex, bindingindex)                     \
    X(void, glUniformBlockBinding,                                             \
      (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding),  \
      VOID_DEFAULT, thread_ctx, program, uniformBlockIndex,                    \
      uniformBlockBinding)                                                     \
    X(void, glBindTransformFeedback, (GLenum target, GLuint id), VOID_DEFAULT, \
      thread_ctx, target, id)                                                  \
    X(void, glPointParameterxvOES, (GLenum pname, const GLfixed *params),      \
      VOID_DEFAULT, thread_ctx, pname, params)                                 \
    X(void, glProgramUniform1ui, (GLuint program, GLint location, GLuint v0),  \
      VOID_DEFAULT, thread_ctx, program, location, v0)                         \
    X(void, glPrintf, (GLint buf_len, GLchar * out_string), VOID_DEFAULT,      \
      thread_ctx, buf_len, out_string)                                         \
    X(GLenum, glGetError, (void), RET_DEFAULT, thread_ctx)                     \
    X(void, glGenTransformFeedbacks, (GLsizei n, GLuint * ids), VOID_DEFAULT,  \
      thread_ctx, n, ids)                                                      \
    X(GLuint64, glTestInt4, (GLint64 a, GLuint64 b), RET_DEFAULT, thread_ctx,  \
      a, b)                                                                    \
    X(void, glCopyTexSubImage3D,                                               \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height),        \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset, x,   \
      y, width, height)                                                        \
    X(void, glLightModelxOES, (GLenum pname, GLfixed param), VOID_DEFAULT,     \
      thread_ctx, pname, param)                                                \
    X(void, glTransformFeedbackVaryings,                                       \
      (GLuint program, GLsizei count, const GLchar *const *varyings,           \
       GLenum bufferMode),                                                     \
      VOID_DEFAULT, thread_ctx, program, count, varyings, bufferMode)          \
    X(void, glGetTexParameterfv,                                               \
      (GLenum target, GLenum pname, GLfloat * params), VOID_DEFAULT,           \
      thread_ctx, target, pname, params)                                       \
    X(void, glGetFixedvOES, (GLenum pname, GLfixed * params), VOID_DEFAULT,    \
      thread_ctx, pname, params)                                               \
    X(const GLubyte *, glGetStringi, (GLenum name, GLuint index), RET_DEFAULT, \
      thread_ctx, name, index)                                                 \
    X(void, glValidateProgramPipeline, (GLuint pipeline), VOID_DEFAULT,        \
      thread_ctx, pipeline)                                                    \
    X(void, glEndQuery, (GLenum target), VOID_DEFAULT, thread_ctx, target)     \
    X(void, glBindAttribLocation,                                              \
      (GLuint program, GLuint index, const GLchar *name), VOID_DEFAULT,        \
      thread_ctx, program, index, name)                                        \
    X(void, glGetBufferParameteriv,                                            \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glFrustumfOES,                                                     \
      (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glProgramUniform3fv,                                               \
      (GLuint program, GLint location, GLsizei count, const GLfloat *value),   \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glGetInternalformativ,                                             \
      (GLenum target, GLenum internalformat, GLenum pname, GLsizei count,      \
       GLint * params),                                                        \
      VOID_DEFAULT, thread_ctx, target, internalformat, pname, count, params)  \
    X(void, glGetIntegeri_v, (GLenum target, GLuint index, GLint * data),      \
      VOID_DEFAULT, thread_ctx, target, index, data)                           \
    X(void, glRenderbufferStorageMultisample,                                  \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height),                                                        \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height)                                                                  \
    X(void, glTestPointer2, (GLint a, const GLint *b, GLint *c), VOID_DEFAULT, \
      thread_ctx, a, b, c)                                                     \
    X(GLint, glGetUniformLocation, (GLuint program, const GLchar *name),       \
      RET_DEFAULT, thread_ctx, program, name)                                  \
    X(void, glFrustumxOES,                                                     \
      (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glBindBufferRange,                                                 \
      (GLenum target, GLuint index, GLuint buffer, GLintptr offset,            \
       GLsizeiptr size),                                                       \
      VOID_DEFAULT, thread_ctx, target, index, buffer, offset, size)           \
    X(void, glDeleteProgram, (GLuint program), VOID_DEFAULT, thread_ctx,       \
      program)                                                                 \
    X(GLdouble, glTestInt6, (GLint a, GLuint b), RET_DEFAULT, thread_ctx, a,   \
      b)                                                                       \
    X(void, glProgramUniform2fv,                                               \
      (GLuint program, GLint location, GLsizei count, const GLfloat *value),   \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glGetFramebufferParameteriv,                                       \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glVertexAttribFormat,                                              \
      (GLuint attribindex, GLint size, GLenum type, GLboolean normalized,      \
       GLuint relativeoffset),                                                 \
      VOID_DEFAULT, thread_ctx, attribindex, size, type, normalized,           \
      relativeoffset)                                                          \
    X(void, glFogxOES, (GLenum pname, GLfixed param), VOID_DEFAULT,            \
      thread_ctx, pname, param)                                                \
    X(void, glGetBufferParameteri64v,                                          \
      (GLenum target, GLenum pname, GLint64 * params), VOID_DEFAULT,           \
      thread_ctx, target, pname, params)                                       \
    X(void, glClearBufferuiv,                                                  \
      (GLenum buffer, GLint drawbuffer, const GLuint *value), VOID_DEFAULT,    \
      thread_ctx, buffer, drawbuffer, value)                                   \
    X(void, glGetVertexAttribfv,                                               \
      (GLuint index, GLenum pname, GLfloat * params), VOID_DEFAULT,            \
      thread_ctx, index, pname, params)                                        \
    X(void, glDepthFunc, (GLenum func), VOID_DEFAULT, thread_ctx, func)        \
    X(void, glReadBuffer, (GLenum src), VOID_DEFAULT, thread_ctx, src)         \
    X(void, glFramebufferTexture2D,                                            \
      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture,     \
       GLint level),                                                           \
      VOID_DEFAULT, thread_ctx, target, attachment, textarget, texture, level) \
    X(void, glVertexAttrib1fv, (GLuint index, const GLfloat *v), VOID_DEFAULT, \
      thread_ctx, index, v)                                                    \
    X(void, glDrawTexiOES,                                                     \
      (GLint x, GLint y, GLint z, GLint width, GLint height), VOID_DEFAULT,    \
      thread_ctx, x, y, z, width, height)                                      \
    X(void, glGetActiveUniformsiv,                                             \
      (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices,     \
       GLenum pname, GLint *params),                                           \
      VOID_DEFAULT, thread_ctx, program, uniformCount, uniformIndices, pname,  \
      params)                                                                  \
    X(void, glScalexOES, (GLfixed x, GLfixed y, GLfixed z), VOID_DEFAULT,      \
      thread_ctx, x, y, z)                                                     \
    X(GLint, glTestInt1, (GLint a, GLuint b), RET_DEFAULT, thread_ctx, a, b)   \
    X(void, glTexGenxOES, (GLenum coord, GLenum pname, GLfixed param),         \
      VOID_DEFAULT, thread_ctx, coord, pname, param)                           \
    X(void, glBlitFramebuffer,                                                 \
      (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0,        \
       GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter), \
      VOID_DEFAULT, thread_ctx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0,      \
      dstX1, dstY1, mask, filter)                                              \
    X(void, glBindVertexArrayOES, (GLuint array), VOID_DEFAULT, thread_ctx,    \
      array)                                                                   \
    X(void, glBlendEquation, (GLenum mode), VOID_DEFAULT, thread_ctx, mode)    \
    X(void, glPrintfAsyn,                                                      \
      (GLint a, GLuint size, GLdouble c, const GLchar *out_string),            \
      VOID_DEFAULT, thread_ctx, a, size, c, out_string)                        \
    X(void, glGetActiveUniformBlockName,                                       \
      (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize,              \
       GLsizei * length, GLchar * uniformBlockName),                           \
      VOID_DEFAULT, thread_ctx, program, uniformBlockIndex, bufSize, length,   \
      uniformBlockName)                                                        \
    X(GLuint, glCreateProgram, (void), RET_DEFAULT, thread_ctx)                \
    X(void, glDrawElementsInstanced,                                           \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei instancecount),                                                 \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, instancecount)     \
    X(void, glCopyTexImage2D,                                                  \
      (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y,    \
       GLsizei width, GLsizei height, GLint border),                           \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, x, y, width,    \
      height, border)                                                          \
    X(void, glLightxvOES, (GLenum light, GLenum pname, const GLfixed *params), \
      VOID_DEFAULT, thread_ctx, light, pname, params)                          \
    X(void, glGenerateMipmap, (GLenum target), VOID_DEFAULT, thread_ctx,       \
      target)                                                                  \
    X(void, glDrawArraysIndirect, (GLenum mode, const void *indirect),         \
      VOID_DEFAULT, thread_ctx, mode, indirect)                                \
    X(void, glBlendFunc, (GLenum sfactor, GLenum dfactor), VOID_DEFAULT,       \
      thread_ctx, sfactor, dfactor)                                            \
    X(void, glGetTexParameterxvOES,                                            \
      (GLenum target, GLenum pname, GLfixed * params), VOID_DEFAULT,           \
      thread_ctx, target, pname, params)                                       \
    X(void, glGetShaderSource,                                                 \
      (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source),     \
      VOID_DEFAULT, thread_ctx, shader, bufSize, length, source)               \
    X(void, glProgramUniform1f, (GLuint program, GLint location, GLfloat v0),  \
      VOID_DEFAULT, thread_ctx, program, location, v0)                         \
    X(void, glPointParameterxOES, (GLenum pname, GLfixed param), VOID_DEFAULT, \
      thread_ctx, pname, param)                                                \
    X(void, glGetProgramPipelineiv,                                            \
      (GLuint pipeline, GLenum pname, GLint * params), VOID_DEFAULT,           \
      thread_ctx, pipeline, pname, params)                                     \
    X(void, glProgramUniformMatrix4x3fv,                                       \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void *, glMapBufferRange,                                                \
      (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access),  \
      RET_DEFAULT, thread_ctx, target, offset, length, access)                 \
    X(void, glUniform3uiv,                                                     \
      (GLint location, GLsizei count, const GLuint *value), VOID_DEFAULT,      \
      thread_ctx, location, count, value)                                      \
    X(void, glBindVertexArray, (GLuint array), VOID_DEFAULT, thread_ctx,       \
      array)                                                                   \
    X(void, glTranslatexOES, (GLfixed x, GLfixed y, GLfixed z), VOID_DEFAULT,  \
      thread_ctx, x, y, z)                                                     \
    X(void, glPauseTransformFeedback, (void), VOID_DEFAULT, thread_ctx)        \
    X(void, glVertexAttrib3f, (GLuint index, GLfloat x, GLfloat y, GLfloat z), \
      VOID_DEFAULT, thread_ctx, index, x, y, z)                                \
    X(void, glGetMaterialxvOES, (GLenum face, GLenum pname, GLfixed * params), \
      VOID_DEFAULT, thread_ctx, face, pname, params)                           \
    X(void, glShadeModel, (GLenum mode), VOID_DEFAULT, thread_ctx, mode)       \
    X(void, glUniform1i, (GLint location, GLint v0), VOID_DEFAULT, thread_ctx, \
      location, v0)                                                            \
    X(void, glTestIntAsyn, (GLint a, GLuint b, GLfloat c, GLdouble d),         \
      VOID_DEFAULT, thread_ctx, a, b, c, d)                                    \
    X(GLint, glGetProgramResourceLocation,                                     \
      (GLuint program, GLenum programInterface, const GLchar *name),           \
      RET_DEFAULT, thread_ctx, program, programInterface, name)                \
    X(void, glFogxvOES, (GLenum pname, const GLfixed *param), VOID_DEFAULT,    \
      thread_ctx, pname, param)                                                \
    X(GLint, glTestPointer3, (GLint a, const GLint *b, GLint *c), RET_DEFAULT, \
      thread_ctx, a, b, c)                                                     \
    X(void, glGetInteger64v, (GLenum pname, GLint64 * data), VOID_DEFAULT,     \
      thread_ctx, pname, data)                                                 \
    X(void, glUniform3i, (GLint location, GLint v0, GLint v1, GLint v2),       \
      VOID_DEFAULT, thread_ctx, location, v0, v1, v2)                          \
    X(void, glProgramUniformMatrix4x2fv,                                       \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glFlushMappedBufferRange,                                          \
      (GLenum target, GLintptr offset, GLsizeiptr length), VOID_DEFAULT,       \
      thread_ctx, target, offset, length)                                      \
    X(void, glClearColorxOES,                                                  \
      (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glReadPixels,                                                      \
      (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format,         \
       GLenum type, void *pixels),                                             \
      VOID_DEFAULT, thread_ctx, x, y, width, height, format, type, pixels)     \
    X(void, glFramebufferParameteri,                                           \
      (GLenum target, GLenum pname, GLint param), VOID_DEFAULT, thread_ctx,    \
      target, pname, param)                                                    \
    X(void, glGetBufferPointerv, (GLenum target, GLenum pname, void **params), \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glGetClipPlanexOES, (GLenum plane, GLfixed * equation),            \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glSamplerParameterfv,                                              \
      (GLuint sampler, GLenum pname, const GLfloat *param), VOID_DEFAULT,      \
      thread_ctx, sampler, pname, param)                                       \
    X(void, glVertexBindingDivisor, (GLuint bindingindex, GLuint divisor),     \
      VOID_DEFAULT, thread_ctx, bindingindex, divisor)                         \
    X(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint * params),      \
      VOID_DEFAULT, thread_ctx, shader, pname, params)                         \
    X(GLint, glGetFragDataLocation, (GLuint program, const GLchar *name),      \
      RET_DEFAULT, thread_ctx, program, name)                                  \
    X(void, glDeleteQueries, (GLsizei n, const GLuint *ids), VOID_DEFAULT,     \
      thread_ctx, n, ids)                                                      \
    X(void, glPolygonOffsetxOES, (GLfixed factor, GLfixed units),              \
      VOID_DEFAULT, thread_ctx, factor, units)                                 \
    X(void, glMemoryBarrierByRegion, (GLbitfield barriers), VOID_DEFAULT,      \
      thread_ctx, barriers)                                                    \
    X(void, glGetTexGenxvOES, (GLenum coord, GLenum pname, GLfixed * params),  \
      VOID_DEFAULT, thread_ctx, coord, pname, params)                          \
    X(void, glFrontFace, (GLenum mode), VOID_DEFAULT, thread_ctx, mode)        \
    X(void, glDeleteSync, (GLsync sync), VOID_DEFAULT, thread_ctx, sync)       \
    X(void, glSamplerParameterf,                                               \
      (GLuint sampler, GLenum pname, GLfloat param), VOID_DEFAULT, thread_ctx, \
      sampler, pname, param)                                                   \
    X(void, glDepthRangexOES, (GLfixed n, GLfixed f), VOID_DEFAULT,            \
      thread_ctx, n, f)                                                        \
    X(void, glCompileShader, (GLuint shader), VOID_DEFAULT, thread_ctx,        \
      shader)                                                                  \
    X(void, glLightModelxvOES, (GLenum pname, const GLfixed *param),           \
      VOID_DEFAULT, thread_ctx, pname, param)                                  \
    X(void, glGetQueryObjectuiv, (GLuint id, GLenum pname, GLuint * params),   \
      VOID_DEFAULT, thread_ctx, id, pname, params)                             \
    X(void, glProgramUniform2i,                                                \
      (GLuint program, GLint location, GLint v0, GLint v1), VOID_DEFAULT,      \
      thread_ctx, program, location, v0, v1)                                   \
    X(GLint, glTestPointer4, (GLint a, const GLint *b, GLint *c), RET_DEFAULT, \
      thread_ctx, a, b, c)                                                     \
    X(void, glUniform2iv, (GLint location, GLsizei count, const GLint *value), \
      VOID_DEFAULT, thread_ctx, location, count, value)                        \
    X(void, glInvalidateFramebuffer,                                           \
      (GLenum target, GLsizei numAttachments, const GLenum *attachments),      \
      VOID_DEFAULT, thread_ctx, target, numAttachments, attachments)           \
    X(void, glStencilOp, (GLenum fail, GLenum zfail, GLenum zpass),            \
      VOID_DEFAULT, thread_ctx, fail, zfail, zpass)                            \
    X(void, glStencilOpSeparate,                                               \
      (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass), VOID_DEFAULT, \
      thread_ctx, face, sfail, dpfail, dppass)                                 \
    X(void, glCullFace, (GLenum mode), VOID_DEFAULT, thread_ctx, mode)         \
    X(void, glFramebufferRenderbuffer,                                         \
      (GLenum target, GLenum attachment, GLenum renderbuffertarget,            \
       GLuint renderbuffer),                                                   \
      VOID_DEFAULT, thread_ctx, target, attachment, renderbuffertarget,        \
      renderbuffer)                                                            \
    X(void, glMultiTexCoord4xOES,                                              \
      (GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q),            \
      VOID_DEFAULT, thread_ctx, texture, s, t, r, q)                           \
    X(void, glGenBuffers, (GLsizei n, GLuint * buffers), VOID_DEFAULT,         \
      thread_ctx, n, buffers)                                                  \
    X(void, glVertexAttribI4iv, (GLuint index, const GLint *v), VOID_DEFAULT,  \
      thread_ctx, index, v)                                                    \
    X(void, glProgramParameteri, (GLuint program, GLenum pname, GLint value),  \
      VOID_DEFAULT, thread_ctx, program, pname, value)                         \
    X(void, glUniform3ui, (GLint location, GLuint v0, GLuint v1, GLuint v2),   \
      VOID_DEFAULT, thread_ctx, location, v0, v1, v2)                          \
    X(void, glTexStorage2DMultisample,                                         \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height, GLboolean fixedsamplelocations),                        \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height, fixedsamplelocations)                                            \
    X(void, glDepthMask, (GLboolean flag), VOID_DEFAULT, thread_ctx, flag)     \
    X(GLenum, glCheckFramebufferStatus, (GLenum target), RET_DEFAULT,          \
      thread_ctx, target)                                                      \
    X(void, glBufferData,                                                      \
      (GLenum target, GLsizeiptr size, const void *data, GLenum usage),        \
      VOID_DEFAULT, thread_ctx, target, size, data, usage)                     \
    X(void, glActiveTexture, (GLenum texture), VOID_DEFAULT, thread_ctx,       \
      texture)                                                                 \
    X(void, glVertexAttrib2fv, (GLuint index, const GLfloat *v), VOID_DEFAULT, \
      thread_ctx, index, v)                                                    \
    X(void, glVertexAttribI4i,                                                 \
      (GLuint index, GLint x, GLint y, GLint z, GLint w), VOID_DEFAULT,        \
      thread_ctx, index, x, y, z, w)                                           \
    X(void, glResumeTransformFeedback, (void), VOID_DEFAULT, thread_ctx)       \
    X(GLboolean, glIsVertexArray, (GLuint array), RET_DEFAULT, thread_ctx,     \
      array)                                                                   \
    X(void, glMaterialxvOES,                                                   \
      (GLenum face, GLenum pname, const GLfixed *param), VOID_DEFAULT,         \
      thread_ctx, face, pname, param)                                          \
    X(void, glGetTexLevelParameteriv,                                          \
      (GLenum target, GLint level, GLenum pname, GLint * params),              \
      VOID_DEFAULT, thread_ctx, target, level, pname, params)                  \
    X(void, glDepthRangefOES, (GLclampf n, GLclampf f), VOID_DEFAULT,          \
      thread_ctx, n, f)                                                        \
    X(void, glDetachShader, (GLuint program, GLuint shader), VOID_DEFAULT,     \
      thread_ctx, program, shader)                                             \
    X(void, glVertexAttrib4f,                                                  \
      (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w),              \
      VOID_DEFAULT, thread_ctx, index, x, y, z, w)                             \
    X(void, glDeleteRenderbuffers, (GLsizei n, const GLuint *renderbuffers),   \
      VOID_DEFAULT, thread_ctx, n, renderbuffers)                              \
    X(void, glGenSamplers, (GLsizei count, GLuint * samplers), VOID_DEFAULT,   \
      thread_ctx, count, samplers)                                             \
    X(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height),     \
      VOID_DEFAULT, thread_ctx, x, y, width, height)                           \
    X(void, glMemoryBarrier, (GLbitfield barriers), VOID_DEFAULT, thread_ctx,  \
      barriers)                                                                \
    X(void, glGetProgramInterfaceiv,                                           \
      (GLuint program, GLenum programInterface, GLenum pname, GLint * params), \
      VOID_DEFAULT, thread_ctx, program, programInterface, pname, params)      \
    X(void, glDisable, (GLenum cap), VOID_DEFAULT, thread_ctx, cap)            \
    X(void, glUniform2f, (GLint location, GLfloat v0, GLfloat v1),             \
      VOID_DEFAULT, thread_ctx, location, v0, v1)                              \
    X(void, glClearBufferfv,                                                   \
      (GLenum buffer, GLint drawbuffer, const GLfloat *value), VOID_DEFAULT,   \
      thread_ctx, buffer, drawbuffer, value)                                   \
    X(GLbitfield, glQueryMatrixxOES, (GLfixed * mantissa, GLint * exponent),   \
      RET_DEFAULT, thread_ctx, mantissa, exponent)                             \
    X(void, glProgramBinary,                                                   \
      (GLuint program, GLenum binaryFormat, const void *binary,                \
       GLsizei length),                                                        \
      VOID_DEFAULT, thread_ctx, program, binaryFormat, binary, length)         \
    X(GLboolean, glIsProgramPipeline, (GLuint pipeline), RET_DEFAULT,          \
      thread_ctx, pipeline)                                                    \
    X(void, glOrthoxOES,                                                       \
      (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glGetProgramResourceName,                                          \
      (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, \
       GLsizei * length, GLchar * name),                                       \
      VOID_DEFAULT, thread_ctx, program, programInterface, index, bufSize,     \
      length, name)                                                            \
    X(void, glGetVertexAttribPointerv,                                         \
      (GLuint index, GLenum pname, void **pointer), VOID_DEFAULT, thread_ctx,  \
      index, pname, pointer)                                                   \
    X(void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer),          \
      VOID_DEFAULT, thread_ctx, target, renderbuffer)                          \
    X(void, glGetUniformfv,                                                    \
      (GLuint program, GLint location, GLfloat * params), VOID_DEFAULT,        \
      thread_ctx, program, location, params)                                   \
    X(GLuint, glCreateShaderProgramv,                                          \
      (GLenum type, GLsizei count, const GLchar *const *strings), RET_DEFAULT, \
      thread_ctx, type, count, strings)                                        \
    X(void, glProgramUniformMatrix4fv,                                         \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glProgramUniform2f,                                                \
      (GLuint program, GLint location, GLfloat v0, GLfloat v1), VOID_DEFAULT,  \
      thread_ctx, program, location, v0, v1)                                   \
    X(void, glReadnPixelsEXT,                                                  \
      (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format,         \
       GLenum type, GLsizei bufSize, void *data),                              \
      VOID_DEFAULT, thread_ctx, x, y, width, height, format, type, bufSize,    \
      data)                                                                    \
    X(void, glGetFloatv, (GLenum pname, GLfloat * data), VOID_DEFAULT,         \
      thread_ctx, pname, data)                                                 \
    X(void, glGetProgramResourceiv,                                            \
      (GLuint program, GLenum programInterface, GLuint index,                  \
       GLsizei propCount, const GLenum *props, GLsizei bufSize,                \
       GLsizei *length, GLint *params),                                        \
      VOID_DEFAULT, thread_ctx, program, programInterface, index, propCount,   \
      props, bufSize, length, params)                                          \
    X(void, glProgramUniform3f,                                                \
      (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2),    \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2)                 \
    X(void, glLineWidthxOES, (GLfixed width), VOID_DEFAULT, thread_ctx, width) \
    X(void, glUseProgramStages,                                                \
      (GLuint pipeline, GLbitfield stages, GLuint program), VOID_DEFAULT,      \
      thread_ctx, pipeline, stages, program)                                   \
    X(void, glReleaseShaderCompiler, (void), VOID_DEFAULT, thread_ctx)         \
    X(void, glProgramUniform4fv,                                               \
      (GLuint program, GLint location, GLsizei count, const GLfloat *value),   \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glTexImage2D,                                                      \
      (GLenum target, GLint level, GLint internalformat, GLsizei width,        \
       GLsizei height, GLint border, GLenum format, GLenum type,               \
       const void *pixels),                                                    \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, width, height,  \
      border, format, type, pixels)                                            \
    X(void, glGetUniformIndices,                                               \
      (GLuint program, GLsizei uniformCount,                                   \
       const GLchar *const *uniformNames, GLuint *uniformIndices),             \
      VOID_DEFAULT, thread_ctx, program, uniformCount, uniformNames,           \
      uniformIndices)                                                          \
    X(void, glProgramUniformMatrix2x4fv,                                       \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glUniform1fv,                                                      \
      (GLint location, GLsizei count, const GLfloat *value), VOID_DEFAULT,     \
      thread_ctx, location, count, value)                                      \
    X(void, glNormal3xOES, (GLfixed nx, GLfixed ny, GLfixed nz), VOID_DEFAULT, \
      thread_ctx, nx, ny, nz)                                                  \
    X(void, glGetVertexAttribIuiv,                                             \
      (GLuint index, GLenum pname, GLuint * params), VOID_DEFAULT, thread_ctx, \
      index, pname, params)                                                    \
    X(GLint64, glTestInt3, (GLint64 a, GLuint64 b), RET_DEFAULT, thread_ctx,   \
      a, b)                                                                    \
    X(void, glGetSamplerParameteriv,                                           \
      (GLuint sampler, GLenum pname, GLint * params), VOID_DEFAULT,            \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glDeleteBuffers, (GLsizei n, const GLuint *buffers), VOID_DEFAULT, \
      thread_ctx, n, buffers)                                                  \
    X(void, glGetAttachedShaders,                                              \
      (GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders),   \
      VOID_DEFAULT, thread_ctx, program, maxCount, count, shaders)             \
    X(void, glUniform1ui, (GLint location, GLuint v0), VOID_DEFAULT,           \
      thread_ctx, location, v0)                                                \
    X(void, glGenFramebuffers, (GLsizei n, GLuint * framebuffers),             \
      VOID_DEFAULT, thread_ctx, n, framebuffers)                               \
    X(GLfloat, glTestInt5, (GLint a, GLuint b), RET_DEFAULT, thread_ctx, a, b) \
    X(void, glAlphaFuncxOES, (GLenum func, GLfixed ref), VOID_DEFAULT,         \
      thread_ctx, func, ref)                                                   \
    X(void, glProgramUniform1iv,                                               \
      (GLuint program, GLint location, GLsizei count, const GLint *value),     \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glDisableVertexAttribArray, (GLuint index), VOID_DEFAULT,          \
      thread_ctx, index)                                                       \
    X(void, glTexEnvi, (GLenum target, GLenum pname, GLint param),             \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glUniform2fv,                                                      \
      (GLint location, GLsizei count, const GLfloat *value), VOID_DEFAULT,     \
      thread_ctx, location, count, value)                                      \
    X(void, glGetLightxvOES, (GLenum light, GLenum pname, GLfixed * params),   \
      VOID_DEFAULT, thread_ctx, light, pname, params)                          \
    X(void, glOrthofOES,                                                       \
      (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glEGLImageTargetTexture2DOES,                                      \
      (GLenum target, GLeglImageOES image), VOID_DEFAULT, thread_ctx, target,  \
      image)                                                                   \
    X(void, glDrawRangeElements,                                               \
      (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,      \
       const void *indices),                                                   \
      VOID_DEFAULT, thread_ctx, mode, start, end, count, type, indices)        \
    X(void, glActiveShaderProgram, (GLuint pipeline, GLuint program),          \
      VOID_DEFAULT, thread_ctx, pipeline, program)                             \
    X(void, glClearStencil, (GLint s), VOID_DEFAULT, thread_ctx, s)            \
    X(void, glStencilFuncSeparate,                                             \
      (GLenum face, GLenum func, GLint ref, GLuint mask), VOID_DEFAULT,        \
      thread_ctx, face, func, ref, mask)                                       \
    X(void, glTexEnvf, (GLenum target, GLenum pname, GLfloat param),           \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glBindBuffer, (GLenum target, GLuint buffer), VOID_DEFAULT,        \
      thread_ctx, target, buffer)                                              \
    X(void, glProgramUniform4iv,                                               \
      (GLuint program, GLint location, GLsizei count, const GLint *value),     \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glDeleteProgramPipelines, (GLsizei n, const GLuint *pipelines),    \
      VOID_DEFAULT, thread_ctx, n, pipelines)                                  \
    X(void, glUniform1uiv,                                                     \
      (GLint location, GLsizei count, const GLuint *value), VOID_DEFAULT,      \
      thread_ctx, location, count, value)                                      \
    X(void, glProgramUniform4f,                                                \
      (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2,     \
       GLfloat v3),                                                            \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2, v3)             \
    X(void, glDispatchCompute,                                                 \
      (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z),         \
      VOID_DEFAULT, thread_ctx, num_groups_x, num_groups_y, num_groups_z)      \
    X(void, glProgramUniformMatrix3x2fv,                                       \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glProgramUniform4ui,                                               \
      (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2,        \
       GLuint v3),                                                             \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2, v3)             \
    X(void, glProgramUniformMatrix2fv,                                         \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glColor4xOES,                                                      \
      (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glSampleCoveragexOES, (GLclampx value, GLboolean invert),          \
      VOID_DEFAULT, thread_ctx, value, invert)                                 \
    X(void, glProgramUniformMatrix2x3fv,                                       \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glGetMultisamplefv, (GLenum pname, GLuint index, GLfloat * val),   \
      VOID_DEFAULT, thread_ctx, pname, index, val)                             \
    X(void, glProgramUniform3i,                                                \
      (GLuint program, GLint location, GLint v0, GLint v1, GLint v2),          \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2)                 \
    X(void, glTestString,                                                      \
      (GLint a, GLint count, const GLchar *const *strings, GLint buf_len,      \
       GLchar *char_buf),                                                      \
      VOID_DEFAULT, thread_ctx, a, count, strings, buf_len, char_buf)          \
    X(void, glDiscardFramebufferEXT,                                           \
      (GLenum target, GLsizei numAttachments, const GLenum *attachments),      \
      VOID_DEFAULT, thread_ctx, target, numAttachments, attachments)           \
    X(void, glTexBufferRangeOES,                                               \
      (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset,   \
       GLsizeiptr size),                                                       \
      VOID_DEFAULT, thread_ctx, target, internalformat, buffer, offset, size)  \
    X(void, glTexBuffer,                                                       \
      (GLenum target, GLenum internalformat, GLuint buffer), VOID_DEFAULT,     \
      thread_ctx, target, internalformat, buffer)                              \
    X(void, glTexBufferRangeEXT,                                               \
      (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset,   \
       GLsizeiptr size),                                                       \
      VOID_DEFAULT, thread_ctx, target, internalformat, buffer, offset, size)  \
    X(void, glTexBufferRange,                                                  \
      (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset,   \
       GLsizeiptr size),                                                       \
      VOID_DEFAULT, thread_ctx, target, internalformat, buffer, offset, size)  \
    X(void, glTexBufferEXT,                                                    \
      (GLenum target, GLenum internalformat, GLuint buffer), VOID_DEFAULT,     \
      thread_ctx, target, internalformat, buffer)                              \
    X(void, glTexBufferOES,                                                    \
      (GLenum target, GLenum internalformat, GLuint buffer), VOID_DEFAULT,     \
      thread_ctx, target, internalformat, buffer)                              \
    X(void, glGetProgramBinaryOES,                                             \
      (GLuint program, GLsizei bufSize, GLsizei * length,                      \
       GLenum * binaryFormat, void *binary),                                   \
      VOID_DEFAULT, thread_ctx, program, bufSize, length, binaryFormat,        \
      binary)                                                                  \
    X(void, glProgramBinaryOES,                                                \
      (GLuint program, GLenum binaryFormat, const void *binary, GLint length), \
      VOID_DEFAULT, thread_ctx, program, binaryFormat, binary, length)         \
    X(void, glColorMaski,                                                      \
      (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a),      \
      VOID_DEFAULT, thread_ctx, index, r, g, b, a)                             \
    X(void, glColorMaskiEXT,                                                   \
      (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a),      \
      VOID_DEFAULT, thread_ctx, index, r, g, b, a)                             \
    X(void, glColorMaskiOES,                                                   \
      (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a),      \
      VOID_DEFAULT, thread_ctx, index, r, g, b, a)                             \
    X(void, glBlendFuncSeparatei,                                              \
      (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,              \
       GLenum dstAlpha),                                                       \
      VOID_DEFAULT, thread_ctx, buf, srcRGB, dstRGB, srcAlpha, dstAlpha)       \
    X(void, glBlendFuncSeparateiEXT,                                           \
      (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,              \
       GLenum dstAlpha),                                                       \
      VOID_DEFAULT, thread_ctx, buf, srcRGB, dstRGB, srcAlpha, dstAlpha)       \
    X(void, glBlendFuncSeparateiOES,                                           \
      (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,              \
       GLenum dstAlpha),                                                       \
      VOID_DEFAULT, thread_ctx, buf, srcRGB, dstRGB, srcAlpha, dstAlpha)       \
    X(void, glBlendEquationSeparatei,                                          \
      (GLuint buf, GLenum modeRGB, GLenum modeAlpha), VOID_DEFAULT,            \
      thread_ctx, buf, modeRGB, modeAlpha)                                     \
    X(void, glBlendEquationSeparateiEXT,                                       \
      (GLuint buf, GLenum modeRGB, GLenum modeAlpha), VOID_DEFAULT,            \
      thread_ctx, buf, modeRGB, modeAlpha)                                     \
    X(void, glBlendEquationSeparateiOES,                                       \
      (GLuint buf, GLenum modeRGB, GLenum modeAlpha), VOID_DEFAULT,            \
      thread_ctx, buf, modeRGB, modeAlpha)

#define LIST_NOT_IMPL_GL_FUNCTIONS(X)                                          \
    X(void, glSignalSemaphoreEXT,                                              \
      (GLuint semaphore, GLuint numBufferBarriers, const GLuint *buffers,      \
       GLuint numTextureBarriers, const GLuint *textures,                      \
       const GLenum *dstLayouts),                                              \
      VOID_DEFAULT, thread_ctx, semaphore, numBufferBarriers, buffers,         \
      numTextureBarriers, textures, dstLayouts)                                \
    X(void, glUniformMatrix2x4fvNV,                                            \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glProgramUniform3fvEXT,                                            \
      (GLuint program, GLint location, GLsizei count, const GLfloat *value),   \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glPathParameterfNV, (GLuint path, GLenum pname, GLfloat value),    \
      VOID_DEFAULT, thread_ctx, path, pname, value)                            \
    X(void, glProgramUniformHandleui64vIMG,                                    \
      (GLuint program, GLint location, GLsizei count, const GLuint64 *values), \
      VOID_DEFAULT, thread_ctx, program, location, count, values)              \
    X(void, glTexParameterIuivEXT,                                             \
      (GLenum target, GLenum pname, const GLuint *params), VOID_DEFAULT,       \
      thread_ctx, target, pname, params)                                       \
    X(GLboolean, glIsSyncAPPLE, (GLsync sync), RET_DEFAULT, thread_ctx, sync)  \
    X(void, glDrawVkImageNV,                                                   \
      (GLuint64 vkImage, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1,   \
       GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1), \
      VOID_DEFAULT, thread_ctx, vkImage, sampler, x0, y0, x1, y1, z, s0, t0,   \
      s1, t1)                                                                  \
    X(GLboolean, glIsSemaphoreEXT, (GLuint semaphore), RET_DEFAULT,            \
      thread_ctx, semaphore)                                                   \
    X(void, glDeleteFencesNV, (GLsizei n, const GLuint *fences), VOID_DEFAULT, \
      thread_ctx, n, fences)                                                   \
    X(void, glExtTexObjectStateOverrideiQCOM,                                  \
      (GLenum target, GLenum pname, GLint param), VOID_DEFAULT, thread_ctx,    \
      target, pname, param)                                                    \
    X(void, glProgramUniform1uivEXT,                                           \
      (GLuint program, GLint location, GLsizei count, const GLuint *value),    \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glGetnUniformuiv,                                                  \
      (GLuint program, GLint location, GLsizei bufSize, GLuint * params),      \
      VOID_DEFAULT, thread_ctx, program, location, bufSize, params)            \
    X(void, glPathStringNV,                                                    \
      (GLuint path, GLenum format, GLsizei length, const void *pathString),    \
      VOID_DEFAULT, thread_ctx, path, format, length, pathString)              \
    X(void, glCreateMemoryObjectsEXT, (GLsizei n, GLuint * memoryObjects),     \
      VOID_DEFAULT, thread_ctx, n, memoryObjects)                              \
    X(void, glViewportIndexedfvNV, (GLuint index, const GLfloat *v),           \
      VOID_DEFAULT, thread_ctx, index, v)                                      \
    X(void, glSignalVkSemaphoreNV, (GLuint64 vkSemaphore), VOID_DEFAULT,       \
      thread_ctx, vkSemaphore)                                                 \
    X(void, glProgramUniform2i64NV,                                            \
      (GLuint program, GLint location, GLint64EXT x, GLint64EXT y),            \
      VOID_DEFAULT, thread_ctx, program, location, x, y)                       \
    X(void, glDrawArraysInstancedEXT,                                          \
      (GLenum mode, GLint start, GLsizei count, GLsizei primcount),            \
      VOID_DEFAULT, thread_ctx, mode, start, count, primcount)                 \
    X(void, glGetPerfQueryDataINTEL,                                           \
      (GLuint queryHandle, GLuint flags, GLsizei dataSize, void *data,         \
       GLuint *bytesWritten),                                                  \
      VOID_DEFAULT, thread_ctx, queryHandle, flags, dataSize, data,            \
      bytesWritten)                                                            \
    X(void, glValidateProgramPipelineEXT, (GLuint pipeline), VOID_DEFAULT,     \
      thread_ctx, pipeline)                                                    \
    X(void, glShadingRateQCOM, (GLenum rate), VOID_DEFAULT, thread_ctx, rate)  \
    X(void, glTexImage3DOES,                                                   \
      (GLenum target, GLint level, GLenum internalformat, GLsizei width,       \
       GLsizei height, GLsizei depth, GLint border, GLenum format,             \
       GLenum type, const void *pixels),                                       \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, width, height,  \
      depth, border, format, type, pixels)                                     \
    X(void, glMaterialxv, (GLenum face, GLenum pname, const GLfixed *param),   \
      VOID_DEFAULT, thread_ctx, face, pname, param)                            \
    X(void, glGetPathCoordsNV, (GLuint path, GLfloat * coords), VOID_DEFAULT,  \
      thread_ctx, path, coords)                                                \
    X(void, glMatrixScalefEXT, (GLenum mode, GLfloat x, GLfloat y, GLfloat z), \
      VOID_DEFAULT, thread_ctx, mode, x, y, z)                                 \
    X(void, glScissorIndexedvNV, (GLuint index, const GLint *v), VOID_DEFAULT, \
      thread_ctx, index, v)                                                    \
    X(void, glPatchParameteriOES, (GLenum pname, GLint value), VOID_DEFAULT,   \
      thread_ctx, pname, value)                                                \
    X(void, glTextureStorageMem2DMultisampleEXT,                               \
      (GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width,  \
       GLsizei height, GLboolean fixedSampleLocations, GLuint memory,          \
       GLuint64 offset),                                                       \
      VOID_DEFAULT, thread_ctx, texture, samples, internalFormat, width,       \
      height, fixedSampleLocations, memory, offset)                            \
    X(void, glGetClipPlanefOES, (GLenum plane, GLfloat * equation),            \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glGetPerfMonitorCounterStringAMD,                                  \
      (GLuint group, GLuint counter, GLsizei bufSize, GLsizei * length,        \
       GLchar * counterString),                                                \
      VOID_DEFAULT, thread_ctx, group, counter, bufSize, length,               \
      counterString)                                                           \
    X(GLVULKANPROCNV, glGetVkProcAddrNV, (const GLchar *name), RET_DEFAULT,    \
      thread_ctx, name)                                                        \
    X(void, glGenFramebuffersOES, (GLsizei n, GLuint * framebuffers),          \
      VOID_DEFAULT, thread_ctx, n, framebuffers)                               \
    X(void, glExtGetProgramsQCOM,                                              \
      (GLuint * programs, GLint maxPrograms, GLint * numPrograms),             \
      VOID_DEFAULT, thread_ctx, programs, maxPrograms, numPrograms)            \
    X(GLboolean, glUnmapBufferOES, (GLenum target), RET_DEFAULT, thread_ctx,   \
      target)                                                                  \
    X(void, glProgramUniformMatrix2fvEXT,                                      \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glEndTilingQCOM, (GLbitfield preserveMask), VOID_DEFAULT,          \
      thread_ctx, preserveMask)                                                \
    X(void, glGetPerfQueryIdByNameINTEL,                                       \
      (GLchar * queryName, GLuint * queryId), VOID_DEFAULT, thread_ctx,        \
      queryName, queryId)                                                      \
    X(void, glDepthRangeIndexedfNV, (GLuint index, GLfloat n, GLfloat f),      \
      VOID_DEFAULT, thread_ctx, index, n, f)                                   \
    X(void, glNamedRenderbufferStorageMultisampleAdvancedAMD,                  \
      (GLuint renderbuffer, GLsizei samples, GLsizei storageSamples,           \
       GLenum internalformat, GLsizei width, GLsizei height),                  \
      VOID_DEFAULT, thread_ctx, renderbuffer, samples, storageSamples,         \
      internalformat, width, height)                                           \
    X(void, glNormal3f, (GLfloat nx, GLfloat ny, GLfloat nz), VOID_DEFAULT,    \
      thread_ctx, nx, ny, nz)                                                  \
    X(void, glTexSubImage3DOES,                                                \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLenum format, GLenum type, const void *pixels),                        \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset,      \
      width, height, depth, format, type, pixels)                              \
    X(void, glGetNextPerfQueryIdINTEL, (GLuint queryId, GLuint * nextQueryId), \
      VOID_DEFAULT, thread_ctx, queryId, nextQueryId)                          \
    X(void, glUniform3ui64vNV,                                                 \
      (GLint location, GLsizei count, const GLuint64EXT *value), VOID_DEFAULT, \
      thread_ctx, location, count, value)                                      \
    X(void, glProgramUniform3uiEXT,                                            \
      (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2),       \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2)                 \
    X(GLboolean, glIsMemoryObjectEXT, (GLuint memoryObject), RET_DEFAULT,      \
      thread_ctx, memoryObject)                                                \
    X(void, glVertexPointer,                                                   \
      (GLint size, GLenum type, GLsizei stride, const void *pointer),          \
      VOID_DEFAULT, thread_ctx, size, type, stride, pointer)                   \
    X(GLuint, glGetDebugMessageLogKHR,                                         \
      (GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types,        \
       GLuint * ids, GLenum * severities, GLsizei * lengths,                   \
       GLchar * messageLog),                                                   \
      RET_DEFAULT, thread_ctx, count, bufSize, sources, types, ids,            \
      severities, lengths, messageLog)                                         \
    X(void, glGetFirstPerfQueryIdINTEL, (GLuint * queryId), VOID_DEFAULT,      \
      thread_ctx, queryId)                                                     \
    X(void, glMinSampleShading, (GLfloat value), VOID_DEFAULT, thread_ctx,     \
      value)                                                                   \
    X(void, glDeleteFramebuffersOES, (GLsizei n, const GLuint *framebuffers),  \
      VOID_DEFAULT, thread_ctx, n, framebuffers)                               \
    X(void, glExtGetTexSubImageQCOM,                                           \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLenum format, GLenum type, void *texels),                              \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset,      \
      width, height, depth, format, type, texels)                              \
    X(void, glFramebufferTexture3DOES,                                         \
      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture,     \
       GLint level, GLint zoffset),                                            \
      VOID_DEFAULT, thread_ctx, target, attachment, textarget, texture, level, \
      zoffset)                                                                 \
    X(void, glProgramUniform2fvEXT,                                            \
      (GLuint program, GLint location, GLsizei count, const GLfloat *value),   \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glTextureStorageMem3DEXT,                                          \
      (GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width,   \
       GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset),         \
      VOID_DEFAULT, thread_ctx, texture, levels, internalFormat, width,        \
      height, depth, memory, offset)                                           \
    X(GLuint64, glGetTextureHandleIMG, (GLuint texture), RET_DEFAULT,          \
      thread_ctx, texture)                                                     \
    X(void, glRenderbufferStorageMultisampleNV,                                \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height),                                                        \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height)                                                                  \
    X(void, glProgramUniform1i64NV,                                            \
      (GLuint program, GLint location, GLint64EXT x), VOID_DEFAULT,            \
      thread_ctx, program, location, x)                                        \
    X(void, glProgramUniformMatrix3x2fvEXT,                                    \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glTexGenivOES, (GLenum coord, GLenum pname, const GLint *params),  \
      VOID_DEFAULT, thread_ctx, coord, pname, params)                          \
    X(void, glDrawArraysInstancedANGLE,                                        \
      (GLenum mode, GLint first, GLsizei count, GLsizei primcount),            \
      VOID_DEFAULT, thread_ctx, mode, first, count, primcount)                 \
    X(void, glRenderbufferStorageMultisampleIMG,                               \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height),                                                        \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height)                                                                  \
    X(void, glBlendFunciEXT, (GLuint buf, GLenum src, GLenum dst),             \
      VOID_DEFAULT, thread_ctx, buf, src, dst)                                 \
    X(void, glDrawBuffersNV, (GLsizei n, const GLenum *bufs), VOID_DEFAULT,    \
      thread_ctx, n, bufs)                                                     \
    X(void, glExtGetFramebuffersQCOM,                                          \
      (GLuint * framebuffers, GLint maxFramebuffers, GLint * numFramebuffers), \
      VOID_DEFAULT, thread_ctx, framebuffers, maxFramebuffers,                 \
      numFramebuffers)                                                         \
    X(GLboolean, glIsEnabledi, (GLenum target, GLuint index), RET_DEFAULT,     \
      thread_ctx, target, index)                                               \
    X(void, glPathStencilDepthOffsetNV, (GLfloat factor, GLfloat units),       \
      VOID_DEFAULT, thread_ctx, factor, units)                                 \
    X(void, glTextureStorage2DEXT,                                             \
      (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat,   \
       GLsizei width, GLsizei height),                                         \
      VOID_DEFAULT, thread_ctx, texture, target, levels, internalformat,       \
      width, height)                                                           \
    X(void, glPathGlyphsNV,                                                    \
      (GLuint firstPathName, GLenum fontTarget, const void *fontName,          \
       GLbitfield fontStyle, GLsizei numGlyphs, GLenum type,                   \
       const void *charcodes, GLenum handleMissingGlyphs,                      \
       GLuint pathParameterTemplate, GLfloat emScale),                         \
      VOID_DEFAULT, thread_ctx, firstPathName, fontTarget, fontName,           \
      fontStyle, numGlyphs, type, charcodes, handleMissingGlyphs,              \
      pathParameterTemplate, emScale)                                          \
    X(void, glUniform1i64vNV,                                                  \
      (GLint location, GLsizei count, const GLint64EXT *value), VOID_DEFAULT,  \
      thread_ctx, location, count, value)                                      \
    X(void, glCopyBufferSubDataNV,                                             \
      (GLenum readTarget, GLenum writeTarget, GLintptr readOffset,             \
       GLintptr writeOffset, GLsizeiptr size),                                 \
      VOID_DEFAULT, thread_ctx, readTarget, writeTarget, readOffset,           \
      writeOffset, size)                                                       \
    X(void, glEGLImageTargetRenderbufferStorageOES,                            \
      (GLenum target, GLeglImageOES image), VOID_DEFAULT, thread_ctx, target,  \
      image)                                                                   \
    X(void, glGetQueryivEXT, (GLenum target, GLenum pname, GLint * params),    \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glBufferStorageMemEXT,                                             \
      (GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset),        \
      VOID_DEFAULT, thread_ctx, target, size, memory, offset)                  \
    X(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param),           \
      VOID_DEFAULT, thread_ctx, face, pname, param)                            \
    X(void, glUniform4ui64NV,                                                  \
      (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z,            \
       GLuint64EXT w),                                                         \
      VOID_DEFAULT, thread_ctx, location, x, y, z, w)                          \
    X(void, glProgramUniform4uivEXT,                                           \
      (GLuint program, GLint location, GLsizei count, const GLuint *value),    \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glWeightPointerOES,                                                \
      (GLint size, GLenum type, GLsizei stride, const void *pointer),          \
      VOID_DEFAULT, thread_ctx, size, type, stride, pointer)                   \
    X(void, glDepthRangeArrayfvNV,                                             \
      (GLuint first, GLsizei count, const GLfloat *v), VOID_DEFAULT,           \
      thread_ctx, first, count, v)                                             \
    X(void, glMakeImageHandleResidentNV, (GLuint64 handle, GLenum access),     \
      VOID_DEFAULT, thread_ctx, handle, access)                                \
    X(void, glTexStorageMem2DEXT,                                              \
      (GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width,    \
       GLsizei height, GLuint memory, GLuint64 offset),                        \
      VOID_DEFAULT, thread_ctx, target, levels, internalFormat, width, height, \
      memory, offset)                                                          \
    X(void, glProgramUniformHandleui64NV,                                      \
      (GLuint program, GLint location, GLuint64 value), VOID_DEFAULT,          \
      thread_ctx, program, location, value)                                    \
    X(void, glPathCoverDepthFuncNV, (GLenum func), VOID_DEFAULT, thread_ctx,   \
      func)                                                                    \
    X(void, glGetPathMetricsNV,                                                \
      (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType,      \
       const void *paths, GLuint pathBase, GLsizei stride, GLfloat *metrics),  \
      VOID_DEFAULT, thread_ctx, metricQueryMask, numPaths, pathNameType,       \
      paths, pathBase, stride, metrics)                                        \
    X(void, glGetFramebufferAttachmentParameterivOES,                          \
      (GLenum target, GLenum attachment, GLenum pname, GLint * params),        \
      VOID_DEFAULT, thread_ctx, target, attachment, pname, params)             \
    X(void, glFramebufferTexture2DOES,                                         \
      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture,     \
       GLint level),                                                           \
      VOID_DEFAULT, thread_ctx, target, attachment, textarget, texture, level) \
    X(void, glGetProgramResourcefvNV,                                          \
      (GLuint program, GLenum programInterface, GLuint index,                  \
       GLsizei propCount, const GLenum *props, GLsizei count, GLsizei *length, \
       GLfloat *params),                                                       \
      VOID_DEFAULT, thread_ctx, program, programInterface, index, propCount,   \
      props, count, length, params)                                            \
    X(void, glProgramUniform2ivEXT,                                            \
      (GLuint program, GLint location, GLsizei count, const GLint *value),     \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glBindFragDataLocationEXT,                                         \
      (GLuint program, GLuint color, const GLchar *name), VOID_DEFAULT,        \
      thread_ctx, program, color, name)                                        \
    X(void, glProgramUniform1ui64NV,                                           \
      (GLuint program, GLint location, GLuint64EXT x), VOID_DEFAULT,           \
      thread_ctx, program, location, x)                                        \
    X(void, glPathCommandsNV,                                                  \
      (GLuint path, GLsizei numCommands, const GLubyte *commands,              \
       GLsizei numCoords, GLenum coordType, const void *coords),               \
      VOID_DEFAULT, thread_ctx, path, numCommands, commands, numCoords,        \
      coordType, coords)                                                       \
    X(void, glViewportSwizzleNV,                                               \
      (GLuint index, GLenum swizzlex, GLenum swizzley, GLenum swizzlez,        \
       GLenum swizzlew),                                                       \
      VOID_DEFAULT, thread_ctx, index, swizzlex, swizzley, swizzlez, swizzlew) \
    X(void, glTexEstimateMotionQCOM,                                           \
      (GLuint ref, GLuint target, GLuint output), VOID_DEFAULT, thread_ctx,    \
      ref, target, output)                                                     \
    X(void, glViewportPositionWScaleNV,                                        \
      (GLuint index, GLfloat xcoeff, GLfloat ycoeff), VOID_DEFAULT,            \
      thread_ctx, index, xcoeff, ycoeff)                                       \
    X(void, glDeletePathsNV, (GLuint path, GLsizei range), VOID_DEFAULT,       \
      thread_ctx, path, range)                                                 \
    X(void, glGetObjectLabelEXT,                                               \
      (GLenum type, GLuint object, GLsizei bufSize, GLsizei * length,          \
       GLchar * label),                                                        \
      VOID_DEFAULT, thread_ctx, type, object, bufSize, length, label)          \
    X(GLuint64, glGetTextureHandleNV, (GLuint texture), RET_DEFAULT,           \
      thread_ctx, texture)                                                     \
    X(void, glBeginQueryEXT, (GLenum target, GLuint id), VOID_DEFAULT,         \
      thread_ctx, target, id)                                                  \
    X(void, glProgramUniform1iEXT, (GLuint program, GLint location, GLint v0), \
      VOID_DEFAULT, thread_ctx, program, location, v0)                         \
    X(void, glVertexAttribDivisorNV, (GLuint index, GLuint divisor),           \
      VOID_DEFAULT, thread_ctx, index, divisor)                                \
    X(void, glReadBufferNV, (GLenum mode), VOID_DEFAULT, thread_ctx, mode)     \
    X(void, glShadingRateSampleOrderNV, (GLenum order), VOID_DEFAULT,          \
      thread_ctx, order)                                                       \
    X(void, glFramebufferTextureOES,                                           \
      (GLenum target, GLenum attachment, GLuint texture, GLint level),         \
      VOID_DEFAULT, thread_ctx, target, attachment, texture, level)            \
    X(void, glUniformMatrix3x2fvNV,                                            \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glPointParameterf, (GLenum pname, GLfloat param), VOID_DEFAULT,    \
      thread_ctx, pname, param)                                                \
    X(void, glDrawArraysInstancedNV,                                           \
      (GLenum mode, GLint first, GLsizei count, GLsizei primcount),            \
      VOID_DEFAULT, thread_ctx, mode, first, count, primcount)                 \
    X(void, glTextureStorageMem2DEXT,                                          \
      (GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width,   \
       GLsizei height, GLuint memory, GLuint64 offset),                        \
      VOID_DEFAULT, thread_ctx, texture, levels, internalFormat, width,        \
      height, memory, offset)                                                  \
    X(void, glGenFencesNV, (GLsizei n, GLuint * fences), VOID_DEFAULT,         \
      thread_ctx, n, fences)                                                   \
    X(void, glCoverageModulationNV, (GLenum components), VOID_DEFAULT,         \
      thread_ctx, components)                                                  \
    X(GLint, glGetProgramResourceLocationIndexEXT,                             \
      (GLuint program, GLenum programInterface, const GLchar *name),           \
      RET_DEFAULT, thread_ctx, program, programInterface, name)                \
    X(GLboolean, glIsTextureHandleResidentNV, (GLuint64 handle), RET_DEFAULT,  \
      thread_ctx, handle)                                                      \
    X(void, glWeightPathsNV,                                                   \
      (GLuint resultPath, GLsizei numPaths, const GLuint *paths,               \
       const GLfloat *weights),                                                \
      VOID_DEFAULT, thread_ctx, resultPath, numPaths, paths, weights)          \
    X(void, glDebugMessageInsertKHR,                                           \
      (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, \
       const GLchar *buf),                                                     \
      VOID_DEFAULT, thread_ctx, source, type, id, severity, length, buf)       \
    X(void, glPathParameterfvNV,                                               \
      (GLuint path, GLenum pname, const GLfloat *value), VOID_DEFAULT,         \
      thread_ctx, path, pname, value)                                          \
    X(void, glDrawElementsInstancedBaseVertexOES,                              \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei instancecount, GLint basevertex),                               \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, instancecount,     \
      basevertex)                                                              \
    X(void, glGetPathParameterfvNV,                                            \
      (GLuint path, GLenum pname, GLfloat * value), VOID_DEFAULT, thread_ctx,  \
      path, pname, value)                                                      \
    X(GLfloat, glGetPathLengthNV,                                              \
      (GLuint path, GLsizei startSegment, GLsizei numSegments), RET_DEFAULT,   \
      thread_ctx, path, startSegment, numSegments)                             \
    X(void, glBlendFunciOES, (GLuint buf, GLenum src, GLenum dst),             \
      VOID_DEFAULT, thread_ctx, buf, src, dst)                                 \
    X(void, glMultiDrawMeshTasksIndirectCountNV,                               \
      (GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount,            \
       GLsizei stride),                                                        \
      VOID_DEFAULT, thread_ctx, indirect, drawcount, maxdrawcount, stride)     \
    X(void, glResolveMultisampleFramebufferAPPLE, (void), VOID_DEFAULT,        \
      thread_ctx)                                                              \
    X(void, glCoverFillPathInstancedNV,                                        \
      (GLsizei numPaths, GLenum pathNameType, const void *paths,               \
       GLuint pathBase, GLenum coverMode, GLenum transformType,                \
       const GLfloat *transformValues),                                        \
      VOID_DEFAULT, thread_ctx, numPaths, pathNameType, paths, pathBase,       \
      coverMode, transformType, transformValues)                               \
    X(void, glPopMatrix, (void), VOID_DEFAULT, thread_ctx)                     \
    X(void, glImportSemaphoreWin32HandleEXT,                                   \
      (GLuint semaphore, GLenum handleType, void *handle), VOID_DEFAULT,       \
      thread_ctx, semaphore, handleType, handle)                               \
    X(void, glStartTilingQCOM,                                                 \
      (GLuint x, GLuint y, GLuint width, GLuint height,                        \
       GLbitfield preserveMask),                                               \
      VOID_DEFAULT, thread_ctx, x, y, width, height, preserveMask)             \
    X(void, glWaitVkSemaphoreNV, (GLuint64 vkSemaphore), VOID_DEFAULT,         \
      thread_ctx, vkSemaphore)                                                 \
    X(void, glPointSize, (GLfloat size), VOID_DEFAULT, thread_ctx, size)       \
    X(void, glMatrixLoaddEXT, (GLenum mode, const GLdouble *m), VOID_DEFAULT,  \
      thread_ctx, mode, m)                                                     \
    X(void, glUniformHandleui64NV, (GLint location, GLuint64 value),           \
      VOID_DEFAULT, thread_ctx, location, value)                               \
    X(void, glUniform4i64NV,                                                   \
      (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z,               \
       GLint64EXT w),                                                          \
      VOID_DEFAULT, thread_ctx, location, x, y, z, w)                          \
    X(void, glDisableiOES, (GLenum target, GLuint index), VOID_DEFAULT,        \
      thread_ctx, target, index)                                               \
    X(void, glCopyTextureLevelsAPPLE,                                          \
      (GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, \
       GLsizei sourceLevelCount),                                              \
      VOID_DEFAULT, thread_ctx, destinationTexture, sourceTexture,             \
      sourceBaseLevel, sourceLevelCount)                                       \
    X(void, glUniform2i64NV, (GLint location, GLint64EXT x, GLint64EXT y),     \
      VOID_DEFAULT, thread_ctx, location, x, y)                                \
    X(void, glUniform3i64vNV,                                                  \
      (GLint location, GLsizei count, const GLint64EXT *value), VOID_DEFAULT,  \
      thread_ctx, location, count, value)                                      \
    X(void, glCopyImageSubDataOES,                                             \
      (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX,           \
       GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget,               \
       GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth,   \
       GLsizei srcHeight, GLsizei srcDepth),                                   \
      VOID_DEFAULT, thread_ctx, srcName, srcTarget, srcLevel, srcX, srcY,      \
      srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth,          \
      srcHeight, srcDepth)                                                     \
    X(void, glBindFragDataLocationIndexedEXT,                                  \
      (GLuint program, GLuint colorNumber, GLuint index, const GLchar *name),  \
      VOID_DEFAULT, thread_ctx, program, colorNumber, index, name)             \
    X(void, glCoverageMaskNV, (GLboolean mask), VOID_DEFAULT, thread_ctx,      \
      mask)                                                                    \
    X(void, glBlendEquationiOES, (GLuint buf, GLenum mode), VOID_DEFAULT,      \
      thread_ctx, buf, mode)                                                   \
    X(GLuint64, glGetTextureSamplerHandleIMG,                                  \
      (GLuint texture, GLuint sampler), RET_DEFAULT, thread_ctx, texture,      \
      sampler)                                                                 \
    X(GLboolean, glIsEnablediOES, (GLenum target, GLuint index), RET_DEFAULT,  \
      thread_ctx, target, index)                                               \
    X(void, glSelectPerfMonitorCountersAMD,                                    \
      (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters,      \
       GLuint * counterList),                                                  \
      VOID_DEFAULT, thread_ctx, monitor, enable, group, numCounters,           \
      counterList)                                                             \
    X(void, glDrawElementsBaseVertex,                                          \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLint basevertex),                                                      \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, basevertex)        \
    X(void, glGetSemaphoreParameterui64vEXT,                                   \
      (GLuint semaphore, GLenum pname, GLuint64 * params), VOID_DEFAULT,       \
      thread_ctx, semaphore, pname, params)                                    \
    X(void, glSignalVkFenceNV, (GLuint64 vkFence), VOID_DEFAULT, thread_ctx,   \
      vkFence)                                                                 \
    X(void, glBindShadingRateImageNV, (GLuint texture), VOID_DEFAULT,          \
      thread_ctx, texture)                                                     \
    X(void, glTexStorage2DEXT,                                                 \
      (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width,    \
       GLsizei height),                                                        \
      VOID_DEFAULT, thread_ctx, target, levels, internalformat, width, height) \
    X(GLboolean, glPointAlongPathNV,                                           \
      (GLuint path, GLsizei startSegment, GLsizei numSegments,                 \
       GLfloat distance, GLfloat * x, GLfloat * y, GLfloat * tangentX,         \
       GLfloat * tangentY),                                                    \
      RET_DEFAULT, thread_ctx, path, startSegment, numSegments, distance, x,   \
      y, tangentX, tangentY)                                                   \
    X(void, glTextureViewOES,                                                  \
      (GLuint texture, GLenum target, GLuint origtexture,                      \
       GLenum internalformat, GLuint minlevel, GLuint numlevels,               \
       GLuint minlayer, GLuint numlayers),                                     \
      VOID_DEFAULT, thread_ctx, texture, target, origtexture, internalformat,  \
      minlevel, numlevels, minlayer, numlayers)                                \
    X(void, glTextureViewEXT,                                                  \
      (GLuint texture, GLenum target, GLuint origtexture,                      \
       GLenum internalformat, GLuint minlevel, GLuint numlevels,               \
       GLuint minlayer, GLuint numlayers),                                     \
      VOID_DEFAULT, thread_ctx, texture, target, origtexture, internalformat,  \
      minlevel, numlevels, minlayer, numlayers)                                \
    X(void, glProgramUniform4ui64NV,                                           \
      (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y,           \
       GLuint64EXT z, GLuint64EXT w),                                          \
      VOID_DEFAULT, thread_ctx, program, location, x, y, z, w)                 \
    X(void, glCompressedTexSubImage3DOES,                                      \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLenum format, GLsizei imageSize, const void *data),                    \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset,      \
      width, height, depth, format, imageSize, data)                           \
    X(void, glInsertEventMarkerEXT, (GLsizei length, const GLchar *marker),    \
      VOID_DEFAULT, thread_ctx, length, marker)                                \
    X(void, glDrawTransformFeedbackEXT, (GLenum mode, GLuint id),              \
      VOID_DEFAULT, thread_ctx, mode, id)                                      \
    X(void, glNamedBufferStorageExternalEXT,                                   \
      (GLuint buffer, GLintptr offset, GLsizeiptr size,                        \
       GLeglClientBufferEXT clientBuffer, GLbitfield flags),                   \
      VOID_DEFAULT, thread_ctx, buffer, offset, size, clientBuffer, flags)     \
    X(void, glRasterSamplesEXT,                                                \
      (GLuint samples, GLboolean fixedsamplelocations), VOID_DEFAULT,          \
      thread_ctx, samples, fixedsamplelocations)                               \
    X(void, glFrustumx,                                                        \
      (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glMultMatrixx, (const GLfixed *m), VOID_DEFAULT, thread_ctx, m)    \
    X(void, glLoadMatrixf, (const GLfloat *m), VOID_DEFAULT, thread_ctx, m)    \
    X(void, glLoadMatrixx, (const GLfixed *m), VOID_DEFAULT, thread_ctx, m)    \
    X(void, glGetMemoryObjectParameterivEXT,                                   \
      (GLuint memoryObject, GLenum pname, GLint * params), VOID_DEFAULT,       \
      thread_ctx, memoryObject, pname, params)                                 \
    X(void, glMultiDrawArraysEXT,                                              \
      (GLenum mode, const GLint *first, const GLsizei *count,                  \
       GLsizei primcount),                                                     \
      VOID_DEFAULT, thread_ctx, mode, first, count, primcount)                 \
    X(void, glTexParameterIivEXT,                                              \
      (GLenum target, GLenum pname, const GLint *params), VOID_DEFAULT,        \
      thread_ctx, target, pname, params)                                       \
    X(void, glObjectPtrLabel,                                                  \
      (const void *ptr, GLsizei length, const GLchar *label), VOID_DEFAULT,    \
      thread_ctx, ptr, length, label)                                          \
    X(void, glGetBufferPointervOES,                                            \
      (GLenum target, GLenum pname, void **params), VOID_DEFAULT, thread_ctx,  \
      target, pname, params)                                                   \
    X(void, glInterpolatePathsNV,                                              \
      (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight),         \
      VOID_DEFAULT, thread_ctx, resultPath, pathA, pathB, weight)              \
    X(void, glProgramUniform2uiEXT,                                            \
      (GLuint program, GLint location, GLuint v0, GLuint v1), VOID_DEFAULT,    \
      thread_ctx, program, location, v0, v1)                                   \
    X(void, glFramebufferTextureMultiviewOVR,                                  \
      (GLenum target, GLenum attachment, GLuint texture, GLint level,          \
       GLint baseViewIndex, GLsizei numViews),                                 \
      VOID_DEFAULT, thread_ctx, target, attachment, texture, level,            \
      baseViewIndex, numViews)                                                 \
    X(void, glFramebufferTexture2DMultisampleIMG,                              \
      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture,     \
       GLint level, GLsizei samples),                                          \
      VOID_DEFAULT, thread_ctx, target, attachment, textarget, texture, level, \
      samples)                                                                 \
    X(void, glPushGroupMarkerEXT, (GLsizei length, const GLchar *marker),      \
      VOID_DEFAULT, thread_ctx, length, marker)                                \
    X(void, glCopyImageSubDataEXT,                                             \
      (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX,           \
       GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget,               \
       GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth,   \
       GLsizei srcHeight, GLsizei srcDepth),                                   \
      VOID_DEFAULT, thread_ctx, srcName, srcTarget, srcLevel, srcX, srcY,      \
      srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth,          \
      srcHeight, srcDepth)                                                     \
    X(void, glMatrixLoadTranspose3x3fNV,                                       \
      (GLenum matrixMode, const GLfloat *m), VOID_DEFAULT, thread_ctx,         \
      matrixMode, m)                                                           \
    X(void, glFramebufferTextureEXT,                                           \
      (GLenum target, GLenum attachment, GLuint texture, GLint level),         \
      VOID_DEFAULT, thread_ctx, target, attachment, texture, level)            \
    X(void, glStencilThenCoverStrokePathInstancedNV,                           \
      (GLsizei numPaths, GLenum pathNameType, const void *paths,               \
       GLuint pathBase, GLint reference, GLuint mask, GLenum coverMode,        \
       GLenum transformType, const GLfloat *transformValues),                  \
      VOID_DEFAULT, thread_ctx, numPaths, pathNameType, paths, pathBase,       \
      reference, mask, coverMode, transformType, transformValues)              \
    X(void, glGenQueriesEXT, (GLsizei n, GLuint * ids), VOID_DEFAULT,          \
      thread_ctx, n, ids)                                                      \
    X(void, glGetTexParameterIivOES,                                           \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glTexEnvfv, (GLenum target, GLenum pname, const GLfloat *params),  \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(GLenum, glPathGlyphIndexRangeNV,                                         \
      (GLenum fontTarget, const void *fontName, GLbitfield fontStyle,          \
       GLuint pathParameterTemplate, GLfloat emScale, GLuint baseAndCount[2]), \
      RET_DEFAULT, thread_ctx, fontTarget, fontName, fontStyle,                \
      pathParameterTemplate, emScale, baseAndCount)                            \
    X(void, glGetDriverControlStringQCOM,                                      \
      (GLuint driverControl, GLsizei bufSize, GLsizei * length,                \
       GLchar * driverControlString),                                          \
      VOID_DEFAULT, thread_ctx, driverControl, bufSize, length,                \
      driverControlString)                                                     \
    X(void, glUniform3i64NV,                                                   \
      (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z),              \
      VOID_DEFAULT, thread_ctx, location, x, y, z)                             \
    X(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z), VOID_DEFAULT,     \
      thread_ctx, x, y, z)                                                     \
    X(GLboolean, glIsRenderbufferOES, (GLuint renderbuffer), RET_DEFAULT,      \
      thread_ctx, renderbuffer)                                                \
    X(void *, glMapBufferRangeEXT,                                             \
      (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access),  \
      RET_DEFAULT, thread_ctx, target, offset, length, access)                 \
    X(void, glTransformPathNV,                                                 \
      (GLuint resultPath, GLuint srcPath, GLenum transformType,                \
       const GLfloat *transformValues),                                        \
      VOID_DEFAULT, thread_ctx, resultPath, srcPath, transformType,            \
      transformValues)                                                         \
    X(void, glProgramUniform4uiEXT,                                            \
      (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2,        \
       GLuint v3),                                                             \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2, v3)             \
    X(void, glBlitFramebufferANGLE,                                            \
      (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0,        \
       GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter), \
      VOID_DEFAULT, thread_ctx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0,      \
      dstX1, dstY1, mask, filter)                                              \
    X(void, glProgramUniform3iEXT,                                             \
      (GLuint program, GLint location, GLint v0, GLint v1, GLint v2),          \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2)                 \
    X(void, glProgramParameteriEXT,                                            \
      (GLuint program, GLenum pname, GLint value), VOID_DEFAULT, thread_ctx,   \
      program, pname, value)                                                   \
    X(void, glEGLImageTargetTextureStorageEXT,                                 \
      (GLuint texture, GLeglImageOES image, const GLint *attrib_list),         \
      VOID_DEFAULT, thread_ctx, texture, image, attrib_list)                   \
    X(void, glGetTranslatedShaderSourceANGLE,                                  \
      (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source),     \
      VOID_DEFAULT, thread_ctx, shader, bufSize, length, source)               \
    X(void, glObjectPtrLabelKHR,                                               \
      (const void *ptr, GLsizei length, const GLchar *label), VOID_DEFAULT,    \
      thread_ctx, ptr, length, label)                                          \
    X(void, glMatrixRotatefEXT,                                                \
      (GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z),           \
      VOID_DEFAULT, thread_ctx, mode, angle, x, y, z)                          \
    X(void, glGetnUniformfvEXT,                                                \
      (GLuint program, GLint location, GLsizei bufSize, GLfloat * params),     \
      VOID_DEFAULT, thread_ctx, program, location, bufSize, params)            \
    X(void, glClipPlanef, (GLenum p, const GLfloat *eqn), VOID_DEFAULT,        \
      thread_ctx, p, eqn)                                                      \
    X(void, glExtGetBufferPointervQCOM, (GLenum target, void **params),        \
      VOID_DEFAULT, thread_ctx, target, params)                                \
    X(void, glBlendFuncSeparateOES,                                            \
      (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha),        \
      VOID_DEFAULT, thread_ctx, srcRGB, dstRGB, srcAlpha, dstAlpha)            \
    X(GLuint, glCreateShaderProgramvEXT,                                       \
      (GLenum type, GLsizei count, const GLchar **strings), RET_DEFAULT,       \
      thread_ctx, type, count, strings)                                        \
    X(void, glUniform3ui64NV,                                                  \
      (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z),           \
      VOID_DEFAULT, thread_ctx, location, x, y, z)                             \
    X(void, glUniform4i64vNV,                                                  \
      (GLint location, GLsizei count, const GLint64EXT *value), VOID_DEFAULT,  \
      thread_ctx, location, count, value)                                      \
    X(void, glMatrixLoadTransposedEXT, (GLenum mode, const GLdouble *m),       \
      VOID_DEFAULT, thread_ctx, mode, m)                                       \
    X(void, glGetShadingRateImagePaletteNV,                                    \
      (GLuint viewport, GLuint entry, GLenum * rate), VOID_DEFAULT,            \
      thread_ctx, viewport, entry, rate)                                       \
    X(GLboolean, glIsQueryEXT, (GLuint id), RET_DEFAULT, thread_ctx, id)       \
    X(void, glGenSemaphoresEXT, (GLsizei n, GLuint * semaphores),              \
      VOID_DEFAULT, thread_ctx, n, semaphores)                                 \
    X(void, glCoverFillPathNV, (GLuint path, GLenum coverMode), VOID_DEFAULT,  \
      thread_ctx, path, coverMode)                                             \
    X(void, glPointParameterxv, (GLenum pname, const GLfixed *params),         \
      VOID_DEFAULT, thread_ctx, pname, params)                                 \
    X(GLint, glGetFragDataIndexEXT, (GLuint program, const GLchar *name),      \
      RET_DEFAULT, thread_ctx, program, name)                                  \
    X(void, glMultiDrawElementsIndirectEXT,                                    \
      (GLenum mode, GLenum type, const void *indirect, GLsizei drawcount,      \
       GLsizei stride),                                                        \
      VOID_DEFAULT, thread_ctx, mode, type, indirect, drawcount, stride)       \
    X(void, glLineWidthx, (GLfixed width), VOID_DEFAULT, thread_ctx, width)    \
    X(void, glClearPixelLocalStorageuiEXT,                                     \
      (GLsizei offset, GLsizei n, const GLuint *values), VOID_DEFAULT,         \
      thread_ctx, offset, n, values)                                           \
    X(void, glQueryCounterEXT, (GLuint id, GLenum target), VOID_DEFAULT,       \
      thread_ctx, id, target)                                                  \
    X(void, glGetSyncivAPPLE,                                                  \
      (GLsync sync, GLenum pname, GLsizei count, GLsizei * length,             \
       GLint * values),                                                        \
      VOID_DEFAULT, thread_ctx, sync, pname, count, length, values)            \
    X(void, glMultiDrawArraysIndirectEXT,                                      \
      (GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride),  \
      VOID_DEFAULT, thread_ctx, mode, indirect, drawcount, stride)             \
    X(void, glProgramUniform3ivEXT,                                            \
      (GLuint program, GLint location, GLsizei count, const GLint *value),     \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glGetSamplerParameterIuivOES,                                      \
      (GLuint sampler, GLenum pname, GLuint * params), VOID_DEFAULT,           \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glDrawTexsOES,                                                     \
      (GLshort x, GLshort y, GLshort z, GLshort width, GLshort height),        \
      VOID_DEFAULT, thread_ctx, x, y, z, width, height)                        \
    X(void, glSemaphoreParameterivNV,                                          \
      (GLuint semaphore, GLenum pname, const GLint *params), VOID_DEFAULT,     \
      thread_ctx, semaphore, pname, params)                                    \
    X(void, glDebugMessageControlKHR,                                          \
      (GLenum source, GLenum type, GLenum severity, GLsizei count,             \
       const GLuint *ids, GLboolean enabled),                                  \
      VOID_DEFAULT, thread_ctx, source, type, severity, count, ids, enabled)   \
    X(void, glGetTexParameterIuiv,                                             \
      (GLenum target, GLenum pname, GLuint * params), VOID_DEFAULT,            \
      thread_ctx, target, pname, params)                                       \
    X(void, glProgramUniform4fvEXT,                                            \
      (GLuint program, GLint location, GLsizei count, const GLfloat *value),   \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glDrawElementsBaseVertexEXT,                                       \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLint basevertex),                                                      \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, basevertex)        \
    X(void, glProgramUniformMatrix4x3fvEXT,                                    \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glMatrixMult3x3fNV, (GLenum matrixMode, const GLfloat *m),         \
      VOID_DEFAULT, thread_ctx, matrixMode, m)                                 \
    X(GLboolean, glIsEnablediNV, (GLenum target, GLuint index), RET_DEFAULT,   \
      thread_ctx, target, index)                                               \
    X(void, glFogx, (GLenum pname, GLfixed param), VOID_DEFAULT, thread_ctx,   \
      pname, param)                                                            \
    X(void, glTexGenfvOES,                                                     \
      (GLenum coord, GLenum pname, const GLfloat *params), VOID_DEFAULT,       \
      thread_ctx, coord, pname, params)                                        \
    X(void, glUniformHandleui64vIMG,                                           \
      (GLint location, GLsizei count, const GLuint64 *value), VOID_DEFAULT,    \
      thread_ctx, location, count, value)                                      \
    X(GLuint64, glGetTextureSamplerHandleNV, (GLuint texture, GLuint sampler), \
      RET_DEFAULT, thread_ctx, texture, sampler)                               \
    X(void, glBeginPerfMonitorAMD, (GLuint monitor), VOID_DEFAULT, thread_ctx, \
      monitor)                                                                 \
    X(void, glRenderbufferStorageMultisampleANGLE,                             \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height),                                                        \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height)                                                                  \
    X(void, glBindProgramPipelineEXT, (GLuint pipeline), VOID_DEFAULT,         \
      thread_ctx, pipeline)                                                    \
    X(void, glGetSamplerParameterIuivEXT,                                      \
      (GLuint sampler, GLenum pname, GLuint * params), VOID_DEFAULT,           \
      thread_ctx, sampler, pname, params)                                      \
    X(GLboolean, glIsPointInStrokePathNV, (GLuint path, GLfloat x, GLfloat y), \
      RET_DEFAULT, thread_ctx, path, x, y)                                     \
    X(void, glGetTexEnvfv, (GLenum target, GLenum pname, GLfloat * params),    \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glScissorArrayvOES, (GLuint first, GLsizei count, const GLint *v), \
      VOID_DEFAULT, thread_ctx, first, count, v)                               \
    X(void, glDisableClientState, (GLenum array), VOID_DEFAULT, thread_ctx,    \
      array)                                                                   \
    X(void, glProgramUniform4i64NV,                                            \
      (GLuint program, GLint location, GLint64EXT x, GLint64EXT y,             \
       GLint64EXT z, GLint64EXT w),                                            \
      VOID_DEFAULT, thread_ctx, program, location, x, y, z, w)                 \
    X(void, glGetRenderbufferParameterivOES,                                   \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glSamplerParameterIivEXT,                                          \
      (GLuint sampler, GLenum pname, const GLint *param), VOID_DEFAULT,        \
      thread_ctx, sampler, pname, param)                                       \
    X(void, glTexEnvxv, (GLenum target, GLenum pname, const GLfixed *params),  \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glTexStorage3DMultisample,                                         \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height, GLsizei depth, GLboolean fixedsamplelocations),         \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height, depth, fixedsamplelocations)                                     \
    X(void, glGetnUniformivEXT,                                                \
      (GLuint program, GLint location, GLsizei bufSize, GLint * params),       \
      VOID_DEFAULT, thread_ctx, program, location, bufSize, params)            \
    X(void, glDrawElementsInstancedEXT,                                        \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei primcount),                                                     \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, primcount)         \
    X(void, glFramebufferTexture2DMultisampleEXT,                              \
      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture,     \
       GLint level, GLsizei samples),                                          \
      VOID_DEFAULT, thread_ctx, target, attachment, textarget, texture, level, \
      samples)                                                                 \
    X(void, glBufferPageCommitmentMemNV,                                       \
      (GLenum target, GLintptr offset, GLsizeiptr size, GLuint memory,         \
       GLuint64 memOffset, GLboolean commit),                                  \
      VOID_DEFAULT, thread_ctx, target, offset, size, memory, memOffset,       \
      commit)                                                                  \
    X(void, glMatrixPopEXT, (GLenum mode), VOID_DEFAULT, thread_ctx, mode)     \
    X(void, glBeginPerfQueryINTEL, (GLuint queryHandle), VOID_DEFAULT,         \
      thread_ctx, queryHandle)                                                 \
    X(void, glFramebufferTexture2DDownsampleIMG,                               \
      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture,     \
       GLint level, GLint xscale, GLint yscale),                               \
      VOID_DEFAULT, thread_ctx, target, attachment, textarget, texture, level, \
      xscale, yscale)                                                          \
    X(void, glNormal3x, (GLfixed nx, GLfixed ny, GLfixed nz), VOID_DEFAULT,    \
      thread_ctx, nx, ny, nz)                                                  \
    X(void, glProgramPathFragmentInputGenNV,                                   \
      (GLuint program, GLint location, GLenum genMode, GLint components,       \
       const GLfloat *coeffs),                                                 \
      VOID_DEFAULT, thread_ctx, program, location, genMode, components,        \
      coeffs)                                                                  \
    X(void, glDrawElementsBaseVertexOES,                                       \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLint basevertex),                                                      \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, basevertex)        \
    X(void, glSubpixelPrecisionBiasNV, (GLuint xbits, GLuint ybits),           \
      VOID_DEFAULT, thread_ctx, xbits, ybits)                                  \
    X(void, glGetProgramPipelineivEXT,                                         \
      (GLuint pipeline, GLenum pname, GLint * params), VOID_DEFAULT,           \
      thread_ctx, pipeline, pname, params)                                     \
    X(void, glGetPointerv, (GLenum pname, void **params), VOID_DEFAULT,        \
      thread_ctx, pname, params)                                               \
    X(void, glImportSemaphoreWin32NameEXT,                                     \
      (GLuint semaphore, GLenum handleType, const void *name), VOID_DEFAULT,   \
      thread_ctx, semaphore, handleType, name)                                 \
    X(void, glGetLightxv, (GLenum light, GLenum pname, GLfixed * params),      \
      VOID_DEFAULT, thread_ctx, light, pname, params)                          \
    X(void, glTexStorage3DMultisampleOES,                                      \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height, GLsizei depth, GLboolean fixedsamplelocations),         \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height, depth, fixedsamplelocations)                                     \
    X(void, glRenderbufferStorageMultisampleAPPLE,                             \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height),                                                        \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height)                                                                  \
    X(void, glTexParameterxvOES,                                               \
      (GLenum target, GLenum pname, const GLfixed *params), VOID_DEFAULT,      \
      thread_ctx, target, pname, params)                                       \
    X(void, glNormalPointer,                                                   \
      (GLenum type, GLsizei stride, const void *pointer), VOID_DEFAULT,        \
      thread_ctx, type, stride, pointer)                                       \
    X(void, glDepthRangex, (GLfixed n, GLfixed f), VOID_DEFAULT, thread_ctx,   \
      n, f)                                                                    \
    X(void, glGetQueryObjectivEXT, (GLuint id, GLenum pname, GLint * params),  \
      VOID_DEFAULT, thread_ctx, id, pname, params)                             \
    X(void, glScissorIndexedOES,                                               \
      (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height), \
      VOID_DEFAULT, thread_ctx, index, left, bottom, width, height)            \
    X(void, glBufferStorageExternalEXT,                                        \
      (GLenum target, GLintptr offset, GLsizeiptr size,                        \
       GLeglClientBufferEXT clientBuffer, GLbitfield flags),                   \
      VOID_DEFAULT, thread_ctx, target, offset, size, clientBuffer, flags)     \
    X(void, glResolveDepthValuesNV, (void), VOID_DEFAULT, thread_ctx)          \
    X(void, glUniform1ui64NV, (GLint location, GLuint64EXT x), VOID_DEFAULT,   \
      thread_ctx, location, x)                                                 \
    X(void, glSamplerParameterIiv,                                             \
      (GLuint sampler, GLenum pname, const GLint *param), VOID_DEFAULT,        \
      thread_ctx, sampler, pname, param)                                       \
    X(void, glRenderbufferStorageOES,                                          \
      (GLenum target, GLenum internalformat, GLsizei width, GLsizei height),   \
      VOID_DEFAULT, thread_ctx, target, internalformat, width, height)         \
    X(void, glProgramUniform2ui64NV,                                           \
      (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y),          \
      VOID_DEFAULT, thread_ctx, program, location, x, y)                       \
    X(GLboolean, glIsImageHandleResidentNV, (GLuint64 handle), RET_DEFAULT,    \
      thread_ctx, handle)                                                      \
    X(void, glGetObjectLabelKHR,                                               \
      (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length,      \
       GLchar * label),                                                        \
      VOID_DEFAULT, thread_ctx, identifier, name, bufSize, length, label)      \
    X(void, glProgramUniformMatrix4fvEXT,                                      \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glEnablei, (GLenum target, GLuint index), VOID_DEFAULT,            \
      thread_ctx, target, index)                                               \
    X(void, glScissorIndexedvOES, (GLuint index, const GLint *v),              \
      VOID_DEFAULT, thread_ctx, index, v)                                      \
    X(void, glProgramUniform4fEXT,                                             \
      (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2,     \
       GLfloat v3),                                                            \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2, v3)             \
    X(void, glWaitSemaphoreEXT,                                                \
      (GLuint semaphore, GLuint numBufferBarriers, const GLuint *buffers,      \
       GLuint numTextureBarriers, const GLuint *textures,                      \
       const GLenum *srcLayouts),                                              \
      VOID_DEFAULT, thread_ctx, semaphore, numBufferBarriers, buffers,         \
      numTextureBarriers, textures, srcLayouts)                                \
    X(void, glStencilThenCoverFillPathNV,                                      \
      (GLuint path, GLenum fillMode, GLuint mask, GLenum coverMode),           \
      VOID_DEFAULT, thread_ctx, path, fillMode, mask, coverMode)               \
    X(void, glTexEnviv, (GLenum target, GLenum pname, const GLint *params),    \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glUniformMatrix4x2fvNV,                                            \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glProgramUniform2fEXT,                                             \
      (GLuint program, GLint location, GLfloat v0, GLfloat v1), VOID_DEFAULT,  \
      thread_ctx, program, location, v0, v1)                                   \
    X(void, glEndPerfMonitorAMD, (GLuint monitor), VOID_DEFAULT, thread_ctx,   \
      monitor)                                                                 \
    X(void, glNamedFramebufferSampleLocationsfvNV,                             \
      (GLuint framebuffer, GLuint start, GLsizei count, const GLfloat *v),     \
      VOID_DEFAULT, thread_ctx, framebuffer, start, count, v)                  \
    X(GLboolean, glIsProgramPipelineEXT, (GLuint pipeline), RET_DEFAULT,       \
      thread_ctx, pipeline)                                                    \
    X(void, glGetPerfMonitorGroupStringAMD,                                    \
      (GLuint group, GLsizei bufSize, GLsizei * length, GLchar * groupString), \
      VOID_DEFAULT, thread_ctx, group, bufSize, length, groupString)           \
    X(void, glPathParameterivNV,                                               \
      (GLuint path, GLenum pname, const GLint *value), VOID_DEFAULT,           \
      thread_ctx, path, pname, value)                                          \
    X(void, glMatrixMult3x2fNV, (GLenum matrixMode, const GLfloat *m),         \
      VOID_DEFAULT, thread_ctx, matrixMode, m)                                 \
    X(void, glUniform2i64vNV,                                                  \
      (GLint location, GLsizei count, const GLint64EXT *value), VOID_DEFAULT,  \
      thread_ctx, location, count, value)                                      \
    X(void, glUniform4ui64vNV,                                                 \
      (GLint location, GLsizei count, const GLuint64EXT *value), VOID_DEFAULT, \
      thread_ctx, location, count, value)                                      \
    X(void, glFramebufferTexture,                                              \
      (GLenum target, GLenum attachment, GLuint texture, GLint level),         \
      VOID_DEFAULT, thread_ctx, target, attachment, texture, level)            \
    X(void, glFramebufferFoveationConfigQCOM,                                  \
      (GLuint framebuffer, GLuint numLayers, GLuint focalPointsPerLayer,       \
       GLuint requestedFeatures, GLuint * providedFeatures),                   \
      VOID_DEFAULT, thread_ctx, framebuffer, numLayers, focalPointsPerLayer,   \
      requestedFeatures, providedFeatures)                                     \
    X(void, glPolygonOffsetx, (GLfixed factor, GLfixed units), VOID_DEFAULT,   \
      thread_ctx, factor, units)                                               \
    X(void, glBeginConditionalRenderNV, (GLuint id, GLenum mode),              \
      VOID_DEFAULT, thread_ctx, id, mode)                                      \
    X(void, glProgramUniformMatrix2x4fvEXT,                                    \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glImportMemoryWin32HandleEXT,                                      \
      (GLuint memory, GLuint64 size, GLenum handleType, void *handle),         \
      VOID_DEFAULT, thread_ctx, memory, size, handleType, handle)              \
    X(void, glProgramUniform3ui64vNV,                                          \
      (GLuint program, GLint location, GLsizei count,                          \
       const GLuint64EXT *value),                                              \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glDrawTexsvOES, (const GLshort *coords), VOID_DEFAULT, thread_ctx, \
      coords)                                                                  \
    X(void, glBufferStorageEXT,                                                \
      (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags),    \
      VOID_DEFAULT, thread_ctx, target, size, data, flags)                     \
    X(void, glCopyPathNV, (GLuint resultPath, GLuint srcPath), VOID_DEFAULT,   \
      thread_ctx, resultPath, srcPath)                                         \
    X(void, glViewportArrayvNV,                                                \
      (GLuint first, GLsizei count, const GLfloat *v), VOID_DEFAULT,           \
      thread_ctx, first, count, v)                                             \
    X(void, glPathGlyphRangeNV,                                                \
      (GLuint firstPathName, GLenum fontTarget, const void *fontName,          \
       GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs,             \
       GLenum handleMissingGlyphs, GLuint pathParameterTemplate,               \
       GLfloat emScale),                                                       \
      VOID_DEFAULT, thread_ctx, firstPathName, fontTarget, fontName,           \
      fontStyle, firstGlyph, numGlyphs, handleMissingGlyphs,                   \
      pathParameterTemplate, emScale)                                          \
    X(void, glGetPerfQueryInfoINTEL,                                           \
      (GLuint queryId, GLuint queryNameLength, GLchar * queryName,             \
       GLuint * dataSize, GLuint * noCounters, GLuint * noInstances,           \
       GLuint * capsMask),                                                     \
      VOID_DEFAULT, thread_ctx, queryId, queryNameLength, queryName, dataSize, \
      noCounters, noInstances, capsMask)                                       \
    X(void, glSamplerParameterIuivOES,                                         \
      (GLuint sampler, GLenum pname, const GLuint *param), VOID_DEFAULT,       \
      thread_ctx, sampler, pname, param)                                       \
    X(void, glMatrixPushEXT, (GLenum mode), VOID_DEFAULT, thread_ctx, mode)    \
    X(void, glProgramUniform2ui64vNV,                                          \
      (GLuint program, GLint location, GLsizei count,                          \
       const GLuint64EXT *value),                                              \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glClipPlanexIMG, (GLenum p, const GLfixed *eqn), VOID_DEFAULT,     \
      thread_ctx, p, eqn)                                                      \
    X(void, glPatchParameteriEXT, (GLenum pname, GLint value), VOID_DEFAULT,   \
      thread_ctx, pname, value)                                                \
    X(void, glSetFenceNV, (GLuint fence, GLenum condition), VOID_DEFAULT,      \
      thread_ctx, fence, condition)                                            \
    X(void, glViewportArrayvOES,                                               \
      (GLuint first, GLsizei count, const GLfloat *v), VOID_DEFAULT,           \
      thread_ctx, first, count, v)                                             \
    X(void, glGetTexGenfvOES, (GLenum coord, GLenum pname, GLfloat * params),  \
      VOID_DEFAULT, thread_ctx, coord, pname, params)                          \
    X(void, glAlphaFuncQCOM, (GLenum func, GLclampf ref), VOID_DEFAULT,        \
      thread_ctx, func, ref)                                                   \
    X(void, glTexStorage1DEXT,                                                 \
      (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width),   \
      VOID_DEFAULT, thread_ctx, target, levels, internalformat, width)         \
    X(void, glGetnUniformiv,                                                   \
      (GLuint program, GLint location, GLsizei bufSize, GLint * params),       \
      VOID_DEFAULT, thread_ctx, program, location, bufSize, params)            \
    X(void, glGetPerfCounterInfoINTEL,                                         \
      (GLuint queryId, GLuint counterId, GLuint counterNameLength,             \
       GLchar * counterName, GLuint counterDescLength, GLchar * counterDesc,   \
       GLuint * counterOffset, GLuint * counterDataSize,                       \
       GLuint * counterTypeEnum, GLuint * counterDataTypeEnum,                 \
       GLuint64 * rawCounterMaxValue),                                         \
      VOID_DEFAULT, thread_ctx, queryId, counterId, counterNameLength,         \
      counterName, counterDescLength, counterDesc, counterOffset,              \
      counterDataSize, counterTypeEnum, counterDataTypeEnum,                   \
      rawCounterMaxValue)                                                      \
    X(void, glMatrixFrustumEXT,                                                \
      (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom,            \
       GLdouble top, GLdouble zNear, GLdouble zFar),                           \
      VOID_DEFAULT, thread_ctx, mode, left, right, bottom, top, zNear, zFar)   \
    X(GLenum, glGetGraphicsResetStatusEXT, (void), RET_DEFAULT, thread_ctx)    \
    X(void, glTexGeniOES, (GLenum coord, GLenum pname, GLint param),           \
      VOID_DEFAULT, thread_ctx, coord, pname, param)                           \
    X(void, glMakeTextureHandleNonResidentNV, (GLuint64 handle), VOID_DEFAULT, \
      thread_ctx, handle)                                                      \
    X(GLboolean, glReleaseKeyedMutexWin32EXT, (GLuint memory, GLuint64 key),   \
      RET_DEFAULT, thread_ctx, memory, key)                                    \
    X(void, glTexParameterIuivOES,                                             \
      (GLenum target, GLenum pname, const GLuint *params), VOID_DEFAULT,       \
      thread_ctx, target, pname, params)                                       \
    X(void, glEnableiOES, (GLenum target, GLuint index), VOID_DEFAULT,         \
      thread_ctx, target, index)                                               \
    X(void, glDisableiNV, (GLenum target, GLuint index), VOID_DEFAULT,         \
      thread_ctx, target, index)                                               \
    X(void, glSemaphoreParameterui64vEXT,                                      \
      (GLuint semaphore, GLenum pname, const GLuint64 *params), VOID_DEFAULT,  \
      thread_ctx, semaphore, pname, params)                                    \
    X(void, glDrawBuffersEXT, (GLsizei n, const GLenum *bufs), VOID_DEFAULT,   \
      thread_ctx, n, bufs)                                                     \
    X(void, glAlphaFuncx, (GLenum func, GLfixed ref), VOID_DEFAULT,            \
      thread_ctx, func, ref)                                                   \
    X(void, glGetPerfMonitorGroupsAMD,                                         \
      (GLint * numGroups, GLsizei groupsSize, GLuint * groups), VOID_DEFAULT,  \
      thread_ctx, numGroups, groupsSize, groups)                               \
    X(GLsync, glFenceSyncAPPLE, (GLenum condition, GLbitfield flags),          \
      RET_DEFAULT, thread_ctx, condition, flags)                               \
    X(void, glDebugMessageCallbackKHR,                                         \
      (GLDEBUGPROCKHR callback, const void *userParam), VOID_DEFAULT,          \
      thread_ctx, callback, userParam)                                         \
    X(void, glGetObjectLabel,                                                  \
      (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length,      \
       GLchar * label),                                                        \
      VOID_DEFAULT, thread_ctx, identifier, name, bufSize, length, label)      \
    X(void, glProgramUniform4ivEXT,                                            \
      (GLuint program, GLint location, GLsizei count, const GLint *value),     \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(GLenum, glPathMemoryGlyphIndexArrayNV,                                   \
      (GLuint firstPathName, GLenum fontTarget, GLsizeiptr fontSize,           \
       const void *fontData, GLsizei faceIndex, GLuint firstGlyphIndex,        \
       GLsizei numGlyphs, GLuint pathParameterTemplate, GLfloat emScale),      \
      RET_DEFAULT, thread_ctx, firstPathName, fontTarget, fontSize, fontData,  \
      faceIndex, firstGlyphIndex, numGlyphs, pathParameterTemplate, emScale)   \
    X(void, glStencilFillPathNV, (GLuint path, GLenum fillMode, GLuint mask),  \
      VOID_DEFAULT, thread_ctx, path, fillMode, mask)                          \
    X(void, glMinSampleShadingOES, (GLfloat value), VOID_DEFAULT, thread_ctx,  \
      value)                                                                   \
    X(void, glImportMemoryFdEXT,                                               \
      (GLuint memory, GLuint64 size, GLenum handleType, GLint fd),             \
      VOID_DEFAULT, thread_ctx, memory, size, handleType, fd)                  \
    X(void, glClearTexImageEXT,                                                \
      (GLuint texture, GLint level, GLenum format, GLenum type,                \
       const void *data),                                                      \
      VOID_DEFAULT, thread_ctx, texture, level, format, type, data)            \
    X(void, glFramebufferRenderbufferOES,                                      \
      (GLenum target, GLenum attachment, GLenum renderbuffertarget,            \
       GLuint renderbuffer),                                                   \
      VOID_DEFAULT, thread_ctx, target, attachment, renderbuffertarget,        \
      renderbuffer)                                                            \
    X(void, glDrawArraysInstancedBaseInstanceEXT,                              \
      (GLenum mode, GLint first, GLsizei count, GLsizei instancecount,         \
       GLuint baseinstance),                                                   \
      VOID_DEFAULT, thread_ctx, mode, first, count, instancecount,             \
      baseinstance)                                                            \
    X(void, glDepthRangeIndexedfOES, (GLuint index, GLfloat n, GLfloat f),     \
      VOID_DEFAULT, thread_ctx, index, n, f)                                   \
    X(void, glWindowRectanglesEXT,                                             \
      (GLenum mode, GLsizei count, const GLint *box), VOID_DEFAULT,            \
      thread_ctx, mode, count, box)                                            \
    X(void, glProgramUniformMatrix3fvEXT,                                      \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glTexStorageMem3DEXT,                                              \
      (GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width,    \
       GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset),         \
      VOID_DEFAULT, thread_ctx, target, levels, internalFormat, width, height, \
      depth, memory, offset)                                                   \
    X(void, glCreateSemaphoresNV, (GLsizei n, GLuint * semaphores),            \
      VOID_DEFAULT, thread_ctx, n, semaphores)                                 \
    X(void, glObjectLabelKHR,                                                  \
      (GLenum identifier, GLuint name, GLsizei length, const GLchar *label),   \
      VOID_DEFAULT, thread_ctx, identifier, name, length, label)               \
    X(void, glBufferAttachMemoryNV,                                            \
      (GLenum target, GLuint memory, GLuint64 offset), VOID_DEFAULT,           \
      thread_ctx, target, memory, offset)                                      \
    X(void, glMatrixLoadTransposefEXT, (GLenum mode, const GLfloat *m),        \
      VOID_DEFAULT, thread_ctx, mode, m)                                       \
    X(void, glGetPerfMonitorCounterInfoAMD,                                    \
      (GLuint group, GLuint counter, GLenum pname, void *data), VOID_DEFAULT,  \
      thread_ctx, group, counter, pname, data)                                 \
    X(void, glPathStencilFuncNV, (GLenum func, GLint ref, GLuint mask),        \
      VOID_DEFAULT, thread_ctx, func, ref, mask)                               \
    X(void, glSamplerParameterIuivEXT,                                         \
      (GLuint sampler, GLenum pname, const GLuint *param), VOID_DEFAULT,       \
      thread_ctx, sampler, pname, param)                                       \
    X(void, glTextureFoveationParametersQCOM,                                  \
      (GLuint texture, GLuint layer, GLuint focalPoint, GLfloat focalX,        \
       GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea),       \
      VOID_DEFAULT, thread_ctx, texture, layer, focalPoint, focalX, focalY,    \
      gainX, gainY, foveaArea)                                                 \
    X(void, glProgramUniform1uiEXT,                                            \
      (GLuint program, GLint location, GLuint v0), VOID_DEFAULT, thread_ctx,   \
      program, location, v0)                                                   \
    X(void, glCoverageOperationNV, (GLenum operation), VOID_DEFAULT,           \
      thread_ctx, operation)                                                   \
    X(void, glDisableDriverControlQCOM, (GLuint driverControl), VOID_DEFAULT,  \
      thread_ctx, driverControl)                                               \
    X(void, glReadBufferIndexedEXT, (GLenum src, GLint index), VOID_DEFAULT,   \
      thread_ctx, src, index)                                                  \
    X(void, glStencilStrokePathNV,                                             \
      (GLuint path, GLint reference, GLuint mask), VOID_DEFAULT, thread_ctx,   \
      path, reference, mask)                                                   \
    X(void, glTexCoordPointer,                                                 \
      (GLint size, GLenum type, GLsizei stride, const void *pointer),          \
      VOID_DEFAULT, thread_ctx, size, type, stride, pointer)                   \
    X(void, glTexParameterxOES, (GLenum target, GLenum pname, GLfixed param),  \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glDeleteQueriesEXT, (GLsizei n, const GLuint *ids), VOID_DEFAULT,  \
      thread_ctx, n, ids)                                                      \
    X(void, glBlendBarrierKHR, (void), VOID_DEFAULT, thread_ctx)               \
    X(void, glSampleCoveragex, (GLclampx value, GLboolean invert),             \
      VOID_DEFAULT, thread_ctx, value, invert)                                 \
    X(void, glGetUniformi64vNV,                                                \
      (GLuint program, GLint location, GLint64EXT * params), VOID_DEFAULT,     \
      thread_ctx, program, location, params)                                   \
    X(void, glTexStorageMem3DMultisampleEXT,                                   \
      (GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width,   \
       GLsizei height, GLsizei depth, GLboolean fixedSampleLocations,          \
       GLuint memory, GLuint64 offset),                                        \
      VOID_DEFAULT, thread_ctx, target, samples, internalFormat, width,        \
      height, depth, fixedSampleLocations, memory, offset)                     \
    X(void, glBlendParameteriNV, (GLenum pname, GLint value), VOID_DEFAULT,    \
      thread_ctx, pname, value)                                                \
    X(void, glDrawRangeElementsBaseVertexEXT,                                  \
      (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,      \
       const void *indices, GLint basevertex),                                 \
      VOID_DEFAULT, thread_ctx, mode, start, end, count, type, indices,        \
      basevertex)                                                              \
    X(void, glGetFixedv, (GLenum pname, GLfixed * params), VOID_DEFAULT,       \
      thread_ctx, pname, params)                                               \
    X(void, glCreatePerfQueryINTEL, (GLuint queryId, GLuint * queryHandle),    \
      VOID_DEFAULT, thread_ctx, queryId, queryHandle)                          \
    X(void, glGetTexEnvxv, (GLenum target, GLenum pname, GLfixed * params),    \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(GLboolean, glTestFenceNV, (GLuint fence), RET_DEFAULT, thread_ctx,       \
      fence)                                                                   \
    X(void, glFogfv, (GLenum pname, const GLfloat *params), VOID_DEFAULT,      \
      thread_ctx, pname, params)                                               \
    X(void, glPushDebugGroupKHR,                                               \
      (GLenum source, GLuint id, GLsizei length, const GLchar *message),       \
      VOID_DEFAULT, thread_ctx, source, id, length, message)                   \
    X(void, glPolygonModeNV, (GLenum face, GLenum mode), VOID_DEFAULT,         \
      thread_ctx, face, mode)                                                  \
    X(void, glDisablei, (GLenum target, GLuint index), VOID_DEFAULT,           \
      thread_ctx, target, index)                                               \
    X(GLboolean, glIsPathNV, (GLuint path), RET_DEFAULT, thread_ctx, path)     \
    X(void, glGetTexParameterIuivOES,                                          \
      (GLenum target, GLenum pname, GLuint * params), VOID_DEFAULT,            \
      thread_ctx, target, pname, params)                                       \
    X(void, glGetTexParameterIiv,                                              \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glOrthof,                                                          \
      (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glEndConditionalRenderNV, (void), VOID_DEFAULT, thread_ctx)        \
    X(void, glGetInteger64vAPPLE, (GLenum pname, GLint64 * params),            \
      VOID_DEFAULT, thread_ctx, pname, params)                                 \
    X(void, glGetTexParameterxv,                                               \
      (GLenum target, GLenum pname, GLfixed * params), VOID_DEFAULT,           \
      thread_ctx, target, pname, params)                                       \
    X(void, glMaxShaderCompilerThreadsKHR, (GLuint count), VOID_DEFAULT,       \
      thread_ctx, count)                                                       \
    X(void, glTexStorageMem2DMultisampleEXT,                                   \
      (GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width,   \
       GLsizei height, GLboolean fixedSampleLocations, GLuint memory,          \
       GLuint64 offset),                                                       \
      VOID_DEFAULT, thread_ctx, target, samples, internalFormat, width,        \
      height, fixedSampleLocations, memory, offset)                            \
    X(void, glGetFloati_vNV, (GLenum target, GLuint index, GLfloat * data),    \
      VOID_DEFAULT, thread_ctx, target, index, data)                           \
    X(void, glGetPathCommandsNV, (GLuint path, GLubyte * commands),            \
      VOID_DEFAULT, thread_ctx, path, commands)                                \
    X(void, glUniformHandleui64vNV,                                            \
      (GLint location, GLsizei count, const GLuint64 *value), VOID_DEFAULT,    \
      thread_ctx, location, count, value)                                      \
    X(void, glGetProgramPipelineInfoLogEXT,                                    \
      (GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog),  \
      VOID_DEFAULT, thread_ctx, pipeline, bufSize, length, infoLog)            \
    X(void, glFramebufferTextureLayerDownsampleIMG,                            \
      (GLenum target, GLenum attachment, GLuint texture, GLint level,          \
       GLint layer, GLint xscale, GLint yscale),                               \
      VOID_DEFAULT, thread_ctx, target, attachment, texture, level, layer,     \
      xscale, yscale)                                                          \
    X(void, glProgramUniform2uivEXT,                                           \
      (GLuint program, GLint location, GLsizei count, const GLuint *value),    \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(GLenum, glClientWaitSyncAPPLE,                                           \
      (GLsync sync, GLbitfield flags, GLuint64 timeout), RET_DEFAULT,          \
      thread_ctx, sync, flags, timeout)                                        \
    X(void, glBindRenderbufferOES, (GLenum target, GLuint renderbuffer),       \
      VOID_DEFAULT, thread_ctx, target, renderbuffer)                          \
    X(void, glEnableClientState, (GLenum array), VOID_DEFAULT, thread_ctx,     \
      array)                                                                   \
    X(void, glShadingRateImageBarrierNV, (GLboolean synchronize),              \
      VOID_DEFAULT, thread_ctx, synchronize)                                   \
    X(void, glPushMatrix, (void), VOID_DEFAULT, thread_ctx)                    \
    X(void, glColor4f,                                                         \
      (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glGetFloati_vOES, (GLenum target, GLuint index, GLfloat * data),   \
      VOID_DEFAULT, thread_ctx, target, index, data)                           \
    X(void, glDeleteSyncAPPLE, (GLsync sync), VOID_DEFAULT, thread_ctx, sync)  \
    X(void, glDrawTexfOES,                                                     \
      (GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height),        \
      VOID_DEFAULT, thread_ctx, x, y, z, width, height)                        \
    X(void, glEGLImageTargetTexStorageEXT,                                     \
      (GLenum target, GLeglImageOES image, const GLint *attrib_list),          \
      VOID_DEFAULT, thread_ctx, target, image, attrib_list)                    \
    X(void, glEndPerfQueryINTEL, (GLuint queryHandle), VOID_DEFAULT,           \
      thread_ctx, queryHandle)                                                 \
    X(void, glPathSubCommandsNV,                                               \
      (GLuint path, GLsizei commandStart, GLsizei commandsToDelete,            \
       GLsizei numCommands, const GLubyte *commands, GLsizei numCoords,        \
       GLenum coordType, const void *coords),                                  \
      VOID_DEFAULT, thread_ctx, path, commandStart, commandsToDelete,          \
      numCommands, commands, numCoords, coordType, coords)                     \
    X(void, glProgramUniform1ui64vNV,                                          \
      (GLuint program, GLint location, GLsizei count,                          \
       const GLuint64EXT *value),                                              \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glFragmentCoverageColorNV, (GLuint color), VOID_DEFAULT,           \
      thread_ctx, color)                                                       \
    X(void, glClearTexSubImageEXT,                                             \
      (GLuint texture, GLint level, GLint xoffset, GLint yoffset,              \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLenum format, GLenum type, const void *data),                          \
      VOID_DEFAULT, thread_ctx, texture, level, xoffset, yoffset, zoffset,     \
      width, height, depth, format, type, data)                                \
    X(void, glPointParameterx, (GLenum pname, GLfixed param), VOID_DEFAULT,    \
      thread_ctx, pname, param)                                                \
    X(void, glScissorArrayvNV, (GLuint first, GLsizei count, const GLint *v),  \
      VOID_DEFAULT, thread_ctx, first, count, v)                               \
    X(void, glPatchParameteri, (GLenum pname, GLint value), VOID_DEFAULT,      \
      thread_ctx, pname, value)                                                \
    X(void, glNamedBufferPageCommitmentMemNV,                                  \
      (GLuint buffer, GLintptr offset, GLsizeiptr size, GLuint memory,         \
       GLuint64 memOffset, GLboolean commit),                                  \
      VOID_DEFAULT, thread_ctx, buffer, offset, size, memory, memOffset,       \
      commit)                                                                  \
    X(void, glEnableiEXT, (GLenum target, GLuint index), VOID_DEFAULT,         \
      thread_ctx, target, index)                                               \
    X(void, glGenProgramPipelinesEXT, (GLsizei n, GLuint * pipelines),         \
      VOID_DEFAULT, thread_ctx, n, pipelines)                                  \
    X(void, glUniformHandleui64IMG, (GLint location, GLuint64 value),          \
      VOID_DEFAULT, thread_ctx, location, value)                               \
    X(void, glCopyTexSubImage3DOES,                                            \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height),        \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset, x,   \
      y, width, height)                                                        \
    X(GLboolean, glIsEnablediEXT, (GLenum target, GLuint index), RET_DEFAULT,  \
      thread_ctx, target, index)                                               \
    X(void, glDrawTransformFeedbackInstancedEXT,                               \
      (GLenum mode, GLuint id, GLsizei instancecount), VOID_DEFAULT,           \
      thread_ctx, mode, id, instancecount)                                     \
    X(void, glBlendEquationOES, (GLenum mode), VOID_DEFAULT, thread_ctx, mode) \
    X(void, glProgramUniform1fvEXT,                                            \
      (GLuint program, GLint location, GLsizei count, const GLfloat *value),   \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(GLboolean, glIsFramebufferOES, (GLuint framebuffer), RET_DEFAULT,        \
      thread_ctx, framebuffer)                                                 \
    X(void, glEnableiNV, (GLenum target, GLuint index), VOID_DEFAULT,          \
      thread_ctx, target, index)                                               \
    X(void, glGenPerfMonitorsAMD, (GLsizei n, GLuint * monitors),              \
      VOID_DEFAULT, thread_ctx, n, monitors)                                   \
    X(void, glProgramUniform1fEXT,                                             \
      (GLuint program, GLint location, GLfloat v0), VOID_DEFAULT, thread_ctx,  \
      program, location, v0)                                                   \
    X(void, glShadingRateImagePaletteNV,                                       \
      (GLuint viewport, GLuint first, GLsizei count, const GLenum *rates),     \
      VOID_DEFAULT, thread_ctx, viewport, first, count, rates)                 \
    X(void, glDrawTexxOES,                                                     \
      (GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height),        \
      VOID_DEFAULT, thread_ctx, x, y, z, width, height)                        \
    X(void, glGetMaterialxv, (GLenum face, GLenum pname, GLfixed * params),    \
      VOID_DEFAULT, thread_ctx, face, pname, params)                           \
    X(void, glReadnPixelsKHR,                                                  \
      (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format,         \
       GLenum type, GLsizei bufSize, void *data),                              \
      VOID_DEFAULT, thread_ctx, x, y, width, height, format, type, bufSize,    \
      data)                                                                    \
    X(void, glPathParameteriNV, (GLuint path, GLenum pname, GLint value),      \
      VOID_DEFAULT, thread_ctx, path, pname, value)                            \
    X(void, glSamplerParameterIivOES,                                          \
      (GLuint sampler, GLenum pname, const GLint *param), VOID_DEFAULT,        \
      thread_ctx, sampler, pname, param)                                       \
    X(GLsizei, glGetFramebufferPixelLocalStorageSizeEXT, (GLuint target),      \
      RET_DEFAULT, thread_ctx, target)                                         \
    X(void, glProgramUniform2iEXT,                                             \
      (GLuint program, GLint location, GLint v0, GLint v1), VOID_DEFAULT,      \
      thread_ctx, program, location, v0, v1)                                   \
    X(void, glColorPointer,                                                    \
      (GLint size, GLenum type, GLsizei stride, const void *pointer),          \
      VOID_DEFAULT, thread_ctx, size, type, stride, pointer)                   \
    X(void, glDrawBuffersIndexedEXT,                                           \
      (GLint n, const GLenum *location, const GLint *indices), VOID_DEFAULT,   \
      thread_ctx, n, location, indices)                                        \
    X(void, glMaterialfv, (GLenum face, GLenum pname, const GLfloat *params),  \
      VOID_DEFAULT, thread_ctx, face, pname, params)                           \
    X(void, glScissorIndexedNV,                                                \
      (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height), \
      VOID_DEFAULT, thread_ctx, index, left, bottom, width, height)            \
    X(void, glTexParameterIiv,                                                 \
      (GLenum target, GLenum pname, const GLint *params), VOID_DEFAULT,        \
      thread_ctx, target, pname, params)                                       \
    X(GLboolean, glIsFenceNV, (GLuint fence), RET_DEFAULT, thread_ctx, fence)  \
    X(void, glScissorExclusiveNV,                                              \
      (GLint x, GLint y, GLsizei width, GLsizei height), VOID_DEFAULT,         \
      thread_ctx, x, y, width, height)                                         \
    X(void, glProgramUniformMatrix4x2fvEXT,                                    \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glTexParameterIuiv,                                                \
      (GLenum target, GLenum pname, const GLuint *params), VOID_DEFAULT,       \
      thread_ctx, target, pname, params)                                       \
    X(void, glGetnUniformfvKHR,                                                \
      (GLuint program, GLint location, GLsizei bufSize, GLfloat * params),     \
      VOID_DEFAULT, thread_ctx, program, location, bufSize, params)            \
    X(GLboolean, glIsVertexArrayOES, (GLuint array), RET_DEFAULT, thread_ctx,  \
      array)                                                                   \
    X(void, glGenRenderbuffersOES, (GLsizei n, GLuint * renderbuffers),        \
      VOID_DEFAULT, thread_ctx, n, renderbuffers)                              \
    X(void, glVertexAttribDivisorEXT, (GLuint index, GLuint divisor),          \
      VOID_DEFAULT, thread_ctx, index, divisor)                                \
    X(void, glMakeTextureHandleResidentNV, (GLuint64 handle), VOID_DEFAULT,    \
      thread_ctx, handle)                                                      \
    X(void, glMatrixMultfEXT, (GLenum mode, const GLfloat *m), VOID_DEFAULT,   \
      thread_ctx, mode, m)                                                     \
    X(void, glMatrixMultdEXT, (GLenum mode, const GLdouble *m), VOID_DEFAULT,  \
      thread_ctx, mode, m)                                                     \
    X(void, glProgramUniformMatrix2x3fvEXT,                                    \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glGetPointervKHR, (GLenum pname, void **params), VOID_DEFAULT,     \
      thread_ctx, pname, params)                                               \
    X(void, glExtGetProgramBinarySourceQCOM,                                   \
      (GLuint program, GLenum shadertype, GLchar * source, GLint * length),    \
      VOID_DEFAULT, thread_ctx, program, shadertype, source, length)           \
    X(void, glFramebufferFetchBarrierQCOM, (void), VOID_DEFAULT, thread_ctx)   \
    X(void, glGetCoverageModulationTableNV, (GLsizei bufsize, GLfloat * v),    \
      VOID_DEFAULT, thread_ctx, bufsize, v)                                    \
    X(void, glGetClipPlanex, (GLenum plane, GLfixed * equation), VOID_DEFAULT, \
      thread_ctx, plane, equation)                                             \
    X(void, glGetIntegeri_vEXT, (GLenum target, GLuint index, GLint * data),   \
      VOID_DEFAULT, thread_ctx, target, index, data)                           \
    X(void, glReadnPixels,                                                     \
      (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format,         \
       GLenum type, GLsizei bufSize, void *data),                              \
      VOID_DEFAULT, thread_ctx, x, y, width, height, format, type, bufSize,    \
      data)                                                                    \
    X(GLboolean, glExtIsProgramBinaryQCOM, (GLuint program), RET_DEFAULT,      \
      thread_ctx, program)                                                     \
    X(void, glMatrixTranslatefEXT,                                             \
      (GLenum mode, GLfloat x, GLfloat y, GLfloat z), VOID_DEFAULT,            \
      thread_ctx, mode, x, y, z)                                               \
    X(void, glGetShadingRateSampleLocationivNV,                                \
      (GLenum rate, GLuint samples, GLuint index, GLint * location),           \
      VOID_DEFAULT, thread_ctx, rate, samples, index, location)                \
    X(void, glProgramUniform4ui64vNV,                                          \
      (GLuint program, GLint location, GLsizei count,                          \
       const GLuint64EXT *value),                                              \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glGetQueryObjectuivEXT,                                            \
      (GLuint id, GLenum pname, GLuint * params), VOID_DEFAULT, thread_ctx,    \
      id, pname, params)                                                       \
    X(void, glNamedBufferAttachMemoryNV,                                       \
      (GLuint buffer, GLuint memory, GLuint64 offset), VOID_DEFAULT,           \
      thread_ctx, buffer, memory, offset)                                      \
    X(void, glDeletePerfMonitorsAMD, (GLsizei n, GLuint * monitors),           \
      VOID_DEFAULT, thread_ctx, n, monitors)                                   \
    X(void, glInOutTest,                                                       \
      (GLint a, GLint b, const GLchar *e, GLint *c, GLdouble *d,               \
       GLsizei buf_len, GLchar *f),                                            \
      VOID_DEFAULT, thread_ctx, a, b, e, c, d, buf_len, f)                     \
    X(void, glExtGetTexLevelParameterivQCOM,                                   \
      (GLuint texture, GLenum face, GLint level, GLenum pname,                 \
       GLint * params),                                                        \
      VOID_DEFAULT, thread_ctx, texture, face, level, pname, params)           \
    X(void, glMatrixLoadIdentityEXT, (GLenum mode), VOID_DEFAULT, thread_ctx,  \
      mode)                                                                    \
    X(void, glUniform2ui64NV, (GLint location, GLuint64EXT x, GLuint64EXT y),  \
      VOID_DEFAULT, thread_ctx, location, x, y)                                \
    X(void *, glMapBufferOES, (GLenum target, GLenum access), RET_DEFAULT,     \
      thread_ctx, target, access)                                              \
    X(void, glUseProgramStagesEXT,                                             \
      (GLuint pipeline, GLbitfield stages, GLuint program), VOID_DEFAULT,      \
      thread_ctx, pipeline, stages, program)                                   \
    X(void, glMatrixScaledEXT,                                                 \
      (GLenum mode, GLdouble x, GLdouble y, GLdouble z), VOID_DEFAULT,         \
      thread_ctx, mode, x, y, z)                                               \
    X(void, glProgramUniformMatrix3x4fvEXT,                                    \
      (GLuint program, GLint location, GLsizei count, GLboolean transpose,     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, program, location, count, transpose, value)    \
    X(void, glOrthox,                                                          \
      (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glTextureAttachMemoryNV,                                           \
      (GLuint texture, GLuint memory, GLuint64 offset), VOID_DEFAULT,          \
      thread_ctx, texture, memory, offset)                                     \
    X(void, glGetnUniformivKHR,                                                \
      (GLuint program, GLint location, GLsizei bufSize, GLint * params),       \
      VOID_DEFAULT, thread_ctx, program, location, bufSize, params)            \
    X(void, glDrawMeshTasksIndirectNV, (GLintptr indirect), VOID_DEFAULT,      \
      thread_ctx, indirect)                                                    \
    X(GLuint, glGetDebugMessageLog,                                            \
      (GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types,        \
       GLuint * ids, GLenum * severities, GLsizei * lengths,                   \
       GLchar * messageLog),                                                   \
      RET_DEFAULT, thread_ctx, count, bufSize, sources, types, ids,            \
      severities, lengths, messageLog)                                         \
    X(void, glUniformMatrix2x3fvNV,                                            \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glEndQueryEXT, (GLenum target), VOID_DEFAULT, thread_ctx, target)  \
    X(void, glCoverStrokePathInstancedNV,                                      \
      (GLsizei numPaths, GLenum pathNameType, const void *paths,               \
       GLuint pathBase, GLenum coverMode, GLenum transformType,                \
       const GLfloat *transformValues),                                        \
      VOID_DEFAULT, thread_ctx, numPaths, pathNameType, paths, pathBase,       \
      coverMode, transformType, transformValues)                               \
    X(void, glDeletePerfQueryINTEL, (GLuint queryHandle), VOID_DEFAULT,        \
      thread_ctx, queryHandle)                                                 \
    X(void, glTextureStorage1DEXT,                                             \
      (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat,   \
       GLsizei width),                                                         \
      VOID_DEFAULT, thread_ctx, texture, target, levels, internalformat,       \
      width)                                                                   \
    X(void, glColor4ub,                                                        \
      (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glPopDebugGroupKHR, (void), VOID_DEFAULT, thread_ctx)              \
    X(void, glLabelObjectEXT,                                                  \
      (GLenum type, GLuint object, GLsizei length, const GLchar *label),       \
      VOID_DEFAULT, thread_ctx, type, object, length, label)                   \
    X(void, glShadingRateSampleOrderCustomNV,                                  \
      (GLenum rate, GLuint samples, const GLint *locations), VOID_DEFAULT,     \
      thread_ctx, rate, samples, locations)                                    \
    X(void, glMakeImageHandleNonResidentNV, (GLuint64 handle), VOID_DEFAULT,   \
      thread_ctx, handle)                                                      \
    X(void, glMultiTexCoord4x,                                                 \
      (GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q),            \
      VOID_DEFAULT, thread_ctx, texture, s, t, r, q)                           \
    X(void, glMatrixRotatedEXT,                                                \
      (GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z),       \
      VOID_DEFAULT, thread_ctx, mode, angle, x, y, z)                          \
    X(void, glGetTexParameterIuivEXT,                                          \
      (GLenum target, GLenum pname, GLuint * params), VOID_DEFAULT,            \
      thread_ctx, target, pname, params)                                       \
    X(void, glMatrixLoad3x2fNV, (GLenum matrixMode, const GLfloat *m),         \
      VOID_DEFAULT, thread_ctx, matrixMode, m)                                 \
    X(void, glMatrixTranslatedEXT,                                             \
      (GLenum mode, GLdouble x, GLdouble y, GLdouble z), VOID_DEFAULT,         \
      thread_ctx, mode, x, y, z)                                               \
    X(void, glProgramUniform3i64NV,                                            \
      (GLuint program, GLint location, GLint64EXT x, GLint64EXT y,             \
       GLint64EXT z),                                                          \
      VOID_DEFAULT, thread_ctx, program, location, x, y, z)                    \
    X(void, glGetPerfMonitorCounterDataAMD,                                    \
      (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint * data,          \
       GLint * bytesWritten),                                                  \
      VOID_DEFAULT, thread_ctx, monitor, pname, dataSize, data, bytesWritten)  \
    X(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z),       \
      VOID_DEFAULT, thread_ctx, angle, x, y, z)                                \
    X(void, glMultiDrawElementsEXT,                                            \
      (GLenum mode, const GLsizei *count, GLenum type,                         \
       const void *const *indices, GLsizei primcount),                         \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, primcount)         \
    X(void, glMemoryObjectParameterivEXT,                                      \
      (GLuint memoryObject, GLenum pname, const GLint *params), VOID_DEFAULT,  \
      thread_ctx, memoryObject, pname, params)                                 \
    X(void, glGetUnsignedBytei_vEXT,                                           \
      (GLenum target, GLuint index, GLubyte * data), VOID_DEFAULT, thread_ctx, \
      target, index, data)                                                     \
    X(void, glFramebufferPixelLocalStorageSizeEXT,                             \
      (GLuint target, GLsizei size), VOID_DEFAULT, thread_ctx, target, size)   \
    X(void, glUniform1ui64vNV,                                                 \
      (GLint location, GLsizei count, const GLuint64EXT *value), VOID_DEFAULT, \
      thread_ctx, location, count, value)                                      \
    X(void, glRotatef, (GLfloat angle, GLfloat x, GLfloat y, GLfloat z),       \
      VOID_DEFAULT, thread_ctx, angle, x, y, z)                                \
    X(void, glConservativeRasterParameteriNV, (GLenum pname, GLint param),     \
      VOID_DEFAULT, thread_ctx, pname, param)                                  \
    X(void, glCurrentPaletteMatrixOES, (GLuint matrixpaletteindex),            \
      VOID_DEFAULT, thread_ctx, matrixpaletteindex)                            \
    X(void, glObjectLabel,                                                     \
      (GLenum identifier, GLuint name, GLsizei length, const GLchar *label),   \
      VOID_DEFAULT, thread_ctx, identifier, name, length, label)               \
    X(void, glGetTexGenivOES, (GLenum coord, GLenum pname, GLint * params),    \
      VOID_DEFAULT, thread_ctx, coord, pname, params)                          \
    X(void, glPathCoordsNV,                                                    \
      (GLuint path, GLsizei numCoords, GLenum coordType, const void *coords),  \
      VOID_DEFAULT, thread_ctx, path, numCoords, coordType, coords)            \
    X(void, glEnableDriverControlQCOM, (GLuint driverControl), VOID_DEFAULT,   \
      thread_ctx, driverControl)                                               \
    X(void, glClipControlEXT, (GLenum origin, GLenum depth), VOID_DEFAULT,     \
      thread_ctx, origin, depth)                                               \
    X(void, glGetObjectPtrLabelKHR,                                            \
      (const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label),      \
      VOID_DEFAULT, thread_ctx, ptr, bufSize, length, label)                   \
    X(void, glDebugMessageControl,                                             \
      (GLenum source, GLenum type, GLenum severity, GLsizei count,             \
       const GLuint *ids, GLboolean enabled),                                  \
      VOID_DEFAULT, thread_ctx, source, type, severity, count, ids, enabled)   \
    X(void, glProgramUniform1ivEXT,                                            \
      (GLuint program, GLint location, GLsizei count, const GLint *value),     \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glWaitSyncAPPLE,                                                   \
      (GLsync sync, GLbitfield flags, GLuint64 timeout), VOID_DEFAULT,         \
      thread_ctx, sync, flags, timeout)                                        \
    X(GLenum, glGetGraphicsResetStatusKHR, (void), RET_DEFAULT, thread_ctx)    \
    X(void, glMatrixMultTransposedEXT, (GLenum mode, const GLdouble *m),       \
      VOID_DEFAULT, thread_ctx, mode, m)                                       \
    X(void, glGetnUniformuivKHR,                                               \
      (GLuint program, GLint location, GLsizei bufSize, GLuint * params),      \
      VOID_DEFAULT, thread_ctx, program, location, bufSize, params)            \
    X(void, glRenderbufferStorageMultisampleAdvancedAMD,                       \
      (GLenum target, GLsizei samples, GLsizei storageSamples,                 \
       GLenum internalformat, GLsizei width, GLsizei height),                  \
      VOID_DEFAULT, thread_ctx, target, samples, storageSamples,               \
      internalformat, width, height)                                           \
    X(void, glFramebufferTextureMultisampleMultiviewOVR,                       \
      (GLenum target, GLenum attachment, GLuint texture, GLint level,          \
       GLsizei samples, GLint baseViewIndex, GLsizei numViews),                \
      VOID_DEFAULT, thread_ctx, target, attachment, texture, level, samples,   \
      baseViewIndex, numViews)                                                 \
    X(void, glGetPathDashArrayNV, (GLuint path, GLfloat * dashArray),          \
      VOID_DEFAULT, thread_ctx, path, dashArray)                               \
    X(GLboolean, glIsPointInFillPathNV,                                        \
      (GLuint path, GLuint mask, GLfloat x, GLfloat y), RET_DEFAULT,           \
      thread_ctx, path, mask, x, y)                                            \
    X(void, glUniform2ui64vNV,                                                 \
      (GLint location, GLsizei count, const GLuint64EXT *value), VOID_DEFAULT, \
      thread_ctx, location, count, value)                                      \
    X(void, glProgramUniform2i64vNV,                                           \
      (GLuint program, GLint location, GLsizei count,                          \
       const GLint64EXT *value),                                               \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glBlendBarrierNV, (void), VOID_DEFAULT, thread_ctx)                \
    X(void, glMatrixIndexPointerOES,                                           \
      (GLint size, GLenum type, GLsizei stride, const void *pointer),          \
      VOID_DEFAULT, thread_ctx, size, type, stride, pointer)                   \
    X(void, glTextureStorageMem3DMultisampleEXT,                               \
      (GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width,  \
       GLsizei height, GLsizei depth, GLboolean fixedSampleLocations,          \
       GLuint memory, GLuint64 offset),                                        \
      VOID_DEFAULT, thread_ctx, texture, samples, internalFormat, width,       \
      height, depth, fixedSampleLocations, memory, offset)                     \
    X(void, glExtGetTexturesQCOM,                                              \
      (GLuint * textures, GLint maxTextures, GLint * numTextures),             \
      VOID_DEFAULT, thread_ctx, textures, maxTextures, numTextures)            \
    X(void, glProgramUniform4i64vNV,                                           \
      (GLuint program, GLint location, GLsizei count,                          \
       const GLint64EXT *value),                                               \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glCoverageModulationTableNV, (GLsizei n, const GLfloat *v),        \
      VOID_DEFAULT, thread_ctx, n, v)                                          \
    X(void, glProgramUniformHandleui64vNV,                                     \
      (GLuint program, GLint location, GLsizei count, const GLuint64 *values), \
      VOID_DEFAULT, thread_ctx, program, location, count, values)              \
    X(void, glBlitFramebufferNV,                                               \
      (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0,        \
       GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter), \
      VOID_DEFAULT, thread_ctx, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0,      \
      dstX1, dstY1, mask, filter)                                              \
    X(void, glCopyImageSubData,                                                \
      (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX,           \
       GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget,               \
       GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth,   \
       GLsizei srcHeight, GLsizei srcDepth),                                   \
      VOID_DEFAULT, thread_ctx, srcName, srcTarget, srcLevel, srcX, srcY,      \
      srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth,          \
      srcHeight, srcDepth)                                                     \
    X(void, glImportMemoryWin32NameEXT,                                        \
      (GLuint memory, GLuint64 size, GLenum handleType, const void *name),     \
      VOID_DEFAULT, thread_ctx, memory, size, handleType, name)                \
    X(void, glGetPathParameterivNV,                                            \
      (GLuint path, GLenum pname, GLint * value), VOID_DEFAULT, thread_ctx,    \
      path, pname, value)                                                      \
    X(void, glGetUnsignedBytevEXT, (GLenum pname, GLubyte * data),             \
      VOID_DEFAULT, thread_ctx, pname, data)                                   \
    X(void, glDrawElementsInstancedNV,                                         \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei primcount),                                                     \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, primcount)         \
    X(void, glProgramUniform3uivEXT,                                           \
      (GLuint program, GLint location, GLsizei count, const GLuint *value),    \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glFramebufferParameteriMESA,                                       \
      (GLenum target, GLenum pname, GLint param), VOID_DEFAULT, thread_ctx,    \
      target, pname, param)                                                    \
    X(void, glResetMemoryObjectParameterNV, (GLuint memory, GLenum pname),     \
      VOID_DEFAULT, thread_ctx, memory, pname)                                 \
    X(void, glPathDashArrayNV,                                                 \
      (GLuint path, GLsizei dashCount, const GLfloat *dashArray),              \
      VOID_DEFAULT, thread_ctx, path, dashCount, dashArray)                    \
    X(void, glPointSizex, (GLfixed size), VOID_DEFAULT, thread_ctx, size)      \
    X(void, glPathSubCoordsNV,                                                 \
      (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType,   \
       const void *coords),                                                    \
      VOID_DEFAULT, thread_ctx, path, coordStart, numCoords, coordType,        \
      coords)                                                                  \
    X(void, glGetQueryObjectui64vEXT,                                          \
      (GLuint id, GLenum pname, GLuint64 * params), VOID_DEFAULT, thread_ctx,  \
      id, pname, params)                                                       \
    X(void, glPolygonOffsetClampEXT,                                           \
      (GLfloat factor, GLfloat units, GLfloat clamp), VOID_DEFAULT,            \
      thread_ctx, factor, units, clamp)                                        \
    X(void, glDisableiEXT, (GLenum target, GLuint index), VOID_DEFAULT,        \
      thread_ctx, target, index)                                               \
    X(void, glLogicOp, (GLenum opcode), VOID_DEFAULT, thread_ctx, opcode)      \
    X(void, glStencilThenCoverStrokePathNV,                                    \
      (GLuint path, GLint reference, GLuint mask, GLenum coverMode),           \
      VOID_DEFAULT, thread_ctx, path, reference, mask, coverMode)              \
    X(void, glUniformMatrix3x4fvNV,                                            \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glFlushMappedBufferRangeEXT,                                       \
      (GLenum target, GLintptr offset, GLsizeiptr length), VOID_DEFAULT,       \
      thread_ctx, target, offset, length)                                      \
    X(void, glProgramUniform1i64vNV,                                           \
      (GLuint program, GLint location, GLsizei count,                          \
       const GLint64EXT *value),                                               \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glScalef, (GLfloat x, GLfloat y, GLfloat z), VOID_DEFAULT,         \
      thread_ctx, x, y, z)                                                     \
    X(void, glGenerateMipmapOES, (GLenum target), VOID_DEFAULT, thread_ctx,    \
      target)                                                                  \
    X(void, glTexParameterIivOES,                                              \
      (GLenum target, GLenum pname, const GLint *params), VOID_DEFAULT,        \
      thread_ctx, target, pname, params)                                       \
    X(void, glFramebufferFoveationParametersQCOM,                              \
      (GLuint framebuffer, GLuint layer, GLuint focalPoint, GLfloat focalX,    \
       GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea),       \
      VOID_DEFAULT, thread_ctx, framebuffer, layer, focalPoint, focalX,        \
      focalY, gainX, gainY, foveaArea)                                         \
    X(void, glProgramUniform3fEXT,                                             \
      (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2),    \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2)                 \
    X(void, glLightxv, (GLenum light, GLenum pname, const GLfixed *params),    \
      VOID_DEFAULT, thread_ctx, light, pname, params)                          \
    X(void, glGetSemaphoreParameterivNV,                                       \
      (GLuint semaphore, GLenum pname, GLint * params), VOID_DEFAULT,          \
      thread_ctx, semaphore, pname, params)                                    \
    X(void, glGetPerfMonitorCountersAMD,                                       \
      (GLuint group, GLint * numCounters, GLint * maxActiveCounters,           \
       GLsizei counterSize, GLuint * counters),                                \
      VOID_DEFAULT, thread_ctx, group, numCounters, maxActiveCounters,         \
      counterSize, counters)                                                   \
    X(void, glGetSamplerParameterIiv,                                          \
      (GLuint sampler, GLenum pname, GLint * params), VOID_DEFAULT,            \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glActiveShaderProgramEXT, (GLuint pipeline, GLuint program),       \
      VOID_DEFAULT, thread_ctx, pipeline, program)                             \
    X(void, glPrimitiveBoundingBox,                                            \
      (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX,   \
       GLfloat maxY, GLfloat maxZ, GLfloat maxW),                              \
      VOID_DEFAULT, thread_ctx, minX, minY, minZ, minW, maxX, maxY, maxZ,      \
      maxW)                                                                    \
    X(void, glLightModelf, (GLenum pname, GLfloat param), VOID_DEFAULT,        \
      thread_ctx, pname, param)                                                \
    X(void, glExtGetRenderbuffersQCOM,                                         \
      (GLuint * renderbuffers, GLint maxRenderbuffers,                         \
       GLint * numRenderbuffers),                                              \
      VOID_DEFAULT, thread_ctx, renderbuffers, maxRenderbuffers,               \
      numRenderbuffers)                                                        \
    X(void, glMatrixLoad3x3fNV, (GLenum matrixMode, const GLfloat *m),         \
      VOID_DEFAULT, thread_ctx, matrixMode, m)                                 \
    X(void, glSamplerParameterIuiv,                                            \
      (GLuint sampler, GLenum pname, const GLuint *param), VOID_DEFAULT,       \
      thread_ctx, sampler, pname, param)                                       \
    X(void, glMatrixLoadfEXT, (GLenum mode, const GLfloat *m), VOID_DEFAULT,   \
      thread_ctx, mode, m)                                                     \
    X(void, glVertexAttribDivisorANGLE, (GLuint index, GLuint divisor),        \
      VOID_DEFAULT, thread_ctx, index, divisor)                                \
    X(void, glDrawTexivOES, (const GLint *coords), VOID_DEFAULT, thread_ctx,   \
      coords)                                                                  \
    X(void, glLightModelxv, (GLenum pname, const GLfixed *param),              \
      VOID_DEFAULT, thread_ctx, pname, param)                                  \
    X(void, glScissorExclusiveArrayvNV,                                        \
      (GLuint first, GLsizei count, const GLint *v), VOID_DEFAULT, thread_ctx, \
      first, count, v)                                                         \
    X(void, glDeleteMemoryObjectsEXT,                                          \
      (GLsizei n, const GLuint *memoryObjects), VOID_DEFAULT, thread_ctx, n,   \
      memoryObjects)                                                           \
    X(void, glPushDebugGroup,                                                  \
      (GLenum source, GLuint id, GLsizei length, const GLchar *message),       \
      VOID_DEFAULT, thread_ctx, source, id, length, message)                   \
    X(void, glBlendEquationi, (GLuint buf, GLenum mode), VOID_DEFAULT,         \
      thread_ctx, buf, mode)                                                   \
    X(GLuint64, glGetImageHandleNV,                                            \
      (GLuint texture, GLint level, GLboolean layered, GLint layer,            \
       GLenum format),                                                         \
      RET_DEFAULT, thread_ctx, texture, level, layered, layer, format)         \
    X(void, glProgramUniform3ui64NV,                                           \
      (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y,           \
       GLuint64EXT z),                                                         \
      VOID_DEFAULT, thread_ctx, program, location, x, y, z)                    \
    X(void, glDeleteProgramPipelinesEXT, (GLsizei n, const GLuint *pipelines), \
      VOID_DEFAULT, thread_ctx, n, pipelines)                                  \
    X(void, glProgramUniform3i64vNV,                                           \
      (GLuint program, GLint location, GLsizei count,                          \
       const GLint64EXT *value),                                               \
      VOID_DEFAULT, thread_ctx, program, location, count, value)               \
    X(void, glViewportIndexedfNV,                                              \
      (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h),              \
      VOID_DEFAULT, thread_ctx, index, x, y, w, h)                             \
    X(void, glGetDriverControlsQCOM,                                           \
      (GLint * num, GLsizei size, GLuint * driverControls), VOID_DEFAULT,      \
      thread_ctx, num, size, driverControls)                                   \
    X(void, glGetQueryObjecti64vEXT,                                           \
      (GLuint id, GLenum pname, GLint64 * params), VOID_DEFAULT, thread_ctx,   \
      id, pname, params)                                                       \
    X(void, glMaterialf, (GLenum face, GLenum pname, GLfloat param),           \
      VOID_DEFAULT, thread_ctx, face, pname, param)                            \
    X(void, glBindFramebufferOES, (GLenum target, GLuint framebuffer),         \
      VOID_DEFAULT, thread_ctx, target, framebuffer)                           \
    X(GLuint, glGenPathsNV, (GLsizei range), RET_DEFAULT, thread_ctx, range)   \
    X(void, glFramebufferFetchBarrierEXT, (void), VOID_DEFAULT, thread_ctx)    \
    X(void, glExtGetBuffersQCOM,                                               \
      (GLuint * buffers, GLint maxBuffers, GLint * numBuffers), VOID_DEFAULT,  \
      thread_ctx, buffers, maxBuffers, numBuffers)                             \
    X(void, glGetMemoryObjectDetachedResourcesuivNV,                           \
      (GLuint memory, GLenum pname, GLint first, GLsizei count,                \
       GLuint * params),                                                       \
      VOID_DEFAULT, thread_ctx, memory, pname, first, count, params)           \
    X(void, glDrawTexfvOES, (const GLfloat *coords), VOID_DEFAULT, thread_ctx, \
      coords)                                                                  \
    X(GLenum, glPathGlyphIndexArrayNV,                                         \
      (GLuint firstPathName, GLenum fontTarget, const void *fontName,          \
       GLbitfield fontStyle, GLuint firstGlyphIndex, GLsizei numGlyphs,        \
       GLuint pathParameterTemplate, GLfloat emScale),                         \
      RET_DEFAULT, thread_ctx, firstPathName, fontTarget, fontName, fontStyle, \
      firstGlyphIndex, numGlyphs, pathParameterTemplate, emScale)              \
    X(void, glMatrixMultTransposefEXT, (GLenum mode, const GLfloat *m),        \
      VOID_DEFAULT, thread_ctx, mode, m)                                       \
    X(void, glLoadIdentity, (void), VOID_DEFAULT, thread_ctx)                  \
    X(void, glDepthRangeArrayfvOES,                                            \
      (GLuint first, GLsizei count, const GLfloat *v), VOID_DEFAULT,           \
      thread_ctx, first, count, v)                                             \
    X(void, glPopDebugGroup, (void), VOID_DEFAULT, thread_ctx)                 \
    X(void, glProgramUniformHandleui64IMG,                                     \
      (GLuint program, GLint location, GLuint64 value), VOID_DEFAULT,          \
      thread_ctx, program, location, value)                                    \
    X(void, glClearDepthx, (GLfixed depth), VOID_DEFAULT, thread_ctx, depth)   \
    X(void, glCompressedTexImage3DOES,                                         \
      (GLenum target, GLint level, GLenum internalformat, GLsizei width,       \
       GLsizei height, GLsizei depth, GLint border, GLsizei imageSize,         \
       const void *data),                                                      \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, width, height,  \
      depth, border, imageSize, data)                                          \
    X(void, glPopGroupMarkerEXT, (void), VOID_DEFAULT, thread_ctx)             \
    X(void, glMatrixOrthoEXT,                                                  \
      (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom,            \
       GLdouble top, GLdouble zNear, GLdouble zFar),                           \
      VOID_DEFAULT, thread_ctx, mode, left, right, bottom, top, zNear, zFar)   \
    X(void, glViewportIndexedfvOES, (GLuint index, const GLfloat *v),          \
      VOID_DEFAULT, thread_ctx, index, v)                                      \
    X(void, glMultiDrawElementsBaseVertexEXT,                                  \
      (GLenum mode, const GLsizei *count, GLenum type,                         \
       const void *const *indices, GLsizei primcount,                          \
       const GLint *basevertex),                                               \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, primcount,         \
      basevertex)                                                              \
    X(void, glDebugMessageInsert,                                              \
      (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, \
       const GLchar *buf),                                                     \
      VOID_DEFAULT, thread_ctx, source, type, id, severity, length, buf)       \
    X(void, glMatrixMultTranspose3x3fNV,                                       \
      (GLenum matrixMode, const GLfloat *m), VOID_DEFAULT, thread_ctx,         \
      matrixMode, m)                                                           \
    X(void, glColor4x,                                                         \
      (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glDrawElementsInstancedBaseVertexEXT,                              \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei instancecount, GLint basevertex),                               \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, instancecount,     \
      basevertex)                                                              \
    X(void, glFramebufferSampleLocationsfvNV,                                  \
      (GLenum target, GLuint start, GLsizei count, const GLfloat *v),          \
      VOID_DEFAULT, thread_ctx, target, start, count, v)                       \
    X(void, glGetObjectPtrLabel,                                               \
      (const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label),      \
      VOID_DEFAULT, thread_ctx, ptr, bufSize, length, label)                   \
    X(void, glGetSamplerParameterIivOES,                                       \
      (GLuint sampler, GLenum pname, GLint * params), VOID_DEFAULT,            \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glTexPageCommitmentEXT,                                            \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLboolean commit),                                                      \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset,      \
      width, height, depth, commit)                                            \
    X(void, glGetTexParameterIivEXT,                                           \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glGetSamplerParameterIuiv,                                         \
      (GLuint sampler, GLenum pname, GLuint * params), VOID_DEFAULT,           \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glGetPathMetricRangeNV,                                            \
      (GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths,     \
       GLsizei stride, GLfloat * metrics),                                     \
      VOID_DEFAULT, thread_ctx, metricQueryMask, firstPathName, numPaths,      \
      stride, metrics)                                                         \
    X(void, glBlendFunci, (GLuint buf, GLenum src, GLenum dst), VOID_DEFAULT,  \
      thread_ctx, buf, src, dst)                                               \
    X(void, glDrawElementsInstancedBaseVertex,                                 \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei instancecount, GLint basevertex),                               \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, instancecount,     \
      basevertex)                                                              \
    X(void, glCoverStrokePathNV, (GLuint path, GLenum coverMode),              \
      VOID_DEFAULT, thread_ctx, path, coverMode)                               \
    X(void, glDrawRangeElementsBaseVertexOES,                                  \
      (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,      \
       const void *indices, GLint basevertex),                                 \
      VOID_DEFAULT, thread_ctx, mode, start, end, count, type, indices,        \
      basevertex)                                                              \
    X(void, glDrawTexxvOES, (const GLfixed *coords), VOID_DEFAULT, thread_ctx, \
      coords)                                                                  \
    X(void, glTexturePageCommitmentMemNV,                                      \
      (GLuint texture, GLint layer, GLint level, GLint xoffset, GLint yoffset, \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLuint memory, GLuint64 offset, GLboolean commit),                      \
      VOID_DEFAULT, thread_ctx, texture, layer, level, xoffset, yoffset,       \
      zoffset, width, height, depth, memory, offset, commit)                   \
    X(void, glLightfv, (GLenum light, GLenum pname, const GLfloat *params),    \
      VOID_DEFAULT, thread_ctx, light, pname, params)                          \
    X(void, glBlendEquationiEXT, (GLuint buf, GLenum mode), VOID_DEFAULT,      \
      thread_ctx, buf, mode)                                                   \
    X(void, glDrawElementsInstancedBaseInstanceEXT,                            \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei instancecount, GLuint baseinstance),                            \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, instancecount,     \
      baseinstance)                                                            \
    X(void, glDrawElementsInstancedBaseVertexBaseInstanceEXT,                  \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei instancecount, GLint basevertex, GLuint baseinstance),          \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, instancecount,     \
      basevertex, baseinstance)                                                \
    X(void, glPointSizePointerOES,                                             \
      (GLenum type, GLsizei stride, const void *pointer), VOID_DEFAULT,        \
      thread_ctx, type, stride, pointer)                                       \
    X(void, glFogxv, (GLenum pname, const GLfixed *param), VOID_DEFAULT,       \
      thread_ctx, pname, param)                                                \
    X(void, glClientActiveTexture, (GLenum texture), VOID_DEFAULT, thread_ctx, \
      texture)                                                                 \
    X(void, glUniformMatrix4x3fvNV,                                            \
      (GLint location, GLsizei count, GLboolean transpose,                     \
       const GLfloat *value),                                                  \
      VOID_DEFAULT, thread_ctx, location, count, transpose, value)             \
    X(void, glMatrixMode, (GLenum mode), VOID_DEFAULT, thread_ctx, mode)       \
    X(void, glTexStorage3DEXT,                                                 \
      (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width,    \
       GLsizei height, GLsizei depth),                                         \
      VOID_DEFAULT, thread_ctx, target, levels, internalformat, width, height, \
      depth)                                                                   \
    X(void, glStencilStrokePathInstancedNV,                                    \
      (GLsizei numPaths, GLenum pathNameType, const void *paths,               \
       GLuint pathBase, GLint reference, GLuint mask, GLenum transformType,    \
       const GLfloat *transformValues),                                        \
      VOID_DEFAULT, thread_ctx, numPaths, pathNameType, paths, pathBase,       \
      reference, mask, transformType, transformValues)                         \
    X(void, glClipPlanex, (GLenum plane, const GLfixed *equation),             \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glGetInteger64vEXT, (GLenum pname, GLint64 * data), VOID_DEFAULT,  \
      thread_ctx, pname, data)                                                 \
    X(void, glDrawMeshTasksNV, (GLuint first, GLuint count), VOID_DEFAULT,     \
      thread_ctx, first, count)                                                \
    X(void, glGetInternalformatSampleivNV,                                     \
      (GLenum target, GLenum internalformat, GLsizei samples, GLenum pname,    \
       GLsizei count, GLint * params),                                         \
      VOID_DEFAULT, thread_ctx, target, internalformat, samples, pname, count, \
      params)                                                                  \
    X(void, glMultiTexCoord4f,                                                 \
      (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q),             \
      VOID_DEFAULT, thread_ctx, target, s, t, r, q)                            \
    X(void, glTexEstimateMotionRegionsQCOM,                                    \
      (GLuint ref, GLuint target, GLuint output, GLuint mask), VOID_DEFAULT,   \
      thread_ctx, ref, target, output, mask)                                   \
    X(void, glLightModelfv, (GLenum pname, const GLfloat *params),             \
      VOID_DEFAULT, thread_ctx, pname, params)                                 \
    X(void, glDebugMessageCallback,                                            \
      (GLDEBUGPROC callback, const void *userParam), VOID_DEFAULT, thread_ctx, \
      callback, userParam)                                                     \
    X(void, glFinishFenceNV, (GLuint fence), VOID_DEFAULT, thread_ctx, fence)  \
    X(void, glUniform1i64NV, (GLint location, GLint64EXT x), VOID_DEFAULT,     \
      thread_ctx, location, x)                                                 \
    X(void, glDeleteSemaphoresEXT, (GLsizei n, const GLuint *semaphores),      \
      VOID_DEFAULT, thread_ctx, n, semaphores)                                 \
    X(void, glGetSamplerParameterIivEXT,                                       \
      (GLuint sampler, GLenum pname, GLint * params), VOID_DEFAULT,            \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glTranslatef, (GLfloat x, GLfloat y, GLfloat z), VOID_DEFAULT,     \
      thread_ctx, x, y, z)                                                     \
    X(void, glTexAttachMemoryNV,                                               \
      (GLenum target, GLuint memory, GLuint64 offset), VOID_DEFAULT,           \
      thread_ctx, target, memory, offset)                                      \
    X(void, glGetFenceivNV, (GLuint fence, GLenum pname, GLint * params),      \
      VOID_DEFAULT, thread_ctx, fence, pname, params)                          \
    X(void, glBlendEquationSeparateOES, (GLenum modeRGB, GLenum modeAlpha),    \
      VOID_DEFAULT, thread_ctx, modeRGB, modeAlpha)                            \
    X(void, glGetLightfv, (GLenum light, GLenum pname, GLfloat * params),      \
      VOID_DEFAULT, thread_ctx, light, pname, params)                          \
    X(void, glPrimitiveBoundingBoxEXT,                                         \
      (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX,   \
       GLfloat maxY, GLfloat maxZ, GLfloat maxW),                              \
      VOID_DEFAULT, thread_ctx, minX, minY, minZ, minW, maxX, maxY, maxZ,      \
      maxW)                                                                    \
    X(void, glExtGetShadersQCOM,                                               \
      (GLuint * shaders, GLint maxShaders, GLint * numShaders), VOID_DEFAULT,  \
      thread_ctx, shaders, maxShaders, numShaders)                             \
    X(void, glLightf, (GLenum light, GLenum pname, GLfloat param),             \
      VOID_DEFAULT, thread_ctx, light, pname, param)                           \
    X(void, glClearColorx,                                                     \
      (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glFogf, (GLenum pname, GLfloat param), VOID_DEFAULT, thread_ctx,   \
      pname, param)                                                            \
    X(GLenum, glCheckFramebufferStatusOES, (GLenum target), RET_DEFAULT,       \
      thread_ctx, target)                                                      \
    X(void, glApplyFramebufferAttachmentCMAAINTEL, (void), VOID_DEFAULT,       \
      thread_ctx)                                                              \
    X(void, glGetPathSpacingNV,                                                \
      (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType,             \
       const void *paths, GLuint pathBase, GLfloat advanceScale,               \
       GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing),  \
      VOID_DEFAULT, thread_ctx, pathListMode, numPaths, pathNameType, paths,   \
      pathBase, advanceScale, kerningScale, transformType, returnedSpacing)    \
    X(void, glDrawElementsInstancedANGLE,                                      \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei primcount),                                                     \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, primcount)         \
    X(void, glDrawRangeElementsBaseVertex,                                     \
      (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,      \
       const void *indices, GLint basevertex),                                 \
      VOID_DEFAULT, thread_ctx, mode, start, end, count, type, indices,        \
      basevertex)                                                              \
    X(void, glFrustumf,                                                        \
      (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glLoadPaletteFromModelViewMatrixOES, (void), VOID_DEFAULT,         \
      thread_ctx)                                                              \
    X(void, glStencilThenCoverFillPathInstancedNV,                             \
      (GLsizei numPaths, GLenum pathNameType, const void *paths,               \
       GLuint pathBase, GLenum fillMode, GLuint mask, GLenum coverMode,        \
       GLenum transformType, const GLfloat *transformValues),                  \
      VOID_DEFAULT, thread_ctx, numPaths, pathNameType, paths, pathBase,       \
      fillMode, mask, coverMode, transformType, transformValues)               \
    X(void, glTexGenfOES, (GLenum coord, GLenum pname, GLfloat param),         \
      VOID_DEFAULT, thread_ctx, coord, pname, param)                           \
    X(void, glMultiDrawMeshTasksIndirectNV,                                    \
      (GLintptr indirect, GLsizei drawcount, GLsizei stride), VOID_DEFAULT,    \
      thread_ctx, indirect, drawcount, stride)                                 \
    X(void, glViewportIndexedfOES,                                             \
      (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h),              \
      VOID_DEFAULT, thread_ctx, index, x, y, w, h)                             \
    X(void, glTexPageCommitmentMemNV,                                          \
      (GLenum target, GLint layer, GLint level, GLint xoffset, GLint yoffset,  \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLuint memory, GLuint64 offset, GLboolean commit),                      \
      VOID_DEFAULT, thread_ctx, target, layer, level, xoffset, yoffset,        \
      zoffset, width, height, depth, memory, offset, commit)                   \
    X(void, glPrimitiveBoundingBoxOES,                                         \
      (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX,   \
       GLfloat maxY, GLfloat maxZ, GLfloat maxW),                              \
      VOID_DEFAULT, thread_ctx, minX, minY, minZ, minW, maxX, maxY, maxZ,      \
      maxW)                                                                    \
    X(void, glGetMaterialfv, (GLenum face, GLenum pname, GLfloat * params),    \
      VOID_DEFAULT, thread_ctx, face, pname, params)                           \
    X(void, glGetFramebufferParameterivMESA,                                   \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glMultMatrixf, (const GLfloat *m), VOID_DEFAULT, thread_ctx, m)    \
    X(GLboolean, glAcquireKeyedMutexWin32EXT,                                  \
      (GLuint memory, GLuint64 key, GLuint timeout), RET_DEFAULT, thread_ctx,  \
      memory, key, timeout)                                                    \
    X(void, glAlphaFunc, (GLenum func, GLfloat ref), VOID_DEFAULT, thread_ctx, \
      func, ref)                                                               \
    X(void, glProgramUniform4iEXT,                                             \
      (GLuint program, GLint location, GLint v0, GLint v1, GLint v2,           \
       GLint v3),                                                              \
      VOID_DEFAULT, thread_ctx, program, location, v0, v1, v2, v3)             \
    X(void, glLightx, (GLenum light, GLenum pname, GLfixed param),             \
      VOID_DEFAULT, thread_ctx, light, pname, param)                           \
    X(void, glExtrapolateTex2DQCOM,                                            \
      (GLuint src1, GLuint src2, GLuint output, GLfloat scaleFactor),          \
      VOID_DEFAULT, thread_ctx, src1, src2, output, scaleFactor)               \
    X(void, glGetClipPlanef, (GLenum plane, GLfloat * equation), VOID_DEFAULT, \
      thread_ctx, plane, equation)                                             \
    X(void, glStencilFillPathInstancedNV,                                      \
      (GLsizei numPaths, GLenum pathNameType, const void *paths,               \
       GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType,    \
       const GLfloat *transformValues),                                        \
      VOID_DEFAULT, thread_ctx, numPaths, pathNameType, paths, pathBase,       \
      fillMode, mask, transformType, transformValues)                          \
    X(void, glNamedBufferStorageMemEXT,                                        \
      (GLuint buffer, GLsizeiptr size, GLuint memory, GLuint64 offset),        \
      VOID_DEFAULT, thread_ctx, buffer, size, memory, offset)                  \
    X(void, glTextureStorage3DEXT,                                             \
      (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat,   \
       GLsizei width, GLsizei height, GLsizei depth),                          \
      VOID_DEFAULT, thread_ctx, texture, target, levels, internalformat,       \
      width, height, depth)                                                    \
    X(void, glLightModelx, (GLenum pname, GLfixed param), VOID_DEFAULT,        \
      thread_ctx, pname, param)                                                \
    X(void, glDeleteRenderbuffersOES,                                          \
      (GLsizei n, const GLuint *renderbuffers), VOID_DEFAULT, thread_ctx, n,   \
      renderbuffers)                                                           \
    X(void, glClipPlanefIMG, (GLenum p, const GLfloat *eqn), VOID_DEFAULT,     \
      thread_ctx, p, eqn)                                                      \
    X(void, glImportSemaphoreFdEXT,                                            \
      (GLuint semaphore, GLenum handleType, GLint fd), VOID_DEFAULT,           \
      thread_ctx, semaphore, handleType, fd)                                   \
    X(void, glGetnUniformfv,                                                   \
      (GLuint program, GLint location, GLsizei bufSize, GLfloat * params),     \
      VOID_DEFAULT, thread_ctx, program, location, bufSize, params)            \
    X(void, glTexParameterxv,                                                  \
      (GLenum target, GLenum pname, const GLfixed *params), VOID_DEFAULT,      \
      thread_ctx, target, pname, params)                                       \
    X(GLenum, glGetGraphicsResetStatus, (void), RET_DEFAULT, thread_ctx)       \
    X(void, glScalex, (GLfixed x, GLfixed y, GLfixed z), VOID_DEFAULT,         \
      thread_ctx, x, y, z)                                                     \
    X(void, glGetTexEnviv, (GLenum target, GLenum pname, GLint * params),      \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glBlendBarrier, (void), VOID_DEFAULT, thread_ctx)                  \
    X(void, glPointParameterfv, (GLenum pname, const GLfloat *params),         \
      VOID_DEFAULT, thread_ctx, pname, params)

#define LIST_GL_EXT_FUNCTIONS(X)                                               \
    X(void, glClearDepthfOES, (GLclampf depth), VOID_DEFAULT, thread_ctx,      \
      depth)                                                                   \
    X(void, glGetTexEnvxvOES, (GLenum target, GLenum pname, GLfixed * params), \
      VOID_DEFAULT, thread_ctx, target, pname, params)                         \
    X(void, glGenVertexArraysOES, (GLsizei n, GLuint * arrays), VOID_DEFAULT,  \
      thread_ctx, n, arrays)                                                   \
    X(void, glClearDepthxOES, (GLfixed depth), VOID_DEFAULT, thread_ctx,       \
      depth)                                                                   \
    X(void, glClipPlanexOES, (GLenum plane, const GLfixed *equation),          \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glRotatexOES, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z),    \
      VOID_DEFAULT, thread_ctx, angle, x, y, z)                                \
    X(void, glMultMatrixxOES, (const GLfixed *m), VOID_DEFAULT, thread_ctx, m) \
    X(void, glTexEnvxvOES,                                                     \
      (GLenum target, GLenum pname, const GLfixed *params), VOID_DEFAULT,      \
      thread_ctx, target, pname, params)                                       \
    X(void, glLightxOES, (GLenum light, GLenum pname, GLfixed param),          \
      VOID_DEFAULT, thread_ctx, light, pname, param)                           \
    X(void, glLoadMatrixxOES, (const GLfixed *m), VOID_DEFAULT, thread_ctx, m) \
    X(void, glClipPlanefOES, (GLenum plane, const GLfloat *equation),          \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glDeleteVertexArraysOES, (GLsizei n, const GLuint *arrays),        \
      VOID_DEFAULT, thread_ctx, n, arrays)                                     \
    X(void, glTexGenxvOES,                                                     \
      (GLenum coord, GLenum pname, const GLfixed *params), VOID_DEFAULT,       \
      thread_ctx, coord, pname, params)                                        \
    X(void, glPointSizexOES, (GLfixed size), VOID_DEFAULT, thread_ctx, size)   \
    X(void, glMaterialxOES, (GLenum face, GLenum pname, GLfixed param),        \
      VOID_DEFAULT, thread_ctx, face, pname, param)                            \
    X(void, glTexEnvxOES, (GLenum target, GLenum pname, GLfixed param),        \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glPointParameterxvOES, (GLenum pname, const GLfixed *params),      \
      VOID_DEFAULT, thread_ctx, pname, params)                                 \
    X(void, glLightModelxOES, (GLenum pname, GLfixed param), VOID_DEFAULT,     \
      thread_ctx, pname, param)                                                \
    X(void, glGetFixedvOES, (GLenum pname, GLfixed * params), VOID_DEFAULT,    \
      thread_ctx, pname, params)                                               \
    X(void, glFrustumfOES,                                                     \
      (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glFrustumxOES,                                                     \
      (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glFogxOES, (GLenum pname, GLfixed param), VOID_DEFAULT,            \
      thread_ctx, pname, param)                                                \
    X(void, glDrawTexiOES,                                                     \
      (GLint x, GLint y, GLint z, GLint width, GLint height), VOID_DEFAULT,    \
      thread_ctx, x, y, z, width, height)                                      \
    X(void, glScalexOES, (GLfixed x, GLfixed y, GLfixed z), VOID_DEFAULT,      \
      thread_ctx, x, y, z)                                                     \
    X(void, glTexGenxOES, (GLenum coord, GLenum pname, GLfixed param),         \
      VOID_DEFAULT, thread_ctx, coord, pname, param)                           \
    X(void, glBindVertexArrayOES, (GLuint array), VOID_DEFAULT, thread_ctx,    \
      array)                                                                   \
    X(void, glLightxvOES, (GLenum light, GLenum pname, const GLfixed *params), \
      VOID_DEFAULT, thread_ctx, light, pname, params)                          \
    X(void, glGetTexParameterxvOES,                                            \
      (GLenum target, GLenum pname, GLfixed * params), VOID_DEFAULT,           \
      thread_ctx, target, pname, params)                                       \
    X(void, glPointParameterxOES, (GLenum pname, GLfixed param), VOID_DEFAULT, \
      thread_ctx, pname, param)                                                \
    X(void, glTranslatexOES, (GLfixed x, GLfixed y, GLfixed z), VOID_DEFAULT,  \
      thread_ctx, x, y, z)                                                     \
    X(void, glGetMaterialxvOES, (GLenum face, GLenum pname, GLfixed * params), \
      VOID_DEFAULT, thread_ctx, face, pname, params)                           \
    X(void, glFogxvOES, (GLenum pname, const GLfixed *param), VOID_DEFAULT,    \
      thread_ctx, pname, param)                                                \
    X(void, glClearColorxOES,                                                  \
      (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glGetClipPlanexOES, (GLenum plane, GLfixed * equation),            \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glPolygonOffsetxOES, (GLfixed factor, GLfixed units),              \
      VOID_DEFAULT, thread_ctx, factor, units)                                 \
    X(void, glGetTexGenxvOES, (GLenum coord, GLenum pname, GLfixed * params),  \
      VOID_DEFAULT, thread_ctx, coord, pname, params)                          \
    X(void, glDepthRangexOES, (GLfixed n, GLfixed f), VOID_DEFAULT,            \
      thread_ctx, n, f)                                                        \
    X(void, glLightModelxvOES, (GLenum pname, const GLfixed *param),           \
      VOID_DEFAULT, thread_ctx, pname, param)                                  \
    X(void, glMultiTexCoord4xOES,                                              \
      (GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q),            \
      VOID_DEFAULT, thread_ctx, texture, s, t, r, q)                           \
    X(void, glMaterialxvOES,                                                   \
      (GLenum face, GLenum pname, const GLfixed *param), VOID_DEFAULT,         \
      thread_ctx, face, pname, param)                                          \
    X(void, glDepthRangefOES, (GLclampf n, GLclampf f), VOID_DEFAULT,          \
      thread_ctx, n, f)                                                        \
    X(GLbitfield, glQueryMatrixxOES, (GLfixed * mantissa, GLint * exponent),   \
      RET_DEFAULT, thread_ctx, mantissa, exponent)                             \
    X(void, glOrthoxOES,                                                       \
      (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glLineWidthxOES, (GLfixed width), VOID_DEFAULT, thread_ctx, width) \
    X(void, glNormal3xOES, (GLfixed nx, GLfixed ny, GLfixed nz), VOID_DEFAULT, \
      thread_ctx, nx, ny, nz)                                                  \
    X(void, glAlphaFuncxOES, (GLenum func, GLfixed ref), VOID_DEFAULT,         \
      thread_ctx, func, ref)                                                   \
    X(void, glGetLightxvOES, (GLenum light, GLenum pname, GLfixed * params),   \
      VOID_DEFAULT, thread_ctx, light, pname, params)                          \
    X(void, glOrthofOES,                                                       \
      (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f),      \
      VOID_DEFAULT, thread_ctx, l, r, b, t, n, f)                              \
    X(void, glEGLImageTargetTexture2DOES,                                      \
      (GLenum target, GLeglImageOES image), VOID_DEFAULT, thread_ctx, target,  \
      image)                                                                   \
    X(void, glColor4xOES,                                                      \
      (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha), VOID_DEFAULT, \
      thread_ctx, red, green, blue, alpha)                                     \
    X(void, glSampleCoveragexOES, (GLclampx value, GLboolean invert),          \
      VOID_DEFAULT, thread_ctx, value, invert)                                 \
    X(void, glBlendEquationSeparateiOES,                                       \
      (GLuint buf, GLenum modeRGB, GLenum modeAlpha), VOID_DEFAULT,            \
      thread_ctx, buf, modeRGB, modeAlpha)                                     \
    X(void, glTexImage3DOES,                                                   \
      (GLenum target, GLint level, GLenum internalformat, GLsizei width,       \
       GLsizei height, GLsizei depth, GLint border, GLenum format,             \
       GLenum type, const void *pixels),                                       \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, width, height,  \
      depth, border, format, type, pixels)                                     \
    X(void, glPatchParameteriOES, (GLenum pname, GLint value), VOID_DEFAULT,   \
      thread_ctx, pname, value)                                                \
    X(void, glGetClipPlanefOES, (GLenum plane, GLfloat * equation),            \
      VOID_DEFAULT, thread_ctx, plane, equation)                               \
    X(void, glProgramBinaryOES,                                                \
      (GLuint program, GLenum binaryFormat, const void *binary, GLint length), \
      VOID_DEFAULT, thread_ctx, program, binaryFormat, binary, length)         \
    X(void, glGenFramebuffersOES, (GLsizei n, GLuint * framebuffers),          \
      VOID_DEFAULT, thread_ctx, n, framebuffers)                               \
    X(GLboolean, glUnmapBufferOES, (GLenum target), RET_DEFAULT, thread_ctx,   \
      target)                                                                  \
    X(void, glTexSubImage3DOES,                                                \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLenum format, GLenum type, const void *pixels),                        \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset,      \
      width, height, depth, format, type, pixels)                              \
    X(void, glDeleteFramebuffersOES, (GLsizei n, const GLuint *framebuffers),  \
      VOID_DEFAULT, thread_ctx, n, framebuffers)                               \
    X(void, glFramebufferTexture3DOES,                                         \
      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture,     \
       GLint level, GLint zoffset),                                            \
      VOID_DEFAULT, thread_ctx, target, attachment, textarget, texture, level, \
      zoffset)                                                                 \
    X(void, glTexGenivOES, (GLenum coord, GLenum pname, const GLint *params),  \
      VOID_DEFAULT, thread_ctx, coord, pname, params)                          \
    X(void, glEGLImageTargetRenderbufferStorageOES,                            \
      (GLenum target, GLeglImageOES image), VOID_DEFAULT, thread_ctx, target,  \
      image)                                                                   \
    X(void, glWeightPointerOES,                                                \
      (GLint size, GLenum type, GLsizei stride, const void *pointer),          \
      VOID_DEFAULT, thread_ctx, size, type, stride, pointer)                   \
    X(void, glGetFramebufferAttachmentParameterivOES,                          \
      (GLenum target, GLenum attachment, GLenum pname, GLint * params),        \
      VOID_DEFAULT, thread_ctx, target, attachment, pname, params)             \
    X(void, glFramebufferTexture2DOES,                                         \
      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture,     \
       GLint level),                                                           \
      VOID_DEFAULT, thread_ctx, target, attachment, textarget, texture, level) \
    X(void, glFramebufferTextureOES,                                           \
      (GLenum target, GLenum attachment, GLuint texture, GLint level),         \
      VOID_DEFAULT, thread_ctx, target, attachment, texture, level)            \
    X(void, glDrawElementsInstancedBaseVertexOES,                              \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLsizei instancecount, GLint basevertex),                               \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, instancecount,     \
      basevertex)                                                              \
    X(void, glBlendFunciOES, (GLuint buf, GLenum src, GLenum dst),             \
      VOID_DEFAULT, thread_ctx, buf, src, dst)                                 \
    X(void, glDisableiOES, (GLenum target, GLuint index), VOID_DEFAULT,        \
      thread_ctx, target, index)                                               \
    X(void, glCopyImageSubDataOES,                                             \
      (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX,           \
       GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget,               \
       GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth,   \
       GLsizei srcHeight, GLsizei srcDepth),                                   \
      VOID_DEFAULT, thread_ctx, srcName, srcTarget, srcLevel, srcX, srcY,      \
      srcZ, dstName, dstTarget, dstLevel, dstX, dstY, dstZ, srcWidth,          \
      srcHeight, srcDepth)                                                     \
    X(void, glBlendEquationiOES, (GLuint buf, GLenum mode), VOID_DEFAULT,      \
      thread_ctx, buf, mode)                                                   \
    X(GLboolean, glIsEnablediOES, (GLenum target, GLuint index), RET_DEFAULT,  \
      thread_ctx, target, index)                                               \
    X(void, glTextureViewOES,                                                  \
      (GLuint texture, GLenum target, GLuint origtexture,                      \
       GLenum internalformat, GLuint minlevel, GLuint numlevels,               \
       GLuint minlayer, GLuint numlayers),                                     \
      VOID_DEFAULT, thread_ctx, texture, target, origtexture, internalformat,  \
      minlevel, numlevels, minlayer, numlayers)                                \
    X(void, glCompressedTexSubImage3DOES,                                      \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLsizei width, GLsizei height, GLsizei depth,            \
       GLenum format, GLsizei imageSize, const void *data),                    \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset,      \
      width, height, depth, format, imageSize, data)                           \
    X(void, glGetBufferPointervOES,                                            \
      (GLenum target, GLenum pname, void **params), VOID_DEFAULT, thread_ctx,  \
      target, pname, params)                                                   \
    X(void, glGetTexParameterIivOES,                                           \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(GLboolean, glIsRenderbufferOES, (GLuint renderbuffer), RET_DEFAULT,      \
      thread_ctx, renderbuffer)                                                \
    X(void, glTexBufferRangeOES,                                               \
      (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset,   \
       GLsizeiptr size),                                                       \
      VOID_DEFAULT, thread_ctx, target, internalformat, buffer, offset, size)  \
    X(void, glBlendFuncSeparateiOES,                                           \
      (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha,              \
       GLenum dstAlpha),                                                       \
      VOID_DEFAULT, thread_ctx, buf, srcRGB, dstRGB, srcAlpha, dstAlpha)       \
    X(void, glBlendFuncSeparateOES,                                            \
      (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha),        \
      VOID_DEFAULT, thread_ctx, srcRGB, dstRGB, srcAlpha, dstAlpha)            \
    X(void, glGetProgramBinaryOES,                                             \
      (GLuint program, GLsizei bufSize, GLsizei * length,                      \
       GLenum * binaryFormat, void *binary),                                   \
      VOID_DEFAULT, thread_ctx, program, bufSize, length, binaryFormat,        \
      binary)                                                                  \
    X(void, glGetSamplerParameterIuivOES,                                      \
      (GLuint sampler, GLenum pname, GLuint * params), VOID_DEFAULT,           \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glDrawTexsOES,                                                     \
      (GLshort x, GLshort y, GLshort z, GLshort width, GLshort height),        \
      VOID_DEFAULT, thread_ctx, x, y, z, width, height)                        \
    X(void, glTexGenfvOES,                                                     \
      (GLenum coord, GLenum pname, const GLfloat *params), VOID_DEFAULT,       \
      thread_ctx, coord, pname, params)                                        \
    X(void, glScissorArrayvOES, (GLuint first, GLsizei count, const GLint *v), \
      VOID_DEFAULT, thread_ctx, first, count, v)                               \
    X(void, glGetRenderbufferParameterivOES,                                   \
      (GLenum target, GLenum pname, GLint * params), VOID_DEFAULT, thread_ctx, \
      target, pname, params)                                                   \
    X(void, glDrawElementsBaseVertexOES,                                       \
      (GLenum mode, GLsizei count, GLenum type, const void *indices,           \
       GLint basevertex),                                                      \
      VOID_DEFAULT, thread_ctx, mode, count, type, indices, basevertex)        \
    X(void, glTexStorage3DMultisampleOES,                                      \
      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width,   \
       GLsizei height, GLsizei depth, GLboolean fixedsamplelocations),         \
      VOID_DEFAULT, thread_ctx, target, samples, internalformat, width,        \
      height, depth, fixedsamplelocations)                                     \
    X(void, glTexParameterxvOES,                                               \
      (GLenum target, GLenum pname, const GLfixed *params), VOID_DEFAULT,      \
      thread_ctx, target, pname, params)                                       \
    X(void, glScissorIndexedOES,                                               \
      (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height), \
      VOID_DEFAULT, thread_ctx, index, left, bottom, width, height)            \
    X(void, glRenderbufferStorageOES,                                          \
      (GLenum target, GLenum internalformat, GLsizei width, GLsizei height),   \
      VOID_DEFAULT, thread_ctx, target, internalformat, width, height)         \
    X(void, glScissorIndexedvOES, (GLuint index, const GLint *v),              \
      VOID_DEFAULT, thread_ctx, index, v)                                      \
    X(void, glDrawTexsvOES, (const GLshort *coords), VOID_DEFAULT, thread_ctx, \
      coords)                                                                  \
    X(void, glSamplerParameterIuivOES,                                         \
      (GLuint sampler, GLenum pname, const GLuint *param), VOID_DEFAULT,       \
      thread_ctx, sampler, pname, param)                                       \
    X(void, glViewportArrayvOES,                                               \
      (GLuint first, GLsizei count, const GLfloat *v), VOID_DEFAULT,           \
      thread_ctx, first, count, v)                                             \
    X(void, glGetTexGenfvOES, (GLenum coord, GLenum pname, GLfloat * params),  \
      VOID_DEFAULT, thread_ctx, coord, pname, params)                          \
    X(void, glTexGeniOES, (GLenum coord, GLenum pname, GLint param),           \
      VOID_DEFAULT, thread_ctx, coord, pname, param)                           \
    X(void, glTexParameterIuivOES,                                             \
      (GLenum target, GLenum pname, const GLuint *params), VOID_DEFAULT,       \
      thread_ctx, target, pname, params)                                       \
    X(void, glEnableiOES, (GLenum target, GLuint index), VOID_DEFAULT,         \
      thread_ctx, target, index)                                               \
    X(void, glMinSampleShadingOES, (GLfloat value), VOID_DEFAULT, thread_ctx,  \
      value)                                                                   \
    X(void, glFramebufferRenderbufferOES,                                      \
      (GLenum target, GLenum attachment, GLenum renderbuffertarget,            \
       GLuint renderbuffer),                                                   \
      VOID_DEFAULT, thread_ctx, target, attachment, renderbuffertarget,        \
      renderbuffer)                                                            \
    X(void, glDepthRangeIndexedfOES, (GLuint index, GLfloat n, GLfloat f),     \
      VOID_DEFAULT, thread_ctx, index, n, f)                                   \
    X(void, glTexParameterxOES, (GLenum target, GLenum pname, GLfixed param),  \
      VOID_DEFAULT, thread_ctx, target, pname, param)                          \
    X(void, glGetTexParameterIuivOES,                                          \
      (GLenum target, GLenum pname, GLuint * params), VOID_DEFAULT,            \
      thread_ctx, target, pname, params)                                       \
    X(void, glBindRenderbufferOES, (GLenum target, GLuint renderbuffer),       \
      VOID_DEFAULT, thread_ctx, target, renderbuffer)                          \
    X(void, glGetFloati_vOES, (GLenum target, GLuint index, GLfloat * data),   \
      VOID_DEFAULT, thread_ctx, target, index, data)                           \
    X(void, glDrawTexfOES,                                                     \
      (GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height),        \
      VOID_DEFAULT, thread_ctx, x, y, z, width, height)                        \
    X(void, glCopyTexSubImage3DOES,                                            \
      (GLenum target, GLint level, GLint xoffset, GLint yoffset,               \
       GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height),        \
      VOID_DEFAULT, thread_ctx, target, level, xoffset, yoffset, zoffset, x,   \
      y, width, height)                                                        \
    X(void, glBlendEquationOES, (GLenum mode), VOID_DEFAULT, thread_ctx, mode) \
    X(GLboolean, glIsFramebufferOES, (GLuint framebuffer), RET_DEFAULT,        \
      thread_ctx, framebuffer)                                                 \
    X(void, glDrawTexxOES,                                                     \
      (GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height),        \
      VOID_DEFAULT, thread_ctx, x, y, z, width, height)                        \
    X(void, glSamplerParameterIivOES,                                          \
      (GLuint sampler, GLenum pname, const GLint *param), VOID_DEFAULT,        \
      thread_ctx, sampler, pname, param)                                       \
    X(GLboolean, glIsVertexArrayOES, (GLuint array), RET_DEFAULT, thread_ctx,  \
      array)                                                                   \
    X(void, glGenRenderbuffersOES, (GLsizei n, GLuint * renderbuffers),        \
      VOID_DEFAULT, thread_ctx, n, renderbuffers)                              \
    X(void, glColorMaskiOES,                                                   \
      (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a),      \
      VOID_DEFAULT, thread_ctx, index, r, g, b, a)                             \
    X(void *, glMapBufferOES, (GLenum target, GLenum access), RET_DEFAULT,     \
      thread_ctx, target, access)                                              \
    X(void, glCurrentPaletteMatrixOES, (GLuint matrixpaletteindex),            \
      VOID_DEFAULT, thread_ctx, matrixpaletteindex)                            \
    X(void, glGetTexGenivOES, (GLenum coord, GLenum pname, GLint * params),    \
      VOID_DEFAULT, thread_ctx, coord, pname, params)                          \
    X(void, glMatrixIndexPointerOES,                                           \
      (GLint size, GLenum type, GLsizei stride, const void *pointer),          \
      VOID_DEFAULT, thread_ctx, size, type, stride, pointer)                   \
    X(void, glGenerateMipmapOES, (GLenum target), VOID_DEFAULT, thread_ctx,    \
      target)                                                                  \
    X(void, glTexParameterIivOES,                                              \
      (GLenum target, GLenum pname, const GLint *params), VOID_DEFAULT,        \
      thread_ctx, target, pname, params)                                       \
    X(void, glDrawTexivOES, (const GLint *coords), VOID_DEFAULT, thread_ctx,   \
      coords)                                                                  \
    X(void, glBindFramebufferOES, (GLenum target, GLuint framebuffer),         \
      VOID_DEFAULT, thread_ctx, target, framebuffer)                           \
    X(void, glDrawTexfvOES, (const GLfloat *coords), VOID_DEFAULT, thread_ctx, \
      coords)                                                                  \
    X(void, glDepthRangeArrayfvOES,                                            \
      (GLuint first, GLsizei count, const GLfloat *v), VOID_DEFAULT,           \
      thread_ctx, first, count, v)                                             \
    X(void, glCompressedTexImage3DOES,                                         \
      (GLenum target, GLint level, GLenum internalformat, GLsizei width,       \
       GLsizei height, GLsizei depth, GLint border, GLsizei imageSize,         \
       const void *data),                                                      \
      VOID_DEFAULT, thread_ctx, target, level, internalformat, width, height,  \
      depth, border, imageSize, data)                                          \
    X(void, glViewportIndexedfvOES, (GLuint index, const GLfloat *v),          \
      VOID_DEFAULT, thread_ctx, index, v)                                      \
    X(void, glGetSamplerParameterIivOES,                                       \
      (GLuint sampler, GLenum pname, GLint * params), VOID_DEFAULT,            \
      thread_ctx, sampler, pname, params)                                      \
    X(void, glDrawRangeElementsBaseVertexOES,                                  \
      (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type,      \
       const void *indices, GLint basevertex),                                 \
      VOID_DEFAULT, thread_ctx, mode, start, end, count, type, indices,        \
      basevertex)                                                              \
    X(void, glDrawTexxvOES, (const GLfixed *coords), VOID_DEFAULT, thread_ctx, \
      coords)                                                                  \
    X(void, glPointSizePointerOES,                                             \
      (GLenum type, GLsizei stride, const void *pointer), VOID_DEFAULT,        \
      thread_ctx, type, stride, pointer)                                       \
    X(void, glTexBufferOES,                                                    \
      (GLenum target, GLenum internalformat, GLuint buffer), VOID_DEFAULT,     \
      thread_ctx, target, internalformat, buffer)                              \
    X(void, glBlendEquationSeparateOES, (GLenum modeRGB, GLenum modeAlpha),    \
      VOID_DEFAULT, thread_ctx, modeRGB, modeAlpha)                            \
    X(GLenum, glCheckFramebufferStatusOES, (GLenum target), RET_DEFAULT,       \
      thread_ctx, target)                                                      \
    X(void, glLoadPaletteFromModelViewMatrixOES, (void), VOID_DEFAULT,         \
      thread_ctx)                                                              \
    X(void, glTexGenfOES, (GLenum coord, GLenum pname, GLfloat param),         \
      VOID_DEFAULT, thread_ctx, coord, pname, param)                           \
    X(void, glViewportIndexedfOES,                                             \
      (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h),              \
      VOID_DEFAULT, thread_ctx, index, x, y, w, h)                             \
    X(void, glPrimitiveBoundingBoxOES,                                         \
      (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX,   \
       GLfloat maxY, GLfloat maxZ, GLfloat maxW),                              \
      VOID_DEFAULT, thread_ctx, minX, minY, minZ, minW, maxX, maxY, maxZ,      \
      maxW)                                                                    \
    X(void, glDeleteRenderbuffersOES,                                          \
      (GLsizei n, const GLuint *renderbuffers), VOID_DEFAULT, thread_ctx, n,   \
      renderbuffers)

#endif // GL_FUNCTIONS_H