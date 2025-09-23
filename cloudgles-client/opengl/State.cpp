#include "State.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glplatform.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#include <GLES3/gl3.h>
#include <GLES3/gl32.h>

#include "Context.h"
#include "Log.h"
#include "Resource.h"
#include "egl/Surface.h"
#include "renderer/MainRenderer.h"
#include "renderer/RenderThread.h"

#define LOG_TAG "GLState"

GLuint GLState::get_guest_binding_texture(void *context, GLenum target) {
    GLContext *opengl_context = (GLContext *)context;

    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    GLuint current_active_texture = status->guest_current_active_texture;

    switch (target) {
        case GL_TEXTURE_2D:
            return status->guest_current_texture_2D[current_active_texture];
        case GL_TEXTURE_2D_MULTISAMPLE:
            return status
                ->guest_current_texture_2D_multisample[current_active_texture];
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            return status->guest_current_texture_2D_multisample_array
                [current_active_texture];
        case GL_TEXTURE_3D:
            return status->guest_current_texture_3D[current_active_texture];
        case GL_TEXTURE_2D_ARRAY:
            return status
                ->guest_current_texture_2D_array[current_active_texture];
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
            return status
                ->guest_current_texture_cube_map[current_active_texture];
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            return status
                ->guest_current_texture_cube_map_array[current_active_texture];
        case GL_TEXTURE_BUFFER:
            return status->guest_current_texture_buffer[current_active_texture];
        case GL_TEXTURE_EXTERNAL_OES:
            return status->current_texture_external;
            break;
        default:
            break;
    }
    ALOGD("error! get_guest_binding_texture target %x\n", target);
    return 0;
}

GLuint GLState::get_guest_binding_buffer(void *context, GLenum target) {
    GLContext *opengl_context = (GLContext *)context;
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    switch (target) {
        case GL_ARRAY_BUFFER:
            return status->guest_array_buffer;
        case GL_ELEMENT_ARRAY_BUFFER:
            return status->guest_element_array_buffer;
        case GL_COPY_READ_BUFFER:
            return status->guest_copy_read_buffer;
        case GL_COPY_WRITE_BUFFER:
            return status->guest_copy_write_buffer;
        case GL_PIXEL_PACK_BUFFER:
            return status->guest_pixel_pack_buffer;
        case GL_PIXEL_UNPACK_BUFFER:
            return status->guest_pixel_unpack_buffer;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            return status->guest_transform_feedback_buffer;
        case GL_UNIFORM_BUFFER:
            return status->guest_uniform_buffer;
        case GL_ATOMIC_COUNTER_BUFFER:
            return status->guest_atomic_counter_buffer;
        case GL_DISPATCH_INDIRECT_BUFFER:
            return status->guest_dispatch_indirect_buffer;
        case GL_DRAW_INDIRECT_BUFFER:
            return status->guest_draw_indirect_buffer;
        case GL_SHADER_STORAGE_BUFFER:
            return status->guest_shader_storage_buffer;
        case GL_TEXTURE_BUFFER:
            return status->guest_texture_buffer;
        default:
            ALOGD("error target %x get_guest_binding_buffer\n", target);
            break;
    }
    return 0;
}

void GLState::d_glBindFramebuffer_special(void *context, GLenum target,
                                          GLuint framebuffer) {
    GLuint draw_fbo0 = ((GLContext *)context)->draw_fbo0;
    GLuint read_fbo0 = ((GLContext *)context)->read_fbo0;

    if (framebuffer == 0) {
        if (target == GL_DRAW_FRAMEBUFFER || target == GL_FRAMEBUFFER) {
            // printf("conetxt %llx bind 0 framebuffer draw
            // %u\n",(uint64_t)context, draw_fbo0);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_fbo0);
        }
        if (target == GL_READ_FRAMEBUFFER || target == GL_FRAMEBUFFER) {
            // printf("conetxt %llx bind 0 framebuffer read
            // %u\n",(uint64_t)context, read_fbo0);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, read_fbo0);
        }
    } else {
        glBindFramebuffer(target, framebuffer);
        // GLenum status = glCheckFramebufferStatus(target) ;
        // printf("conetxt %llx bind framebuffer %u status
        // %x\n",(uint64_t)context, framebuffer, status);
    }
}

