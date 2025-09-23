#include "Helper.h"

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

#include <algorithm>
#include <cstring>

#include "Log.h"

#define LOG_TAG "GLHelper"

size_t GLHelper::gl_pname_size(GLenum pname) {
    size_t s = 0;

    switch (pname) {
        case GL_DEPTH_TEST:
        case GL_DEPTH_FUNC:
        case GL_DEPTH_BITS:
        case GL_MAX_CLIP_PLANES:
        case GL_MAX_COLOR_ATTACHMENTS:
        case GL_MAX_DRAW_BUFFERS:
        case GL_GREEN_BITS:
        case GL_MAX_MODELVIEW_STACK_DEPTH:
        case GL_MAX_PROJECTION_STACK_DEPTH:
        case GL_MAX_TEXTURE_STACK_DEPTH:
        case GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES:
        case GL_IMPLEMENTATION_COLOR_READ_TYPE_OES:
        case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
        case GL_MAX_TEXTURE_SIZE:
            // case GL_TEXTURE_GEN_MODE_OES:
        case GL_TEXTURE_ENV_MODE:
        case GL_FOG_MODE:
        case GL_FOG_DENSITY:
        case GL_FOG_START:
        case GL_FOG_END:
        case GL_SPOT_EXPONENT:
        case GL_CONSTANT_ATTENUATION:
        case GL_LINEAR_ATTENUATION:
        case GL_QUADRATIC_ATTENUATION:
        case GL_SHININESS:
        case GL_LIGHT_MODEL_TWO_SIDE:
        case GL_POINT_SIZE:
        case GL_POINT_SIZE_MIN:
        case GL_POINT_SIZE_MAX:
        case GL_POINT_FADE_THRESHOLD_SIZE:
        case GL_CULL_FACE:
        case GL_CULL_FACE_MODE:
        case GL_FRONT_FACE:
        case GL_SHADE_MODEL:
        case GL_DEPTH_WRITEMASK:
        case GL_DEPTH_CLEAR_VALUE:
        case GL_STENCIL_FAIL:
        case GL_STENCIL_PASS_DEPTH_FAIL:
        case GL_STENCIL_PASS_DEPTH_PASS:
        case GL_STENCIL_REF:
        case GL_STENCIL_WRITEMASK:
        case GL_MATRIX_MODE:
        case GL_MODELVIEW_STACK_DEPTH:
        case GL_PROJECTION_STACK_DEPTH:
        case GL_TEXTURE_STACK_DEPTH:
        case GL_ALPHA_TEST_FUNC:
        case GL_ALPHA_TEST_REF:
        case GL_ALPHA_TEST:
        case GL_DITHER:
        case GL_BLEND_DST:
        case GL_BLEND_SRC:
        case GL_BLEND:
        case GL_LOGIC_OP_MODE:
        case GL_SCISSOR_TEST:
        case GL_MAX_TEXTURE_UNITS:
        case GL_ACTIVE_TEXTURE:
        case GL_ALPHA_BITS:
        case GL_ARRAY_BUFFER_BINDING:
        case GL_BLUE_BITS:
        case GL_CLIENT_ACTIVE_TEXTURE:
        case GL_CLIP_PLANE0:
        case GL_CLIP_PLANE1:
        case GL_CLIP_PLANE2:
        case GL_CLIP_PLANE3:
        case GL_CLIP_PLANE4:
        case GL_CLIP_PLANE5:
        case GL_COLOR_ARRAY:
        case GL_COLOR_ARRAY_BUFFER_BINDING:
        case GL_COLOR_ARRAY_SIZE:
        case GL_COLOR_ARRAY_STRIDE:
        case GL_COLOR_ARRAY_TYPE:
        case GL_COLOR_LOGIC_OP:
        case GL_COLOR_MATERIAL:
        case GL_PACK_ALIGNMENT:
        case GL_PERSPECTIVE_CORRECTION_HINT:
            // case GL_POINT_SIZE_ARRAY_BUFFER_BINDING_OES:
            // case GL_POINT_SIZE_ARRAY_STRIDE_OES:
            // case GL_POINT_SIZE_ARRAY_TYPE_OES:
        case GL_POINT_SMOOTH:
        case GL_POINT_SMOOTH_HINT:
            // case GL_POINT_SPRITE_OES:
            // case GL_COORD_REPLACE_OES:
        case GL_COMBINE_ALPHA:
        case GL_SRC0_RGB:
        case GL_SRC1_RGB:
        case GL_SRC2_RGB:
        case GL_OPERAND0_RGB:
        case GL_OPERAND1_RGB:
        case GL_OPERAND2_RGB:
        case GL_SRC0_ALPHA:
        case GL_SRC1_ALPHA:
        case GL_SRC2_ALPHA:
        case GL_OPERAND0_ALPHA:
        case GL_OPERAND1_ALPHA:
        case GL_OPERAND2_ALPHA:
        case GL_RGB_SCALE:
        case GL_ALPHA_SCALE:
        case GL_COMBINE_RGB:
        case GL_POLYGON_OFFSET_FACTOR:
        case GL_POLYGON_OFFSET_FILL:
        case GL_POLYGON_OFFSET_UNITS:
        case GL_RED_BITS:
        case GL_RESCALE_NORMAL:
        case GL_SAMPLE_ALPHA_TO_COVERAGE:
        case GL_SAMPLE_ALPHA_TO_ONE:
        case GL_SAMPLE_BUFFERS:
        case GL_SAMPLE_COVERAGE:
        case GL_SAMPLE_COVERAGE_INVERT:
        case GL_SAMPLE_COVERAGE_VALUE:
        case GL_SAMPLES:
        case GL_MAX_SAMPLES_EXT:
        case GL_MAX_ELEMENTS_VERTICES:
        case GL_MAX_VERTEX_UNIFORM_COMPONENTS:
        case GL_MAX_VERTEX_UNIFORM_BLOCKS:
        case GL_MAX_VERTEX_OUTPUT_COMPONENTS:
        case GL_MAX_FRAGMENT_UNIFORM_COMPONENTS:
        case GL_MAX_FRAGMENT_UNIFORM_BLOCKS:
        case GL_MAX_FRAGMENT_INPUT_COMPONENTS:
        case GL_MIN_PROGRAM_TEXEL_OFFSET:
        case GL_MAX_PROGRAM_TEXEL_OFFSET:
        case GL_MAX_COMBINED_UNIFORM_BLOCKS:
        case GL_MAX_VARYING_COMPONENTS:
        case GL_NUM_EXTENSIONS:
        case GL_VERTEX_ARRAY_BINDING:
        case GL_BLEND_SRC_RGB:
        case GL_BLEND_SRC_ALPHA:
        case GL_BLEND_DST_RGB:
        case GL_BLEND_DST_ALPHA:
        case GL_BLEND_EQUATION:
        case GL_BLEND_EQUATION_ALPHA:
            // case GL_BLEND_EQUATION_RGB: //duplicated case
        case GL_DRAW_BUFFER0:
        case GL_DRAW_BUFFER1:
        case GL_DRAW_BUFFER2:
        case GL_DRAW_BUFFER3:
        case GL_DRAW_BUFFER4:
        case GL_DRAW_BUFFER5:
        case GL_DRAW_BUFFER6:
        case GL_DRAW_BUFFER7:
        case GL_DRAW_BUFFER8:
        case GL_DRAW_BUFFER9:
        case GL_DRAW_BUFFER10:
        case GL_DRAW_BUFFER11:
        case GL_DRAW_BUFFER12:
        case GL_DRAW_BUFFER13:
        case GL_DRAW_BUFFER14:
        case GL_DRAW_BUFFER15:
        case GL_FRAGMENT_SHADER_DERIVATIVE_HINT:
        case GL_TRANSFORM_FEEDBACK_BUFFER_START:
        case GL_TRANSFORM_FEEDBACK_BUFFER_SIZE:
        case GL_TRANSFORM_FEEDBACK_PAUSED:
        case GL_STENCIL_BITS:
        case GL_STENCIL_CLEAR_VALUE:
        case GL_STENCIL_FUNC:
        case GL_STENCIL_TEST:
        case GL_STENCIL_VALUE_MASK:
        case GL_STENCIL_BACK_FUNC:
        case GL_STENCIL_BACK_VALUE_MASK:
        case GL_STENCIL_BACK_REF:
        case GL_STENCIL_BACK_FAIL:
        case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
        case GL_STENCIL_BACK_PASS_DEPTH_PASS:
        case GL_STENCIL_BACK_WRITEMASK:
        case GL_TEXTURE_2D:
        case GL_TEXTURE_BASE_LEVEL:
        case GL_TEXTURE_BINDING_2D:
        case GL_TEXTURE_BINDING_EXTERNAL_OES:
        case GL_TEXTURE_BINDING_CUBE_MAP:
            // case GL_TEXTURE_BINDING_EXTERNAL_OES:
        case GL_TEXTURE_COMPARE_FUNC:
        case GL_TEXTURE_COMPARE_MODE:
        case GL_TEXTURE_COORD_ARRAY:
        case GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING:
        case GL_TEXTURE_COORD_ARRAY_SIZE:
        case GL_TEXTURE_COORD_ARRAY_STRIDE:
        case GL_TEXTURE_COORD_ARRAY_TYPE:
        case GL_TEXTURE_IMMUTABLE_FORMAT:
        case GL_UNPACK_ALIGNMENT:
        case GL_VERTEX_ARRAY:
        case GL_VERTEX_ARRAY_BUFFER_BINDING:
        case GL_VERTEX_ARRAY_SIZE:
        case GL_VERTEX_ARRAY_STRIDE:
        case GL_VERTEX_ARRAY_TYPE:
        case GL_SPOT_CUTOFF:
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_MAG_FILTER:
        case GL_TEXTURE_MAX_LOD:
        case GL_TEXTURE_MIN_LOD:
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
        case GL_TEXTURE_SWIZZLE_R:
        case GL_TEXTURE_SWIZZLE_G:
        case GL_TEXTURE_SWIZZLE_B:
        case GL_TEXTURE_SWIZZLE_A:
        case GL_GENERATE_MIPMAP:
        case GL_GENERATE_MIPMAP_HINT:
            // case GL_RENDERBUFFER_WIDTH_OES:
            // case GL_RENDERBUFFER_HEIGHT_OES:
            // case GL_RENDERBUFFER_INTERNAL_FORMAT_OES:
            // case GL_RENDERBUFFER_RED_SIZE_OES:
            // case GL_RENDERBUFFER_GREEN_SIZE_OES:
            // case GL_RENDERBUFFER_BLUE_SIZE_OES:
            // case GL_RENDERBUFFER_ALPHA_SIZE_OES:
            // case GL_RENDERBUFFER_DEPTH_SIZE_OES:
            // case GL_RENDERBUFFER_STENCIL_SIZE_OES:
        case GL_RENDERBUFFER_BINDING:
        case GL_FRAMEBUFFER_BINDING:
            // case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES:
            // case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES:
            // case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES:
            // case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES:
        case GL_FENCE_STATUS_NV:
        case GL_FENCE_CONDITION_NV:
            // case GL_TEXTURE_WIDTH_QCOM:
            // case GL_TEXTURE_HEIGHT_QCOM:
            // case GL_TEXTURE_DEPTH_QCOM:
            // case GL_TEXTURE_INTERNAL_FORMAT_QCOM:
            // case GL_TEXTURE_FORMAT_QCOM:
            // case GL_TEXTURE_TYPE_QCOM:
            // case GL_TEXTURE_IMAGE_VALID_QCOM:
            // case GL_TEXTURE_NUM_LEVELS_QCOM:
            // case GL_TEXTURE_TARGET_QCOM:
            // case GL_TEXTURE_OBJECT_VALID_QCOM:
            // case GL_BLEND_EQUATION_RGB_OES:
            // case GL_BLEND_EQUATION_ALPHA_OES:
            // case GL_BLEND_DST_RGB_OES:
            // case GL_BLEND_SRC_RGB_OES:
            // case GL_BLEND_DST_ALPHA_OES:
            // case GL_BLEND_SRC_ALPHA_OES:
        case GL_MAX_LIGHTS:
        case GL_SHADER_TYPE:
        case GL_COMPILE_STATUS:
        case GL_SHADER_SOURCE_LENGTH:
        case GL_CURRENT_PROGRAM:
        case GL_SUBPIXEL_BITS:
        case GL_MAX_3D_TEXTURE_SIZE:
        case GL_MAX_ARRAY_TEXTURE_LAYERS:
        case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
        case GL_NUM_SHADER_BINARY_FORMATS:
        case GL_SHADER_COMPILER:
        case GL_MAX_VERTEX_ATTRIBS:
        case GL_MAX_VERTEX_UNIFORM_VECTORS:
        case GL_MAX_VARYING_VECTORS:
        case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
        case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
        case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
        case GL_MAX_RENDERBUFFER_SIZE:
        case GL_MAX_TEXTURE_IMAGE_UNITS:
            // case GL_REQUIRED_TEXTURE_IMAGE_UNITS_OES:
            // case GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES:
        case GL_LINE_WIDTH:
        case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS:
        case GL_MAX_UNIFORM_BUFFER_BINDINGS:
        case GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS:
        case GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS:
        case GL_UNIFORM_BLOCK_BINDING:
        case GL_UNIFORM_BLOCK_DATA_SIZE:
        case GL_UNIFORM_BLOCK_NAME_LENGTH:
        case GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS:
        case GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER:
        case GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER:
        case GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT:
        case GL_CURRENT_QUERY:
        case GL_QUERY_RESULT:
        case GL_QUERY_RESULT_AVAILABLE:
        case GL_READ_BUFFER:
        case GL_NUM_PROGRAM_BINARY_FORMATS:
        case GL_ACTIVE_ATOMIC_COUNTER_BUFFERS:
        case GL_ACTIVE_ATTRIBUTES:
        case GL_ACTIVE_ATTRIBUTE_MAX_LENGTH:
        case GL_ACTIVE_UNIFORM_BLOCKS:
        case GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH:
        case GL_ACTIVE_UNIFORMS:
        case GL_ACTIVE_UNIFORM_MAX_LENGTH:
        case GL_ATTACHED_SHADERS:
        case GL_DELETE_STATUS:
        case GL_INFO_LOG_LENGTH:
        case GL_LINK_STATUS:
        case GL_PROGRAM_BINARY_LENGTH:
        case GL_PROGRAM_BINARY_RETRIEVABLE_HINT:
        case GL_PROGRAM_SEPARABLE:
        case GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT:
        case GL_TRANSFORM_FEEDBACK_BUFFER_MODE:
        case GL_TRANSFORM_FEEDBACK_VARYINGS:
        case GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH:
        case GL_TRANSFORM_FEEDBACK_ACTIVE:
#define GL_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
        case GL_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS:
        case GL_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS:
        case GL_VALIDATE_STATUS:
        case GL_ACTIVE_RESOURCES:
        case GL_MAX_IMAGE_UNITS:
        case GL_FRAMEBUFFER_DEFAULT_WIDTH:
        case GL_FRAMEBUFFER_DEFAULT_HEIGHT:
        case GL_FRAMEBUFFER_DEFAULT_SAMPLES:
        case GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS:
        case GL_FRAMEBUFFER_DEFAULT_LAYERS:
        case GL_MAX_NAME_LENGTH:
        case GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT:
            s = 1;
            break;
        case GL_ALIASED_LINE_WIDTH_RANGE:
        case GL_ALIASED_POINT_SIZE_RANGE:
        case GL_DEPTH_RANGE:
        case GL_MAX_VIEWPORT_DIMS:
        case GL_SMOOTH_POINT_SIZE_RANGE:
        case GL_SMOOTH_LINE_WIDTH_RANGE:
        case GL_SAMPLE_POSITION:
            s = 2;
            break;
        case GL_SPOT_DIRECTION:
        case GL_POINT_DISTANCE_ATTENUATION:
        case GL_CURRENT_NORMAL:
        case GL_COMPUTE_WORK_GROUP_SIZE:
            s = 3;
            break;
        case GL_CURRENT_VERTEX_ATTRIB:
        case GL_CURRENT_TEXTURE_COORDS:
        case GL_CURRENT_COLOR:
        case GL_FOG_COLOR:
        case GL_AMBIENT:
        case GL_DIFFUSE:
        case GL_SPECULAR:
        case GL_EMISSION:
        case GL_POSITION:
        case GL_LIGHT_MODEL_AMBIENT:
        case GL_TEXTURE_ENV_COLOR:
        case GL_SCISSOR_BOX:
        case GL_VIEWPORT:
            // case GL_TEXTURE_CROP_RECT_OES:
        case GL_COLOR_CLEAR_VALUE:
        case GL_COLOR_WRITEMASK:
        case GL_AMBIENT_AND_DIFFUSE:
        case GL_BLEND_COLOR:
        case GL_TEXTURE_BORDER_COLOR:
            s = 4;
            break;
        case GL_MODELVIEW_MATRIX:
        case GL_PROJECTION_MATRIX:
        case GL_TEXTURE_MATRIX:
            s = 16;
            break;
        case GL_MAX_ELEMENT_INDEX:
            s = 2;
            break;
        case GL_MAX_TEXTURE_LOD_BIAS:
            s = 1;
            break;
        case GL_MAX_ELEMENTS_INDICES:
            s = 2;
            break;
        case GL_MAX_SERVER_WAIT_TIMEOUT:
            s = 2;
            break;
        case GL_MAX_UNIFORM_BLOCK_SIZE:
            s = 2;
            break;
        case GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS:
            s = 2;
            break;
        case GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS:
            s = 2;
            break;
        case GL_PROGRAM_BINARY_FORMATS:
            s = 8;
            break;
        case GL_SHADER_BINARY_FORMATS:
            s = 8;
            break;
        case GL_COMPRESSED_TEXTURE_FORMATS:
            s = 128;
            break;
        case GL_TEXTURE_CROP_RECT_OES:
            s = 4;
            break;
        default:
            ALOGE("gl_pname_size: unknow pname 0x%08x\n", pname);
            s = 4;  // assume 1
            break;
    }
    return s;
}

