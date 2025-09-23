#ifndef GL_RESOURCE_H
#define GL_RESOURCE_H

#include <GLES/gl.h>

#include "Context.h"

class GLResource {
   public:
    static void d_glCreateShader(void *context, GLenum type, GLuint shader);
    static void d_glFenceSync(void *context, GLenum condition, GLbitfield flags,
                              GLsync sync);
    static void d_glGenFramebuffers(void *context, GLsizei n,
                                    const GLuint *framebuffers);
    static void d_glGenProgramPipelines(void *context, GLsizei n,
                                        const GLuint *pipelines);
    static void d_glGenTransformFeedbacks(void *context, GLsizei n,
                                          const GLuint *ids);
    static void d_glGenVertexArrays(void *context, GLsizei n,
                                    const GLuint *arrays);
    static void d_glGenQueries(void *context, GLsizei n, const GLuint *ids);
    static void d_glDeleteBuffers(void *context, GLsizei n,
                                  const GLuint *buffers);
    static void get_host_resource_ids(ResourceMapStatus *status, GLsizei n,
                                      const unsigned int *guest_ids,
                                      unsigned int *host_ids);
    static void remove_host_map_ids(ResourceMapStatus *status, int n,
                                    const unsigned int *guest_ids);
    static void d_glDeleteRenderbuffers(void *context, GLsizei n,
                                        const GLuint *renderbuffers);
    static void d_glDeleteTextures(void *context, GLsizei n,
                                   const GLuint *textures);
    static void d_glDeleteSamplers(void *context, GLsizei count,
                                   const GLuint *samplers);
    static void d_glDeleteProgram(void *context, GLuint program);
    static void d_glDeleteShader(void *context, GLuint shader);
    static void d_glDeleteSync(void *context, GLsync sync);
    static void d_glDeleteFramebuffers(void *context, GLsizei n,
                                       const GLuint *framebuffers);
    static void d_glDeleteProgramPipelines(void *context, GLsizei n,
                                           const GLuint *pipelines);
    static void d_glDeleteTransformFeedbacks(void *context, GLsizei n,
                                             const GLuint *ids);
    static void d_glDeleteVertexArrays(void *context, GLsizei n,
                                       const GLuint *arrays);
    static void d_glDeleteQueries(void *context, GLsizei n, const GLuint *ids);
    static unsigned long long get_host_program_id(void *context,
                                                  unsigned int id);
    static long long get_host_resource_id(ResourceMapStatus *status,
                                          unsigned int id);
    static unsigned long long get_host_sync_id(void *context, unsigned int id);
    static int create_host_map_ids(ResourceMapStatus *status, int n,
                                   const unsigned int *guest_ids,
                                   unsigned long long *host_ids);
    static unsigned long long get_host_shader_id(void *context,
                                                 unsigned int id);
    static unsigned long long get_host_buffer_id(void *context,
                                                 unsigned int id);

    static long long set_host_map_id(ResourceMapStatus *status, int guest_id,
                                     int host_id);

    static char set_host_resource_init(ResourceMapStatus *status,
                                       unsigned int id);

    static int guest_has_resource_id(ResourceMapStatus *status,
                                     unsigned int id);

    static char set_host_texture_init(void *context, unsigned int id);
    // char set_host_buffer_init(void *context, unsigned int id);

    static GraphicBuffer *get_texture_gbuffer_ptr(void *context,
                                                  GLuint texture);
    static void set_texture_gbuffer_ptr(void *context, GLuint texture,
                                        GraphicBuffer *gbuffer);

    static unsigned long long get_host_texture_id(void *context,
                                                  unsigned int id);
    static unsigned long long get_host_renderbuffer_id(void *context,
                                                       unsigned int id);
    static unsigned long long get_host_sampler_id(void *context,
                                                  unsigned int id);

    static unsigned long long get_host_framebuffer_id(void *context,
                                                      unsigned int id);
    static unsigned long long get_host_pipeline_id(void *context,
                                                   unsigned int id);
    static unsigned long long get_host_feedback_id(void *context,
                                                   unsigned int id);

    static unsigned long long get_host_query_id(void *context, unsigned int id);

    static void d_glGenBuffers(void *context, GLsizei n, const GLuint *buffers);

    static void d_glGenRenderbuffers(void *context, GLsizei n,
                                     const GLuint *renderbuffers);

    static void d_glGenTextures(void *context, GLsizei n,
                                const GLuint *textures);

    static long long set_share_texture(void *context, GLuint texture,
                                       GLuint share_texture);

    static void d_glGenSamplers(void *context, GLsizei count,
                                const GLuint *samplers);

    static void d_glCreateProgram(void *context, GLuint program);

    static void d_glCreateShaderProgramv_special(void *context, GLenum type,
                                                 GLsizei count,
                                                 const GLchar *const *strings,
                                                 GLuint program);

    static void g_delete_check_vao_ebo(void *key, void *data, void *user_data);

    static unsigned long long get_host_array_id(void *context, unsigned int id);

    static GLuint recorded_program;
};

#endif  // GL_RESOURCE_H
