
#define LOG_TAG "EglConfig"
#include "Config.h"

#include <EGL/eglext.h>

#include "EglUtils.h"
#include "ThreadContext.h"

#define RENDERABLE_SUPPORT \
    (0 | EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT | EGL_OPENGL_ES3_BIT)
#define PBUFFER_MAX_WIDTH 32767
#define PBUFFER_MAX_HEIGHT 32767
#define PBUFFER_MAX_PIXELS (PBUFFER_MAX_WIDTH * PBUFFER_MAX_HEIGHT)
#define MIN_SWAP_INTERVAL 0
#define MAX_SWAP_INTERVAL 10

EGLint config_attrs[NUM_ATTRS] = {EGL_RED_SIZE,
                                  EGL_GREEN_SIZE,
                                  EGL_BLUE_SIZE,
                                  EGL_ALPHA_SIZE,
                                  EGL_BIND_TO_TEXTURE_RGB,
                                  EGL_BIND_TO_TEXTURE_RGBA,
                                  EGL_CONFIG_CAVEAT,
                                  EGL_CONFIG_ID,
                                  EGL_LEVEL,
                                  EGL_DEPTH_SIZE,
                                  EGL_MAX_PBUFFER_WIDTH,
                                  EGL_MAX_PBUFFER_HEIGHT,
                                  EGL_MAX_PBUFFER_PIXELS,
                                  EGL_MAX_SWAP_INTERVAL,
                                  EGL_MIN_SWAP_INTERVAL,
                                  EGL_NATIVE_RENDERABLE,
                                  EGL_RENDERABLE_TYPE,
                                  EGL_NATIVE_VISUAL_ID,
                                  EGL_NATIVE_VISUAL_TYPE,
                                  EGL_SAMPLE_BUFFERS,
                                  EGL_SAMPLES,
                                  EGL_STENCIL_SIZE,
                                  EGL_LUMINANCE_SIZE,
                                  EGL_BUFFER_SIZE,
                                  EGL_SURFACE_TYPE,
                                  EGL_TRANSPARENT_TYPE,
                                  EGL_TRANSPARENT_RED_VALUE,
                                  EGL_TRANSPARENT_GREEN_VALUE,
                                  EGL_TRANSPARENT_BLUE_VALUE,
                                  EGL_CONFORMANT,
                                  EGL_COLOR_BUFFER_TYPE,
                                  EGL_ALPHA_MASK_SIZE,
                                  EGL_RECORDABLE_ANDROID,
                                  EGL_FRAMEBUFFER_TARGET_ANDROID};

EGLint default_configs[NUM_ATTRS * NUM_DEFAULT_CONF] = {
    5,          6,     5,          0,     0,          0,     12344,      1,
    0,          24,    32767,      32767, 1073676289, 10,    1,          0,
    69,         4,     12344,      0,     0,          8,     0,          16,
    5,          12344, 0,          0,     0,          69,    12430,      0,
    0,          1,     8,          8,     8,          0,     0,          0,
    12344,      2,     0,          24,    32767,      32767, 1073676289, 10,
    1,          0,     69,         2,     12344,      0,     0,          8,
    0,          24,    5,          12344, 0,          0,     0,          69,
    12430,      0,     0,          1,     8,          8,     8,          8,
    0,          0,     12344,      3,     0,          24,    32767,      32767,
    1073676289, 10,    1,          0,     69,         1,     12344,      0,
    0,          8,     0,          32,    5,          12344, 0,          0,
    0,          69,    12430,      0,     0,          1,     8,          8,
    8,          8,     0,          0,     12344,      4,     0,          24,
    32767,      32767, 1073676289, 10,    1,          0,     69,         1,
    12344,      0,     0,          8,     0,          32,    5,          12344,
    0,          0,     0,          69,    12430,      0,     0,          1};

/**
 * 
 * 
 */
