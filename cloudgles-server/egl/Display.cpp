#include "Display.h"

#include <EGL/eglext.h>
#include <dlfcn.h>

#include <algorithm>
#include <fstream>
#include <vector>

#include "EglDef.h"
#include "EglUtils.h"
#include "ThreadContext.h"

#define GLES_LIB_PATH "libGLESv1_CM.so"
#define GLES_V2_LIB_PATH "libGLESv2.so"

#define CURRENT_EGL_MAJOR_VER 1
#define CURRENT_EGL_MINOR_VER 5

#define LOG_TAG "EGL_DISPLAY"

static void *g_gles_lib = nullptr;
static void *g_glesv2_lib = nullptr;
static const char VENDOR_NAME[] = "Express GPU";
static const char API_STR[] = "OpenGL_ES";
static const char EGL_EXT_STR[] =
    "EGL_ANDROID_image_native_buffer "
    "EGL_KHR_get_all_proc_addresses "
    "EGL_KHR_image "
    "EGL_KHR_image_base "
    "EGL_KHR_fence_sync "
    "EGL_KHR_gl_texture_2D_image "
    "EGL_KHR_wait_sync";
// "EGL_ANDROID_native_fence_sync";

// "EGL_KHR_image "
// "EGL_ANDROID_framebuffer_target "
// "EGL_ANDROID_recordable "
// "EGL_KHR_gl_texture_cubemap_image "
// "EGL_ANDROID_recordable "
// "EGL_KHR_gl_texture_2D_image "
// "EGL_KHR_gl_texture_cubemap_image "
// "EGL_ANDROID_framebuffer_target "

// extern thread_local void *ctx;

#define NUM_DEPTH_VAL 2
#define NUM_STENCILE_VAL 2
#define NUM_SAMPLE_VAL 3
const int depth_vals[NUM_DEPTH_VAL] = {0, 24};
const int stencil_vals[NUM_STENCILE_VAL] = {0, 8};
const int sample_vals[NUM_SAMPLE_VAL] = {0, 2, 4};

/**
 * eglDisplay
 */
EglDisplay::EglDisplay()
    : m_is_init(false),
      m_ver_major(0),
      m_ver_minor(0),
      m_api_str(nullptr),
      m_version_str(nullptr),
      m_vendor_str(nullptr),
      m_extension_str(nullptr),
      m_num_configs(0),
      m_num_config_attrs(0),
      m_attrs(),
      m_config_attr_list(nullptr) {
    pthread_mutex_init(&m_thread_lock, nullptr);
    pthread_mutex_init(&m_ctx_lock, nullptr);
    pthread_mutex_init(&m_surface_lock, nullptr);
    pthread_mutex_lock(&m_thread_lock);
    set_proc_name();
    pthread_mutex_unlock(&m_thread_lock);
}

EglDisplay::~EglDisplay() {
    terminate();
    pthread_mutex_destroy(&m_thread_lock);
    pthread_mutex_destroy(&m_ctx_lock);
    pthread_mutex_destroy(&m_surface_lock);
}

/**
 * Initialize the display config info
 * @return True for success, false for failure
 */
bool EglDisplay::initialize() {
    pthread_mutex_lock(&m_thread_lock);
    ALOGD("Initialize display");

    if (!egl_thread_context) {
        egl_thread_context = thread_context_init();
    }

    // Already initialized
    if (m_is_init) {
        pthread_mutex_unlock(&m_thread_lock);
        return true;
    }

    m_ver_major = CURRENT_EGL_MAJOR_VER;
    m_ver_minor = CURRENT_EGL_MINOR_VER;
    set_proc_name();

    m_num_configs = NUM_DEFAULT_CONF;
    m_num_config_attrs = NUM_ATTRS;

    // The first line is the enum value of each attribute
    // So the actual number of config attributes needs to minus 1
    uint32_t total_number_attrs = (m_num_configs + 1) * m_num_config_attrs;

    // The first line is stored in the map: attribute name <-> Index of the
    // corresponding attribute in the config
    uint32_t actual_number_attrs = total_number_attrs - m_num_config_attrs;
    m_config_attr_list = new EGLint[actual_number_attrs];
    for (int i = 0; i < m_num_config_attrs; i++) {
        m_attrs[config_attrs[i]] = i;
    }

    // Store the default config attributes
    memcpy(m_config_attr_list, default_configs,
           actual_number_attrs * sizeof(EGLint));

    process_configs();
    m_is_init = true;
    pthread_mutex_unlock(&m_thread_lock);

    return true;
}

