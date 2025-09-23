#include "Surface.h"

#include <EGL/egl.h>
#include <android/native_window.h>

#include <cstdint>

#include "EglDef.h"
#include "EglUtils.h"
#include "ThreadContext.h"
#include "system/window.h"

#define LOG_TAG "EGL_SURFACE"

EglSurface::EglSurface(EGLint surface_type) : surface_type(surface_type) {
    egl_default_display->add_surface(this);
}

void EglSurface::init(int swap_interval) {
#if defined(ANDROID_SYSTEM)
    ANativeWindow *a_win = native_window;
    ANativeWindowBuffer *a_win_buffer = native_window_buffer;
    EGLint final_swap_interval = 0;

    if (a_win_buffer != nullptr) {
        int ret = a_win->cancelBuffer(a_win, a_win_buffer, fence_fd);
        a_win_buffer = nullptr;
        ALOGD("MakeCurrent cancelBuffer %llx, ret is %d", (long long)gbuffer_id,
              ret);
    }

    if (a_win != nullptr && a_win_buffer == nullptr) {
        final_swap_interval = swap_interval < a_win->minSwapInterval
                                  ? a_win->minSwapInterval
                                  : swap_interval;
        final_swap_interval = final_swap_interval > a_win->maxSwapInterval
                                  ? a_win->maxSwapInterval
                                  : final_swap_interval;
        a_win->setSwapInterval(a_win, final_swap_interval);

        int ret = 0;
        ret += a_win->dequeueBuffer(a_win, &a_win_buffer, &fence_fd);

        int retry_count = 0;
        while (a_win_buffer == nullptr) {
            retry_count++;
            ALOGE("native window dequeueBuffer failed! format %d",
                  window_format);
            window_format = HAL_PIXEL_FORMAT_RGBA_8888;
            a_win->perform(a_win, NATIVE_WINDOW_SET_BUFFERS_FORMAT,
                           window_format);
            ret += a_win->dequeueBuffer(a_win, &a_win_buffer, &fence_fd);

            if (retry_count >= 2) break;
        }
    }

    if (a_win != nullptr && a_win_buffer != nullptr) {
        ALOGD("a_win_buffer %llx dequeued", (unsigned long long)a_win_buffer);
#if ANDROID_PLATFORM_SDK_VERSION >= 28
        android::sp<android::GraphicBuffer> g_buffer =
            android::GraphicBuffer::from(a_win_buffer);
#else
        android::sp<android::GraphicBuffer> g_buffer =
            static_cast<android::GraphicBuffer *>(a_win_buffer);
#endif
        gbuffer_id = (unsigned long long)g_buffer->getId();
        int64_t fence_id = egl_fence->get_fence(gbuffer_id);
        if (fence_id != -1) {
            egl_fence->wait_fence(fence_id);
        }
        if ((int)(gbuffer_id >> 32) != composer_pid) {
            ALOGE("set NATIVE_WINDOW_SET_BUFFERS_TRANSFORM 0");
            a_win->perform(a_win, NATIVE_WINDOW_SET_BUFFERS_TRANSFORM, 0);
        }

        native_window_buffer = a_win_buffer;
        width = a_win_buffer->width;
        height = a_win_buffer->height;
        window_format = a_win_buffer->format;
    }

    reset_frame_statistic();

    ALOGD(
        "ANativeWindow MakeCurrent width %d height %d gbuffer_id %llx swap "
        "%d %d fence_fd %d",
        width, height, (unsigned long long)gbuffer_id, final_swap_interval,
        swap_interval, fence_fd);

#else
    ANativeWindow *a_win = native_window;
    if (a_win != nullptr) {
        gbuffer_id = (((long long)composer_pid << 32)) + CLOUD_GBUFFER_ID;
        fence_fd = 0;
        width = CLOUD_GBUFFER_WIDTH;
        height = CLOUD_GBUFFER_HEIGHT;
        window_format = HAL_PIXEL_FORMAT_RGB_888;
    }

    reset_frame_statistic();
#endif

    if (gbuffer_id != 0 &&
        global_gbuffer_types.find(gbuffer_id) == global_gbuffer_types.end()) {
        global_gbuffer_types[gbuffer_id] = GBUFFER_TYPE_WINDOW;
    }
}

