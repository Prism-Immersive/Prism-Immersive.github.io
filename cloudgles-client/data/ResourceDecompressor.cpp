#define LOG_TAG "ResourceDecompressor"
#define QOI_IMPLEMENTATION
#define USE_FP16 0
#define FP16_PSIMD

#include "ResourceDecompressor.h"

#include <fp16.h>
#include <fp16/psimd.h>
#include <fpzip.h>
#include <zlib.h>

#include <cassert>

#include "EglFuncID.h"
#include "GLFuncID.h"
#include "Log.h"
#include "QOI.hpp"
#include "Utils.h"
#include "renderer/RenderThread.h"

int ResourceDecompressor::decompress(char *buf, int buf_start, int buf_end,
                                     int data_len) {
    return 0;
}

int ResourceDecompressor::decompress(CompressGLCall *call, int res_index,
                                     char *out_buf, size_t out_buf_len) {
    auto vertex_call = vertex_calls.find(call->call_id);
    auto texture_call = texture_calls.find(call->call_id);
    if (vertex_call != vertex_calls.end())
        return decompress_vertex(call, res_index, out_buf, out_buf_len);
    else if (texture_call != texture_calls.end())
        return decompress_texture(call, res_index, out_buf, out_buf_len);
    else {
        return -1;
    }
}

int ResourceDecompressor::decompress_texture(CompressGLCall *call,
                                             int res_index, char *out_buf,
                                             size_t out_buf_len) {
    auto *head =
        reinterpret_cast<CompressHead *>(call->paras[res_index].para_ptr);
    if (head->texture_head.compressed == 1) {
        if (call->call_id == FUNID_glTexImage2D_without_bound) {
            int px_len = head->texture_head.raw_len;
            if (px_len > out_buf_len) {
                ALOGE("not enough buffer space for texture decompression");
                return px_len;
            }
            qoi_desc desc;
            qoi_decode(
                reinterpret_cast<const void *>(call->paras[res_index].para_ptr +
                                               sizeof(CompressHead)),
                call->paras[res_index].para_size - sizeof(CompressHead), &desc,
                0, reinterpret_cast<unsigned char *>(out_buf));
            call->paras[res_index].para_ptr =
                reinterpret_cast<uint64_t>(out_buf);
            return head->texture_head.raw_len;
        } else
            return head->texture_head.raw_len;
    } else {
        call->paras[res_index].para_ptr =
            call->paras[res_index].para_ptr + sizeof(CompressHead);
        return head->texture_head.raw_len;
    }
}