void GLState::d_glBindBuffer_special(void *context, GLenum target,
                                     GLuint guest_buffer) {
    GLContext *opengl_context = (GLContext *)context;
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    GLuint buffer = (GLuint)GLResource::get_host_buffer_id(
        opengl_context, (unsigned int)guest_buffer);

    switch (target) {
        case GL_ARRAY_BUFFER:
            status->guest_array_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_array_buffer = buffer;
            }
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            // if(guest_buffer == 0 && status->guest_vao ==
            // opengl_context->vao0)
            // {

            // }
            status->guest_element_array_buffer = buffer;
            // opengl_context->bound_buffer_status.attrib_point->element_array_buffer
            // = buffer;
            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                if (buffer == 0) {
                    AttribPoint *point_data =
                        opengl_context->bound_buffer_status.attrib_point;
                    if (status->guest_vao_ebo !=
                        point_data->indices_buffer_object) {
                        status->guest_vao_ebo =
                            point_data->indices_buffer_object;
                        //                        glVertexArrayElementBuffer(status->guest_vao,
                        //                        point_data->indices_buffer_object);
                        ALOGE("DSA not supported.");
                    }
                } else {
                    if (status->guest_vao_ebo != buffer) {
                        status->guest_vao_ebo = buffer;
                        //                        glVertexArrayElementBuffer(status->guest_vao,
                        //                        status->guest_vao_ebo);
                        ALOGE("DSA not supported.");
                    }
                }
            }

            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_element_array_buffer = buffer;
            }
            break;
        case GL_COPY_READ_BUFFER:
            status->guest_copy_read_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_copy_read_buffer = buffer;
            }
            break;
        case GL_COPY_WRITE_BUFFER:
            status->guest_copy_write_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_copy_write_buffer = buffer;
            }
            break;
        case GL_PIXEL_PACK_BUFFER:
            status->guest_pixel_pack_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_pixel_pack_buffer = buffer;
            }
            break;
        case GL_PIXEL_UNPACK_BUFFER:
            status->guest_pixel_unpack_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_pixel_unpack_buffer = buffer;
            }
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            status->guest_transform_feedback_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_transform_feedback_buffer = buffer;
            }
            break;
        case GL_UNIFORM_BUFFER:
            status->guest_uniform_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_uniform_buffer = buffer;
            }
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            status->guest_atomic_counter_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_atomic_counter_buffer = buffer;
            }
            break;
        case GL_DISPATCH_INDIRECT_BUFFER:
            status->guest_dispatch_indirect_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_dispatch_indirect_buffer = buffer;
            }
            break;
        case GL_DRAW_INDIRECT_BUFFER:
            status->guest_draw_indirect_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_draw_indirect_buffer = buffer;
            }
            break;
        case GL_SHADER_STORAGE_BUFFER:
            status->guest_shader_storage_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_shader_storage_buffer = buffer;
            }
            break;
        case GL_TEXTURE_BUFFER:
            status->guest_texture_buffer = buffer;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_texture_buffer = buffer;
            }
            break;
        default:
            printf("error target %x buffer %d glBindBuffer\n", target, buffer);
            break;
    }

    ALOGD("context %llx glBindBuffer target %x buffer %d guest %d\n",
          (uint64_t)context, target, buffer, guest_buffer);

    // if((MainRenderer::HOST_OPENGL_VERSION < 45 || MainRenderer::DSA_ENABLE
    // == 0) || is_init == 0)
    if (MainRenderer::HOST_OPENGL_VERSION < 45 || MainRenderer::DSA_ENABLE == 0)
    // if(target != GL_ELEMENT_ARRAY_BUFFER)
    {
        glBindBuffer(target, buffer);
    }
}

void GLState::d_glBindBufferRange_special(void *context, GLenum target,
                                          GLuint index, GLuint guest_buffer,
                                          GLintptr offset, GLsizeiptr size) {
    GLContext *opengl_context = (GLContext *)context;
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    GLuint buffer = (GLuint)GLResource::get_host_buffer_id(
        opengl_context, (unsigned int)guest_buffer);

    switch (target) {
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            status->guest_transform_feedback_buffer = buffer;
            status->host_transform_feedback_buffer = buffer;
            break;
        case GL_UNIFORM_BUFFER:
            status->guest_uniform_buffer = buffer;
            status->host_uniform_buffer = buffer;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            status->guest_atomic_counter_buffer = buffer;
            status->host_atomic_counter_buffer = buffer;
            break;
        case GL_SHADER_STORAGE_BUFFER:
            status->guest_shader_storage_buffer = buffer;
            status->host_shader_storage_buffer = buffer;
            break;
        default:
            printf("error target %x buffer %d glBindBufferRange\n", target,
                   buffer);
            break;
    }

    ALOGD("context %llx glBindBufferRange target %x buffer %d\n",
          (uint64_t)context, target, buffer);

    glBindBufferRange(target, index, buffer, offset, size);
}

void GLState::d_glBindBufferBase_special(void *context, GLenum target,
                                         GLuint index, GLuint guest_buffer) {
    GLContext *opengl_context = (GLContext *)context;
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    GLuint buffer = (GLuint)GLResource::get_host_buffer_id(
        opengl_context, (unsigned int)guest_buffer);

    switch (target) {
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            status->guest_transform_feedback_buffer = buffer;
            status->host_transform_feedback_buffer = buffer;
            break;
        case GL_UNIFORM_BUFFER:
            status->guest_uniform_buffer = buffer;
            status->host_uniform_buffer = buffer;
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            status->guest_atomic_counter_buffer = buffer;
            status->host_atomic_counter_buffer = buffer;
            break;
        case GL_SHADER_STORAGE_BUFFER:
            status->guest_shader_storage_buffer = buffer;
            status->host_shader_storage_buffer = buffer;
            break;
        default:
            printf("error target %x buffer %d glBindBufferBase\n", target,
                   buffer);
            break;
    }

    ALOGD("context %llx glBindBufferBase target %x buffer %d\n",
          (uint64_t)context, target, buffer);
    glBindBufferBase(target, index, buffer);
}

