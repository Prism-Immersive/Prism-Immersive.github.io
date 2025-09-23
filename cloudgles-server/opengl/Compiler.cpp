
#ifdef USE_LOCAL_COMPILER
#define LOG_TAG "GLSLCompiler"
#include "EGL/eglplatform.h"
#include <cstdint>

#include <dirent.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vndksupport/linker.h>

#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>
#include <unordered_map>

#include "Compiler.h"
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <EGL/eglext_angle.h>
#include "Log.h"
#include "ThreadContext.h"

static char const *const mock_egl_api_names[] = {
    LIST_MOCK_EGL_ENTRIES(EGL_NAME) nullptr};

static char const *const mock_gl_api_names[] = {
    LIST_MOCK_GLv2_ENTRIES(GL_NAME) nullptr};

GLSLCompiler::GLSLCompiler(GLSLCompiler *shared_compiler) {
    char *env = getenv("LIBGL_ALWAYS_SOFTWARE");
    ALOGE("LIBGL_ALWAYS_SOFTWARE is %s", env);
    attempt_to_load_drivers();

    MockCtx *shared_ctx =
        shared_compiler ? &shared_compiler->mock_ctx : nullptr;
    mock_ctx.setup(this, shared_ctx);

    if (mock_ctx.is_initialized()) {
        initialized = true;
    }
}

GLSLCompiler::~GLSLCompiler() {
    if (dso[EGL] != nullptr) {
        dlclose(dso[EGL]);
    }

    if (dso[GLESv2] != nullptr) {
        dlclose(dso[GLESv2]);
    }
}

void GLSLCompiler::attempt_to_load_drivers() {
    dso[EGL] = load_driver("EGL");
    if (dso[EGL]) {
        get_proc_address =
            (GetProcAddressType)dlsym(dso[EGL], "eglGetProcAddress");
        ALOGE_IF(!get_proc_address,
                 "can't find eglGetProcAddress() in EGL driver library");
        initialize_api(EGL);

        dso[GLESv1_CM] = load_driver("GLESv1_CM");
        dso[GLESv2] = load_driver("GLESv2");
        if (dso[GLESv2]) {
            initialize_api(GLESv2);
        }
    }
}

void *GLSLCompiler::load_driver(const char *kind) {
    const char *const search_path =
#if defined(__LP64__)
        "/vendor/lib64/egl";
#else
        "/vendor/lib/egl";
#endif
    std::string driver_absolute_path = std::string(search_path) + "/lib" +
                                       std::string(kind) + "_" +
                                       std::string(suffix) + ".so";
    if (access(driver_absolute_path.c_str(), F_OK) < 0) {
        ALOGE("driver %s not found, error is %s", driver_absolute_path.c_str(),
              strerror(errno));
    }

    void *dso = android_load_sphal_library(driver_absolute_path.c_str(),
                                           RTLD_NOW | RTLD_LOCAL);
    if (dso == nullptr) {
        ALOGE("load %s failed %s", driver_absolute_path.c_str(), dlerror());
    }

    return dso;
}

void GLSLCompiler::initialize_api(uint32_t mask) {
    __eglMustCastToProperFunctionPointerType *curr = nullptr;
    char const *const *api = nullptr;
    void *dso_handle = nullptr;
    switch (mask) {
        case EGL:
            curr = (__eglMustCastToProperFunctionPointerType *)&egl;
            api = mock_egl_api_names;
            dso_handle = dso[EGL];
            break;
        case GLESv2:
            curr = (__eglMustCastToProperFunctionPointerType *)&glv2;
            api = mock_gl_api_names;
            dso_handle = dso[GLESv2];
            break;
        default:
            ALOGE("invalid mask %d", mask);
            return;
    }

    while (*api) {
        char const *name = *api;
        __eglMustCastToProperFunctionPointerType f =
            (__eglMustCastToProperFunctionPointerType)dlsym(dso_handle, name);
        if (f == nullptr) {
            // couldn't find the entry-point, use eglGetProcAddress()
            f = get_proc_address(name);
            if (f == nullptr) {
                f = (__eglMustCastToProperFunctionPointerType) nullptr;
                ALOGE("GLSLCompiler couldn't load %s", name);
            }
        }
        *curr++ = f;
        api++;
    }
}

