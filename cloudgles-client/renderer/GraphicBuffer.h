#ifndef GRAPHIC_BUFFER_H
#define GRAPHIC_BUFFER_H

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

#include <mutex>

#include "Event.h"

#define MAX_WINDOW_LIFE_TIME (10)
#define MAX_BITMAP_LIFE_TIME (60 * 15)

class EglSurface;

class GraphicBuffer {
   public:
    int is_writing;

    
    void *writing_ok_event;
    int is_lock;
    int sampler_num;

    GLuint data_texture;
    GLuint reverse_rbo;
    GLuint sampler_rbo;
    GLuint rbo_depth;
    GLuint rbo_stencil;

    
    GLuint data_fbo;
    GLuint sampler_fbo;

    
    int has_connected_fbo;

    GLsync data_sync;
    GLsync delete_sync;
    uint64_t gbuffer_id;

    int remain_life_time;
    int is_dying;
    int is_using;
    int need_reverse;

    int format;
    int pixel_type;
    int internal_format;
    int row_byte_len;
    int depth_internal_format;
    int stencil_internal_format;

    int hal_format;
    int stride;
    int width;
    int height;
    int usage_type;

    std::atomic_flag lock = ATOMIC_FLAG_INIT;

    void prepare_to_die();
    void do_not_die();

    static GraphicBuffer *create_gbuffer_from_surface(EglSurface *surface);
    static GraphicBuffer *create_gbuffer_from_hal(int width, int height,
                                                  int hal_format,
                                                  EglSurface *surface,
                                                  uint64_t gbuffer_id);
    static GraphicBuffer *create_gbuffer_with_context(int width, int height,
                                                      int hal_format,
                                                      void *t_context,
                                                      EGLContext ctx,
                                                      uint64_t gbuffer_id);
    static GraphicBuffer *create_gbuffer(int width, int height, int sampler_num,
                                         int format, int pixel_type,
                                         int internal_format,
                                         int depth_internal_format,
                                         int stencil_internal_format,
                                         uint64_t gbuffer_id);
    static void connect_gbuffer_to_surface(GraphicBuffer *gbuffer,
                                           EglSurface *surface);

    static void reverse_gbuffer(GraphicBuffer *gbuffer);
    static void destroy_gbuffer(GraphicBuffer *gbuffer);

    static void gbuffer_map_destroy(GraphicBuffer *data);
};

#endif  // GRAPHIC_BUFFER_H
