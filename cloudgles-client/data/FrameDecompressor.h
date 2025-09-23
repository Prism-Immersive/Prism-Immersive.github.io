#ifndef FRAME_DECOMPRESSOR_H
#define FRAME_DECOMPRESSOR_H

#include <string>

#include "Compression.h"
#include "Decompressor.h"
#include "zstd.h"

class GLFrameData;

class FrameDecompressor : public Decompressor {
   public:
    FrameDecompressor();
    ~FrameDecompressor();
    int decompress(char *buf, int buf_start, int buf_end,
                   int data_len) override;

    void decompress(void *context, GLFrameData *cur_frame_data, char *raw_data,
                    DataPacketHeader *header, int compress_type);
    void decompress(const DataPacketHeader *header, const char *in_buf, uint64_t in_buf_len, std::string *out_buf);

    std::string decompressed_data;

   private:
    ZSTD_DCtx *zstd_dctx = nullptr;
};

#endif  // FRAME_DECOMPRESSOR_H