int EglSurface::set_surface_attrib(const EGLint *attrib_list) {
    int i = 0;

    while (attrib_list != nullptr && attrib_list[i] != EGL_NONE) {
        switch (attrib_list[i]) {
            case EGL_GL_COLORSPACE:
                this->gl_colorspace = attrib_list[i + 1];
                break;
            case EGL_HEIGHT:
                if (this->surface_type == WINDOW_SURFACE) {
                    ALOGE("window surface set window height %d",
                          attrib_list[i + 1]);
                    break;
                }
                this->height = attrib_list[i + 1];
                break;
            case EGL_WIDTH:
                if (this->surface_type == WINDOW_SURFACE) {
                    ALOGE("window surface set window width %d",
                          attrib_list[i + 1]);
                    break;
                }
                this->width = attrib_list[i + 1];
                break;
            case EGL_LARGEST_PBUFFER:
                if (this->surface_type == WINDOW_SURFACE) {
                    break;
                }
                this->largest_pbuffer = attrib_list[i + 1];
                break;
            case EGL_MIPMAP_LEVEL:
                this->mipmap_level = attrib_list[i + 1];
                break;
            case EGL_MIPMAP_TEXTURE:
                if (this->surface_type == WINDOW_SURFACE) {
                    break;
                }
                this->mipmap_texture = attrib_list[i + 1];
                break;
            case EGL_RENDER_BUFFER:
                if (this->surface_type == PBUFFER_SURFACE) {
                    break;
                }
                this->render_buffer = attrib_list[i + 1];
                break;
            case EGL_SWAP_BEHAVIOR:
                this->swap_behavior = attrib_list[i + 1];
                break;
            case EGL_TEXTURE_FORMAT:
                if (this->surface_type == WINDOW_SURFACE) {
                    break;
                }
                this->texture_format = attrib_list[i + 1];
                break;
            case EGL_TEXTURE_TARGET:
                if (this->surface_type == WINDOW_SURFACE) {
                    break;
                }
                this->texture_target = attrib_list[i + 1];
                break;
            case EGL_VG_COLORSPACE:
                this->vg_colorspace = attrib_list[i + 1];
                break;
            default:
                ALOGE("window surface set window attrib %x %d", attrib_list[i],
                      attrib_list[i + 1]);
                break;
        }
        i += 2;
    }
    return i;
}

void EglSurface::reset_frame_statistic() {
    frame_server_start_time = 0;
    frame_client_start_time = 0;
    remain_sleep_time = 0;
    frame_end_time = 0;
}

void EglSurface::reset_frame() {
    if (native_window != nullptr) {
        reset_frame_statistic();
    }

    call_streamer->flush_all_call();
}

EglSurface::~EglSurface() {
    if (egl_default_display) egl_default_display->destroy_surface(this);
}

EGLSurface d_eglGetCurrentSurface_special(void *context, EGLint readdraw) {
    if (((ThreadContext *)context)->opengl_context == nullptr) {
        return EGL_NO_SURFACE;
    } else {
        if (readdraw == EGL_READ) {
            return (EGLSurface)((ThreadContext *)context)->surface_read;
        } else {
            return (EGLSurface)((ThreadContext *)context)->surface_draw;
        }
    }
}

