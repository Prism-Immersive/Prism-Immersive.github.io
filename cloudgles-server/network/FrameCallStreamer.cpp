#include <sys/types.h>
#define LOG_TAG "FrameCallStreamer"
#define ATRACE_TAG ATRACE_TAG_GRAPHICS
#include "FrameCallStreamer.h"

#include <bits/pthread_types.h>
#include <pthread.h>
#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "Compression.h"
#include "GLFuncID.h"
#include "Log.h"
#include "ThreadContext.h"
#include "Utils.h"
#include "network/Event.h"
#include "network/ThreadPacketStreamer.h"

GLFrameData::GLFrameData() {
    call_offsets_in_data = (uint64_t *)malloc(128 * sizeof(uint64_t));
    call_data = (char *)malloc(128);
    resource_data = (char *)malloc(128);
    cur_thread_context = egl_thread_context;
}

GLFrameData::~GLFrameData() {
    free(call_offsets_in_data);
    free(call_data);
    free(resource_data);
}

int GLFrameData::add_one_call(void *buf_data, size_t buf_data_len) {
    ATRACE_CALL();
    auto *send_buf64 = (uint64_t *)buf_data;
    if (buf_data_len / 16 - 1 != send_buf64[1]) {
        ALOGE("error send_buf_len %lld paras num %lld",
              (unsigned long long)buf_data_len / 8,
              (unsigned long long)send_buf64[1]);
    }

    auto *gl_call = (GLCall *)buf_data;

    size_t compress_call_size = compress_call_size_from(gl_call);

    if (unlikely(max_num_calls <= num_calls + 1)) {
        size_t len = ALIGN((max_num_calls)*2);
        void *temp = realloc(call_offsets_in_data, len * sizeof(uint64_t));
        if (likely(temp)) {
            call_offsets_in_data = (uint64_t *)temp;
            max_num_calls = len;
        } else
            ALOGE("server failed to realloc call_offsets_in_data");
    }

    if (unlikely(call_data_len + compress_call_size > call_data_max_len)) {
        size_t len = ALIGN((call_data_len + compress_call_size) * 2);
        void *temp = realloc(call_data, len);
        if (likely(temp)) {
            call_data = (char *)temp;
            call_data_max_len = len;
        } else
            ALOGE("server failed to realloc call_data");
    }

    auto *compress_call = (CompressGLCall *)(call_data + call_data_len);

    gl_call_to_compress_call(gl_call, compress_call, &resource_para_len);

    if (compress_call->para_type == MULTI_PARA) {
        for (int i = 0; i < compress_call->resource_para_num; i++) {
            auto *buf =
                reinterpret_cast<char *>(compress_call->paras[i].para_ptr);
            size_t buf_len = compress_call->paras[i].para_size;

            cache_resource_data(egl_thread_context, num_calls, compress_call, i,
                                buf, buf_len);
        }
    }

    call_offsets_in_data[num_calls] = call_data_len;
    int call_size = COMPRESS_GL_CALL_SIZEOF(compress_call);
    call_data_len += call_size;
    num_calls++;

    return call_size;
}

void GLFrameData::gl_call_to_compress_call(GLCall *gl_call,
                                           CompressGLCall *compress_call,
                                           uint64_t *resource_size) {
    bool has_common_para = false;
    if (gl_call->para_num > 0 &&
        gl_call->paras[0].para_size < MAX_COMMON_PARA_LEN)
        has_common_para = true;
    compress_call->call_id = (unsigned short)gl_call->call_id;

    if (gl_call->para_num == 0) {
        compress_call->para_type = ZERO_PARA;
    } else {
        compress_call->para_type = MULTI_PARA;
        int start_index = 0;
        if (has_common_para) {
            compress_call->common_para_len =
                (unsigned short)gl_call->paras[0].para_size;
            start_index = 1;
        } else {
            compress_call->common_para_len = 0;
        }

        compress_call->resource_para_num =
            (unsigned short)(gl_call->para_num - start_index);

        int para_size;
        uint64_t para_ptr;
        for (int i = start_index; i < gl_call->para_num; i++) {
            para_size = (int)gl_call->paras[i].para_size;
            para_ptr = gl_call->paras[i].para_ptr;

            // glBufferData and others may pass NULL pointer
            if (para_ptr == 0) para_size = 0;

            compress_call->paras[i - start_index].para_size = para_size;
            compress_call->paras[i - start_index].para_ptr = para_ptr;
            *resource_size += para_size;
        }

        if (has_common_para) {
            // compress_call->paras[compress_call->para_num] is the end of the
            // array
            memcpy(&(compress_call->paras[compress_call->resource_para_num]),
                   (const void *)gl_call->paras[0].para_ptr,
                   compress_call->common_para_len);
        }
    }
}