void GLHelper::prepare_integer_value(Static_Context_Values *s_values) {
    s_values->implementation_color_read_type = 5121;
    s_values->implementation_color_read_format = 6408;
    s_values->max_array_texture_layers = 2048;
    s_values->max_color_attachments = 8;
    s_values->max_combined_uniform_blocks = 84;
    s_values->max_draw_buffers = 16;
    s_values->max_fragment_input_components = 128;
    s_values->max_fragment_uniform_blocks = 14;
    s_values->max_program_texel_offset = 7;
    s_values->max_transform_feedback_interleaved_components = 128;
    s_values->max_transform_feedback_separate_attribs = 4;
    s_values->max_transform_feedback_separate_components = 4;
    s_values->max_uniform_buffer_bindings = 84;
    s_values->max_varying_components = 124;
    s_values->max_varying_vectors = 31;
    s_values->max_vertex_output_components = 128;
    s_values->max_vertex_uniform_blocks = 14;
    s_values->min_program_texel_offset = -8;
    s_values->max_uniform_block_size = 65536;
    s_values->aliased_point_size_range[0] = 1.0f;
    s_values->aliased_point_size_range[1] = 2047.0f;

    int temp_int_value[128];

    glGetIntegerv(GL_NUM_EXTENSIONS, (GLint *)temp_int_value);
    s_values->num_extensions = temp_int_value[0];

    glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, (GLint *)temp_int_value);
    s_values->num_shader_binary_formats = temp_int_value[0];
    glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, (GLint *)temp_int_value);
    s_values->num_compressed_texture_formats = temp_int_value[0];
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, (GLint *)temp_int_value);
    s_values->num_program_binary_formats = temp_int_value[0];

    int *temp_int_array =
        new int[(std::max(std::max(s_values->num_shader_binary_formats,
                                   s_values->num_compressed_texture_formats),
                          s_values->num_program_binary_formats) *
                 sizeof(int))];

    glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, (GLint *)temp_int_array);
    memcpy(
        s_values->compressed_texture_formats, temp_int_array,
        std::max(s_values->num_compressed_texture_formats, 128) * sizeof(int));
    glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, (GLint *)temp_int_array);
    memcpy(s_values->program_binary_formats, temp_int_array,
           std::max(s_values->num_program_binary_formats, 8) * sizeof(int));
    glGetIntegerv(GL_SHADER_BINARY_FORMATS, (GLint *)temp_int_array);
    memcpy(s_values->shader_binary_formats, temp_int_array,
           std::max(s_values->num_shader_binary_formats, 8) * sizeof(int));
    glGetIntegerv(GL_SUBPIXEL_BITS, (GLint *)temp_int_value);
    s_values->subpixel_bits = temp_int_value[0];
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, (GLint *)temp_int_value);
    s_values->max_3d_texture_size = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint *)temp_int_value);
    s_values->max_combined_texture_image_units = temp_int_value[0];
    glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, (GLint *)temp_int_value);
    s_values->max_cube_map_texture_size = temp_int_value[0];
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, (GLint *)temp_int_value);
    s_values->max_elements_vertices = temp_int_value[0];
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, (GLint *)temp_int_value);
    s_values->max_elements_indices = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, (GLint *)temp_int_value);
    s_values->max_fragment_uniform_components = temp_int_value[0];
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, (GLint *)temp_int_value);
    s_values->max_renderbuffer_size = temp_int_value[0];
    glGetIntegerv(GL_MAX_SAMPLES, (GLint *)temp_int_value);
    s_values->max_samples = temp_int_value[0];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint *)temp_int_value);
    s_values->max_texture_size = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint *)temp_int_value);
    s_values->max_vertex_attribs = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, (GLint *)temp_int_value);
    s_values->max_vertex_uniform_components = temp_int_value[0];

    glGetIntegerv(GL_MAX_IMAGE_UNITS, (GLint *)temp_int_value);
    s_values->max_image_units = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, (GLint *)temp_int_value);
    s_values->max_vertex_attrib_bindings = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, (GLint *)temp_int_value);
    s_values->max_computer_uniform_blocks = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, (GLint *)temp_int_value);
    s_values->max_computer_texture_image_units = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_IMAGE_UNIFORMS, (GLint *)temp_int_value);
    s_values->max_computer_image_uniforms = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, (GLint *)temp_int_value);
    s_values->max_computer_sharde_memory_size = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, (GLint *)temp_int_value);
    s_values->max_computer_uniform_components = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS,
                  (GLint *)temp_int_value);
    s_values->max_computer_atomic_counter_buffers = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_ATOMIC_COUNTERS, (GLint *)temp_int_value);
    s_values->max_computer_atomic_counters = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS,
                  (GLint *)temp_int_value);
    s_values->max_combined_compute_uniform_components = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS,
                  (GLint *)temp_int_value);
    s_values->max_computer_work_group_invocations = temp_int_value[0];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0,
                    (GLint *)(temp_int_value));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1,
                    (GLint *)(temp_int_value + 1));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2,
                    (GLint *)(temp_int_value + 2));
    memcpy(s_values->max_computer_work_group_count, temp_int_array,
           3 * sizeof(int));

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0,
                    (GLint *)(temp_int_value));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1,
                    (GLint *)(temp_int_value + 1));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2,
                    (GLint *)(temp_int_value + 2));
    memcpy(s_values->max_computer_work_group_size, temp_int_array,
           3 * sizeof(int));

    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, (GLint *)temp_int_value);
    s_values->max_uniform_locations = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, (GLint *)temp_int_value);
    s_values->max_framebuffer_width = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, (GLint *)temp_int_value);
    s_values->max_framebuffer_height = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, (GLint *)temp_int_value);
    s_values->max_framebuffer_samples = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS,
                  (GLint *)temp_int_value);
    s_values->max_vertex_atomic_counter_buffers = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS,
                  (GLint *)temp_int_value);
    s_values->max_fragment_atomic_counter_buffers = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS,
                  (GLint *)temp_int_value);
    s_values->max_combined_atomic_counter_buffers = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAGMENT_ATOMIC_COUNTERS, (GLint *)temp_int_value);
    s_values->max_fragment_atomic_counters = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMBINED_ATOMIC_COUNTERS, (GLint *)temp_int_value);
    s_values->max_combined_atomic_counters = temp_int_value[0];
    glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE, (GLint *)temp_int_value);
    s_values->max_atomic_counter_buffer_size = temp_int_value[0];
    glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS,
                  (GLint *)temp_int_value);
    s_values->max_atomic_counter_buffer_bindings = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_IMAGE_UNIFORMS, (GLint *)temp_int_value);
    s_values->max_vertex_image_uniforms = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAGMENT_IMAGE_UNIFORMS, (GLint *)temp_int_value);
    s_values->max_fragment_image_uniforms = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMBINED_IMAGE_UNIFORMS, (GLint *)temp_int_value);
    s_values->max_combined_image_uniforms = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, (GLint *)temp_int_value);
    s_values->max_vertex_shader_storage_blocks = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS,
                  (GLint *)temp_int_value);
    s_values->max_fragment_shader_storage_blocks = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS,
                  (GLint *)temp_int_value);
    s_values->max_compute_shader_storage_blocks = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS,
                  (GLint *)temp_int_value);
    s_values->max_combined_shader_storage_blocks = temp_int_value[0];
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,
                  (GLint *)temp_int_value);
    s_values->max_shader_storage_buffer_bindings = temp_int_value[0];
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, (GLint *)temp_int_value);
    s_values->max_shader_storage_block_size = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES,
                  (GLint *)temp_int_value);
    s_values->max_combined_shader_output_resources = temp_int_value[0];
    glGetIntegerv(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET,
                  (GLint *)temp_int_value);
    s_values->min_program_texture_gather_offset = temp_int_value[0];
    glGetIntegerv(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET,
                  (GLint *)temp_int_value);
    s_values->max_program_texture_gather_offset = temp_int_value[0];
    glGetIntegerv(GL_MAX_SAMPLE_MASK_WORDS, (GLint *)temp_int_value);
    s_values->max_sample_mask_words = temp_int_value[0];
    glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, (GLint *)temp_int_value);
    s_values->max_color_texture_samples = temp_int_value[0];
    glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, (GLint *)temp_int_value);
    s_values->max_depth_texture_samples = temp_int_value[0];
    glGetIntegerv(GL_MAX_INTEGER_SAMPLES, (GLint *)temp_int_value);
    s_values->max_integer_samples = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET,
                  (GLint *)temp_int_value);
    s_values->max_vertex_attrib_relative_offset = temp_int_value[0];
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, (GLint
    // *)&(s_values->max_vertex_attrib_bindings));
    glGetIntegerv(GL_MAX_VERTEX_ATTRIB_STRIDE, (GLint *)temp_int_value);
    s_values->max_vertex_attrib_stride = temp_int_value[0];

    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, (GLint *)temp_int_value);
    s_values->max_vertex_texture_image_units = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, (GLint *)temp_int_value);
    s_values->max_vertex_uniform_vectors = temp_int_value[0];
    // s_values->max_vertex_uniform_vectors = 256;
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, (GLint *)temp_int_value);
    s_values->max_viewport_dims[0] = temp_int_value[0];
    s_values->max_viewport_dims[1] = temp_int_value[1];

    // glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, (GLint
    // *)&(s_values->uniform_buffer_offset_alignment));
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, (GLint *)temp_int_value);
    s_values->max_array_texture_layers = temp_int_value[0];
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (GLint *)temp_int_value);
    s_values->max_color_attachments = temp_int_value[0];
    glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, (GLint *)temp_int_value);
    s_values->max_combined_uniform_blocks = temp_int_value[0];
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, (GLint *)temp_int_value);
    s_values->max_draw_buffers = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, (GLint *)temp_int_value);
    s_values->max_fragment_input_components = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, (GLint *)temp_int_value);
    s_values->max_fragment_uniform_blocks = temp_int_value[0];
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, (GLint *)temp_int_value);
    s_values->max_fragment_uniform_vectors = temp_int_value[0];
    // s_values->max_fragment_uniform_vectors = 256;
    glGetIntegerv(GL_MAX_PROGRAM_TEXEL_OFFSET, (GLint *)temp_int_value);
    s_values->max_program_texel_offset = temp_int_value[0];
    glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS,
                  (GLint *)temp_int_value);
    s_values->max_transform_feedback_separate_attribs = temp_int_value[0];
    glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS,
                  (GLint *)temp_int_value);
    s_values->max_transform_feedback_separate_components = temp_int_value[0];
    glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS,
                  (GLint *)temp_int_value);
    s_values->max_transform_feedback_interleaved_components = temp_int_value[0];
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, (GLint *)temp_int_value);
    s_values->max_uniform_buffer_bindings = temp_int_value[0];
    // glGetIntegerv(GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS, (GLint
    // *)&(s_values->max_atomic_counter_buffer_bindings));
    // glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, (GLint
    // *)&(s_values->max_shader_storage_buffer_bindings));
    glGetIntegerv(GL_MAX_VARYING_VECTORS, (GLint *)temp_int_value);
    s_values->max_varying_vectors = temp_int_value[0];
    glGetIntegerv(GL_MAX_VARYING_COMPONENTS, (GLint *)temp_int_value);
    s_values->max_varying_components = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, (GLint *)temp_int_value);
    s_values->max_vertex_output_components = temp_int_value[0];
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, (GLint *)temp_int_value);
    s_values->max_vertex_uniform_blocks = temp_int_value[0];
    glGetIntegerv(GL_MIN_PROGRAM_TEXEL_OFFSET, (GLint *)temp_int_value);
    s_values->min_program_texel_offset = temp_int_value[0];
    // glGetIntegerv(GL_SAMPLES, (GLint *)&(s_values->samples));
    // glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, (GLint
    // *)&(s_values->shader_storage_buffer_offset_alignment));
    glGetIntegerv(GL_SUBPIXEL_BITS, (GLint *)temp_int_value);
    s_values->subpixel_bits = temp_int_value[0];

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint *)temp_int_value);
    s_values->texture_image_units = temp_int_value[0];
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, (GLint *)temp_int_value);
    s_values->uniform_buffer_offset_alignment = temp_int_value[0];
    //    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY, (GLint *)temp_int_value);
    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, (GLint *)temp_int_value);
    s_values->max_texture_anisotropy = temp_int_value[0];
    // s_values->uniform_buffer_offset_alignment = 1;

    GLfloat temp_float_value[2];
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, temp_float_value);
    s_values->aliased_line_width_range[0] = temp_float_value[0];
    s_values->aliased_line_width_range[1] = temp_float_value[1];
    glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, temp_float_value);
    s_values->aliased_point_size_range[0] = temp_float_value[0];
    s_values->aliased_point_size_range[1] = temp_float_value[1];
    glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS, temp_float_value);
    s_values->max_texture_log_bias = temp_float_value[0];

    GLint64 temp_int64_value;
    glGetInteger64v(GL_MAX_ELEMENT_INDEX, &temp_int64_value);
    s_values->max_element_index = temp_int64_value;
    glGetInteger64v(GL_MAX_SERVER_WAIT_TIMEOUT, &temp_int64_value);
    s_values->max_server_wait_timeout = temp_int64_value;
    glGetInteger64v(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,
                    &temp_int64_value);
    s_values->max_combined_vertex_uniform_components = temp_int64_value;
    glGetInteger64v(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,
                    &temp_int64_value);
    s_values->max_combined_fragment_uniform_components = temp_int64_value;
    glGetInteger64v(GL_MAX_UNIFORM_BLOCK_SIZE, &temp_int64_value);
    s_values->max_uniform_block_size = temp_int64_value;

