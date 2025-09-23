#ifndef FRAMECOMPRESSOR_H
#define FRAMECOMPRESSOR_H

#include <zstd.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Compression.h"
#include "Compressor.h"
#include "GLES/gl.h"
#include "GLFuncID.h"
#include "LRUCache.h"

#define LRU_CACHE_SIZE 1000
#define LRU_CACHE_THRESHOLD 100000
#define FPZIP_PREC 16

enum class CompressMode {
    ZLIB = 0,
    ZSTD,
};

class GLFrameData;

class ResourceCompressor : public Compressor {
   public:
    int compress(char *buf, int buf_start, int buf_end, int data_len) override;

    /**
     * Compress the resource data of the call, mainly for vertex and texture
     * data
     * @param call the call to compress
     * @param out_buf the buffer to store the compressed data
     * @param out_buf_len out_buf's length
     * @return compressed data's length
     */
    int compress(void *context, uint32_t call_index, CompressGLCall *call, int res_index,
                 char *out_buf, size_t out_buf_len);

    /**
     * Calculate the length of the buffer needed to store the compressed data
     * @param data_len the length of the original data
     * @return the length of the buffer needed to store the compressed data
     */
    int64_t compress_bound(CompressGLCall *call, int res_index);
    
    ResourceCompressor();
    ~ResourceCompressor() override;

   private:
    static int compress_texture(CompressGLCall *call, int res_index,
                                char *out_buf, size_t out_buf_len);

    int compress_vertex(void *context, uint32_t call_index, CompressGLCall *call, int res_index,
                        char *out_buf, size_t out_buf_len);

    char *bitwise_xor(char *a, char *b, uint64_t a_len, uint64_t b_len);

    int gzip_compress(unsigned char *idata, uint64_t ilen, unsigned char *odata,
                      uint64_t *olen);
    int zstd_compress(unsigned char *idata, uint64_t ilen, unsigned char *odata,
                      uint64_t *olen);
    int fpzip_compress(unsigned char *idata, uint32_t nx, uint32_t ny, unsigned char *odata,
                       uint64_t *olen);
    int fp16_compress(unsigned char *idata, uint64_t ilen, unsigned char *odata, uint64_t *olen);

    const std::unordered_set<int> vertex_calls{
        FUNID_glBufferData_custom, FUNID_glBufferSubData_custom,
        FUNID_glFlushMappedBufferRange_special};
    const std::unordered_set<int> texture_calls{
        FUNID_glTexImage2D_without_bound, FUNID_glTexImage2D_with_bound,
        FUNID_glTexSubImage2D_without_bound, FUNID_glTexSubImage2D_with_bound};
    std::unordered_set<int> vertex_texture_calls;

    std::unordered_map<GLuint, std::string> prev_buf_map;
    std::unique_ptr<LRUCache<uint64_t, bool>> vertex_cache;

    char *xor_buf = nullptr;
    int xor_buf_len = 0;
    std::string quantization_buf;

    CompressMode compress_mode = CompressMode::ZSTD;
    int (ResourceCompressor::*compress_func)(unsigned char *, uint64_t,
                                             unsigned char *,
                                             uint64_t *) = nullptr;

    ZSTD_CCtx *zstd_cctx;
};

#endif  // FRAMECOMPRESSOR_H