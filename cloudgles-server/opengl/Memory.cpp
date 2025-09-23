#define LOG_TAG "GLMemory"
#define SELECTIVE_MEMORY_SAVE
#include "Memory.h"

#include <GLES3/gl32.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <vector>

#include "ErrorHandler.h"
#include "GLDef.h"
#include "GLES/gl.h"
#include "Log.h"
#include "ThreadContext.h"
#include "Utils.h"
#include "opengl/GLUtils.h"
#include "opengl/State.h"
#include "opengl/Vertex.h"

void StrideLayout::reset() {
    start_offset = 0;
    num_strides = 1;
    single_stride_len = 0;
    layouts.clear();
}

uint32_t StrideLayout::get_layout_size() const {
    uint32_t size = 0;
    for (const Layout &layout : layouts) {
        size += layout.size * (uint32_t)gl_sizeof(layout.type);
    }
    return size;
}

void StrideLayout::merge_with(const StrideLayout &other) {
    // Deal with partial overlap of memory regions
    const StrideLayout &left =
        start_offset < other.start_offset ? *this : other;
    const StrideLayout &right =
        start_offset < other.start_offset ? other : *this;
    start_offset = left.start_offset;
    int64_t overlap = left.start_offset +
                      left.single_stride_len * left.num_strides -
                      right.start_offset;
    if (overlap > 0 && left.single_stride_len == right.single_stride_len) {
        num_strides = left.num_strides + right.num_strides -
                      (uint32_t)overlap / left.single_stride_len;
        return;
    }
    // Overwrite case
    if (overlap > 0 && left.single_stride_len != right.single_stride_len) {
        return;
    }
    num_strides += other.num_strides;
}

bool StrideLayout::can_merge_with(const StrideLayout &other) const {
    // Two strides can be merged if and only if they have the same layout and
    // they are adjacent
    const StrideLayout &left =
        start_offset < other.start_offset ? *this : other;
    const StrideLayout &right =
        start_offset < other.start_offset ? other : *this;

    // Left stride overwrites the right stride
    if (left.start_offset + left.single_stride_len * left.num_strides >
        right.start_offset) {
        return true;
    }
    
    if (single_stride_len != other.single_stride_len ||
        layouts.size() != other.layouts.size() ||
        // Allow partial overlap, thus should be < rather than ==
        left.start_offset + left.single_stride_len * left.num_strides <
            right.start_offset) {
        return false;
    }

    for (size_t i = 0; i < layouts.size(); i++) {
        if (layouts[i].intra_stride_offset !=
                other.layouts[i].intra_stride_offset ||
            layouts[i].size != other.layouts[i].size ||
            layouts[i].type != other.layouts[i].type) {
            return false;
        }
    }
    return true;
}

