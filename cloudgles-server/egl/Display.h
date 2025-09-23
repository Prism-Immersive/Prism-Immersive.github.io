#ifndef EGL_DISPLAY_H
#define EGL_DISPLAY_H

#include <pthread.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Config.h"
#include "EGL/egl.h"

// Android's special config attributes
#define EGL_COLOR_COMPONENT_TYPE_EXT 0x3339
#define EGL_COLOR_COMPONENT_TYPE_FIXED_EXT 0x333A

// For EGL_NATIVE_VISUAL_ID
enum class PixelFormat : int32_t {
    UNSPECIFIED = 0,
    RGBA_8888 = 1,
    RGBX_8888 = 2,
    RGB_888 = 3,
    RGB_565 = 4,
    BGRA_8888 = 5,
    RGBA_5551 = 6,
    RGBA_4444 = 7,
    YCBCR_422_SP = 16,
    YCRCB_420_SP = 17,
    YCBCR_422_I = 20,
    RGBA_FP16 = 22,
    RAW16 = 32,
    BLOB = 33,
    IMPLEMENTATION_DEFINED = 34,
    YCBCR_420_888 = 35,
    RAW_OPAQUE = 36,
    RAW10 = 37,
    RAW12 = 38,
    RGBA_1010102 = 43,
    Y8 = 538982489,
    Y16 = 540422489,
    YV12 = 842094169,
    DEPTH_16 = 48,
    DEPTH_24 = 49,
    DEPTH_24_STENCIL_8 = 50,
    DEPTH_32F = 51,
    DEPTH_32F_STENCIL_8 = 52,
    STENCIL_8 = 53,
    YCBCR_P010 = 54,
    HSV_888 = 55,
};

typedef struct {
    void *(*getProcAddress)(const char *funcName);

    void (*init)();

    void (*finish)();

    void (*getIntegerv)(unsigned int pname, int *param);
} GLESInterface_EGL;

class EglDisplay {
   public:
    EglDisplay();
    ~EglDisplay();

    bool initialize();
    void terminate();
    EGLBoolean choose_config(const EGLint *attrib_list, EGLConfig *configs,
                             EGLint config_size, EGLint *num_config);
    const char *query_string(EGLint name);

    int get_major_ver() const { return m_ver_major; }
    int get_minor_ver() const { return m_ver_minor; }
    EGLint get_num_configs() const { return m_num_configs; }
    std::string get_proc_name() const { return m_proc_name; }
    EglConfig *get_config(EGLConfig cfg);
    EGLint get_configs(EGLConfig *configs, EGLint config_size);
    EGLBoolean get_config_attr(EGLConfig cfg, EGLint attr, EGLint *val);

    void add_context(EGLContext ctx);
    void add_surface(EGLSurface surface);
    void destroy_context(EGLContext ctx);
    void destroy_surface(EGLSurface surface);

    bool is_init() const { return m_is_init; }
    bool is_valid_config(EGLConfig cfg);
    bool is_valid_context(EGLContext ctx);
    bool is_valid_surface(EGLSurface surface);

   private:
    void process_configs();
    static EGLBoolean get_config_pixel_format(EglConfig *cfg,
                                              PixelFormat *format);
    void set_proc_name();

   private:
    bool m_is_init;
    int m_ver_major;
    int m_ver_minor;
    std::string m_proc_name;
    char *m_api_str;
    char *m_version_str;
    char *m_vendor_str;
    char *m_extension_str;

    size_t m_num_configs;        // Number of available EGL configs
    int m_num_config_attrs;      // Number of attributes in each config
    EGLint *m_config_attr_list;  // The flat list of config attributes
    std::vector<std::unique_ptr<EglConfig>>
        m_configs;  // Config list sorted by config id

    std::unordered_map<EGLint, int>
        m_attrs;  // Attribute name <-> Index of the corresponding attribute in
                  // the config

    pthread_mutex_t m_thread_lock{};   // Global thread lock
    pthread_mutex_t m_ctx_lock{};      // EGLContext lock
    pthread_mutex_t m_surface_lock{};  // EGLSurface lock

    typedef std::unordered_set<EGLContext> EGLContextSet;
    typedef std::unordered_set<EGLSurface> EGLSurfaceSet;
    EGLContextSet m_contexts;
    EGLSurfaceSet m_surfaces;
};

EGLDisplay d_eglGetCurrentDisplay_special(void *context);

EGLDisplay d_eglGetDisplay_special(void *context,
                                   EGLNativeDisplayType display_id);

#endif  // EGL_DISPLAY_H
