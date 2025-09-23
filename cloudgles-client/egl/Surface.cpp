#define LOG_TAG "EglSurface"

#include "Surface.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES3/gl3.h>

#include "Log.h"
#include "Utils.h"
#include "egl/Context.h"
#include "opengl/Resource.h"
#include "renderer/MainController.h"
#include "renderer/MainRenderer.h"
#include "renderer/RenderThread.h"

EglSurface::EglSurface(EGLConfig egl_config, int surface_width,
                       int surface_height, int surface_type) {
    call_poller = new FrameCallPoller();
    reset_current_event = create_event(0, 0);
    type = surface_type;
    width = surface_width;
    height = surface_height;

    config = egl_default_display.egl_config_map[egl_config];

    EGLint default_internal_format = GL_RGB;
    GLint default_format = GL_RGB;
    GLenum default_type = GL_UNSIGNED_BYTE;

    // int row_byte_len = 4 * width;

    GLenum default_depth_internal_format = 0;
    GLenum default_stencil_internal_format = 0;

    EGLint red_bits = config->red_size;
    EGLint green_bits = config->green_size;
    EGLint blue_bits = config->blue_size;
    EGLint alpha_bits = config->alpha_size;
    EGLint stencil_bits = config->stencil_size;
    EGLint depth_bits = config->depth_size;
    if (config->sample_buffers_num != 0) {
        // printf("surface enable sample %d\n", config->sample_buffers_num);
        if (config->sample_buffers_num == -1) {
            config->sample_buffers_num = 0;
        }
    }
    // EGLint need_sampler = config->sample_buffers_num;
    EGLint default_sampler_num = config->samples_per_pixel;

    // ALOGD("rgba %d %d %d %d ds %d %d MSAA %dX\n", red_bits, green_bits,
    // blue_bits, alpha_bits, depth_bits, stencil_bits, default_sampler_num);

    // 2222
    // 3320
    // 4440
    // 4444
    // 5550
    // 5551
    // 5650
    // 8000
    // 8800
    // 8880
    // 8888
    // 1010100
    // 1010102
    // 1212120
    // 12121212
    // 1616160
    // 16161616
    if (red_bits == 4 && green_bits == 4 && blue_bits == 4 && alpha_bits == 4) {
        // 4444
        default_internal_format = GL_RGBA4;
        default_format = GL_RGBA;
        default_type = GL_UNSIGNED_BYTE;
        // row_byte_len = 2*width;
        // ALOGD("choose rgba 4444 ");
    } else if (red_bits == 5 && green_bits == 5 && blue_bits == 5 &&
               alpha_bits == 1) {
        // 5551
        default_internal_format = GL_RGB5_A1;
        default_format = GL_RGBA;
        default_type = GL_UNSIGNED_BYTE;
        // row_byte_len = 2*width;
        // ALOGD("choose rgba 5551 ");
    } else if (red_bits == 5 && green_bits == 6 && blue_bits == 5 &&
               alpha_bits == 0) {
        // 5650
        default_internal_format = GL_RGB565;
        default_format = GL_RGB;
        default_type = GL_UNSIGNED_BYTE;
        // row_byte_len = 2*width;
        // ALOGD("choose rgba 5650 ");
    } else if (red_bits == 8 && green_bits == 0 && blue_bits == 0 &&
               alpha_bits == 0) {
        // 8000
        default_internal_format = GL_R8;
        default_format = GL_RED;
        default_type = GL_UNSIGNED_BYTE;
        // row_byte_len = 1*width;
        // ALOGD("choose rgba 8000 ");
    } else if (red_bits == 8 && green_bits == 8 && blue_bits == 0 &&
               alpha_bits == 0) {
        // 8800
        default_internal_format = GL_RG8;
        default_format = GL_RG;
        default_type = GL_UNSIGNED_BYTE;
        // row_byte_len = 2*width;
        // ALOGD("choose rgba 8800 ");
    } else if (red_bits == 8 && green_bits == 8 && blue_bits == 8 &&
               alpha_bits == 0) {
        // 8880
        default_internal_format = GL_RGB8;
        default_format = GL_RGB;
        default_type = GL_UNSIGNED_BYTE;
        // row_byte_len = 3*width;
        // ALOGD("choose rgba 8880 ");
    } else if (red_bits == 8 && green_bits == 8 && blue_bits == 8 &&
               alpha_bits == 8) {
        // 8888
        default_internal_format = GL_RGBA8;
        default_format = GL_RGBA;
        default_type = GL_UNSIGNED_BYTE;
        // row_byte_len = 4*width;
        // ALOGD("choose rgba 8888 ");
    } else if (red_bits == 10 && green_bits == 10 && blue_bits == 10 &&
               alpha_bits == 2) {
        // 1010102
        default_internal_format = GL_RGB10_A2;
        default_format = GL_RGBA;
        default_type = GL_UNSIGNED_INT_2_10_10_10_REV;
        // row_byte_len = 4*width;
        // ALOGD("choose rgba 1010102 ");
    } else {
        ALOGE(
            "surface choose error red %d green %d blue %d alpha %d depth %d "
            "stencil %d width %d height %d\n",
            red_bits, green_bits, blue_bits, alpha_bits, depth_bits,
            stencil_bits, surface_width, surface_height);
    }

    if (default_internal_format == GL_RGB565) {
        default_internal_format = GL_RGB8;
        default_format = GL_RGB;
        default_type = GL_UNSIGNED_BYTE;
    }

    if (depth_bits == 16) {
        default_depth_internal_format = GL_DEPTH_COMPONENT16;
        // ALOGD("GL_DEPTH_COMPONENT16\n");
    } else if (depth_bits == 24) {
        default_depth_internal_format = GL_DEPTH_COMPONENT24;
        // ALOGD("GL_DEPTH_COMPONENT24\n");
    } else if (depth_bits == 32) {
        default_depth_internal_format = GL_DEPTH_COMPONENT32F;
        // ALOGD("GL_DEPTH_COMPONENT32F\n");
    }

    if (stencil_bits == 8) {
        default_stencil_internal_format = GL_STENCIL_INDEX8;
        // ALOGD("GL_STENCIL_INDEX8\n");
        if (default_depth_internal_format == GL_DEPTH_COMPONENT24 ||
            default_depth_internal_format == GL_DEPTH_COMPONENT16 ||
            default_depth_internal_format == GL_DEPTH_COMPONENT32F) {
            // default_depth_internal_format = GL_DEPTH_COMPONENT24;

            default_depth_internal_format = GL_DEPTH24_STENCIL8;
            // ALOGD("GL_DEPTH24_STENCIL8\n");
        }
    }

    sampler_num = default_sampler_num;
    format = default_format;
    internal_format = default_internal_format;
    pixel_type = default_type;
    depth_internal_format = default_depth_internal_format;
    stencil_internal_format = default_stencil_internal_format;
}

