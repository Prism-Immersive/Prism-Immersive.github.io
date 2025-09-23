
#ifndef GLDEF_H
#define GLDEF_H

// android
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "GLES/glext.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

#define MAX_OUT_BUF_LEN 4096

GLenum r_glClientWaitSync(void *context, GLsync sync, GLbitfield flags,
                          GLuint64 timeout);
GLenum d_glClientWaitSync(void *context, GLsync sync, GLbitfield flags,
                          GLuint64 timeout);

GLint r_glTestInt1(void *context, GLint a, GLuint b);
GLint d_glTestInt1(void *context, GLint a, GLuint b);

GLuint r_glTestInt2(void *context, GLint a, GLuint b);
GLuint d_glTestInt2(void *context, GLint a, GLuint b);

GLint64 r_glTestInt3(void *context, GLint64 a, GLuint64 b);
GLint64 d_glTestInt3(void *context, GLint64 a, GLuint64 b);

GLuint64 r_glTestInt4(void *context, GLint64 a, GLuint64 b);
GLuint64 d_glTestInt4(void *context, GLint64 a, GLuint64 b);

GLfloat r_glTestInt5(void *context, GLint a, GLuint b);
GLfloat d_glTestInt5(void *context, GLint a, GLuint b);

GLdouble r_glTestInt6(void *context, GLint a, GLuint b);
GLdouble d_glTestInt6(void *context, GLint a, GLuint b);

void r_glTestPointer1(void *context, GLint a, const GLint *b);
void d_glTestPointer1(void *context, GLint a, const GLint *b);

void r_glTestPointer2(void *context, GLint a, const GLint *b, GLint *c);
void d_glTestPointer2(void *context, GLint a, const GLint *b, GLint *c);

GLint r_glTestPointer4(void *context, GLint a, const GLint *b, GLint *c);
GLint d_glTestPointer4(void *context, GLint a, const GLint *b, GLint *c);

void r_glTestString(void *context, GLint a, GLint count,
                    const GLchar *const *strings, GLint buf_len,
                    GLchar *char_buf);
void d_glTestString(void *context, GLint a, GLint count,
                    const GLchar *const *strings, GLint buf_len,
                    GLchar *char_buf);

// GLboolean r_glIsBuffer(void *context, GLuint buffer);
GLboolean d_glIsBuffer(void *context, GLuint buffer);
GLboolean d_glIsProgramPipeline(void *context, GLuint pipeline);

GLboolean r_glIsEnabled(void *context, GLenum cap);
GLboolean d_glIsEnabled(void *context, GLenum cap);

// GLboolean r_glIsFramebuffer(void *context, GLuint framebuffer);
GLboolean d_glIsFramebuffer(void *context, GLuint framebuffer);

// GLboolean r_glIsProgram(void *context, GLuint program);
GLboolean d_glIsProgram(void *context, GLuint program);

// GLboolean r_glIsRenderbuffer(void *context, GLuint renderbuffer);
GLboolean d_glIsRenderbuffer(void *context, GLuint renderbuffer);

// GLboolean r_glIsShader(void *context, GLuint shader);
GLboolean d_glIsShader(void *context, GLuint shader);

// GLboolean r_glIsTexture(void *context, GLuint texture);
GLboolean d_glIsTexture(void *context, GLuint texture);

// GLboolean r_glIsQuery(void *context, GLuint id);
GLboolean d_glIsQuery(void *context, GLuint id);

// GLboolean r_glIsVertexArray(void *context, GLuint array);
GLboolean d_glIsVertexArray(void *context, GLuint array);

// GLboolean r_glIsSampler(void *context, GLuint sampler);
GLboolean d_glIsSampler(void *context, GLuint sampler);

// GLboolean r_glIsTransformFeedback(void *context, GLuint id);
GLboolean d_glIsTransformFeedback(void *context, GLuint id);

// GLboolean r_glIsSync(void *context, GLsync sync);
GLboolean d_glIsSync(void *context, GLsync sync);

GLenum r_glGetError(void *context);
GLenum d_glGetError(void *context);

void r_glGetString_special(void *context, GLenum name, GLubyte *buffer);
const GLubyte *d_glGetString(void *context, GLenum name);

void r_glGetStringi_special(void *context, GLenum name, GLuint index,
                            GLubyte *buffer);
const GLubyte *d_glGetStringi(void *context, GLenum name, GLuint index);

GLenum r_glCheckFramebufferStatus(void *context, GLenum target);
GLenum d_glCheckFramebufferStatus(void *context, GLenum target);

GLbitfield r_glQueryMatrixxOES(void *context, GLfixed *mantissa,
                               GLint *exponent);
GLbitfield d_glQueryMatrixxOES(void *context, GLfixed *mantissa,
                               GLint *exponent);

void r_glGetFramebufferAttachmentParameteriv(void *context, GLenum target,
                                             GLenum attachment, GLenum pname,
                                             GLint *params);
void d_glGetFramebufferAttachmentParameteriv(void *context, GLenum target,
                                             GLenum attachment, GLenum pname,
                                             GLint *params);

void r_glGetProgramInfoLog(void *context, GLuint program, GLsizei bufSize,
                           GLsizei *length, GLchar *infoLog);
void d_glGetProgramInfoLog(void *context, GLuint program, GLsizei bufSize,
                           GLsizei *length, GLchar *infoLog);

void r_glGetRenderbufferParameteriv(void *context, GLenum target, GLenum pname,
                                    GLint *params);
void d_glGetRenderbufferParameteriv(void *context, GLenum target, GLenum pname,
                                    GLint *params);

void r_glGetShaderInfoLog(void *context, GLuint shader, GLsizei bufSize,
                          GLsizei *length, GLchar *infoLog);
void d_glGetShaderInfoLog(void *context, GLuint shader, GLsizei bufSize,
                          GLsizei *length, GLchar *infoLog);

void r_glGetShaderPrecisionFormat(void *context, GLenum shadertype,
                                  GLenum precisiontype, GLint *range,
                                  GLint *precision);
void d_glGetShaderPrecisionFormat(void *context, GLenum shadertype,
                                  GLenum precisiontype, GLint *range,
                                  GLint *precision);

void r_glGetShaderSource(void *context, GLuint shader, GLsizei bufSize,
                         GLsizei *length, GLchar *source);
void d_glGetShaderSource(void *context, GLuint shader, GLsizei bufSize,
                         GLsizei *length, GLchar *source);

void r_glGetTexParameterfv(void *context, GLenum target, GLenum pname,
                           GLfloat *params);
void d_glGetTexParameterfv(void *context, GLenum target, GLenum pname,
                           GLfloat *params);

void r_glGetTexParameteriv(void *context, GLenum target, GLenum pname,
                           GLint *params);
void d_glGetTexParameteriv(void *context, GLenum target, GLenum pname,
                           GLint *params);

void r_glGetQueryiv(void *context, GLenum target, GLenum pname, GLint *params);
void d_glGetQueryiv(void *context, GLenum target, GLenum pname, GLint *params);

void r_glGetQueryObjectuiv(void *context, GLuint id, GLenum pname,
                           GLuint *params);
void d_glGetQueryObjectuiv(void *context, GLuint id, GLenum pname,
                           GLuint *params);

void r_glGetTransformFeedbackVarying(void *context, GLuint program,
                                     GLuint index, GLsizei bufSize,
                                     GLsizei *length, GLsizei *size,
                                     GLenum *type, GLchar *name);
void d_glGetTransformFeedbackVarying(void *context, GLuint program,
                                     GLuint index, GLsizei bufSize,
                                     GLsizei *length, GLsizei *size,
                                     GLenum *type, GLchar *name);

void r_glGetActiveUniformsiv(void *context, GLuint program,
                             GLsizei uniformCount, const GLuint *uniformIndices,
                             GLenum pname, GLint *params);
void d_glGetActiveUniformsiv(void *context, GLuint program,
                             GLsizei uniformCount, const GLuint *uniformIndices,
                             GLenum pname, GLint *params);

void r_glGetActiveUniformBlockiv(void *context, GLuint program,
                                 GLuint uniformBlockIndex, GLenum pname,
                                 GLint *params);
void d_glGetActiveUniformBlockiv(void *context, GLuint program,
                                 GLuint uniformBlockIndex, GLenum pname,
                                 GLint *params);

void r_glGetActiveUniformBlockName(void *context, GLuint program,
                                   GLuint uniformBlockIndex, GLsizei bufSize,
                                   GLsizei *length, GLchar *uniformBlockName);
void d_glGetActiveUniformBlockName(void *context, GLuint program,
                                   GLuint uniformBlockIndex, GLsizei bufSize,
                                   GLsizei *length, GLchar *uniformBlockName);

void r_glGetSamplerParameteriv(void *context, GLuint sampler, GLenum pname,
                               GLint *params);
void d_glGetSamplerParameteriv(void *context, GLuint sampler, GLenum pname,
                               GLint *params);

void r_glGetSamplerParameterfv(void *context, GLuint sampler, GLenum pname,
                               GLfloat *params);
void d_glGetSamplerParameterfv(void *context, GLuint sampler, GLenum pname,
                               GLfloat *params);

void r_glGetProgramBinary(void *context, GLuint program, GLsizei bufSize,
                          GLsizei *length, GLenum *binaryFormat, void *binary);
void d_glGetProgramBinary(void *context, GLuint program, GLsizei bufSize,
                          GLsizei *length, GLenum *binaryFormat, void *binary);

void d_glGetProgramBinaryOES(void *context, GLuint program, GLsizei bufSize,
                             GLsizei *length, GLenum *binaryFormat,
                             void *binary);

void r_glGetInternalformativ(void *context, GLenum target,
                             GLenum internalformat, GLenum pname, GLsizei count,
                             GLint *params);
void d_glGetInternalformativ(void *context, GLenum target,
                             GLenum internalformat, GLenum pname, GLsizei count,
                             GLint *params);

void r_glGetClipPlanexOES(void *context, GLenum plane, GLfixed *equation);
void d_glGetClipPlanexOES(void *context, GLenum plane, GLfixed *equation);

void r_glGetFixedvOES(void *context, GLenum pname, GLfixed *params);
void d_glGetFixedvOES(void *context, GLenum pname, GLfixed *params);

void r_glGetTexEnvxvOES(void *context, GLenum target, GLenum pname,
                        GLfixed *params);
void d_glGetTexEnvxvOES(void *context, GLenum target, GLenum pname,
                        GLfixed *params);

void r_glGetTexParameterxvOES(void *context, GLenum target, GLenum pname,
                              GLfixed *params);
