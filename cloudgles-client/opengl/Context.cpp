#include "Context.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglext_angle.h>
#include <EGL/eglplatform.h>
#include <GLES/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl3platform.h>

#include "GLTranslator.h"
#include "Log.h"
#include "Program.h"
#include "Resource.h"
#include "Utils.h"
#include "egl/Context.h"
#include "renderer/MainRenderer.h"

#define LOG_TAG "GLContext"

std::list<GLContext *> GLContext::native_context_pool{};
std::atomic_flag GLContext::native_context_pool_lock = ATOMIC_FLAG_INIT;

GLContext *GLContext::opengl_context_create(GLContext *share_context,
                                            int independ_mode) {
    GLContext *opengl_context = new GLContext();
    opengl_context->is_current = 0;
    opengl_context->need_destroy = 0;
    opengl_context->native_context = NULL;
    opengl_context->is_using_external_program = 0;
    opengl_context->share_context = share_context;
    opengl_context->independent_mode = independ_mode;
    opengl_context->mc_fence = -1;

    // opengl_context->bind_image = NULL;

    TextureBindingStatus *texture_status =
        &(opengl_context->texture_binding_status);
    memset(texture_status, 0, sizeof(TextureBindingStatus));

    texture_status->guest_current_texture_2D = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->host_current_texture_2D = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->guest_current_texture_cube_map = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->host_current_texture_cube_map = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->guest_current_texture_3D = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->host_current_texture_3D = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->guest_current_texture_2D_array = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->host_current_texture_2D_array = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->guest_current_texture_2D_multisample = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->host_current_texture_2D_multisample = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->guest_current_texture_2D_multisample_array =
        (GLuint *)calloc(
            MainRenderer::get_instance()
                ->static_context_values->max_combined_texture_image_units,
            sizeof(GLuint));

    texture_status->host_current_texture_2D_multisample_array =
        (GLuint *)calloc(
            MainRenderer::get_instance()
                ->static_context_values->max_combined_texture_image_units,
            sizeof(GLuint));

    texture_status->guest_current_texture_cube_map_array = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->host_current_texture_cube_map_array = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->guest_current_texture_buffer = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    texture_status->host_current_texture_buffer = (GLuint *)calloc(
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units,
        sizeof(GLuint));

    // texture_status->guest_current_texture_unit = g_malloc(sizeof(GLuint) *
    // MainRenderer::get_instance()->static_context_values->max_combined_texture_image_units);
    // memset(texture_status->guest_current_texture_unit, 0, sizeof(GLuint) *
    // MainRenderer::get_instance()->static_context_values->max_combined_texture_image_units);

    // texture_status->host_current_texture_unit = g_malloc(sizeof(GLuint) *
    // MainRenderer::get_instance()->static_context_values->max_combined_texture_image_units);
    // memset(texture_status->host_current_texture_unit, 0, sizeof(GLuint) *
    // MainRenderer::get_instance()->static_context_values->max_combined_texture_image_units);

    texture_status->texture_unit_num =
        MainRenderer::get_instance()
            ->static_context_values->max_combined_texture_image_units;

    // opengl_context->current_target = GL_TEXTURE_2D;

    opengl_context->view_x = 0;
    opengl_context->view_y = 0;
    opengl_context->view_w = 0;
    opengl_context->view_h = 0;

    
    //  #ifdef USE_GLFW_AS_WGL
    //  send_message_to_main_window(MAIN_CREATE_CHILD_WINDOW,
    //  &(opengl_context->window));
    opengl_context->native_context = get_native_opengl_context(independ_mode);
    // #else
    
    //     opengl_context->window = egl_createContext();
    // #endif
    // printf("send message create window opengl context %llx window_ptr
    // %llx\n", (uint64_t)opengl_context, &(opengl_context->window));

    ShareResources *share_resources = NULL;
    if (share_context != NULL) {
        share_resources = share_context->resource_status.share_resources;
        share_context->share_context = opengl_context;
    }

    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);
    //    memset(bound_buffer, 0, sizeof(BoundBuffer));

    //    opengl_context->buffer_map = g_hash_table_new_full(g_direct_hash,
    //    g_direct_equal, NULL, g_buffer_map_destroy);
    //
    //    bound_buffer->vao_point_data = g_hash_table_new_full(g_direct_hash,
    //    g_direct_equal, NULL, g_vao_point_data_destroy);

    AttribPoint *temp_point = (AttribPoint *)calloc(1, sizeof(AttribPoint));

    bound_buffer->attrib_point = temp_point;

    resource_context_init(&(opengl_context->resource_status), share_resources);

    bound_buffer->async_unpack_texture_buffer = 0;
    bound_buffer->async_pack_texture_buffer = 0;

    bound_buffer->has_init = 0;

    opengl_context->draw_fbo0 = 0;
    opengl_context->read_fbo0 = 0;

    opengl_context->draw_texi_vbo = 0;
    opengl_context->draw_texi_vao = 0;
    opengl_context->draw_texi_ebo = 0;

    return opengl_context;
}