void VertexMemLayout::set_layout(const AttribPoint *attrib_point,
                                 uint32_t index) {
    size_t data_size =
        attrib_point->size[index] * gl_sizeof(attrib_point->type[index]);
    auto data_offset = (size_t)attrib_point->data[index];
    // First check if the layout in attrib_point can be put into an existing
    // stride layout
    for (StrideLayout &stride_layout : stride_layouts) {
        // The vertex attribute indicates that the vertex attrib is inside the
        // stride. Add the layout info to the stride layout if it's not already
        // there.

        // The layout is in the stride if their stride length is the same and
        // their memory regions overlap
        size_t stride_size =
            stride_layout.single_stride_len * stride_layout.num_strides;
        size_t stride_offset = stride_layout.start_offset;
        size_t left_offset =
            stride_offset < data_offset ? stride_offset : data_offset;
        size_t left_stride = stride_offset < data_offset
                                 ? stride_size
                                 : attrib_point->stride[index];
        bool overlap = left_offset + left_stride > stride_offset;

        if (stride_layout.single_stride_len != attrib_point->stride[index] &&
            overlap) {
            stride_layout.reset();
            fully_parsed = false;
            stride_layout.single_stride_len = attrib_point->stride[index];
            stride_layout.start_offset = data_offset;
        }

        if (stride_layout.single_stride_len == attrib_point->stride[index] &&
            overlap) {
            // Need to reset all the layouts' intra_stride_offset after
            // resetting the start_offset
            if (data_offset < stride_offset) {
                auto offset_diff =
                    (uint32_t)(stride_layout.start_offset - data_offset);
                stride_layout.start_offset = data_offset;
                for (size_t i = 0; i < stride_layout.layouts.size(); i++) {
                    stride_layout.layouts[i].intra_stride_offset += offset_diff;
                }
            }

            uint32_t intra_stride_offset =
                ((uint64_t)attrib_point->data[index] -
                 stride_layout.start_offset) %
                stride_layout.single_stride_len;
            uint32_t stride_end = intra_stride_offset + data_size;

            uint32_t cur_stride_offset = 0;
            uint32_t cur_stride_end = 0;
            uint32_t next_stride_offset = 0;
            uint32_t layout_index = 0;
            for (layout_index = 0; layout_index < stride_layout.layouts.size();
                 layout_index++) {
                cur_stride_offset =
                    stride_layout.layouts[layout_index].intra_stride_offset;
                cur_stride_end =
                    cur_stride_offset +
                    stride_layout.layouts[layout_index].size *
                        (uint32_t)gl_sizeof(
                            stride_layout.layouts[layout_index].type);
                if (layout_index + 1 < stride_layout.layouts.size()) {
                    next_stride_offset = stride_layout.layouts[layout_index + 1]
                                             .intra_stride_offset;
                } else {
                    next_stride_offset = stride_layout.single_stride_len;
                }

                // TODO: Even the same buffer may change layouts, need to handle
                // this

                if (intra_stride_offset >= cur_stride_offset &&
                    stride_end <= cur_stride_end) {
                    // The layout is overlapped with a layout in the stride.
                    // Skip the rest.
                    return;
                }

                if (intra_stride_offset >= cur_stride_end &&
                    stride_end <= next_stride_offset) {
                    layout_index++;
                    break;
                }
            }

            // Add the layout to the stride layout
            Layout layout{.intra_stride_offset = intra_stride_offset,
                          .size = attrib_point->size[index],
                          .type = attrib_point->type[index]};
            stride_layout.layouts.insert(
                stride_layout.layouts.begin() + layout_index, layout);
            return;
        }
    }

    // Otherwise, create a new stride layout
    StrideLayout stride_layout;
    stride_layout.start_offset = data_offset;
    stride_layout.single_stride_len = attrib_point->stride[index];
    stride_layout.num_strides = 1;
    stride_layouts.push_back(stride_layout);
    // Recursively call set_layout to add the layout to the newly created stride
    set_layout(attrib_point, index);
}

VertexMemLayout *VertexMemLayout::get_vertex_mem_layout_with_id(void *context,
                                                                GLuint id) {
    BufferMapped *buffer_ptr = ((GLThreadContext *)context)->buffer_ptr;
    VertexMemLayout *vertex_mem_layout = nullptr;

    SPIN_LOCK(buffer_ptr->buffer_maps_lock);
    vertex_mem_layout = &buffer_ptr->buffer_layout_maps[id];
    vertex_mem_layout->id = id;
    SPIN_UNLOCK(buffer_ptr->buffer_maps_lock);

    return vertex_mem_layout;
}