void d_glGetTexParameterxvOES(void *context, GLenum target, GLenum pname,
                              GLfixed *params);

void r_glGetLightxvOES(void *context, GLenum light, GLenum pname,
                       GLfixed *params);
void d_glGetLightxvOES(void *context, GLenum light, GLenum pname,
                       GLfixed *params);

void r_glGetMaterialxvOES(void *context, GLenum face, GLenum pname,
                          GLfixed *params);
void d_glGetMaterialxvOES(void *context, GLenum face, GLenum pname,
                          GLfixed *params);

void r_glGetTexGenxvOES(void *context, GLenum coord, GLenum pname,
                        GLfixed *params);
void d_glGetTexGenxvOES(void *context, GLenum coord, GLenum pname,
                        GLfixed *params);

void r_glGetFramebufferParameteriv(void *context, GLenum target, GLenum pname,
                                   GLint *params);
void d_glGetFramebufferParameteriv(void *context, GLenum target, GLenum pname,
                                   GLint *params);

void r_glGetProgramInterfaceiv(void *context, GLuint program,
                               GLenum programInterface, GLenum pname,
                               GLint *params);
void d_glGetProgramInterfaceiv(void *context, GLuint program,
                               GLenum programInterface, GLenum pname,
                               GLint *params);

void r_glGetProgramResourceName(void *context, GLuint program,
                                GLenum programInterface, GLuint index,
                                GLsizei bufSize, GLsizei *length, GLchar *name);
void d_glGetProgramResourceName(void *context, GLuint program,
                                GLenum programInterface, GLuint index,
                                GLsizei bufSize, GLsizei *length, GLchar *name);

void r_glGetProgramResourceiv(void *context, GLuint program,
                              GLenum programInterface, GLuint index,
                              GLsizei propCount, const GLenum *props,
                              GLsizei bufSize, GLsizei *length, GLint *params);
void d_glGetProgramResourceiv(void *context, GLuint program,
                              GLenum programInterface, GLuint index,
                              GLsizei propCount, const GLenum *props,
                              GLsizei bufSize, GLsizei *length, GLint *params);

void r_glGetProgramPipelineiv(void *context, GLuint pipeline, GLenum pname,
                              GLint *params);
void d_glGetProgramPipelineiv(void *context, GLuint pipeline, GLenum pname,
                              GLint *params);

void r_glGetProgramPipelineInfoLog(void *context, GLuint pipeline,
                                   GLsizei bufSize, GLsizei *length,
                                   GLchar *infoLog);
void d_glGetProgramPipelineInfoLog(void *context, GLuint pipeline,
                                   GLsizei bufSize, GLsizei *length,
                                   GLchar *infoLog);

void r_glGetMultisamplefv(void *context, GLenum pname, GLuint index,
                          GLfloat *val);
void d_glGetMultisamplefv(void *context, GLenum pname, GLuint index,
                          GLfloat *val);

void r_glGetTexLevelParameteriv(void *context, GLenum target, GLint level,
                                GLenum pname, GLint *params);
void d_glGetTexLevelParameteriv(void *context, GLenum target, GLint level,
                                GLenum pname, GLint *params);

void r_glGetTexLevelParameterfv(void *context, GLenum target, GLint level,
                                GLenum pname, GLfloat *params);
void d_glGetTexLevelParameterfv(void *context, GLenum target, GLint level,
                                GLenum pname, GLfloat *params);

void r_glGetSynciv(void *context, GLsync sync, GLenum pname, GLsizei bufSize,
                   GLsizei *length, GLint *values);
void d_glGetSynciv(void *context, GLsync sync, GLenum pname, GLsizei bufSize,
                   GLsizei *length, GLint *values);

GLint r_glGetAttribLocation(void *context, GLuint program, const GLchar *name);
GLint d_glGetAttribLocation(void *context, GLuint program, const GLchar *name);

GLint r_glGetUniformLocation(void *context, GLuint program, const GLchar *name);
GLint d_glGetUniformLocation(void *context, GLuint program, const GLchar *name);

GLint r_glGetFragDataLocation(void *context, GLuint program,
                              const GLchar *name);
GLint d_glGetFragDataLocation(void *context, GLuint program,
                              const GLchar *name);

GLuint r_glGetUniformBlockIndex(void *context, GLuint program,
                                const GLchar *uniformBlockName);
GLuint d_glGetUniformBlockIndex(void *context, GLuint program,
                                const GLchar *uniformBlockName);

GLuint r_glGetProgramResourceIndex(void *context, GLuint program,
                                   GLenum programInterface, const GLchar *name);
GLuint d_glGetProgramResourceIndex(void *context, GLuint program,
                                   GLenum programInterface, const GLchar *name);

GLint r_glGetProgramResourceLocation(void *context, GLuint program,
                                     GLenum programInterface,
                                     const GLchar *name);
GLint d_glGetProgramResourceLocation(void *context, GLuint program,
                                     GLenum programInterface,
                                     const GLchar *name);

void r_glGetActiveAttrib(void *context, GLuint program, GLuint index,
                         GLsizei bufSize, GLsizei *length, GLint *size,
                         GLenum *type, GLchar *name);
void d_glGetActiveAttrib(void *context, GLuint program, GLuint index,
                         GLsizei bufSize, GLsizei *length, GLint *size,
                         GLenum *type, GLchar *name);

void r_glGetActiveUniform(void *context, GLuint program, GLuint index,
                          GLsizei bufSize, GLsizei *length, GLint *size,
                          GLenum *type, GLchar *name);
void d_glGetActiveUniform(void *context, GLuint program, GLuint index,
                          GLsizei bufSize, GLsizei *length, GLint *size,
                          GLenum *type, GLchar *name);

void r_glGetAttachedShaders(void *context, GLuint program, GLsizei maxCount,
                            GLsizei *count, GLuint *shaders);
void d_glGetAttachedShaders(void *context, GLuint program, GLsizei maxCount,
                            GLsizei *count, GLuint *shaders);

void r_glGetProgramiv(void *context, GLuint program, GLenum pname,
                      GLint *params);
void d_glGetProgramiv(void *context, GLuint program, GLenum pname,
                      GLint *params);

void r_glGetShaderiv(void *context, GLuint shader, GLenum pname, GLint *params);
void d_glGetShaderiv(void *context, GLuint shader, GLenum pname, GLint *params);

void r_glGetUniformfv(void *context, GLuint program, GLint location,
                      GLfloat *params);
void d_glGetUniformfv(void *context, GLuint program, GLint location,
                      GLfloat *params);

void r_glGetUniformiv(void *context, GLuint program, GLint location,
                      GLint *params);
void d_glGetUniformiv(void *context, GLuint program, GLint location,
                      GLint *params);

void r_glGetUniformuiv(void *context, GLuint program, GLint location,
                       GLuint *params);
void d_glGetUniformuiv(void *context, GLuint program, GLint location,
                       GLuint *params);

void r_glGetUniformIndices(void *context, GLuint program, GLsizei uniformCount,
                           const GLchar *const *uniformNames,
                           GLuint *uniformIndices);
void d_glGetUniformIndices(void *context, GLuint program, GLsizei uniformCount,
                           const GLchar *const *uniformNames,
                           GLuint *uniformIndices);

void r_glGetVertexAttribfv(void *context, GLuint index, GLenum pname,
                           GLfloat *params);
void d_glGetVertexAttribfv(void *context, GLuint index, GLenum pname,
                           GLfloat *params);

void r_glGetVertexAttribiv(void *context, GLuint index, GLenum pname,
                           GLint *params);
void d_glGetVertexAttribiv(void *context, GLuint index, GLenum pname,
                           GLint *params);

void r_glGetVertexAttribIiv(void *context, GLuint index, GLenum pname,
                            GLint *params);
void d_glGetVertexAttribIiv(void *context, GLuint index, GLenum pname,
                            GLint *params);

void r_glGetVertexAttribIuiv(void *context, GLuint index, GLenum pname,
                             GLuint *params);
void d_glGetVertexAttribIuiv(void *context, GLuint index, GLenum pname,
                             GLuint *params);

void r_glGetBufferParameteriv(void *context, GLenum target, GLenum pname,
                              GLint *params);
void d_glGetBufferParameteriv(void *context, GLenum target, GLenum pname,
                              GLint *params);

void r_glGetBufferParameteri64v(void *context, GLenum target, GLenum pname,
                                GLint64 *params);
void d_glGetBufferParameteri64v(void *context, GLenum target, GLenum pname,
                                GLint64 *params);

void r_glGetBooleanv(void *context, GLenum pname, GLboolean *data);
void d_glGetBooleanv(void *context, GLenum pname, GLboolean *data);

void r_glGetBooleani_v(void *context, GLenum target, GLuint index,
                       GLboolean *data);
void d_glGetBooleani_v(void *context, GLenum target, GLuint index,
                       GLboolean *data);

void r_glGetFloatv(void *context, GLenum pname, GLfloat *data);
void d_glGetFloatv(void *context, GLenum pname, GLfloat *data);

void r_glGetIntegerv(void *context, GLenum pname, GLint *data);
void d_glGetIntegerv(void *context, GLenum pname, GLint *data);

void r_glGetIntegeri_v(void *context, GLenum target, GLuint index, GLint *data);
void d_glGetIntegeri_v(void *context, GLenum target, GLuint index, GLint *data);

void r_glGetInteger64v(void *context, GLenum pname, GLint64 *data);
void d_glGetInteger64v(void *context, GLenum pname, GLint64 *data);

void r_glGetInteger64i_v(void *context, GLenum target, GLuint index,
                         GLint64 *data);
void d_glGetInteger64i_v(void *context, GLenum target, GLuint index,
                         GLint64 *data);

void r_glMapBufferRange_read(void *context, GLenum target, GLintptr offset,
                             GLsizeiptr length, GLbitfield access,
                             void *mem_buf);

void r_glReadPixels_without_bound(void *context, GLint x, GLint y,
                                  GLsizei width, GLsizei height, GLenum format,
                                  GLenum type, GLint buf_len, void *pixels);

GLint r_glTestPointer3(void *context, GLint a, const GLint *b, GLint *c);
GLint d_glTestPointer3(void *context, GLint a, const GLint *b, GLint *c);

void r_glFlush(void *context);
void d_glFlush(void *context);

void r_glFinish(void *context);
void d_glFinish(void *context);

void r_glBeginQuery(void *context, GLenum target, GLuint id);
void d_glBeginQuery(void *context, GLenum target, GLuint id);

void r_glEndQuery(void *context, GLenum target);
void d_glEndQuery(void *context, GLenum target);