void EglDisplay::process_configs() {
    for (int i = 0; i < m_num_configs; i++) {
        std::unique_ptr<EglConfig> cfg(new EglConfig(
            m_config_attr_list + i * m_num_config_attrs, &m_attrs));
        PixelFormat format;
        if (get_config_pixel_format(cfg.get(), &format)) {
            cfg->set_val_by_enum(static_cast<EGLint>(format),
                                 EGL_NATIVE_VISUAL_ID);
        }
        m_configs.emplace_back(cfg.release());
    }
    std::sort(m_configs.begin(), m_configs.end(),
              [&](const std::unique_ptr<EglConfig> &first,
                  const std::unique_ptr<EglConfig> &second) {
                  return *first < *second;
              });
}

EGLBoolean EglDisplay::get_config_pixel_format(EglConfig *cfg,
                                               PixelFormat *format) {
    EGLint red_size, blue_size, green_size, alpha_size;

    if (!((cfg->get_val_by_enum(EGL_RED_SIZE, &red_size)) &&
          (cfg->get_val_by_enum(EGL_BLUE_SIZE, &blue_size)) &&
          (cfg->get_val_by_enum(EGL_GREEN_SIZE, &green_size)) &&
          (cfg->get_val_by_enum(EGL_ALPHA_SIZE, &alpha_size)))) {
        ALOGE("Incomplete pixel value %s", __FUNCTION__);
        return EGL_FALSE;
    }

    if ((red_size == 8) && (green_size == 8) && (blue_size == 8) &&
        (alpha_size == 8))
        *format = PixelFormat::RGBA_8888;
    else if ((red_size == 8) && (green_size == 8) && (blue_size == 8) &&
             (alpha_size == 0))
        *format = PixelFormat::RGBX_8888;
    else if ((red_size == 5) && (green_size == 6) && (blue_size == 5) &&
             (alpha_size == 0))
        *format = PixelFormat::RGB_565;
    else if ((red_size == 5) && (green_size == 5) && (blue_size == 5) &&
             (alpha_size == 1))
        *format = PixelFormat::RGBA_5551;
    else if ((red_size == 4) && (green_size == 4) && (blue_size == 4) &&
             (alpha_size == 4))
        *format = PixelFormat::RGBA_4444;
    else {
        return EGL_FALSE;
    }
    return EGL_TRUE;
}

/**
 * Get the config by config_id
 * @param id config_id
 * @return EglConfig pointer
 */
EglConfig *EglDisplay::get_config(EGLConfig cfg) {
    for (auto &m_config : m_configs) {
        if (m_config->m_config_id == (int64_t)cfg) return m_config.get();
    }
    return nullptr;
}

/**
 * Get all the configs of the current display
 * @param configs config array which are in fact an array of config id
 * @param config_size the size of the array
 * @return the number of configs
 */
EGLint EglDisplay::get_configs(EGLConfig *configs, EGLint config_size) {
    pthread_mutex_lock(&m_thread_lock);
    int i = 0;
    for (auto it = m_configs.begin(); it != m_configs.end() && i < config_size;
         i++, ++it) {
        configs[i] = (EGLConfig)(uint64_t)(*it)->m_config_id;
    }
    pthread_mutex_unlock(&m_thread_lock);
    return i;
}

/**
 * Get the attribute value of an config by attribute enum
 * @param cfg Config id
 * @param attr Attribute enum
 * @param val Attribute value
 * @return True for success, false for failure
 */