void GLState::buffer_binding_status_sync(void *context, GLenum target) {
    int need_sync = 0;
    GLuint new_buffer = 0;

    GLContext *opengl_context = (GLContext *)context;
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    switch (target) {
        case GL_ARRAY_BUFFER:
            if (status->guest_array_buffer != status->host_array_buffer) {
                status->host_array_buffer = status->guest_array_buffer;
                new_buffer = status->host_array_buffer;
                need_sync = 1;
            }
            break;
        case GL_ELEMENT_ARRAY_BUFFER:
            if (status->guest_element_array_buffer !=
                status->host_element_array_buffer) {
                status->host_element_array_buffer =
                    status->guest_element_array_buffer;
                new_buffer = status->host_element_array_buffer;
                need_sync = 1;
            }
            printf(
                "@todo! buffer sync element array buffer (glDrawElements)!\n");
            break;
        case GL_COPY_READ_BUFFER:
            if (status->guest_copy_read_buffer !=
                status->host_copy_read_buffer) {
                status->host_copy_read_buffer = status->guest_copy_read_buffer;
                new_buffer = status->host_copy_read_buffer;
                need_sync = 1;
            }
            break;
        case GL_COPY_WRITE_BUFFER:
            if (status->guest_copy_write_buffer !=
                status->host_copy_write_buffer) {
                status->host_copy_write_buffer =
                    status->guest_copy_write_buffer;
                new_buffer = status->host_copy_write_buffer;
                need_sync = 1;
            }
            break;
        case GL_PIXEL_PACK_BUFFER:
            if (status->guest_pixel_pack_buffer !=
                status->host_pixel_pack_buffer) {
                status->host_pixel_pack_buffer =
                    status->guest_pixel_pack_buffer;
                new_buffer = status->host_pixel_pack_buffer;
                need_sync = 1;
            }
            break;
        case GL_PIXEL_UNPACK_BUFFER:
            if (status->guest_pixel_unpack_buffer !=
                status->host_pixel_unpack_buffer) {
                status->host_pixel_unpack_buffer =
                    status->guest_pixel_unpack_buffer;
                new_buffer = status->host_pixel_unpack_buffer;
                need_sync = 1;
            }
            break;
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            if (status->guest_transform_feedback_buffer !=
                status->host_transform_feedback_buffer) {
                status->host_transform_feedback_buffer =
                    status->guest_transform_feedback_buffer;
                new_buffer = status->host_transform_feedback_buffer;
                need_sync = 1;
            }
            break;
        case GL_UNIFORM_BUFFER:
            if (status->guest_uniform_buffer != status->host_uniform_buffer) {
                status->host_uniform_buffer = status->guest_uniform_buffer;
                new_buffer = status->host_uniform_buffer;
                need_sync = 1;
            }
            break;
        case GL_ATOMIC_COUNTER_BUFFER:
            if (status->guest_atomic_counter_buffer !=
                status->host_atomic_counter_buffer) {
                status->host_atomic_counter_buffer =
                    status->guest_atomic_counter_buffer;
                new_buffer = status->host_atomic_counter_buffer;
                need_sync = 1;
            }
            break;
        case GL_DISPATCH_INDIRECT_BUFFER:
            if (status->guest_dispatch_indirect_buffer !=
                status->host_dispatch_indirect_buffer) {
                status->host_dispatch_indirect_buffer =
                    status->guest_dispatch_indirect_buffer;
                new_buffer = status->host_dispatch_indirect_buffer;
                need_sync = 1;
            }
            break;
        case GL_DRAW_INDIRECT_BUFFER:
            if (status->guest_draw_indirect_buffer !=
                status->host_draw_indirect_buffer) {
                status->host_draw_indirect_buffer =
                    status->guest_draw_indirect_buffer;
                new_buffer = status->host_draw_indirect_buffer;
                need_sync = 1;
            }
            break;
        case GL_SHADER_STORAGE_BUFFER:
            if (status->guest_shader_storage_buffer !=
                status->host_shader_storage_buffer) {
                status->host_shader_storage_buffer =
                    status->guest_shader_storage_buffer;
                new_buffer = status->host_shader_storage_buffer;
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_BUFFER:
            if (status->guest_texture_buffer != status->host_texture_buffer) {
                status->host_texture_buffer = status->guest_texture_buffer;
                new_buffer = status->host_texture_buffer;
                need_sync = 1;
            }
            break;
    }
    if (need_sync != 0) {
        glBindBuffer(target, new_buffer);
    }
}
GLuint __load_shader(GLenum type, const char *shaderSrc) {
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);
    ALOGD("create shader");

    if (shader == 0) {
        return 0;
    }

    //        // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);
    ALOGD("shader source");

    // Compile the shader
    glCompileShader(shader);
    ALOGD("compile shader");

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    ALOGD("get shader iv %d", compiled);

    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        ALOGE("GL_INFO_LOG_LENGTH %d", infoLen);

        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            ALOGE("Error compiling shader:\n%s\n", infoLog);
            free(infoLog);
        }

        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint __load_program(const char *vertShaderSrc, const char *fragShaderSrc) {
    GLuint p = 0;

    GLuint vertexShader;
    GLuint fragmentShader;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = __load_shader(GL_VERTEX_SHADER, vertShaderSrc);

    if (vertexShader == 0) {
        return 0;
    }

    fragmentShader = __load_shader(GL_FRAGMENT_SHADER, fragShaderSrc);

    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return 0;
    }

    // Create the program object
    p = glCreateProgram();

    if (p == 0) {
        return GL_FALSE;
    }

    glAttachShader(p, vertexShader);
    glAttachShader(p, fragmentShader);

    // Link the program
    glLinkProgram(p);

    // Check the link status
    glGetProgramiv(p, GL_LINK_STATUS, &linked);

    ALOGD("linked %d program %u", linked, p);

    if (!linked) {
        GLint infoLen = 0;

        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &infoLen);

        ALOGE("GL_INFO_LOG_LENGTH %d", infoLen);

        if (infoLen > 1) {
            char *infoLog = (char *)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(p, infoLen, NULL, infoLog);
            ALOGE("Error linking program:\n%s\n", infoLog);
            free(infoLog);
        }

        glDeleteProgram(p);
        return 0;
    }

    // Free up no longer needed shader resources
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return p;
}
void DrawPrimitiveWithVBOs(GLint numVertices, GLfloat **vtxBuf,
                           GLint *vtxStrides, GLint numIndices,
                           GLushort *indices) {
    static GLuint vboIds[3];
    vboIds[0] = 0;
    vboIds[1] = 0;
    vboIds[2] = 0;
#define VERTEX_POS_SIZE 3    // x, y and z
#define VERTEX_COLOR_SIZE 4  // r, g, b, and a

#define VERTEX_POS_INDX 0
#define VERTEX_COLOR_INDX 1

    if (vboIds[0] == 0 && vboIds[1] == 0 && vboIds[2] == 0) {
        // Only allocate on the first draw
        glGenBuffers(3, vboIds);

        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, vtxStrides[0] * numVertices, vtxBuf[0],
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
        glBufferData(GL_ARRAY_BUFFER, vtxStrides[1] * numVertices, vtxBuf[1],
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices,
                     indices, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE,
                          vtxStrides[0], 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);
    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE, GL_FLOAT,
                          GL_FALSE, vtxStrides[1], 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_COLOR_INDX);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLState::d_glViewport_special(void *context, GLint x, GLint y,
                                   GLsizei width, GLsizei height) {
    GLContext *real_opengl_context = (GLContext *)context;

    const char vShaderStr[] =
        "#version 300 es                            \n"
        "layout(location = 0) in vec4 a_position;   \n"
        "layout(location = 1) in vec4 a_color;      \n"
        "out vec4 v_color;                          \n"
        "void main()                                \n"
        "{                                          \n"
        "    v_color = a_color;                     \n"
        "    gl_Position = a_position;              \n"
        "}";

    const char fShaderStr[] =
        "#version 300 es            \n"
        "precision mediump float;   \n"
        "in vec4 v_color;           \n"
        "out vec4 o_fragColor;      \n"
        "void main()                \n"
        "{                          \n"
        "    o_fragColor = v_color; \n"
        "}";

    // if(width < real_opengl_context->view_w && height <
    // real_opengl_context->view_h)
    // {
    //     real_opengl_context->view_x = x;
    //     real_opengl_context->view_y = real_opengl_context->view_h - height;
    //     real_opengl_context->view_w = width;
    //     real_opengl_context->view_h = height;
    //     glViewport(x, real_opengl_context->view_h - height, width, height);
    //     printf("context %llx glViewport change y %d w %d h %d\n",
    //     (uint64_t)context, real_opengl_context->view_y, width, height);

    //     return;
    // }

    //    GLuint p = __load_program(vShaderStr, fShaderStr);

    real_opengl_context->view_x = x;
    real_opengl_context->view_y = y;
    real_opengl_context->view_w = width;
    real_opengl_context->view_h = height;

    ALOGD("context %llx glViewport w %d h %d\n", (uint64_t)context, width,
          height);
    glViewport(x, y, width, height);

    //    GLfloat vertexPos[3 * VERTEX_POS_SIZE] = {
    //            0.0f,  0.5f,  0.0f, // v0
    //            -0.5f, -0.5f, 0.0f, // v1
    //            0.5f,  -0.5f, 0.0f  // v2
    //    };
    //    GLfloat color[4 * VERTEX_COLOR_SIZE] = {
    //            1.0f, 0.0f, 0.0f, 1.0f, // c0
    //            0.0f, 1.0f, 0.0f, 1.0f, // c1
    //            0.0f, 0.0f, 1.0f, 1.0f  // c2
    //    };
    //    GLint vtxStrides[2] = {VERTEX_POS_SIZE * sizeof(GLfloat),
    //                           VERTEX_COLOR_SIZE * sizeof(GLfloat)};
    //
    //    // Index buffer data
    //    GLushort indices[3] = {0, 1, 2};
    //    GLfloat *vtxBuf[2] = {vertexPos, color};
    //
    //    glViewport(0, 0, width, height);
    //    glClearColor(1,1,1,1);
    //    glClear(GL_COLOR_BUFFER_BIT);
    //    glUseProgram(GLResource::recorded_program);
    ////    glUseProgram(p);
    //
    //    DrawPrimitiveWithVBOs(3, vtxBuf, vtxStrides, 3, indices);
    //
    //    glFlush();
    //    MainRenderer::get_instance()->send_window_message(MAIN_PAINT,
    //    real_opengl_context->cur_draw_surface->gbuffer);

    //    static GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
    //                                  -0.5f, -0.5f,0.0f,
    //                                  0.5f, -0.5f, 0.0f};
    //    static unsigned int VBO, VAO, EBO;
    //    glGenVertexArrays(1, &VAO);
    //    glGenBuffers(1, &VBO);
    //    glGenBuffers(1, &EBO);
    //
    
    //    glBindVertexArray(VAO);
    //
    
    //    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices,
    //    GL_STATIC_DRAW);
    //
    
    //    glVertexAttribPointer(0 ,3, GL_FLOAT, GL_FALSE, 3* sizeof(float),
    //    nullptr); glEnableVertexAttribArray(0);
    //
    ////    glBindVertexArray(0);
    ////    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ////    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ////
    ////        // Clear the color buffer
    //
    //    static int t=1;
    //    if(t==1){
    //        t=2;
    //        glClearColor(0,1,0,0.5);
    //    }else{
    //        t=1;
    //        glClearColor(0,0,1,0.5);
    //    }
    //
    //    glClear(GL_COLOR_BUFFER_BIT);
    ////
    //    // Use the program object
    //    glUseProgram(GLResource::recorded_program);
    //
    ////    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    ////    glEnableVertexAttribArray(0);
    //
    //    glDrawArrays(GL_TRIANGLES, 0, 3);
    //    glFlush();
    //    MainRenderer::get_instance()->send_window_message(MAIN_PAINT,
    //    real_opengl_context->cur_draw_surface->gbuffer);
    return;
}

void GLState::d_glEGLImageTargetTexture2DOES(void *context, GLenum target,
                                             GLeglImageOES image) {
    
    return;
}

void GLState::d_glBindEGLImage(void *t_context, GLenum target, uint64_t image,
                               GLuint texture, GLuint share_texture,
                               EGLContext guest_share_ctx) {
    ThreadContext *thread_context = (ThreadContext *)t_context;

    ProcessContext *process_context = thread_context->process_context.get();
    GLContext *opengl_context = (GLContext *)thread_context->opengl_context;
    uint64_t gbuffer_id = (uint64_t)image;
    GraphicBuffer *gbuffer = NULL;

    GLuint host_share_texture;

    
    if (opengl_context == NULL) {
        ALOGE("error! opengl_context null when bindEGLImage image id %llx\n",
              gbuffer_id);
        return;
    }

    //    gbuffer = (GraphicBuffer
    //    *)g_hash_table_lookup(process_context->gbuffer_map,
    //    GUINT_TO_POINTER(gbuffer_id));
    gbuffer = process_context->gbuffer_map[gbuffer_id];
    if (gbuffer == NULL) {
        ALOGE(
            "error! glBindEGLImage with NULL gbuffer when finding in process "
            "gbuffer_id %llx type %d\n",
            gbuffer_id, gbuffer->usage_type);
        return;
    }

    // printf("glBindEGLImage gbuffer %llx ptr %llx type %d
    // target-texture(%d)\n",gbuffer->gbuffer_id, gbuffer, gbuffer->usage_type,
    // target == GL_TEXTURE_2D);

    if (gbuffer->usage_type == GBUFFER_TYPE_NATIVE) {
        GLResource::set_texture_gbuffer_ptr(opengl_context, texture, gbuffer);
        // printf("glBindEGLImage gbuffer_id %llx when write %d sync %d\n",
        // gbuffer_id, gbuffer->is_writing, gbuffer->data_sync);
        TextureBindingStatus *status =
            &(opengl_context->texture_binding_status);
        if (target == GL_TEXTURE_2D) {
            status->current_2D_gbuffer = gbuffer;
        } else {
            status->current_external_gbuffer = gbuffer;
        }
    }

    
    if (gbuffer->is_writing == 1) {
        int waiting_time = 1000 / MainRenderer::COMPOSER_REFRESH_RATE / 4 * 6;
        wait_event(gbuffer->writing_ok_event, waiting_time);
        ALOGD("glBindEGLImage gbuffer is writing(waiting end %d)\n",
              gbuffer->is_writing);
    }

    if (gbuffer->data_sync != 0) {
        if (gbuffer->delete_sync != 0) {
            glDeleteSync(gbuffer->delete_sync);
        }
        glWaitSync(gbuffer->data_sync, 0, GL_TIMEOUT_IGNORED);

        gbuffer->delete_sync = gbuffer->data_sync;
        gbuffer->data_sync = 0;

        // glClientWaitSync(gbuffer->data_sync, GL_SYNC_FLUSH_COMMANDS_BIT,
        // 1000000000);
    }

    host_share_texture = gbuffer->data_texture;

    
    unsigned int origin_texture = (int)GLResource::set_share_texture(
        opengl_context, texture, host_share_texture);
    if (origin_texture > 0) {
        glDeleteTextures(1, &origin_texture);
    }

    TextureBindingStatus *status = &(opengl_context->texture_binding_status);
    switch (target) {
        case GL_TEXTURE_2D:
            status->guest_current_texture_2D
                [status->guest_current_active_texture] = host_share_texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                status->host_current_texture_2D
                    [status->guest_current_active_texture] = host_share_texture;
            }
            break;
        case GL_TEXTURE_EXTERNAL_OES:
            status->current_texture_external = host_share_texture;
            break;
    }

    if (target != GL_TEXTURE_EXTERNAL_OES) {
        if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
            MainRenderer::DSA_ENABLE == 0) {
            glBindTexture(target, host_share_texture);
        }
        // else
        // {
        //     status->guest_current_texture_unit[status->guest_current_active_texture]
        //     = host_share_texture;
        // }
    }

    return;
}

