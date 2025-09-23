#ifndef MAIN_RENDERER_H
#define MAIN_RENDERER_H

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <android/native_window.h>

#include <atomic>
#include <queue>
#include <unordered_map>

#include "egl/Display.h"
#include "egl/Fence.h"
#include "renderer/GraphicBuffer.h"

#define GBUFFER_TYPE_NONE 0
#define GBUFFER_TYPE_WINDOW 1
#define GBUFFER_TYPE_BITMAP 2
#define GBUFFER_TYPE_NATIVE 3
#define GBUFFER_TYPE_BITMAP_NEED_DATA 4
#define GBUFFER_TYPE_TEXTURE 5
#define GBUFFER_TYPE_FBO 6
#define GBUFFER_TYPE_FBO_NEED_DATA 7

#define MAIN_PAINT 1
#define MAIN_CREATE_CHILD_WINDOW 2
#define MAIN_DESTROY_SURFACE 3
#define MAIN_DESTROY_CONTEXT 4
#define MAIN_DESTROY_IMAGE 5
#define MAIN_DESTROY_ALL_EGLSYNC 6
#define MAIN_DESTROY_ONE_SYNC 7
#define MAIN_DESTROY_ONE_TEXTURE 8
#define MAIN_DESTROY_GBUFFER 9
#define MAIN_CANCEL_GBUFFER 10

typedef struct Static_Context_Values {
    GLuint composer_HZ;
    GLuint composer_pid;
    GLint num_extensions;
    //____________ FIXED VALUE ____________
    GLint major_version;
    GLint minor_version;
    GLint implementation_color_read_format;
    GLint implementation_color_read_type;
    GLint max_array_texture_layers;
    GLint max_color_attachments;
    GLint max_combined_uniform_blocks;
    GLint max_draw_buffers;
    GLint max_fragment_input_components;
    GLint max_fragment_uniform_blocks;
    GLint max_program_texel_offset;
    GLint max_transform_feedback_interleaved_components;
    GLint max_transform_feedback_separate_attribs;
    GLint max_transform_feedback_separate_components;
    GLint max_uniform_buffer_bindings;
    GLint max_varying_components;
    GLint max_varying_vectors;
    GLint max_vertex_output_components;
    GLint max_vertex_uniform_blocks;
    GLint min_program_texel_offset;
    GLint num_program_binary_formats;
    //____________ QUERY HOST ___________
    GLint subpixel_bits;
    GLint num_compressed_texture_formats;
    GLint compressed_texture_formats[128];
    GLint max_3d_texture_size;
    GLint max_texture_size;
    GLint max_combined_texture_image_units;
    GLint max_cube_map_texture_size;
    GLint max_elements_vertices;
    GLint max_elements_indices;
    GLint max_fragment_uniform_components;
    GLint max_fragment_uniform_vectors;
    GLint max_renderbuffer_size;
    GLint max_vertex_attribs;
    GLint max_image_units;
    GLint max_vertex_attrib_bindings;
    GLint max_computer_uniform_blocks;
    GLint max_computer_texture_image_units;
    GLint max_computer_image_uniforms;
    GLint max_computer_sharde_memory_size;
    GLint max_computer_uniform_components;
    GLint max_computer_atomic_counter_buffers;
    GLint max_computer_atomic_counters;
    GLint max_combined_compute_uniform_components;
    GLint max_computer_work_group_invocations;
    GLint max_computer_work_group_count[3];
    GLint max_computer_work_group_size[3];
    GLint max_uniform_locations;
    GLint max_framebuffer_width;
    GLint max_framebuffer_height;
    GLint max_framebuffer_samples;
    GLint max_vertex_atomic_counter_buffers;
    GLint max_fragment_atomic_counter_buffers;
    GLint max_combined_atomic_counter_buffers;
    GLint max_vertex_atomic_counters;
    GLint max_fragment_atomic_counters;
    GLint max_combined_atomic_counters;
    GLint max_atomic_counter_buffer_size;
    GLint max_atomic_counter_buffer_bindings;
    GLint max_vertex_image_uniforms;
    GLint max_fragment_image_uniforms;
    GLint max_combined_image_uniforms;
    GLint max_vertex_shader_storage_blocks;
    GLint max_fragment_shader_storage_blocks;
    GLint max_compute_shader_storage_blocks;
    GLint max_combined_shader_storage_blocks;
    GLint max_shader_storage_buffer_bindings;
    GLint max_shader_storage_block_size;
    GLint max_combined_shader_output_resources;
    GLint min_program_texture_gather_offset;
    GLint max_program_texture_gather_offset;
    GLint max_sample_mask_words;
    GLint max_color_texture_samples;
    GLint max_depth_texture_samples;
    GLint max_integer_samples;
    GLint max_vertex_attrib_relative_offset;
    GLint max_vertex_attrib_stride;
    GLint max_vertex_texture_image_units;
    GLint max_vertex_uniform_components;
    GLint max_vertex_uniform_vectors;
    GLint max_viewport_dims[2];
    GLint max_samples;
    GLint texture_image_units;
    GLint uniform_buffer_offset_alignment;
    GLint max_texture_anisotropy;
    GLint num_shader_binary_formats;
    GLint program_binary_formats[8];
    GLint shader_binary_formats[8];
    GLint shader_precision_ranges[24];
    GLint shader_precisions[12];
    GLfloat aliased_line_width_range[2];
    GLfloat aliased_point_size_range[2];
    GLfloat max_texture_log_bias;
    GLint64 max_element_index;
    GLint64 max_server_wait_timeout;
    GLint64 max_combined_fragment_uniform_components;
    GLint64 max_combined_vertex_uniform_components;
    GLint64 max_uniform_block_size;

    // Pointer to actual data
    // Around 512 * 100 + 400 bytes
    GLuint64 vendor;
    GLuint64 version;
    GLuint64 renderer;
    GLuint64 shading_language_version;
    GLuint64 extensions_gles2;
    GLuint64 extensions[512];

} __attribute__((packed, aligned(1))) StaticContextValues;