#define PRECISION_TYPE_SIZE 6
    GLenum precision_types[PRECISION_TYPE_SIZE] = {
            GL_LOW_FLOAT, GL_MEDIUM_FLOAT, GL_HIGH_FLOAT,
            GL_LOW_INT, GL_MEDIUM_INT, GL_HIGH_INT};
    for (int i = 0; i < PRECISION_TYPE_SIZE; i++) {
        glGetShaderPrecisionFormat(GL_VERTEX_SHADER, precision_types[i],
                                   s_values->shader_precision_ranges + i * 2,
                                   s_values->shader_precisions + i);
        glGetShaderPrecisionFormat(GL_FRAGMENT_SHADER, precision_types[i],
                                   s_values->shader_precision_ranges + i * 2 + PRECISION_TYPE_SIZE * 2,
                                   s_values->shader_precisions + i + PRECISION_TYPE_SIZE);
    }
}

size_t GLHelper::gl_sizeof(GLenum type) {
    size_t retval = 0;
    switch (type) {
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            retval = 1;
            break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
        case GL_HALF_FLOAT:
            // case GL_HALF_FLOAT_OES:
            retval = 2;
            break;
        case GL_IMAGE_2D:
        case GL_IMAGE_3D:
        case GL_UNSIGNED_INT:
        case GL_INT:
        case GL_FLOAT:
        case GL_FIXED:
        case GL_BOOL:
            retval = 4;
            break;
#ifdef GL_DOUBLE
        case GL_DOUBLE:
            retval = 8;
            break;
        case GL_DOUBLE_VEC2:
            retval = 16;
            break;
        case GL_DOUBLE_VEC3:
            retval = 24;
            break;
        case GL_DOUBLE_VEC4:
            retval = 32;
            break;
        case GL_DOUBLE_MAT2:
            retval = 8 * 4;
            break;
        case GL_DOUBLE_MAT3:
            retval = 8 * 9;
            break;
        case GL_DOUBLE_MAT4:
            retval = 8 * 16;
            break;
        case GL_DOUBLE_MAT2x3:
        case GL_DOUBLE_MAT3x2:
            retval = 8 * 6;
            break;
        case GL_DOUBLE_MAT2x4:
        case GL_DOUBLE_MAT4x2:
            retval = 8 * 8;
            break;
        case GL_DOUBLE_MAT3x4:
        case GL_DOUBLE_MAT4x3:
            retval = 8 * 12;
            break;
#endif
        case GL_FLOAT_VEC2:
        case GL_INT_VEC2:
        case GL_UNSIGNED_INT_VEC2:
        case GL_BOOL_VEC2:
            retval = 8;
            break;
        case GL_INT_VEC3:
        case GL_UNSIGNED_INT_VEC3:
        case GL_BOOL_VEC3:
        case GL_FLOAT_VEC3:
            retval = 12;
            break;
        case GL_FLOAT_VEC4:
        case GL_BOOL_VEC4:
        case GL_INT_VEC4:
        case GL_UNSIGNED_INT_VEC4:
        case GL_FLOAT_MAT2:
            retval = 16;
            break;
        case GL_FLOAT_MAT3:
            retval = 36;
            break;
        case GL_FLOAT_MAT4:
            retval = 64;
            break;
        case GL_FLOAT_MAT2x3:
        case GL_FLOAT_MAT3x2:
            retval = 4 * 6;
            break;
        case GL_FLOAT_MAT2x4:
        case GL_FLOAT_MAT4x2:
            retval = 4 * 8;
            break;
        case GL_FLOAT_MAT3x4:
        case GL_FLOAT_MAT4x3:
            retval = 4 * 12;
            break;
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
        case GL_SAMPLER_CUBE:
        case GL_SAMPLER_2D_SHADOW:
        case GL_SAMPLER_2D_ARRAY:
        case GL_SAMPLER_2D_ARRAY_SHADOW:
        case GL_SAMPLER_2D_MULTISAMPLE:
        case GL_SAMPLER_CUBE_SHADOW:
        case GL_INT_SAMPLER_2D:
        case GL_INT_SAMPLER_3D:
        case GL_INT_SAMPLER_CUBE:
        case GL_INT_SAMPLER_2D_ARRAY:
        case GL_INT_SAMPLER_2D_MULTISAMPLE:
        case GL_UNSIGNED_INT_SAMPLER_2D:
        case GL_UNSIGNED_INT_SAMPLER_3D:
        case GL_UNSIGNED_INT_SAMPLER_CUBE:
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
        case GL_IMAGE_CUBE:
        case GL_IMAGE_2D_ARRAY:
        case GL_INT_IMAGE_2D:
        case GL_INT_IMAGE_3D:
        case GL_INT_IMAGE_CUBE:
        case GL_INT_IMAGE_2D_ARRAY:
        case GL_UNSIGNED_INT_IMAGE_2D:
        case GL_UNSIGNED_INT_IMAGE_3D:
        case GL_UNSIGNED_INT_IMAGE_CUBE:
        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
        case GL_UNSIGNED_INT_ATOMIC_COUNTER:
        case GL_UNSIGNED_INT_IMAGE_BUFFER:
        case GL_INT_IMAGE_BUFFER:
        case GL_IMAGE_BUFFER:
            retval = 4;
            break;
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT:
        case GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT:
            retval = 2;
            break;
        case GL_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_10F_11F_11F_REV:
        case GL_UNSIGNED_INT_5_9_9_9_REV:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
        case GL_UNSIGNED_INT_24_8_OES:
            retval = 4;
            break;
        case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
            retval = 4 + 4;
            break;
        default:
            ALOGD("**** ERROR unknown type 0x%x (%s,%d)\n", type, __FUNCTION__,
                  __LINE__);
            retval = 4;
    }
    return retval;
}