void r_glViewport(void *context, GLint x, GLint y, GLsizei width,
                  GLsizei height);
void d_glViewport(void *context, GLint x, GLint y, GLsizei width,
                  GLsizei height);

void r_glTexStorage2D(void *context, GLenum target, GLsizei levels,
                      GLenum internalformat, GLsizei width, GLsizei height);
void d_glTexStorage2D(void *context, GLenum target, GLsizei levels,
                      GLenum internalformat, GLsizei width, GLsizei height);

void r_glTexStorage3D(void *context, GLenum target, GLsizei levels,
                      GLenum internalformat, GLsizei width, GLsizei height,
                      GLsizei depth);
void d_glTexStorage3D(void *context, GLenum target, GLsizei levels,
                      GLenum internalformat, GLsizei width, GLsizei height,
                      GLsizei depth);

void r_glTexImage2D_with_bound(void *context, GLenum target, GLint level,
                               GLint internalformat, GLsizei width,
                               GLsizei height, GLint border, GLenum format,
                               GLenum type, GLintptr pixels);

void r_glTexSubImage2D_with_bound(void *context, GLenum target, GLint level,
                                  GLint xoffset, GLint yoffset, GLsizei width,
                                  GLsizei height, GLenum format, GLenum type,
                                  GLintptr pixels);

void r_glTexImage3D_with_bound(void *context, GLenum target, GLint level,
                               GLint internalformat, GLsizei width,
                               GLsizei height, GLsizei depth, GLint border,
                               GLenum format, GLenum type, GLintptr pixels);

void r_glTexSubImage3D_with_bound(void *context, GLenum target, GLint level,
                                  GLint xoffset, GLint yoffset, GLint zoffset,
                                  GLsizei width, GLsizei height, GLsizei depth,
                                  GLenum format, GLenum type, GLintptr pixels);

void r_glReadPixels_with_bound(void *context, GLint x, GLint y, GLsizei width,
                               GLsizei height, GLenum format, GLenum type,
                               GLintptr pixels);

void r_glCompressedTexImage2D_with_bound(void *context, GLenum target,
                                         GLint level, GLenum internalformat,
                                         GLsizei width, GLsizei height,
                                         GLint border, GLsizei imageSize,
                                         GLintptr data);

void r_glCompressedTexSubImage2D_with_bound(void *context, GLenum target,
                                            GLint level, GLint xoffset,
                                            GLint yoffset, GLsizei width,
                                            GLsizei height, GLenum format,
                                            GLsizei imageSize, GLintptr data);

void r_glCompressedTexImage3D_with_bound(void *context, GLenum target,
                                         GLint level, GLenum internalformat,
                                         GLsizei width, GLsizei height,
                                         GLsizei depth, GLint border,
                                         GLsizei imageSize, GLintptr data);

void r_glCompressedTexSubImage3D_with_bound(void *context, GLenum target,
                                            GLint level, GLint xoffset,
                                            GLint yoffset, GLint zoffset,
                                            GLsizei width, GLsizei height,
                                            GLsizei depth, GLenum format,
                                            GLsizei imageSize, GLintptr data);

void r_glCopyTexImage2D(void *context, GLenum target, GLint level,
                        GLenum internalformat, GLint x, GLint y, GLsizei width,
                        GLsizei height, GLint border);
void d_glCopyTexImage2D(void *context, GLenum target, GLint level,
                        GLenum internalformat, GLint x, GLint y, GLsizei width,
                        GLsizei height, GLint border);

void r_glCopyTexSubImage2D(void *context, GLenum target, GLint level,
                           GLint xoffset, GLint yoffset, GLint x, GLint y,
                           GLsizei width, GLsizei height);
void d_glCopyTexSubImage2D(void *context, GLenum target, GLint level,
                           GLint xoffset, GLint yoffset, GLint x, GLint y,
                           GLsizei width, GLsizei height);

void r_glCopyTexSubImage3D(void *context, GLenum target, GLint level,
                           GLint xoffset, GLint yoffset, GLint zoffset, GLint x,
                           GLint y, GLsizei width, GLsizei height);
void d_glCopyTexSubImage3D(void *context, GLenum target, GLint level,
                           GLint xoffset, GLint yoffset, GLint zoffset, GLint x,
                           GLint y, GLsizei width, GLsizei height);

void r_glVertexAttribPointer_with_bound(void *context, GLuint index, GLint size,
                                        GLenum type, GLboolean normalized,
                                        GLsizei stride, GLintptr pointer);

void r_glVertexAttribPointer_offset(void *context, GLuint index, GLuint size,
                                    GLenum type, GLboolean normalized,
                                    GLsizei stride, GLuint index_father,
                                    GLintptr offset);

void r_glMapBufferRange_write(void *context, GLenum target, GLintptr offset,
                              GLsizeiptr length, GLbitfield access);

void r_glUnmapBuffer_special(void *context, GLenum target);
GLboolean d_glUnmapBuffer(void *context, GLenum target);

void r_glWaitSync(void *context, GLsync sync, GLbitfield flags,
                  GLuint64 timeout);
void d_glWaitSync(void *context, GLsync sync, GLbitfield flags,
                  GLuint64 timeout);

void r_glShaderBinary(void *context, GLsizei count, const GLuint *shaders,
                      GLenum binaryFormat, const void *binary, GLsizei length);
void d_glShaderBinary(void *context, GLsizei count, const GLuint *shaders,
                      GLenum binaryFormat, const void *binary, GLsizei length);

void r_glProgramBinary_special(void *context, GLuint program,
                               GLenum binaryFormat, const void *binary,
                               GLsizei length);
void d_glProgramBinary(void *context, GLuint program, GLenum binaryFormat,
                       const void *binary, GLsizei length);

void d_glProgramBinaryOES(void *context, GLuint program, GLenum binaryFormat,
                          const void *binary, GLsizei length);

void r_glDrawBuffers(void *context, GLsizei n, const GLenum *bufs);
void d_glDrawBuffers(void *context, GLsizei n, const GLenum *bufs);

void r_glDrawArrays_origin(void *context, GLenum mode, GLint first,
                           GLsizei count);
void d_glDrawArrays(void *context, GLenum mode, GLint first, GLsizei count);

void r_glDrawArraysInstanced_origin(void *context, GLenum mode, GLint first,
                                    GLsizei count, GLsizei instancecount);
void d_glDrawArraysInstanced(void *context, GLenum mode, GLint first,
                             GLsizei count, GLsizei instancecount);

void r_glDrawElementsInstanced_with_bound(void *context, GLenum mode,
                                          GLsizei count, GLenum type,
                                          GLsizeiptr indices,
                                          GLsizei instancecount);

void r_glDrawElements_with_bound(void *context, GLenum mode, GLsizei count,
                                 GLenum type, GLsizeiptr indices);

void r_glDrawRangeElements_with_bound(void *context, GLenum mode, GLuint start,
                                      GLuint end, GLsizei count, GLenum type,
                                      GLsizeiptr indices);

void r_glTestIntAsyn(void *context, GLint a, GLuint b, GLfloat c, GLdouble d);
void d_glTestIntAsyn(void *context, GLint a, GLuint b, GLfloat c, GLdouble d);

void r_glPrintfAsyn(void *context, GLint a, GLuint size, GLdouble c,
                    const GLchar *out_string);
void d_glPrintfAsyn(void *context, GLint a, GLuint size, GLdouble c,
                    const GLchar *out_string);

void r_glEGLImageTargetTexture2DOES(void *context, GLenum target,
                                    uint64_t gbuffer_id);
void d_glEGLImageTargetTexture2DOES(void *context, GLenum target,
                                    GLeglImageOES image);

void r_glEGLImageTargetRenderbufferStorageOES(void *context, GLenum target,
                                              uint64_t gbuffer_id);
void d_glEGLImageTargetRenderbufferStorageOES(void *context, GLenum target,
                                              GLeglImageOES image);

void r_glGenBuffers(void *context, GLsizei n, const GLuint *buffers);
void d_glGenBuffers(void *context, GLsizei n, GLuint *buffers);

void r_glGenRenderbuffers(void *context, GLsizei n,
                          const GLuint *renderbuffers);
void d_glGenRenderbuffers(void *context, GLsizei n, GLuint *renderbuffers);

void r_glGenTextures(void *context, GLsizei n, const GLuint *textures);
void d_glGenTextures(void *context, GLsizei n, GLuint *textures);

void r_glGenSamplers(void *context, GLsizei count, const GLuint *samplers);
void d_glGenSamplers(void *context, GLsizei count, GLuint *samplers);

void r_glCreateProgram(void *context, GLuint program);
GLuint d_glCreateProgram(void *context);

void r_glCreateShader(void *context, GLenum type, GLuint shader);
GLuint d_glCreateShader(void *context, GLenum type);

void r_glFenceSync(void *context, GLenum condition, GLbitfield flags,
                   GLsync sync);
GLsync d_glFenceSync(void *context, GLenum condition, GLbitfield flags);

void r_glCreateShaderProgramv(void *context, GLenum type, GLsizei count,
                              const GLchar *const *strings, GLuint program);
GLuint d_glCreateShaderProgramv(void *context, GLenum type, GLsizei count,
                                const GLchar *const *strings);

void r_glGenFramebuffers(void *context, GLsizei n, const GLuint *framebuffers);
void d_glGenFramebuffers(void *context, GLsizei n, GLuint *framebuffers);

void r_glGenProgramPipelines(void *context, GLsizei n, const GLuint *pipelines);
void d_glGenProgramPipelines(void *context, GLsizei n, GLuint *pipelines);

void r_glGenTransformFeedbacks(void *context, GLsizei n, const GLuint *ids);
void d_glGenTransformFeedbacks(void *context, GLsizei n, GLuint *ids);

void r_glGenVertexArrays(void *context, GLsizei n, const GLuint *arrays);
void d_glGenVertexArrays(void *context, GLsizei n, GLuint *arrays);

void r_glGenQueries(void *context, GLsizei n, const GLuint *ids);
void d_glGenQueries(void *context, GLsizei n, GLuint *ids);

void r_glDeleteBuffers_origin(void *context, GLsizei n, const GLuint *buffers);
void d_glDeleteBuffers(void *context, GLsizei n, const GLuint *buffers);

void r_glDeleteRenderbuffers(void *context, GLsizei n,
                             const GLuint *renderbuffers);
void d_glDeleteRenderbuffers(void *context, GLsizei n,
                             const GLuint *renderbuffers);