EGLBoolean EglDisplay::get_config_attr(EGLConfig cfg, EGLint attr,
                                       EGLint *val) {
    //    if (attr == EGL_FRAMEBUFFER_TARGET_ANDROID) {
    //        *val = EGL_TRUE;
    //        return EGL_TRUE;
    //    }
    if (attr == EGL_COVERAGE_SAMPLES_NV || attr == EGL_COVERAGE_BUFFERS_NV) {
        *val = 0;
        return EGL_TRUE;
    }
    if (attr == EGL_DEPTH_ENCODING_NV) {
        *val = EGL_DEPTH_ENCODING_NONE_NV;
        return EGL_TRUE;
    }
    if (attr == EGL_COLOR_COMPONENT_TYPE_EXT) {
        *val = EGL_COLOR_COMPONENT_TYPE_FIXED_EXT;
        return EGL_TRUE;
    }
    if (attr == EGL_NONE) {
        *val = EGL_NONE;
        return EGL_TRUE;
    }
    if (attr == EGL_MATCH_NATIVE_PIXMAP) {
        *val = EGL_NONE;
        return EGL_TRUE;
    }
    // 0x3030
    if (attr == 0x3030) {
        *val = EGL_FALSE;
        return EGL_TRUE;
    }

    pthread_mutex_lock(&m_thread_lock);
    EglConfig *config = get_config(cfg);
    EGLBoolean ret = config->get_val_by_enum(attr, val);
    pthread_mutex_unlock(&m_thread_lock);
    return ret;
}

void EglDisplay::terminate() {
    pthread_mutex_lock(&m_thread_lock);
    if (m_is_init) {
        auto contexts_to_delete = m_contexts;
        auto ctx_it = contexts_to_delete.begin();
        while (ctx_it != contexts_to_delete.end()) {
            EGLContext ctx = *ctx_it;
            d_eglDestroyContext(egl_thread_context, this, ctx);
            ctx_it++;
        }
        auto surfaces_to_delete = m_surfaces;
        auto surface_it = surfaces_to_delete.begin();
        while (surface_it != surfaces_to_delete.end()) {
            EGLSurface surface = *surface_it;
            d_eglDestroySurface(egl_thread_context, this, surface);
            surface_it++;
        }
        delete[] m_config_attr_list;
        m_config_attr_list = nullptr;
        m_configs.clear();
        m_proc_name.clear();
        m_is_init = false;
    }

    if (m_api_str) {
        free(m_api_str);
        m_api_str = nullptr;
    }
    if (m_version_str) {
        free(m_version_str);
        m_version_str = nullptr;
    }
    if (m_vendor_str) {
        free(m_vendor_str);
        m_vendor_str = nullptr;
    }
    if (m_extension_str) {
        free(m_extension_str);
        m_extension_str = nullptr;
    }

    pthread_mutex_unlock(&m_thread_lock);
}

bool EglDisplay::is_valid_config(EGLConfig cfg) {
    return (get_config(cfg) != nullptr);
}

/**
 * Choose the best config for the current display based on the given attributes
 * @param attrib_list The list of attributes to match
 * @param configs Matched config array
 * @param config_size The size of the array
 * @param num_config The actual number of matched configs
 * @return True for success, false for failure
 */
