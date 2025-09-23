#include "FrameDecompressor.h"

#include "Log.h"
#include "Utils.h"
#include "network/FrameCallPoller.h"

FrameDecompressor::FrameDecompressor() { zstd_dctx = ZSTD_createDCtx(); }

FrameDecompressor::~FrameDecompressor() {
    if (zstd_dctx) ZSTD_freeDCtx(zstd_dctx);
}

int FrameDecompressor::decompress(char *buf, int buf_start, int buf_end,
                                  int data_len) {
    return 0;
}

void FrameDecompressor::decompress(void *context, GLFrameData *cur_frame_data,
                                   char *raw_data, DataPacketHeader *header,
                                   int compress_type) {
    if (compress_type == COMPRESS_TYPE_NONE) {
        cur_frame_data->add_calls(reinterpret_cast<ThreadContext *>(context),
                                  raw_data, header->payload_len,
                                  header->actual_gl_num);
    } else if (compress_type == COMPRESS_TYPE_NORMAL) {
        if (decompressed_data.size() < header->actual_data_len) {
            decompressed_data.resize(ALIGN(header->actual_data_len));
        }
        size_t decompressed_size = ZSTD_decompressDCtx(
            zstd_dctx, &decompressed_data[0], decompressed_data.size(),
            raw_data, header->payload_len);
        assert(decompressed_size == header->actual_data_len);
        cur_frame_data->add_calls(reinterpret_cast<ThreadContext *>(context),
                                  &decompressed_data[0], decompressed_size,
                                  header->actual_gl_num);
    }
}

void FrameDecompressor::decompress(const DataPacketHeader *header, const char *in_buf, uint64_t in_buf_len,
                                   std::string *out_buf) {
    if (header->compress_type == COMPRESS_TYPE_NONE) {
        out_buf->resize(in_buf_len);
        memcpy(&(*out_buf)[0], in_buf, in_buf_len);
        assert(in_buf_len == header->actual_data_len);
    } else if (header->compress_type == COMPRESS_TYPE_NORMAL) {
        out_buf->resize(header->actual_data_len);
        size_t decompressed_size = ZSTD_decompressDCtx(
            zstd_dctx, &(*out_buf)[0], out_buf->size(),
            in_buf, in_buf_len);
        assert(decompressed_size == header->actual_data_len);
    }
}