void VertexMemLayout::draw_arrays(void *context, GLsizei count) {
    AttribPoint *attrib_point =
        ((GLThreadContext *)context)
            ->context_binding_status.bound_buffer->attrib_point;
    // For all enabled vertex attributes, find their corresponding stride
    // layout, and set the stride layout's num_strides to count
    std::unordered_set<VertexMemLayout *> drawn_vertex_mem_layouts;
    for (size_t i = 0; i < MAX_VERTEX_ATTRIBS_NUM; i++) {
        if (attrib_point->enabled[i]) {
            VertexMemLayout *vertex_mem_layout = get_vertex_mem_layout_with_id(
                context, attrib_point->array_buffer_binding[i]);
            if (vertex_mem_layout->fully_parsed) continue;

            auto data_offset = (uint64_t)attrib_point->data[i];
            uint64_t data_size =
                attrib_point->size[i] * gl_sizeof(attrib_point->type[i]);

            drawn_vertex_mem_layouts.insert(vertex_mem_layout);
            std::vector<StrideLayout> &stride_layouts =
                vertex_mem_layout->stride_layouts;

            for (size_t i = 0; i < stride_layouts.size(); i++) {
                if (stride_layouts[i].start_offset <= data_offset &&
                    data_offset + data_size <=
                        stride_layouts[i].start_offset +
                            stride_layouts[i].single_stride_len *
                                stride_layouts[i].num_strides) {
                    uint32_t num_strides_before_offset =
                        (uint32_t)(data_offset -
                                   stride_layouts[i].start_offset) /
                        stride_layouts[i].single_stride_len;
                    stride_layouts[i].num_strides =
                        std::max(stride_layouts[i].num_strides,
                                 (uint32_t)count + num_strides_before_offset);
                    break;
                }
            }
        }
    }

    // For all vertex mem that are drawn, check if their stride layouts can be
    // merged with adjacent ones
    for (VertexMemLayout *vertex_mem_layout : drawn_vertex_mem_layouts) {
        std::vector<StrideLayout> &stride_layouts =
            vertex_mem_layout->stride_layouts;
        uint32_t total_len = 0;
        for (auto it = stride_layouts.begin(); it != stride_layouts.end();) {
            auto next_it = it + 1;
            while (next_it != stride_layouts.end()) {
                if (it->can_merge_with(*next_it)) {
                    it->merge_with(*next_it);
                    next_it = stride_layouts.erase(next_it);
                } else {
                    next_it++;
                }
            }
            // No need to add start_offset which is already included in
            // single_stride_len
            total_len += it->single_stride_len * it->num_strides;
            it++;
        }
        MapResult *map_res =
            get_map_result_with_id(context, vertex_mem_layout->id);
        if (map_res != nullptr && (total_len == map_res->total_len || total_len == map_res->length) ) {
            vertex_mem_layout->fully_parsed = true;
        }
        if (map_res != nullptr && total_len > map_res->total_len) {
            ALOGE(
                "Error! Total length %d is larger than total len %ld in "
                "map_res",
                total_len, map_res->total_len);
        }
    }
}

void VertexMemLayout::draw_elements(void *context, GLsizei count, GLenum type,
                                    const void *elements) {
    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    GLuint element_array_buffer_binding =
        bound_buffer->buffer_status->element_array_buffer;
    if (element_array_buffer_binding == 0) {
        return;
    }
    if (bound_buffer->ebo_buffer.find(element_array_buffer_binding) == bound_buffer->ebo_buffer.end()) {
        return;
    }
    const char *ebo_data =
        bound_buffer->ebo_buffer[element_array_buffer_binding]->data;
    if (ebo_data == nullptr) {
        return;
    }
    ebo_data += (uint64_t)elements;
    uint32_t max_index = 0;
    switch (type) {
        case GL_UNSIGNED_BYTE: {
            auto *data = reinterpret_cast<const uint8_t *>(ebo_data);
            max_index = *std::max_element(data, data + count);
        } break;
        case GL_UNSIGNED_SHORT: {
            auto *data = reinterpret_cast<const uint16_t *>(ebo_data);
            max_index = *std::max_element(data, data + count);
        } break;
        case GL_UNSIGNED_INT: {
            auto *data = reinterpret_cast<const uint32_t *>(ebo_data);
            max_index = *std::max_element(data, data + count);
        } break;
        default:
            ALOGE("VertexMemLayout: Error! Invalid type %x in draw_elements",
                  type);
            break;
    }

    // Index starts from 0, should +1 to get the max number
    draw_arrays(context, max_index + 1);
}