class Message {
   public:
    int msg;
    void *data;
};

extern EglDisplay egl_default_display;

class MainRenderer {
   public:
    static int COMPOSER_REFRESH_RATE;

    static const int OPENGL_MAJOR_VERSION = 3;
    static const int OPENGL_MINOR_VERSION = 1;

    static const int SPECIAL_EXTENSIONS_SIZE = 46 - 1;
    static const char *SPECIAL_EXTENSIONS[];

    static const char GPU_VENDOR[];
    static const char GPU_VERSION[];
    static const char GPU_RENDERER[];
    static const char GPU_SHADER_LANGUAGE_VERSION[];

    static int DSA_ENABLE;
    static int HOST_OPENGL_VERSION;

    static EGLint display_config_attrib_list[];

    Static_Context_Values *static_context_values;

    std::atomic_flag main_gbuffer_map_lock = ATOMIC_FLAG_INIT;
    std::unordered_map<uint64_t, GraphicBuffer *> main_gbuffer_map;

    std::atomic_flag main_gbuffer_type_map_lock = ATOMIC_FLAG_INIT;
    std::unordered_map<uint64_t, int> main_gbuffer_type_map;

    std::atomic_flag main_fence_map_lock = ATOMIC_FLAG_INIT;
    std::unordered_map<int64_t, std::unique_ptr<EglFence>> main_fence_map;

    std::atomic_flag message_queue_lock = ATOMIC_FLAG_INIT;
    std::queue<Message *> message_queue;

    std::atomic_flag main_native_context_surface_map_lock = ATOMIC_FLAG_INIT;
    std::unordered_map<EGLContext, EGLSurface> main_native_context_surface_map;

    std::vector<GraphicBuffer *> dying_gbuffer_list;

    EglFence eglFence;

    EGLDisplay main_native_display = EGL_NO_DISPLAY;
    EGLContext main_native_context = EGL_NO_CONTEXT;
    EGLSurface main_native_surface = EGL_NO_SURFACE;
    GraphicBuffer *main_display_gbuffer = nullptr;
    ANativeWindow *main_native_window = nullptr;
    GLuint main_program_id;
    GLuint main_draw_vao;

    pthread_t render_thread_id{};

    bool main_renderer_should_run = false;
    bool static_context_values_initialized = false;

    void start(ANativeWindow *window);
    void force_stop();
    void send_window_message(int msg, void *data);

    GraphicBuffer *get_gbuffer_from_global_map(uint64_t gbuffer_id);
    void add_gbuffer_to_global(GraphicBuffer *global_gbuffer);
    void set_global_gbuffer_type(uint64_t gbuffer_id, int type);
    int get_global_gbuffer_type(uint64_t gbuffer_id);
    void set_display_gbuffer(GraphicBuffer *gbuffer);
    int try_destroy_gbuffer(GraphicBuffer *gbuffer);
    void remove_gbuffer_from_global_map(uint64_t gbuffer_id);
    void set_rotation(int rotation);

    static MainRenderer *get_instance();

   private:
    static MainRenderer *instance;
    static std::atomic_flag singleton_lock;

    long window_width;
    long window_height;

    int main_render_rotation = 0;
    bool should_rotate = false;
    int rotate_loc = 0;

    void prepare_egl();
    void prepare_opengl();
    void init_static_context_values();
    static void *main_render_thread(void *renderer);
    void render();
    void handle_window_event();
    void opengl_paint(GraphicBuffer *gbuffer);

    MainRenderer();


};

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#ifdef DSA
// int MainRenderer::DSA_ENABLE = 1;
#define DSA_LIKELY(t) likely(t)
#else
// int MainRenderer::DSA_ENABLE = 0;
#define DSA_LIKELY(t) unlikely(t)
#endif

#endif  // MAIN_RENDERER_H
