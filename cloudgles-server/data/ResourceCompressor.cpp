#define LOG_TAG "ResourceCompressor"
#define USE_FP16 0
#include "ResourceCompressor.h"

#include <fp16.h>
#include <fpzip.h>
#include <xxhash.h>
#include <zlib.h>

#include <cstddef>
#include <cstdint>
#include <vector>

#include "Compression.h"
#include "GLES/gl.h"
#include "GLES3/gl3.h"
#include "GLFuncID.h"
#include "Log.h"
#include "QOI.hpp"
#include "ThreadContext.h"
#include "network/FrameCallStreamer.h"
#include "opengl/GLUtils.h"
#include "opengl/Memory.h"
#include "opengl/State.h"
#include "opengl/Vertex.h"

char *ResourceCompressor::bitwise_xor(char *a, char *b, uint64_t a_len,
                                      uint64_t b_len) {
    ATRACE_CALL();
    uint64_t len = std::max(a_len, b_len);
    uint64_t short_len = std::min(a_len, b_len);

    if (len > xor_buf_len) {
        auto tmp = static_cast<char *>(realloc(xor_buf, len));
        if (likely(tmp)) {
            xor_buf = tmp;
            xor_buf_len = len;
        } else {
            ALOGE("server failed to realloc xor_buf");
        }
    }

    for (int i = 0; i < short_len; i++)
        xor_buf[i] = static_cast<char>(a[i] ^ b[i]);
    for (int i = short_len; i < len; i++) {
        if (short_len == a_len)
            xor_buf[i] = b[i];
        else
            xor_buf[i] = a[i];
    }
    return xor_buf;
}

int ResourceCompressor::gzip_compress(unsigned char *idata, uint64_t ilen,
                                      unsigned char *odata, uint64_t *olen) {
    ATRACE_CALL();
    if (ilen == 0) {
        *olen = 0;
        return 0;
    }

    z_stream z = {};

    z.next_in = idata;
    z.avail_in = ilen;
    z.next_out = odata;
    z.avail_out = *olen;

    if (deflateInit(&z, Z_BEST_SPEED) != Z_OK) {  // gzip -1
        ALOGE("deflateInit failed!\n");
        return -1;
    }

    if (deflate(&z, Z_NO_FLUSH) != Z_OK) {
        ALOGE("deflate Z_NO_FLUSH failed!\n");
        return -1;
    }

    if (deflate(&z, Z_FINISH) != Z_STREAM_END) {
        ALOGE("deflate Z_FINISH failed!\n");
        return -1;
    }

    if (deflateEnd(&z) != Z_OK) {
        ALOGE("deflateEnd failed!\n");
        return -1;
    }

    *olen = *olen - z.avail_out;

    return 0;
}

int ResourceCompressor::zstd_compress(unsigned char *idata, uint64_t ilen,
                                      unsigned char *odata, uint64_t *olen) {
    if (ilen == 0) {
        *olen = 0;
        return 0;
    }

    size_t size = ZSTD_compress2(zstd_cctx, odata, *olen, idata, ilen);
    if (ZSTD_isError(size)) {
        ALOGE("ZSTD_compressCCtx failed: %s\n", ZSTD_getErrorName(size));
        return -1;
    }
    *olen = size;

    return 0;
}

int ResourceCompressor::fpzip_compress(unsigned char *idata, uint32_t nx,
                                       uint32_t ny, unsigned char *odata,
                                       uint64_t *olen) {
    FPZ *fpz = fpzip_write_to_buffer(odata, *olen);
    fpz->type = FPZIP_TYPE_FLOAT;
    fpz->prec = FPZIP_PREC;
    fpz->nx = nx;
    fpz->ny = ny;
    fpz->nz = 1;
    fpz->nf = 1;

    size_t size;
    /* write header */
    if (!fpzip_write_header(fpz)) {
        ALOGE("FPZIP cannot write header: %s", fpzip_errstr[fpzip_errno]);
        return 0;
    }
    /* perform actual compression */
    size = fpzip_write(fpz, idata);
    if (!size) {
        ALOGE("FPZIP compression failed: %s", fpzip_errstr[fpzip_errno]);
        return -1;
    }
    *olen = size;

    fpzip_write_close(fpz);
    return 0;
}