void r_glDeleteTextures(void *context, GLsizei n, const GLuint *textures);
void d_glDeleteTextures(void *context, GLsizei n, const GLuint *textures);

void r_glDeleteSamplers(void *context, GLsizei count, const GLuint *samplers);
void d_glDeleteSamplers(void *context, GLsizei count, const GLuint *samplers);

void r_glDeleteProgram_origin(void *context, GLuint program);
void d_glDeleteProgram(void *context, GLuint program);

void r_glDeleteShader(void *context, GLuint shader);
void d_glDeleteShader(void *context, GLuint shader);

void r_glDeleteSync(void *context, GLsync sync);
void d_glDeleteSync(void *context, GLsync sync);

void r_glDeleteFramebuffers(void *context, GLsizei n,
                            const GLuint *framebuffers);
void d_glDeleteFramebuffers(void *context, GLsizei n,
                            const GLuint *framebuffers);

void r_glDeleteProgramPipelines(void *context, GLsizei n,
                                const GLuint *pipelines);
void d_glDeleteProgramPipelines(void *context, GLsizei n,
                                const GLuint *pipelines);

void r_glDeleteTransformFeedbacks(void *context, GLsizei n, const GLuint *ids);
void d_glDeleteTransformFeedbacks(void *context, GLsizei n, const GLuint *ids);

void r_glDeleteVertexArrays_origin(void *context, GLsizei n,
                                   const GLuint *arrays);
void d_glDeleteVertexArrays(void *context, GLsizei n, const GLuint *arrays);

void r_glDeleteQueries(void *context, GLsizei n, const GLuint *ids);
void d_glDeleteQueries(void *context, GLsizei n, const GLuint *ids);

void r_glLinkProgram_special(void *context, GLuint program);
void d_glLinkProgram(void *context, GLuint program);

void r_glPixelStorei_origin(void *context, GLenum pname, GLint param);
void d_glPixelStorei(void *context, GLenum pname, GLint param);

void r_glDisableVertexAttribArray_origin(void *context, GLuint index);
void d_glDisableVertexAttribArray(void *context, GLuint index);

void r_glEnableVertexAttribArray_origin(void *context, GLuint index);
void d_glEnableVertexAttribArray(void *context, GLuint index);

void r_glReadBuffer_special(void *context, GLenum src);
void d_glReadBuffer(void *context, GLenum src);

void r_glVertexAttribDivisor_origin(void *context, GLuint index,
                                    GLuint divisor);
void r_glVertexAttribDivisor(void *context, GLuint index, GLuint divisor);

void r_glShaderSource_origin(void *context, GLuint shader, GLsizei count,
                             const GLint *length, const GLchar *const *string);
void d_glShaderSource(void *context, GLuint shader, GLsizei count,
                      const GLchar *const *string, const GLint *length);

void r_glVertexAttribIPointer_with_bound(void *context, GLuint index,
                                         GLint size, GLenum type,
                                         GLsizei stride, GLintptr pointer);

void r_glVertexAttribIPointer_offset(void *context, GLuint index, GLint size,
                                     GLenum type, GLsizei stride,
                                     GLuint index_father, GLintptr offset);

void r_glBindVertexArray_special(void *context, GLuint array);
void d_glBindVertexArray(void *context, GLuint array);

void r_glBindBuffer_origin(void *context, GLenum target, GLuint buffer);
void d_glBindBuffer(void *context, GLenum target, GLuint buffer);

void r_glBeginTransformFeedback(void *context, GLenum primitiveMode);
void d_glBeginTransformFeedback(void *context, GLenum primitiveMode);

void r_glEndTransformFeedback(void *context);
void d_glEndTransformFeedback(void *context);

void r_glPauseTransformFeedback(void *context);
void d_glPauseTransformFeedback(void *context);

void r_glResumeTransformFeedback(void *context);
void d_glResumeTransformFeedback(void *context);

void r_glBindBufferRange(void *context, GLenum target, GLuint index,
                         GLuint buffer, GLintptr offset, GLsizeiptr size);
void d_glBindBufferRange(void *context, GLenum target, GLuint index,
                         GLuint buffer, GLintptr offset, GLsizeiptr size);

void r_glBindBufferBase(void *context, GLenum target, GLuint index,
                        GLuint buffer);
void d_glBindBufferBase(void *context, GLenum target, GLuint index,
                        GLuint buffer);

void r_glBindTexture(void *context, GLenum target, GLuint texture);
void d_glBindTexture(void *context, GLenum target, GLuint texture);

void r_glBindRenderbuffer(void *context, GLenum target, GLuint renderbuffer);
void d_glBindRenderbuffer(void *context, GLenum target, GLuint renderbuffer);

void r_glBindSampler(void *context, GLuint unit, GLuint sampler);
void d_glBindSampler(void *context, GLuint unit, GLuint sampler);

void r_glBindFramebuffer(void *context, GLenum target, GLuint framebuffer);
void d_glBindFramebuffer(void *context, GLenum target, GLuint framebuffer);

void r_glBindProgramPipeline(void *context, GLuint pipeline);
void d_glBindProgramPipeline(void *context, GLuint pipeline);

void r_glBindTransformFeedback(void *context, GLenum target,
                               GLuint feedback_id);
void d_glBindTransformFeedback(void *context, GLenum target,
                               GLuint feedback_id);

void r_glActiveTexture(void *context, GLenum texture);
void d_glActiveTexture(void *context, GLenum texture);

void r_glAttachShader(void *context, GLuint program, GLuint shader);
void d_glAttachShader(void *context, GLuint program, GLuint shader);

void r_glBlendColor(void *context, GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat alpha);
void d_glBlendColor(void *context, GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat alpha);

void r_glBlendEquation(void *context, GLenum mode);
void d_glBlendEquation(void *context, GLenum mode);

void r_glBlendEquationSeparate(void *context, GLenum modeRGB, GLenum modeAlpha);
void d_glBlendEquationSeparate(void *context, GLenum modeRGB, GLenum modeAlpha);

void r_glBlendFunc(void *context, GLenum sfactor, GLenum dfactor);
void d_glBlendFunc(void *context, GLenum sfactor, GLenum dfactor);

void r_glBlendFuncSeparate(void *context, GLenum sfactorRGB, GLenum dfactorRGB,
                           GLenum sfactorAlpha, GLenum dfactorAlpha);
void d_glBlendFuncSeparate(void *context, GLenum sfactorRGB, GLenum dfactorRGB,
                           GLenum sfactorAlpha, GLenum dfactorAlpha);

void r_glClear(void *context, GLbitfield mask);
void d_glClear(void *context, GLbitfield mask);

void r_glClearColor(void *context, GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat alpha);
void d_glClearColor(void *context, GLfloat red, GLfloat green, GLfloat blue,
                    GLfloat alpha);

void r_glClearDepthf(void *context, GLfloat d);
void d_glClearDepthf(void *context, GLfloat d);

void r_glClearStencil(void *context, GLint s);
void d_glClearStencil(void *context, GLint s);

void r_glColorMask(void *context, GLboolean red, GLboolean green,
                   GLboolean blue, GLboolean alpha);
void d_glColorMask(void *context, GLboolean red, GLboolean green,
                   GLboolean blue, GLboolean alpha);

void r_glCompileShader(void *context, GLuint shader);
void d_glCompileShader(void *context, GLuint shader);

void r_glCullFace(void *context, GLenum mode);
void d_glCullFace(void *context, GLenum mode);

void r_glDepthFunc(void *context, GLenum func);
void d_glDepthFunc(void *context, GLenum func);

void r_glDepthMask(void *context, GLboolean flag);
void d_glDepthMask(void *context, GLboolean flag);

void r_glDepthRangef(void *context, GLfloat n, GLfloat f);
void d_glDepthRangef(void *context, GLfloat n, GLfloat f);

void r_glDetachShader(void *context, GLuint program, GLuint shader);
void d_glDetachShader(void *context, GLuint program, GLuint shader);

void r_glDisable(void *context, GLenum cap);
void d_glDisable(void *context, GLenum cap);

void r_glEnable(void *context, GLenum cap);
void d_glEnable(void *context, GLenum cap);

void r_glFramebufferRenderbuffer(void *context, GLenum target,
                                 GLenum attachment, GLenum renderbuffertarget,
                                 GLuint renderbuffer);
void d_glFramebufferRenderbuffer(void *context, GLenum target,
                                 GLenum attachment, GLenum renderbuffertarget,
                                 GLuint renderbuffer);

void r_glFramebufferTexture2D(void *context, GLenum target, GLenum attachment,
                              GLenum textarget, GLuint texture, GLint level);
void d_glFramebufferTexture2D(void *context, GLenum target, GLenum attachment,
                              GLenum textarget, GLuint texture, GLint level);

void r_glFrontFace(void *context, GLenum mode);
void d_glFrontFace(void *context, GLenum mode);

void r_glGenerateMipmap(void *context, GLenum target);
void d_glGenerateMipmap(void *context, GLenum target);

void r_glHint(void *context, GLenum target, GLenum mode);
void d_glHint(void *context, GLenum target, GLenum mode);

void r_glLineWidth(void *context, GLfloat width);
void d_glLineWidth(void *context, GLfloat width);

void r_glPolygonOffset(void *context, GLfloat factor, GLfloat units);
void d_glPolygonOffset(void *context, GLfloat factor, GLfloat units);

void r_glReleaseShaderCompiler(void *context);
void d_glReleaseShaderCompiler(void *context);

void r_glRenderbufferStorage(void *context, GLenum target,
                             GLenum internalformat, GLsizei width,
                             GLsizei height);
void d_glRenderbufferStorage(void *context, GLenum target,
                             GLenum internalformat, GLsizei width,
                             GLsizei height);

void r_glSampleCoverage(void *context, GLfloat value, GLboolean invert);
void d_glSampleCoverage(void *context, GLfloat value, GLboolean invert);

void r_glScissor(void *context, GLint x, GLint y, GLsizei width,
                 GLsizei height);
void d_glScissor(void *context, GLint x, GLint y, GLsizei width,
                 GLsizei height);

void r_glStencilFunc(void *context, GLenum func, GLint ref, GLuint mask);
void d_glStencilFunc(void *context, GLenum func, GLint ref, GLuint mask);

void r_glStencilFuncSeparate(void *context, GLenum face, GLenum func, GLint ref,
                             GLuint mask);
void d_glStencilFuncSeparate(void *context, GLenum face, GLenum func, GLint ref,
                             GLuint mask);

void r_glStencilMask(void *context, GLuint mask);
void d_glStencilMask(void *context, GLuint mask);