void *GLContext::get_native_opengl_context(int independ_mode) {
    void *native_context;

    EGLint num_configs = 0;
    EGLConfig egl_config;
    if (!eglChooseConfig(MainRenderer::get_instance()->main_native_display,
                         MainRenderer::display_config_attrib_list, &egl_config,
                         1, &num_configs)) {
        ALOGE("eglChooseConfig return false");
    }
    EGLint surface_attrs[] = {EGL_WIDTH,           400,      EGL_HEIGHT, 400,
                              EGL_LARGEST_PBUFFER, EGL_TRUE, EGL_NONE};
    EGLSurface native_surface = eglCreatePbufferSurface(
        MainRenderer::get_instance()->main_native_display, egl_config,
        surface_attrs);
    EGLint context_attrs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    native_context = eglCreateContext(
        MainRenderer::get_instance()->main_native_display, egl_config,
        MainRenderer::get_instance()->main_native_context, context_attrs);
    MainRenderer::get_instance()
        ->main_native_context_surface_map[native_context] = native_surface;
    if (native_context == EGL_NO_CONTEXT) {
        ALOGE("eglCreateContext return EGL_NO_CONTEXT %x", eglGetError());
    }

    return native_context;
}

void GLContext::resource_context_init(ResourceContext *resources,
                                      ShareResources *share_resources) {
    if (share_resources != NULL) {
        resources->share_resources = share_resources;
        resources->share_resources->counter += 1;
    } else {
        resources->share_resources = new ShareResources();
        memset(resources->share_resources, 0, sizeof(ShareResources));
        resources->share_resources->counter = 1;
    }

    resources->exclusive_resources = new ExclusiveResources();
    memset(resources->exclusive_resources, 0, sizeof(ExclusiveResources));

    resources->texture_resource =
        &(resources->share_resources->texture_resource);
    resources->buffer_resource = &(resources->share_resources->buffer_resource);
    resources->render_buffer_resource =
        &(resources->share_resources->render_buffer_resource);
    resources->sampler_resource =
        &(resources->share_resources->sample_resource);

    resources->shader_resource = &(resources->share_resources->shader_resource);
    resources->program_resource =
        &(resources->share_resources->program_resource);

    resources->sync_resource = &(resources->share_resources->sync_resource);

    resources->frame_buffer_resource =
        &(resources->exclusive_resources->frame_buffer_resource);
    resources->program_pipeline_resource =
        &(resources->exclusive_resources->program_pipeline_resource);
    resources->transform_feedback_resource =
        &(resources->exclusive_resources->transform_feedback_resource);
    resources->vertex_array_resource =
        &(resources->exclusive_resources->vertex_array_resource);

    resources->query_resource =
        &(resources->exclusive_resources->query_resource);
}