EglConfig::EglConfig()
    : m_red_size(0),
      m_green_size(0),
      m_blue_size(0),
      m_alpha_size(0),
      m_bind_to_tex_rgb(EGL_DONT_CARE),
      m_bind_to_tex_rgba(EGL_DONT_CARE),
      m_caveat(EGL_DONT_CARE),
      m_config_id(EGL_DONT_CARE),
      m_frame_buffer_level(0),
      m_depth_size(0),
      m_max_pbuffer_width(0),
      m_max_pbuffer_height(0),
      m_max_pbuffer_size(0),
      m_max_swap_interval(EGL_DONT_CARE),
      m_min_swap_interval(EGL_DONT_CARE),
      m_native_renderable(EGL_DONT_CARE),
      m_renderable_type(EGL_OPENGL_ES_BIT),
      m_native_visual_id(0),
      m_native_visual_type(EGL_DONT_CARE),
      m_sample_buffers_num(0),
      m_samples_per_pixel(0),
      m_stencil_size(0),
      m_luminance_size(0),
      m_wanted_buffer_size(EGL_DONT_CARE),
      m_surface_type(EGL_WINDOW_BIT),
      m_transparent_type(EGL_NONE),
      m_trans_red_val(EGL_DONT_CARE),
      m_trans_green_val(EGL_DONT_CARE),
      m_trans_blue_val(EGL_DONT_CARE),
      m_conformant(0),
      m_color_buffer_type(EGL_RGB_BUFFER),
      m_recordable_android(EGL_FALSE),
      m_framebuffer_target_android(EGL_DONT_CARE),
      m_alpha_mask_size(0) {}

EglConfig::EglConfig(EGLint red, EGLint green, EGLint blue, EGLint alpha)
    : m_red_size(0),
      m_green_size(0),
      m_blue_size(0),
      m_alpha_size(0),
      m_bind_to_tex_rgb(EGL_FALSE),
      m_bind_to_tex_rgba(EGL_FALSE),
      m_caveat(EGL_NONE),
      m_config_id(EGL_DONT_CARE),
      m_frame_buffer_level(0),
      m_depth_size(0),
      m_max_pbuffer_width(PBUFFER_MAX_WIDTH),
      m_max_pbuffer_height(PBUFFER_MAX_HEIGHT),
      m_max_pbuffer_size(PBUFFER_MAX_PIXELS),
      m_max_swap_interval(MAX_SWAP_INTERVAL),
      m_min_swap_interval(MIN_SWAP_INTERVAL),
      m_native_renderable(EGL_FALSE),
      m_renderable_type(RENDERABLE_SUPPORT),
      m_native_visual_id(0),
      m_native_visual_type(EGL_NONE),
      m_sample_buffers_num(0),
      m_samples_per_pixel(0),
      m_stencil_size(0),
      m_luminance_size(0),
      m_wanted_buffer_size(EGL_DONT_CARE),
      m_surface_type((EGL_WINDOW_BIT | EGL_PBUFFER_BIT)),
      m_transparent_type(EGL_NONE),
      m_trans_red_val(EGL_DONT_CARE),
      m_trans_green_val(EGL_DONT_CARE),
      m_trans_blue_val(EGL_DONT_CARE),
      m_conformant(0),
      m_color_buffer_type(EGL_RGB_BUFFER),
      m_recordable_android(EGL_FALSE),
      m_framebuffer_target_android(EGL_DONT_CARE),
      m_alpha_mask_size(0) {
    set_val_by_enum(red, EGL_RED_SIZE);
    set_val_by_enum(green, EGL_GREEN_SIZE);
    set_val_by_enum(blue, EGL_BLUE_SIZE);
    set_val_by_enum(alpha, EGL_ALPHA_SIZE);
    set_val_by_enum(0, EGL_LUMINANCE_SIZE);
}