int ResourceCompressor::fp16_compress(unsigned char *idata, uint64_t ilen,
                                      unsigned char *odata, uint64_t *olen) {
    if (ilen == 0) {
        *olen = 0;
        return 0;
    }

    if (ilen % sizeof(float) != 0) {
        return -1;
    }

    uint64_t num_elements = ilen / sizeof(float);
    auto *fdata = reinterpret_cast<float *>(idata);
    auto *hdata = reinterpret_cast<uint16_t *>(odata);

    for (uint64_t i = 0; i < num_elements; i++) {
        hdata[i] = fp16_ieee_from_fp32_value(fdata[i]);
    }
    
    *olen = num_elements * sizeof(uint16_t);

    return 0;
}

int ResourceCompressor::compress(char *buf, int buf_start, int buf_end,
                                 int data_len) {
    return 0;
}

int ResourceCompressor::compress(void *context, uint32_t call_index,
                                 CompressGLCall *call, int res_index,
                                 char *out_buf, size_t out_buf_len) {
    auto vertex_call = vertex_calls.find(call->call_id);
    auto texture_call = texture_calls.find(call->call_id);
    if (vertex_call != vertex_calls.end()) {  // vertex call
        return compress_vertex(context, call_index, call, res_index, out_buf,
                               out_buf_len);
    } else if (texture_call != texture_calls.end())  // texture call
        return compress_texture(call, res_index, out_buf, out_buf_len);
    else {
        memcpy(out_buf, (char *)call->paras[res_index].para_ptr,
               call->paras[res_index].para_size);
        return call->paras[res_index].para_size;
    }
}

int ResourceCompressor::compress_texture(CompressGLCall *call, int res_index,
                                         char *out_buf, size_t out_buf_len) {
    bool compressed = true;
    CompressHead head;
    if (call->call_id == FUNID_glTexImage2D_without_bound) {
        char *_ptr = ((char *)call->paras) +
                     call->resource_para_num * sizeof(CompressParaElem);
        int target = *(int *)(_ptr);
        if (target != GL_TEXTURE_2D) {
            compressed = false;
        }
        int width = *(int *)(_ptr + 12);
        int height = *(int *)(_ptr + 16);
        int border = *(int *)(_ptr + 20);
        int format = *(int *)(_ptr + 24);
        int type = *(int *)(_ptr + 28);
        //        if (width < 256) {
        //            compressed = false;
        //        }
        if (type != GL_UNSIGNED_BYTE) {
            compressed = false;
        }
        unsigned char channels = 0;
        if (format == GL_RGB) {
            channels = 3;
        } else if (format == GL_RGBA) {
            channels = 4;
        } else {
            compressed = false;
        }
        if (call->resource_para_num != 1) {
            compressed = false;
        }

        if (width * channels % 4 != 0) {  // do not support unaligned texture
            compressed = false;
        }
        if (compressed) {
            head.texture_head.compressed = 1;
            head.texture_head.raw_len =
                call->paras[res_index]
                    .para_size;  // equal to width * height * channels
            memcpy(out_buf, &head, sizeof(head));
            qoi_desc desc = {
                .width = (unsigned int)width,
                .height = (unsigned int)height,
                .channels = channels,
                .colorspace = QOI_SRGB  // purely informative
            };
            int len;
            qoi_encode((void *)call->paras[res_index].para_ptr, &desc, &len,
                       (unsigned char *)(out_buf + sizeof(head)));
            return (int)sizeof(head) + len;
        }
    }
    head.texture_head.compressed = 0;
    head.texture_head.raw_len = call->paras[res_index].para_size;
    memcpy(out_buf, &head, sizeof(head));
    memcpy(out_buf + sizeof(head), (char *)call->paras[res_index].para_ptr,
           call->paras[res_index].para_size);
    return (int)sizeof(head) + call->paras[res_index].para_size;
}