void GLState::d_glEGLImageTargetRenderbufferStorageOES(void *context,
                                                       GLenum target,
                                                       GLeglImageOES image) {
    
}

void GLState::d_glBindTexture_special(void *context, GLenum target,
                                      GLuint guest_texture) {
    GLContext *opengl_context = (GLContext *)context;

    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    GLuint texture = (GLuint)GLResource::get_host_texture_id(
        opengl_context, (unsigned int)guest_texture);

    char is_init =
        GLResource::set_host_texture_init(opengl_context, guest_texture);

    ALOGD("context %llx target %x texture %u guest %d current %d\n",
          (uint64_t)opengl_context, target, texture, guest_texture,
          status->guest_current_active_texture);

    if (is_init == 0) {
        if (status->guest_current_active_texture !=
            status->host_current_active_texture) {
            status->host_current_active_texture =
                status->guest_current_active_texture;
            glActiveTexture(status->guest_current_active_texture + GL_TEXTURE0);
        }
    }

    switch (target) {
        case GL_TEXTURE_2D:
            status->guest_current_texture_2D
                [status->guest_current_active_texture] = texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0 || is_init == 0) {
                status->host_current_texture_2D
                    [status->host_current_active_texture] = texture;
            }
            if (is_init == 2) {
                status->current_2D_gbuffer =
                    GLResource::get_texture_gbuffer_ptr(context, guest_texture);
            } else {
                status->current_2D_gbuffer = NULL;
            }
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            status->guest_current_texture_2D_multisample
                [status->guest_current_active_texture] = texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0 || is_init == 0) {
                status->host_current_texture_2D_multisample
                    [status->host_current_active_texture] = texture;
            }
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            status->guest_current_texture_2D_multisample_array
                [status->guest_current_active_texture] = texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0 || is_init == 0) {
                status->host_current_texture_2D_multisample_array
                    [status->host_current_active_texture] = texture;
            }
            break;
        case GL_TEXTURE_3D:
            status->guest_current_texture_3D
                [status->guest_current_active_texture] = texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0 || is_init == 0) {
                status->host_current_texture_3D
                    [status->host_current_active_texture] = texture;
            }
            break;
        case GL_TEXTURE_2D_ARRAY:
            status->guest_current_texture_2D_array
                [status->guest_current_active_texture] = texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0 || is_init == 0) {
                status->host_current_texture_2D_array
                    [status->host_current_active_texture] = texture;
            }
            break;
        case GL_TEXTURE_CUBE_MAP:
            status->guest_current_texture_cube_map
                [status->guest_current_active_texture] = texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0 || is_init == 0) {
                status->host_current_texture_cube_map
                    [status->host_current_active_texture] = texture;
            }
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            status->guest_current_texture_cube_map_array
                [status->guest_current_active_texture] = texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0 || is_init == 0) {
                status->host_current_texture_cube_map_array
                    [status->host_current_active_texture] = texture;
            }
            break;
        case GL_TEXTURE_BUFFER:
            status->guest_current_texture_buffer
                [status->guest_current_active_texture] = texture;
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0 || is_init == 0) {
                status->host_current_texture_buffer
                    [status->host_current_active_texture] = texture;
            }
            break;
        case GL_TEXTURE_EXTERNAL_OES:
            status->current_texture_external = texture;
            if (is_init == 2) {
                status->current_external_gbuffer =
                    GLResource::get_texture_gbuffer_ptr(context, guest_texture);
            } else {
                status->current_external_gbuffer = NULL;
            }
            break;
        default:
            printf("error! glBindBuffer error target %x\n", target);
            break;
    }

    if (target != GL_TEXTURE_EXTERNAL_OES) {
        if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
            MainRenderer::DSA_ENABLE == 0) {
            // GLuint a = 0;
            // glGetIntegerv(GL_ACTIVE_TEXTURE, &a);
            // printf("context %llx bind texuture %d active %d\n",
            // (uint64_t)context, texture, a-GL_TEXTURE0);
            glBindTexture(target, texture);
        } else {
            if (is_init == 0) {
                glBindTexture(target, texture);
            }
            // status->guest_current_texture_unit[status->guest_current_active_texture]
            // = texture;

            // if(status->current_texture_unit[status->guest_current_active_texture]
            // != texture)
            // {
            
            //     glBindTextureUnit(status->guest_current_active_texture,
            //     texture);
            //     status->current_texture_unit[status->guest_current_active_texture]
            //     = texture;
            // }
        }
    }

    // if (target == GL_TEXTURE_2D)
    // {
    //     opengl_context->current_texture_2D[opengl_context->current_active_texture]
    //     = texture;
    // }
    // else if(target == GL_TEXTURE_EXTERNAL_OES)
    // {
    
    //     opengl_context->current_texture_external = texture;
    //     return;
    // }

    // glBindTexture(target, texture);
}