EGLBoolean EglDisplay::choose_config(const EGLint *attrib_list,
                                     EGLConfig *configs, EGLint config_size,
                                     EGLint *num_config) {
    EglConfig desired_config;
    std::unordered_set<EGLint> desired_attrs;

    if (attrib_list && attrib_list[0] != EGL_NONE) {
        int idx = 0;
        bool desire_config_id = false;
        while (attrib_list[idx] != EGL_NONE && !desire_config_id) {
            EGLint attrib_enum = attrib_list[idx];
            EGLint attrib_val = attrib_list[idx + 1];
            
            if (attrib_enum != EGL_LEVEL &&
                attrib_enum != EGL_MATCH_NATIVE_PIXMAP &&
                attrib_val == EGL_DONT_CARE) {
                idx += 2;
                continue;
            }

            switch (attrib_enum) {
                
                case EGL_MAX_PBUFFER_WIDTH:
                case EGL_MAX_PBUFFER_HEIGHT:
                case EGL_MAX_PBUFFER_PIXELS:
                case EGL_NATIVE_VISUAL_ID:
                    break;
                case EGL_LEVEL: {
                    if (attrib_val == EGL_DONT_CARE) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_frame_buffer_level = attrib_val;
                    desired_attrs.insert(EGL_LEVEL);
                    break;
                }
                case EGL_BUFFER_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_wanted_buffer_size = attrib_val;
                    desired_attrs.insert(EGL_BUFFER_SIZE);
                    break;
                }
                case EGL_RED_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_red_size = attrib_val;
                    desired_attrs.insert(EGL_RED_SIZE);
                    break;
                }
                case EGL_GREEN_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_green_size = attrib_val;
                    desired_attrs.insert(EGL_GREEN_SIZE);
                    break;
                }
                case EGL_BLUE_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_blue_size = attrib_val;
                    desired_attrs.insert(EGL_BLUE_SIZE);
                    break;
                }
                case EGL_LUMINANCE_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_luminance_size = attrib_val;
                    desired_attrs.insert(EGL_LUMINANCE_SIZE);
                    break;
                }
                case EGL_ALPHA_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_alpha_size = attrib_val;
                    desired_attrs.insert(EGL_ALPHA_SIZE);
                    break;
                }
                case EGL_ALPHA_MASK_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_attrs.insert(EGL_ALPHA_MASK_SIZE);
                    break;
                }
                case EGL_BIND_TO_TEXTURE_RGB: {
                    if (attrib_val != EGL_TRUE && attrib_val != EGL_FALSE) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_bind_to_tex_rgb = attrib_val;
                    desired_attrs.insert(EGL_BIND_TO_TEXTURE_RGB);
                    break;
                }
                case EGL_BIND_TO_TEXTURE_RGBA: {
                    if (attrib_val != EGL_TRUE && attrib_val != EGL_FALSE) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_bind_to_tex_rgba = attrib_val;
                    desired_attrs.insert(EGL_BIND_TO_TEXTURE_RGBA);
                    break;
                }
                case EGL_CONFIG_CAVEAT: {
                    if (attrib_val != EGL_NONE &&
                        attrib_val != EGL_SLOW_CONFIG &&
                        attrib_val != EGL_NON_CONFORMANT_CONFIG) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_caveat = attrib_val;
                    desired_attrs.insert(EGL_CONFIG_CAVEAT);
                    break;
                }
                case EGL_CONFORMANT: {
                    desired_config.m_conformant = attrib_val;
                    desired_attrs.insert(EGL_CONFORMANT);
                    break;
                }
                case EGL_CONFIG_ID: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_config_id = attrib_val;
                    desire_config_id = true;
                    desired_attrs.insert(EGL_CONFIG_ID);
                    break;
                }
                case EGL_DEPTH_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_depth_size = attrib_val;
                    desired_attrs.insert(EGL_DEPTH_SIZE);
                    break;
                }
                case EGL_MAX_SWAP_INTERVAL: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_max_swap_interval = attrib_val;
                    desired_attrs.insert(EGL_MAX_SWAP_INTERVAL);
                    break;
                }
                case EGL_MIN_SWAP_INTERVAL: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_min_swap_interval = attrib_val;
                    desired_attrs.insert(EGL_MIN_SWAP_INTERVAL);
                    break;
                }
                case EGL_NATIVE_RENDERABLE: {
                    if (attrib_val != EGL_TRUE && attrib_val != EGL_FALSE) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_native_renderable = attrib_val;
                    desired_attrs.insert(EGL_NATIVE_RENDERABLE);
                    break;
                }
                case EGL_RENDERABLE_TYPE: {
                    desired_config.m_renderable_type = attrib_val;
                    desired_attrs.insert(EGL_RENDERABLE_TYPE);
                    break;
                }
                case EGL_NATIVE_VISUAL_TYPE: {
                    if (attrib_val < 0 || attrib_val > 1) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_native_visual_type = attrib_val;
                    desired_attrs.insert(EGL_NATIVE_VISUAL_TYPE);
                    break;
                }
                case EGL_SAMPLE_BUFFERS: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_sample_buffers_num = attrib_val;
                    desired_attrs.insert(EGL_SAMPLE_BUFFERS);
                    break;
                }
                case EGL_SAMPLES: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_samples_per_pixel = attrib_val;
                    desired_attrs.insert(EGL_SAMPLES);
                    break;
                }
                case EGL_STENCIL_SIZE: {
                    if (attrib_val < 0) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_stencil_size = attrib_val;
                    desired_attrs.insert(EGL_STENCIL_SIZE);
                    break;
                }
                case EGL_SURFACE_TYPE: {
                    desired_config.m_surface_type = attrib_val;
                    desired_attrs.insert(EGL_SURFACE_TYPE);
                    break;
                }
                case EGL_TRANSPARENT_TYPE: {
                    if (attrib_val != EGL_NONE &&
                        attrib_val != EGL_TRANSPARENT_RGB) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_config.m_transparent_type = attrib_val;
                    desired_attrs.insert(EGL_TRANSPARENT_TYPE);
                    break;
                }
                case EGL_TRANSPARENT_RED_VALUE: {
                    desired_config.m_trans_red_val = attrib_val;
                    desired_attrs.insert(EGL_TRANSPARENT_RED_VALUE);
                    break;
                }
                case EGL_TRANSPARENT_GREEN_VALUE: {
                    desired_config.m_trans_green_val = attrib_val;
                    desired_attrs.insert(EGL_TRANSPARENT_GREEN_VALUE);
                    break;
                }
                case EGL_TRANSPARENT_BLUE_VALUE: {
                    desired_config.m_trans_blue_val = attrib_val;
                    desired_attrs.insert(EGL_TRANSPARENT_BLUE_VALUE);
                    break;
                }
                case EGL_COLOR_BUFFER_TYPE: {
                    if (attrib_val != EGL_RGB_BUFFER &&
                        attrib_val != EGL_LUMINANCE_BUFFER) {
                        RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                    }
                    desired_attrs.insert(EGL_COLOR_BUFFER_TYPE);
                    break;
                }
                case EGL_RECORDABLE_ANDROID: {
                    desired_config.m_recordable_android = attrib_val;
                    desired_attrs.insert(EGL_RECORDABLE_ANDROID);
                    break;
                }
                case EGL_FRAMEBUFFER_TARGET_ANDROID: {
                    desired_config.m_framebuffer_target_android = attrib_val;
                    desired_attrs.insert(EGL_FRAMEBUFFER_TARGET_ANDROID);
                    break;
                }
                default: {
                    ALOGE("Unknown attribute enum 0x%x", attrib_enum);
                    RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
                }
            }
            idx += 2;
        }
        if (desire_config_id) {
            if (desired_config.m_config_id > m_num_configs ||
                desired_config.m_config_id <= 0) {
                RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
            }
            configs[0] = (EGLConfig)(uint64_t)desired_config.m_config_id;
            *num_config = 1;
            return EGL_TRUE;
        }
    }

    pthread_mutex_lock(&m_thread_lock);
    *num_config = 0;
    std::vector<EglConfig> selected_configs;
    for (int idx = 0;
         idx < m_num_configs && (*num_config < config_size || !configs);
         idx++) {
        EglConfig *config = m_configs[idx].get();
        if (config->is_matched(&desired_config, &desired_attrs)) {
            if (configs) {
                selected_configs.push_back(*config);
            }
            (*num_config)++;
        }
    }
    pthread_mutex_unlock(&m_thread_lock);

    std::sort(selected_configs.begin(), selected_configs.end(),
              ConfigCompare(&desired_config, &desired_attrs));

    for (int i = 0; configs && i < *num_config; i++) {
        configs[i] = (EGLConfig)(uint64_t)selected_configs[i].m_config_id;
    }
    return EGL_TRUE;
}