int ResourceDecompressor::decompress_vertex(CompressGLCall *call, int res_index,
                                            char *out_buf, size_t out_buf_len) {
    // Vertex buffer information
    Tracer tracer("Vertex decompress", "decompress");
    tracer.begin_trace();
    Tracer quantization_tracer("Quantization decompress", "decompress");
    Tracer zstd_tracer("Zstd decompress", "decompress");

    uint64_t offset = 0;
    uint64_t original_data_size = 0;
    auto *common_para_ptr =
        (unsigned char *)&call->paras[call->resource_para_num];
    if (call->call_id == FUNID_glBufferData_custom) {
        original_data_size = *reinterpret_cast<uint64_t *>(common_para_ptr + 4);
    } else if (call->call_id == FUNID_glBufferSubData_custom ||
               call->call_id == FUNID_glFlushMappedBufferRange_special) {
        offset = *reinterpret_cast<GLintptr *>(common_para_ptr + 4);
        original_data_size =
            *reinterpret_cast<GLintptr *>(common_para_ptr + 12);
    } else {
        return call->paras[res_index].para_size;
    }
    assert(out_buf_len >= original_data_size);

    // Compress head
    auto *in_buf =
        reinterpret_cast<unsigned char *>(call->paras[res_index].para_ptr);
    size_t in_buf_len = call->paras[res_index].para_size;
    auto head = (CompressHead *)in_buf;
    in_buf += sizeof(CompressHead);
    in_buf_len -= sizeof(CompressHead);

    // Set call para's in_buf
    call->paras[res_index].para_ptr = reinterpret_cast<uint64_t>(out_buf);

    // If it's an LRU cache hit
    uint64_t decompress_out_len = out_buf_len;
    uint64_t server_hash = 0;
    bool has_cache = false;
    if (head->vertex_head.compress_mode == VERTEX_LRU_HIT) {
        uint64_t hash_key = *(reinterpret_cast<uint64_t *>(in_buf));
        boost::optional<std::string> result = vertex_cache->get(hash_key);
        assert(result != boost::none);
        memcpy(out_buf, result->data(), result->size());
        decompress_out_len = result->size();
        has_cache = true;
    } else {
        server_hash = *((uint64_t *)in_buf);
        in_buf += sizeof(uint64_t);
        in_buf_len -= sizeof(uint64_t);
    }

    if (!has_cache) {
        int err;
        if (head->vertex_head.compress_mode != VERTEX_QUANTIZATION_ENTROPY) {
            zstd_tracer.begin_trace();
            err = (this->*decompress_func)(
                in_buf, in_buf_len, reinterpret_cast<unsigned char *>(out_buf),
                &decompress_out_len);
            zstd_tracer.end_trace();
        } else {
            decompress_out_len += 1024;
            if (quantization_buf.size() < decompress_out_len) {
                quantization_buf.resize(ALIGN(decompress_out_len));
            }

            zstd_tracer.begin_trace();
            err = (this->*decompress_func)(
                in_buf, in_buf_len,
                reinterpret_cast<unsigned char *>(&quantization_buf[0]),
                &decompress_out_len);
            zstd_tracer.end_trace();
#if USE_FP16
            quantization_tracer.begin_trace();
            err += fp16_decompress(
                reinterpret_cast<unsigned char *>(&quantization_buf[0]),
                decompress_out_len, reinterpret_cast<unsigned char *>(out_buf),
                &decompress_out_len);
            quantization_tracer.end_trace();
#else
            decompress_out_len = original_data_size;
            err += fpzip_decompress(
                reinterpret_cast<unsigned char *>(&quantization_buf[0]),
                reinterpret_cast<unsigned char *>(out_buf),
                &decompress_out_len);
#endif
        }
        assert(err == 0);
        assert(out_buf_len >= decompress_out_len);
        assert(original_data_size == decompress_out_len);
    }

    if (prev_buf_map.find(head->vertex_head.buffer_id) != prev_buf_map.end()) {
        ResourceBuffer *resource_buffer =
            prev_buf_map[head->vertex_head.buffer_id];
        uint64_t prev_len = resource_buffer->len;

        if (!has_cache && head->vertex_head.compress_mode == VERTEX_XOR_ENTROPY)
            bitwise_xor(resource_buffer->data + offset, out_buf,
                        resource_buffer->len - offset, decompress_out_len);

        uint64_t len = offset + decompress_out_len;
        resource_buffer->len =
            len > resource_buffer->len ? len : resource_buffer->len;
        ;
        if (prev_len < resource_buffer->len) {
            resource_buffer->data = reinterpret_cast<char *>(
                realloc(resource_buffer->data, resource_buffer->len));
        }
        memcpy(resource_buffer->data + offset, out_buf, decompress_out_len);
    } else {
        auto *resource_buffer = new ResourceBuffer();
        resource_buffer->len = decompress_out_len;
        resource_buffer->data =
            reinterpret_cast<char *>(calloc(decompress_out_len, sizeof(char)));
        memcpy(resource_buffer->data, out_buf, decompress_out_len);
        prev_buf_map[head->vertex_head.buffer_id] = resource_buffer;
    }

    // We should cache the data
    if (!has_cache && decompress_out_len > LRU_CACHE_THRESHOLD) {
        std::string vertex_data;
        vertex_data.resize(decompress_out_len);
        memcpy((char *)vertex_data.data(), out_buf, decompress_out_len);
        assert(server_hash != UINT64_MAX);
        vertex_cache->insert(server_hash, std::move(vertex_data));
    }

    tracer.end_trace();
    return static_cast<int>(decompress_out_len);
}

int ResourceDecompressor::decompress_bound(CompressGLCall *call, int res_index,
                                           uint64_t res_addr_offset) {
    if (vertex_texture_calls.find(call->call_id) == vertex_texture_calls.end())
        return -1;

    auto texture_call = texture_calls.find(call->call_id);
    if (texture_call != texture_calls.end()) {  // texture
        return static_cast<int>(
            (*reinterpret_cast<unsigned int *>(call->paras[res_index].para_ptr +
                                               res_addr_offset)) >>
            1);  // head.raw_len;
    } else if (call->call_id == FUNID_glBufferData_custom) {
        return static_cast<int>(*reinterpret_cast<uint64_t *>(
            (reinterpret_cast<char *>(&call->paras[call->resource_para_num]) +
             4)));
    } else if (call->call_id == FUNID_glBufferSubData_custom ||
               call->call_id == FUNID_glFlushMappedBufferRange_special) {
        return static_cast<int>(*reinterpret_cast<uint64_t *>(
            (reinterpret_cast<char *>(&call->paras[call->resource_para_num]) +
             12)));
    }
    return call->paras[res_index].para_size;
}

ResourceDecompressor::ResourceDecompressor() {
    vertex_texture_calls.insert(vertex_calls.begin(), vertex_calls.end());
    vertex_texture_calls.insert(texture_calls.begin(), vertex_calls.end());
    vertex_cache =
        std::make_unique<LRUCache<uint64_t, std::string>>(LRU_CACHE_SIZE);

    switch (compress_mode) {
        case CompressMode::ZLIB:
            decompress_func = &ResourceDecompressor::gzip_decompress;
            break;
        case CompressMode::ZSTD:
            decompress_func = &ResourceDecompressor::zstd_decompress;
            break;
        default:
            ALOGE("Unknown compress mode");
            break;
    }

    zstd_dctx = ZSTD_createDCtx();
}