void GLState::texture_binding_status_sync(void *context, GLenum target) {
    GLContext *opengl_context = (GLContext *)context;

    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    GLuint current_active_texture = status->guest_current_active_texture;

    if (current_active_texture != status->host_current_active_texture) {
        printf("active texture sync host %d guest %d\n",
               status->host_current_active_texture, current_active_texture);
        status->host_current_active_texture = current_active_texture;
        glActiveTexture(current_active_texture + GL_TEXTURE0);
    }

    int need_sync = 0;
    GLuint new_texture = 0;

    // GLuint temp=0;
    switch (target) {
        case GL_TEXTURE_2D:
            // glGetIntegerv(GL_TEXTURE_BINDING_2D, &temp);
            // printf("sync texture current %d guest %d host %d real host
            // %d\n",current_active_texture,status->guest_current_texture_2D[current_active_texture],
            // status->host_current_texture_2D[current_active_texture], temp);
            if (status->guest_current_texture_2D[current_active_texture] !=
                status->host_current_texture_2D[current_active_texture]) {
                status->host_current_texture_2D[current_active_texture] =
                    status->guest_current_texture_2D[current_active_texture];
                new_texture =
                    status->guest_current_texture_2D[current_active_texture];
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            if (status->guest_current_texture_2D_multisample
                    [current_active_texture] !=
                status->host_current_texture_2D_multisample
                    [current_active_texture]) {
                status->host_current_texture_2D_multisample
                    [current_active_texture] =
                    status->guest_current_texture_2D_multisample
                        [current_active_texture];
                new_texture = status->guest_current_texture_2D_multisample
                                  [current_active_texture];
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            if (status->guest_current_texture_2D_multisample_array
                    [current_active_texture] !=
                status->host_current_texture_2D_multisample_array
                    [current_active_texture]) {
                status->host_current_texture_2D_multisample_array
                    [current_active_texture] =
                    status->guest_current_texture_2D_multisample_array
                        [current_active_texture];
                new_texture = status->guest_current_texture_2D_multisample_array
                                  [current_active_texture];
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_3D:
            if (status->guest_current_texture_3D[current_active_texture] !=
                status->host_current_texture_3D[current_active_texture]) {
                status->host_current_texture_3D[current_active_texture] =
                    status->guest_current_texture_3D[current_active_texture];
                new_texture =
                    status->guest_current_texture_3D[current_active_texture];
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_2D_ARRAY:
            if (status
                    ->guest_current_texture_2D_array[current_active_texture] !=
                status->host_current_texture_2D_array[current_active_texture]) {
                status->host_current_texture_2D_array[current_active_texture] =
                    status->guest_current_texture_2D_array
                        [current_active_texture];
                new_texture = status->guest_current_texture_2D_array
                                  [current_active_texture];
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        case GL_TEXTURE_CUBE_MAP:
            if (status
                    ->guest_current_texture_cube_map[current_active_texture] !=
                status->host_current_texture_cube_map[current_active_texture]) {
                target = GL_TEXTURE_CUBE_MAP;
                status->host_current_texture_cube_map[current_active_texture] =
                    status->guest_current_texture_cube_map
                        [current_active_texture];
                new_texture = status->guest_current_texture_cube_map
                                  [current_active_texture];
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            if (status->guest_current_texture_cube_map_array
                    [current_active_texture] !=
                status->host_current_texture_cube_map_array
                    [current_active_texture]) {
                status->host_current_texture_cube_map_array
                    [current_active_texture] =
                    status->guest_current_texture_cube_map_array
                        [current_active_texture];
                new_texture = status->guest_current_texture_cube_map_array
                                  [current_active_texture];
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_BUFFER:
            if (status->guest_current_texture_buffer[current_active_texture] !=
                status->host_current_texture_buffer[current_active_texture]) {
                status->host_current_texture_buffer[current_active_texture] =
                    status
                        ->guest_current_texture_buffer[current_active_texture];
                new_texture =
                    status
                        ->guest_current_texture_buffer[current_active_texture];
                need_sync = 1;
            }
            break;
        case GL_TEXTURE_EXTERNAL_OES:
            break;
        default:
            printf("error texture target %x need sync\n", target);
            break;
    }

    if (need_sync != 0) {
        glBindTexture(target, new_texture);
    }
}

//#define CHANGE_TEXTURE_UNIT_WITH_TYPE(status, type, index)                               \
//    {                                                                                    \
//        if (status->guest_current_texture_##type != status->host_current_texture_##type) \
//        {                                                                                \
//            status->host_current_texture_##type = status->guest_current_texture_##type;  \
//            glBindTextureUnit(index, status->guest_current_texture_##type);              \
//        }                                                                                \
//    }

#define CHANGE_TEXTURE_UNIT_WITH_TYPE(status, type, index)       \
    {                                                            \
        if (status->guest_current_texture_##type !=              \
            status->host_current_texture_##type) {               \
            status->host_current_texture_##type =                \
                status->guest_current_texture_##type;            \
            glActiveTexture(GL_TEXTURE0 + index);                \
            glBindTexture(GL_TEXTURE_2D,                         \
                          status->guest_current_texture_##type); \
        }                                                        \
    }

void GLState::texture_unit_status_sync(void *context, GLint index) {
    GLContext *opengl_context = (GLContext *)context;

    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (index < 0) {
        for (int i = 0; i <= status->now_max_texture_unit; i++) {
            CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D [i], i);
            CHANGE_TEXTURE_UNIT_WITH_TYPE(status, cube_map[i], i);
            CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 3D [i], i);
            CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D_array [i], i);
            CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D_multisample [i], i);
            CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D_multisample_array [i], i);
            CHANGE_TEXTURE_UNIT_WITH_TYPE(status, cube_map_array[i], i);
            CHANGE_TEXTURE_UNIT_WITH_TYPE(status, buffer[i], i);

            // if(status->host_current_texture_unit[i] !=
            // status->guest_current_texture_unit[i])
            // {
            //     GLuint compare_id = status->guest_current_texture_unit[i];
            //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D[i],compare_id);
            //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
            //     cube_map[i],compare_id);
            //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 3D[i],compare_id);
            //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
            //     2D_array[i],compare_id);
            //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
            //     2D_multisample[i],compare_id);
            //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
            //     2D_multisample_array[i],compare_id);
            //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
            //     cube_map_array[i],compare_id);
            //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status, buffer[i],compare_id);

            //     glBindTextureUnit(i, status->guest_current_texture_unit[i]);
            //     status->host_current_texture_unit[i] =
            //     status->guest_current_texture_unit[i];
            // }
        }
    } else {
        CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D [index], index);
        CHANGE_TEXTURE_UNIT_WITH_TYPE(status, cube_map[index], index);
        CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 3D [index], index);
        CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D_array [index], index);
        CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D_multisample [index], index);
        CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D_multisample_array [index],
                                      index);
        CHANGE_TEXTURE_UNIT_WITH_TYPE(status, cube_map_array[index], index);
        CHANGE_TEXTURE_UNIT_WITH_TYPE(status, buffer[index], index);

        // if(status->host_current_texture_unit[index] !=
        // status->guest_current_texture_unit[index])
        // {
        //     GLuint compare_id = status->guest_current_texture_unit[index];
        //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 2D[index],compare_id);
        //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
        //     cube_map[index],compare_id);
        //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status, 3D[index],compare_id);
        //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
        //     2D_array[index],compare_id);
        //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
        //     2D_multisample[index],compare_id);
        //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
        //     2D_multisample_array[index],compare_id);
        //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status,
        //     cube_map_array[index],compare_id);
        //     CHANGE_TEXTURE_UNIT_WITH_TYPE(status, buffer[index],compare_id);

        //     glBindTextureUnit(index,
        //     status->guest_current_texture_unit[index]);
        //     status->host_current_texture_unit[index] =
        //     status->guest_current_texture_unit[index];
        // }
    }
}

