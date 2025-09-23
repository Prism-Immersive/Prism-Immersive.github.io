#ifndef EGL_DISPLAY_H
#define EGL_DISPLAY_H

#include <unordered_map>

#include "Config.h"

#define NUM_DEPTH_VAL 2
#define NUM_STENCILE_VAL 2
#define NUM_SAMPLE_VAL 3

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

class EglDisplay {
   public:
    std::unordered_map<EGLConfig, EglConfig *> egl_config_map;
    EGLint guest_ver_major;
    EGLint guest_ver_minor;
    int is_init;

    EglDisplay();
    void add_config(EglConfig *config);
    static EGLBoolean get_config_pixel_format(EglConfig *cfg,
                                              PixelFormat *format);

    static int depth_vals[NUM_DEPTH_VAL];
    static int stencil_vals[NUM_STENCILE_VAL];
    static int sample_vals[NUM_SAMPLE_VAL];

   private:
    void init_configs();
};

#endif  // EGL_DISPLAY_H