void GLContext::opengl_context_init(GLContext *context) {
    
    BoundBuffer *bound_buffer = &(context->bound_buffer_status);
    ResourceContext *resource_status = &(context->resource_status);

    ResourceMapStatus *map_status = resource_status->vertex_array_resource;

    if (bound_buffer->has_init == 0) {
        
        bound_buffer->has_init = 1;

        GLuint vao0 = 0;
        GLuint temp_guest_vao = 0;
        unsigned long long temp_host_vao = 0;

        if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                       MainRenderer::DSA_ENABLE != 0)) {
            //            glCreateVertexArrays(1, &vao0);
            //            glCreateBuffers(1,
            //            &(bound_buffer->async_unpack_texture_buffer));
            //            glCreateBuffers(1,
            //            &(bound_buffer->async_pack_texture_buffer));
            //
            //            glCreateBuffers(1,
            //            &(bound_buffer->attrib_point->indices_buffer_object));
            //            glCreateBuffers(MAX_VERTEX_ATTRIBS_NUM,
            //            bound_buffer->attrib_point->buffer_object);
            //
            //            glVertexArrayElementBuffer(vao0,
            //            bound_buffer->attrib_point->indices_buffer_object);
            bound_buffer->attrib_point->element_array_buffer =
                bound_buffer->attrib_point->indices_buffer_object;
        } else {
            glGenVertexArrays(1, &vao0);

            glGenBuffers(1, &(bound_buffer->async_unpack_texture_buffer));
            glGenBuffers(1, &(bound_buffer->async_pack_texture_buffer));

            glGenBuffers(1,
                         &(bound_buffer->attrib_point->indices_buffer_object));
            glGenBuffers(MAX_VERTEX_ATTRIBS_NUM,
                         bound_buffer->attrib_point->buffer_object);
        }

        // printf("context %llx init vao %d\n",(uint64_t)context, vao0);

        temp_host_vao = vao0;
        GLResource::create_host_map_ids(map_status, 1, &temp_guest_vao,
                                        &temp_host_vao);

        //        g_hash_table_insert(bound_buffer->vao_point_data,
        //        GUINT_TO_POINTER(vao0), (gpointer)bound_buffer->attrib_point);
        bound_buffer->vao_point_data[vao0] = bound_buffer->attrib_point;

        bound_buffer->buffer_status.host_vao = vao0;
        bound_buffer->buffer_status.guest_vao = vao0;
        context->vao0 = vao0;

        glBindVertexArray(vao0);

        
        //         glEnable(GL_PROGRAM_POINT_SIZE);
        //         glEnable(GL_POINT_SPRITE);

        
        //         glEnable(GL_FRAMEBUFFER_SRGB);

        
        
        //  glActiveTexture(GL_TEXTURE0);
        //  glDisable(GL_CULL_FACE);
        //  glDisable(GL_POLYGON_OFFSET_FILL);
        //  glDisable(GL_SCISSOR_TEST);
        //  glDisable(GL_SAMPLE_COVERAGE);
        //  glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        //  glDisable(GL_STENCIL_TEST);
        //  glDisable(GL_DEPTH_TEST);
        //  glDisable(GL_BLEND);
        //  glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
        //  glDisable(GL_RASTERIZER_DISCARD);
        //  glDisable(GL_DITHER);
        //  glDisable(GL_CULL_FACE);
        //  glDisable(GL_CULL_FACE);
        

        // for(int i =0;i<16;i++)
        // {
        //     glDisableVertexAttribArray(i);
        // }

        
        
        //  glViewport(context->view_x, context->view_y, context->view_w,
        //  context->view_h);
    }
}

void GLContext::d_glGetString_special(void *context, GLenum name,
                                      GLubyte *buffer) {
    const GLubyte *static_string = glGetString(name);
    int len = strlen((const char *)static_string);
    if (len >= 1024) {
        len = 1023;
        printf("error, glGetString string too long %x %s", name, static_string);
    }
    memcpy(buffer, static_string, len);
    // #1024
}

void GLContext::d_glGetStringi_special(void *context, GLenum name, GLuint index,
                                       GLubyte *buffer) {
    const GLubyte *static_string = glGetStringi(name, index);
    ALOGD("getStringi index %u:%s\n", index, static_string);
    int len = strlen((const char *)static_string);
    if (len >= 1024) {
        len = 1023;
        printf("error, glGetStringi string too long %x %u %s", name, index,
               static_string);
    }
    memcpy(buffer, static_string, len);
}