void GLState::d_glActiveTexture_special(void *context, GLenum texture) {
    GLContext *opengl_context = (GLContext *)context;

    if ((int)texture - GL_TEXTURE0 >= 0 &&
        texture - GL_TEXTURE0 <
            MainRenderer::get_instance()
                ->static_context_values->max_combined_texture_image_units) {
        TextureBindingStatus *status =
            &(opengl_context->texture_binding_status);
        status->guest_current_active_texture = texture - GL_TEXTURE0;

        if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
            MainRenderer::DSA_ENABLE == 0) {
            status->host_current_active_texture = texture - GL_TEXTURE0;
            glActiveTexture(texture);
        }

        if (texture - GL_TEXTURE0 > status->now_max_texture_unit) {
            status->now_max_texture_unit = texture - GL_TEXTURE0;
        }
        // GLuint tex = 0;
        // GLuint ac = 0;
        // glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
        // glGetIntegerv(GL_ACTIVE_TEXTURE, &ac);
        // printf("context %llx active texture %d tex %u active %d\n",
        // (uint64_t)context, status->host_current_active_texture, tex,
        // ac-GL_TEXTURE0);
    }

    // glActiveTexture(texture);
}

void GLState::active_texture_sync(void *context) {
    GLContext *opengl_context = (GLContext *)context;

    TextureBindingStatus *status = &(opengl_context->texture_binding_status);

    if (status->guest_current_active_texture !=
        status->host_current_active_texture) {
        status->host_current_active_texture =
            status->guest_current_active_texture;
        glActiveTexture(status->host_current_active_texture + GL_TEXTURE0);
    }
}

