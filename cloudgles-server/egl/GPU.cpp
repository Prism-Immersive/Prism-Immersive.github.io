#define LOG_TAG "EglGPU"
#define LOG_LEVEL LOG_LEVEL_ERROR
#include <android/native_window.h>
#include <sys/time.h>

#include <cassert>
#include <cmath>
#include <cstdint>

#include "EglDef.h"
#include "network/FlowController.h"
#include "opengl/GLDef.h"

#if defined(ANDROID_SYSTEM)
#if ANDROID_PLATFORM_SDK_VERSION > 28
#include <nativebase/nativebase.h>
#endif
#else
#include "Constants.h"
#endif

#include "Display.h"
#include "EglUtils.h"
#include "GPU.h"
#include "Surface.h"
#include "ThreadContext.h"

#if ANDROID_PLATFORM_SDK_VERSION == 29
static const EGLint composer_type = NO_SURFACE;
#else
static const EGLint composer_type = WINDOW_SURFACE;
#endif

EGLBoolean d_eglMakeCurrent_special(void *context, EGLDisplay dpy,
                                    EGLSurface draw, EGLSurface read,
                                    EGLContext ctx) {
    // TODO: Complete error checking
    ALOGD("MakeCurrent guest draw %llx, read %llx ctx %llx", (long long)draw,
          (long long)read, (long long)ctx);
    CHECK_DISPLAY(dpy, EGL_FALSE);
    if (read != EGL_NO_SURFACE || draw != EGL_NO_SURFACE ||
        ctx != EGL_NO_CONTEXT) {
        CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    }
    CHECK_CONTEXT(ctx, EGL_FALSE);
    CHECK_SURFACE(draw, EGL_FALSE);
    CHECK_SURFACE(read, EGL_FALSE);
    if (unlikely((read == EGL_NO_SURFACE && draw == EGL_NO_SURFACE) &&
                 (ctx != EGL_NO_CONTEXT))) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_MATCH);
    }
    if (unlikely((read != EGL_NO_SURFACE || draw != EGL_NO_SURFACE) &&
                 (ctx == EGL_NO_CONTEXT))) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_MATCH);
    }
    if (unlikely((read != EGL_NO_SURFACE && draw == EGL_NO_SURFACE) ||
                 (read == EGL_NO_SURFACE && draw != EGL_NO_SURFACE))) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_MATCH);
    }

    auto *thread_context = (ThreadContext *)context;

    // Everything is the same
    if (thread_context->surface_draw == draw &&
        thread_context->surface_draw == read &&
        thread_context->opengl_context == ctx) {
        return EGL_TRUE;
    }

    // The send_to_host function sends eglMakeCurrent in a contextless
    // call_streamer, but wait_fence is not, resulting in eglMakeCurrent
    // arriving first at the client. To avoid this, we must call wait_fence
    // before make_current. In this way, the call is put into the previous
    // surface's call_streamer (if any), and flushed to the client during
    // make_current.
    if (draw) {
        // Need to make sure the draw surface is created at the client side
        auto *surface_draw = (EglSurface *)draw;
        int64_t fence_id = egl_fence->get_fence(surface_draw->fence_key);
        if (fence_id != -1) {
            egl_fence->wait_fence(fence_id);
        }
        ALOGE("make current surface guest %llx wait fence %lld with key %llud",
              (long long)surface_draw, (long long)fence_id,
              (unsigned long long)surface_draw->fence_key);
    }

    thread_context->make_current(dpy, draw, read, ctx);
    auto const *surface_draw = thread_context->surface_draw;
    uint64_t gbuffer_id =
        surface_draw == nullptr ? 0 : surface_draw->gbuffer_id;
    int width = surface_draw == nullptr ? 1 : surface_draw->width;
    int height = surface_draw == nullptr ? 1 : surface_draw->height;
    int hal_format = surface_draw == nullptr ? 0 : surface_draw->window_format;

    int64_t mc_fence = -1;
    if (thread_context->opengl_context != nullptr) {
        mc_fence = thread_context->opengl_context->fetch_mc_fence_add();
    }

    bool res = r_eglMakeCurrent(context, dpy, draw, read, ctx, gbuffer_id,
                                width, height, hal_format, mc_fence);

    // Set fence for surface and wait fence in destroy surface
    if (surface_draw) {
        int64_t fence_id = egl_fence->set_fence(surface_draw->fence_key);
        ALOGE(
            "make current surface guest %llx set fence %lld with key %llud "
            "mc_fence %lld",
            (long long)surface_draw, (long long)fence_id,
            (unsigned long long)surface_draw->fence_key, (long long)mc_fence);
    }

    return res;
}

