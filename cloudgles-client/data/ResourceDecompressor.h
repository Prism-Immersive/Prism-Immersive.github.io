#ifndef RESOURCE_DECOMPRESSOR_H
#define RESOURCE_DECOMPRESSOR_H

#include <stdint.h>

#include <unordered_map>
#include <unordered_set>
#include <zstd.h>

#include "Compression.h"
#include "Decompressor.h"
#include "GLFuncID.h"
#include "LRUCache.h"

class GLFrameData;
class ThreadContext;

#define LRU_CACHE_SIZE 1000
#define LRU_CACHE_THRESHOLD 100000

typedef struct {
    uint64_t len;
    char *data;
} ResourceBuffer;

enum class CompressMode {
    ZLIB = 0,
    ZSTD,
};

class ResourceDecompressor : public Decompressor {
   public:
    int decompress(char *buf, int buf_start, int buf_end,
                   int data_len) override;

    int decompress(CompressGLCall *call, int res_index, char *out_buf,
                   size_t out_buf_len);

    int decompress_bound(CompressGLCall *call, int res_index,
                         uint64_t res_addr_offset);

    ResourceDecompressor();

    ~ResourceDecompressor();

   private:
    static int decompress_texture(CompressGLCall *call, int res_index,
                                  char *out_buf, size_t out_buf_len);

    int decompress_vertex(CompressGLCall *call, int res_index, char *out_buf,
                          size_t out_buf_len);

    int gzip_decompress(unsigned char *idata, uint64_t ilen,
                               unsigned char *odata, uint64_t *olen);
    int zstd_decompress(unsigned char *idata, uint64_t ilen,
                        unsigned char *odata, uint64_t *olen);
    int fpzip_decompress(unsigned char *idata, unsigned char *odata, uint64_t *olen);
    int fp16_decompress(unsigned char *idata, uint64_t ilen, unsigned char *odata, uint64_t *olen);

    const std::unordered_set<int> vertex_calls{
        FUNID_glBufferData_custom, FUNID_glBufferSubData_custom,
        FUNID_glFlushMappedBufferRange_special};
    const std::unordered_set<int> texture_calls{
        FUNID_glTexImage2D_without_bound, FUNID_glTexImage2D_with_bound,
        FUNID_glTexSubImage2D_without_bound, FUNID_glTexSubImage2D_with_bound};
    static void bitwise_xor(const char *a, char *output, uint64_t a_len,
                            uint64_t out_len);
    std::unordered_set<int> vertex_texture_calls;
    std::unordered_map<uint32_t, ResourceBuffer *> prev_buf_map;
    std::unique_ptr<LRUCache<uint64_t, std::string>> vertex_cache;
    std::string quantization_buf;

    CompressMode compress_mode = CompressMode::ZSTD;
    int (ResourceDecompressor::*decompress_func)(unsigned char *, uint64_t, unsigned char *, uint64_t *) = nullptr;

    ZSTD_DCtx *zstd_dctx;
};

#endif  // RESOURCE_DECOMPRESSOR_H
