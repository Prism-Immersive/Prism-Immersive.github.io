#define LOG_TAG "Loader"
#include "Loader.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <cutils/properties.h>
#include <dlfcn.h>
#include <unistd.h>
#include <vndksupport/linker.h>

#include <cerrno>
#include <string>

#include "Log.h"
#include "Utils.h"
#include "data/DataTracer.h"
#include "egl/EglDef.h"
#include "egl/EglFuncList.h"
#include "opengl/GLFuncList.h"

static char const *const egl_api_names[] = {LIST_IMPL_EGL_FUNCTIONS(
    API_NAME) LIST_NOT_IMPL_EGL_FUNCTIONS(API_NAME) nullptr};

static char const *const gl_api_names[] = {LIST_IMPL_GL_FUNCTIONS(
    API_NAME) LIST_NOT_IMPL_GL_FUNCTIONS(API_NAME) nullptr};

Loader::Loader() {
    char value[PROPERTY_VALUE_MAX];
    property_get("loader.egl", value, suffix.c_str());
    suffix = value;

    check_cloud_rendering();
    ALOGD("Cloud rendering is %s", is_cloud ? "enabled" : "disabled");

    if (!is_cloud) {
        attempt_to_load_drivers();
    }
}

Loader::~Loader() {
    if (dso[EGL] != nullptr) {
        dlclose(dso[EGL]);
    }

    if (dso[GLESv2] != nullptr) {
        dlclose(dso[GLESv2]);
    }
}

bool Loader::is_cloud_rendering() {
    if (unlikely(getpid() != pid)) {
        ALOGD("pid changed, recheck cloud rendering");
        pid = getpid();
        check_cloud_rendering();
    }
    return is_cloud;
}

void Loader::check_cloud_rendering() {
    char value[PROPERTY_VALUE_MAX];
    std::string package = DataTracer::get_app_package_name();
    ALOGD("package name is %s", package.c_str());
    property_get("cloud.app", value, "0");
    ALOGD("cloud app is %s", value);
    if (package == std::string(value)) {
        ALOGD("cloud rendering enabled for %s", package.c_str());
        is_cloud = true;
    }
}

void Loader::attempt_to_load_drivers() {
    dso[EGL] = load_driver("EGL");
    if (dso[EGL]) {
        get_proc_address =
            (GetProcAddressType)dlsym(dso[EGL], "eglGetProcAddress");
        if (!get_proc_address) {
            ALOGE("Can't find eglGetProcAddress() in EGL driver library");
            return;
        }

        initialize_api(EGL);

        dso[GLESv1_CM] = load_driver("GLESv1_CM");
        dso[GLESv2] = load_driver("GLESv2");
        if (dso[GLESv1_CM] && dso[GLESv2]) {
            initialize_api(GLESv2);
        }
    }
}

void *Loader::load_driver(const char *kind) {
    const char *const search_path =
#if defined(__LP64__)
        "/vendor/lib64/egl";
#else
        "/vendor/lib/egl";
#endif
    std::string driver_absolute_path = std::string(search_path) + "/lib" +
                                       std::string(kind) + "_" + suffix + ".so";
    if (access(driver_absolute_path.c_str(), F_OK) < 0) {
        ALOGE("driver %s not found, error is %s", driver_absolute_path.c_str(),
              strerror(errno));
    }

    void *so = android_load_sphal_library(driver_absolute_path.c_str(),
                                          RTLD_NOW | RTLD_LOCAL);
    if (so == nullptr) {
        ALOGE("load %s failed %s", driver_absolute_path.c_str(), dlerror());
    }

    return so;
}

void Loader::initialize_api(uint32_t mask) {
    __eglMustCastToProperFunctionPointerType *curr = nullptr;
    char const *const *api = nullptr;
    void *dso_handle = nullptr;
    switch (mask) {
        case EGL:
            curr = (__eglMustCastToProperFunctionPointerType *)&egl;
            api = egl_api_names;
            dso_handle = dso[EGL];
            break;
        case GLESv2:
            curr = (__eglMustCastToProperFunctionPointerType *)&gl;
            api = gl_api_names;
            dso_handle = dso[GLESv2];
            break;
        default:
            ALOGE("invalid mask %d", mask);
            return;
    }

    while (*api) {
        char const *name = *api;
        auto f =
            (__eglMustCastToProperFunctionPointerType)dlsym(dso_handle, name);

        if (f == nullptr && mask == GLESv2) {
            f = (__eglMustCastToProperFunctionPointerType)dlsym(dso[GLESv1_CM],
                                                                name);
        }

        if (f == nullptr) {
            // couldn't find the entry-point, use eglGetProcAddress()
            f = get_proc_address(name);
            if (f == nullptr) {
                f = (__eglMustCastToProperFunctionPointerType) nullptr;
                ALOGE("Loader couldn't load %s", name);
            }
        }
        *curr++ = f;
        api++;
    }
}