ResourceDecompressor::~ResourceDecompressor() {
    for (auto const &pair : prev_buf_map) free(pair.second->data);
    if (zstd_dctx) ZSTD_freeDCtx(zstd_dctx);
}

int ResourceDecompressor::gzip_decompress(unsigned char *idata, uint64_t ilen,
                                          unsigned char *odata,
                                          uint64_t *olen) {
    if (ilen == 0) {
        *olen = 0;
        return 0;
    }

    z_stream z = {nullptr};

    z.next_in = idata;
    z.avail_in = ilen;
    z.next_out = odata;
    z.avail_out = *olen;

    if (inflateInit(&z) != Z_OK) {  // gzip -1
        ALOGE("inflateInit failed!\n");
        return -1;
    }

    //    if (inflate(&z, Z_NO_FLUSH) != Z_OK) {
    //        ALOGE("inflate Z_NO_FLUSH failed!\n");
    //        return -1;
    //    }

    if (inflate(&z, Z_FINISH) != Z_STREAM_END) {
        ALOGE("inflate Z_FINISH failed!\n");
        return -1;
    }

    if (inflateEnd(&z) != Z_OK) {
        ALOGE("inflateEnd failed!\n");
        return -1;
    }

    *olen = *olen - z.avail_out;

    return 0;
}

int ResourceDecompressor::zstd_decompress(unsigned char *idata, uint64_t ilen,
                                          unsigned char *odata,
                                          uint64_t *olen) {
    if (ilen == 0) {
        *olen = 0;
        return 0;
    }

    size_t size = ZSTD_decompressDCtx(zstd_dctx, odata, *olen, idata, ilen);
    if (ZSTD_isError(size)) {
        ALOGE("zstd decompress failed: %s\n", ZSTD_getErrorName(size));
        return -1;
    }
    *olen = size;

    return 0;
}

int ResourceDecompressor::fpzip_decompress(unsigned char *idata,
                                           unsigned char *odata,
                                           uint64_t *olen) {
    FPZ *fpz = fpzip_read_from_buffer(idata);
    if (!fpz) {
        ALOGE("FPZIP cannot read header: %s", fpzip_errstr[fpzip_errno]);
        return -1;
    }

    size_t size;
    /* read header */
    if (!fpzip_read_header(fpz)) {
        ALOGE("FPZIP cannot read header: %s", fpzip_errstr[fpzip_errno]);
        return -1;
    }
    if ((fpz->type == FPZIP_TYPE_FLOAT ? sizeof(float) : sizeof(double)) *
            fpz->nx * fpz->ny * fpz->nz * fpz->nf !=
        *olen) {
        ALOGE("FPZIP decompression size mismatch");
        return -1;
    }
    /* perform actual decompression */
    size = fpzip_read(fpz, odata);
    if (!size) {
        ALOGE("FPZIP decompression failed: %s", fpzip_errstr[fpzip_errno]);
        return -1;
    }

    fpzip_read_close(fpz);
    return 0;
}

int ResourceDecompressor::fp16_decompress(unsigned char *idata, uint64_t ilen,
                                          unsigned char *odata,
                                          uint64_t *olen) {
    if (ilen == 0) {
        *olen = 0;
        return 0;
    }

    if (ilen % sizeof(uint16_t) != 0) {
        ALOGE("Invalid input data length for fp16 decompression");
        return -1;
    }

    uint64_t num_elements = ilen / sizeof(uint16_t);
#ifdef FP16_PSIMD
    // Convert psimd_u16 (only use the first 4 uint16_t) to psimd_f32 (4 float)
    uint64_t num_psimd_u16 = num_elements / 4;
    uint64_t num_residues = num_elements % 4;
    auto *fdata = reinterpret_cast<psimd_f32 *>(odata);
    for (uint64_t i = 0; i < num_psimd_u16; i++) {
        fdata[i] = fp16_ieee_to_fp32_psimd(*reinterpret_cast<psimd_u16 *>(idata + i * 4 * sizeof(uint16_t)));
    }
    if (num_residues > 0) {
        uint16_t *residue_hdata = reinterpret_cast<uint16_t *>(idata + num_psimd_u16 * 4 * sizeof(uint16_t));
        float *residue_fdata = reinterpret_cast<float *>(fdata + num_psimd_u16);
        for (uint64_t i = 0; i < num_residues; i++) {
            residue_fdata[i] = fp16_ieee_to_fp32_value(residue_hdata[i]);
        }
    }
#else
    uint16_t *hdata = reinterpret_cast<uint16_t *>(idata);
    float *fdata = reinterpret_cast<float *>(odata);
    for (uint64_t i = 0; i < num_elements; i++) {
        fdata[i] = fp16_ieee_to_fp32_value(hdata[i]);
    }
#endif

    *olen = num_elements * sizeof(float);

    return 0;
}

void ResourceDecompressor::bitwise_xor(const char *a, char *output,
                                       uint64_t a_len, uint64_t out_len) {
    uint64_t short_len = a_len < out_len ? a_len : out_len;

    for (int i = 0; i < short_len; i++) {
        output[i] = static_cast<char>(a[i] ^ output[i]);
    }
}
