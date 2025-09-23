#define LOG_TAG "RenderThread"

#include "RenderThread.h"

#include <atomic>

#include "EglFuncID.h"
#include "GLFuncID.h"
#include "Log.h"
#include "MainController.h"
#include "Utils.h"
#include "egl/Context.h"
#include "egl/EglTranslator.h"
#include "egl/Surface.h"
#include "network/FrameCallPoller.h"
#include "opengl/GLTranslator.h"

ProcessContext::ProcessContext(pid_t pid) {
    remote_pid = pid;
    egl_sync_resource = new ResourceMapStatus();
}

ProcessContext::~ProcessContext() {
    unordered_map_clear_with_callback(
        gl_context_map, static_cast<std::function<void(GLContext *)>>(
                            GLContext::g_context_map_destroy));
    unordered_map_clear_with_callback(
        surface_map, static_cast<std::function<void(EglSurface *)>>(
                         EglSurface::g_surface_map_destroy));
    unordered_map_clear_with_callback(
        gbuffer_map, static_cast<std::function<void(GraphicBuffer *)>>(
                         GraphicBuffer::gbuffer_map_destroy));
    MainController::get_instance()->render_threads.erase(remote_pid);
    MainRenderer::get_instance()->send_window_message(MAIN_DESTROY_ALL_EGLSYNC,
                                                      egl_sync_resource);
}

ThreadContext::ThreadContext(std::shared_ptr<ProcessContext> &context,
                             uint64_t tid) {
    remote_tid = tid;
    process_context = context;
    cur_read_surface = nullptr;
    cur_draw_surface = nullptr;
    opengl_context = nullptr;
    packet_poller = new ThreadPacketPoller(tid);
    default_call_poller = new FrameCallPoller();
    remote_mem_writer = new RemoteMemWriter(tid);
}

ThreadContext::ThreadContext(uint64_t pid, uint64_t tid) {
    remote_tid = tid;
    process_context = std::make_shared<ProcessContext>((pid_t)pid);
    cur_read_surface = nullptr;
    cur_draw_surface = nullptr;
    opengl_context = nullptr;
    packet_poller = new ThreadPacketPoller(tid);
    default_call_poller = new FrameCallPoller();
    remote_mem_writer = new RemoteMemWriter(tid);
}

ThreadContext::~ThreadContext() {
    delete packet_poller;
    delete default_call_poller;
    delete remote_mem_writer;

    if (opengl_context) {
        unordered_map_erase_with_callback(
            process_context->gl_context_map, opengl_context->guest_context,
            static_cast<std::function<void(GLContext *)>>(
                GLContext::g_context_map_destroy));
        EglContext::native_make_current(nullptr);
    }
}

bool ThreadContext::make_current(EglSurface *draw_surface,
                                 EglSurface *read_surface, GLContext *context,
                                 uint64_t gbuffer_id) {
    if (cur_draw_surface == draw_surface && cur_read_surface == read_surface) {
        return EGL_TRUE;
    }

    // check if previously bound draw surface needs reset
    if (cur_draw_surface != nullptr && cur_draw_surface != draw_surface) {
        cur_draw_surface->reset();

        GraphicBuffer *old_draw_gbuffer = cur_draw_surface->gbuffer;
        ALOGD("makecurrent free draw_surface_handle surface %llx\n",
              (uint64_t)cur_draw_surface);
        if (cur_draw_surface->type == WINDOW_SURFACE &&
            old_draw_gbuffer->gbuffer_id != gbuffer_id) {
            //            old_draw_gbuffer->prepare_to_die();
        }
        if (cur_draw_surface->need_destroy) {
            delete cur_draw_surface;
        }
    }

    // check if previously bound read surface needs reset
    if (cur_read_surface != nullptr && cur_read_surface != cur_draw_surface &&
        cur_read_surface != read_surface) {
        cur_read_surface->reset();

        GraphicBuffer *old_draw_gbuffer = cur_read_surface->gbuffer;
        ALOGD("makecurrent free read_surface_handle surface %llx\n",
              (uint64_t)cur_read_surface);
        if (cur_read_surface->type == WINDOW_SURFACE &&
            old_draw_gbuffer->gbuffer_id != gbuffer_id) {
            //            old_draw_gbuffer->prepare_to_die();
        }
        if (cur_read_surface->need_destroy) {
            delete cur_read_surface;
        }
    }

    // check if previously bound context needs to be destroyed
    if (opengl_context != nullptr && opengl_context != context) {
        ALOGD("makecurrent context change %llx guest %llx %d window %llx\n",
              (uint64_t)opengl_context, (uint64_t)opengl_context->guest_context,
              opengl_context->need_destroy,
              (uint64_t)opengl_context->native_context);
        opengl_context->is_current = 0;
        opengl_context->signal_sync();
        if (opengl_context->need_destroy) {
            GLContext::opengl_context_destroy(opengl_context);
            free(opengl_context);
        }
    }

    // context is null, just release currently bound context
    if (read_surface == nullptr && draw_surface == nullptr &&
        context == nullptr) {
        if (opengl_context != nullptr) {
            ALOGD("thread %llx context %llx makecurrent window %llx null\n",
                  (int64_t)this, (int64_t)opengl_context,
                  (int64_t)opengl_context->native_context);
        }
        ALOGD(
            "#%llx makecurrent null read_surface_handle %llx "
            "draw_surface_handle %llx\n",
            (int64_t)context, (int64_t)cur_read_surface,
            (int64_t)cur_draw_surface);
        EglContext::native_make_current(nullptr);
        opengl_context->is_current = 0;
        opengl_context->signal_sync();

        opengl_context = nullptr;
        cur_draw_surface = nullptr;
        cur_read_surface = nullptr;
        return EGL_TRUE;
    }

    if (context->native_context == nullptr) {
        ALOGE("makecurrent context %llx native context null",
              (uint64_t)context);
    }
    EglContext::native_make_current(context->native_context);

    // bind the surfaces and context
    cur_read_surface = read_surface;
    if (read_surface != nullptr) read_surface->is_current = 1;

    cur_draw_surface = draw_surface;
    if (draw_surface != nullptr) {
        draw_surface->is_current = 1;
    }

    opengl_context = context;
    context->is_current = 1;

    return EGL_FALSE;
}