MapResult *get_map_result_with_target(void *context, GLenum target) {
    GLuint id;
    id = get_bound_buffer(context, target);

    return get_map_result_with_id(context, id);
}

MapResult *get_map_result_with_id(void *context, GLuint id) {
    BufferMapped *buffer_ptr = ((GLThreadContext *)context)->buffer_ptr;
    MapResult *map_res = nullptr;

    SPIN_LOCK(buffer_ptr->buffer_maps_lock);
    if (buffer_ptr->buffer_maps.find(id) != buffer_ptr->buffer_maps.end()) {
        map_res = buffer_ptr->buffer_maps[id];
    } else {
        map_res = nullptr;
    }
    SPIN_UNLOCK(buffer_ptr->buffer_maps_lock);

    return map_res;
}

void set_map_result_with_target(void *context, GLenum target,
                                MapResult *map_res) {
    GLuint id;
    id = get_bound_buffer(context, target);

    set_map_result_with_id(context, id, map_res);
}

void set_map_result_with_id(void *context, GLuint id, MapResult *map_res) {
    BufferMapped *buffer_ptr = ((GLThreadContext *)context)->buffer_ptr;

    SPIN_LOCK(buffer_ptr->buffer_maps_lock);
    if (map_res == nullptr) {
        buffer_ptr->buffer_maps.erase(id);
    } else {
        buffer_ptr->buffer_maps[id] = map_res;
    }
    SPIN_UNLOCK(buffer_ptr->buffer_maps_lock);
}

void *d_glMapBufferRange(void *context, GLenum target, GLintptr offset,
                         GLsizeiptr length, GLbitfield access) {
    if (offset < 0 || length < 0) {
        set_gl_error(context, GL_INVALID_VALUE);
        return nullptr;
    }
    if (!(access & GL_MAP_UNSYNCHRONIZED_BIT) && !(access & GL_MAP_READ_BIT) &&
        !(access & GL_MAP_WRITE_BIT) &&
        !(access & GL_MAP_INVALIDATE_RANGE_BIT) &&
        !(access & GL_MAP_INVALIDATE_BUFFER_BIT) &&
        !(access & GL_MAP_FLUSH_EXPLICIT_BIT)) {
        set_gl_error(context, GL_INVALID_VALUE);
        return nullptr;
    }
    if ((!(access & GL_MAP_READ_BIT)) && !(access & GL_MAP_WRITE_BIT)) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return nullptr;
    }
    if (access & GL_MAP_READ_BIT && (access & GL_MAP_INVALIDATE_RANGE_BIT ||
                                     access & GL_MAP_INVALIDATE_BUFFER_BIT ||
                                     access & GL_MAP_UNSYNCHRONIZED_BIT)) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return nullptr;
    }
    if (access & GL_MAP_FLUSH_EXPLICIT_BIT && !(access & GL_MAP_WRITE_BIT)) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return nullptr;
    }

    ALOGD("mapbufferrange target %x id %u offset %d length %d access %x\n",
          (int)target, get_bound_buffer(context, target), (int)offset,
          (int)length, (int)access);

    MapResult *map_res = get_map_result_with_target(context, target);

    char *map_data = nullptr;

    if (map_res == nullptr) {
        set_gl_error(context, GL_INVALID_VALUE);

        ALOGE(
            "error! cannot find map_res target %x id %u offset %lld length "
            "%lld access %x",
            target, get_bound_buffer(context, target), (long long)offset,
            (long long)length, access);
        return nullptr;
    }

    map_res->map_cnt += 1;

    if (map_res->map_data != nullptr) {
        ALOGE("Buffer with target %x is already mapped", target);
        set_gl_error(context, GL_INVALID_OPERATION);
        return nullptr;
    }

    if (map_res->dynamic_data == nullptr && map_res->map_cnt > 1) {
        if (access & GL_MAP_WRITE_BIT) {
            ALOGE(
                "MapBuffer dynamic data is NULL! Target %x usage %x id "
                "%u all_len %lld offset %lld length %lld access %x "
                "map_cnt %d",
                target, map_res->usage, get_bound_buffer(context, target),
                (long long)map_res->total_len, (long long)offset,
                (long long)length, access, map_res->map_cnt);
        }
    } else {
        if (offset + length > map_res->total_len) {
            set_gl_error(context, GL_INVALID_OPERATION);
            return nullptr;
        }

        map_data = map_res->dynamic_data + offset;
    }

    map_res->access = access;
    map_res->length = length;
    map_res->offset = offset;
    map_res->target = target;

    if (map_data == nullptr) {
        map_data = new char[length];
    }

    if (map_data == nullptr) {
        set_gl_error(context, GL_OUT_OF_MEMORY);
        return nullptr;
    }

    map_res->map_data = map_data;

    if (access & GL_MAP_READ_BIT) {
        r_glMapBufferRange_read(context, target, offset, length, access,
                                map_data);
    } else {
        r_glMapBufferRange_write(context, target, offset, length, access);
    }

    return map_data;
}