void r_glStencilMaskSeparate(void *context, GLenum face, GLuint mask);
void d_glStencilMaskSeparate(void *context, GLenum face, GLuint mask);

void r_glStencilOp(void *context, GLenum fail, GLenum zfail, GLenum zpass);
void d_glStencilOp(void *context, GLenum fail, GLenum zfail, GLenum zpass);

void r_glStencilOpSeparate(void *context, GLenum face, GLenum sfail,
                           GLenum dpfail, GLenum dppass);
void d_glStencilOpSeparate(void *context, GLenum face, GLenum sfail,
                           GLenum dpfail, GLenum dppass);

void r_glTexParameterf(void *context, GLenum target, GLenum pname,
                       GLfloat param);
void d_glTexParameterf(void *context, GLenum target, GLenum pname,
                       GLfloat param);

void r_glTexParameteri(void *context, GLenum target, GLenum pname, GLint param);
void d_glTexParameteri(void *context, GLenum target, GLenum pname, GLint param);

void r_glUniform1f(void *context, GLint location, GLfloat v0);
void d_glUniform1f(void *context, GLint location, GLfloat v0);

void r_glUniform1i(void *context, GLint location, GLint v0);
void d_glUniform1i(void *context, GLint location, GLint v0);

void r_glUniform2f(void *context, GLint location, GLfloat v0, GLfloat v1);
void d_glUniform2f(void *context, GLint location, GLfloat v0, GLfloat v1);

void r_glUniform2i(void *context, GLint location, GLint v0, GLint v1);
void d_glUniform2i(void *context, GLint location, GLint v0, GLint v1);

void r_glUniform3f(void *context, GLint location, GLfloat v0, GLfloat v1,
                   GLfloat v2);
void d_glUniform3f(void *context, GLint location, GLfloat v0, GLfloat v1,
                   GLfloat v2);

void r_glUniform3i(void *context, GLint location, GLint v0, GLint v1, GLint v2);
void d_glUniform3i(void *context, GLint location, GLint v0, GLint v1, GLint v2);

void r_glUniform4f(void *context, GLint location, GLfloat v0, GLfloat v1,
                   GLfloat v2, GLfloat v3);
void d_glUniform4f(void *context, GLint location, GLfloat v0, GLfloat v1,
                   GLfloat v2, GLfloat v3);

void r_glUniform4i(void *context, GLint location, GLint v0, GLint v1, GLint v2,
                   GLint v3);
void d_glUniform4i(void *context, GLint location, GLint v0, GLint v1, GLint v2,
                   GLint v3);

void r_glUseProgram(void *context, GLuint program);
void d_glUseProgram(void *context, GLuint program);

void r_glValidateProgram(void *context, GLuint program);
void d_glValidateProgram(void *context, GLuint program);

void r_glVertexAttrib1f(void *context, GLuint index, GLfloat x);
void d_glVertexAttrib1f(void *context, GLuint index, GLfloat x);

void r_glVertexAttrib2f(void *context, GLuint index, GLfloat x, GLfloat y);
void d_glVertexAttrib2f(void *context, GLuint index, GLfloat x, GLfloat y);

void r_glVertexAttrib3f(void *context, GLuint index, GLfloat x, GLfloat y,
                        GLfloat z);
void d_glVertexAttrib3f(void *context, GLuint index, GLfloat x, GLfloat y,
                        GLfloat z);

void r_glVertexAttrib4f(void *context, GLuint index, GLfloat x, GLfloat y,
                        GLfloat z, GLfloat w);
void d_glVertexAttrib4f(void *context, GLuint index, GLfloat x, GLfloat y,
                        GLfloat z, GLfloat w);

void r_glBlitFramebuffer(void *context, GLint srcX0, GLint srcY0, GLint srcX1,
                         GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1,
                         GLint dstY1, GLbitfield mask, GLenum filter);
void d_glBlitFramebuffer(void *context, GLint srcX0, GLint srcY0, GLint srcX1,
                         GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1,
                         GLint dstY1, GLbitfield mask, GLenum filter);

void r_glRenderbufferStorageMultisample(void *context, GLenum target,
                                        GLsizei samples, GLenum internalformat,
                                        GLsizei width, GLsizei height);
void d_glRenderbufferStorageMultisample(void *context, GLenum target,
                                        GLsizei samples, GLenum internalformat,
                                        GLsizei width, GLsizei height);

void r_glFramebufferTextureLayer(void *context, GLenum target,
                                 GLenum attachment, GLuint texture, GLint level,
                                 GLint layer);
void d_glFramebufferTextureLayer(void *context, GLenum target,
                                 GLenum attachment, GLuint texture, GLint level,
                                 GLint layer);

void r_glVertexAttribI4i(void *context, GLuint index, GLint x, GLint y, GLint z,
                         GLint w);
void d_glVertexAttribI4i(void *context, GLuint index, GLint x, GLint y, GLint z,
                         GLint w);

void r_glVertexAttribI4ui(void *context, GLuint index, GLuint x, GLuint y,
                          GLuint z, GLuint w);
void d_glVertexAttribI4ui(void *context, GLuint index, GLuint x, GLuint y,
                          GLuint z, GLuint w);

void r_glUniform1ui(void *context, GLint location, GLuint v0);
void d_glUniform1ui(void *context, GLint location, GLuint v0);

void r_glUniform2ui(void *context, GLint location, GLuint v0, GLuint v1);
void d_glUniform2ui(void *context, GLint location, GLuint v0, GLuint v1);

void r_glUniform3ui(void *context, GLint location, GLuint v0, GLuint v1,
                    GLuint v2);
void d_glUniform3ui(void *context, GLint location, GLuint v0, GLuint v1,
                    GLuint v2);

void r_glUniform4ui(void *context, GLint location, GLuint v0, GLuint v1,
                    GLuint v2, GLuint v3);
void d_glUniform4ui(void *context, GLint location, GLuint v0, GLuint v1,
                    GLuint v2, GLuint v3);

void r_glClearBufferfi(void *context, GLenum buffer, GLint drawbuffer,
                       GLfloat depth, GLint stencil);
void d_glClearBufferfi(void *context, GLenum buffer, GLint drawbuffer,
                       GLfloat depth, GLint stencil);

void r_glCopyBufferSubData(void *context, GLenum readTarget, GLenum writeTarget,
                           GLintptr readOffset, GLintptr writeOffset,
                           GLsizeiptr size);
void d_glCopyBufferSubData(void *context, GLenum readTarget, GLenum writeTarget,
                           GLintptr readOffset, GLintptr writeOffset,
                           GLsizeiptr size);

void r_glUniformBlockBinding(void *context, GLuint program,
                             GLuint uniformBlockIndex,
                             GLuint uniformBlockBinding);
void d_glUniformBlockBinding(void *context, GLuint program,
                             GLuint uniformBlockIndex,
                             GLuint uniformBlockBinding);

void r_glSamplerParameteri(void *context, GLuint sampler, GLenum pname,
                           GLint param);
void d_glSamplerParameteri(void *context, GLuint sampler, GLenum pname,
                           GLint param);

void r_glSamplerParameterf(void *context, GLuint sampler, GLenum pname,
                           GLfloat param);
void d_glSamplerParameterf(void *context, GLuint sampler, GLenum pname,
                           GLfloat param);

void r_glProgramParameteri(void *context, GLuint program, GLenum pname,
                           GLint value);
void d_glProgramParameteri(void *context, GLuint program, GLenum pname,
                           GLint value);

void r_glAlphaFuncxOES(void *context, GLenum func, GLfixed ref);
void d_glAlphaFuncxOES(void *context, GLenum func, GLfixed ref);

void r_glClearColorxOES(void *context, GLfixed red, GLfixed green, GLfixed blue,
                        GLfixed alpha);
void d_glClearColorxOES(void *context, GLfixed red, GLfixed green, GLfixed blue,
                        GLfixed alpha);

void r_glClearDepthxOES(void *context, GLfixed depth);
void d_glClearDepthxOES(void *context, GLfixed depth);

void r_glColor4xOES(void *context, GLfixed red, GLfixed green, GLfixed blue,
                    GLfixed alpha);
void d_glColor4xOES(void *context, GLfixed red, GLfixed green, GLfixed blue,
                    GLfixed alpha);

void r_glDepthRangexOES(void *context, GLfixed n, GLfixed f);
void d_glDepthRangexOES(void *context, GLfixed n, GLfixed f);

void r_glFogxOES(void *context, GLenum pname, GLfixed param);
void d_glFogxOES(void *context, GLenum pname, GLfixed param);

void r_glFrustumxOES(void *context, GLfixed l, GLfixed r, GLfixed b, GLfixed t,
                     GLfixed n, GLfixed f);
void d_glFrustumxOES(void *context, GLfixed l, GLfixed r, GLfixed b, GLfixed t,
                     GLfixed n, GLfixed f);

void r_glLightModelxOES(void *context, GLenum pname, GLfixed param);
void d_glLightModelxOES(void *context, GLenum pname, GLfixed param);

void r_glLightxOES(void *context, GLenum light, GLenum pname, GLfixed param);
void d_glLightxOES(void *context, GLenum light, GLenum pname, GLfixed param);

void r_glLineWidthxOES(void *context, GLfixed width);
void d_glLineWidthxOES(void *context, GLfixed width);

void r_glMaterialxOES(void *context, GLenum face, GLenum pname, GLfixed param);
void d_glMaterialxOES(void *context, GLenum face, GLenum pname, GLfixed param);

void r_glMultiTexCoord4xOES(void *context, GLenum texture, GLfixed s, GLfixed t,
                            GLfixed r, GLfixed q);
void d_glMultiTexCoord4xOES(void *context, GLenum texture, GLfixed s, GLfixed t,
                            GLfixed r, GLfixed q);

void r_glNormal3xOES(void *context, GLfixed nx, GLfixed ny, GLfixed nz);
void d_glNormal3xOES(void *context, GLfixed nx, GLfixed ny, GLfixed nz);

void r_glOrthoxOES(void *context, GLfixed l, GLfixed r, GLfixed b, GLfixed t,
                   GLfixed n, GLfixed f);
void d_glOrthoxOES(void *context, GLfixed l, GLfixed r, GLfixed b, GLfixed t,
                   GLfixed n, GLfixed f);

void r_glPointSizexOES(void *context, GLfixed size);
void d_glPointSizexOES(void *context, GLfixed size);

void r_glPolygonOffsetxOES(void *context, GLfixed factor, GLfixed units);
void d_glPolygonOffsetxOES(void *context, GLfixed factor, GLfixed units);