EGLBoolean d_eglBindTexImage_special(void *context, EGLDisplay dpy,
                                     EGLSurface surface, EGLint buffer) {
    r_eglBindTexImage(context, dpy, surface, buffer);
    return 0;
}

EGLBoolean d_eglReleaseTexImage_special(void *context, EGLDisplay dpy,
                                        EGLSurface surface, EGLint buffer) {
    r_eglReleaseTexImage(context, dpy, surface, buffer);
    return 0;
}

EGLImage d_eglCreateImage_special(void *context, EGLDisplay dpy, EGLContext ctx,
                                  EGLenum target, EGLClientBuffer buffer,
                                  const EGLint *attrib_list) {
    auto *thread_context = (ThreadContext *)context;

    GLThreadContext *opengl_context = thread_context->opengl_context;

    if (ctx == nullptr) {
        ALOGE("createimage with ctx null opengl_context %llx",
              (unsigned long long)opengl_context);
    }

    ALOGD("create image target %x", target);
    int i = 0;
    while (attrib_list != nullptr && attrib_list[i] != EGL_NONE) {
        ALOGD("image attrib %x %x", attrib_list[i], attrib_list[i + 1]);
        i += 2;
    }

    uint64_t gbuffer_id = 0;
    EGLImageInfo *image_info = nullptr;
    EGLint width = 0;
    EGLint height = 0;
    EGLint stride = 0;
    EGLint format = 0;
    uint64_t usage = 0;
    GLuint share_texture = 0;
    void *graphic_buffer = nullptr;

    if (target == EGL_GL_TEXTURE_2D) {
        gbuffer_id = (((uint64_t)(uintptr_t)opengl_context) << 32) +
                     (uint64_t)(uintptr_t)buffer;
        share_texture = (uint)(uint64_t)buffer;
        ;
    } else {
        // EXTERNAL Mode

#ifdef ANDROID_SYSTEM
        auto *a_win_buffer = (ANativeWindowBuffer *)buffer;
        ALOGD("eglCreateImage a_win_buffer is %llx",
              (unsigned long long)a_win_buffer);
#endif
        ALOGD(
            "eglCreateImage context %llx dpy %llx ctx %llx target %x buffer "
            "%llx",
            (unsigned long long)context, (unsigned long long)dpy,
            (unsigned long long)ctx, target, (unsigned long long)buffer);

        width = CLOUD_GBUFFER_WIDTH;
        height = CLOUD_GBUFFER_HEIGHT;

#ifdef ANDROID_SYSTEM
        stride = a_win_buffer->stride < 0 ? 0 : a_win_buffer->stride;
        format = a_win_buffer->format;
        usage = a_win_buffer->usage;
        ALOGD("createImage format %d", a_win_buffer->format);
#endif

#if ANDROID_SYSTEM
#if ANDROID_PLATFORM_SDK_VERSION >= 28
        android::sp<android::GraphicBuffer> gbuffer =
            android::GraphicBuffer::from(a_win_buffer);
#else
        android::sp<android::GraphicBuffer> gbuffer =
            static_cast<android::GraphicBuffer *>(a_win_buffer);
#endif
        gbuffer_id = gbuffer->getId();
        graphic_buffer = gbuffer.get();
#else
        gbuffer_id =
            (((uint64_t)(uintptr_t)my_pid) << 32) + (uint64_t)(uintptr_t)buffer;
#endif
    }

    if (unlikely(all_image_info.find((EGLImage)gbuffer_id) !=
                 all_image_info.end())) {
        image_info = all_image_info[(EGLImage)gbuffer_id];
        image_info->need_destroy = 0;
        ALOGE(
            "gbuffer %llx create again attrib change width %d=>%d height "
            "%d=>%d stride %d=>%d format %x=>%x target %x=>%x",
            (unsigned long long)gbuffer_id, image_info->width, width,
            image_info->height, height, image_info->stride, stride,
            image_info->format, format, image_info->target, target);
        return (EGLImage)gbuffer_id;
    } else {
        image_info = new EGLImageInfo;
        all_image_info[(EGLImage)gbuffer_id] = image_info;
        memset(image_info, 0, sizeof(EGLImageInfo));

        image_info->target = target;
        image_info->gbuffer_id = gbuffer_id;
        image_info->format = format;
        image_info->height = height;
        image_info->width = width;
        image_info->stride = stride;
        image_info->usage = usage;

        image_info->share_texture_id = share_texture;
        image_info->share_context = opengl_context;
        image_info->graphic_buffer = graphic_buffer;
    }

    EGLint *temp_attrib_list = nullptr;
    int list_loc = 0;

    if (attrib_list == nullptr) {
        temp_attrib_list = new EGLint[9];
    } else {
        while (attrib_list[list_loc] != EGL_NONE) {
            list_loc++;
        }
        temp_attrib_list = new EGLint[list_loc + 9];
    }

    temp_attrib_list[list_loc] = EGL_WIDTH;
    temp_attrib_list[list_loc + 1] = width;
    temp_attrib_list[list_loc + 2] = EGL_HEIGHT;
    temp_attrib_list[list_loc + 3] = height;
    temp_attrib_list[list_loc + 4] = EGL_TEXTURE_FORMAT;
    temp_attrib_list[list_loc + 5] = format;

    temp_attrib_list[list_loc + 6] = EGL_BUFFER_SIZE;
    temp_attrib_list[list_loc + 7] = stride;

    temp_attrib_list[list_loc + 8] = EGL_NONE;

    r_eglCreateImage(context, dpy, opengl_context, target,
                     (EGLClientBuffer)gbuffer_id, temp_attrib_list, gbuffer_id);
    ALOGE(
        "Create Native Image %llx width %d height %d format %d stride %d usage "
        "%lld ptr %llx",
        (unsigned long long)gbuffer_id, width, height, format, stride,
        (long long)usage, (unsigned long long)image_info);
    delete[] temp_attrib_list;

    int gbuffer_type = GBUFFER_TYPE_NATIVE;

    if (target == EGL_GL_TEXTURE_2D) {
        gbuffer_type = GBUFFER_TYPE_TEXTURE;
        if (global_gbuffer_types.find(gbuffer_id) ==
            global_gbuffer_types.end()) {
            global_gbuffer_types[gbuffer_id] = GBUFFER_TYPE_TEXTURE;
        }
        r_glSync(context);
    } else {
        // target == EGL_NATIVE_BUFFER_ANDROID
        if (global_gbuffer_types.find(gbuffer_id) !=
            global_gbuffer_types.end()) {
            uint64_t old_time = global_gbuffer_died_times[gbuffer_id];

            gbuffer_type = global_gbuffer_types[gbuffer_id];
            // CRITICAL DIFF
            //            struct timeval now;
            //            gettimeofday(&now, NULL);
            //            uint64_t now_time = (int64_t)now.tv_sec * 1000000 +
            //            now.tv_usec;
            //
            //            if (now_time - old_time >= 1000000 *
            //            MAX_BITMAP_LIFE_TIME || old_time > now_time)
            //            {
            //                ALOGE("eglCreateImage time escape %llu gbuffer_id
            //                %llx", (unsigned long long)(now_time - old_time),
            //                (unsigned long long)gbuffer_id); if (gbuffer_type
            //                == GBUFFER_TYPE_BITMAP || gbuffer_type ==
            //                GBUFFER_TYPE_FBO)
            //                {
            //                    gbuffer_type = r_eglGetGBufferType(context,
            //                    gbuffer_id);
            //                }
            //            }
        } else {
            //            gbuffer_type = r_eglGetGBufferType(context,
            //            gbuffer_id); ALOGE("eglCreateImage gbuffer_id %llx
            //            my_pid %d composer_pid %d type %d", (unsigned long
            //            long)gbuffer_id, my_pid, composer_pid, gbuffer_type);
            //
            //            if (gbuffer_type == GBUFFER_TYPE_NONE)
            //            {
            //                ALOGE("error! get gbuffer type none gbuffer_id
            //                %llx", (unsigned long long)gbuffer_id);
            //                gbuffer_type = GBUFFER_TYPE_NATIVE;
            //            }

            assert((gbuffer_id >> 32) == my_pid);
            if ((gbuffer_id >> 32) == my_pid) {
                gbuffer_type = GBUFFER_TYPE_NATIVE;
            } else {
                gbuffer_type = GBUFFER_TYPE_WINDOW;
            }

            global_gbuffer_types[gbuffer_id] = gbuffer_type;
        }
    }

    //    if (gbuffer_type == GBUFFER_TYPE_BITMAP_NEED_DATA || gbuffer_type ==
    //    GBUFFER_TYPE_FBO_NEED_DATA)
    //    {
    //        upload_graphic_buffer(opengl_context, image_info);
    //        if (gbuffer_type == GBUFFER_TYPE_BITMAP_NEED_DATA)
    //        {
    //            gbuffer_type = GBUFFER_TYPE_BITMAP;
    //        }
    //        if (gbuffer_type == GBUFFER_TYPE_FBO_NEED_DATA)
    //        {
    //            gbuffer_type = GBUFFER_TYPE_FBO;
    //        }
    //    }

    image_info->gbuffer_type = gbuffer_type;

    // // ALOGD("usage %lld %lld %lld",(long
    // long)android::GraphicBuffer::USAGE_HW_TEXTURE,(long long)
    // android::GraphicBuffer::USAGE_CURSOR, (long long)(usage &
    // android::GraphicBuffer::USAGE_HW_TEXTURE) ); if((usage &
    // android::GraphicBuffer::USAGE_SW_READ_OFTEN) == 0 && (usage &
    // android::GraphicBuffer::USAGE_SW_WRITE_OFTEN) == 0)
    // {
    //     image_info->data_in_host = 1;
    // }

    // if(image_info->data_in_host == 0 && ((image_info->usage)&8192) == 0)
    // {
    //     upload_graphic_buffer(opengl_context,image_info);
    // }

    // if(has_gbuffer_map.find(gbuffer_id) == has_gbuffer_map.end()){
    //     has_gbuffer_map[gbuffer_id] = 1;
    //     r_glSync(context);
    // }

    return (EGLImage)gbuffer_id;
}