EglConfig::EglConfig(EGLint *config_as_list,
                     std::unordered_map<EGLint, int> *attr_index_map)
    : m_red_size(0),
      m_green_size(0),
      m_blue_size(0),
      m_alpha_size(0),
      m_bind_to_tex_rgb(EGL_DONT_CARE),
      m_bind_to_tex_rgba(EGL_DONT_CARE),
      m_caveat(EGL_DONT_CARE),
      m_config_id(EGL_DONT_CARE),
      m_frame_buffer_level(0),
      m_depth_size(0),
      m_max_pbuffer_width(0),
      m_max_pbuffer_height(0),
      m_max_pbuffer_size(0),
      m_max_swap_interval(EGL_DONT_CARE),
      m_min_swap_interval(EGL_DONT_CARE),
      m_native_renderable(EGL_DONT_CARE),
      m_renderable_type(EGL_OPENGL_ES_BIT),
      m_native_visual_id(0),
      m_native_visual_type(EGL_DONT_CARE),
      m_sample_buffers_num(0),
      m_samples_per_pixel(0),
      m_stencil_size(0),
      m_luminance_size(0),
      m_wanted_buffer_size(EGL_DONT_CARE),
      m_surface_type(EGL_WINDOW_BIT),
      m_transparent_type(EGL_NONE),
      m_trans_red_val(EGL_DONT_CARE),
      m_trans_green_val(EGL_DONT_CARE),
      m_trans_blue_val(EGL_DONT_CARE),
      m_conformant(0),
      m_color_buffer_type(EGL_RGB_BUFFER),
      m_recordable_android(EGL_FALSE),
      m_framebuffer_target_android(EGL_DONT_CARE),
      m_alpha_mask_size(0) {
    for (auto &it : *attr_index_map) {
        set_val_by_enum(config_as_list[it.second], it.first);
    }
}

EglConfig::EglConfig(const EglConfig *obj) {
    m_red_size = obj->m_red_size;
    m_green_size = obj->m_green_size;
    m_blue_size = obj->m_blue_size;
    m_alpha_size = obj->m_alpha_size;
    m_bind_to_tex_rgb = obj->m_bind_to_tex_rgb;
    m_bind_to_tex_rgba = obj->m_bind_to_tex_rgba;
    m_caveat = obj->m_caveat;
    m_config_id = obj->m_config_id;
    m_frame_buffer_level = obj->m_frame_buffer_level;
    m_depth_size = obj->m_depth_size;
    m_max_pbuffer_width = obj->m_max_pbuffer_width;
    m_max_pbuffer_height = obj->m_max_pbuffer_height;
    m_max_pbuffer_size = obj->m_max_pbuffer_size;
    m_max_swap_interval = obj->m_max_swap_interval;
    m_min_swap_interval = obj->m_min_swap_interval;
    m_native_renderable = obj->m_native_renderable;
    m_renderable_type = obj->m_renderable_type;
    m_native_visual_id = obj->m_native_visual_id;
    m_native_visual_type = obj->m_native_visual_type;
    m_sample_buffers_num = obj->m_sample_buffers_num;
    m_samples_per_pixel = obj->m_samples_per_pixel;
    m_stencil_size = obj->m_stencil_size;
    m_luminance_size = obj->m_luminance_size;
    m_wanted_buffer_size = obj->m_wanted_buffer_size;
    m_surface_type = obj->m_surface_type;
    m_transparent_type = obj->m_transparent_type;
    m_trans_red_val = obj->m_trans_red_val;
    m_trans_green_val = obj->m_trans_green_val;
    m_trans_blue_val = obj->m_trans_blue_val;
    m_conformant = obj->m_conformant;
    m_color_buffer_type = obj->m_color_buffer_type;
    m_alpha_mask_size = obj->m_alpha_mask_size;
    m_recordable_android = obj->m_recordable_android;
    m_framebuffer_target_android = obj->m_framebuffer_target_android;
}

