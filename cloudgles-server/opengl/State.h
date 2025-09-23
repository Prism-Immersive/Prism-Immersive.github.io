#ifndef GL_STATE_H
#define GL_STATE_H

#include <unordered_map>

#include "Texture.h"
#include "Vertex.h"



#define MAX_IMAGE_UNITS_NUM 16

typedef struct BufferStatus {
    
    GLuint array_buffer;          // vbo
    GLuint element_array_buffer;  // ebo
    GLuint copy_read_buffer;
    GLuint copy_write_buffer;
    GLuint pixel_pack_buffer;
    GLuint pixel_unpack_buffer;
    GLuint transform_feedback_buffer;
    GLuint uniform_buffer;
    GLuint atomic_counter_buffer;
    GLuint dispatch_indirect_buffer;
    GLuint draw_indirect_buffer;
    GLuint shader_storage_buffer;
    GLuint vertex_array_buffer;  // vao
    GLuint texture_buffer;
} BufferStatus;

typedef struct StaticContextValues {
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

typedef struct TextureBindingStatus {
    
    
    GLuint texture_binding_external;
    GLuint texture_binding_2d;
    GLuint texture_binding_cube_map;
    GLuint texture_binding_3d;
    GLuint texture_binding_2d_array;
    GLuint texture_binding_2d_multisample;
    GLuint texture_binding_2d_multisample_array;
    GLuint texture_binding_cube_map_array;
    GLuint texture_binding_texture_buffer;

} TextureBindingStatus;

typedef struct BoundBuffer {
    BufferStatus *buffer_status;
    AttribPoint *attrib_point;
    GLuint now_vao;

    std::unordered_map<GLint, GLenum> buffer_type;
    std::unordered_map<GLint, AttribPoint *> vao_point_data;
    std::unordered_map<GLint, ElementArrayBuffer *> ebo_buffer;

    
    GLuint *ebo_mm;
    GLuint ebo_mm_num;

    BoundBuffer();
    ~BoundBuffer();
} BoundBuffer;

typedef struct BindingStatus {
    GLuint read_frame_buffer;
    GLuint draw_frame_buffer;

    GLuint render_buffer;

    GLuint uniform_max_index;
    GLuint *uniform_buffer_binding_index;
    GLuint feedback_max_index;
    GLuint *feedback_buffer_binding_index;
    GLuint atomic_counter_max_index;
    GLuint *atomic_counter_buffer_binding_index;
    GLuint shader_storage_max_index;
    GLuint *shader_storage_buffer_binding_index;

    BoundBuffer *bound_buffer;

    TextureBindingStatus *texture_binding;
    // int is_binding_external;
    EGLImageInfo *gbuffer_2D_binding;
    EGLImageInfo *gbuffer_external_binding;

    GLuint *sampler_binding_2d;
    GLuint transform_feedback_binding;

    GLint active_texture;
    GLint drawbuffers[16];
    GLint current_program;
    GLint read_buffer;
    GLenum fragment_shader_derivative_hint;
    GLenum generate_mipmap_hint;
    ImageInfo image_info[MAX_IMAGE_UNITS_NUM];

    GLboolean transform_feedback_paused;
    GLboolean transform_feedback_active;
} BindingStatus;

typedef struct EnableStatus {
    GLboolean cull_face;
    GLboolean polygon_offset_fill;
    GLboolean scissor_test;
    GLboolean sample_coverage;
    GLboolean sample_alpha_to_coverage;
    GLboolean stencil_test;
    GLboolean depth_test;
    GLboolean blend;
    GLboolean primitive_resstart_fixed_index;
    GLboolean rasterizer_discard;
    GLboolean dither;
    GLboolean texture_2d;
} EnableStatus;

GLuint get_bound_buffer(void *context, const GLenum &target);

GLuint get_index_binding_buffer(void *context, const GLenum &target,
                                GLuint index);

GLuint get_binging_status(void *context, const GLenum &target);

GLint *get_static_int_value_ptr(void *context, const GLenum &target);
GLint get_static_int_value(void *context, const GLenum &target);

GLint64 *get_static_int64_value_ptr(void *context, const GLenum &target);
GLfloat *get_static_float_value_ptr(void *context, const GLenum &target);

const GLubyte *get_static_string(void *context, const GLenum &target,
                                 GLint idx = -1);

void d_glBindTexture_special(void *context, const GLenum &target, GLuint id);
void d_glActiveTexture_special(void *context, const GLenum &target);

void d_glBindSampler_special(void *context, GLuint unit, GLuint id);

void d_glBindTransformFeedback_special(void *context, const GLenum &target,
                                       GLuint feedback_id);

void d_glHint_special(void *context, const GLenum &target, GLenum mode);

void d_glDrawBuffers_special(void *context, GLsizei n, const GLenum *bufs);

void d_glBindBuffer_special(void *context, GLenum target, GLuint buffer);

void d_glBindBufferBase_special(void *context, GLenum target, GLuint index,
                                GLuint buffer);

void d_glBindBufferRange_special(void *context, GLenum target, GLuint index,
                                 GLuint buffer, GLintptr offset,
                                 GLsizeiptr size);

void d_glPauseTransformFeedback_special(void *context);

void d_glResumeTransformFeedback_special(void *context);

void d_glBeginTransformFeedback_special(void *context, GLenum primitiveMode);

void d_glEndTransformFeedback_special(void *context);
void d_glGetIntegeri_v_special(void *context, GLenum target, GLuint index,
                               GLint *data);

void d_glDeleteBuffers_special(void *context, GLsizei n, const GLuint *buffers);

void d_glEnable_special(void *context, GLenum cap);
void d_glDisable_special(void *context, GLenum cap);

void d_glGetIntegerv_special(void *context, GLenum pname, GLint *data);
void d_glGetIntegeri_v_special(void *context, GLenum target, GLuint index,
                               GLint *data);
void d_glGetInteger64v_special(void *context, GLenum pname, GLint64 *data);
void d_glGetInteger64i_v_special(void *context, GLenum target, GLuint index,
                                 GLint64 *data);
void d_glGetFloatv_special(void *context, GLenum pname, GLfloat *data);
void d_glGetBooleanv_special(void *context, GLenum pname, GLboolean *data);

const GLubyte *d_glGetStringi_special(void *context, GLenum name, GLuint index);
const GLubyte *d_glGetString_special(void *context, GLenum name);

#define set_gl_error(context, error) \
    set_gl_error_origin(context, error, __FUNCTION__)

void set_gl_error_origin(void *context, GLenum error, const char *fun_name);

GLenum get_gl_error(void *context);

void set_host_error(void *context);

void d_glGetBufferParameteriv_special(void *context, GLenum target,
                                      GLenum pname, GLint *params);

void d_glGetBufferParameteri64v_special(void *context, GLenum target,
                                        GLenum pname, GLint64 *params);

void d_glBindRenderbuffer_special(void *context, GLenum target,
                                  GLuint renderbuffer);

#endif  // GL_STATE_H