void GLContext::opengl_context_destroy(GLContext *context) {
    ALOGD("opengl context destroy %llx guest %llx\n", (uint64_t)context,
          (uint64_t)context->guest_context);
    GLContext *opengl_context = (GLContext *)context;

    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);
    for (auto &pair: opengl_context->fence_event_map) {
        if (pair.second != nullptr) {
            delete_event(pair.second);
        }
    }

    TextureBindingStatus *texture_status =
        &(opengl_context->texture_binding_status);
    free(texture_status->guest_current_texture_2D);
    free(texture_status->host_current_texture_2D);
    free(texture_status->guest_current_texture_cube_map);
    free(texture_status->host_current_texture_cube_map);
    free(texture_status->guest_current_texture_3D);
    free(texture_status->host_current_texture_3D);
    free(texture_status->guest_current_texture_2D_array);
    free(texture_status->host_current_texture_2D_array);
    free(texture_status->guest_current_texture_2D_multisample);
    free(texture_status->host_current_texture_2D_multisample);
    free(texture_status->guest_current_texture_2D_multisample_array);
    free(texture_status->host_current_texture_2D_multisample_array);
    free(texture_status->guest_current_texture_cube_map_array);
    free(texture_status->host_current_texture_cube_map_array);
    free(texture_status->guest_current_texture_buffer);
    free(texture_status->host_current_texture_buffer);
    // g_free(texture_status->guest_current_texture_unit);
    // g_free(texture_status->host_current_texture_unit);

    if (opengl_context->independent_mode == 1) {
        ALOGE("glfwMakeContextCurrent((GLFWwindow *)opengl_context->window);");
    } else {
        EglContext::native_make_current(opengl_context->native_context);
    }

    // TODO: g_hash_table_destroy
    //    // g_hash_table_remove_all(opengl_context->buffer_map);
    //    g_hash_table_destroy(opengl_context->buffer_map);
    //    // g_hash_table_destroy(bound_buffer->vao_status);
    //    g_hash_table_destroy(bound_buffer->vao_point_data);

    if (bound_buffer->has_init == 1) {
        glDeleteBuffers(1, &(bound_buffer->async_unpack_texture_buffer));
        glDeleteBuffers(1, &(bound_buffer->async_pack_texture_buffer));
    }

    if (opengl_context->draw_texi_vbo != 0) {
        glDeleteBuffers(1, &(opengl_context->draw_texi_vbo));
    }

    if (opengl_context->draw_texi_ebo != 0) {
        glDeleteBuffers(1, &(opengl_context->draw_texi_ebo));
    }

    if (opengl_context->draw_texi_vao != 0) {
        glDeleteVertexArrays(1, &(opengl_context->draw_texi_vao));
    }

    resource_context_destroy(&(opengl_context->resource_status));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if (opengl_context->independent_mode == 1) {
        //        glfwHideWindow(opengl_context->window);
        //        glfwMakeContextCurrent(NULL);
    } else {
        ALOGD("context %llx native context %llx makecurrent null\n",
              (uint64_t)opengl_context, opengl_context->native_context);
        EglContext::native_make_current(nullptr);
    }
    release_native_opengl_context(opengl_context->native_context,
                                  opengl_context->independent_mode);
}

#define DESTROY_RESOURCES(resource_name, resource_delete)                    \
    if (resources->resource_name->resource_id_map != NULL) {                 \
        for (int i = 1; i <= resources->resource_name->max_id; i++) {        \
            if (resources->resource_name->resource_id_map[i] <= 0) continue; \
            if (now_delete_len < 1000) {                                     \
                delete_buffers[now_delete_len] =                             \
                    (GLuint)resources->resource_name->resource_id_map[i];    \
                now_delete_len += 1;                                         \
            } else {                                                         \
                resource_delete(now_delete_len, delete_buffers);             \
                now_delete_len = 0;                                          \
            }                                                                \
        }                                                                    \
        if (now_delete_len != 0) {                                           \
            resource_delete(now_delete_len, delete_buffers);                 \
            now_delete_len = 0;                                              \
        }                                                                    \
        free(resources->resource_name->resource_is_init);                    \
        free(resources->resource_name->resource_id_map);                     \
    }

void GLContext::resource_context_destroy(ResourceContext *resources) {
    GLuint delete_buffers[1000];
    GLuint now_delete_len = 0;

    resources->share_resources->counter -= 1;
    if (resources->share_resources->counter == 0) {
        DESTROY_RESOURCES(texture_resource, glDeleteTextures);
        
        if (resources->texture_resource->gbuffer_ptr_map != NULL) {
            free(resources->texture_resource->gbuffer_ptr_map);
        }

        DESTROY_RESOURCES(buffer_resource, glDeleteBuffers);
        DESTROY_RESOURCES(render_buffer_resource, glDeleteRenderbuffers);
        DESTROY_RESOURCES(sampler_resource, glDeleteSamplers);

        if (resources->shader_resource->resource_id_map != NULL) {
            for (int i = 1; i <= resources->shader_resource->max_id; i++) {
                if (resources->shader_resource->resource_id_map[i] != 0) {
                    glDeleteShader(
                        (GLuint)resources->shader_resource->resource_id_map[i]);
                }
            }
            free(resources->shader_resource->resource_id_map);
        }

        if (resources->program_resource->resource_id_map != NULL) {
            for (int i = 1; i <= resources->program_resource->max_id; i++) {
                if (resources->program_resource->resource_id_map[i] != 0) {
                    // TODO: Free program resources
                    auto res_id =
                        (GLuint)resources->program_resource->resource_id_map[i];
                    unordered_map_erase_with_callback(
                        GLProgram::program_is_external_map, res_id,
                        static_cast<std::function<void(GLuint)>>(nullptr));
                    unordered_map_erase_with_callback(
                        GLProgram::program_data_map, res_id,
                        static_cast<std::function<void(GLchar *)>>(
                            GLProgram::g_program_data_destroy));
                    glDeleteProgram((GLuint)resources->program_resource
                                        ->resource_id_map[i]);
                }
            }
            free(resources->program_resource->resource_id_map);
        }

        if (resources->sync_resource->resource_id_map != NULL) {
            for (int i = 1; i <= resources->sync_resource->max_id; i++) {
                if (resources->sync_resource->resource_id_map[i] != 0) {
                    glDeleteSync(
                        (GLsync)resources->sync_resource->resource_id_map[i]);
                }
            }
            free(resources->sync_resource->resource_id_map);
        }

        free(resources->share_resources);
    }

    
    DESTROY_RESOURCES(frame_buffer_resource, glDeleteFramebuffers);
    DESTROY_RESOURCES(program_pipeline_resource, glDeleteProgramPipelines);
    DESTROY_RESOURCES(transform_feedback_resource, glDeleteTransformFeedbacks);
    DESTROY_RESOURCES(vertex_array_resource, glDeleteVertexArrays);

    DESTROY_RESOURCES(query_resource, glDeleteQueries);

    free(resources->exclusive_resources);
}