void EglConfig::set_val_by_enum(EGLint val, EGLint attr_enum) {
    switch (attr_enum) {
        case EGL_MAX_PBUFFER_WIDTH:
            m_max_pbuffer_width = val;
            break;
        case EGL_MAX_PBUFFER_HEIGHT:
            m_max_pbuffer_height = val;
            break;
        case EGL_MAX_PBUFFER_PIXELS:
            m_max_pbuffer_size = val;
            break;
        case EGL_NATIVE_VISUAL_ID:
            m_native_visual_id = val;
            break;
        case EGL_LEVEL:
            m_frame_buffer_level = val;
            break;
        case EGL_BUFFER_SIZE:
            m_wanted_buffer_size = val;
            break;
        case EGL_RED_SIZE:
            m_red_size = val;
            m_conformant = ((get_buffer_size() > 0) &&
                            (m_caveat != EGL_NON_CONFORMANT_CONFIG))
                               ? m_renderable_type
                               : 0;
            m_framebuffer_target_android =
                (get_buffer_size() == 16 || get_buffer_size() == 32)
                    ? EGL_TRUE
                    : EGL_FALSE;
            break;
        case EGL_GREEN_SIZE:
            m_green_size = val;
            m_conformant = ((get_buffer_size() > 0) &&
                            (m_caveat != EGL_NON_CONFORMANT_CONFIG))
                               ? m_renderable_type
                               : 0;
            m_framebuffer_target_android =
                (get_buffer_size() == 16 || get_buffer_size() == 32)
                    ? EGL_TRUE
                    : EGL_FALSE;
            break;
        case EGL_BLUE_SIZE:
            m_blue_size = val;
            m_conformant = ((get_buffer_size() > 0) &&
                            (m_caveat != EGL_NON_CONFORMANT_CONFIG))
                               ? m_renderable_type
                               : 0;
            m_framebuffer_target_android =
                (get_buffer_size() == 16 || get_buffer_size() == 32)
                    ? EGL_TRUE
                    : EGL_FALSE;
            break;
        case EGL_LUMINANCE_SIZE:
            m_luminance_size = val;
            break;
        case EGL_ALPHA_SIZE:
            m_alpha_size = val;
            m_conformant = ((get_buffer_size() > 0) &&
                            (m_caveat != EGL_NON_CONFORMANT_CONFIG))
                               ? m_renderable_type
                               : 0;
            m_framebuffer_target_android =
                (get_buffer_size() == 16 || get_buffer_size() == 32)
                    ? EGL_TRUE
                    : EGL_FALSE;
            break;
        case EGL_BIND_TO_TEXTURE_RGB:
            m_bind_to_tex_rgb = val;
            break;
        case EGL_BIND_TO_TEXTURE_RGBA:
            m_bind_to_tex_rgba = val;
            break;
        case EGL_CONFIG_CAVEAT:
            m_caveat = val;
            m_conformant = ((get_buffer_size() > 0) &&
                            (m_caveat != EGL_NON_CONFORMANT_CONFIG))
                               ? m_renderable_type
                               : 0;
            break;
        case EGL_CONFORMANT:
            m_conformant = val;
            break;
        case EGL_CONFIG_ID:
            m_config_id = val;
            break;
        case EGL_DEPTH_SIZE:
            m_depth_size = val;
            break;
        case EGL_MAX_SWAP_INTERVAL:
            m_max_swap_interval = val;
            break;
        case EGL_MIN_SWAP_INTERVAL:
            m_min_swap_interval = val;
            break;
        case EGL_NATIVE_RENDERABLE:
            m_native_renderable = val;
            break;
        case EGL_RENDERABLE_TYPE:
            m_renderable_type = val;
            break;
        case EGL_NATIVE_VISUAL_TYPE:
            m_native_visual_type = val;
            break;
        case EGL_SAMPLE_BUFFERS:
            m_sample_buffers_num = val;
            if (m_sample_buffers_num == EGL_DONT_CARE) {
                m_sample_buffers_num = 0;
            }
            break;
        case EGL_SAMPLES:
            m_samples_per_pixel = val;
            m_samples_per_pixel = m_samples_per_pixel > 0 ? 1 : 0;
            break;
        case EGL_STENCIL_SIZE:
            m_stencil_size = val;
            break;
        case EGL_SURFACE_TYPE:
            m_surface_type = val;
            break;
        case EGL_TRANSPARENT_TYPE:
            m_transparent_type = val;
            m_conformant = ((get_buffer_size() > 0) &&
                            (m_caveat != EGL_NON_CONFORMANT_CONFIG))
                               ? m_renderable_type
                               : 0;
            break;
        case EGL_TRANSPARENT_RED_VALUE:
            m_trans_red_val = val;
            break;
        case EGL_TRANSPARENT_GREEN_VALUE:
            m_trans_green_val = val;
            break;
        case EGL_TRANSPARENT_BLUE_VALUE:
            m_trans_blue_val = val;
            break;
        case EGL_COLOR_BUFFER_TYPE:
            m_color_buffer_type = val;
            break;
        case EGL_ALPHA_MASK_SIZE:
            m_alpha_mask_size = val;
            break;
        case EGL_RECORDABLE_ANDROID:
            m_recordable_android = val;
            break;
        case EGL_FRAMEBUFFER_TARGET_ANDROID:
            m_framebuffer_target_android = val;
            break;
        default:
            break;
    }
}

