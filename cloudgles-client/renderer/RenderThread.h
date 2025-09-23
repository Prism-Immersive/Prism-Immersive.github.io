#ifndef RENDER_THREAD_H
#define RENDER_THREAD_H

#include <boost/lockfree/spsc_queue.hpp>
#include <memory>

#include "data/Decompressor.h"
#include "data/FrameDecompressor.h"
#include "data/ResourceDecompressor.h"
#include "network/FlowController.h"
#include "network/RemoteMemWriter.h"
#include "network/ThreadPacketPoller.h"
#include "opengl/Context.h"

class GLFrameData;
class FrameCallPoller;

class MainController;

class ProcessContext {
   public:
    uint64_t remote_pid;
    std::unordered_map<EGLSurface, EglSurface *> surface_map;
    std::unordered_map<EGLContext, GLContext *> gl_context_map;
    std::unordered_map<GLuint64, GraphicBuffer *> gbuffer_map;

    ResourceMapStatus *egl_sync_resource;

    ProcessContext(pid_t pid);

    ~ProcessContext();
};

struct StreamData {
    DataPacketHeader header{};
    std::shared_ptr<std::string> payload = nullptr;
};

class ThreadContext {
   public:
    uint64_t remote_tid;
    std::shared_ptr<ProcessContext> process_context;

    EglSurface *cur_read_surface;
    EglSurface *cur_draw_surface;

    GLContext *opengl_context;

    FrameCallPoller *default_call_poller;
    ThreadPacketPoller *packet_poller;
    RemoteMemWriter *remote_mem_writer;

    FrameDecompressor frame_decompressor;
    ResourceDecompressor resource_decompressor;

    uint64_t remote_mem = 0;

    FlowController flow_controller;

    boost::lockfree::spsc_queue<std::shared_ptr<StreamData>,
                                boost::lockfree::capacity<4096>>
        stream_data_queue;

    ThreadContext(std::shared_ptr<ProcessContext> &context, uint64_t tid);
    ThreadContext(uint64_t pid, uint64_t tid);
    ~ThreadContext();

    bool make_current(EglSurface *draw_surface, EglSurface *read_surface,
                      GLContext *context, uint64_t gbuffer_id);
};

class RenderThread {
   public:
    ThreadContext *thread_context;

    RenderThread(uint64_t pid, uint64_t tid);

    RenderThread(std::shared_ptr<ProcessContext> &context, uint64_t tid);

    ~RenderThread();

    void stop();

   private:
    bool thread_should_run = true;
    bool poll_should_run = true;
    pthread_t rendering_pthread{};
    pthread_t poll_pthread{};

    static void *rendering_thread(void *data);
    static void *poll_thread(void *data);
};

#endif  // RENDER_THREAD_H