GLboolean d_glUnmapBuffer(void *context, GLenum target) {
    ALOGD("unmap buffer %x\n", (int)target);

    MapResult *map_res = get_map_result_with_target(context, target);
    if (map_res == nullptr || map_res->map_data == nullptr) {
        ALOGE("unmap buffer error target %x map_res %llx\n", (int)target,
              (unsigned long long)map_res);
        set_gl_error(context, GL_INVALID_OPERATION);
        return GL_FALSE;
    }

    GLboolean ret = GL_TRUE;

    // Note that here we first check the write bit because it's possible to have
    // both read and write bits set, and when the write bit is set, the related
    // memory should be released.

    // Only send the entire buffer when the write bit is set and the flush
    // bit is not set
    if ((map_res->access & GL_MAP_WRITE_BIT) &&
        !(map_res->access & GL_MAP_FLUSH_EXPLICIT_BIT)) {
        ALOGD("UnmapBuffer data size %d", (int)map_res->length);

        ((ThreadContext *)egl_thread_context)
            ->data_tracer.trace_common(
                "glUnmapBuffer target %x id %u offset %lld size %lld", target,
                get_bound_buffer(context, target), (long long)map_res->offset,
                (long long)map_res->length);

        if (map_res->map_data != nullptr) {
            ((ThreadContext *)egl_thread_context)
                ->data_tracer.trace_resource((const char *)map_res->map_data,
                                             map_res->length);
        }

        r_glFlushMappedBufferRange_special(context, target, 0, map_res->length,
                                           (const void *)map_res->map_data);
    }
    r_glUnmapBuffer_special(context, target);

    // Delete the map_data after the buffer is unmapped
    // But we need to save the data if it's an element array buffer
    if (target == GL_ELEMENT_ARRAY_BUFFER &&
        (map_res->access & GL_MAP_WRITE_BIT)) {
        BoundBuffer *bound_buffer =
            ((GLThreadContext *)context)->context_binding_status.bound_buffer;
        BufferStatus *status = bound_buffer->buffer_status;
        if (bound_buffer->ebo_buffer.find(status->element_array_buffer) !=
            bound_buffer->ebo_buffer.end()) {
            ElementArrayBuffer *ebo_data =
                bound_buffer->ebo_buffer[status->element_array_buffer];
            if (ebo_data == nullptr || ebo_data->data == nullptr) {
                ALOGE("Error! Ebo_data is NULL during unmap!");
            } else {
                if (map_res->length + map_res->offset > ebo_data->size) {
                    ALOGE(
                        "Error! Ebo size %lld is too small during unmap! Unmap "
                        "buffer offset %lld len %lld",
                        (long long)ebo_data->size, (long long)map_res->offset,
                        (long long)map_res->length);
                } else {
                    ebo_data->has_update = GL_TRUE;
                }
            }
        }
    }

    if (map_res->dynamic_data == nullptr) {
        delete[] map_res->map_data;
    }
    map_res->map_data = nullptr;

    return ret;
}