void GLHelper::d_glPrintf(void *context, GLint buf_len,
                          const GLchar *out_string) {
    // char *t="temp test abcd";
    // memcpy(out_string,t,strlen(t));
    char *temp = (char *)malloc(buf_len);
    // TODO: guest_write
    //    guest_write((Guest_Mem *)out_string, temp, 0, buf_len);
    memcpy(temp, out_string, buf_len);

    if (buf_len < 100) {
        ALOGD("glALOGD %d %s\n", buf_len, temp);
    } else {
        int flag = 1;
        for (int i = 0; i < buf_len; i++) {
            if (temp[i] != 'c') {
                flag = 0;
            }
        }
        if (flag == 0) {
            ALOGD("glALOGD check error!\n");
        } else {
            ALOGD("glALOGD check ok!\n");
        }
    }
    free(temp);
}

GLint GLHelper::glTestInt1(GLint a, GLuint b) {
    ALOGD("glTestInt1 %d,%u\n", a, b);
    return 576634565;
}

GLuint GLHelper::glTestInt2(GLint a, GLuint b) {
    ALOGD("glTestInt2 %d,%u\n", a, b);
    return 4000001200u;
}

GLint64 GLHelper::glTestInt3(GLint64 a, GLuint64 b) {
    ALOGD("glTestInt3 %lld,%llu\n", a, b);
    return 453489431344456;
}