void GLFrameData::cache_resource_data(void *context, uint32_t call_index,
                                      CompressGLCall *call, int res_index,
                                      char *buf, size_t buf_len) {
    ATRACE_CALL();
    auto *thread_context = reinterpret_cast<ThreadContext *>(context);

    // glBufferData and others may pass NULL pointer
    if (buf == nullptr) return;

    int64_t compress_bound =
        thread_context->resource_compressor.compress_bound(call, res_index);
    if (compress_bound >= 0) buf_len = compress_bound;

    if (unlikely(resource_data_len + buf_len > resource_data_max_len)) {
        size_t len = ALIGN((resource_data_len + buf_len) * 2);
        void *temp = realloc(resource_data, len);
        if (likely(temp)) {
            resource_data = (char *)temp;
            resource_data_max_len = len;
        }
    }

    uint64_t ret = resource_data_len;
    if (compress_bound >= 0) {
        int compressed_len = thread_context->resource_compressor.compress(
            context, call_index, call, res_index,
            (resource_data + resource_data_len), buf_len);
        resource_data_len += compressed_len;
        resource_para_len +=
            (compressed_len - call->paras[res_index].para_size);
        call->paras[res_index].para_size = compressed_len;
    } else {
        memcpy(resource_data + resource_data_len, buf, buf_len);
        resource_data_len += buf_len;
    }
    call->paras[res_index].para_ptr = ret;
}

void GLFrameData::reset() {
    num_calls = 0;
    num_send_calls = 0;

    call_data_len = 0;
    call_data_send_len = 0;

    resource_para_len = 0;
    resource_para_send_len = 0;

    resource_data_len = 4;
}

size_t GLFrameData::compress_call_size_from(GLCall *gl_call) {
    size_t compress_call_len = 0;
    bool has_common_para = false;

    if (gl_call->para_num > 0 &&
        gl_call->paras[0].para_size < MAX_COMMON_PARA_LEN) {
        compress_call_len += gl_call->paras[0].para_size;
        has_common_para = true;
    }

    return compress_call_len +
           (gl_call->para_num - (has_common_para ? 1 : 0)) *
               sizeof(CompressParaElem) +
           4;
}

FrameCallStreamer::FrameCallStreamer() {
    pthread_create(&async_stream_thread, nullptr, async_stream, this);
}

FrameCallStreamer::~FrameCallStreamer() {
    flush_all_call();
    stream_should_run = false;
    pthread_join(async_stream_thread, nullptr);
    delete cur_frame_data;
    delete pre_frame_data;
}

/**
 * Add a call to the current frame or stream the call buf to the client
 * @param buf_data The data of the call
 * @param buf_data_len The length of the data
 */
void FrameCallStreamer::call_and_stream(void *buf_data, size_t buf_data_len,
                                        FrameCallType call_type) {
    if (call_type == FRAME_CALL_CONTEXT_LESS) {
        context_less_call(buf_data, buf_data_len);
    } else {
        cur_frame_data->add_one_call(buf_data, buf_data_len);

        if (call_type == FRAME_CALL_FLUSH) {
            flush_all_call(true);
        } else if (cur_frame_data->num_calls - cur_frame_data->num_send_calls >=
                   UINT16_MAX) {
            flush_all_call(false);
        }

        if (call_type == FRAME_CALL_END) {
            end_one_frame();
        }
    }
}

/**
 * Flush the call buffer to the client
 */