EGLBoolean d_eglQueryContext_special(void *context, EGLDisplay dpy,
                                     EGLContext ctx, EGLint attribute,
                                     EGLint *value) {
    CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    CHECK_CONTEXT_STRICT(ctx, EGL_FALSE);
    
    switch (attribute) {
        case EGL_CONFIG_ID:
            *value = ((GLThreadContext *)ctx)->config_id;
            break;
        case EGL_CONTEXT_CLIENT_TYPE:
            *value = EGL_OPENGL_ES_API;
            break;
        case EGL_CONTEXT_CLIENT_VERSION:
            *value = (EGL_OPENGL_ES3_BIT | EGL_OPENGL_ES2_BIT);
            break;
        case EGL_RENDER_BUFFER:
            if (((ThreadContext *)context)->opengl_context == nullptr) {
                *value = EGL_NONE;
            } else {
                *value = EGL_BACK_BUFFER;
            }
            break;
        case EGL_PROTECTED_CONTENT_EXT:
            *value = EGL_FALSE;
            break;
        default: {
            ALOGE("query context error attribute %x", attribute);
            RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
        }
    }
    return EGL_TRUE;
}

EGLBoolean d_eglQuerySurface_special(void *context, EGLDisplay dpy,
                                     EGLSurface surface, EGLint attribute,
                                     EGLint *value) {
    (void)context;
    CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    CHECK_SURFACE_STRICT(surface, EGL_FALSE);
    auto const *real_surface = (EglSurface *)surface;
    switch (attribute) {
        case EGL_CONFIG_ID:
            *value = real_surface->config_id;
            break;
        case EGL_GL_COLORSPACE:
            *value = real_surface->gl_colorspace;
            break;
        case EGL_HEIGHT:
            // ALOGE("query surface height %d", real_surface->height);
            *value = real_surface->height;
            break;
        case EGL_WIDTH:
            // ALOGE("query surface height %d", real_surface->width);
            *value = real_surface->width;
            break;
        case EGL_LARGEST_PBUFFER:
            *value = real_surface->largest_pbuffer;
            break;
        case EGL_MIPMAP_LEVEL:
            *value = real_surface->mipmap_level;
            break;
        case EGL_MIPMAP_TEXTURE:
            *value = real_surface->mipmap_texture;
            break;
        case EGL_RENDER_BUFFER:
            *value = real_surface->render_buffer;
            break;
        case EGL_SWAP_BEHAVIOR:
            *value = real_surface->swap_behavior;
            break;
        case EGL_TEXTURE_FORMAT:
            *value = real_surface->texture_format;
            break;
        case EGL_TEXTURE_TARGET:
            *value = real_surface->texture_target;
            break;
        case EGL_VG_COLORSPACE:
            *value = real_surface->vg_colorspace;
            break;
        case EGL_VG_ALPHA_FORMAT:
            *value = real_surface->vg_alpha_format;
            break;
        case EGL_MULTISAMPLE_RESOLVE:
            ALOGE("query surface unknow attribute %x", attribute);
            // @todo
            break;
        case EGL_PIXEL_ASPECT_RATIO:
            ALOGE("query surface unknow attribute %x", attribute);
            //@todo
            break;
        case EGL_HORIZONTAL_RESOLUTION:
            ALOGE("query surface unknow attribute %x", attribute);
            // @todo
            break;
        case EGL_VERTICAL_RESOLUTION:
            ALOGE("query surface unknow attribute %x", attribute);
            // @todo
            break;
        default: {
            ALOGE("query surface error attribute %x", attribute);
            RETURN_ERROR(EGL_FALSE, EGL_BAD_ATTRIBUTE);
        }
    }
    return EGL_TRUE;
}

EGLContext d_eglGetCurrentContext_special(void *context) {
    auto *thread_context = (ThreadContext *)context;
    if (thread_context->opengl_context == nullptr) {
        return EGL_NO_CONTEXT;
    } else {
        return (EGLContext)thread_context->opengl_context;
    }
}

