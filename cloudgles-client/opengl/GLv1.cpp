#include "GLv1.h"

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

#include "Context.h"
#include "Log.h"
#include "State.h"
#include "Texture.h"
#include "renderer/MainRenderer.h"

#define LOG_TAG "GLv1"

GLuint GLv1::draw_texi_program = 0;
GLint GLv1::draw_texi_texture_id_loc = 0;

void GLv1::d_glTexEnvf_special(void *context, GLenum target, GLenum pname,
                               GLfloat param) {
    glTexEnvf(target, pname, param);
}

void GLv1::d_glTexEnvi_special(void *context, GLenum target, GLenum pname,
                               GLint param) {
    glTexEnvi(target, pname, param);
}

void GLv1::d_glTexEnvx_special(void *context, GLenum target, GLenum pname,
                               GLfixed param) {
    //    ALOGD("null glTexEnvxOES %llx %llx %llx pname %x param %x\n",
    //    (unsigned long long)glTexEnvxOES, (unsigned long long)glTexEnvi,
    //    (unsigned long long)glTexEnvf, pname, param);
    ALOGD("null glTexEnvxOES %llx %llx %llx pname %x param %x\n",
          (unsigned long long) 0, (unsigned long long) glTexEnvi,
          (unsigned long long) glTexEnvf, pname, param);
    glTexEnvi(target, pname, param);
}

void GLv1::d_glTexParameterx_special(void *context, GLenum target, GLenum pname,
                                     GLint param) {
    if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                   MainRenderer::DSA_ENABLE != 0)) {
        //        GLuint bind_texture = get_guest_binding_texture(context,
        //        target); glTextureParameteri(bind_texture, pname, param);
        ALOGE("DSA not supported.");
    } else {
        GLContext *opengl_context = (GLContext *) context;
        if (target == GL_TEXTURE_EXTERNAL_OES) {
            TextureBindingStatus *texture_status =
                    &(opengl_context->texture_binding_status);
            if (texture_status->host_current_active_texture != 0) {
                glActiveTexture(GL_TEXTURE0);
            }
            glBindTexture(GL_TEXTURE_2D,
                          texture_status->current_texture_external);
            glTexParameterx(GL_TEXTURE_2D, pname, param);
            glBindTexture(GL_TEXTURE_2D,
                          texture_status->host_current_texture_2D[0]);
            if (texture_status->host_current_active_texture != 0) {
                glActiveTexture(texture_status->host_current_active_texture +
                                GL_TEXTURE0);
            }
        } else {
            glTexParameteri(target, pname, param);
        }
    }
}

void GLv1::d_glShadeModel_special(void *context, GLenum mode) {
    // glShadeModel(mode);
}

void GLv1::d_glDrawTexiOES_special(void *context, GLint x, GLint y, GLint z,
                                   GLint width, GLint height, GLfloat left_x,
                                   GLfloat right_x, GLfloat bottom_y,
                                   GLfloat top_y) {
    GLContext *opengl_context = (GLContext *) context;
    GLuint pre_vbo;
    GLuint pre_vao;

    float fz = z >= 1 ? 1.0f : z;
    fz = z <= 0 ? 0.0f : z;
    fz = fz * 2.0f - 1.0f;

    float positions_tex_coord[] = {
            1.0f, 1.0f, fz,  // top right
            1.0f, -1.0f, fz,  // bottom right
            -1.0f, -1.0f, fz,  // bottom left
            -1.0f, 1.0f, fz,  // top left
            right_x, top_y,         // top right
            right_x, bottom_y,      // bottom right
            left_x, bottom_y,      // bottom left
            left_x, top_y,         // top left
    };

    unsigned int indices[] = {
            0, 1, 3,  // first triangle
            1, 2, 3   // second triangle
    };

    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint *) &pre_vbo);

    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint *) &pre_vao);

    glUseProgram(draw_texi_program);

    if (opengl_context->draw_texi_vao == 0) {
        glGenVertexArrays(1, &(opengl_context->draw_texi_vao));
        glBindVertexArray(opengl_context->draw_texi_vao);
        glGenBuffers(1, &(opengl_context->draw_texi_vbo));
        glGenBuffers(1, &(opengl_context->draw_texi_ebo));
        glBindBuffer(GL_ARRAY_BUFFER, opengl_context->draw_texi_vbo);
        glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), NULL,
                     GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                     opengl_context->draw_texi_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indices,
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
                              (const void *) (12 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // printf("glv1 draw texture %d x %d y %d z %d width %d height %d left_x
    // %f right_x %f bottom_y %f top_y
    // %f\n",opengl_context->current_texture_2D[opengl_context->current_active_texture],
    // x, y, z, width, height, left_x, right_x, bottom_y, top_y);

    GLint now_texture_target;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &now_texture_target);

    // GLuint now_bind_texture;
    // glGetIntegerv(GL_TEXTURE_BINDING_2D, &now_bind_texture);

    glUniform1i(draw_texi_texture_id_loc, now_texture_target - GL_TEXTURE0);
    glViewport(x, y, width, height);

    glBindVertexArray(opengl_context->draw_texi_vao);

    glBindBuffer(GL_ARRAY_BUFFER, opengl_context->draw_texi_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr) 0,
                    (GLsizeiptr) 20 * sizeof(float), positions_tex_coord);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glViewport(opengl_context->view_x, opengl_context->view_y,
               opengl_context->view_w, opengl_context->view_h);

    glUseProgram(0);
    glBindVertexArray(pre_vao);
    glBindBuffer(GL_ARRAY_BUFFER, pre_vbo);
}

void GLv1::prepare_draw_texi(void) {
    if (draw_texi_program == 0) {
        const char *vShaderCode =
                "#version 300 es\n"
                "layout(location = 0) in vec3 a_pos;\n"
                "layout(location = 1) in vec2 atex_coord;\n"
                "out vec2 tex_coord;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = vec4(a_pos, 1.0);\n"
                "    tex_coord = atex_coord;\n"
                "}\n";

        const char *fShaderCode =
                "#version 300 es\n"
                "precision mediump float;\n"
                "out vec4 frag_color;\n"
                "in vec2 tex_coord;\n"
                "uniform sampler2D texture_id;\n"
                "void main()\n"
                "{\n"
                "   frag_color = texture(texture_id, tex_coord);\n"
                "}\n";

        GLuint vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);

        // shader Program
        GLuint program_id = glCreateProgram();
        glAttachShader(program_id, vertex);
        glAttachShader(program_id, fragment);

        glLinkProgram(program_id);

        draw_texi_texture_id_loc =
                glGetUniformLocation(program_id, "texture_id");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        draw_texi_program = program_id;

        GLint linked;
        glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
        ALOGD("linked %d program %u\n", linked, program_id);
        if (!linked) {
            GLint infoLen = 0;
            glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &infoLen);
            printf("GL_INFO_LOG_LENGTH %d\n", infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetProgramInfoLog(program_id, infoLen, NULL, infoLog);
                printf("Error linking program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
    }
}