void GLState::d_glPixelStorei_origin(void *context, GLenum pname, GLint param) {
    // Pixel_Store_Status *pixel_store = &(((GLContext
    // *)context)->pixel_store_status); switch (pname)
    // {
    // case GL_UNPACK_ALIGNMENT:
    //     pixel_store->unpack_alignment = param;
    //     break;
    // case GL_PACK_ALIGNMENT:
    //     pixel_store->pack_alignment = param;
    //     break;
    // case GL_UNPACK_ROW_LENGTH:
    //     pixel_store->unpack_row_length = param;
    //     break;
    // case GL_UNPACK_IMAGE_HEIGHT:
    //     pixel_store->unpack_image_height = param;
    //     break;
    // case GL_UNPACK_SKIP_PIXELS:
    //     pixel_store->unpack_skip_pixels = param;
    //     break;
    // case GL_UNPACK_SKIP_ROWS:
    //     pixel_store->unpack_skip_rows = param;
    //     break;
    // case GL_UNPACK_SKIP_IMAGES:
    //     pixel_store->unpack_skip_images = param;
    //     break;
    // case GL_PACK_ROW_LENGTH:
    //     pixel_store->pack_row_length = param;
    //     break;
    // case GL_PACK_SKIP_PIXELS:
    //     pixel_store->pack_skip_pixels = param;
    //     break;
    // case GL_PACK_SKIP_ROWS:
    //     pixel_store->pack_skip_rows = param;
    //     break;
    // default:
    //     return;
    // }
    glPixelStorei(pname, param);
    // ALOGD("glPixelStorei %x %d\n",pname,param);

    return;
}

