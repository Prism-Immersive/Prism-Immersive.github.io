#define LOG_TAG "GLCloud"
#define LOG_LEVEL LOG_LEVEL_ERROR
#define ATRACE_TAG ATRACE_TAG_GRAPHICS

#include <GLES3/gl32.h>

#include "ExtFunc.h"
#include "GLDef.h"
#include "GLFuncList.h"
#include "Log.h"
#include "ThreadContext.h"

#define REMOVE_CTX_VARIABLE(ctx, ...) __VA_ARGS__

extern "C" {
#define DEFINE_IMPL_GL_FUNCTIONS(return_type, func_name, param,               \
                                 default_return, ...)                         \
    return_type func_name param {                                             \
        ATRACE_CALL();                                                        \
        if (unlikely(!main_loader->is_cloud_rendering()))                     \
            return main_loader->gl.func_name(                                 \
                REMOVE_CTX_VARIABLE(__VA_ARGS__));                            \
        if (unlikely(egl_thread_context == NULL)) default_return;             \
        char thread_name[16];                                                 \
        pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name)); \
        ALOGD("%s %s: %s has been called!",                                   \
              egl_default_display->get_proc_name().c_str(), thread_name,      \
              #func_name);                                                    \
        void *thread_ctx =                                                    \
            ((ThreadContext *)egl_thread_context)->opengl_context;            \
        if (unlikely(thread_ctx == NULL)) default_return;                     \
        return d##_##func_name(__VA_ARGS__);                                  \
    }
LIST_IMPL_GL_FUNCTIONS(DEFINE_IMPL_GL_FUNCTIONS);

#define DEFINE_NOT_IMPL_GL_FUNCTIONS(return_type, func_name, param, \
                                     default_return, ...)           \
    return_type func_name param {                                   \
        if (unlikely(!main_loader->is_cloud_rendering()))           \
            return main_loader->gl.func_name(                       \
                REMOVE_CTX_VARIABLE(__VA_ARGS__));                  \
        ALOGE("%s hasn't been implemented!", #func_name);           \
        default_return;                                             \
    }
LIST_NOT_IMPL_GL_FUNCTIONS(DEFINE_NOT_IMPL_GL_FUNCTIONS)

const GLEXTFunc gl_ext_func_table[] = {
#define DEFINE_GL_EXT_FUNCTIONS(return_type, func_name, param, default_return, \
                                ...)                                           \
    {#func_name, (void *)func_name},
    LIST_GL_EXT_FUNCTIONS(DEFINE_GL_EXT_FUNCTIONS)};

const int GL_EXT_FUNC_TABLE_SIZE =
    sizeof(gl_ext_func_table) / sizeof(GLEXTFunc);
}