EGLSurface d_eglCreatePbufferSurface_special(void *context, EGLDisplay dpy,
                                             EGLConfig config,
                                             const EGLint *attrib_list) {
    CHECK_DISPLAY_INIT(dpy, nullptr);
    CHECK_CONFIG(config, nullptr);
    auto *surface = new EglSurface(PBUFFER_SURFACE);
    surface->native_window = nullptr;

    surface->set_surface_attrib(attrib_list);
    surface->native_window = nullptr;
    egl_default_display->get_config_attr(config, EGL_CONFIG_ID,
                                         &surface->config_id);

    ALOGD("create pbuffer surface guest %llx", (long long)surface);

    r_eglCreatePbufferSurface(context, dpy, config, attrib_list,
                              (EGLSurface)surface);
    surface->fence_key = egl_fence->gen_key();
    int64_t fence_id = egl_fence->set_fence(surface->fence_key);
    ALOGE("create pbuffer surface guest %llx set fence %lld with key %llud",
          (long long)surface, (long long)fence_id,
          (unsigned long long)surface->fence_key);

    return surface;
}

EGLSurface d_eglCreateWindowSurface_special(void *context, EGLDisplay dpy,
                                            EGLConfig config,
                                            EGLNativeWindowType win,
                                            const EGLint *attrib_list) {
    CHECK_DISPLAY_INIT(dpy, nullptr);
    CHECK_CONFIG(config, nullptr);
    auto const *thread_context = (ThreadContext *)context;

    if (thread_context == nullptr) {
        return nullptr;
    }

    auto *surface = new EglSurface(WINDOW_SURFACE);

    int attrib_size = surface->set_surface_attrib(attrib_list);
    egl_default_display->get_config_attr(config, EGL_CONFIG_ID,
                                         &surface->config_id);

    EGLint width = 0;
    EGLint height = 0;
    ANativeWindow *a_win = win;

#if defined(ANDROID_SYSTEM)
    EGLint default_width = 0;
    EGLint default_height = 0;
    int native_window_format = 0;

    a_win->query(a_win, NATIVE_WINDOW_WIDTH, &width);
    a_win->query(a_win, NATIVE_WINDOW_HEIGHT, &height);
    a_win->query(a_win, NATIVE_WINDOW_DEFAULT_WIDTH, &default_width);
    a_win->query(a_win, NATIVE_WINDOW_DEFAULT_HEIGHT, &default_height);
    a_win->query(a_win, NATIVE_WINDOW_FORMAT, &native_window_format);

    width = default_width < 0 ? 0 : default_width;
    height = default_height < 0 ? 0 : default_height;

    surface->width = width;
    surface->height = height;
    surface->window_format = native_window_format;
#else
    surface->width = ANativeWindow_getWidth(a_win);
    surface->height = ANativeWindow_getHeight(a_win);
    surface->window_format = ANativeWindow_getFormat(a_win);
#endif

#if defined(ANDROID_SYSTEM)
    a_win->perform(a_win, NATIVE_WINDOW_SET_BUFFERS_TRANSFORM,
                   HAL_TRANSFORM_FLIP_V);
    a_win->setSwapInterval(a_win, thread_context->swap_interval);
#endif

    surface->native_window = a_win;

    auto *attrib_list_new = new EGLint[attrib_size + 5];
    if (attrib_list != nullptr) {
        memcpy(attrib_list_new, attrib_list, attrib_size);
    }
    attrib_list_new[attrib_size] = EGL_WIDTH;
    attrib_list_new[attrib_size + 1] = width;
    attrib_list_new[attrib_size + 2] = EGL_HEIGHT;
    attrib_list_new[attrib_size + 3] = height;
    attrib_list_new[attrib_size + 4] = EGL_NONE;

    r_eglCreateWindowSurface(context, dpy, config, win, attrib_list_new,
                             (EGLSurface)surface);

    surface->fence_key = egl_fence->gen_key();
    int64_t fence_id = egl_fence->set_fence(surface->fence_key);
    ALOGE("create window surface guest %llx set fence %lld with key %llud",
          (long long)surface, (long long)fence_id,
          (unsigned long long)surface->fence_key);

    delete[] attrib_list_new;

    return surface;
}

