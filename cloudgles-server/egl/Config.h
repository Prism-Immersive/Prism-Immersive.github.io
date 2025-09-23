#ifndef EGL_CONFIG_H
#define EGL_CONFIG_H

#include <EGL/egl.h>

#include <unordered_map>
#include <unordered_set>

#define NUM_ATTRS 34
#define NUM_DEFAULT_CONF 4

extern EGLint config_attrs[NUM_ATTRS];
extern EGLint default_configs[NUM_ATTRS * NUM_DEFAULT_CONF];

class EglConfig {
   public:
    EglConfig();

    EglConfig(const EglConfig *obj);

    EglConfig(EGLint red, EGLint green, EGLint blue, EGLint alpha);

    EglConfig(EGLint *config_as_list,
              std::unordered_map<EGLint, int> *attr_index_map);

    bool is_matched(EglConfig *config,
                    std::unordered_set<EGLint> *desired_attrs) const;

    void set_val_by_enum(EGLint val, EGLint attr_enum);

    EGLBoolean get_val_by_enum(EGLint attr_enum, EGLint *val) const;

    EGLint get_buffer_size() const {
        return m_red_size + m_green_size + m_blue_size + m_alpha_size;
    }

    bool operator<(const EglConfig &conf) const;

   public:
    EGLint m_red_size;
    EGLint m_green_size;
    EGLint m_blue_size;
    EGLint m_alpha_size;
    EGLBoolean m_bind_to_tex_rgb;
    EGLBoolean m_bind_to_tex_rgba;
    EGLenum m_caveat;
    EGLint m_config_id;
    EGLint m_frame_buffer_level;
    EGLint m_depth_size;
    EGLint m_max_pbuffer_width;
    EGLint m_max_pbuffer_height;
    EGLint m_max_pbuffer_size;
    EGLint m_max_swap_interval;
    EGLint m_min_swap_interval;
    EGLBoolean m_native_renderable;
    EGLint m_renderable_type;
    EGLint m_native_visual_id;
    EGLint m_native_visual_type;
    EGLint m_sample_buffers_num;
    EGLint m_samples_per_pixel;
    EGLint m_stencil_size;
    EGLint m_luminance_size;
    EGLint m_wanted_buffer_size;
    EGLint m_surface_type;
    EGLenum m_transparent_type;
    EGLint m_trans_red_val;
    EGLint m_trans_green_val;
    EGLint m_trans_blue_val;
    EGLenum m_conformant;
    EGLint m_color_buffer_type;
    EGLint m_alpha_mask_size;
    EGLBoolean m_recordable_android;
    EGLBoolean m_framebuffer_target_android;
};

class ConfigCompare {
   public:
    ConfigCompare(const EglConfig *config,
                  std::unordered_set<EGLint> *desired_attrs) {
        desire_red_size =
            desired_attrs->count(EGL_RED_SIZE) &&
            (config->m_red_size != 0 && config->m_red_size != EGL_DONT_CARE);
        desire_green_size = desired_attrs->count(EGL_GREEN_SIZE) &&
                            (config->m_green_size != 0 &&
                             config->m_green_size != EGL_DONT_CARE);
        desire_blue_size =
            desired_attrs->count(EGL_BLUE_SIZE) &&
            (config->m_blue_size != 0 && config->m_blue_size != EGL_DONT_CARE);
        desire_luminance_size = desired_attrs->count(EGL_LUMINANCE_SIZE) &&
                                (config->m_luminance_size != 0 &&
                                 config->m_luminance_size != EGL_DONT_CARE);
        desire_alpha_size = desired_attrs->count(EGL_ALPHA_SIZE) &&
                            (config->m_alpha_size != 0 &&
                             config->m_alpha_size != EGL_DONT_CARE);
    }

    bool operator()(EglConfig &left, EglConfig &right) const {
        if (left.m_conformant != right.m_conformant)
            return left.m_conformant != 0;

        if (left.m_caveat != right.m_caveat)
            return left.m_caveat < right.m_caveat;

        if (left.m_color_buffer_type != right.m_color_buffer_type)
            return colorBufferTypeToInt(left.m_color_buffer_type) <
                   colorBufferTypeToInt(right.m_color_buffer_type);

        EGLint left_buffer_size = 0;
        EGLint right_buffer_size = 0;
        if (desire_red_size) {
            left_buffer_size += left.m_red_size;
            right_buffer_size += right.m_red_size;
        }
        if (desire_green_size) {
            left_buffer_size += left.m_green_size;
            right_buffer_size += right.m_green_size;
        }
        if (desire_blue_size) {
            left_buffer_size += left.m_blue_size;
            right_buffer_size += right.m_blue_size;
        }
        if (desire_luminance_size) {
            left_buffer_size += left.m_luminance_size;
            right_buffer_size += right.m_luminance_size;
        }
        if (desire_alpha_size) {
            left_buffer_size += left.m_alpha_size;
            right_buffer_size += right.m_alpha_size;
        }
        if (left_buffer_size != right_buffer_size) {
            
            return left_buffer_size > right_buffer_size;
        }

        if (left.m_wanted_buffer_size != right.m_wanted_buffer_size)
            return left.m_wanted_buffer_size < right.m_wanted_buffer_size;

        if (left.m_sample_buffers_num != right.m_sample_buffers_num)
            return left.m_sample_buffers_num < right.m_sample_buffers_num;

        if (left.m_samples_per_pixel != right.m_samples_per_pixel)
            return left.m_samples_per_pixel < right.m_samples_per_pixel;

        if (left.m_depth_size != right.m_depth_size)
            return left.m_depth_size < right.m_depth_size;

        if (left.m_stencil_size != right.m_stencil_size)
            return left.m_stencil_size < right.m_stencil_size;

        return left.m_config_id < right.m_config_id;
    }

   public:
    static int colorBufferTypeToInt(EGLenum type) {
#define EGL_YUV_BUFFER_EXT 0x3300
        switch (type) {
            case EGL_RGB_BUFFER:
                return 0;
            case EGL_LUMINANCE_BUFFER:
                return 1;
            case EGL_YUV_BUFFER_EXT:
                return 2;
            default:
                return 3;
        }
    }

   private:
    bool desire_red_size;
    bool desire_green_size;
    bool desire_blue_size;
    bool desire_luminance_size;
    bool desire_alpha_size;
};

EGLBoolean d_eglGetConfigAttrib_special(void *context, EGLDisplay dpy,
                                        EGLConfig config, EGLint attribute,
                                        EGLint *value);

EGLBoolean d_eglGetConfigs_special(void *context, EGLDisplay dpy,
                                   EGLConfig *configs, EGLint config_size,
                                   EGLint *num_config);

EGLBoolean d_eglChooseConfig_special(void *context, EGLDisplay dpy,
                                     const EGLint *attrib_list,
                                     EGLConfig *configs, EGLint config_size,
                                     EGLint *num_config);

#endif  // EGL_CONFIG_H