GLuint64 GLHelper::glTestInt4(GLint64 a, GLuint64 b) {
    ALOGD("glTestInt4 %lld,%llu\n", a, b);
    return 436004354364364345;
}

GLfloat GLHelper::glTestInt5(GLint a, GLuint b) {
    ALOGD("glTestInt5 %d,%u\n", a, b);
    return 3.1415926;
}

GLdouble GLHelper::glTestInt6(GLint a, GLuint b) {
    ALOGD("glTestInt6 %d,%u\n", a, b);
    return 3.1415926535;
}

void GLHelper::glTestPointer1(GLint a, const GLint *b) {
    ALOGD("glTestPointer1 %d ", a);
    for (int i = 0; i < 10; i++) {
        ALOGD("%d ", b[i]);
    }
    ALOGD("\n");
}

void GLHelper::glTestPointer2(GLint a, const GLint *b, GLint *c) {
    ALOGD("glTestPointer2 %d %d\n", a, *b);
    for (int i = 0; i < 10; i++) {
        c[i] = b[i];
    }
}

GLint GLHelper::glTestPointer4(GLint a, const GLint *b, GLint *c) {
    ALOGD("glTestPointer4 %d,%d\n", a, *b);
    for (int i = 0; i < 1000; i++) {
        c[i] = b[i];
    }
    return 12456687;
}