void r_glRotatexOES(void *context, GLfixed angle, GLfixed x, GLfixed y,
                    GLfixed z);
void d_glRotatexOES(void *context, GLfixed angle, GLfixed x, GLfixed y,
                    GLfixed z);

void r_glScalexOES(void *context, GLfixed x, GLfixed y, GLfixed z);
void d_glScalexOES(void *context, GLfixed x, GLfixed y, GLfixed z);

void r_glTexEnvxOES(void *context, GLenum target, GLenum pname, GLfixed param);
void d_glTexEnvxOES(void *context, GLenum target, GLenum pname, GLfixed param);

void r_glTranslatexOES(void *context, GLfixed x, GLfixed y, GLfixed z);
void d_glTranslatexOES(void *context, GLfixed x, GLfixed y, GLfixed z);

void r_glPointParameterxOES(void *context, GLenum pname, GLfixed param);
void d_glPointParameterxOES(void *context, GLenum pname, GLfixed param);

void r_glSampleCoveragexOES(void *context, GLclampx value, GLboolean invert);
void d_glSampleCoveragexOES(void *context, GLclampx value, GLboolean invert);

void r_glTexGenxOES(void *context, GLenum coord, GLenum pname, GLfixed param);
void d_glTexGenxOES(void *context, GLenum coord, GLenum pname, GLfixed param);

void r_glClearDepthfOES(void *context, GLclampf depth);
void d_glClearDepthfOES(void *context, GLclampf depth);

void r_glDepthRangefOES(void *context, GLclampf n, GLclampf f);
void d_glDepthRangefOES(void *context, GLclampf n, GLclampf f);

void r_glFrustumfOES(void *context, GLfloat l, GLfloat r, GLfloat b, GLfloat t,
                     GLfloat n, GLfloat f);
void d_glFrustumfOES(void *context, GLfloat l, GLfloat r, GLfloat b, GLfloat t,
                     GLfloat n, GLfloat f);

void r_glOrthofOES(void *context, GLfloat l, GLfloat r, GLfloat b, GLfloat t,
                   GLfloat n, GLfloat f);
void d_glOrthofOES(void *context, GLfloat l, GLfloat r, GLfloat b, GLfloat t,
                   GLfloat n, GLfloat f);

void r_glRenderbufferStorageMultisampleEXT(void *context, GLenum target,
                                           GLsizei samples,
                                           GLenum internalformat, GLsizei width,
                                           GLsizei height);
void d_glRenderbufferStorageMultisampleEXT(void *context, GLenum target,
                                           GLsizei samples,
                                           GLenum internalformat, GLsizei width,
                                           GLsizei height);

void r_glUseProgramStages(void *context, GLuint pipeline, GLbitfield stages,
                          GLuint program);
void d_glUseProgramStages(void *context, GLuint pipeline, GLbitfield stages,
                          GLuint program);

void r_glActiveShaderProgram(void *context, GLuint pipeline, GLuint program);
void d_glActiveShaderProgram(void *context, GLuint pipeline, GLuint program);

void r_glProgramUniform1i(void *context, GLuint program, GLint location,
                          GLint v0);
void d_glProgramUniform1i(void *context, GLuint program, GLint location,
                          GLint v0);

void r_glProgramUniform2i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1);
void d_glProgramUniform2i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1);

void r_glProgramUniform3i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1, GLint v2);
void d_glProgramUniform3i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1, GLint v2);

void r_glProgramUniform4i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1, GLint v2, GLint v3);
void d_glProgramUniform4i(void *context, GLuint program, GLint location,
                          GLint v0, GLint v1, GLint v2, GLint v3);

void r_glProgramUniform1ui(void *context, GLuint program, GLint location,
                           GLuint v0);
void d_glProgramUniform1ui(void *context, GLuint program, GLint location,
                           GLuint v0);

void r_glProgramUniform2ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1);
void d_glProgramUniform2ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1);

void r_glProgramUniform3ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1, GLuint v2);
void d_glProgramUniform3ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1, GLuint v2);

void r_glProgramUniform4ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void d_glProgramUniform4ui(void *context, GLuint program, GLint location,
                           GLuint v0, GLuint v1, GLuint v2, GLuint v3);

void r_glProgramUniform1f(void *context, GLuint program, GLint location,
                          GLfloat v0);
void d_glProgramUniform1f(void *context, GLuint program, GLint location,
                          GLfloat v0);

void r_glProgramUniform2f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1);
void d_glProgramUniform2f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1);

void r_glProgramUniform3f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1, GLfloat v2);
void d_glProgramUniform3f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1, GLfloat v2);

void r_glProgramUniform4f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void d_glProgramUniform4f(void *context, GLuint program, GLint location,
                          GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

void r_glTransformFeedbackVaryings(void *context, GLuint program, GLsizei count,
                                   const GLchar *const *varyings,
                                   GLenum bufferMode);
void d_glTransformFeedbackVaryings(void *context, GLuint program, GLsizei count,
                                   const GLchar *const *varyings,
                                   GLenum bufferMode);

void r_glTexParameterfv(void *context, GLenum target, GLenum pname,
                        const GLfloat *params);
void d_glTexParameterfv(void *context, GLenum target, GLenum pname,
                        const GLfloat *params);

void r_glTexParameteriv(void *context, GLenum target, GLenum pname,
                        const GLint *params);
void d_glTexParameteriv(void *context, GLenum target, GLenum pname,
                        const GLint *params);

void r_glUniform1fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value);
void d_glUniform1fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value);

void r_glUniform1iv(void *context, GLint location, GLsizei count,
                    const GLint *value);
void d_glUniform1iv(void *context, GLint location, GLsizei count,
                    const GLint *value);

void r_glUniform2fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value);
void d_glUniform2fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value);

void r_glUniform2iv(void *context, GLint location, GLsizei count,
                    const GLint *value);
void d_glUniform2iv(void *context, GLint location, GLsizei count,
                    const GLint *value);

void r_glUniform3fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value);
void d_glUniform3fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value);

void r_glUniform3iv(void *context, GLint location, GLsizei count,
                    const GLint *value);
void d_glUniform3iv(void *context, GLint location, GLsizei count,
                    const GLint *value);

void r_glUniform4fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value);
void d_glUniform4fv(void *context, GLint location, GLsizei count,
                    const GLfloat *value);

void r_glUniform4iv(void *context, GLint location, GLsizei count,
                    const GLint *value);
void d_glUniform4iv(void *context, GLint location, GLsizei count,
                    const GLint *value);

void r_glVertexAttrib1fv(void *context, GLuint index, const GLfloat *v);
void d_glVertexAttrib1fv(void *context, GLuint index, const GLfloat *v);

void r_glVertexAttrib2fv(void *context, GLuint index, const GLfloat *v);
void d_glVertexAttrib2fv(void *context, GLuint index, const GLfloat *v);

void r_glVertexAttrib3fv(void *context, GLuint index, const GLfloat *v);
void d_glVertexAttrib3fv(void *context, GLuint index, const GLfloat *v);

void r_glVertexAttrib4fv(void *context, GLuint index, const GLfloat *v);
void d_glVertexAttrib4fv(void *context, GLuint index, const GLfloat *v);

void r_glUniformMatrix2fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix2fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value);

void r_glUniformMatrix3fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix3fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value);

void r_glUniformMatrix4fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix4fv(void *context, GLint location, GLsizei count,
                          GLboolean transpose, const GLfloat *value);

void r_glUniformMatrix2x3fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix2x3fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);

void r_glUniformMatrix3x2fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix3x2fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);

void r_glUniformMatrix2x4fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix2x4fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);

void r_glUniformMatrix4x2fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix4x2fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);

void r_glUniformMatrix3x4fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix3x4fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);

void r_glUniformMatrix4x3fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);
void d_glUniformMatrix4x3fv(void *context, GLint location, GLsizei count,
                            GLboolean transpose, const GLfloat *value);

void r_glVertexAttribI4iv(void *context, GLuint index, const GLint *v);
void d_glVertexAttribI4iv(void *context, GLuint index, const GLint *v);

void r_glVertexAttribI4uiv(void *context, GLuint index, const GLuint *v);
void d_glVertexAttribI4uiv(void *context, GLuint index, const GLuint *v);

void r_glUniform1uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value);
void d_glUniform1uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value);

void r_glUniform2uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value);
void d_glUniform2uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value);

void r_glUniform3uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value);
void d_glUniform3uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value);

void r_glUniform4uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value);
void d_glUniform4uiv(void *context, GLint location, GLsizei count,
                     const GLuint *value);

void r_glClearBufferiv(void *context, GLenum buffer, GLint drawbuffer,
                       const GLint *value);
void d_glClearBufferiv(void *context, GLenum buffer, GLint drawbuffer,
                       const GLint *value);

void r_glClearBufferuiv(void *context, GLenum buffer, GLint drawbuffer,
                        const GLuint *value);
void d_glClearBufferuiv(void *context, GLenum buffer, GLint drawbuffer,
                        const GLuint *value);

void r_glClearBufferfv(void *context, GLenum buffer, GLint drawbuffer,
                       const GLfloat *value);
void d_glClearBufferfv(void *context, GLenum buffer, GLint drawbuffer,
                       const GLfloat *value);

void r_glSamplerParameteriv(void *context, GLuint sampler, GLenum pname,
                            const GLint *param);
void d_glSamplerParameteriv(void *context, GLuint sampler, GLenum pname,
                            const GLint *param);

void r_glSamplerParameterfv(void *context, GLuint sampler, GLenum pname,
                            const GLfloat *param);
void d_glSamplerParameterfv(void *context, GLuint sampler, GLenum pname,
                            const GLfloat *param);

void r_glInvalidateFramebuffer(void *context, GLenum target,
                               GLsizei numAttachments,
                               const GLenum *attachments);
void d_glInvalidateFramebuffer(void *context, GLenum target,
                               GLsizei numAttachments,
                               const GLenum *attachments);

void r_glInvalidateSubFramebuffer(void *context, GLenum target,
                                  GLsizei numAttachments,
                                  const GLenum *attachments, GLint x, GLint y,
                                  GLsizei width, GLsizei height);
void d_glInvalidateSubFramebuffer(void *context, GLenum target,
                                  GLsizei numAttachments,
                                  const GLenum *attachments, GLint x, GLint y,
                                  GLsizei width, GLsizei height);

void r_glClipPlanexOES(void *context, GLenum plane, const GLfixed *equation);
void d_glClipPlanexOES(void *context, GLenum plane, const GLfixed *equation);