// #include <utils/CallStack.h>
EGLBoolean d_eglDestroyImage_special(void *context, EGLDisplay dpy,
                                     EGLImage image) {
    // android::CallStack callstack;
    // callstack.update();
    // callstack.log("callstack-tag-display", ANDROID_LOG_DEBUG, "  ");
    (void)dpy;
    auto *thread_context = (ThreadContext *)context;

    GLThreadContext *opengl_context = thread_context->opengl_context;

    EGLImageInfo *image_info = nullptr;
    uint64_t gbuffer_id = 0;
    ALOGE("d_eglDestroyImage_special image %llx", (long long)image);
    if (all_image_info.find(image) != all_image_info.end()) {
        image_info = all_image_info[image];
        image_info->need_destroy = 1;

        //        if (image_info->need_data_back == 1 ||
        //        image_info->gbuffer_type == GBUFFER_TYPE_FBO)
        //        {
        //            image_info->need_data_back = 0;
        //            if (my_pid != composer_pid)
        //            {
        //                ALOGE("download graphic_buffer");
        //                download_graphic_buffer(context, image_info);
        //            }
        //        }

        try_destroy_EGLImage(opengl_context, image_info);
        return EGL_TRUE;
    } else {
        return EGL_FALSE;
    }
}

#if ANDROID_PLATFORM_SDK_VERSION < 30
static void gbuffer_free_callback(uint64_t gbuffer_id) {
#elif ANDROID_PLATFORM_SDK_VERSION >= 30
static void gbuffer_free_callback(void *context, uint64_t gbuffer_id) {
    (void)context;
#endif
    int64_t fence_id = egl_fence->get_fence(gbuffer_id);
    if (fence_id != -1) {
        egl_fence->wait_fence(fence_id);
    }

    r_eglGBufferDestroy(egl_thread_context, gbuffer_id);
    egl_fence->destroy_fence(gbuffer_id);
}

EGLBoolean d_eglSwapBuffers_special(void *context, EGLDisplay dpy,
                                    EGLSurface surface) {
    CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    auto *real_surface = (EglSurface *)surface;
    auto *thread_context = (ThreadContext *)context;
    if (unlikely(real_surface != thread_context->surface_draw)) {
        RETURN_ERROR(EGL_FALSE, EGL_BAD_SURFACE);
    }

    struct timeval now {};
    int64_t cur_time = 0;

    ANativeWindow *a_win = real_surface->native_window;
    uint64_t gbuffer_id = 0;
    int width = 0;
    int height = 0;
    int hal_format = 0;
    EGLint w_width = 0;
    EGLint w_height = 0;
    int64_t fence_id = -1;

    if (a_win != nullptr) {
        int fence_fd = real_surface->fence_fd;
        r_eglQueueBuffer(context, real_surface->gbuffer_id, 1);

#if defined(ANDROID_SYSTEM)
#if ANDROID_PLATFORM_SDK_VERSION >= 28
        android::sp<android::GraphicBuffer> g_buffer =
            android::GraphicBuffer::from(real_surface->native_window_buffer);
#else
        android::sp<android::GraphicBuffer> g_buffer =
            static_cast<android::GraphicBuffer *>(a_win_buffer);
#endif
#if ANDROID_PLATFORM_SDK_VERSION <= 28
        g_buffer->setBufferFreeCallback(gbuffer_free_callback);
        fence_id = egl_fence->set_fence(g_buffer->getId());
#elif ANDROID_PLATFORM_SDK_VERSION >= 30
        g_buffer->addDeathCallback(gbuffer_free_callback, nullptr);
        fence_id = egl_fence->set_fence(g_buffer->getId());
#endif
#endif

#if defined(ANDROID_SYSTEM)
        ANativeWindowBuffer *a_win_buffer = real_surface->native_window_buffer;
        int queue_ret = a_win->queueBuffer(a_win, a_win_buffer, fence_fd);
        int dequeue_ret = a_win->dequeueBuffer(a_win, &a_win_buffer, &fence_fd);
        real_surface->native_window_buffer = a_win_buffer;

#if ANDROID_PLATFORM_SDK_VERSION >= 28
        g_buffer = android::GraphicBuffer::from(a_win_buffer);
#else
        g_buffer = static_cast<android::GraphicBuffer *>(a_win_buffer);
#endif
        gbuffer_id = (unsigned long long)g_buffer->getId();
        fence_id = egl_fence->get_fence(gbuffer_id);
        width = a_win_buffer->width;
        height = a_win_buffer->height;
        hal_format = a_win_buffer->format;
        a_win->query(a_win, NATIVE_WINDOW_WIDTH, &w_width);
        a_win->query(a_win, NATIVE_WINDOW_HEIGHT, &w_height);
        ALOGD("swapbuffer queue %d dequeue %d\n", queue_ret, dequeue_ret);
#else
        gbuffer_id = (real_surface->gbuffer_id_generator) % 3 + 1;
        real_surface->gbuffer_id_generator = gbuffer_id;

        width = CLOUD_GBUFFER_WIDTH;
        height = CLOUD_GBUFFER_HEIGHT;
        hal_format = HAL_PIXEL_FORMAT_RGB_888;
        w_width = ANativeWindow_getWidth(a_win);
        w_height = ANativeWindow_getHeight(a_win);
#endif
        real_surface->gbuffer_id = gbuffer_id;
        real_surface->width = width;
        real_surface->height = height;
        real_surface->fence_fd = fence_fd;

        ALOGD(
            "ANativeWindow swapbuffer gbuffer_id %llx width %d height %d "
            "format %d real width %d height %d fence_fd %d",
            (unsigned long long)gbuffer_id, w_width, w_height, hal_format,
            width, height, fence_fd);
    }

    gettimeofday(&now, nullptr);
    cur_time = now.tv_sec * 1000000 + now.tv_usec;

    uint64_t swap_data[6];
    swap_data[0] = (uint64_t)dpy;
    swap_data[1] = (uint64_t)surface;
    swap_data[2] = gbuffer_id;
    swap_data[3] = (uint64_t)thread_context->remote_mem;
    (reinterpret_cast<int *>(swap_data + 4))[0] = width;
    (reinterpret_cast<int *>(swap_data + 4))[1] = height;
    (reinterpret_cast<int *>(swap_data + 4))[2] = hal_format;

    if (fence_id != -1) egl_fence->wait_fence(fence_id);
    r_eglSwapBuffers(context, reinterpret_cast<int64_t *>(swap_data));

    thread_context->flow_controller.decision_kingman(thread_context, real_surface,
                                                   cur_time);

    real_surface->frame_end_time = cur_time;
    real_surface->frame_server_start_time = 0;
    real_surface->frame_client_start_time = 0;

    return EGL_TRUE;
}

EGLBoolean d_eglSwapInterval_special(void *context, EGLDisplay dpy,
                                     EGLint interval) {
    CHECK_DISPLAY_INIT(dpy, EGL_FALSE);
    auto *thread_context = (ThreadContext *)context;

    ALOGD("set swap interval %d", interval);

    thread_context->swap_interval = interval;

    EglSurface *surface = thread_context->surface_draw;
    if (surface != nullptr) {
        ANativeWindow *a_win = surface->native_window;
    }

    return EGL_TRUE;
}

void d_eglQueueBuffer(void *context, EGLImage gbuffer_id, int is_composer) {
    EGLImageInfo const *image_info = all_image_info[gbuffer_id];
    if (image_info == nullptr) {
        all_image_info.erase(gbuffer_id);
        ALOGE("error! eglQueueBuffer get NULL image_info %llx",
              (unsigned long long)gbuffer_id);
        return;
    }
    r_eglQueueBuffer(context, image_info->gbuffer_id, is_composer);
}