EglSurface::~EglSurface() {
    if (type == P_SURFACE && gbuffer != nullptr) {
        MainRenderer::get_instance()->send_window_message(MAIN_DESTROY_GBUFFER,
                                                          gbuffer);
    }
    delete call_poller;
    delete_event(reset_current_event);
}

void EglSurface::init() {
    int num = 1;
    if (type == WINDOW_SURFACE) {
        num = 3;
    }
    if (data_fbo[0] != 0) {
        return;
    }
    glGenFramebuffers(num, data_fbo);
    if (sampler_num > 1) {
        glGenFramebuffers(num, sampler_fbo);
    }
}

void EglSurface::reset() {
    is_current = 0;
    int num = 1;
    if (type == WINDOW_SURFACE) {
        num = 3;
    }
    if (data_fbo[0] == 0) {
        return;
    }
    glDeleteFramebuffers(num, data_fbo);
    memset(data_fbo, 0, sizeof(data_fbo));
    memset(connect_texture, 0, sizeof(connect_texture));

    if (sampler_num > 1) {
        glDeleteFramebuffers(num, sampler_fbo);
        memset(sampler_fbo, 0, sizeof(sampler_fbo));
    }
    set_event(reset_current_event);
}

void EglSurface::d_eglCreateWindowSurface(void *context, EGLDisplay dpy,
                                          EGLConfig config,
                                          EGLNativeWindowType win,
                                          const EGLint *attrib_list,
                                          EGLSurface guest_surface) {
    auto *thread_context = reinterpret_cast<ThreadContext *>(context);
    ProcessContext *process_context = thread_context->process_context.get();

    int i = 0;
    int width = 0;
    int height = 0;
    while (attrib_list != nullptr && attrib_list[i] != EGL_NONE) {
        switch (attrib_list[i]) {
            case EGL_WIDTH:
                width = attrib_list[i + 1];
                break;
            case EGL_HEIGHT:
                height = attrib_list[i + 1];
                break;
            default:
                printf("window_surface attrib_list %x %x\n", attrib_list[i],
                       attrib_list[i + 1]);
                break;
        }
        i += 2;
    }

    auto *host_surface = new EglSurface(config, width, height, WINDOW_SURFACE);
    ALOGD(
        "surface create host %llx guest %llx width %d height %d guest width %d "
        "height %d\n",
        host_surface, guest_surface, host_surface->width, host_surface->height,
        width, height);
    process_context->surface_map[guest_surface] = host_surface;
}