void r_glFogxvOES(void *context, GLenum pname, const GLfixed *param);
void d_glFogxvOES(void *context, GLenum pname, const GLfixed *param);

void r_glLightModelxvOES(void *context, GLenum pname, const GLfixed *param);
void d_glLightModelxvOES(void *context, GLenum pname, const GLfixed *param);

void r_glLightxvOES(void *context, GLenum light, GLenum pname,
                    const GLfixed *params);
void d_glLightxvOES(void *context, GLenum light, GLenum pname,
                    const GLfixed *params);

void r_glLoadMatrixxOES(void *context, const GLfixed *m);
void d_glLoadMatrixxOES(void *context, const GLfixed *m);

void r_glMaterialxvOES(void *context, GLenum face, GLenum pname,
                       const GLfixed *param);
void d_glMaterialxvOES(void *context, GLenum face, GLenum pname,
                       const GLfixed *param);

void r_glMultMatrixxOES(void *context, const GLfixed *m);
void d_glMultMatrixxOES(void *context, const GLfixed *m);

void r_glPointParameterxvOES(void *context, GLenum pname,
                             const GLfixed *params);
void d_glPointParameterxvOES(void *context, GLenum pname,
                             const GLfixed *params);

void r_glTexEnvxvOES(void *context, GLenum target, GLenum pname,
                     const GLfixed *params);
void d_glTexEnvxvOES(void *context, GLenum target, GLenum pname,
                     const GLfixed *params);

void r_glClipPlanefOES(void *context, GLenum plane, const GLfloat *equation);
void d_glClipPlanefOES(void *context, GLenum plane, const GLfloat *equation);

void r_glTexGenxvOES(void *context, GLenum coord, GLenum pname,
                     const GLfixed *params);
void d_glTexGenxvOES(void *context, GLenum coord, GLenum pname,
                     const GLfixed *params);

void r_glProgramUniform1iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value);
void d_glProgramUniform1iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value);

void r_glProgramUniform2iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value);
void d_glProgramUniform2iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value);

void r_glProgramUniform3iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value);
void d_glProgramUniform3iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value);

void r_glProgramUniform4iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value);
void d_glProgramUniform4iv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLint *value);

void r_glProgramUniform1uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value);
void d_glProgramUniform1uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value);

void r_glProgramUniform2uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value);
void d_glProgramUniform2uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value);

void r_glProgramUniform3uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value);
void d_glProgramUniform3uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value);

void r_glProgramUniform4uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value);
void d_glProgramUniform4uiv(void *context, GLuint program, GLint location,
                            GLsizei count, const GLuint *value);

void r_glProgramUniform1fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value);
void d_glProgramUniform1fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value);

void r_glProgramUniform2fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value);
void d_glProgramUniform2fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value);

void r_glProgramUniform3fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value);
void d_glProgramUniform3fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value);

void r_glProgramUniform4fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value);
void d_glProgramUniform4fv(void *context, GLuint program, GLint location,
                           GLsizei count, const GLfloat *value);

void r_glProgramUniformMatrix2fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value);
void d_glProgramUniformMatrix2fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value);

void r_glProgramUniformMatrix3fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value);
void d_glProgramUniformMatrix3fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value);

void r_glProgramUniformMatrix4fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value);
void d_glProgramUniformMatrix4fv(void *context, GLuint program, GLint location,
                                 GLsizei count, GLboolean transpose,
                                 const GLfloat *value);

void r_glProgramUniformMatrix2x3fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);
void d_glProgramUniformMatrix2x3fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);

void r_glProgramUniformMatrix3x2fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);
void d_glProgramUniformMatrix3x2fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);

void r_glProgramUniformMatrix2x4fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);
void d_glProgramUniformMatrix2x4fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);

void r_glProgramUniformMatrix4x2fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);
void d_glProgramUniformMatrix4x2fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);

void r_glProgramUniformMatrix3x4fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);
void d_glProgramUniformMatrix3x4fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);

void r_glProgramUniformMatrix4x3fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);
void d_glProgramUniformMatrix4x3fv(void *context, GLuint program,
                                   GLint location, GLsizei count,
                                   GLboolean transpose, const GLfloat *value);

void r_glBindAttribLocation(void *context, GLuint program, GLuint index,
                            const GLchar *name);
void d_glBindAttribLocation(void *context, GLuint program, GLuint index,
                            const GLchar *name);

void r_glTexEnvf(void *context, GLenum target, GLenum pname, GLfloat param);
void d_glTexEnvf(void *context, GLenum target, GLenum pname, GLfloat param);

void r_glTexEnvi(void *context, GLenum target, GLenum pname, GLint param);
void d_glTexEnvi(void *context, GLenum target, GLenum pname, GLint param);

void r_glTexEnvx(void *context, GLenum target, GLenum pname, GLfixed param);
void d_glTexEnvx(void *context, GLenum target, GLenum pname, GLfixed param);

void r_glTexParameterx(void *context, GLenum target, GLenum pname, GLint param);
void d_glTexParameterx(void *context, GLenum target, GLenum pname, GLint param);

void r_glShadeModel(void *context, GLenum mode);
void d_glShadeModel(void *context, GLenum mode);

void r_glDrawTexiOES(void *context, GLint x, GLint y, GLint z, GLint width,
                     GLint height, GLfloat left_x, GLfloat right_x,
                     GLfloat bottom_y, GLfloat top_y);
void d_glDrawTexiOES(void *context, GLint x, GLint y, GLint z, GLint width,
                     GLint height);

void r_glVertexAttribIPointer_without_bound(void *context, GLuint index,
                                            GLint size, GLenum type,
                                            GLsizei stride, GLuint offset,
                                            GLsizei length,
                                            const void *pointer);

void r_glVertexAttribPointer_without_bound(void *context, GLuint index,
                                           GLint size, GLenum type,
                                           GLboolean normalized, GLsizei stride,
                                           GLuint offset, GLuint length,
                                           const void *pointer);

void r_glDrawElements_without_bound(void *context, GLenum mode, GLsizei count,
                                    GLenum type, const void *indices);

void r_glDrawElementsInstanced_without_bound(void *context, GLenum mode,
                                             GLsizei count, GLenum type,
                                             const void *indices,
                                             GLsizei instancecount);

void r_glDrawRangeElements_without_bound(void *context, GLenum mode,
                                         GLuint start, GLuint end,
                                         GLsizei count, GLenum type,
                                         const void *indices);

void r_glFlushMappedBufferRange_special(void *context, GLenum target,
                                        GLintptr offset, GLsizeiptr length,
                                        const void *data);
void d_glFlushMappedBufferRange(void *context, GLenum target, GLintptr offset,
                                GLsizeiptr length);

void r_glBufferData_custom(void *context, GLenum target, GLsizeiptr size,
                           const void *data, GLenum usage);
void d_glBufferData(void *context, GLenum target, GLsizeiptr size,
                    const void *data, GLenum usage);

void r_glBufferSubData_custom(void *context, GLenum target, GLintptr offset,
                              GLsizeiptr size, const void *data);
void d_glBufferSubData(void *context, GLenum target, GLintptr offset,
                       GLsizeiptr size, const void *data);

void r_glCompressedTexImage2D_without_bound(void *context, GLenum target,
                                            GLint level, GLenum internalformat,
                                            GLsizei width, GLsizei height,
                                            GLint border, GLsizei imageSize,
                                            const void *data);

void r_glCompressedTexSubImage2D_without_bound(void *context, GLenum target,
                                               GLint level, GLint xoffset,
                                               GLint yoffset, GLsizei width,
                                               GLsizei height, GLenum format,
                                               GLsizei imageSize,
                                               const void *data);

void r_glCompressedTexImage3D_without_bound(void *context, GLenum target,
                                            GLint level, GLenum internalformat,
                                            GLsizei width, GLsizei height,
                                            GLsizei depth, GLint border,
                                            GLsizei imageSize,
                                            const void *data);

void r_glCompressedTexSubImage3D_without_bound(
    void *context, GLenum target, GLint level, GLint xoffset, GLint yoffset,
    GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format,
    GLsizei imageSize, const void *data);

void r_glTexImage2D_without_bound(void *context, GLenum target, GLint level,
                                  GLint internalformat, GLsizei width,
                                  GLsizei height, GLint border, GLenum format,
                                  GLenum type, GLint buf_len,
                                  const void *pixels);

void r_glTexImage3D_without_bound(void *context, GLenum target, GLint level,
                                  GLint internalformat, GLsizei width,
                                  GLsizei height, GLsizei depth, GLint border,
                                  GLenum format, GLenum type, GLint buf_len,
                                  const void *pixels);

void r_glTexSubImage2D_without_bound(void *context, GLenum target, GLint level,
                                     GLint xoffset, GLint yoffset,
                                     GLsizei width, GLsizei height,
                                     GLenum format, GLenum type, GLint buf_len,
                                     const void *pixels);

void r_glTexSubImage3D_without_bound(void *context, GLenum target, GLint level,
                                     GLint xoffset, GLint yoffset,
                                     GLint zoffset, GLsizei width,
                                     GLsizei height, GLsizei depth,
                                     GLenum format, GLenum type, GLint buf_len,
                                     const void *pixels);

void r_glPrintf(void *context, GLint buf_len, const GLchar *out_string);
void d_glPrintf(void *context, GLint buf_len, const GLchar *out_string);

void *d_glMapBufferRange(void *context, GLenum target, GLintptr offset,
                         GLsizeiptr length, GLbitfield access);
void d_glGetBufferPointerv(void *context, GLenum target, GLenum pname,
                           void **params);

void d_glTexImage2D(void *context, GLenum target, GLint level,
                    GLint internalformat, GLsizei width, GLsizei height,
                    GLint border, GLenum format, GLenum type,
                    const void *pixels);

void d_glTexSubImage2D(void *context, GLenum target, GLint level, GLint xoffset,
                       GLint yoffset, GLsizei width, GLsizei height,
                       GLenum format, GLenum type, const void *pixels);

void d_glTexImage3D(void *context, GLenum target, GLint level,
                    GLint internalformat, GLsizei width, GLsizei height,
                    GLsizei depth, GLint border, GLenum format, GLenum type,
                    const void *pixels);

void d_glTexSubImage3D(void *context, GLenum target, GLint level, GLint xoffset,
                       GLint yoffset, GLint zoffset, GLsizei width,
                       GLsizei height, GLsizei depth, GLenum format,
                       GLenum type, const void *pixels);

