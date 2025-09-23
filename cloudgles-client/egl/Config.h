#ifndef EGL_CONFIG_H
#define EGL_CONFIG_H

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#include <unordered_map>

#define RENDERABLE_SUPPORT \
    (0 | EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT | EGL_OPENGL_ES3_BIT)
#define PBUFFER_MAX_WIDTH 32767
#define PBUFFER_MAX_HEIGHT 32767
#define PBUFFER_MAX_PIXELS (PBUFFER_MAX_WIDTH * PBUFFER_MAX_HEIGHT)
#define MIN_SWAP_INTERVAL 0
#define MAX_SWAP_INTERVAL 10
#define NUM_ATTRS 34
#define NUM_DEFAULT_CONF 4

extern EGLint default_configs[NUM_ATTRS * NUM_DEFAULT_CONF];

class EglConfig {
   public:
    EglConfig();

    EglConfig(const EglConfig *config);

    EglConfig(EGLint red, EGLint green, EGLint blue, EGLint alpha);

    EglConfig(EGLint *config_as_list);

    void set_val_by_enum(EGLint val, EGLint attr_enum);

    EGLBoolean get_val_by_enum(EGLint attr_enum, EGLint *val) const;

    EGLint get_buffer_size() const;

    static void d_eglGetDisplay(void *context, EGLNativeDisplayType display_id,
                                EGLDisplay guest_display);

    static EGLBoolean d_eglChooseConfig(void *context, EGLDisplay dpy,
                                        const EGLint *attrib_list,
                                        EGLConfig *configs, EGLint config_size,
                                        EGLint *num_config);
    static size_t get_attrib_list_len(const EGLint *attrib_list);

   public:
    EGLint red_size;
    EGLint green_size;
    EGLint blue_size;
    EGLint alpha_size;
    EGLBoolean bind_to_tex_rgb;
    EGLBoolean bind_to_tex_rgba;
    EGLenum caveat;
    EGLint config_id;
    EGLint frame_buffer_level;
    EGLint depth_size;
    EGLint max_pbuffer_width;
    EGLint max_pbuffer_height;
    EGLint max_pbuffer_size;
    EGLint max_swap_interval;
    EGLint min_swap_interval;
    EGLBoolean native_renderable;
    EGLint renderable_type;
    EGLint native_visual_id;
    EGLint native_visual_type;
    EGLint sample_buffers_num;
    EGLint samples_per_pixel;
    EGLint stencil_size;
    EGLint luminance_size;
    EGLint wanted_buffer_size;
    EGLint surface_type;
    EGLenum transparent_type;
    EGLint trans_red_val;
    EGLint trans_green_val;
    EGLint trans_blue_val;
    EGLenum conformant;
    EGLint color_buffer_type;
    EGLint alpha_mask_size;
    EGLBoolean recordable_android;
    EGLBoolean framebuffer_target_android;

    void *pixel_format;

    static const EGLint config_attrs[];
};

#endif  // EGL_CONFIG_H
