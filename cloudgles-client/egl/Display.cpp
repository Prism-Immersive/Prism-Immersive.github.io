#include "Display.h"

#include <vector>

#include "Log.h"

#define LOG_TAG "EglDisplay"

int EglDisplay::depth_vals[NUM_DEPTH_VAL] = {0, 24};
int EglDisplay::stencil_vals[NUM_STENCILE_VAL] = {0, 8};
int EglDisplay::sample_vals[NUM_SAMPLE_VAL] = {0, 2, 4};

EglDisplay::EglDisplay() {
    guest_ver_major = 1;
    guest_ver_minor = 5;
    init_configs();
    is_init = true;
}

void EglDisplay::init_configs() {
    for (int i = 0; i < NUM_DEFAULT_CONF; i++) {
        std::unique_ptr<EglConfig> cfg(
            new EglConfig(default_configs + i * NUM_ATTRS));
        PixelFormat format;
        if (get_config_pixel_format(cfg.get(), &format)) {
            cfg->set_val_by_enum(static_cast<EGLint>(format),
                                 EGL_NATIVE_VISUAL_ID);
        }
        add_config(cfg.release());
    }
}

void EglDisplay::add_config(EglConfig *config) {
    config->config_id = (EGLint)egl_config_map.size();
    egl_config_map[(EGLConfig)config->config_id] = config;
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