void GLState::d_glBindVertexArray_special(void *context, GLuint array) {
    GLContext *opengl_context = (GLContext *)context;

    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    // BufferStatus *vao_status = g_hash_table_lookup(bound_buffer->vao_status,
    // GUINT_TO_POINTER(array));

    // bound_buffer->buffer_status=vao_status;

    // buffer_binding_status_sync(context, GL_ELEMENT_ARRAY_BUFFER);

    GLuint pre_vao = status->guest_vao;

    GLuint now_vao =
        (GLuint)GLResource::get_host_array_id(context, (unsigned int)array);
    if (array == 0 && now_vao == 0) {
        now_vao = opengl_context->vao0;
    }

    //    AttribPoint *now_point =
    //    g_hash_table_lookup(bound_buffer->vao_point_data,
    //    GUINT_TO_POINTER(now_vao)); AttribPoint *pre_point =
    //    g_hash_table_lookup(bound_buffer->vao_point_data,
    //    GUINT_TO_POINTER(pre_vao));

    AttribPoint *now_point = bound_buffer->vao_point_data[now_vao];
    AttribPoint *pre_point = bound_buffer->vao_point_data[pre_vao];

    ALOGD("context %llx bind vao host %d guest %d pre %d\n", (uint64_t)context,
          now_vao, array, pre_vao);

    if (now_point == NULL) {
        //        now_point = g_hash_table_lookup(bound_buffer->vao_point_data,
        //        GUINT_TO_POINTER(0));
        now_point = bound_buffer->vao_point_data[0];
        ALOGD("error! vao %d cannot find\n", now_vao);
        return;
    }

    if (pre_point->element_array_buffer != status->guest_vao_ebo) {
        pre_point->element_array_buffer = status->guest_vao_ebo;
    }

    bound_buffer->attrib_point = now_point;

    status->guest_element_array_buffer = now_point->element_array_buffer;

    status->guest_vao_ebo = now_point->element_array_buffer;

    status->guest_vao = now_vao;

    if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
        MainRenderer::DSA_ENABLE == 0) {
        glBindVertexArray(now_vao);
    }
}

GLuint GLState::get_guest_binding_vao(void *context) {
    GLContext *opengl_context = (GLContext *)context;
    BufferStatus *status = &(opengl_context->bound_buffer_status.buffer_status);

    return (GLuint)status->guest_vao;
}

void GLState::vao_binding_status_sync(void *context) {
    GLContext *opengl_context = (GLContext *)context;
    BoundBuffer *bound_buffer = &(opengl_context->bound_buffer_status);

    BufferStatus *status = &(bound_buffer->buffer_status);

    // AttribPoint *point_data = bound_buffer->attrib_point;

    if (status->host_vao != status->guest_vao) {
        ALOGD("#%llx vao_binding_status_sync bind vao %d pre %d ebo %d\n",
              (uint64_t)opengl_context, status->guest_vao, status->host_vao,
              status->guest_vao_ebo);
        glBindVertexArray(status->guest_vao);

        status->host_vao = status->guest_vao;
        status->host_vao_ebo = status->guest_vao_ebo;

        status->guest_element_array_buffer = status->guest_vao_ebo;
        status->host_element_array_buffer = status->guest_vao_ebo;
    }
}