GLint GLHelper::d_glTestPointer3(void *context, GLint a, const GLint *b,
                                 GLint *c) {
    //    int len;
    //    char *temp = g_malloc(a * sizeof(int));
    //    memset(temp, 0, a * sizeof(int));
    //    ALOGD("glTestPointer3 %d\n", a);
    //    guest_write((Guest_Mem *) b, temp, 0, a * sizeof(int));
    //
    //    char *temp_s[100];
    //    int loc = 0;
    //    for (int i = a / 2; i < a / 2 + 10; i++) {
    //        loc += sALOGD(temp_s + loc, "%d ", temp[i]);
    //    }
    //    ALOGD("glTestPointer3 %s\n", temp_s);
    //
    //    guest_read((Guest_Mem *) c, temp, 0, a * sizeof(int));
    //
    //    fflush(stdout);
    return 12456687;
}

void GLHelper::glTestString(GLint a, GLint count, const GLchar *const *strings,
                            GLint buf_len, GLchar *char_buf) {
    ALOGD("glTestString %d %d %d\nString:\n", a, count, buf_len);
    for (int i = 0; i < count; i++) {
        ALOGD("%s\n", strings[i]);
    }
    const char *t = "ALOGD ok!";
    strcpy((char *)char_buf, t);
}

void GLHelper::glTestIntAsyn(GLint a, GLuint b, GLfloat c, GLdouble d) {
    ALOGD("glTestInt asyn %d,%u,%f,%lf\n", a, b, c, d);
    fflush(stdout);
}