void d_glFlushMappedBufferRange(void *context, GLenum target, GLintptr offset,
                                GLsizeiptr length) {
    CHECK_NEGATIVE(__LINE__, offset < 0 || length, GL_INVALID_VALUE)
    GLuint buffer_binding;
    buffer_binding = get_bound_buffer(context, target);
    CHECK_ZERO(__LINE__, buffer_binding, GL_INVALID_OPERATION)

    MapResult *map_res = get_map_result_with_target(context, target);
    if (map_res == nullptr || map_res->map_data == nullptr ||
        (map_res->access & GL_MAP_FLUSH_EXPLICIT_BIT) == 0) {
        set_gl_error(context, GL_INVALID_OPERATION);
        return;
    }
    if (map_res->length < length + offset) {
        set_gl_error(context, GL_INVALID_VALUE);
        return;
    }

    ((ThreadContext *)egl_thread_context)
        ->data_tracer.trace_common(
            "glFlushMappedBufferRange target %x id %u offset %lld size %lld",
            target, get_bound_buffer(context, target), (long long)offset,
            (long long)length);
    if (map_res->map_data != nullptr) {
        ((ThreadContext *)egl_thread_context)
            ->data_tracer.trace_resource(
                (const char *)map_res->map_data + offset, length);
    }

    r_glFlushMappedBufferRange_special(
        context, target, offset, length,
        (const void *)(map_res->map_data + offset));

    return;
}

void d_glGetBufferPointerv(void *context, GLenum target, GLenum pname,
                           void **params) {
    CHECK_VALID_ENUM(__LINE__, target, 8, GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER,
                     GL_COPY_WRITE_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
                     GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER,
                     GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER);

    GLuint buffer_binding;
    buffer_binding = get_bound_buffer(context, target);
    CHECK_ZERO(__LINE__, buffer_binding, GL_INVALID_OPERATION)
    CHECK_VALID_ENUM(__LINE__, pname, 1, GL_BUFFER_MAP_POINTER)

    if (params == nullptr) {
        return;
    }

    // BufferMapped *buffer_ptr = ((GLThreadContext *)context)->buffer_ptr;

    GLuint id = get_bound_buffer(context, target);

    MapResult *map_res = get_map_result_with_id(context, id);

    if (map_res == nullptr) {
        *params = nullptr;
    } else {
        *params = map_res->map_data;
    }

    return;
}