void d_glCompressedTexSubImage2D(void *context, GLenum target, GLint level,
                                 GLint xoffset, GLint yoffset, GLsizei width,
                                 GLsizei height, GLenum format,
                                 GLsizei imageSize, const void *data);

void d_glCompressedTexSubImage3D(void *context, GLenum target, GLint level,
                                 GLint xoffset, GLint yoffset, GLint zoffset,
                                 GLsizei width, GLsizei height, GLsizei depth,
                                 GLenum format, GLsizei imageSize,
                                 const void *data);

void d_glCompressedTexImage3D(void *context, GLenum target, GLint level,
                              GLenum internalformat, GLsizei width,
                              GLsizei height, GLsizei depth, GLint border,
                              GLsizei imageSize, const void *data);

void d_glCompressedTexImage2D(void *context, GLenum target, GLint level,
                              GLenum internalformat, GLsizei width,
                              GLsizei height, GLint border, GLsizei imageSize,
                              const void *data);

void d_glReadPixels(void *context, GLint x, GLint y, GLsizei width,
                    GLsizei height, GLenum format, GLenum type, void *pixels);
void d_glReadnPixelsEXT(void *context, GLint x, GLint y, GLsizei width,
                        GLsizei height, GLenum format, GLenum type,
                        GLsizei bufSize, void *data);

void d_glVertexAttribPointer(void *context, GLuint index, GLint size,
                             GLenum type, GLboolean normalized, GLsizei stride,
                             const void *pointer);

void d_glVertexAttribIPointer(void *context, GLuint index, GLint size,
                              GLenum type, GLsizei stride, const void *pointer);

void d_glGetVertexAttribPointerv(void *context, GLuint index, GLenum pname,
                                 void **pointer);

//
// void d_glDisableVertexAttribArray(void *context,GLuint index);
// void d_glEnableVertexAttribArray(void *context,GLuint index);
//
//
void d_glVertexAttribDivisor(void *context, GLuint index, GLuint divisor);

void d_glDrawElements(void *context, GLenum mode, GLsizei count, GLenum type,
                      const void *indices);

void d_glDrawRangeElements(void *context, GLenum mode, GLuint start, GLuint end,
                           GLsizei count, GLenum type, const void *indices);

void d_glDrawElementsInstanced(void *context, GLenum mode, GLsizei count,
                               GLenum type, const void *indices,
                               GLsizei instancecount);

void d_glGenVertexArraysOES(void *context, GLsizei n, GLuint *arrays);

void d_glBindVertexArrayOES(void *context, GLuint array);

void d_glDeleteVertexArraysOES(void *context, GLsizei n, const GLuint *arrays);

void r_glBindEGLImage(void *context, GLenum target, uint64_t gbuffer_id,
                      GLuint texture, GLuint share_texture,
                      EGLContext share_ctx);

void r_glGraphicBufferData(void *context, EGLContext ctx, uint64_t gbuffer_id,
                           int width, int height, int buf_len, int row_byte_len,
                           int stride, const void *real_buffer);

void r_glReadGraphicBuffer(void *context, EGLContext ctx, uint64_t gbuffer_id,
                           int width, int height, int buf_len, int row_byte_len,
                           int stride, void *real_buffer);

void r_glGetStaticValues(void *context, int static_values_len,
                         void *static_values);

void r_glGetProgramDataSize(void *context, GLuint program, int *data_len);

void r_glGetProgramData(void *context, GLuint program, int buf_len,
                        void *program_data);

void r_glSync(void *context);

void r_glBindImageTexture(void *context, GLuint unit, GLuint texture,
                          GLint level, GLboolean layered, GLint layer,
                          GLenum access, GLenum format);
void d_glBindImageTexture(void *context, GLuint unit, GLuint texture,
                          GLint level, GLboolean layered, GLint layer,
                          GLenum access, GLenum format);

void r_glBindVertexBuffer(void *context, GLuint bindingindex, GLuint buffer,
                          GLintptr offset, GLsizei stride);
void d_glBindVertexBuffer(void *context, GLuint bindingindex, GLuint buffer,
                          GLintptr offset, GLsizei stride);

void r_glVertexAttribFormat(void *context, GLuint attribindex, GLint size,
                            GLenum type, GLboolean normalized,
                            GLuint relativeoffset);
void d_glVertexAttribFormat(void *context, GLuint attribindex, GLint size,
                            GLenum type, GLboolean normalized,
                            GLuint relativeoffset);

void r_glVertexAttribIFormat(void *context, GLuint attribindex, GLint size,
                             GLenum type, GLuint relativeoffset);
void d_glVertexAttribIFormat(void *context, GLuint attribindex, GLint size,
                             GLenum type, GLuint relativeoffset);

void r_glVertexAttribBinding(void *context, GLuint attribindex,
                             GLuint bindingindex);
void d_glVertexAttribBinding(void *context, GLuint attribindex,
                             GLuint bindingindex);

void r_glDispatchCompute(void *context, GLuint num_groups_x,
                         GLuint num_groups_y, GLuint num_groups_z);
void d_glDispatchCompute(void *context, GLuint num_groups_x,
                         GLuint num_groups_y, GLuint num_groups_z);

void r_glDispatchComputeIndirect(void *context, GLintptr indirect);
void d_glDispatchComputeIndirect(void *context, GLintptr indirect);

void r_glMemoryBarrier(void *context, GLbitfield barriers);
void d_glMemoryBarrier(void *context, GLbitfield barriers);

void r_glMemoryBarrierByRegion(void *context, GLbitfield barriers);
void d_glMemoryBarrierByRegion(void *context, GLbitfield barriers);

void r_glFramebufferParameteri(void *context, GLenum target, GLenum pname,
                               GLint param);
void d_glFramebufferParameteri(void *context, GLenum target, GLenum pname,
                               GLint param);

void r_glSampleMaski(void *context, GLuint maskNumber, GLbitfield mask);
void d_glSampleMaski(void *context, GLuint maskNumber, GLbitfield mask);

void r_glTexStorage2DMultisample(void *context, GLenum target, GLsizei samples,
                                 GLenum internalformat, GLsizei width,
                                 GLsizei height,
                                 GLboolean fixedsamplelocations);
void d_glTexStorage2DMultisample(void *context, GLenum target, GLsizei samples,
                                 GLenum internalformat, GLsizei width,
                                 GLsizei height,
                                 GLboolean fixedsamplelocations);

void r_glValidateProgramPipeline(void *context, GLuint pipeline);
void d_glValidateProgramPipeline(void *context, GLuint pipeline);

void r_glVertexBindingDivisor(void *context, GLuint bindingindex,
                              GLuint divisor);
void d_glVertexBindingDivisor(void *context, GLuint bindingindex,
                              GLuint divisor);

void d_glDrawArraysIndirect(void *context, GLenum mode, const void *indirect);
void r_glDrawArraysIndirect_with_bound(void *context, GLenum mode,
                                       GLintptr indirect);
void r_glDrawArraysIndirect_without_bound(void *context, GLenum mode,
                                          const void *indirect);

void d_glDrawElementsIndirect(void *context, GLenum mode, GLenum type,
                              const void *indirect);
void r_glDrawElementsIndirect_with_bound(void *context, GLenum mode,
                                         GLenum type, GLintptr indirect);
void r_glDrawElementsIndirect_without_bound(void *context, GLenum mode,
                                            GLenum type, const void *indirect);

void d_glDiscardFramebufferEXT(void *context, GLenum target,
                               GLsizei numAttachments,
                               const GLenum *attachments);
void r_glDiscardFramebufferEXT(void *context, GLenum target,
                               GLsizei numAttachments,
                               const GLenum *attachments);

void d_glTexBuffer(void *context, GLenum target, GLenum internalformat,
                   GLuint buffer);
void d_glTexBufferRange(void *context, GLenum target, GLenum internalformat,
                        GLuint buffer, GLintptr offset, GLsizeiptr size);

void d_glTexBufferOES(void *context, GLenum target, GLenum internalformat,
                      GLuint buffer);
void d_glTexBufferRangeOES(void *context, GLenum target, GLenum internalformat,
                           GLuint buffer, GLintptr offset, GLsizeiptr size);

void d_glTexBufferEXT(void *context, GLenum target, GLenum internalformat,
                      GLuint buffer);
void d_glTexBufferRangeEXT(void *context, GLenum target, GLenum internalformat,
                           GLuint buffer, GLintptr offset, GLsizeiptr size);

void r_glTexBuffer(void *context, GLenum target, GLenum internalformat,
                   GLuint buffer);
void r_glTexBufferRange(void *context, GLenum target, GLenum internalformat,
                        GLuint buffer, GLintptr offset, GLsizeiptr size);

void d_glColorMaski(void *context, GLuint buf, GLboolean red, GLboolean green,
                    GLboolean blue, GLboolean alpha);
void d_glColorMaskiEXT(void *context, GLuint buf, GLboolean red,
                       GLboolean green, GLboolean blue, GLboolean alpha);
void d_glColorMaskiOES(void *context, GLuint buf, GLboolean red,
                       GLboolean green, GLboolean blue, GLboolean alpha);

void r_glColorMaski(void *context, GLuint buf, GLboolean red, GLboolean green,
                    GLboolean blue, GLboolean alpha);

void d_glBlendFuncSeparatei(void *context, GLuint buf, GLenum srcRGB,
                            GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void d_glBlendFuncSeparateiEXT(void *context, GLuint buf, GLenum srcRGB,
                               GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void d_glBlendFuncSeparateiOES(void *context, GLuint buf, GLenum srcRGB,
                               GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

void r_glBlendFuncSeparatei(void *context, GLuint buf, GLenum srcRGB,
                            GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

void d_glBlendEquationSeparatei(void *context, GLuint buf, GLenum modeRGB,
                                GLenum modeAlpha);
void d_glBlendEquationSeparateiEXT(void *context, GLuint buf, GLenum modeRGB,
                                   GLenum modeAlpha);
void d_glBlendEquationSeparateiOES(void *context, GLuint buf, GLenum modeRGB,
                                   GLenum modeAlpha);

void r_glBlendEquationSeparatei(void *context, GLuint buf, GLenum modeRGB,
                                GLenum modeAlpha);

// void r_glBindSharedGLImage(void *context, GLenum target, GLint texture,
// EGLContext share_context);

// void r_glFramebufferSharedTexture2D(void *context, GLenum target, GLenum
// attachment, GLenum textarget, GLuint texture, GLint level, EGLContext
// share_context);

// void r_glFramebufferEGLImage(void *context, GLenum target, GLenum attachment,
// GLenum textarget, uint64_t image, GLint level);

#endif // GLDEF_H
