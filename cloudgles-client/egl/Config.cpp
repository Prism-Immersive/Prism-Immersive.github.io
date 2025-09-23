#include "Config.h"

#include <cstring>

#include "renderer/MainRenderer.h"

const EGLint EglConfig::config_attrs[] = {EGL_RED_SIZE,
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

EglConfig::EglConfig()
    : red_size(0),
      green_size(0),
      blue_size(0),
      alpha_size(0),
      bind_to_tex_rgb(EGL_DONT_CARE),
      bind_to_tex_rgba(EGL_DONT_CARE),
      caveat(EGL_DONT_CARE),
      config_id(EGL_DONT_CARE),
      frame_buffer_level(0),
      depth_size(0),
      max_pbuffer_width(0),
      max_pbuffer_height(0),
      max_pbuffer_size(0),
      max_swap_interval(EGL_DONT_CARE),
      min_swap_interval(EGL_DONT_CARE),
      native_renderable(EGL_DONT_CARE),
      renderable_type(EGL_OPENGL_ES_BIT),
      native_visual_id(0),
      native_visual_type(EGL_DONT_CARE),
      sample_buffers_num(0),
      samples_per_pixel(0),
      stencil_size(0),
      luminance_size(0),
      wanted_buffer_size(EGL_DONT_CARE),
      surface_type(EGL_WINDOW_BIT),
      transparent_type(EGL_NONE),
      trans_red_val(EGL_DONT_CARE),
      trans_green_val(EGL_DONT_CARE),
      trans_blue_val(EGL_DONT_CARE),
      conformant(0),
      color_buffer_type(EGL_RGB_BUFFER),
      recordable_android(EGL_FALSE),
      framebuffer_target_android(EGL_DONT_CARE),
      alpha_mask_size(0),
      pixel_format(nullptr) {}

EglConfig::EglConfig(const EglConfig *config) {
    red_size = config->red_size;
    green_size = config->green_size;
    blue_size = config->blue_size;
    alpha_size = config->alpha_size;
    bind_to_tex_rgb = config->bind_to_tex_rgb;
    bind_to_tex_rgba = config->bind_to_tex_rgba;
    caveat = config->caveat;
    config_id = config->config_id;
    frame_buffer_level = config->frame_buffer_level;
    depth_size = config->depth_size;
    max_pbuffer_width = config->max_pbuffer_width;
    max_pbuffer_height = config->max_pbuffer_height;
    max_pbuffer_size = config->max_pbuffer_size;
    max_swap_interval = config->max_swap_interval;
    min_swap_interval = config->min_swap_interval;
    native_renderable = config->native_renderable;
    renderable_type = config->renderable_type;
    native_visual_id = config->native_visual_id;
    native_visual_type = config->native_visual_type;
    sample_buffers_num = config->sample_buffers_num;
    samples_per_pixel = config->samples_per_pixel;
    stencil_size = config->stencil_size;
    luminance_size = config->luminance_size;
    wanted_buffer_size = config->wanted_buffer_size;
    surface_type = config->surface_type;
    transparent_type = config->transparent_type;
    trans_red_val = config->trans_red_val;
    trans_green_val = config->trans_green_val;
    trans_blue_val = config->trans_blue_val;
    conformant = config->conformant;
    color_buffer_type = config->color_buffer_type;
    alpha_mask_size = config->alpha_mask_size;
    recordable_android = config->recordable_android;
    framebuffer_target_android = config->framebuffer_target_android;
    pixel_format = config->pixel_format;
}

EglConfig::EglConfig(EGLint red, EGLint green, EGLint blue, EGLint alpha)
    : red_size(0),
      green_size(0),
      blue_size(0),
      alpha_size(0),
      bind_to_tex_rgb(EGL_FALSE),
      bind_to_tex_rgba(EGL_FALSE),
      caveat(EGL_NONE),
      config_id(EGL_DONT_CARE),
      frame_buffer_level(0),
      depth_size(0),
      max_pbuffer_width(PBUFFER_MAX_WIDTH),
      max_pbuffer_height(PBUFFER_MAX_HEIGHT),
      max_pbuffer_size(PBUFFER_MAX_PIXELS),
      max_swap_interval(MAX_SWAP_INTERVAL),
      min_swap_interval(MIN_SWAP_INTERVAL),
      native_renderable(EGL_FALSE),
      renderable_type(RENDERABLE_SUPPORT),
      native_visual_id(0),
      native_visual_type(EGL_NONE),
      sample_buffers_num(0),
      samples_per_pixel(0),
      stencil_size(0),
      luminance_size(0),
      wanted_buffer_size(EGL_DONT_CARE),
      surface_type((EGL_WINDOW_BIT | EGL_PBUFFER_BIT)),
      transparent_type(EGL_NONE),
      trans_red_val(EGL_DONT_CARE),
      trans_green_val(EGL_DONT_CARE),
      trans_blue_val(EGL_DONT_CARE),
      conformant(0),
      color_buffer_type(EGL_RGB_BUFFER),
      recordable_android(EGL_FALSE),
      framebuffer_target_android(EGL_DONT_CARE),
      alpha_mask_size(0),
      pixel_format(nullptr) {
    set_val_by_enum(red, EGL_RED_SIZE);
    set_val_by_enum(green, EGL_GREEN_SIZE);
    set_val_by_enum(blue, EGL_BLUE_SIZE);
    set_val_by_enum(alpha, EGL_ALPHA_SIZE);
    set_val_by_enum(0, EGL_LUMINANCE_SIZE);
}

EglConfig::EglConfig(EGLint *config_as_list)
    : red_size(0),
      green_size(0),
      blue_size(0),
      alpha_size(0),
      bind_to_tex_rgb(EGL_FALSE),
      bind_to_tex_rgba(EGL_FALSE),
      caveat(EGL_NONE),
      config_id(EGL_DONT_CARE),
      frame_buffer_level(0),
      depth_size(0),
      max_pbuffer_width(PBUFFER_MAX_WIDTH),
      max_pbuffer_height(PBUFFER_MAX_HEIGHT),
      max_pbuffer_size(PBUFFER_MAX_PIXELS),
      max_swap_interval(MAX_SWAP_INTERVAL),
      min_swap_interval(MIN_SWAP_INTERVAL),
      native_renderable(EGL_FALSE),
      renderable_type(RENDERABLE_SUPPORT),
      native_visual_id(0),
      native_visual_type(EGL_NONE),
      sample_buffers_num(0),
      samples_per_pixel(0),
      stencil_size(0),
      luminance_size(0),
      wanted_buffer_size(EGL_DONT_CARE),
      surface_type((EGL_WINDOW_BIT | EGL_PBUFFER_BIT)),
      transparent_type(EGL_NONE),
      trans_red_val(EGL_DONT_CARE),
      trans_green_val(EGL_DONT_CARE),
      trans_blue_val(EGL_DONT_CARE),
      conformant(0),
      color_buffer_type(EGL_RGB_BUFFER),
      recordable_android(EGL_FALSE),
      framebuffer_target_android(EGL_DONT_CARE),
      alpha_mask_size(0),
      pixel_format(nullptr) {
    for (int i = 0; i < NUM_ATTRS; i++) {
        set_val_by_enum(config_as_list[i], config_attrs[i]);
    }
}

void EglConfig::set_val_by_enum(EGLint val, EGLint attr_enum) {
    switch (attr_enum) {
        case EGL_MAX_PBUFFER_WIDTH:
            max_pbuffer_width = val;
            break;
        case EGL_MAX_PBUFFER_HEIGHT:
            max_pbuffer_height = val;
            break;
        case EGL_MAX_PBUFFER_PIXELS:
            max_pbuffer_size = val;
            break;
        case EGL_NATIVE_VISUAL_ID:
            native_visual_id = val;
            break;
        case EGL_LEVEL:
            frame_buffer_level = val;
            break;
        case EGL_BUFFER_SIZE:
            wanted_buffer_size = val;
            break;
        case EGL_RED_SIZE:
            red_size = val;
            conformant = ((get_buffer_size() > 0) &&
                          (caveat != EGL_NON_CONFORMANT_CONFIG))
                             ? renderable_type
                             : 0;
            framebuffer_target_android =
                (get_buffer_size() == 16 || get_buffer_size() == 32)
                    ? EGL_TRUE
                    : EGL_FALSE;
            break;
        case EGL_GREEN_SIZE:
            green_size = val;
            conformant = ((get_buffer_size() > 0) &&
                          (caveat != EGL_NON_CONFORMANT_CONFIG))
                             ? renderable_type
                             : 0;
            framebuffer_target_android =
                (get_buffer_size() == 16 || get_buffer_size() == 32)
                    ? EGL_TRUE
                    : EGL_FALSE;
            break;
        case EGL_BLUE_SIZE:
            blue_size = val;
            conformant = ((get_buffer_size() > 0) &&
                          (caveat != EGL_NON_CONFORMANT_CONFIG))
                             ? renderable_type
                             : 0;
            framebuffer_target_android =
                (get_buffer_size() == 16 || get_buffer_size() == 32)
                    ? EGL_TRUE
                    : EGL_FALSE;
            break;
        case EGL_LUMINANCE_SIZE:
            luminance_size = val;
            break;
        case EGL_ALPHA_SIZE:
            alpha_size = val;
            conformant = ((get_buffer_size() > 0) &&
                          (caveat != EGL_NON_CONFORMANT_CONFIG))
                             ? renderable_type
                             : 0;
            framebuffer_target_android =
                (get_buffer_size() == 16 || get_buffer_size() == 32)
                    ? EGL_TRUE
                    : EGL_FALSE;
            break;
        case EGL_BIND_TO_TEXTURE_RGB:
            bind_to_tex_rgb = val;
            break;
        case EGL_BIND_TO_TEXTURE_RGBA:
            bind_to_tex_rgba = val;
            break;
        case EGL_CONFIG_CAVEAT:
            caveat = val;
            conformant = ((get_buffer_size() > 0) &&
                          (caveat != EGL_NON_CONFORMANT_CONFIG))
                             ? renderable_type
                             : 0;
            break;
        case EGL_CONFORMANT:
            conformant = val;
            break;
        case EGL_CONFIG_ID:
            config_id = val;
            break;
        case EGL_DEPTH_SIZE:
            depth_size = val;
            break;
        case EGL_MAX_SWAP_INTERVAL:
            max_swap_interval = val;
            break;
        case EGL_MIN_SWAP_INTERVAL:
            min_swap_interval = val;
            break;
        case EGL_NATIVE_RENDERABLE:
            native_renderable = val;
            break;
        case EGL_RENDERABLE_TYPE:
            renderable_type = val;
            break;
        case EGL_NATIVE_VISUAL_TYPE:
            native_visual_type = val;
            break;
        case EGL_SAMPLE_BUFFERS:
            sample_buffers_num = val;
            if (sample_buffers_num == EGL_DONT_CARE) {
                sample_buffers_num = 0;
            }
            break;
        case EGL_SAMPLES:
            samples_per_pixel = val;
            samples_per_pixel = samples_per_pixel > 0 ? 1 : 0;
            break;
        case EGL_STENCIL_SIZE:
            stencil_size = val;
            break;
        case EGL_SURFACE_TYPE:
            surface_type = val;
            break;
        case EGL_TRANSPARENT_TYPE:
            transparent_type = val;
            conformant = ((get_buffer_size() > 0) &&
                          (caveat != EGL_NON_CONFORMANT_CONFIG))
                             ? renderable_type
                             : 0;
            break;
        case EGL_TRANSPARENT_RED_VALUE:
            trans_red_val = val;
            break;
        case EGL_TRANSPARENT_GREEN_VALUE:
            trans_green_val = val;
            break;
        case EGL_TRANSPARENT_BLUE_VALUE:
            trans_blue_val = val;
            break;
        case EGL_COLOR_BUFFER_TYPE:
            color_buffer_type = val;
            break;
        case EGL_ALPHA_MASK_SIZE:
            alpha_mask_size = val;
            break;
        case EGL_RECORDABLE_ANDROID:
            recordable_android = val;
            break;
        case EGL_FRAMEBUFFER_TARGET_ANDROID:
            framebuffer_target_android = val;
            break;
        default:
            break;
    }
}

EGLBoolean EglConfig::get_val_by_enum(EGLint attr_enum, EGLint *val) const {
    switch (attr_enum) {
        case EGL_MAX_PBUFFER_WIDTH:
            *val = max_pbuffer_width;
            break;
        case EGL_MAX_PBUFFER_HEIGHT:
            *val = max_pbuffer_height;
            break;
        case EGL_MAX_PBUFFER_PIXELS:
            *val = max_pbuffer_size;
            break;
        case EGL_NATIVE_VISUAL_ID:
            *val = native_visual_id;
            break;
        case EGL_LEVEL:
            *val = frame_buffer_level;
            break;
        case EGL_BUFFER_SIZE:
            *val = get_buffer_size();
            break;
        case EGL_RED_SIZE:
            *val = red_size;
            break;
        case EGL_GREEN_SIZE:
            *val = green_size;
            break;
        case EGL_BLUE_SIZE:
            *val = blue_size;
            break;
        case EGL_LUMINANCE_SIZE:
            *val = luminance_size;
            break;
        case EGL_ALPHA_SIZE:
            *val = alpha_size;
            break;
        case EGL_BIND_TO_TEXTURE_RGB:
            *val = bind_to_tex_rgb;
            break;
        case EGL_BIND_TO_TEXTURE_RGBA:
            *val = bind_to_tex_rgba;
            break;
        case EGL_CONFIG_CAVEAT:
            *val = caveat;
            break;
        case EGL_CONFORMANT:
            *val = conformant;
            break;
        case EGL_CONFIG_ID:
            *val = config_id;
            break;
        case EGL_DEPTH_SIZE:
            *val = depth_size;
            break;
        case EGL_MAX_SWAP_INTERVAL:
            *val = max_swap_interval;
            break;
        case EGL_MIN_SWAP_INTERVAL:
            *val = min_swap_interval;
            break;
        case EGL_NATIVE_RENDERABLE:
            *val = native_renderable;
            break;
        case EGL_RENDERABLE_TYPE:
            *val = renderable_type;
            break;
        case EGL_NATIVE_VISUAL_TYPE:
            *val = native_visual_type;
            break;
        case EGL_SAMPLE_BUFFERS:
            *val = sample_buffers_num;
            break;
        case EGL_SAMPLES:
            *val = samples_per_pixel;
            break;
        case EGL_STENCIL_SIZE:
            *val = stencil_size;
            break;
        case EGL_SURFACE_TYPE:
            *val = surface_type;
            break;
        case EGL_TRANSPARENT_TYPE:
            *val = transparent_type;
            break;
        case EGL_TRANSPARENT_RED_VALUE:
            *val = trans_red_val;
            break;
        case EGL_TRANSPARENT_GREEN_VALUE:
            *val = trans_green_val;
            break;
        case EGL_TRANSPARENT_BLUE_VALUE:
            *val = trans_blue_val;
            break;
        case EGL_COLOR_BUFFER_TYPE:
            *val = color_buffer_type;
            break;
        case EGL_ALPHA_MASK_SIZE:
            *val = alpha_mask_size;
            break;
        case EGL_RECORDABLE_ANDROID:
            *val = recordable_android;
            break;
        case EGL_FRAMEBUFFER_TARGET_ANDROID:
            *val = framebuffer_target_android;
            break;
        default:
            *val = 0;
            return EGL_FALSE;
    }
    return EGL_TRUE;
}

EGLint EglConfig::get_buffer_size() const {
    return red_size + green_size + blue_size + alpha_size;
}

size_t EglConfig::get_attrib_list_len(const EGLint *attrib_list) {
    size_t i = 0;
    while (attrib_list[i] != EGL_NONE) {
        i++;
    }
    return (i + 1) * sizeof(EGLint);
}

void EglConfig::d_eglGetDisplay(void *context, EGLNativeDisplayType display_id,
                                EGLDisplay guest_display) {
    
}

EGLBoolean EglConfig::d_eglChooseConfig(void *context, EGLDisplay dpy,
                                        const EGLint *attrib_list,
                                        EGLConfig *configs, EGLint config_size,
                                        EGLint *num_config) {
    //@todo
    
    
    return EGL_TRUE;
}
