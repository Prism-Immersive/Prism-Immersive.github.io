#include "FrameCallPoller.h"

#include <algorithm>

#include "EglFuncID.h"
#include "Log.h"
#include "Utils.h"
#include "egl/EglTranslator.h"
#include "opengl/GLTranslator.h"
#include "xxhash.h"

#define LOG_TAG "FrameCallPoller"

GLFrameData::GLFrameData() {
    call_offsets_in_data =
        reinterpret_cast<uint64_t *>(malloc(128 * sizeof(uint64_t)));
    max_num_calls = 128;
    num_calls = 0;
    read_cursor = 0;

    call_data = reinterpret_cast<char *>(malloc(128));
    call_data_len = 0;
    call_data_max_len = 128;
    resource_para_len = 0;
}

GLFrameData::~GLFrameData() {
    free(call_offsets_in_data);
    free(call_data);
}

void GLFrameData::add_calls(ThreadContext *context, char *buf_data,
                            size_t buf_data_len, int num_added_calls) {
    if (unlikely(max_num_calls <= num_calls + num_added_calls)) {
        int len = ALIGN((num_calls + num_added_calls) * 2);
        void *temp = realloc(call_offsets_in_data, len * sizeof(uint64_t));
        if (likely(temp)) {
            call_offsets_in_data = reinterpret_cast<uint64_t *>(temp);
            max_num_calls = len;
        } else
            ALOGE("server failed to realloc call_offsets_in_data");
    }

    if (unlikely(call_data_len + buf_data_len > call_data_max_len)) {
        int len = ALIGN((call_data_len + buf_data_len) * 2);
        void *temp = realloc(call_data, len);
        if (likely(temp)) {
            call_data = reinterpret_cast<char *>(temp);
            call_data_max_len = len;
        } else
            ALOGE("server failed to realloc call_data");
    }

    char *cur_call_data = call_data + call_data_len;
    memcpy(cur_call_data, buf_data, buf_data_len);

    char *buf = cur_call_data;
    uint64_t data_len = 0;
    uint64_t decompression_space_size = 0;
    int start_index = num_calls;
    while (buf < cur_call_data + buf_data_len) {
        call_offsets_in_data[num_calls] =
            static_cast<uint64_t>(buf - call_data);

        auto *compress_gl_call = reinterpret_cast<CompressGLCall *>(buf);
        int call_size = COMPRESS_GL_CALL_SIZEOF(compress_gl_call);
        int resource_size = 0;
        for (int i = 0; compress_gl_call->para_type == MULTI_PARA &&
                        i < compress_gl_call->resource_para_num;
             i++) {
            // glBufferData and others may pass a null pointer
            if (compress_gl_call->paras[i].para_ptr == 0) continue;
            compress_gl_call->paras[i].para_ptr =
                call_data_len + data_len + call_size + resource_size;
            resource_size += compress_gl_call->paras[i].para_size;
            int decompress_bound =
                context->resource_decompressor.decompress_bound(
                    compress_gl_call, i, reinterpret_cast<uint64_t>(call_data));
            if (decompress_bound > 0)
                decompression_space_size += decompress_bound;
        }

        int total_size = call_size + resource_size;
        buf += total_size;
        data_len += total_size;
        resource_para_len += resource_size;
        num_calls++;
    }
    call_data_len += data_len;

    if (unlikely(call_data_len + decompression_space_size >
                 call_data_max_len)) {
        int len = ALIGN((call_data_len + decompression_space_size) * 2);
        void *temp = realloc(call_data, len);
        if (likely(temp)) {
            call_data = reinterpret_cast<char *>(temp);
            call_data_max_len = len;
        } else
            ALOGE("server failed to realloc call_data");
    }

    for (int i = start_index; i < num_calls; i++) {
        auto *call = reinterpret_cast<CompressGLCall *>(
            call_data + call_offsets_in_data[i]);
        for (int j = 0;
             call->para_type == MULTI_PARA && j < call->resource_para_num;
             j++) {
            if (call->paras[j].para_ptr == 0) continue;
            call->paras[j].para_ptr += reinterpret_cast<uint64_t>(call_data);
            parse_resource_data(context, call, j);
        }
    }
}

