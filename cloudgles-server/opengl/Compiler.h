#ifdef USE_LOCAL_COMPILER
#ifndef COMPILER_H
#define COMPILER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <leveldb/db.h>

#include <cstdint>

#include "data/ProgramCache.h"

#define GL_ENTRY(_r, _api, ...) _r (*(_api))(__VA_ARGS__);
#define GL_NAME(_r, _api, ...) #_api,
#define EGL_ENTRY(_r, _api, ...) _r (*(_api))(__VA_ARGS__);
#define EGL_NAME(_r, _api, ...) #_api,

#define LIST_MOCK_EGL_ENTRIES(X)                                            \
    X(EGLDisplay, eglGetPlatformDisplay, EGLenum, EGLNativeDisplayType,     \
      const EGLAttrib *)                                                    \
    X(EGLDisplay, eglGetDisplay, NativeDisplayType)                         \
    X(EGLBoolean, eglInitialize, EGLDisplay, EGLint *, EGLint *)            \
    X(EGLBoolean, eglTerminate, EGLDisplay)                                 \
    X(EGLBoolean, eglGetConfigs, EGLDisplay, EGLConfig *, EGLint, EGLint *) \
    X(EGLBoolean, eglChooseConfig, EGLDisplay, const EGLint *, EGLConfig *, \
      EGLint, EGLint *)                                                     \
    X(EGLSurface, eglCreatePbufferSurface, EGLDisplay, EGLConfig,           \
      const EGLint *)                                                       \
    X(EGLBoolean, eglDestroySurface, EGLDisplay, EGLSurface)                \
    X(EGLContext, eglCreateContext, EGLDisplay, EGLConfig, EGLContext,      \
      const EGLint *)                                                       \
    X(EGLBoolean, eglDestroyContext, EGLDisplay, EGLContext)                \
    X(EGLBoolean, eglMakeCurrent, EGLDisplay, EGLSurface, EGLSurface,       \
      EGLContext)                                                           \
    X(EGLContext, eglGetCurrentContext, void)                               \
    X(EGLint, eglGetError, void)                                            \
    X(__eglMustCastToProperFunctionPointerType, eglGetProcAddress, const char *)

#define LIST_MOCK_GLv2_ENTRIES(X)                                              \
    X(void, glGetIntegerv, GLenum pname, GLint *data)                          \
    X(const GLubyte *, glGetString, GLenum name)                               \
    X(const GLubyte *, glGetStringi, GLenum name, GLuint index)                \
    X(GLuint, glCreateProgram, void)                                           \
    X(GLuint, glCreateShader, GLenum type)                                     \
    X(GLuint, glCreateShaderProgramv, GLenum type, GLsizei count,              \
      const GLchar *const *strings)                                            \
    X(void, glShaderSource, GLuint shader, GLsizei count,                      \
      const GLchar *const *string, const GLint *length)                        \
    X(void, glCompileShader, GLuint shader)                                    \
    X(void, glAttachShader, GLuint program, GLuint shader)                     \
    X(void, glLinkProgram, GLuint program)                                     \
    X(void, glProgramBinary, GLuint program, GLenum binaryFormat,              \
      const void *binary, GLsizei length)                                      \
    X(void, glGetProgramBinary, GLuint program, GLsizei bufSize,               \
      GLsizei *length, GLenum *binaryFormat, void *binary)                     \
    X(void, glUseProgram, GLuint program)                                      \
    X(void, glDetachShader, GLuint program, GLuint shader)                     \
    X(void, glDeleteShader, GLuint shader)                                     \
    X(void, glDeleteProgram, GLuint program)                                   \
    X(void, glGetAttachedShaders, GLuint program, GLsizei maxCount,            \
      GLsizei *count, GLuint *shaders)                                         \
    X(void, glGetShaderiv, GLuint shader, GLenum pname, GLint *params)         \
    X(void, glGetProgramiv, GLuint program, GLenum pname, GLint *params)       \
    X(void, glGetShaderInfoLog, GLuint shader, GLsizei bufSize,                \
      GLsizei *length, GLchar *infoLog)                                        \
    X(void, glGetProgramInfoLog, GLuint program, GLsizei bufSize,              \
      GLsizei *length, GLchar *infoLog)                                        \
    X(void, glBindAttribLocation, GLuint program, GLuint index,                \
      const GLchar *name)                                                      \
    X(void, glGetActiveAttrib, GLuint program, GLuint index, GLsizei bufSize,  \
      GLsizei *length, GLint *size, GLenum *type, GLchar *name)                \
    X(GLint, glGetAttribLocation, GLuint program, const GLchar *name)          \
    X(GLint, glGetUniformLocation, GLuint program, const GLchar *name)         \
    X(void, glGetActiveUniformsiv, GLuint program, GLsizei uniformCount,       \
      const GLuint *uniformIndices, GLenum pname, GLint *params)               \
    X(void, glGetActiveUniform, GLuint program, GLuint index, GLsizei bufSize, \
      GLsizei *length, GLint *size, GLenum *type, GLchar *name)                \
    X(void, glGetActiveUniformBlockiv, GLuint program,                         \
      GLuint uniformBlockIndex, GLenum pname, GLint *params)                   \
    X(void, glGetActiveUniformBlockName, GLuint program,                       \
      GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length,              \
      GLchar *uniformBlockName)                                                \
    X(GLenum, glGetError, void)