void FrameCallStreamer::flush_all_call(bool synchronous) {
    ATRACE_CALL();

    auto *thread_context =
        reinterpret_cast<ThreadContext *>(egl_thread_context);

    if (cur_frame_data->num_calls == 0 ||
        cur_frame_data->call_data_send_len == cur_frame_data->call_data_len) {
        return;
    }

    uint64_t original_len = 0;
    uint32_t num_calls = 0;
    int compress_type = COMPRESS_TYPE_NORMAL;
    std::shared_ptr<std::string> frame_data =
        thread_context->frame_compressor.compress_top_half(
            cur_frame_data, &original_len, &num_calls);

    stream_to_client(frame_data, original_len, FRAME_CALL_FLUSH, num_calls,
                     compress_type);

    if (synchronous) {
        finish_stream();
    }
}

/**
 * Mark the start of a new frame
 */
void FrameCallStreamer::start_frames() { is_first_frame = true; }

/**
 * Mark the end of a frame and stream the data to the client
 */
void FrameCallStreamer::end_one_frame() {
    auto *thread_context =
        reinterpret_cast<ThreadContext *>(egl_thread_context);

    uint64_t original_len = 0;
    uint32_t num_calls = 0;
    int compress_type = COMPRESS_TYPE_NORMAL;
    std::shared_ptr<std::string> frame_data =
        thread_context->frame_compressor.compress_top_half(
            cur_frame_data, &original_len, &num_calls);

    stream_to_client(frame_data, original_len, FRAME_CALL_END, num_calls,
                     compress_type);

    // Do not swap the frame data if it's the first frame
    if (is_first_frame) {
        is_first_frame = false;
    } else {
        GLFrameData *temp = cur_frame_data;
        cur_frame_data = pre_frame_data;
        pre_frame_data = temp;
    }

    cur_frame_data->reset();
}

void FrameCallStreamer::stream_to_client(
    std::shared_ptr<std::string> frame_data, uint64_t original_len,
    FrameCallType call_type, uint32_t num_calls, int compress_type) {
    ATRACE_CALL();
    auto stream_data = std::make_shared<StreamData>();

    // Set payload
    stream_data->payload = std::move(frame_data);

    // Set thread context
    auto *thread_context = (ThreadContext *)egl_thread_context;
    stream_data->context = thread_context;

    // Set frame statistics
    if (call_type == FRAME_CALL_END) {
        struct timeval cur_time {};
        gettimeofday(&cur_time, nullptr);
        uint64_t cur_time_us = cur_time.tv_sec * 1000000 + cur_time.tv_usec;
        stream_data->frame_start_time = thread_context->surface_draw->frame_server_start_time;
        stream_data->last_sleep_time = thread_context->flow_controller.last_sleep_time;
        stream_data->header.server_latency = (uint32_t)(cur_time_us - stream_data->frame_start_time);
    }

    // Set packet header
    DataPacketHeader &packet_header = stream_data->header;
    // Need to be adjusted post bottom half compression
    packet_header.payload_len = (uint32_t)original_len;
    packet_header.compress_type = compress_type;
    // No need to adjust post bottom half compression
    packet_header.frame_call_type = call_type;
    packet_header.actual_data_len = (uint32_t)original_len;
    packet_header.actual_gl_num = num_calls;

    // Push to the queue
    while (!stream_data_queue.push(stream_data)) {
        // Wait for the queue to be available
    }

    // Shift cur_frame_data cursors
    cur_frame_data->num_send_calls += num_calls;
    cur_frame_data->call_data_send_len = cur_frame_data->call_data_len;
    cur_frame_data->resource_para_send_len = cur_frame_data->resource_para_len;

    // Get render statistics
    if (unlikely(thread_context->surface_draw != nullptr &&
                 thread_context->surface_draw->frame_client_start_time == 0)) {
        struct timeval cur_time {};
        gettimeofday(&cur_time, nullptr);
        thread_context->surface_draw->frame_client_start_time =
            cur_time.tv_sec * 1000000 + cur_time.tv_usec;
    }
}