void d_glBufferData(void *context, GLenum target, GLsizeiptr size,
                    const void *data, GLenum usage) {
    CHECK_VALID_ENUM(
        __LINE__, target, 12, GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER,
        GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER,
        GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
        GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_SHADER_STORAGE_BUFFER,
        GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER);
    CHECK_VALID_ENUM(__LINE__, usage, 9, GL_STREAM_DRAW, GL_STREAM_READ,
                     GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ,
                     GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ,
                     GL_DYNAMIC_COPY);
    CHECK_NEGATIVE(__LINE__, size, GL_INVALID_VALUE)
    GLint buffer_binding;
    buffer_binding = get_bound_buffer(context, target);
    if (target != GL_ATOMIC_COUNTER_BUFFER &&
        target != GL_DRAW_INDIRECT_BUFFER &&
        target != GL_SHADER_STORAGE_BUFFER &&
        target != GL_DISPATCH_INDIRECT_BUFFER) {
        CHECK_ZERO(__LINE__, buffer_binding, GL_INVALID_OPERATION)
    }

    MapResult *map_res = get_map_result_with_target(context, target);

    if (map_res != nullptr && map_res->map_data != nullptr) {
        ALOGD("unmapbuffer id %d when bufferdata", buffer_binding);
        d_glUnmapBuffer(context, target);
    }

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    if (target == GL_ELEMENT_ARRAY_BUFFER) {
        ElementArrayBuffer *ebo_data =
            bound_buffer->ebo_buffer[status->element_array_buffer];
        if (ebo_data != nullptr) {
            if (ebo_data->dp_max != nullptr) {
                for (int i = 0; i <= bound_buffer->ebo_mm[ebo_data->dp_num];
                     i++) {
                    delete[] ebo_data->dp_max[i];
                    delete[] ebo_data->dp_min[i];
                }
                delete[] ebo_data->dp_max;
                delete[] ebo_data->dp_min;
            }
        } else {
            ebo_data = new ElementArrayBuffer;
        }
        memset(ebo_data, 0, sizeof(ElementArrayBuffer));
        ebo_data->size = size;
        ebo_data->has_update = GL_TRUE;
        ebo_data->type = 0;
        bound_buffer->ebo_buffer[status->element_array_buffer] = ebo_data;
    }

    ALOGD("glBufferData target %x id %u size %lld usage %x crc %x", target,
          get_bound_buffer(context, target), (long long)size, usage, crc);

    // Save the buffer data to memory so that apps can correctly get the data
    // when the buffer is mapped (even in write mode).

    // Also, apps expect the entire buffer to be the same even if only a small
    // part of the buffer is accessed.
    if (map_res == nullptr) {
        map_res = new MapResult;
        map_res->access = 0;
        map_res->length = 0;
        map_res->map_data = nullptr;
        map_res->offset = 0;
        map_res->target = target;
        map_res->usage = usage;
        map_res->map_cnt = 0;

        // Element Array Buffer needs to be saved for calculating the range of
        // vertex data in glDrawElement.

        // In theory, all data usage mode should be saved to memory, so that the
        // data is unique.

        // However, in order to reduce the memory usage, only the data of
        // GL_DYNAMIC_DRAW is temporarily saved to memory.

        // In Asphalt 9 even some GL_ARRAY_BUFFER data of 128k in GL_STATIC_DRAW
        // mode needs to be saved to memory as it expects the entire buffer to
        // be readable
#ifdef SELECTIVE_MEMORY_SAVE
        if (usage == GL_DYNAMIC_DRAW || target == GL_ELEMENT_ARRAY_BUFFER ||
            (target == GL_ARRAY_BUFFER && size <= 1024 * 1024 &&
             (usage == GL_STATIC_DRAW || usage == GL_STREAM_DRAW))) {
            map_res->dynamic_data = new char[size];
        } else {
            map_res->dynamic_data = nullptr;
        }
#else
        map_res->dynamic_data = new char[size];
#endif

        map_res->total_len = size;

        ALOGD("glBufferData init dynamic data target %x", target);

        set_map_result_with_target(context, target, map_res);
    } else {
        map_res->target = target;

        map_res->usage = usage;
        if (map_res->total_len != size) {
#ifdef SELECTIVE_MEMORY_SAVE
            if (usage == GL_DYNAMIC_DRAW || target == GL_ELEMENT_ARRAY_BUFFER ||
                (target == GL_ARRAY_BUFFER && size <= 1024 * 1024 &&
                 (usage == GL_STATIC_DRAW || usage == GL_STREAM_DRAW))) {
                if (map_res->dynamic_data != nullptr) {
                    delete[] map_res->dynamic_data;
                }
                map_res->dynamic_data = new char[size];
            } else {
                map_res->dynamic_data = nullptr;
            }
#else
            if (map_res->dynamic_data != nullptr) {
                delete[] map_res->dynamic_data;
            }
            map_res->dynamic_data = new char[size];
#endif

            map_res->total_len = size;
        }
        ALOGD("glBufferData get old dynamic data");
    }

    if (target == GL_ELEMENT_ARRAY_BUFFER) {
        ElementArrayBuffer *ebo_data =
            bound_buffer->ebo_buffer[status->element_array_buffer];
        ebo_data->data = map_res->dynamic_data;
    }

    if (data != nullptr && map_res->dynamic_data != nullptr) {
        memcpy(map_res->dynamic_data, data, size);
    }

    ((ThreadContext *)egl_thread_context)
        ->data_tracer.trace_common(
            "glBufferData target %x id %u size %lld usage %x", target,
            get_bound_buffer(context, target), (long long)size, usage);
    if (data != nullptr) {
        ((ThreadContext *)egl_thread_context)
            ->data_tracer.trace_resource((const char *)data, size);
    }

    r_glBufferData_custom(context, target, size, data, usage);
}