struct MockEglAPI {
    LIST_MOCK_EGL_ENTRIES(EGL_ENTRY)
};

struct MockGlv2API {
    LIST_MOCK_GLv2_ENTRIES(GL_ENTRY)
};

class GLSLCompiler {
   public:
    explicit GLSLCompiler(GLSLCompiler *shared_compiler);
    GLSLCompiler() = delete;
    ~GLSLCompiler();

    MockGlv2API glv2;

    void make_current(EGLBoolean is_current);
    GLuint create_program();
    GLuint create_shader(GLenum type);
    GLuint create_shader_programv(GLenum type, GLsizei count,
                                  const GLchar *const *strings);
    void shader_source(GLuint shader, GLsizei count,
                       const GLchar *const *string, const GLint *length);
    void compile_shader(GLuint shader);
    void attach_shader(GLuint program, GLuint shader);
    void link_program(GLuint program);
    void program_binary(GLuint program, GLenum binaryFormat, const void *binary,
                        GLsizei length, GLenum *realBinaryFormat,
                        std::string *realBinary, uint64_t *realLength);
    void get_program_binary(GLuint program, GLsizei bufSize, GLsizei *length,
                            GLenum *binaryFormat, void *binary,
                            GLenum realBinaryFormat, void *realBinary,
                            GLsizei realLength);
    void use_program(GLuint program);
    void detach_shader(GLuint program, GLuint shader);
    void delete_shader(GLuint mock_shader_id);
    void delete_program(GLuint mock_program_id);
    void get_shaderiv(GLuint shader, GLenum pname, GLint *params);
    void get_programiv(GLuint program, GLenum pname, GLint *params);
    void bind_attrib_location(GLuint program, GLuint index, const GLchar *name);
    void get_active_uniformsiv(GLuint program, GLsizei uniformCount,
                               const GLuint *uniformIndices, GLenum pname,
                               GLint *params);
    void get_active_uniform(GLuint program, GLuint index, GLsizei bufSize,
                            GLsizei *length, GLint *size, GLenum *type,
                            GLchar *name);
    GLint get_uniform_location(GLuint program, const GLchar *name);

   private:
    enum {
        EGL = 0x00,
        // It's highly unlikely we'll use GLv1
        GLESv1_CM = 0x01,
        GLESv2 = 0x02,
    };

    class MockCtx {
       public:
        void setup(GLSLCompiler *compiler,
                   GLSLCompiler::MockCtx *shared_context);
        void make_current(GLSLCompiler *compiler, EGLBoolean is_current);
        void destroy(GLSLCompiler *compiler);
        bool is_initialized() const { return initialized; }

       private:
        EGLConfig config = 0;
        EGLDisplay display = EGL_NO_DISPLAY;
        EGLSurface surface = EGL_NO_SURFACE;
        EGLContext context = EGL_NO_CONTEXT;
        bool initialized = false;
    };

    typedef __eglMustCastToProperFunctionPointerType (*GetProcAddressType)(
        const char *);

    void attempt_to_load_drivers();
    void *load_driver(const char *kind);
    void initialize_api(uint32_t mask);
    GLuint get_mock_program_id(GLuint program_id);
    GLuint get_mock_shader_id(GLuint shader_id);

    void *dso[3] = {nullptr, nullptr, nullptr};

    static constexpr char suffix[] = "angle";

    bool initialized = false;

    MockCtx mock_ctx;
    MockEglAPI egl;
    GetProcAddressType get_proc_address;
};

#endif  // COMPILER_H
#endif  // USE_LOCAL_COMPILER