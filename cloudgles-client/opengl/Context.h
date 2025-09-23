#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <atomic>
#include <list>
#include <unordered_map>

#include "State.h"
#include "renderer/GraphicBuffer.h"

#define MAX_VERTEX_ATTRIBS_NUM 16

class MappedBuffer {
   public:
    GLubyte *data;
    unsigned long map_len;
    GLenum target;
    GLbitfield access;
};

class AttribPoint {
   public:
    GLuint buffer_object[MAX_VERTEX_ATTRIBS_NUM];
    GLint buffer_loc[MAX_VERTEX_ATTRIBS_NUM];

    GLint remain_buffer_len[MAX_VERTEX_ATTRIBS_NUM];
    GLint buffer_len[MAX_VERTEX_ATTRIBS_NUM];

    GLuint indices_buffer_object;
    GLint indices_buffer_len;

    GLint remain_indices_buffer_len;

    GLint element_array_buffer;

    static void g_vao_point_data_destroy(AttribPoint *data);
};

class BoundBuffer {
   public:
    AttribPoint *attrib_point;

    std::unordered_map<GLuint, AttribPoint *> vao_point_data;

    GLuint async_unpack_texture_buffer;
    GLuint async_pack_texture_buffer;

    BufferStatus buffer_status;

    int has_init;
};

class ResourceMapStatus {
   public:
    unsigned int max_id = 0;
    unsigned int map_size = 0;

    long long *resource_id_map = nullptr;
    char *resource_is_init;

    unsigned int gbuffer_map_max_size;
    GraphicBuffer **gbuffer_ptr_map;
};

class ShareResources {
   public:
    int counter;

    // Regular objects
    ResourceMapStatus texture_resource;
    ResourceMapStatus buffer_resource;
    ResourceMapStatus render_buffer_resource;
    ResourceMapStatus sample_resource;

    // GLSL objects, including program and shader
    ResourceMapStatus program_resource;
    ResourceMapStatus shader_resource;

    // EglSync objects
    ResourceMapStatus sync_resource;
};

class ExclusiveResources {
   public:
    // Container objects
    ResourceMapStatus frame_buffer_resource;
    ResourceMapStatus program_pipeline_resource;
    ResourceMapStatus transform_feedback_resource;
    ResourceMapStatus vertex_array_resource;

    // query objects
    ResourceMapStatus query_resource;
};

class ResourceContext {
   public:
    // Regular objects
    ResourceMapStatus *texture_resource;
    ResourceMapStatus *buffer_resource;
    ResourceMapStatus *render_buffer_resource;
    ResourceMapStatus *sampler_resource;

    // GLSL objects, include program,shader
    ResourceMapStatus *shader_resource;
    ResourceMapStatus *program_resource;

    // EglSync objects
    ResourceMapStatus *sync_resource;

    // Container objects
    ResourceMapStatus *frame_buffer_resource;
    ResourceMapStatus *program_pipeline_resource;
    ResourceMapStatus *transform_feedback_resource;
    ResourceMapStatus *vertex_array_resource;

    // query objects
    ResourceMapStatus *query_resource;

    ShareResources *share_resources;
    ExclusiveResources *exclusive_resources;
};

class GLContext {
   public:
    void *native_context;

    BoundBuffer bound_buffer_status;

    ResourceContext resource_status;

    void *share_context;
    std::unordered_map<uint64_t, MappedBuffer *> buffer_map;
    GLuint draw_fbo0;
    GLuint read_fbo0;
    GLuint vao0;

    GLint view_x;
    GLint view_y;

    GLsizei view_w;
    GLsizei view_h;

    EglSurface *draw_surface;

    int is_current;
    int need_destroy;
    EGLContext guest_context;
    int should_use_flush = 0;

    TextureBindingStatus texture_binding_status;
    GLuint is_using_external_program;

    GLuint enable_scissor;

    int independent_mode;

    // GLv1 Stuff
    GLuint draw_texi_vao;
    GLuint draw_texi_vbo;
    GLuint draw_texi_ebo;

    static std::list<GLContext *> native_context_pool;
    static std::atomic_flag native_context_pool_lock;

    std::atomic_int64_t mc_fence;
    std::atomic_flag mc_fence_lock = ATOMIC_FLAG_INIT;
    std::unordered_map<int64_t, void *> fence_event_map;

    static GLContext *opengl_context_create(GLContext *share_context,
                                            int independ_mode);
    static void g_context_map_destroy(GLContext *data);
    static void *get_native_opengl_context(int independ_mode);
    static void resource_context_init(ResourceContext *resources,
                                      ShareResources *share_resources);
    static void opengl_context_init(GLContext *context);
    static void opengl_context_destroy(GLContext *context);
    static void resource_context_destroy(ResourceContext *resources);
    static void release_native_opengl_context(void *native_context,
                                              int independ_mode);
    static void d_glGetString_special(void *context, GLenum name,
                                      GLubyte *buffer);
    static void d_glGetStringi_special(void *context, GLenum name, GLuint index,
                                       GLubyte *buffer);

    void fence_sync(int64_t fence);
    void signal_sync();
};

#endif  // GL_CONTEXT_H
