#ifndef FRAMECOMPRESSIOR_H
#define FRAMECOMPRESSIOR_H

#include <stdint.h>
#include <zstd.h>

#include <atomic>
#include <cstdint>
#include <memory>

#include "Compressor.h"

class GLFrameData;

class FrameCompressor : public Compressor {
   public:
    int compress(char *buf, int buf_start, int buf_end, int data_len) override;

    std::shared_ptr<std::string> compress_top_half(GLFrameData *cur_frame_data, uint64_t *original_len, uint32_t *num_calls);

    void compress_bottom_half(int *compress_type, std::string *data_buf, uint64_t data_len, std::string &out_buf, uint64_t *out_len);

    char *get_compressed_data() { return compressed_data; }

    uint64_t get_compressed_data_len() { return compressed_data_len; }

    FrameCompressor();

    ~FrameCompressor() final;

   private:
    char *compressed_data = nullptr;
    uint64_t compressed_data_len = 0;
    uint64_t data_max_len = 10000;
    ZSTD_CCtx *zstd_cctx;
};

#endif  // FRAMECOMPRESSIOR_H