EGLBoolean EglConfig::get_val_by_enum(EGLint attr_enum, EGLint *val) const {
    switch (attr_enum) {
        case EGL_MAX_PBUFFER_WIDTH:
            *val = m_max_pbuffer_width;
            break;
        case EGL_MAX_PBUFFER_HEIGHT:
            *val = m_max_pbuffer_height;
            break;
        case EGL_MAX_PBUFFER_PIXELS:
            *val = m_max_pbuffer_size;
            break;
        case EGL_NATIVE_VISUAL_ID:
            *val = m_native_visual_id;
            break;
        case EGL_LEVEL:
            *val = m_frame_buffer_level;
            break;
        case EGL_BUFFER_SIZE:
            *val = get_buffer_size();
            break;
        case EGL_RED_SIZE:
            *val = m_red_size;
            break;
        case EGL_GREEN_SIZE:
            *val = m_green_size;
            break;
        case EGL_BLUE_SIZE:
            *val = m_blue_size;
            break;
        case EGL_LUMINANCE_SIZE:
            *val = m_luminance_size;
            break;
        case EGL_ALPHA_SIZE:
            *val = m_alpha_size;
            break;
        case EGL_BIND_TO_TEXTURE_RGB:
            *val = m_bind_to_tex_rgb;
            break;
        case EGL_BIND_TO_TEXTURE_RGBA:
            *val = m_bind_to_tex_rgba;
            break;
        case EGL_CONFIG_CAVEAT:
            *val = m_caveat;
            break;
        case EGL_CONFORMANT:
            *val = m_conformant;
            break;
        case EGL_CONFIG_ID:
            *val = m_config_id;
            break;
        case EGL_DEPTH_SIZE:
            *val = m_depth_size;
            break;
        case EGL_MAX_SWAP_INTERVAL:
            *val = m_max_swap_interval;
            break;
        case EGL_MIN_SWAP_INTERVAL:
            *val = m_min_swap_interval;
            break;
        case EGL_NATIVE_RENDERABLE:
            *val = m_native_renderable;
            break;
        case EGL_RENDERABLE_TYPE:
            *val = m_renderable_type;
            break;
        case EGL_NATIVE_VISUAL_TYPE:
            *val = m_native_visual_type;
            break;
        case EGL_SAMPLE_BUFFERS:
            *val = m_sample_buffers_num;
            break;
        case EGL_SAMPLES:
            *val = m_samples_per_pixel;
            break;
        case EGL_STENCIL_SIZE:
            *val = m_stencil_size;
            break;
        case EGL_SURFACE_TYPE:
            *val = m_surface_type;
            break;
        case EGL_TRANSPARENT_TYPE:
            *val = m_transparent_type;
            break;
        case EGL_TRANSPARENT_RED_VALUE:
            *val = m_trans_red_val;
            break;
        case EGL_TRANSPARENT_GREEN_VALUE:
            *val = m_trans_green_val;
            break;
        case EGL_TRANSPARENT_BLUE_VALUE:
            *val = m_trans_blue_val;
            break;
        case EGL_COLOR_BUFFER_TYPE:
            *val = m_color_buffer_type;
            break;
        case EGL_ALPHA_MASK_SIZE:
            *val = m_alpha_mask_size;
            break;
        case EGL_RECORDABLE_ANDROID:
            *val = m_recordable_android;
            break;
        case EGL_FRAMEBUFFER_TARGET_ANDROID:
            *val = m_framebuffer_target_android;
            break;
        default:
            *val = 0;
            return EGL_FALSE;
    }
    return EGL_TRUE;
}