RenderThread::RenderThread(uint64_t pid, uint64_t tid) {
    thread_context = new ThreadContext(pid, tid);
    pthread_create(&poll_pthread, nullptr, poll_thread, this);
    pthread_setname_np(poll_pthread, ("pt-" + std::to_string(tid)).c_str());
    pthread_create(&rendering_pthread, nullptr, rendering_thread, this);
    pthread_setname_np(rendering_pthread,
                       ("rt-" + std::to_string(tid)).c_str());
}

RenderThread::RenderThread(std::shared_ptr<ProcessContext> &context,
                           uint64_t tid) {
    thread_context = new ThreadContext(context, tid);
    pthread_create(&poll_pthread, nullptr, poll_thread, (void *)this);
    pthread_setname_np(poll_pthread, ("pt-" + std::to_string(tid)).c_str());
    pthread_create(&rendering_pthread, nullptr, rendering_thread, (void *)this);
    pthread_setname_np(rendering_pthread,
                       ("rt-" + std::to_string(tid)).c_str());
}

RenderThread::~RenderThread() { delete thread_context; }

void RenderThread::stop() { thread_should_run = false; }

void *RenderThread::rendering_thread(void *data) {
    auto *render_thread = (RenderThread *)data;
    auto *thread_context = render_thread->thread_context;
    DataPacketHeader header{};

    while (render_thread->thread_should_run) {
        auto *surface_draw = thread_context->cur_draw_surface;
        FrameCallPoller *call_poller;
        if (surface_draw != nullptr) {
            call_poller = surface_draw->call_poller;
        } else {
            call_poller = thread_context->default_call_poller;
        }

        call_poller->poll_and_call(thread_context);
    }

    render_thread->poll_should_run = false;
    pthread_join(render_thread->poll_pthread, nullptr);

    delete render_thread;
    return nullptr;
}
void *RenderThread::poll_thread(void *data) {
    auto *render_thread = (RenderThread *)data;
    auto *thread_context = render_thread->thread_context;
    ThreadPacketPoller *packet_poller = thread_context->packet_poller;

    while (render_thread->poll_should_run) {
        auto stream_data = std::make_shared<StreamData>();
        DataPacketHeader *header = &stream_data->header;

        // Receive header
        packet_poller->recv_packet(reinterpret_cast<char *>(header),
                                   sizeof(DataPacketHeader));

        // Receive payload
        packet_poller->recv_packet(header->payload_len);

        // Decompress payload
        const char *payload = packet_poller->get_packet();
        stream_data->payload = std::make_shared<std::string>();
        thread_context->frame_decompressor.decompress(
            header, payload, header->payload_len, stream_data->payload.get());

        // Push stream data to queue
        thread_context->stream_data_queue.push(stream_data);
    }

    return nullptr;
}