void EglDisplay::add_context(EGLContext ctx) {
    pthread_mutex_lock(&m_ctx_lock);
    m_contexts.insert(ctx);
    pthread_mutex_unlock(&m_ctx_lock);
}

void EglDisplay::add_surface(EGLSurface surface) {
    pthread_mutex_lock(&m_surface_lock);
    m_surfaces.insert(surface);
    pthread_mutex_unlock(&m_surface_lock);
}

void EglDisplay::destroy_context(EGLContext ctx) {
    pthread_mutex_lock(&m_ctx_lock);
    m_contexts.erase(ctx);
    pthread_mutex_unlock(&m_ctx_lock);
}

void EglDisplay::destroy_surface(EGLSurface surface) {
    pthread_mutex_lock(&m_surface_lock);
    m_surfaces.erase(surface);
    pthread_mutex_unlock(&m_surface_lock);
}

bool EglDisplay::is_valid_surface(EGLSurface surface) {
    pthread_mutex_lock(&m_surface_lock);
    bool ret = m_surfaces.find(surface) != m_surfaces.end();
    pthread_mutex_unlock(&m_surface_lock);
    return ret;
}

bool EglDisplay::is_valid_context(EGLContext ctx) {
    pthread_mutex_lock(&m_ctx_lock);
    bool ret = m_contexts.find(ctx) != m_contexts.end();
    pthread_mutex_unlock(&m_ctx_lock);
    return ret;
}