void *FrameCallStreamer::async_stream(void *data) {
    auto *call_streamer = (FrameCallStreamer *)data;
    while (call_streamer->stream_should_run) {
        while (call_streamer->stream_data_queue.read_available()) {
            std::shared_ptr<StreamData> stream_data =
                std::move(call_streamer->stream_data_queue.front());

            FrameCompressor *frame_compressor =
                &stream_data->context->frame_compressor;
            ThreadPacketStreamer *packet_streamer =
                stream_data->context->thread_packet_streamer;

            // Bottom half compression of the frame data
            int compress_type = 0;
            std::string compressed_data;
            uint64_t compressed_data_len = 0;
            frame_compressor->compress_bottom_half(
                &compress_type, stream_data->payload.get(),
                stream_data->header.payload_len, compressed_data, &compressed_data_len);

            stream_data->header.compress_type = compress_type;
            stream_data->header.payload_len =
                (uint32_t)compressed_data_len;

            // Set frame end time
            if (stream_data->header.frame_call_type == FRAME_CALL_END) {
                struct timeval cur_time {};
                gettimeofday(&cur_time, nullptr);
                uint64_t cur_time_us = cur_time.tv_sec * 1000000 + cur_time.tv_usec;
                stream_data->header.frame_time = stream_data->last_sleep_time + cur_time_us - stream_data->frame_start_time;
            } else {
                stream_data->header.frame_time = 0;
            }

            // Send the packet
            packet_streamer->send_packet(
                &stream_data->header, compressed_data.data());

            // Pop the data from the queue
            call_streamer->stream_data_queue.pop();
        }
        usleep(1);
    }
    return nullptr;
}

void FrameCallStreamer::finish_stream() {
    while (!stream_data_queue.empty()) {
        // Wait for the queue to be empty
    }
}

/**
 * Used to stream call data when there's still no context currently
 * @param buf_data The data of the call
 * @param buf_data_len The length of the data
 */
void FrameCallStreamer::context_less_call(void *buf_data, size_t buf_data_len) {
    auto *send_buf64 = (uint64_t *)buf_data;
    if (buf_data_len / 16 - 1 != send_buf64[1]) {
        ALOGE("error send_buf_len %lld paras num %lld",
              (unsigned long long)buf_data_len / 8,
              (unsigned long long)send_buf64[1]);
    }

    auto *gl_call = (GLCall *)buf_data;
    size_t compress_gl_call_size =
        GLFrameData::compress_call_size_from(gl_call);
    auto *compress_gl_call = (CompressGLCall *)alloca(compress_gl_call_size);
    uint64_t resource_data_len = 0;
    GLFrameData::gl_call_to_compress_call(gl_call, compress_gl_call,
                                          &resource_data_len);

    DataPacketHeader packet_header;

    packet_header.payload_len = COMPRESS_GL_CALL_SIZEOF(compress_gl_call);
    packet_header.frame_call_type = FRAME_CALL_CONTEXT_LESS;
    packet_header.actual_data_len =
        (uint32_t)resource_data_len + packet_header.payload_len;
    packet_header.compress_type = COMPRESS_TYPE_NONE;
    packet_header.actual_gl_num = 1;

    struct iovec *extra_data = nullptr;
    int extra_data_num = 0;
    if (compress_gl_call->para_type == MULTI_PARA) {
        extra_data_num = compress_gl_call->resource_para_num;
        extra_data = new iovec[extra_data_num];
        for (int i = 0; i < compress_gl_call->resource_para_num; i++) {
            extra_data[i].iov_len = compress_gl_call->paras[i].para_size;
            extra_data[i].iov_base =
                (void *)compress_gl_call->paras[i].para_ptr;
        }
    }

    auto *thread_context = (ThreadContext *)egl_thread_context;
    auto *packet_streamer = thread_context->thread_packet_streamer;
    packet_streamer->send_packet(&packet_header, (char *)compress_gl_call,
                                 extra_data, extra_data_num);

    delete[] extra_data;
}

void FrameCallStreamer::call_return(char *buf, size_t buf_len) {
    auto *thread_context = (ThreadContext *)egl_thread_context;
    auto *packet_streamer = thread_context->thread_packet_streamer;
    packet_streamer->recv_packet(buf, buf_len);
}
