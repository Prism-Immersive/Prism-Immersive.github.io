#ifndef FRAME_CALL_STREAMER_H
#define FRAME_CALL_STREAMER_H

#include <bits/pthread_types.h>
#include <pthread.h>

#include <atomic>
#include <boost/lockfree/spsc_queue.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "Compression.h"
#include "Event.h"
#include "data/FrameCompressor.h"
#include "network/ThreadPacketStreamer.h"

#define BUFFER_LENGTH 4 * 1024

class ThreadContext;

class GLFrameData {
   public:
    uint64_t *call_offsets_in_data;  // The offset of the gl_call in call_data
    uint64_t max_num_calls = 128;    // The max size of call_offsets_in_data

    uint32_t num_calls = 0;       // The index of the next call to be added in
                                  // call_offsets_in_data
    uint64_t num_send_calls = 0;  // The index of the next call to be sent in
                                  // call_offsets_in_data
    int draw_call_cursor = 0;     // The index of the latest draw call

    char *call_data;                   // An array of CompressGLCall
    uint64_t call_data_max_len = 128;  // The max size of call_data
    uint64_t call_data_len = 0;        // The size of call_data
    uint64_t call_data_send_len =
        0;  // The size of call_data that has been sent

    uint64_t resource_para_len = 0;       // The total length of all resource
                                          // parameters in call_data
    uint64_t resource_para_send_len = 0;  // The length of resource parameters
                                          // that has been sent

    char *resource_data;                   // The actual resource data
    uint64_t resource_data_max_len = 128;  // The max size of resource_data
    uint64_t resource_data_len =
        4;  // The size of resource_data, set to 4 to avoid offset calculation
            // to be 0, which specifically represents NULL pointer

    void *cur_thread_context = nullptr;

    GLFrameData();

    ~GLFrameData();

    int add_one_call(void *buf_data, size_t buf_data_len);

    void cache_resource_data(void *context, uint32_t call_index,
                             CompressGLCall *call, int res_index, char *buf,
                             size_t buf_len);

    void reset();

    static void gl_call_to_compress_call(GLCall *gl_call,
                                         CompressGLCall *compress_call,
                                         uint64_t *resource_size);

    static size_t compress_call_size_from(GLCall *gl_call);
};

struct StreamData {
    DataPacketHeader header{};
    std::shared_ptr<std::string> payload = nullptr;
    ThreadContext *context = nullptr;
    uint64_t last_sleep_time = 0;
    uint64_t frame_start_time = 0;
};

class FrameCallStreamer {
   public:
    FrameCallStreamer();
    FrameCallStreamer(const FrameCallStreamer &) = delete;
    ~FrameCallStreamer();
    FrameCallStreamer &operator=(const FrameCallStreamer &) = delete;

    void call_and_stream(void *buf_data, size_t buf_data_len,
                         FrameCallType call_type = FRAME_CALL_NORMAL);

    static void call_return(char *buf, size_t buf_len);

    void start_frames();

    void end_one_frame();

    void flush_all_call(bool synchronous = true);

   private:
    GLFrameData *pre_frame_data = new GLFrameData();
    GLFrameData *cur_frame_data = new GLFrameData();
    bool is_first_frame = true;

    pthread_t async_stream_thread{};
    bool stream_should_run = true;
    boost::lockfree::spsc_queue<std::shared_ptr<StreamData>, boost::lockfree::capacity<4096>>
        stream_data_queue;

    void stream_to_client(std::shared_ptr<std::string> frame_data,
                          uint64_t original_len, FrameCallType call_type,
                          uint32_t num_calls, int compress_type);

    void context_less_call(void *buf_data, size_t buf_data_len);
    static void *async_stream(void *data);
    void finish_stream();
};

#endif  // FRAME_CALL_STREAMER_H