void GLHelper::glPrintfAsyn(GLint a, GLuint size, GLdouble c,
                            const GLchar *out_string) {
    ALOGD("glALOGDAsyn asyn string %d,%u,%lf,%s\n", a, size, c, out_string);
}

GLuint GLHelper::load_shader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;

    shader = glCreateShader(type);

    if (shader == 0) {
        //    express_ALOGD("Shader==0\n");
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);

            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            ALOGE("Shader compile error %s", infoLog);

            free(infoLog);
        }

        glDeleteShader(shader);
        //    express_ALOGD("compiled==0\n");
        return 0;
    }

    return shader;
}

int GLHelper::main_window_opengl_prepare(GLuint *program, GLuint *VAO) {
    char vShaderStr[] =
        "#version 300 es\n"
        "layout (location = 0) in vec2 position;\n"
        "layout (location = 1) in vec2 texCoords;\n"
        "uniform int rotation;\n"
        "out vec2 TexCoords;\n"
        "void main()\n"
        "{\n"
        "    mat2 transform = mat2(0.0, 1.0, -1.0, 0.0);"
        "    if(rotation == 0)\n"
        "    {\n"
        "       gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);\n"
        "    }\n"
        "    else\n"
        "    {\n"
        "       vec2 rotated_position = position * transform;"
        "       gl_Position = vec4(rotated_position.x, rotated_position.y, 0.0f, 1.0f);\n"
        "    }\n"
        "    TexCoords = texCoords;\n"
        "}\n";

    char fShaderStr[] =
        "#version 300 es\n"
        "precision mediump float;                     \n"
        "in vec2 TexCoords;\n"
        "out vec4 color;\n"
        "uniform sampler2D screenTexture;\n"
        "void main(){\n"
        "color = texture(screenTexture, TexCoords);\n"
        "}\n";

    GLuint programObject = glCreateProgram();
    if (programObject == 0) {
        // express_ALOGD("shit glCreateProgram2 %ld\n", GetLastError());
        return 0;
    }

    GLuint vertexShader = load_shader(GL_VERTEX_SHADER, vShaderStr);
    GLuint fragmentShader = load_shader(GL_FRAGMENT_SHADER, fShaderStr);
    ALOGD("shader %d %d\n", vertexShader, fragmentShader);

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    glLinkProgram(programObject);

    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked) {

        ALOGE("Failed to compile program for main renderer, error is %x", glGetError());
        return 0;
    }

    GLfloat quadVertices[] = {// Vertex attributes for a quad that fills the
                              // entire screen in Normalized Device Coordinates.
                              // Positions   // TexCoords
                              -1.0f, 1.0f, 0.0f, 1.0f,  -1.0f, -1.0f,
                              0.0f,  0.0f, 1.0f, -1.0f, 1.0f,  0.0f,

                              -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  -1.0f,
                              1.0f,  0.0f, 1.0f, 1.0f,  1.0f,  1.0f};

    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid *)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    *program = programObject;
    *VAO = quadVAO;

    
    //  glEnable(GL_BLEND);
    //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(programObject);

    glClearColor(0, 0, 0, 1);

    return 1;
}