void GLSLCompiler::MockCtx::setup(GLSLCompiler *compiler,
                                  GLSLCompiler::MockCtx *shared_context) {
    EGLint major_ver;
    EGLint minor_ver;
    EGLint ctx_attrs[] = {EGL_CONTEXT_MAJOR_VERSION, 3,
                          EGL_CONTEXT_MINOR_VERSION, 1, EGL_NONE};
    display = compiler->egl.eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (!compiler->egl.eglInitialize(display, &major_ver, &minor_ver)) {
        ALOGE("%s eglInitialize failed", __FUNCTION__);
        return;
    }
    EGLint num_config;
    EGLint attrs[] = {EGL_RED_SIZE,
                      8,
                      EGL_GREEN_SIZE,
                      8,
                      EGL_BLUE_SIZE,
                      8,
                      EGL_ALPHA_SIZE,
                      8,
                      EGL_DEPTH_SIZE,
                      24,
                      EGL_STENCIL_SIZE,
                      8,
                      EGL_SAMPLE_BUFFERS,
                      1,
                      EGL_SAMPLES,
                      4,
                      EGL_RENDERABLE_TYPE,
                      EGL_OPENGL_ES3_BIT_KHR,
                      EGL_NONE};

    if (!compiler->egl.eglChooseConfig(display, attrs, &config, 1,
                                       &num_config)) {
        ALOGE("%s eglChooseConfig failed", __FUNCTION__);
        return;
    }

    EGLContext shared =
        shared_context ? shared_context->context : EGL_NO_CONTEXT;
    context =
        compiler->egl.eglCreateContext(display, config, shared, ctx_attrs);
    surface = compiler->egl.eglCreatePbufferSurface(display, config, nullptr);

    if (compiler->egl.eglGetError() != EGL_SUCCESS) {
        ALOGE("setup mock context failed");
    } else {
        initialized = true;
    }
}

void GLSLCompiler::MockCtx::make_current(GLSLCompiler *compiler, EGLBoolean is_current) {
    if (is_current) {
        if (!compiler->egl.eglMakeCurrent(display, surface, surface, context)) {
            ALOGE("%s: Mock_Ctx eglMakeCurrent failed, error is %x", __FUNCTION__, compiler->egl.eglGetError());
        }
    } else {
        if (!compiler->egl.eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                                         EGL_NO_CONTEXT)) {
            ALOGE("%s: Mock_Ctx eglMakeCurrent with NULL context failed, error is %x", __FUNCTION__, compiler->egl.eglGetError());
        }
    
    }
}

void GLSLCompiler::MockCtx::destroy(GLSLCompiler *compiler) {
    if (compiler->egl.eglGetCurrentContext() != EGL_NO_CONTEXT) {
        compiler->egl.eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE,
                                     EGL_NO_CONTEXT);
        compiler->egl.eglDestroySurface(display, surface);
        compiler->egl.eglDestroyContext(display, context);
        compiler->egl.eglTerminate(display);
    }
}

GLuint GLSLCompiler::get_mock_program_id(GLuint program_id) {
    GLThreadContext *opengl_context =
        ((ThreadContext *)egl_thread_context)->opengl_context;

    if (opengl_context->program_info_map->find(program_id) ==
        opengl_context->program_info_map->end()) {
        ALOGE("%s: Cannot find program %d in program map", __FUNCTION__,
              program_id);
        return 0;
    }

    GLSLProgram *glsl_program = (*opengl_context->program_info_map)[program_id];
    if (!glsl_program->mock_program_id) {
        ALOGE("%s: Cannot find mock program id for program %d", __FUNCTION__,
              program_id);
        return 0;
    }

    return glsl_program->mock_program_id;
}

GLuint GLSLCompiler::get_mock_shader_id(GLuint shader_id) {
    GLThreadContext *opengl_context =
        ((ThreadContext *)egl_thread_context)->opengl_context;

    if (opengl_context->shader_map->find(shader_id) ==
        opengl_context->shader_map->end()) {
        ALOGE("%s: Cannot find shader %d in shader map", __FUNCTION__,
              shader_id);
        return 0;
    }
    GLSLShader *glsl_shader = (*opengl_context->shader_map)[shader_id].get();
    if (!glsl_shader->mock_shader_id) {
        ALOGE("%s: Cannot find mock shader id for shader %d", __FUNCTION__,
              shader_id);
        return 0;
    }
    return glsl_shader->mock_shader_id;
}