bool EglConfig::is_matched(EglConfig *config,
                           std::unordered_set<EGLint> *desired_attrs) const {
    
    CHECK_CONFIG_NOT_MATCHED(config, get_buffer_size(), >);
    CHECK_CONFIG_NOT_MATCHED(config, m_red_size, >);
    CHECK_CONFIG_NOT_MATCHED(config, m_green_size, >);
    CHECK_CONFIG_NOT_MATCHED(config, m_blue_size, >);
    CHECK_CONFIG_NOT_MATCHED(config, m_alpha_size, >);
    CHECK_CONFIG_NOT_MATCHED(config, m_depth_size, >);
    CHECK_CONFIG_NOT_MATCHED(config, m_stencil_size, >);
    CHECK_CONFIG_NOT_MATCHED(config, m_luminance_size, >);
    CHECK_CONFIG_NOT_MATCHED(config, m_sample_buffers_num, >);
    CHECK_CONFIG_NOT_MATCHED(config, m_samples_per_pixel, >);

    if (desired_attrs->count(EGL_BUFFER_SIZE)) {
        if (config->m_wanted_buffer_size != EGL_DONT_CARE &&
            config->m_wanted_buffer_size > get_buffer_size()) {
            return false;
        }
    }

    
    CHECK_CONFIG_NOT_MATCHED(config, m_frame_buffer_level, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_config_id, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_native_visual_type, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_min_swap_interval, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_max_swap_interval, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_trans_red_val, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_trans_green_val, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_trans_blue_val, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_bind_to_tex_rgb, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_bind_to_tex_rgba, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_caveat, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_native_renderable, !=);
    CHECK_CONFIG_NOT_MATCHED(config, m_transparent_type, !=);

    
    if (config->m_surface_type != EGL_DONT_CARE &&
        ((config->m_surface_type & (m_surface_type | EGL_WINDOW_BIT)) !=
         config->m_surface_type)) {
        
        return false;
    }

    if (config->m_conformant != (EGLenum)EGL_DONT_CARE &&
        ((config->m_conformant & m_conformant) != config->m_conformant)) {
        return false;
    }

    if (config->m_renderable_type != EGL_DONT_CARE &&
        ((config->m_renderable_type & m_renderable_type) !=
         config->m_renderable_type)) {
        return false;
    }

    if ((EGLint)(config->m_framebuffer_target_android) != EGL_DONT_CARE &&
        config->m_framebuffer_target_android != m_framebuffer_target_android) {
        return false;
    }

    return true;
}

