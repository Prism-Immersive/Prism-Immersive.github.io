#ifndef LOADER_H
#define LOADER_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <sys/types.h>
#include <string>

#include "GLFuncList.h"
#include "egl/EglFuncList.h"

#define API_ENTRY(return_type, func_name, param, default_return, ...) \
    return_type(*(func_name)) param;
#define API_NAME(return_type, func_name, ...) #func_name,

struct EglAPI {
    LIST_IMPL_EGL_FUNCTIONS(API_ENTRY)
    LIST_NOT_IMPL_EGL_FUNCTIONS(API_ENTRY)
};

struct GLAPI {
    LIST_IMPL_GL_FUNCTIONS(API_ENTRY)
    LIST_NOT_IMPL_GL_FUNCTIONS(API_ENTRY)
};

class Loader {
   public:
    Loader();
    ~Loader();

    bool is_cloud_rendering();
    void check_cloud_rendering();

    EglAPI egl;
    GLAPI gl;

   private:
    enum {
        EGL = 0x00,
        GLESv1_CM = 0x01,
        GLESv2 = 0x02,
    };

    typedef __eglMustCastToProperFunctionPointerType (*GetProcAddressType)(
        const char *);

    void attempt_to_load_drivers();
    void *load_driver(const char *kind);
    void initialize_api(uint32_t mask);

    void *dso[3] = {nullptr, nullptr, nullptr};
    std::string suffix = "swiftshader";
    bool is_cloud = false;
    pid_t pid = 0;
    GetProcAddressType get_proc_address;
};

#endif  // LOADER_H