/*---------------------------------------API----------------------------------------------------------------*/

void GLSLCompiler::make_current(EGLBoolean is_current) {
    mock_ctx.make_current(this, is_current);
}

GLuint GLSLCompiler::create_program() { return glv2.glCreateProgram(); }

GLuint GLSLCompiler::create_shader(GLenum type) {
    return glv2.glCreateShader(type);
}

GLuint GLSLCompiler::create_shader_programv(GLenum type, GLsizei count,
                                            const GLchar *const *strings) {
    return glv2.glCreateShaderProgramv(type, count, strings);
}

void GLSLCompiler::shader_source(GLuint shader, GLsizei count,
                                 const GLchar *const *string,
                                 const GLint *length) {
    GLuint mock_shader_id = get_mock_shader_id(shader);
    if (mock_shader_id == 0) {
        ALOGE("%s: Cannot get mock shader ID for program %d", __FUNCTION__, shader);
        return;
    }

    glv2.glShaderSource(mock_shader_id, count, string, length);
}

void GLSLCompiler::compile_shader(GLuint shader) {
    GLuint mock_shader_id = get_mock_shader_id(shader);
    if (mock_shader_id == 0) {
        ALOGE("%s: Cannot get mock shader ID for program %d", __FUNCTION__, shader);
        return;
    }

    glv2.glCompileShader(mock_shader_id);
    GLint compiled;
    glv2.glGetShaderiv(mock_shader_id, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint maxLength = 0;
        glv2.glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        auto errorLog = std::unique_ptr<char>((char *)calloc(maxLength, 1));
        glv2.glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.get());
        ALOGE("glCompileShader failed %s", errorLog.get());
    }
}

void GLSLCompiler::attach_shader(GLuint program, GLuint shader) {
    GLuint mock_program_id = get_mock_program_id(program);
    GLuint mock_shader_id = get_mock_shader_id(shader);

    if (mock_program_id == 0 || mock_shader_id == 0) {
        ALOGE("%s: Cannot get mock program or shader ID for program %d or shader %d", __FUNCTION__, program, shader);
        return;
    }

    glv2.glAttachShader(mock_program_id, mock_shader_id);
}

void GLSLCompiler::link_program(GLuint program) {
    GLuint mock_program_id = get_mock_program_id(program);
    if (mock_program_id == 0) {
        ALOGE("%s: Cannot get mock program ID for program %d", __FUNCTION__, program);
        return;
    }

    glv2.glLinkProgram(mock_program_id);
    GLint linked;
    glv2.glGetProgramiv(mock_program_id, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint maxLength = 0;
        glv2.glGetProgramiv(mock_program_id, GL_INFO_LOG_LENGTH, &maxLength);
        auto errorLog = std::unique_ptr<char>((char *)calloc(maxLength, 1));
        glv2.glGetShaderInfoLog(mock_program_id, maxLength, &maxLength,
                                errorLog.get());
        ALOGE("glCompileShader failed %s", errorLog.get());
    }
}

