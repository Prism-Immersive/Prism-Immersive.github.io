#include "GLv1.h"

#include "GLDef.h"
#include "ThreadContext.h"

void d_glDrawTexiOES_special(void *context, GLint x, GLint y, GLint z,
                             GLint width, GLint height) {
    BindingStatus *binding_status =
        &(((GLThreadContext *)context)->context_binding_status);

    GLuint now_texture =
        binding_status->texture_binding[binding_status->active_texture]
            .texture_binding_2d;
    TextureInfo *info = NULL;
    if (((GLThreadContext *)context)->texture_info_map->find(now_texture) !=
        ((GLThreadContext *)context)->texture_info_map->end()) {
        info = ((GLThreadContext *)context)->texture_info_map[0][now_texture];
    } else {
        return;
    }

    
    if (info->crop_rect[0] == 0 && info->crop_rect[1] == 0 &&
        info->crop_rect[2] == 0 && info->crop_rect[3] == 0) {
        return;
    }

    GLsizei texture_width = info->width[0];
    GLsizei texture_height = info->height[0];

    GLfloat left_x, right_x, bottom_y, top_y;

    left_x = info->crop_rect[0] * 1.0f / texture_width;
    right_x = (info->crop_rect[0] + info->crop_rect[2]) * 1.0f / texture_width;

    bottom_y = info->crop_rect[1] * 1.0f / texture_height;
    top_y = (info->crop_rect[1] + info->crop_rect[3]) * 1.0f / texture_height;

    r_glDrawTexiOES(context, x, y, z, width, height, left_x, right_x, bottom_y,
                    top_y);
}

void d_glGetTexParameterxvOES_special(void *context, GLenum target,
                                      GLenum pname, GLfixed *params) {
    if (pname == GL_TEXTURE_CROP_RECT_OES) {
        BindingStatus *binding_status =
            &(((GLThreadContext *)context)->context_binding_status);

        GLuint now_texture =
            binding_status->texture_binding[binding_status->active_texture]
                .texture_binding_2d;
        TextureInfo *info = NULL;
        if (((GLThreadContext *)context)->texture_info_map->find(now_texture) !=
            ((GLThreadContext *)context)->texture_info_map->end()) {
            info =
                ((GLThreadContext *)context)->texture_info_map[0][now_texture];
        } else {
            return;
        }
        params[0] = (GLint)info->crop_rect[0];
        params[1] = (GLint)info->crop_rect[1];
        params[2] = (GLint)info->crop_rect[2];
        params[3] = (GLint)info->crop_rect[3];
    } else {
        r_glGetTexParameteriv(context, target, pname, params);
        //        ALOGE("%s get remote_controller target %x pname %x param %d",
        //        __FUNCTION__, target, pname, *params);
    }
}