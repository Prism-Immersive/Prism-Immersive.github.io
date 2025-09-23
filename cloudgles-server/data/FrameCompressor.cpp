#define LOG_TAG "FrameCompressor"
#include "FrameCompressor.h"

#include <atomic>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <string>

#include "Compression.h"
#include "GLFuncID.h"
#include "Log.h"
#include "Utils.h"
#include "network/FrameCallStreamer.h"

FrameCompressor::FrameCompressor() {
    compressed_data = new char[data_max_len];
    zstd_cctx = ZSTD_createCCtx();
    ZSTD_CCtx_setParameter(zstd_cctx, ZSTD_c_compressionLevel, 1);
    ZSTD_CCtx_setParameter(zstd_cctx, ZSTD_c_checksumFlag, 0);
    ZSTD_CCtx_reset(zstd_cctx, ZSTD_reset_session_only);
}

FrameCompressor::~FrameCompressor() {
    delete[] compressed_data;
    ZSTD_freeCCtx(zstd_cctx);
}

std::shared_ptr<std::string> FrameCompressor::compress_top_half(
    GLFrameData *cur_frame_data, uint64_t *original_len, uint32_t *num_calls) {
    char *buf = cur_frame_data->call_data;
    uint64_t buf_start = cur_frame_data->call_data_send_len;
    uint64_t buf_end = cur_frame_data->call_data_len;
    uint64_t data_len = cur_frame_data->resource_para_len -
                        cur_frame_data->resource_para_send_len;
    auto resource_data_base = (uint64_t)cur_frame_data->resource_data;

    uint64_t max_buf_size = buf_end - buf_start + data_len;
    auto data_buf = std::make_shared<std::string>();
    data_buf->resize(max_buf_size);

    uint64_t buf_cursor = buf_start;
    uint64_t call_len = 0;
    *num_calls = 0;
    uint64_t data_buf_ptr = 0;

    while (true) {
        buf_cursor += call_len;
        if (buf_cursor >= buf_end) break;
        auto *call = (CompressGLCall *)(buf + buf_cursor);
        call_len = COMPRESS_GL_CALL_SIZEOF(call);
        *num_calls += 1;
        memcpy(data_buf->data() + data_buf_ptr, call, call_len);
        call = (CompressGLCall *)(data_buf->data() + data_buf_ptr);
        data_buf_ptr += call_len;

        // Do not remove the 8 bytes address of each parameter,
        // because it is too small compared to the data it points to,
        // and will not cause great pressure on data transmission.

        // If it is removed, it will be copied separately, reducing the data
        // locality and affecting the copy efficiency. It is better to use it to
        // store the data offset.
        for (int i = 0;
             call->para_type == MULTI_PARA && i < call->resource_para_num;
             i++) {
            if (call->paras[i].para_ptr == 0) continue;
            memcpy(data_buf->data() + data_buf_ptr,
                   (const void *)(call->paras[i].para_ptr + resource_data_base),
                   call->paras[i].para_size);
            call->paras[i].para_ptr =
                (uint64_t)data_buf->data() + data_buf_ptr - (uint64_t)call;
            data_buf_ptr += call->paras[i].para_size;
        }
    }
    *original_len = data_buf_ptr;

    return data_buf;
}

void FrameCompressor::compress_bottom_half(int *compress_type, std::string *data_buf,
                                         uint64_t data_len, std::string &out_buf, uint64_t *out_len) {
    *compress_type = COMPRESS_TYPE_NORMAL;
    uint64_t compress_bound = ZSTD_compressBound(data_len);
    out_buf.resize(ALIGN(compress_bound));
    *out_len = ZSTD_compress2(
        zstd_cctx, out_buf.data(), out_buf.size(), data_buf->data(), data_len);
}

int FrameCompressor::compress(char *buf, int buf_start, int buf_end,
                              int data_len) {
    return 0;
}