void GLSLCompiler::program_binary(GLuint program, GLenum binaryFormat,
                                  const void *binary, GLsizei length, GLenum *realBinaryFormat, std::string *realBinary, uint64_t *realLength) {
    GLuint mock_program_id = get_mock_program_id(program);
    if (mock_program_id == 0) {
        ALOGE("%s: Cannot get mock program ID for program %d", __FUNCTION__, program);
        return;
    }

    glv2.glProgramBinary(mock_program_id, binaryFormat, binary, length);

    if (GLenum error = glv2.glGetError(); error != GL_NO_ERROR) {
    if (GLenum error = glv2.glGetError(); error != GL_NO_ERROR) {
        ALOGE("%s: glProgramBinary failed %x", __FUNCTION__, error);
    }

    main_program_cache->get_program(binaryFormat, reinterpret_cast<const GLchar *>(binary), length, realBinaryFormat, realBinary, realLength);
}

void GLSLCompiler::get_program_binary(GLuint program, GLsizei bufSize,
                                      GLsizei *length, GLenum *binaryFormat,
                                      void *binary, GLenum realBinaryFormat, void *realBinary, GLsizei realLength) {
    GLuint mock_program_id = get_mock_program_id(program);

    if (mock_program_id == 0) {
        ALOGE("%s: Cannot get mock program ID for program %d", __FUNCTION__, program);
        return;
    }

    GLsizei mock_binary_size;
    glv2.glGetProgramBinary(mock_program_id, bufSize, &mock_binary_size, binaryFormat, binary);
    if (length != nullptr) {
        *length = mock_binary_size;
    }

    if (GLenum error = glv2.glGetError(); error != GL_NO_ERROR) {
    if (GLenum error = glv2.glGetError(); error != GL_NO_ERROR) {
        ALOGE("%s: glGetProgramBinary failed %x", __FUNCTION__, error);
        return;
    }

    main_program_cache->put_program(*binaryFormat, reinterpret_cast<const GLchar *>(binary), mock_binary_size, realBinaryFormat, reinterpret_cast<const GLchar *>(realBinary), realLength);
}

void GLSLCompiler::use_program(GLuint program) {
    GLuint mock_program_id = get_mock_program_id(program);
    if (mock_program_id == 0) {
        ALOGE("%s: Cannot get mock program ID for program %d", __FUNCTION__, program);
        return;
    }

    glv2.glUseProgram(mock_program_id);
}

void GLSLCompiler::detach_shader(GLuint program, GLuint shader) {
    GLuint mock_program_id = get_mock_program_id(program);
    GLuint mock_shader_id = get_mock_shader_id(shader);

    if (mock_program_id == 0 || mock_shader_id == 0) {
        ALOGE("%s: Cannot get mock program or shader ID for program %d or shader %d", __FUNCTION__, program, shader);
        return;
    }

    glv2.glDetachShader(mock_program_id, mock_shader_id);
}

void GLSLCompiler::delete_shader(GLuint mock_shader_id) {
    glv2.glDeleteShader(mock_shader_id);
}

void GLSLCompiler::delete_program(GLuint mock_program_id) {
    glv2.glDeleteProgram(mock_program_id);
}

void GLSLCompiler::get_shaderiv(GLuint shader, GLenum pname, GLint *params) {
    GLuint mock_shader_id = get_mock_shader_id(shader);
    if (mock_shader_id == 0) {
        ALOGE("%s: Cannot get mock shader ID for program %d", __FUNCTION__, shader);
        return;
    }

    glv2.glGetShaderiv(mock_shader_id, pname, params);
}

void GLSLCompiler::get_programiv(GLuint program, GLenum pname, GLint *params) {
    GLuint mock_program_id = get_mock_program_id(program);
    if (mock_program_id == 0) {
        ALOGE("%s: Cannot get mock program ID for program %d", __FUNCTION__, program);
        return;
    }

    glv2.glGetProgramiv(mock_program_id, pname, params);
}

void GLSLCompiler::bind_attrib_location(GLuint program, GLuint index, const GLchar *name) {
    GLuint mock_program_id = get_mock_program_id(program);
    if (mock_program_id == 0) {
        ALOGE("%s: Cannot get mock program ID for program %d", __FUNCTION__, program);
        return;
    }

    glv2.glBindAttribLocation(mock_program_id, index, name);

}

void GLSLCompiler::get_active_uniformsiv(GLuint program, GLsizei uniformCount,
                                         const GLuint *uniformIndices,
                                         GLenum pname, GLint *params) {
    GLuint mock_program_id = get_mock_program_id(program);
    if (mock_program_id == 0) return;

    glv2.glGetActiveUniformsiv(mock_program_id, uniformCount, uniformIndices,
                               pname, params);
}

void GLSLCompiler::get_active_uniform(GLuint program, GLuint index,
                                      GLsizei bufSize, GLsizei *length,
                                      GLint *size, GLenum *type, GLchar *name) {
    GLuint mock_program_id = get_mock_program_id(program);
    if (mock_program_id == 0) return;

    glv2.glGetActiveUniform(mock_program_id, index, bufSize, length, size, type,
                            name);
}

GLint GLSLCompiler::get_uniform_location(GLuint program, const GLchar *name) {
    GLuint mock_program_id = get_mock_program_id(program);
    if (mock_program_id == 0) return -1;

    return glv2.glGetUniformLocation(mock_program_id, name);
}
#endif // USE_LOCAL_COMPILER