#ifndef GL_RESOURCE_H
#define GL_RESOURCE_H

#include <atomic>
#include <queue>

class ResourceCreator {
   private:
    std::atomic_flag locker = ATOMIC_FLAG_INIT;
    unsigned int bitmap_size;
    unsigned char *resource_id_bimap;
    unsigned int max_id;

    std::queue<unsigned int> delete_id_queue;

   public:
    ResourceCreator();
    explicit ResourceCreator(int size);
    ~ResourceCreator();

    unsigned int get_max_id();

    unsigned int create_id();

    void delete_id(unsigned int id);
    void set_id(unsigned int id);
    bool unset_id(unsigned int id);

    bool has_id(unsigned int id);
};

typedef struct ShareResource {
    // int counter;

    // Regular objects
    ResourceCreator texture_has_gbuffer;
    ResourceCreator texture_resource;
    ResourceCreator buffer_resource;
    ResourceCreator render_buffer_resource;
    ResourceCreator sample_resource;

    // GLSL objects, include program,shader
    //     Resource_Creator glsl_resource;
    ResourceCreator program_resource;
    ResourceCreator shader_resource;
    ResourceCreator shader_is_external;

    // Sync objects
    ResourceCreator sync_resource;

    int has_create;

} ShareResource;

typedef struct ExclusiveResource {
    // Container objects
    ResourceCreator frame_buffer_resource;
    ResourceCreator program_pipeline_resource;
    ResourceCreator transform_feedback_resource;
    ResourceCreator vertex_array_resource;

    // query objects
    ResourceCreator query_resource;

} ExclusiveResource;

typedef struct ResourceContext {
    // Regular objects
    ResourceCreator *texture_has_gbuffer() {
        return &share_resources->texture_has_gbuffer;
    };  
    ResourceCreator *texture_resource() {
        return &share_resources->texture_resource;
    };
    ResourceCreator *buffer_resource() {
        return &share_resources->buffer_resource;
    };
    ResourceCreator *render_buffer_resource() {
        return &share_resources->render_buffer_resource;
    };
    ResourceCreator *sampler_resource() {
        return &share_resources->sample_resource;
    };

    // GLSL objects, include program,shader
    ResourceCreator *shader_resource() {
        return &share_resources->shader_resource;
    };
    ResourceCreator *program_resource() {
        return &share_resources->program_resource;
    };
    ResourceCreator *shader_is_external() {
        return &share_resources->shader_is_external;
    };

    // Sync objects
    ResourceCreator *sync_resource() {
        return &share_resources->sync_resource;
    };

    // Container objects
    ResourceCreator *frame_buffer_resource() {
        return &exclusive_resources->frame_buffer_resource;
    };
    ResourceCreator *program_pipeline_resource() {
        return &exclusive_resources->program_pipeline_resource;
    };
    ResourceCreator *transform_feedback_resource() {
        return &exclusive_resources->transform_feedback_resource;
    };
    ResourceCreator *vertex_array_resource() {
        return &exclusive_resources->vertex_array_resource;
    };

    //  query objects
    ResourceCreator *query_resource() {
        return &exclusive_resources->query_resource;
    };

    ShareResource *share_resources;

    ExclusiveResource *exclusive_resources;

} ResourceContext;

void create_resource_context(ResourceContext *cur_context,
                             ResourceContext *share_context);

void set_share_context(ResourceContext *share_context);

#endif  // GL_RESOURCE_H