EGLContext d_eglCreateContext_special(void *context, EGLDisplay dpy,
                                      EGLConfig config,
                                      EGLContext share_context,
                                      const EGLint *attrib_list) {
    CHECK_DISPLAY_INIT(dpy, nullptr);
    CHECK_CONFIG(config, nullptr);
    if (share_context == EGL_NO_CONTEXT) {
        share_context = nullptr;
    }
    auto *opengl_context =
        new GLThreadContext((GLThreadContext *)share_context);

    egl_default_display->get_config_attr(config, EGL_CONFIG_ID,
                                         &opengl_context->config_id);

    EGLint new_attrib_list[] = {EGL_NONE};
    if (attrib_list == nullptr) {
        attrib_list = new_attrib_list;
    }

    //    EGLint new_attrib_list2[] = {0xffffff, 0xffffff, EGL_NONE};

    // if(egl_default_display->get_proc_name() == "droid.launcher3")
    // if(egl_default_display->get_proc_name() == "MainThread-UE4")
    // if(egl_default_display->get_proc_name() == "nt.tmgp.pubgmhd")
    // if(egl_default_display->get_proc_name() == ".ANMP.GloftA9HM")
    // if(egl_default_display->get_proc_name() == "surfaceflinger")
    // if(egl_default_display->get_proc_name() == "oter.warfareops") War After
    // if(egl_default_display->getProcName() == ".android.chrome" ||
    // egl_default_display->get_proc_name() == "ileged_process0") chrome
    // {
    //     attrib_list = new_attrib_list2;
    // }

    ALOGD("create window context guest %llx ptr size %lu",
          (long long)opengl_context, (unsigned long)sizeof(void *));
    r_eglCreateContext(context, dpy, config, share_context, attrib_list,
                       opengl_context);

    return opengl_context;
}

EGLBoolean d_eglDestroyContext_special(void *context, EGLDisplay dpy,
                                       EGLContext ctx) {
    r_eglDestroyContext(context, dpy, ctx);

    auto *real_context = (GLThreadContext *)ctx;

    if (real_context->has_make_current) {
        real_context->need_destroy = 1;
    } else {
        delete real_context;
    }
    return EGL_TRUE;
}

EGLBoolean d_eglDestroySurface_special(void *context, EGLDisplay dpy,
                                       EGLSurface surface) {
    auto *real_surface = (EglSurface *)surface;
    int64_t fence_id = egl_fence->get_fence(real_surface->fence_key);
    ALOGE("destroy surface guest %llx wait fence %lld with key %llud",
          (long long)surface, (long long)fence_id,
          (unsigned long long)real_surface->fence_key);
    if (fence_id != -1) {
        egl_fence->wait_fence(fence_id);
    }
    r_eglDestroySurface(context, dpy, surface);

    egl_fence->destroy_fence(reinterpret_cast<uint64_t>(real_surface));

    if (real_surface->has_make_current) {
        real_surface->need_destroy = 1;
    } else {
        delete real_surface;
    }
    return EGL_TRUE;
}

EGLBoolean d_eglSurfaceAttrib_special(void *context, EGLDisplay dpy,
                                      EGLSurface surface, EGLint attribute,
                                      EGLint value) {
    (void)context;
    (void)dpy;
    auto *real_surface = (EglSurface *)surface;
    switch (attribute) {
        case EGL_MIPMAP_LEVEL:
            real_surface->mipmap_level = value;
            break;
        case EGL_MULTISAMPLE_RESOLVE:
            real_surface->multisample_resolve = value;
            break;
        case EGL_SWAP_BEHAVIOR:
            real_surface->swap_behavior = value;
            break;
        default:
            ALOGE("surface error attribute %x %d", attribute, value);
            break;
    }
    return EGL_TRUE;
}