int ResourceCompressor::compress_vertex(void *context, uint32_t call_index,
                                        CompressGLCall *call, int res_index,
                                        char *out_buf, size_t out_buf_len) {
    // Vertex buffer infomation: target, offset, buffer_id
    GLenum target = 0;
    GLintptr offset = 0;
    auto *common_para_ptr =
        (unsigned char *)&call->paras[call->resource_para_num];
    target = *(GLenum *)common_para_ptr;
    if (call->call_id == FUNID_glBufferSubData_custom ||
        call->call_id == FUNID_glFlushMappedBufferRange_special) {
        offset = *(GLintptr *)(common_para_ptr + 4);
    }
    GLuint buffer_binding =
        get_bound_buffer(((ThreadContext *)context)->opengl_context, target);
    if (buffer_binding == 0)
        ALOGE("COMPRESS VERTEX ERR: call_id=%d, no binding buffers",
              call->call_id);

    // Compress head
    auto head = (CompressHead *)out_buf;
    out_buf += sizeof(CompressHead);
    head->vertex_head.buffer_id = buffer_binding;

    auto update_buf = (char *)call->paras[res_index].para_ptr;
    int update_buf_len = call->paras[res_index].para_size;
    char *in_buf = nullptr;

    // Try LRU cache first
    // If the buffer is big enough, calculate the hash and put it into the
    // cache Also, put the hash into the out_buf
    bool has_cache = false;
    if (update_buf_len > LRU_CACHE_THRESHOLD) {
        uint64_t hash = XXH3_64bits((void *)update_buf, update_buf_len);
        if (vertex_cache->get(hash) != boost::none) {
            has_cache = true;
            *(reinterpret_cast<uint64_t *>(out_buf)) = hash;
            head->vertex_head.compress_mode = VERTEX_LRU_HIT;
        } else {
            vertex_cache->insert(hash, true);
            *(reinterpret_cast<uint64_t *>(out_buf)) = hash;
        }
    } else {
        *(reinterpret_cast<uint64_t *>(out_buf)) = UINT64_MAX;
    }
    out_buf += sizeof(uint64_t);

    bool use_quantization = false;
    VertexMemLayout *vertex_layout = nullptr;
    if (!has_cache) {
        vertex_layout = VertexMemLayout::get_vertex_mem_layout_with_id(
            ((ThreadContext *)context)->opengl_context, buffer_binding);

        if (vertex_layout->fully_parsed &&
            vertex_layout->stride_layouts.size() == 1) {
            bool full_float = true;
            bool contiguous = false;
            bool correct = false;
            size_t actual_layout_size = 0;
            for (Layout &layout : vertex_layout->stride_layouts[0].layouts) {
                if (layout.type != GL_FLOAT) {
                    full_float = false;
                }
                actual_layout_size += layout.size * gl_sizeof(layout.type);
            }
            if (actual_layout_size ==
                vertex_layout->stride_layouts[0].single_stride_len) {
                contiguous = true;
            }
            correct = update_buf_len %
                          vertex_layout->stride_layouts[0].single_stride_len ==
                      0;
            // Only use FPZIP when we're 100% sure that the data is full
            // float and contiguous
            if (full_float && contiguous && correct) {
                use_quantization = true;
            }
        }
    }

    // If there's a cache miss and the buffer is not suited for FPZIP, do
    // xor But always save the data diff no matter if there's a cache hit
    if (!has_cache && !use_quantization) {
        head->vertex_head.compress_mode = VERTEX_XOR_ENTROPY;
    } else if (!has_cache && use_quantization) {
        head->vertex_head.compress_mode = VERTEX_QUANTIZATION_ENTROPY;
    }
    if (prev_buf_map.find(buffer_binding) != prev_buf_map.end()) {
        std::string *resource_buffer = &prev_buf_map[buffer_binding];
        char *prev_buf = resource_buffer->data();
        size_t prev_buf_len = resource_buffer->size();

        if (!has_cache && !use_quantization) {
            bitwise_xor(prev_buf + offset, update_buf, prev_buf_len - offset,
                        update_buf_len);
            in_buf = xor_buf;
        }

        // update unordered_map
        if (offset + update_buf_len > resource_buffer->size()) {
            resource_buffer->resize(offset + update_buf_len);
        }
        memcpy(resource_buffer->data() + offset, update_buf, update_buf_len);
    } else {
        // the buffer is not used before, skip xor
        in_buf = update_buf;
        prev_buf_map[buffer_binding].resize(update_buf_len);
        memcpy(prev_buf_map[buffer_binding].data(), update_buf, update_buf_len);
    }

    // FPZIP compress
    uint64_t in_buf_len = update_buf_len;
    if (!has_cache && use_quantization) {
#if USE_FP16
        in_buf_len = ALIGN(in_buf_len);
        if (quantization_buf.size() < in_buf_len) {
            quantization_buf.resize(in_buf_len);
        }
        int err = fp16_compress((unsigned char *)update_buf, update_buf_len,
                                (unsigned char *)quantization_buf.data(),
                                &in_buf_len);
#else
        in_buf_len = ALIGN(in_buf_len + 1024);
        if (quantization_buf.size() < in_buf_len) {
            quantization_buf.resize(in_buf_len);
        }
        uint32_t nx =
            vertex_layout->stride_layouts[0].single_stride_len / sizeof(float);
        uint32_t ny =
            update_buf_len / vertex_layout->stride_layouts[0].single_stride_len;
        int err = fpzip_compress((unsigned char *)update_buf, nx, ny,
                                 (unsigned char *)quantization_buf.data(),
                                 &in_buf_len);
#endif
        if (err) {
            in_buf = update_buf;
            in_buf_len = update_buf_len;
            head->vertex_head.compress_mode = VERTEX_ENTROPY;
        } else {
            in_buf = quantization_buf.data();
        }
    }

    uint64_t result_len = out_buf_len;
    if (!has_cache) {
        // Compress with entropy encoding
        int err =
            (this->*compress_func)((unsigned char *)in_buf, in_buf_len,
                                   (unsigned char *)(out_buf), &result_len);
        if (err) {
            memcpy(out_buf + sizeof(CompressHead),
                   (char *)call->paras[res_index].para_ptr,
                   update_buf_len < out_buf_len - sizeof(CompressHead)
                       ? update_buf_len
                       : out_buf_len - sizeof(CompressHead));
            ALOGE("GZIP COMPRESS ERR call_id=%d\n", call->call_id);
            return update_buf_len + sizeof(CompressHead) < out_buf_len
                       ? update_buf_len + sizeof(CompressHead)
                       : static_cast<int>(out_buf_len);
        }
        // The hash of the data
        result_len += sizeof(uint64_t);
    } else {
        result_len = sizeof(uint64_t);
    }

    ALOGD(
        "Vertex compress, buffer_id %u, target %x, offset %ld, ori_len=%d, "
        "result_len=%" PRIu64 ", compress ratio=%f%%",
        buffer_binding, target, offset, update_buf_len, result_len,
        (1 - (float)result_len / update_buf_len) * 100);

    return static_cast<int>(result_len + sizeof(CompressHead));
}