void d_glBufferSubData(void *context, GLenum target, GLintptr offset,
                       GLsizeiptr size, const void *data) {
    CHECK_VALID_ENUM(
        __LINE__, target, 12, GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER,
        GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER,
        GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
        GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_SHADER_STORAGE_BUFFER,
        GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER);
    CHECK_NEGATIVE(__LINE__, size, GL_INVALID_VALUE)
    if (target != GL_ATOMIC_COUNTER_BUFFER &&
        target != GL_DRAW_INDIRECT_BUFFER &&
        target != GL_SHADER_STORAGE_BUFFER &&
        target != GL_DISPATCH_INDIRECT_BUFFER) {
        GLint buffer_binding;
        buffer_binding = get_bound_buffer(context, target);
        CHECK_ZERO(__LINE__, buffer_binding, GL_INVALID_OPERATION)
    }

    MapResult *map_res = get_map_result_with_target(context, target);
    if (map_res != nullptr) {
        if (map_res->map_data != nullptr) {
            ALOGE(
                "glBufferSubData: Buffer has already been mapped! Target %x "
                "buf_size %lld offset %lld size %lld size %lld length %lld",
                (int)target, (long long)map_res->total_len, (long long)offset,
                (long long)size, (long long)map_res->offset,
                (long long)map_res->length);
            set_gl_error(context, GL_INVALID_OPERATION);
            return;
        }
        if (map_res->dynamic_data != nullptr) {
            if (map_res->total_len >= offset + size) {
                memcpy(map_res->dynamic_data + offset, data, size);
            } else {
                ALOGE(
                    "glBufferSubData target %x buf_size %lld offset %lld size "
                    "%lld\n",
                    (int)target, (long long)map_res->total_len,
                    (long long)offset, (long long)size);
                set_gl_error(context, GL_INVALID_VALUE);
                return;
            }
        } else {
            ALOGE(
                "glBufferSubData target %x buf_size %lld offset %lld size %lld "
                "dynamic data is NULL\n",
                (int)target, (long long)map_res->total_len, (long long)offset,
                (long long)size);
        }
    } else {
        set_gl_error(context, GL_INVALID_VALUE);
        ALOGE("glBufferSubData target %x offset %lld size %lld data not init\n",
              (int)target, (long long)offset, (long long)size);
        return;
    }

    BoundBuffer *bound_buffer =
        ((GLThreadContext *)context)->context_binding_status.bound_buffer;
    BufferStatus *status = bound_buffer->buffer_status;
    if (target == GL_ELEMENT_ARRAY_BUFFER &&
        bound_buffer->ebo_buffer.find(status->element_array_buffer) !=
            bound_buffer->ebo_buffer.end()) {
        ElementArrayBuffer *ebo_data =
            bound_buffer->ebo_buffer[status->element_array_buffer];
        if (ebo_data == nullptr || ebo_data->data == nullptr) {
            ALOGE("Error! Ebo_data is not NULL during glBufferSubData!");
        } else {
            if (size + offset > ebo_data->size) {
                ALOGE("error! ebo size %lld but write offset %lld len %lld",
                      (long long)ebo_data->size, (long long)offset,
                      (long long)size);
            } else {
                // EBO is updated. ST table needs recalculations
                ebo_data->has_update = GL_TRUE;
            }
        }
    }

    ((ThreadContext *)egl_thread_context)
        ->data_tracer.trace_common(
            "glBufferSubData target %x id %u offset %lld size %lld", target,
            get_bound_buffer(context, target), (long long)offset,
            (long long)size);
    if (data != nullptr) {
        ((ThreadContext *)egl_thread_context)
            ->data_tracer.trace_resource((const char *)data, size);
    }

    r_glBufferSubData_custom(context, target, offset, size, data);
}