void EglSurface::set_gbuffer(uint64_t gbuffer_id, int hal_format,
                             int gbuffer_width, int gbuffer_height) {
    GraphicBuffer *target_gbuffer;
    if (type == WINDOW_SURFACE) {
        target_gbuffer =
            MainRenderer::get_instance()->get_gbuffer_from_global_map(
                gbuffer_id);
        if (target_gbuffer == nullptr) {
            target_gbuffer = GraphicBuffer::create_gbuffer_from_hal(
                gbuffer_width, gbuffer_height, hal_format, this, gbuffer_id);

            ALOGD(
                "create target_gbuffer when makecurrent target_gbuffer %llx "
                "ptr %llx\n",
                gbuffer_id, (int64_t)target_gbuffer);

            MainRenderer::get_instance()->add_gbuffer_to_global(target_gbuffer);
            MainRenderer::get_instance()->set_global_gbuffer_type(
                gbuffer_id, GBUFFER_TYPE_WINDOW);
        } else {
            target_gbuffer->do_not_die();
        }
    } else {
        // surface type is pbuffer
        target_gbuffer = gbuffer;
        if (target_gbuffer == nullptr) {
            ALOGD("create target_gbuffer for id %llx using pbuffer %llx\n",
                  gbuffer_id, (int64_t)this);
            target_gbuffer = GraphicBuffer::create_gbuffer_from_surface(this);

            target_gbuffer->gbuffer_id = 0;
            target_gbuffer->remain_life_time = 0;
        }
    }

    if (gbuffer != nullptr) {
        gbuffer->is_writing = 0;
        set_event(gbuffer->writing_ok_event);
    }

    gbuffer = target_gbuffer;

    GraphicBuffer::connect_gbuffer_to_surface(gbuffer, this);
}

void EglSurface::swap_gbuffer(void *render_context, uint64_t next_gbuffer_id,
                              int gbuffer_width, int gbuffer_height,
                              int hal_format) {
    (void)render_context;
    GraphicBuffer *cur_draw_gbuffer = gbuffer;

    ALOGD("surface %llx swapbuffer next_gbuffer_id %llx sync %d\n",
          (uint64_t)this, next_gbuffer_id, cur_draw_gbuffer->data_sync);

    GraphicBuffer *next_draw_gbuffer;

    if (type == WINDOW_SURFACE) {
        next_draw_gbuffer =
            MainRenderer::get_instance()->get_gbuffer_from_global_map(
                next_gbuffer_id);
        if (next_draw_gbuffer == nullptr) {
            ALOGD(
                "create next_gbuffer_id %llx when surface %llx swapbuffer "
                " gbuffer_width %d gbuffer_height %d\n",
                next_gbuffer_id, this, gbuffer_width, gbuffer_height);
            next_draw_gbuffer = GraphicBuffer::create_gbuffer_from_hal(
                gbuffer_width, gbuffer_height, hal_format, this,
                next_gbuffer_id);

            ALOGD("create gbuffer when swapbuffer gbuffer %llx gbuffer\n",
                  next_gbuffer_id, next_draw_gbuffer);

            MainRenderer::get_instance()->add_gbuffer_to_global(
                next_draw_gbuffer);
            MainRenderer::get_instance()->set_global_gbuffer_type(
                next_gbuffer_id, GBUFFER_TYPE_WINDOW);
        } else {
            next_draw_gbuffer->do_not_die();
        }
    } else {
        next_draw_gbuffer = cur_draw_gbuffer;
    }

    GraphicBuffer::connect_gbuffer_to_surface(next_draw_gbuffer, this);

    GLenum attachments[] = {GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT,
                            GL_DEPTH_STENCIL_ATTACHMENT};
    glInvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, 3, attachments);

    if (next_draw_gbuffer->data_sync != nullptr) {
        glWaitSync(next_draw_gbuffer->data_sync, 0, GL_TIMEOUT_IGNORED);

        if (next_draw_gbuffer->delete_sync != nullptr) {
            glDeleteSync(next_draw_gbuffer->delete_sync);
        }
        next_draw_gbuffer->delete_sync = next_draw_gbuffer->data_sync;
        next_draw_gbuffer->data_sync = nullptr;
    }

    gbuffer = next_draw_gbuffer;

    if (gbuffer->sampler_num > 1) {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer->sampler_fbo);
    } else {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gbuffer->data_fbo);
    }
}