int64_t ResourceCompressor::compress_bound(CompressGLCall *call,
                                           int res_index) {
    auto texture_call = texture_calls.find(call->call_id);
    if (texture_call != texture_calls.end()) {  // texture compress bound
        if (call->call_id == FUNID_glTexImage2D_without_bound) {
            return (int)sizeof(CompressHead) +
                   call->paras[res_index].para_size +
                   (QOI_HEADER_SIZE + (int)sizeof(qoi_padding));
        } else {
            return (int)sizeof(CompressHead) + call->paras[res_index].para_size;
        }
    }
    auto vertex_call = vertex_calls.find(call->call_id);
    if (vertex_call != vertex_calls.end()) {  // vertex compress bound
        int64_t len = call->paras[res_index].para_size;
        int64_t compress_len = len;

        switch (compress_mode) {
            case CompressMode::ZLIB:
                compress_len = compressBound(len);
                break;
            case CompressMode::ZSTD:
                compress_len = ZSTD_compressBound(len);
                break;
            default:
                break;
        }
        return compress_len > len + 1024
                   ? compress_len + sizeof(CompressHead) + sizeof(uint64_t)
                   : len + 1024 + sizeof(CompressHead) + sizeof(uint64_t);
    }

    return -1;
}

ResourceCompressor::ResourceCompressor() {
    vertex_texture_calls.insert(vertex_calls.begin(), vertex_calls.end());
    vertex_texture_calls.insert(texture_calls.begin(), vertex_calls.end());
    vertex_cache = std::make_unique<LRUCache<uint64_t, bool>>(LRU_CACHE_SIZE);

    switch (compress_mode) {
        case CompressMode::ZLIB:
            compress_func = &ResourceCompressor::gzip_compress;
            break;
        case CompressMode::ZSTD:
            compress_func = &ResourceCompressor::zstd_compress;
            break;
        default:
            ALOGE("Unknown compress mode");
            break;
    }

    zstd_cctx = ZSTD_createCCtx();
    ZSTD_CCtx_setParameter(zstd_cctx, ZSTD_c_compressionLevel, 1);
    ZSTD_CCtx_setParameter(zstd_cctx, ZSTD_c_checksumFlag, 0);
    ZSTD_CCtx_reset(zstd_cctx, ZSTD_reset_session_only);
}

ResourceCompressor::~ResourceCompressor() {
    if (xor_buf != nullptr) {
        free(xor_buf);
        xor_buf_len = 0;
    }

    if (zstd_cctx != nullptr) {
        ZSTD_freeCCtx(zstd_cctx);
    }
}