CompressGLCall *GLFrameData::read_one_call() {
    if (read_cursor >= num_calls) return nullptr;

    auto *compress_gl_call =
        (CompressGLCall *)&call_data[call_offsets_in_data[read_cursor]];
    read_cursor++;
    return compress_gl_call;
}

void GLFrameData::parse_resource_data(ThreadContext *context,
                                      CompressGLCall *call, int res_index) {
    if (call->paras[res_index].para_ptr == 0) return;

    int decompress_bound =
        context->resource_decompressor.decompress_bound(call, res_index, 0);
    if (decompress_bound < 0) return;

    char *out_buf = call_data + call_data_len;
    call_data_len += decompress_bound;
    int ret = context->resource_decompressor.decompress(
        call, res_index, out_buf, decompress_bound);
    if (ret > 0) {
        call->paras[res_index].para_size = ret;
    }
}

void FrameCallPoller::poll_and_call(ThreadContext *context) {
    while (context->stream_data_queue.empty()) {
        usleep(1);
    }
    std::shared_ptr<StreamData> stream_data =
        std::move(context->stream_data_queue.front());
    context->stream_data_queue.pop();
    DataPacketHeader *header = &stream_data->header;
    std::string *payload = stream_data->payload.get();

    context->flow_controller.begin_service();
    CompressGLCall *compress_gl_call;
    if (header->frame_call_type == FRAME_CALL_CONTEXT_LESS) {
        process_context_less_call(context, payload);

        compress_gl_call = reinterpret_cast<CompressGLCall *>(&(*payload)[0]);

        if (compress_gl_call->call_id < FUNID_EGL_END)
            EglTranslator::translate(context, compress_gl_call);
        else
            GLTranslator::translate(context, compress_gl_call);
    } else {
        process_frame_calls(context, header, payload);

        compress_gl_call = cur_frame_data->read_one_call();
        while (compress_gl_call) {
            if (compress_gl_call->call_id < FUNID_EGL_END)
                EglTranslator::translate(context, compress_gl_call);
            else
                GLTranslator::translate(context, compress_gl_call);
            compress_gl_call = cur_frame_data->read_one_call();
        }
    }

    context->flow_controller.end_service();
    if (header->frame_call_type == FRAME_CALL_END) {
        context->flow_controller.set_inter_arrival_time(header->frame_time);
        context->flow_controller.end_frame_service();
        context->flow_controller.decision(context);
        end_one_frame();
    }
}

void FrameCallPoller::process_frame_calls(ThreadContext *context,
                                          DataPacketHeader *header,
                                          std::string *data) {
    cur_frame_data->add_calls(reinterpret_cast<ThreadContext *>(context),
                              &(*data)[0], header->actual_data_len,
                              header->actual_gl_num);
}

void FrameCallPoller::process_context_less_call(ThreadContext *context,
                                                std::string *data) {
    char *call_buf = &(*data)[0];
    auto *compress_gl_call = reinterpret_cast<CompressGLCall *>(call_buf);
    int call_size = COMPRESS_GL_CALL_SIZEOF(compress_gl_call);
    int resource_size = 0;
    for (int i = 0; compress_gl_call->para_type == MULTI_PARA &&
                    i < compress_gl_call->resource_para_num;
         i++) {
        compress_gl_call->paras[i].para_ptr =
            (uint64_t)(call_buf + call_size + resource_size);
        resource_size += compress_gl_call->paras[i].para_size;
    }
}

FrameCallPoller::FrameCallPoller() {
    pre_frame_data = new GLFrameData();
    cur_frame_data = new GLFrameData();
}

FrameCallPoller::~FrameCallPoller() {
    delete pre_frame_data;
    delete cur_frame_data;
}

void FrameCallPoller::end_one_frame() {
    GLFrameData *temp = cur_frame_data;
    cur_frame_data = pre_frame_data;
    pre_frame_data = temp;

    cur_frame_data->resource_para_len = 0;
    cur_frame_data->num_calls = 0;
    cur_frame_data->call_data_len = 0;
    cur_frame_data->read_cursor = 0;
}

void FrameCallPoller::return_call(ThreadContext *context, char *buf,
                                  size_t buf_len) {
    DataPacketHeader header;
    header.payload_len = buf_len;

    context->packet_poller->send_packet(&header, buf);
}