void EglSurface::d_eglIamComposer(void *context, EGLSurface surface,
                                  unsigned int pid) {
    auto *thread_context = reinterpret_cast<ThreadContext *>(context);
    ProcessContext *process_context = thread_context->process_context.get();

    EglSurface *real_surface = process_context->surface_map[surface];

    MainRenderer::get_instance()->static_context_values->composer_pid = pid;
}

void EglSurface::d_eglCreatePbufferSurface(void *context, EGLDisplay dpy,
                                           EGLConfig config,
                                           const EGLint *attrib_list,
                                           EGLSurface guest_surface) {
    auto *thread_context = reinterpret_cast<ThreadContext *>(context);
    ProcessContext *process_context = thread_context->process_context.get();

    int i = 0;
    int width = 0;
    int height = 0;
    while (attrib_list != nullptr && attrib_list[i] != EGL_NONE) {
        switch (attrib_list[i]) {
            case EGL_WIDTH:
                width = attrib_list[i + 1];
                break;
            case EGL_HEIGHT:
                height = attrib_list[i + 1];
                break;
            default:
                break;
        }
        i += 2;
    }

    
    
    
    if (width == 0) {
        width = 1;
    }

    if (height == 0) {
        height = 1;
    }

    auto *host_surface = new EglSurface(config, width, height, P_SURFACE);

    ALOGD(
        "pbuffer surface create host %llx guest %llx width %d height %d guest "
        "width %d height %d\n",
        (long long)host_surface, (long long)guest_surface, host_surface->width, host_surface->height,
        width, height);

    process_context->surface_map[guest_surface] = host_surface;
}

EGLBoolean EglSurface::d_eglDestroySurface(void *context, EGLDisplay dpy,
                                           EGLSurface surface) {
    auto *thread_context = reinterpret_cast<ThreadContext *>(context);
    ProcessContext *process_context = thread_context->process_context.get();

    EglSurface *real_surface = process_context->surface_map[surface];
    if (real_surface == nullptr) {
        return EGL_FALSE;
    }

    unordered_map_erase_with_callback(
        process_context->surface_map, surface,
        static_cast<std::function<void(EglSurface *)>>(
            EglSurface::g_surface_map_destroy));

    ALOGD("destroy surface host %llx guest %llx\n", (uint64_t)real_surface,
          (uint64_t)surface);
    return EGL_TRUE;
}

EGLBoolean EglSurface::d_eglSurfaceAttrib(void *context, EGLDisplay dpy,
                                          EGLSurface surface, EGLint attribute,
                                          EGLint value) {
    return EGL_TRUE;
}

