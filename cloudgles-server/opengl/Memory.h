#ifndef GL_MEM_H
#define GL_MEM_H

#include <GLES2/gl2.h>
#include <GLES3/gl3.h>

#include <atomic>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "GLES/gl.h"
#include "Vertex.h"

struct Layout {
    uint32_t intra_stride_offset;
    uint32_t size;
    GLenum type;
};

// A StrideLayout contains the layout of a single stride in a vertex buffer.
// Also, it contains the start offset (and number of) of all the strides sharing
// the same layout.
struct StrideLayout {
    uint64_t start_offset = 0;
    uint32_t num_strides = 1;
    uint32_t single_stride_len = 0;

    std::vector<Layout> layouts;

    void reset();    
    uint32_t get_layout_size() const;
    void merge_with(const StrideLayout &other);
    bool can_merge_with(const StrideLayout &other) const;
};

struct VertexMemLayout {
    GLuint id = 0;
    std::vector<StrideLayout> stride_layouts;
    bool fully_parsed = false;

    void set_layout(const AttribPoint *attrib_point, uint32_t index);
    static VertexMemLayout *get_vertex_mem_layout_with_id(void *context,
                                                          GLuint id);
    static void draw_arrays(void *context, GLsizei count);
    static void draw_elements(void *context, GLsizei count, GLenum type,
                              const void *elements);
};

struct MapResult {
    GLenum target;
    GLenum usage;
    GLintptr offset;
    GLsizeiptr length;
    GLsizeiptr total_len;
    GLbitfield access;
    int map_cnt;
    char *dynamic_data;
    char *map_data;
};

struct BufferMapped {
    std::unordered_map<GLuint, MapResult *> buffer_maps;
    std::unordered_map<GLuint, VertexMemLayout> buffer_layout_maps;
    std::atomic_flag buffer_maps_lock = ATOMIC_FLAG_INIT;
};

MapResult *get_map_result_with_target(void *context, GLenum target);
MapResult *get_map_result_with_id(void *context, GLuint id);

void set_map_result_with_target(void *context, GLenum target,
                                MapResult *map_res);
void set_map_result_with_id(void *context, GLuint id, MapResult *map_res);

#endif  // GL_MEM_H