void GLContext::release_native_opengl_context(void *native_context,
                                              int independ_mode) {
    if (independ_mode == 1) {
        ALOGE(
            "glfwSetWindowShouldClose(native_context, "
            "1);glfwDestroyWindow(native_context);");
    } else {
        eglDestroyContext(MainRenderer::get_instance()->main_native_display,
                          native_context);
        eglDestroySurface(
            MainRenderer::get_instance()->main_native_display,
            MainRenderer::get_instance()
                ->main_native_context_surface_map[native_context]);
    }
}

void GLContext::g_context_map_destroy(GLContext *data) {
    auto *real_context = (GLContext *)data;
    if (real_context == nullptr) return;
    if (real_context->is_current) {
        ALOGD("context %llx guest %llx is using\n", (uint64_t)real_context,
              (uint64_t)real_context->guest_context);
        real_context->need_destroy = 1;
    } else {
        ALOGD("destroy context %llx\n", (uint64_t)real_context);
        opengl_context_destroy(real_context);
        free(real_context);
    }
}

void GLContext::fence_sync(int64_t fence) {
    do {
        SPIN_LOCK(mc_fence_lock);
        volatile int64_t current_fence =
            mc_fence.load(std::memory_order_seq_cst);
        if (!is_current &&
            ((current_fence == (fence - 1)) ||
             (current_fence == INT64_MAX && fence == 0)) /* fence overflow */) {
            // MakeCurrent has been called in the correct order, directly return
            mc_fence.store(fence, std::memory_order_seq_cst);
            if (fence_event_map.find(fence) != fence_event_map.end()) {
                delete_event(fence_event_map[fence]);
                fence_event_map.erase(fence);
            }
            SPIN_UNLOCK(mc_fence_lock);
            return;
        }
        // MakeCurrent has been called in the wrong order, wait for signal
        // Each fence must have its own event, so that there can't be signals mistakenly consumed by another fence
        if (fence_event_map.find(fence) == fence_event_map.end()) {
            fence_event_map[fence] = create_event(0, 0);
        }
        void *event = fence_event_map[fence];
        SPIN_UNLOCK(mc_fence_lock);
        wait_event(event);
    } while (true);
}

void GLContext::signal_sync() {
    SPIN_LOCK(mc_fence_lock);
    int64_t current_fence = mc_fence.load(std::memory_order_seq_cst);
    if (fence_event_map.find(current_fence + 1) != fence_event_map.end()) {
        set_event(fence_event_map[current_fence + 1]);
    } else {
        fence_event_map[current_fence + 1] = create_event(0, 0);
        set_event(fence_event_map[current_fence + 1]);
    }
    SPIN_UNLOCK(mc_fence_lock);
}

void AttribPoint::g_vao_point_data_destroy(AttribPoint *data) {
    auto *vao_point = (AttribPoint *)data;
    if (vao_point == nullptr) return;
    if (vao_point->indices_buffer_object != 0) {
        glDeleteBuffers(1, &(vao_point->indices_buffer_object));
        glDeleteBuffers(MAX_VERTEX_ATTRIBS_NUM, vao_point->buffer_object);
    }

    ALOGD("vao_point destroy\n");

    free(vao_point);
}