void EglDisplay::set_proc_name() {
    std::ifstream in;
    in.open("/proc/self/status", std::ios::in);
    std::string proc_status;
    int pos = 0;
    do {
        getline(in, proc_status);
        pos = proc_status.find("Name:");
    } while (pos < 0);
    m_proc_name = proc_status.substr(pos + 6);
    //    global_proc_name = m_proc_name.c_str();
    //    global_proc_name_len = m_proc_name.length();

    in.close();
}

const char *EglDisplay::query_string(EGLint name) {
    pthread_mutex_lock(&m_thread_lock);
    switch (name) {
        case EGL_CLIENT_APIS: {
            if (m_api_str) {
                pthread_mutex_unlock(&m_thread_lock);
                return m_api_str;
            }
            asprintf(&m_api_str, API_STR);
            pthread_mutex_unlock(&m_thread_lock);
            return m_api_str;
        }
        case EGL_VENDOR: {
            if (m_vendor_str) {
                pthread_mutex_unlock(&m_thread_lock);
                return m_vendor_str;
            }
            asprintf(&m_vendor_str, VENDOR_NAME);
            pthread_mutex_unlock(&m_thread_lock);
            return m_vendor_str;
        }
        case EGL_VERSION: {
            if (m_version_str) {
                pthread_mutex_unlock(&m_thread_lock);
                return m_version_str;
            }
            asprintf(&m_version_str, "%d.%d", m_ver_major, m_ver_minor);
            pthread_mutex_unlock(&m_thread_lock);
            return m_version_str;
        }
        case EGL_EXTENSIONS: {
            if (m_extension_str) {
                pthread_mutex_unlock(&m_thread_lock);
                return m_extension_str;
            }
            m_extension_str = strdup(EGL_EXT_STR);
            pthread_mutex_unlock(&m_thread_lock);
            return m_extension_str;
        }
        default: {
            pthread_mutex_unlock(&m_thread_lock);
            return nullptr;
        }
    }
}

EGLDisplay d_eglGetCurrentDisplay_special(void *context) {
    GLThreadContext *opengl_context =
        ((ThreadContext *)context)->opengl_context;
    if (!opengl_context) {
        return EGL_NO_DISPLAY;
    } else {
        return (EGLDisplay)((ThreadContext *)context)->display;
    }
}

/**
 * @brief 
 * @param display_id 
 * @return 
 */
EGLDisplay d_eglGetDisplay_special(void *context,
                                   EGLNativeDisplayType display_id) {
    
    if (display_id != EGL_DEFAULT_DISPLAY) {
        return EGL_NO_DISPLAY;
    }
    auto guest_display = (EGLDisplay)egl_default_display;
    return guest_display;
}