bool EglConfig::operator<(const EglConfig &conf) const {
    if (m_conformant != conf.m_conformant) {
        return m_conformant != 0;  // We want the conformant ones first
    }

    if (m_caveat != conf.m_caveat) {
        return m_caveat < conf.m_caveat;  // EGL_NONE < EGL_SLOW_CONFIG <
                                          // EGL_NON_CONFORMANT_CONFIG
    }

    if (m_color_buffer_type != conf.m_color_buffer_type) {
        return ConfigCompare::colorBufferTypeToInt(m_color_buffer_type) <
               ConfigCompare::colorBufferTypeToInt(conf.m_color_buffer_type);
    }

    if (get_buffer_size() != conf.get_buffer_size()) {
        return get_buffer_size() < conf.get_buffer_size();
    }

    if (m_sample_buffers_num != conf.m_sample_buffers_num) {
        return m_sample_buffers_num < conf.m_sample_buffers_num;
    }

    if (m_samples_per_pixel != conf.m_samples_per_pixel) {
        return m_samples_per_pixel < conf.m_samples_per_pixel;
    }

    if (m_depth_size != conf.m_depth_size) {
        return m_depth_size < conf.m_depth_size;
    }

    if (m_stencil_size != conf.m_stencil_size) {
        return m_stencil_size < conf.m_stencil_size;
    }

    return m_config_id < conf.m_config_id;
}

EGLBoolean d_eglGetConfigAttrib_special(void *context, EGLDisplay dpy,
                                        EGLConfig config, EGLint attribute,
                                        EGLint *value) {
    CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    CHECK_CONFIG(config, EGL_FALSE)

    if (egl_default_display->get_config_attr(config, attribute, value)) {
        return EGL_TRUE;
    }
    ALOGE("attribute %x", attribute);
    RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
}

EGLBoolean d_eglGetConfigs_special(void *context, EGLDisplay dpy,
                                   EGLConfig *configs, EGLint config_size,
                                   EGLint *num_config) {
    CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    if (!num_config) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_PARAMETER);
    }

    EGLint display_num_configs = egl_default_display->get_num_configs();
    if (!configs) {
        *num_config = display_num_configs;
        return EGL_TRUE;
    }

    *num_config = egl_default_display->get_configs(configs, config_size);
    return EGL_TRUE;
}

/******* file '2-1' *******/

EGLBoolean d_eglChooseConfig_special(void *context, EGLDisplay dpy,
                                     const EGLint *attrib_list,
                                     EGLConfig *configs, EGLint config_size,
                                     EGLint *num_config) {
    
    CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    if (!num_config) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_PARAMETER);
    }

    
    /* EGLint const attrib_list[] = {
        EGL_RED_SIZE, 4,
        EGL_GREEN_SIZE, 4,
        EGL_BLUE_SIZE, 4,
        EGL_NONE }; */
    int attrib_list_size = 0;
    if (attrib_list) {
        const EGLint *temp_attrib_list = attrib_list;
        while (temp_attrib_list[0] != EGL_NONE) {
            // ALOGE("choose config attrib %x
            // %x",temp_attrib_list[0],temp_attrib_list[1]);
            attrib_list_size += 2;
            temp_attrib_list += 2;
        }
        attrib_list_size++;  
    }

    
    
#ifdef ANDROID_SYSTEM
    long platform_sdk_version = ANDROID_PLATFORM_SDK_VERSION;
#else
    char sdk[128] = "0", *temp_str;
    __system_property_get("ro.build.version.sdk", sdk);
    long platform_sdk_version = strtol(sdk, &temp_str, 10);
#endif
    EGLint *actual_attrib_list = nullptr;
    if (platform_sdk_version <= 19) {
        actual_attrib_list = new EGLint[attrib_list_size];
        memcpy(actual_attrib_list, attrib_list,
               sizeof(EGLint) * attrib_list_size);
        EGLint *temp_actual_attrib_list = actual_attrib_list;
        while (temp_actual_attrib_list[0] != EGL_NONE) {
            if (temp_actual_attrib_list[0] == EGL_SURFACE_TYPE) {
                temp_actual_attrib_list[1] &=
                    ~(EGLint)EGL_SWAP_BEHAVIOR_PRESERVED_BIT;
            }
            temp_actual_attrib_list += 2;
        }
    }

    if (!egl_default_display->choose_config(
            actual_attrib_list ? actual_attrib_list : attrib_list, configs,
            config_size, num_config)) {
        ALOGE("Cannot choose config");
        return EGL_FALSE;
    }

    if (!actual_attrib_list) delete[] actual_attrib_list;

    return EGL_TRUE;
}