EGLint EglSurface::d_eglCreateImage(void *context, EGLDisplay dpy,
                                    EGLContext ctx, EGLenum target,
                                    EGLClientBuffer depressed_buffer,
                                    const EGLint *attrib_list,
                                    EGLImage guest_image) {
    
    

    
    
    // if (depressed_buffer != guest_image)
    // {
    //     printf("error! buffer %llx != guest_image %llx\n", depressed_buffer,
    //     guest_image);
    //     // return -1;
    // }

    if (attrib_list == nullptr) {
        return -1;
    }

    int width = 1;
    int height = 1;
    int hal_format = 0;
    // int stride = 0;
    int i = 0;
    while (attrib_list[i] != EGL_NONE) {
        switch (attrib_list[i]) {
            case EGL_WIDTH:
                width = attrib_list[i + 1];
                break;
            case EGL_HEIGHT:
                height = attrib_list[i + 1];
                break;
            case EGL_TEXTURE_FORMAT:
                hal_format = attrib_list[i + 1];
                break;
                // case EGL_BUFFER_SIZE:
                
                //     stride = attrib_list[i + 1];
                //     break;
            default:
                
                break;
        }
        i += 2;
    }

    auto *thread_context = reinterpret_cast<ThreadContext *>(context);

    ProcessContext *process_context = thread_context->process_context.get();

    auto gbuffer_id = (uint64_t)guest_image;

    GraphicBuffer *gbuffer = nullptr;

    if (target == EGL_NATIVE_BUFFER_ANDROID) {
        int gbuffer_type =
            (int)MainRenderer::get_instance()->get_global_gbuffer_type(
                gbuffer_id);

        if (gbuffer_type != GBUFFER_TYPE_NONE) {
            gbuffer = MainRenderer::get_instance()->get_gbuffer_from_global_map(
                gbuffer_id);
        }

        if (gbuffer == nullptr) {
            ALOGD(
                "create image with gbuffer id %llx width %d height %d format "
                "%d process_context %llx\n",
                gbuffer_id, width, height, hal_format, process_context);
            gbuffer = GraphicBuffer::create_gbuffer_with_context(
                width, height, hal_format, thread_context, ctx, gbuffer_id);

            MainRenderer::get_instance()->add_gbuffer_to_global(gbuffer);
            if (gbuffer_type == GBUFFER_TYPE_NONE) {
                MainRenderer::get_instance()->set_global_gbuffer_type(
                    gbuffer_id, GBUFFER_TYPE_NATIVE);
                gbuffer->usage_type = GBUFFER_TYPE_NATIVE;
            } else {
                if (gbuffer_type != GBUFFER_TYPE_BITMAP_NEED_DATA) {
                    printf("error! gbuffer NULL with gbuffer_type %d\n",
                           gbuffer_type);
                }
                gbuffer->usage_type = GBUFFER_TYPE_BITMAP_NEED_DATA;
                gbuffer->remain_life_time = MAX_BITMAP_LIFE_TIME;
            }
        } else {
            ALOGD("cancel gbuffer delete %" PRIu64 "ptr %" PRIu64 "\n",
                  gbuffer->gbuffer_id, (uint64_t)gbuffer);

            SPIN_LOCK(gbuffer->lock);
            gbuffer->is_using = 1;
            if (gbuffer->is_dying == 1) {
                gbuffer->is_dying = 0;
                MainRenderer::get_instance()->send_window_message(
                    MAIN_CANCEL_GBUFFER, gbuffer);
            }
            SPIN_UNLOCK(gbuffer->lock);
        }
    } else {
        GLContext *share_opengl_context = process_context->gl_context_map[ctx];

        if (share_opengl_context == nullptr) {
            printf("error! glCreateImage with null share_context\n");
            return 0;
        }

        
        GLuint host_share_texture = GLResource::get_host_texture_id(
            share_opengl_context, (GLuint)gbuffer_id);

        
        gbuffer = new GraphicBuffer();
        gbuffer->usage_type = GBUFFER_TYPE_TEXTURE;
        gbuffer->data_texture = host_share_texture;
        gbuffer->gbuffer_id = gbuffer_id;

        if (thread_context->opengl_context == NULL) {
            
            printf("create eglImage gbuffer with different context!\n");
            if (share_opengl_context->independent_mode == 1) {
                //                glfwMakeContextCurrent((GLFWwindow
                //                *)share_opengl_context->window);
                ALOGE(
                    "glfwMakeContextCurrent((GLFWwindow "
                    "*)share_opengl_context->window);");
            } else {
                EglContext::native_make_current(
                    share_opengl_context->native_context);
            }
        }
        gbuffer->data_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        // glFinish();

        if (thread_context->opengl_context == NULL) {
            if (share_opengl_context->independent_mode == 1) {
                //                glfwMakeContextCurrent(NULL);
                ALOGE("glfwMakeContextCurrent(NULL);");
            } else {
                EglContext::native_make_current(nullptr);
            }
        }
        
    }
    ALOGD("createImage gbuffer %llx type %d ptr %llx\n", gbuffer_id,
          gbuffer->usage_type, gbuffer);

    process_context->gbuffer_map[gbuffer_id] = gbuffer;

    return 1;
}

EGLBoolean EglSurface::d_eglDestroyImage(void *context, EGLDisplay dpy,
                                         EGLImage image) {
    auto gbuffer_id = (uint64_t)image;
    auto *thread_context = reinterpret_cast<ThreadContext *>(context);
    ProcessContext *process_context = thread_context->process_context.get();

    GraphicBuffer *gbuffer = process_context->gbuffer_map[gbuffer_id];

    if (gbuffer == nullptr) {
        printf("error! destroy eglimage with null!\n");
        return EGL_FALSE;
    }

    unordered_map_erase_with_callback(
        process_context->gbuffer_map, gbuffer_id,
        static_cast<std::function<void(GraphicBuffer *)>>(
            GraphicBuffer::gbuffer_map_destroy));

    return EGL_TRUE;
}

void EglSurface::g_surface_map_destroy(EglSurface *data) {
    auto *real_surface = reinterpret_cast<EglSurface *>(data);
    if (real_surface == nullptr) return;
    ALOGD("try free surface %llx\n", (uint64_t)real_surface);
    if (real_surface->is_current) {
        real_surface->need_destroy = 1;
    } else {
        delete real_surface;
    }
}
