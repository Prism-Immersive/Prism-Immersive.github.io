#define LOG_TAG "EGLCloud"
#define LOG_LEVEL LOG_LEVEL_ERROR
#define ATRACE_TAG ATRACE_TAG_GRAPHICS

#include "EglDef.h"
#include "EglFunc.h"
#include "EglFuncList.h"
#include "Log.h"
#include "ThreadContext.h"

#define REMOVE_CTX_VARIABLE(ctx, ...) __VA_ARGS__

extern "C" {
#define DEFINE_IMPL_EGL_FUNCTIONS(return_type, func_name, param,              \
                                  default_return, ...)                        \
    return_type func_name param {                                             \
        /*const char *proc_name =                                             \
         * egl_default_display->get_proc_name().c_str(); */                   \
        ATRACE_CALL();                                                        \
        if (unlikely(!main_loader->is_cloud_rendering())) {                   \
            if (likely(main_loader->egl.func_name != nullptr)) {              \
                return main_loader->egl.func_name(                            \
                    REMOVE_CTX_VARIABLE(__VA_ARGS__));                        \
            } else {                                                          \
                void *thread_ctx = nullptr;                                   \
                return d##_##func_name(__VA_ARGS__);                          \
            }                                                                 \
        }                                                                     \
        if (unlikely(egl_thread_context == nullptr) ||                        \
            unlikely(my_pid != getpid())) {                                   \
            egl_thread_context = thread_context_init();                       \
        };                                                                    \
        char thread_name[16];                                                 \
        pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name)); \
        ALOGD("%s %s: %s has been called!",                                   \
              egl_default_display->get_proc_name().c_str(), thread_name,      \
              #func_name);                                                    \
        void *thread_ctx = egl_thread_context;                                \
        if (unlikely(thread_ctx == NULL)) default_return;                     \
        return d##_##func_name(__VA_ARGS__);                                  \
    }
LIST_IMPL_EGL_FUNCTIONS(DEFINE_IMPL_EGL_FUNCTIONS);

#define DEFINE_NOT_IMPL_EGL_FUNCTIONS(return_type, func_name, param, \
                                      default_return, ...)           \
    return_type func_name param {                                    \
        if (unlikely(!main_loader->is_cloud_rendering()))            \
            return main_loader->egl.func_name(                       \
                REMOVE_CTX_VARIABLE(__VA_ARGS__));                   \
        ALOGE("%s hasn't been implemented!", #func_name);            \
        default_return;                                              \
    }
LIST_NOT_IMPL_EGL_FUNCTIONS(DEFINE_NOT_IMPL_EGL_FUNCTIONS)

const Egl_Func egl_func_table[] = {
#define DEFINE_EGL_FUNC_ITEM(return_type, func_name, param, default_return, \
                             ...)                                           \
    {#func_name, (void *)func_name},
    LIST_IMPL_EGL_FUNCTIONS(DEFINE_EGL_FUNC_ITEM)};

const int EGL_FUNC_TABLE_SIZE = sizeof(egl_func_table) / sizeof(Egl_Func);
}