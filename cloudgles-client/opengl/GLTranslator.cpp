#define LOG_TAG "GLTranslator"
#define LOG_LEVEL LOG_LEVEL_ERROR

#include "GLTranslator.h"

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

#include "GLFuncID.h"
#include "GLv1.h"
#include "Helper.h"
#include "Log.h"
#include "Memory.h"
#include "Program.h"
#include "Resource.h"
#include "State.h"
#include "Texture.h"
#include "Utils.h"
#include "Vertex.h"
#include "egl/Surface.h"
#include "renderer/Event.h"
#include "renderer/MainRenderer.h"
#include "renderer/RenderThread.h"

void GLTranslator::translate(ThreadContext *thread_context,
                             CompressGLCall *call) {
    
    

    // void* glMapBufferRange GLenum target, GLintptr offset, GLsizeiptr length,
    // GLbitfield access
    
    // length, GLbitfield access void *ret
    
    //  GLenum target, GLsizeiptr length, void* unmap_buf
    

    
    

    // glGetDriverControlStringQCOM GLuint driverControl, GLsizei bufSize,
    // GLsizei *length#sizeof(GLsizei), GLchar *driverControlString
    // glExtGetProgramBinarySourceQCOM GLuint program, GLenum shadertype, GLchar
    // *source, GLint *length

    // glEGLImageTargetRenderbufferStorageOES

    GLContext *opengl_context = thread_context->opengl_context;

    EglSurface *draw_surface = thread_context->cur_draw_surface;
    if (likely(draw_surface != nullptr)) {
        if (unlikely(draw_surface->type == WINDOW_SURFACE &&
                     draw_surface->gbuffer != nullptr &&
                     draw_surface->gbuffer->is_writing == 0)) {
            draw_surface->gbuffer->is_writing = 1;
            reset_event(draw_surface->gbuffer->writing_ok_event);
        }
    }

    ALOGD("Thread %" PRIu64 ": %s has been called", thread_context->remote_tid,
          get_function_name(call->call_id).c_str());

    unsigned char ret_local_buf[1024 * 4];

    switch (call->call_id) {
            /******* file '1-1' *******/

        case FUNID_glClientWaitSync: {
            /* readline: "GLenum glClientWaitSync GLsync sync, GLbitfield flags,
             * GLuint64 timeout" */
            /* func name: "glClientWaitSync" */
            /* args: [{'type': 'GLsync', 'name': 'sync', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLbitfield', 'name':
             * 'flags', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint64', 'name': 'timeout', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "GLenum" */
            /* type: "110" */

            /* Define variables */
            GLsync sync;
            GLbitfield flags;
            GLuint64 timeout;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClientWaitSync)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sync = *(GLsync *)(temp + temp_loc);
            temp_loc += 8;

            flags = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            timeout = *(GLuint64 *)(temp + temp_loc);
            temp_loc += 8;

            GLenum ret = glClientWaitSync(
                (GLsync)GLResource::get_host_sync_id(
                    opengl_context, (unsigned int)(uint64_t)sync),
                flags, timeout);

            ALOGD("glClientWaitSync context %llx guest %u host %llu ret %x\n",
                  (uint64_t)opengl_context, (unsigned int)(uint64_t)sync,
                  (uint64_t)GLResource::get_host_sync_id(
                      opengl_context, (unsigned int)(uint64_t)sync),
                  ret);

            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(GLenum));
        } break;

        case FUNID_glTestInt1: {
            /* readline: "GLint glTestInt1 GLint a, GLuint b" */
            /* func name: "glTestInt1" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'b', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "GLint" */
            /* type: "110" */

            /* Define variables */
            GLint a;
            GLuint b;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestInt1)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            GLint ret = GLHelper::glTestInt1(a, b);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(GLint));
        } break;

        case FUNID_glTestInt2: {
            /* readline: "GLuint glTestInt2 GLint a, GLuint b" */
            /* func name: "glTestInt2" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'b', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "GLuint" */
            /* type: "110" */

            /* Define variables */
            GLint a;
            GLuint b;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestInt2)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            GLuint ret = GLHelper::glTestInt2(a, b);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(GLuint));
        } break;

        case FUNID_glTestInt3: {
            /* readline: "GLint64 glTestInt3 GLint64 a, GLuint64 b" */
            /* func name: "glTestInt3" */
            /* args: [{'type': 'GLint64', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint64', 'name':
             * 'b', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "GLint64" */
            /* type: "110" */

            /* Define variables */
            GLint64 a;
            GLuint64 b;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestInt3)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint64 *)(temp + temp_loc);
            temp_loc += 8;

            b = *(GLuint64 *)(temp + temp_loc);
            temp_loc += 8;

            GLint64 ret = GLHelper::glTestInt3(a, b);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(GLint64));
        } break;

        case FUNID_glTestInt4: {
            /* readline: "GLuint64 glTestInt4 GLint64 a, GLuint64 b" */
            /* func name: "glTestInt4" */
            /* args: [{'type': 'GLint64', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint64', 'name':
             * 'b', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "GLuint64" */
            /* type: "110" */

            /* Define variables */
            GLint64 a;
            GLuint64 b;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestInt4)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint64 *)(temp + temp_loc);
            temp_loc += 8;

            b = *(GLuint64 *)(temp + temp_loc);
            temp_loc += 8;

            GLuint64 ret = GLHelper::glTestInt4(a, b);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(GLuint64));
        } break;

        case FUNID_glTestInt5: {
            /* readline: "GLfloat glTestInt5 GLint a, GLuint b" */
            /* func name: "glTestInt5" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'b', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "GLfloat" */
            /* type: "110" */

            /* Define variables */
            GLint a;
            GLuint b;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestInt5)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            GLfloat ret = GLHelper::glTestInt5(a, b);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(GLfloat));
        } break;

        case FUNID_glTestInt6: {
            /* readline: "GLdouble glTestInt6 GLint a, GLuint b" */
            /* func name: "glTestInt6" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'b', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "GLdouble" */
            /* type: "110" */

            /* Define variables */
            GLint a;
            GLuint b;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestInt6)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            GLdouble ret = GLHelper::glTestInt6(a, b);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(GLdouble));
        } break;

        case FUNID_glTestPointer1: {
            /* readline: "glTestPointer1 GLint a, const GLint
             * *b#sizeof(GLint)*10" */
            /* func name: "glTestPointer1" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLint*',
             * 'name': 'b', 'ptr': 'in', 'ptr_len': 'sizeof(GLint)*10', 'loc':
             * 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "110" */

            /* Define variables */
            GLint a;
            GLint *b;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestPointer1)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            b = (GLint *)call->paras[0].para_ptr;

            GLHelper::glTestPointer1(a, b);
        } break;

        case FUNID_glTestPointer2: {
            /* readline: "glTestPointer2 GLint a, const GLint
             * *b#sizeof(GLint)*10, GLint *c#sizeof(GLint)*10" */
            /* func name: "glTestPointer2" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLint*',
             * 'name': 'b', 'ptr': 'in', 'ptr_len': 'sizeof(GLint)*10', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'c', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLint)*10', 'loc': 2, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "110" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLint a;
            GLint *b;
            GLint *c;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestPointer2)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            b = (GLint *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLint) * 10;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            c = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint) * 10;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLHelper::glTestPointer2(a, b, c);
            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glTestPointer4: {
            /* readline: "GLint glTestPointer4 GLint a, const GLint
             * *b#sizeof(GLint)*1000, GLint *c#sizeof(GLint)*1000" */
            /* func name: "glTestPointer4" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLint*',
             * 'name': 'b', 'ptr': 'in', 'ptr_len': 'sizeof(GLint)*1000', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'c', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLint)*1000', 'loc': 2, 'ptr_ptr':
             * False}] */
            /* ret: "GLint" */
            /* type: "110" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLint a;
            GLint *b;
            GLint *c;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestPointer4)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            b = (GLint *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLint) * 1000 + sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            c = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint) * 1000;

            GLint *ret_ptr = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            ALOGD("glTestPointer4 %d,%d\n", a, *b);
            for (int i = 0; i < 1000; i++) {
                c[i] = b[i];
            }
            GLint ret = GLHelper::glTestPointer4(a, b, c);
            *ret_ptr = ret;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glTestString: {
            /* readline: "glTestString GLint a, GLint count, const GLchar
             * *const*strings#count|strlen(strings[i])+1, GLint buf_len, GLchar
             * *char_buf#buf_len" */
            /* func name: "glTestString" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name':
             * 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'const GLchar*', 'name': 'strings', 'ptr': 'in',
             * 'ptr_len': 'count|strlen(strings[i])+1', 'loc': 2, 'ptr_ptr':
             * True}, {'type': 'GLint', 'name': 'buf_len', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLchar*',
             * 'name': 'char_buf', 'ptr': 'out', 'ptr_len': 'buf_len', 'loc': 4,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "110" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLint a;
            GLint count;
            GLint buf_len;
            GLchar **strings;
            GLchar *char_buf;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestString)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            strings = (GLchar **)malloc(count * sizeof(const GLchar *));

            for (int i = 0; i < count; i++) {
                strings[i] = (GLchar *)call->paras[i].para_ptr;
            }

            int out_buf_len = buf_len;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }

            int out_buf_loc = 0;
            char_buf = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += buf_len;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLHelper::glTestString(a, count, (const GLchar *const *)strings,
                                   buf_len, char_buf);
            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            delete[] strings;
            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

            /******* end of file '1-1', 12/11 functions*******/

            /******* file '1-1-1' *******/

        case FUNID_glIsBuffer: {
            /* readline: "GLboolean glIsBuffer GLuint buffer" */
            /* func name: "glIsBuffer" */
            /* args: [{'type': 'GLuint', 'name': 'buffer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint buffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsBuffer)) {
                break;
            }
        } break;

        case FUNID_glIsEnabled: {
            /* readline: "GLboolean glIsEnabled GLenum cap" */
            /* func name: "glIsEnabled" */
            /* args: [{'type': 'GLenum', 'name': 'cap', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLenum cap;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsEnabled)) {
                break;
            }
        } break;

        case FUNID_glIsFramebuffer: {
            /* readline: "GLboolean glIsFramebuffer GLuint framebuffer" */
            /* func name: "glIsFramebuffer" */
            /* args: [{'type': 'GLuint', 'name': 'framebuffer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint framebuffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsFramebuffer)) {
                break;
            }
        } break;

        case FUNID_glIsProgram: {
            /* readline: "GLboolean glIsProgram GLuint program" */
            /* func name: "glIsProgram" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsProgram)) {
                break;
            }
        } break;

        case FUNID_glIsRenderbuffer: {
            /* readline: "GLboolean glIsRenderbuffer GLuint renderbuffer" */
            /* func name: "glIsRenderbuffer" */
            /* args: [{'type': 'GLuint', 'name': 'renderbuffer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint renderbuffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsRenderbuffer)) {
                break;
            }
        } break;

        case FUNID_glIsShader: {
            /* readline: "GLboolean glIsShader GLuint shader" */
            /* func name: "glIsShader" */
            /* args: [{'type': 'GLuint', 'name': 'shader', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint shader;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsShader)) {
                break;
            }
        } break;

        case FUNID_glIsTexture: {
            /* readline: "GLboolean glIsTexture GLuint texture" */
            /* func name: "glIsTexture" */
            /* args: [{'type': 'GLuint', 'name': 'texture', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint texture;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsTexture)) {
                break;
            }
        } break;

        case FUNID_glIsQuery: {
            /* readline: "GLboolean glIsQuery GLuint id" */
            /* func name: "glIsQuery" */
            /* args: [{'type': 'GLuint', 'name': 'id', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsQuery)) {
                break;
            }
        } break;

        case FUNID_glIsVertexArray: {
            /* readline: "GLboolean glIsVertexArray GLuint array" */
            /* func name: "glIsVertexArray" */
            /* args: [{'type': 'GLuint', 'name': 'array', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint array;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsVertexArray)) {
                break;
            }
        } break;

        case FUNID_glIsSampler: {
            /* readline: "GLboolean glIsSampler GLuint sampler" */
            /* func name: "glIsSampler" */
            /* args: [{'type': 'GLuint', 'name': 'sampler', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint sampler;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsSampler)) {
                break;
            }
        } break;

        case FUNID_glIsTransformFeedback: {
            /* readline: "GLboolean glIsTransformFeedback GLuint id" */
            /* func name: "glIsTransformFeedback" */
            /* args: [{'type': 'GLuint', 'name': 'id', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLuint id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsTransformFeedback)) {
                break;
            }
        } break;

        case FUNID_glIsSync: {
            /* readline: "GLboolean glIsSync GLsync sync" */
            /* func name: "glIsSync" */
            /* args: [{'type': 'GLsync', 'name': 'sync', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLboolean" */
            /* type: "111" */

            /* Define variables */
            GLsync sync;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glIsSync)) {
                break;
            }
        } break;

            /******* end of file '1-1-1', 13/23 functions*******/

            /******* file '1-1-2' *******/

        case FUNID_glGetError: {
            /* readline: "GLenum glGetError void" */
            /* func name: "glGetError" */
            /* args: [] */
            /* ret: "GLenum" */
            /* type: "112" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetError)) {
                break;
            }

            int out_buf_len = sizeof(GLenum);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLenum *ret_ptr = (GLenum *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLenum);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLenum ret = glGetError();
            *ret_ptr = ret;
            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetString_special: {
            /* readline: "glGetString_special GLenum name, GLubyte *buffer#1024"
             */
            /* func name: "glGetString_special" */
            /* args: [{'type': 'GLenum', 'name': 'name', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLubyte*', 'name':
             * 'buffer', 'ptr': 'out', 'ptr_len': '1024', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum name;
            GLubyte *buffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetString_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            name = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = 1024;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            buffer = (GLubyte *)(ret_buf + out_buf_loc);
            out_buf_loc += 1024;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLContext::d_glGetString_special(opengl_context, name, buffer);
            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetStringi_special: {
            /* readline: "glGetStringi_special GLenum name, GLuint index,
             * GLubyte *buffer#1024" */
            /* func name: "glGetStringi_special" */
            /* args: [{'type': 'GLenum', 'name': 'name', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLubyte*', 'name': 'buffer', 'ptr': 'out',
             * 'ptr_len': '1024', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum name;
            GLuint index;
            GLubyte *buffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetStringi_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            name = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = 1024;
            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            buffer = (GLubyte *)(ret_buf + out_buf_loc);
            out_buf_loc += 1024;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLContext::d_glGetStringi_special(opengl_context, name, index,
                                              buffer);
            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glCheckFramebufferStatus: {
            /* readline: "GLenum glCheckFramebufferStatus GLenum target
             * @{if(target!=GL_DRAW_FRAMEBUFFER&&target!=
             * GL_READ_FRAMEBUFFER&&target!=GL_FRAMEBUFFER){set_gl_error(context,GL_INVALID_ENUM);return
             * 0;}}" */
            /* func name: "glCheckFramebufferStatus" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "GLenum" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum *ret_ptr;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCheckFramebufferStatus)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLenum);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            ret_ptr = (GLenum *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLenum);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLenum ret = glCheckFramebufferStatus(target);
            *ret_ptr = ret;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glQueryMatrixxOES: {
            /* readline: "GLbitfield glQueryMatrixxOES GLfixed
             * *mantissa#sizeof(GLint)*16, GLint *exponent#sizeof(GLint)*16" */
            /* func name: "glQueryMatrixxOES" */
            /* args: [{'type': 'GLfixed*', 'name': 'mantissa', 'ptr': 'out',
             * 'ptr_len': 'sizeof(GLint)*16', 'loc': 0, 'ptr_ptr': False},
             * {'type': 'GLint*', 'name': 'exponent', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLint)*16', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "GLbitfield" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLfixed *mantissa;
            GLint *exponent;
            GLbitfield *ret_ptr;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glQueryMatrixxOES)) {
                break;
            }

            int out_buf_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            mantissa = (GLfixed *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint) * 16;

            exponent = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint) * 16;

            ret_ptr = (GLbitfield *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLbitfield);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }
            ALOGE("GLbitfield ret = glQueryMatrixxOES(mantissa, exponent);");
            //            GLbitfield ret = glQueryMatrixxOES(mantissa,
            //            exponent);
            *ret_ptr = 0;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetFramebufferAttachmentParameteriv: {
            /* readline: "glGetFramebufferAttachmentParameteriv GLenum target,
             * GLenum attachment, GLenum pname, GLint *params#sizeof(GLint)" */
            /* func name: "glGetFramebufferAttachmentParameteriv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'attachment', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'pname', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint*', 'name': 'params', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLint)', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum attachment;
            GLenum pname;
            GLint *params;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glGetFramebufferAttachmentParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            attachment = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetFramebufferAttachmentParameteriv(target, attachment, pname,
                                                  params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramInfoLog: {
            /* readline: "glGetProgramInfoLog GLuint program, GLsizei bufSize,
             * GLsizei *length#sizeof(GLsizei), GLchar *infoLog#bufSize
             * @{if(bufSize<0){ set_gl_error(context,GL_INVALID_VALUE); return;
             * }}" */
            /* func name: "glGetProgramInfoLog" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'bufSize', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei*', 'name': 'length', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLsizei)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLchar*', 'name': 'infoLog', 'ptr': 'out', 'ptr_len':
             * 'bufSize', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLsizei bufSize;
            GLsizei *length;
            GLchar *infoLog;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramInfoLog)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + bufSize;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            infoLog = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetProgramInfoLog((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                bufSize, length, infoLog);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetRenderbufferParameteriv: {
            /* readline: "glGetRenderbufferParameteriv GLenum target, GLenum
             * pname, GLint *params#sizeof(GLint)" */
            /* func name: "glGetRenderbufferParameteriv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLint)', 'loc': 2, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;
            GLint *params;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glGetRenderbufferParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetRenderbufferParameteriv(target, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetShaderInfoLog: {
            /* readline: "glGetShaderInfoLog GLuint shader, GLsizei bufSize,
             * GLsizei *length#sizeof(GLsizei), GLchar *infoLog#bufSize
             * @{if(bufSize<0){ set_gl_error(context,GL_INVALID_VALUE); return;
             * }}" */
            /* func name: "glGetShaderInfoLog" */
            /* args: [{'type': 'GLuint', 'name': 'shader', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'bufSize', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei*', 'name': 'length', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLsizei)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLchar*', 'name': 'infoLog', 'ptr': 'out', 'ptr_len':
             * 'bufSize', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint shader;
            GLsizei bufSize;
            GLsizei *length;
            GLchar *infoLog;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetShaderInfoLog)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + bufSize;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            infoLog = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetShaderInfoLog((GLuint)GLResource::get_host_shader_id(
                                   opengl_context, (unsigned int)shader),
                               bufSize, length, infoLog);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetShaderPrecisionFormat: {
            /* readline: "glGetShaderPrecisionFormat GLenum shadertype, GLenum
             * precisiontype, GLint *range#2*sizeof(GLint), GLint
             * *precision#sizeof(GLint)" */
            /* func name: "glGetShaderPrecisionFormat" */
            /* args: [{'type': 'GLenum', 'name': 'shadertype', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'precisiontype', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'range', 'ptr':
             * 'out', 'ptr_len': '2*sizeof(GLint)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLint*', 'name': 'precision', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLint)', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLenum shadertype;
            GLenum precisiontype;
            GLint *range;
            GLint *precision;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetShaderPrecisionFormat)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            shadertype = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            precisiontype = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = 2 * sizeof(GLint) + sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            range = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += 2 * sizeof(GLint);

            precision = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetShaderPrecisionFormat(shadertype, precisiontype, range,
                                       precision);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetShaderSource: {
            /* readline: "glGetShaderSource GLuint shader, GLsizei bufSize,
             * GLsizei *length#sizeof(GLsizei), GLchar *source#bufSize
             * @{if(bufSize<0){ set_gl_error(context,GL_INVALID_VALUE); return;
             * }}" */
            /* func name: "glGetShaderSource" */
            /* args: [{'type': 'GLuint', 'name': 'shader', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'bufSize', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei*', 'name': 'length', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLsizei)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLchar*', 'name': 'source', 'ptr': 'out', 'ptr_len':
             * 'bufSize', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint shader;
            GLsizei bufSize;
            GLsizei *length;
            GLchar *source;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetShaderSource)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + bufSize;

            unsigned char *ret_buf = nullptr;

            if ((out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            source = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetShaderSource((GLuint)GLResource::get_host_shader_id(
                                  opengl_context, (unsigned int)shader),
                              bufSize, length, source);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if ((out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetTexParameterfv: {
            /* readline: "glGetTexParameterfv GLenum target, GLenum pname,
             * GLfloat *params#gl_pname_size(pname)*sizeof(GLfloat)" */
            /* func name: "glGetTexParameterfv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfloat)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetTexParameterfv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfloat *params = (GLfloat *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            // TODO: Need Check HOST_OPENGL_VERSION
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                if (target == GL_TEXTURE_EXTERNAL_OES) {
                    TextureBindingStatus *texture_status =
                        &(opengl_context->texture_binding_status);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(GL_TEXTURE0);
                    }
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->current_texture_external);
                    glGetTexParameterfv(GL_TEXTURE_2D, pname, params);
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->host_current_texture_2D[0]);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(
                            texture_status->host_current_active_texture +
                            GL_TEXTURE0);
                    }
                } else {
                    glGetTexParameterfv(target, pname, params);
                }
            } else {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                glGetTexParameterfv(bind_texture, pname, params);
                ALOGE("DSA not supported.");
            }

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetTexParameteriv: {
            /* readline: "glGetTexParameteriv GLenum target, GLenum pname, GLint
             * *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetTexParameteriv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetTexParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            // TODO: Need Check HOST_OPENGL_VERSION
            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                if (target == GL_TEXTURE_EXTERNAL_OES) {
                    TextureBindingStatus *texture_status =
                        &(opengl_context->texture_binding_status);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(GL_TEXTURE0);
                    }
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->current_texture_external);
                    glGetTexParameteriv(GL_TEXTURE_2D, pname, params);
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->host_current_texture_2D[0]);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(
                            texture_status->host_current_active_texture +
                            GL_TEXTURE0);
                    }
                } else {
                    glGetTexParameteriv(target, pname, params);
                }
            } else {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                glGetTexParameteriv(bind_texture, pname, params);

                ALOGE("DSA not supported.");
            }

            // ALOGD("glGetTexParameteriv pname %x params %d\n",pname, *params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetQueryiv: {
            /* readline: "glGetQueryiv GLenum target, GLenum pname, GLint
             * *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetQueryiv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetQueryiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetQueryiv(target, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetQueryObjectuiv: {
            /* readline: "glGetQueryObjectuiv GLuint id, GLenum pname, GLuint
             * *params#gl_pname_size(pname)*sizeof(GLuint)" */
            /* func name: "glGetQueryObjectuiv" */
            /* args: [{'type': 'GLuint', 'name': 'id', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint*', 'name': 'params', 'ptr': 'out',
             * 'ptr_len': 'gl_pname_size(pname)*sizeof(GLuint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint id;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetQueryObjectuiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            id = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLuint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLuint *params = (GLuint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLuint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetQueryObjectuiv((GLuint)GLResource::get_host_query_id(
                                    opengl_context, (unsigned int)id),
                                pname, params);
            // ALOGD("glGetQueryObjectuiv %llx %d -> %d r_context
            // %llx\n",(uint64_t)opengl_context, id,
            // (GLuint)GLResource::get_host_query_id(opengl_context, (unsigned
            // int)id), r_context);

            // if(glGetError()!=GL_NO_ERROR)
            // {
            //     ALOGD("%llx error get query id %d host %d is(%d) pname
            //     %x\n",opengl_context ,id,
            //     (GLuint)GLResource::get_host_query_id(opengl_context,
            //     (unsigned int)id),
            //     (int)glIsQuery((GLuint)GLResource::get_host_query_id(opengl_context,
            //     (unsigned int)id)), pname);
            // }

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetTransformFeedbackVarying: {
            /* readline: "glGetTransformFeedbackVarying GLuint program, GLuint
             * index, GLsizei bufSize, GLsizei *length#sizeof(GLsizei), GLsizei
             * *size#sizeof(GLsizei), GLenum *type#sizeof(GLenum), GLchar
             * *name#bufSize @{if(bufSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGetTransformFeedbackVarying" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'bufSize', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei*', 'name': 'length', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLsizei)', 'loc': 3, 'ptr_ptr': False}, {'type':
             * 'GLsizei*', 'name': 'size', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLsizei)', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLenum*', 'name': 'type', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLenum)', 'loc': 5, 'ptr_ptr': False}, {'type':
             * 'GLchar*', 'name': 'name', 'ptr': 'out', 'ptr_len': 'bufSize',
             * 'loc': 6, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLuint index;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glGetTransformFeedbackVarying)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len =
                sizeof(GLsizei) + sizeof(GLsizei) + sizeof(GLenum) + bufSize;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLsizei *size = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLenum *type = (GLenum *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLenum);

            GLchar *name = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetTransformFeedbackVarying(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                index, bufSize, length, size, type, name);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetActiveUniformsiv: {
            /* readline: "glGetActiveUniformsiv GLuint program, GLsizei
             * uniformCount, const GLuint
             * *uniformIndices#uniformCount*sizeof(GLuint), GLenum pname, GLint
             * *params#uniformCount*sizeof(GLint)" */
            /* func name: "glGetActiveUniformsiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'uniformCount', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLuint*', 'name':
             * 'uniformIndices', 'ptr': 'in', 'ptr_len':
             * 'uniformCount*sizeof(GLuint)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLenum', 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params',
             * 'ptr': 'out', 'ptr_len': 'uniformCount*sizeof(GLint)', 'loc': 4,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLsizei uniformCount;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetActiveUniformsiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            uniformCount = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            int uniformIndices_flag = 0;
            int uniformIndices_null_flag = 0;
            GLuint *uniformIndices = (GLuint *)call->paras[0].para_ptr;

            int out_buf_len = uniformCount * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += uniformCount * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetActiveUniformsiv((GLuint)GLResource::get_host_program_id(
                                      opengl_context, (unsigned int)program),
                                  uniformCount, uniformIndices, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetActiveUniformBlockiv: {
            /* readline: "glGetActiveUniformBlockiv GLuint program, GLuint
             * uniformBlockIndex, GLenum pname, GLuint param_len, GLint
             * *params#param_len" */
            /* func name: "glGetActiveUniformBlockiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'uniformBlockIndex', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'pname', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'size_t', 'name': 'param_len', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params',
             * 'ptr': 'out', 'ptr_len':
             * 'gl_get_uniform_block_para_size(context,program,uniformBlockIndex,pname)*sizeof(GLint)',
             * 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint program;
            GLuint uniformBlockIndex;
            GLenum pname;
            GLuint param_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetActiveUniformBlockiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            uniformBlockIndex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param_len = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = (int)param_len;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += param_len;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetActiveUniformBlockiv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                uniformBlockIndex, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetActiveUniformBlockName: {
            /* readline: "glGetActiveUniformBlockName GLuint program, GLuint
             * uniformBlockIndex, GLsizei bufSize, GLsizei
             * *length#sizeof(GLsizei), GLchar *uniformBlockName#bufSize
             * @{if(bufSize<0){ set_gl_error(context,GL_INVALID_VALUE); return;
             * }}" */
            /* func name: "glGetActiveUniformBlockName" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'uniformBlockIndex', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'bufSize',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei*', 'name': 'length', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLsizei)', 'loc': 3, 'ptr_ptr': False}, {'type':
             * 'GLchar*', 'name': 'uniformBlockName', 'ptr': 'out', 'ptr_len':
             * 'bufSize', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLuint uniformBlockIndex;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetActiveUniformBlockName)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            uniformBlockIndex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + bufSize;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLchar *uniformBlockName = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetActiveUniformBlockName(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                uniformBlockIndex, bufSize, length, uniformBlockName);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetSamplerParameteriv: {
            /* readline: "glGetSamplerParameteriv GLuint sampler, GLenum pname,
             * GLint *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetSamplerParameteriv" */
            /* args: [{'type': 'GLuint', 'name': 'sampler', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint sampler;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetSamplerParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sampler = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetSamplerParameteriv((GLuint)GLResource::get_host_sampler_id(
                                        opengl_context, (unsigned int)sampler),
                                    pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetSamplerParameterfv: {
            /* readline: "glGetSamplerParameterfv GLuint sampler, GLenum pname,
             * GLfloat *params#gl_pname_size(pname)*sizeof(GLfloat)" */
            /* func name: "glGetSamplerParameterfv" */
            /* args: [{'type': 'GLuint', 'name': 'sampler', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfloat)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint sampler;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetSamplerParameterfv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sampler = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfloat *params = (GLfloat *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetSamplerParameterfv((GLuint)GLResource::get_host_sampler_id(
                                        opengl_context, (unsigned int)sampler),
                                    pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramBinary: {
            /* readline: "glGetProgramBinary GLuint program, GLsizei bufSize,
             * GLsizei *length#sizeof(GLsizei), GLenum
             * *binaryFormat#sizeof(GLenum), void *binary#bufSize
             * @{if(bufSize<0){ set_gl_error(context,GL_INVALID_VALUE); return;
             * }}" */
            /* func name: "glGetProgramBinary" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'bufSize', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei*', 'name': 'length', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLsizei)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLenum*', 'name': 'binaryFormat', 'ptr': 'out',
             * 'ptr_len': 'sizeof(GLenum)', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'void*', 'name': 'binary', 'ptr': 'out', 'ptr_len':
             * 'bufSize', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramBinary)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + sizeof(GLenum) + bufSize;

            unsigned char *ret_buf = nullptr;

            if (likely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLenum *binaryFormat = (GLenum *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLenum);

            void *binary = (void *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetProgramBinary((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               bufSize, length, binaryFormat, binary);

            ALOGD("glGetProgramBinary len %u format %lld\n", *length,
                  *binaryFormat);
            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (likely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetInternalformativ: {
            /* readline: "glGetInternalformativ GLenum target, GLenum
             * internalformat, GLenum pname, GLsizei count, GLint
             * *params#count*sizeof(GLint)" */
            /* func name: "glGetInternalformativ" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'internalformat', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'pname', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'count*sizeof(GLint)', 'loc': 4, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum internalformat;
            GLenum pname;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetInternalformativ)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = count * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += count * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetInternalformativ(target, internalformat, pname, count, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetClipPlanexOES: {
            /* readline: "glGetClipPlanexOES GLenum plane, GLfixed
             * *equation#4*sizeof(GLfixed)" */
            /* func name: "glGetClipPlanexOES" */
            /* args: [{'type': 'GLenum', 'name': 'plane', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLfixed*', 'name': 'equation', 'ptr': 'out', 'ptr_len':
             * '4*sizeof(GLfixed)', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum plane;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetClipPlanexOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            plane = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = 4 * sizeof(GLfixed);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfixed *equation = (GLfixed *)(ret_buf + out_buf_loc);
            out_buf_loc += 4 * sizeof(GLfixed);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }
            //            glGetClipPlanexOES(plane, equation);
            ALOGE("glGetClipPlanexOES(plane, equation);");

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetFixedvOES: {
            /* readline: "glGetFixedvOES GLenum pname, GLfixed
             * *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glGetFixedvOES" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLfixed*', 'name': 'params', 'ptr': 'out', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetFixedvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfixed *params = (GLfixed *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            //            glGetFixedvOES(pname, params);
            ALOGE("glGetFixedvOES(pname, params);");

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetTexEnvxvOES: {
            /* readline: "glGetTexEnvxvOES GLenum target, GLenum pname, GLfixed
             * *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glGetTexEnvxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfixed*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetTexEnvxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfixed *params = (GLfixed *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            //            glGetTexEnvxvOES(target, pname, params);
            ALOGE("glGetTexEnvxvOES(target, pname, params);");

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetTexParameterxvOES: {
            /* readline: "glGetTexParameterxvOES GLenum target, GLenum pname,
             * GLfixed *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glGetTexParameterxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfixed*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetTexParameterxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfixed *params = (GLfixed *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
            } else {
                GLState::texture_binding_status_sync(opengl_context, target);
            }
            if (target == GL_TEXTURE_EXTERNAL_OES) {
                TextureBindingStatus *texture_status =
                    &(opengl_context->texture_binding_status);
                if (texture_status->host_current_active_texture != 0) {
                    glActiveTexture(GL_TEXTURE0);
                }
                glBindTexture(GL_TEXTURE_2D,
                              texture_status->current_texture_external);
                //                glGetTexParameterxvOES(GL_TEXTURE_2D, pname,
                //                params);
                ALOGE("glGetTexParameterxvOES(GL_TEXTURE_2D, pname, params);");
                glBindTexture(GL_TEXTURE_2D,
                              texture_status->host_current_texture_2D[0]);
                if (texture_status->host_current_active_texture != 0) {
                    glActiveTexture(
                        texture_status->host_current_active_texture +
                        GL_TEXTURE0);
                }
            } else {
                //                glGetTexParameterxvOES(target, pname, params);
                ALOGE("glGetTexParameterxvOES(target, pname, params);");
            }
            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetLightxvOES: {
            /* readline: "glGetLightxvOES GLenum light, GLenum pname, GLfixed
             * *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glGetLightxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'light', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfixed*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum light;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetLightxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            light = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfixed *params = (GLfixed *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            //            glGetLightxvOES(light, pname, params);
            ALOGE("glGetLightxvOES(light, pname, params);");

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetMaterialxvOES: {
            /* readline: "glGetMaterialxvOES GLenum face, GLenum pname, GLfixed
             * *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glGetMaterialxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'face', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfixed*', 'name': 'params', 'ptr': 'out',
             * 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum face;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetMaterialxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            face = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfixed *params = (GLfixed *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            //            glGetMaterialxvOES(face, pname, params);
            ALOGE("glGetMaterialxvOES(face, pname, params);");

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetTexGenxvOES: {
            /* readline: "glGetTexGenxvOES GLenum coord, GLenum pname, GLfixed
             * *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glGetTexGenxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'coord', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfixed*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum coord;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetTexGenxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            coord = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfixed *params = (GLfixed *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            //            glGetTexGenxvOES(coord, pname, params);
            ALOGE("glGetTexGenxvOES(coord, pname, params);");

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetFramebufferParameteriv: {
            /* readline: "glGetFramebufferParameteriv GLenum target, GLenum
             * pname, GLint *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetFramebufferParameteriv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetFramebufferParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetFramebufferParameteriv(target, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramInterfaceiv: {
            /* readline: "glGetProgramInterfaceiv GLuint program, GLenum
             * programInterface, GLenum pname, GLint
             * *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetProgramInterfaceiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'programInterface', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'pname', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint*', 'name': 'params', 'ptr': 'out', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint program;
            GLenum programInterface;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramInterfaceiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            programInterface = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetProgramInterfaceiv((GLuint)GLResource::get_host_program_id(
                                        opengl_context, (unsigned int)program),
                                    programInterface, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramResourceName: {
            /* readline: "glGetProgramResourceName GLuint program, GLenum
             * programInterface, GLuint index, GLsizei bufSize, GLsizei
             * *length#sizeof(GLsizei), GLchar *name#bufSize @{if(bufSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGetProgramResourceName" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'programInterface', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'index', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'bufSize', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei*', 'name':
             * 'length', 'ptr': 'out', 'ptr_len': 'sizeof(GLsizei)', 'loc': 4,
             * 'ptr_ptr': False}, {'type': 'GLchar*', 'name': 'name', 'ptr':
             * 'out', 'ptr_len': 'bufSize', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLenum programInterface;
            GLuint index;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramResourceName)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            programInterface = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + bufSize;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLchar *name = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetProgramResourceName((GLuint)GLResource::get_host_program_id(
                                         opengl_context, (unsigned int)program),
                                     programInterface, index, bufSize, length,
                                     name);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramResourceiv: {
            /* readline: "glGetProgramResourceiv GLuint program, GLenum
             * programInterface, GLuint index, GLsizei propCount, const GLenum
             * *props#propCount*sizeof(GLenum), GLsizei bufSize, GLsizei
             * *length#sizeof(GLsizei), GLint *params#bufSize*sizeof(GLint)
             * @{if(bufSize<0){ set_gl_error(context,GL_INVALID_VALUE); return;
             * }}" */
            /* func name: "glGetProgramResourceiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'programInterface', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'index', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'propCount', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLenum*', 'name':
             * 'props', 'ptr': 'in', 'ptr_len': 'propCount*sizeof(GLenum)',
             * 'loc': 4, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'bufSize', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr':
             * False}, {'type': 'GLsizei*', 'name': 'length', 'ptr': 'out',
             * 'ptr_len': 'sizeof(GLsizei)', 'loc': 6, 'ptr_ptr': False},
             * {'type': 'GLint*', 'name': 'params', 'ptr': 'out', 'ptr_len':
             * 'bufSize*sizeof(GLint)', 'loc': 7, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLenum programInterface;
            GLuint index;
            GLsizei propCount;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramResourceiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            programInterface = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            propCount = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            int props_flag = 0;
            int props_null_flag = 0;
            GLenum *props = (GLenum *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLsizei) + bufSize * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetProgramResourceiv((GLuint)GLResource::get_host_program_id(
                                       opengl_context, (unsigned int)program),
                                   programInterface, index, propCount, props,
                                   bufSize, length, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramPipelineiv: {
            /* readline: "glGetProgramPipelineiv GLuint pipeline, GLenum pname,
             * GLint *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetProgramPipelineiv" */
            /* args: [{'type': 'GLuint', 'name': 'pipeline', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint pipeline;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramPipelineiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pipeline = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetProgramPipelineiv((GLuint)GLResource::get_host_pipeline_id(
                                       opengl_context, (unsigned int)pipeline),
                                   pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramPipelineInfoLog: {
            /* readline: "glGetProgramPipelineInfoLog GLuint pipeline, GLsizei
             * bufSize, GLsizei *length#sizeof(GLsizei), GLchar *infoLog#bufSize
             * @{if(bufSize<0){ set_gl_error(context,GL_INVALID_VALUE); return;
             * }}" */
            /* func name: "glGetProgramPipelineInfoLog" */
            /* args: [{'type': 'GLuint', 'name': 'pipeline', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'bufSize', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei*', 'name': 'length', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLsizei)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLchar*', 'name': 'infoLog', 'ptr': 'out', 'ptr_len':
             * 'bufSize', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint pipeline;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramPipelineInfoLog)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pipeline = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + bufSize;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLchar *infoLog = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetProgramPipelineInfoLog(
                (GLuint)GLResource::get_host_pipeline_id(
                    opengl_context, (unsigned int)pipeline),
                bufSize, length, infoLog);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetMultisamplefv: {
            /* readline: "glGetMultisamplefv GLenum pname, GLuint index, GLfloat
             * *val#gl_pname_size(pname)*sizeof(GLfloat)" */
            /* func name: "glGetMultisamplefv" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat*', 'name': 'val', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfloat)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum pname;
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetMultisamplefv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfloat *val = (GLfloat *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetMultisamplefv(pname, index, val);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetTexLevelParameteriv: {
            /* readline: "glGetTexLevelParameteriv GLenum target, GLint level,
             * GLenum pname, GLint *params#gl_pname_size(pname)*sizeof(GLint)"
             */
            /* func name: "glGetTexLevelParameteriv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'pname', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint*', 'name': 'params', 'ptr': 'out', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetTexLevelParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                glGetTexLevelParameteriv(target, level, pname, params);
            } else {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                glGetTexLevelParameteriv(bind_texture, level, pname, params);
                ALOGE("DSA not supported.");
            }
            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetTexLevelParameterfv: {
            /* readline: "glGetTexLevelParameterfv GLenum target, GLint level,
             * GLenum pname, GLfloat
             * *params#gl_pname_size(pname)*sizeof(GLfloat)" */
            /* func name: "glGetTexLevelParameterfv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'pname', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLfloat*', 'name': 'params', 'ptr': 'out', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLfloat)', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetTexLevelParameterfv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfloat *params = (GLfloat *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
                MainRenderer::DSA_ENABLE == 0) {
                glGetTexLevelParameterfv(target, level, pname, params);
            } else {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                glGetTexLevelParameterfv(bind_texture, level, pname, params);
                ALOGE("DSA not supported.");
            }

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetSynciv: {
            /* readline: "glGetSynciv GLsync sync, GLenum pname, GLsizei
             * bufSize, GLsizei *length#sizeof(GLsizei), GLint
             * *values#bufSize*sizeof(GLint) @{if(bufSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGetSynciv" */
            /* args: [{'type': 'GLsync', 'name': 'sync', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'bufSize', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei*', 'name': 'length', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLsizei)', 'loc': 3, 'ptr_ptr': False}, {'type':
             * 'GLint*', 'name': 'values', 'ptr': 'out', 'ptr_len':
             * 'bufSize*sizeof(GLint)', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLsync sync;
            GLenum pname;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetSynciv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sync = *(GLsync *)(temp + temp_loc);
            temp_loc += 8;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + bufSize * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLint *values = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetSynciv((GLsync)GLResource::get_host_sync_id(
                            opengl_context, (unsigned int)(uint64_t)sync),
                        pname, bufSize, length, values);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetAttribLocation: {
            /* readline: "GLint glGetAttribLocation GLuint program, const GLchar
             * *name#strlen(name)+1" */
            /* func name: "glGetAttribLocation" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLchar*', 'name': 'name', 'ptr': 'in', 'ptr_len':
             * 'strlen(name)+1', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "GLint" */
            /* type: "112" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetAttribLocation)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int name_flag = 0;
            int name_null_flag = 0;
            GLchar *name = (GLchar *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *ret_ptr = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLint ret =
                glGetAttribLocation((GLuint)GLResource::get_host_program_id(
                                        opengl_context, (unsigned int)program),
                                    name);
            *ret_ptr = ret;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (name_flag == 1) {
                // g_free(name);
            }

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetUniformLocation: {
            /* readline: "GLint glGetUniformLocation GLuint program, const
             * GLchar *name#strlen(name)+1" */
            /* func name: "glGetUniformLocation" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLchar*', 'name': 'name', 'ptr': 'in', 'ptr_len':
             * 'strlen(name)+1', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "GLint" */
            /* type: "112" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetUniformLocation)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;

            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int name_flag = 0;
            int name_null_flag = 0;
            GLchar *name = (GLchar *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *ret_ptr = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLint ret =
                glGetUniformLocation((GLuint)GLResource::get_host_program_id(
                                         opengl_context, (unsigned int)program),
                                     name);
            *ret_ptr = ret;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (name_flag == 1) {
                // g_free(name);
            }

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetFragDataLocation: {
            /* readline: "GLint glGetFragDataLocation GLuint program, const
             * GLchar *name#strlen(name)+1" */
            /* func name: "glGetFragDataLocation" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLchar*', 'name': 'name', 'ptr': 'in', 'ptr_len':
             * 'strlen(name)+1', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "GLint" */
            /* type: "112" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetFragDataLocation)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int name_flag = 0;
            int name_null_flag = 0;
            GLchar *name = (GLchar *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *ret_ptr = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLint ret = glGetFragDataLocation(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                name);
            *ret_ptr = ret;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetUniformBlockIndex: {
            /* readline: "GLuint glGetUniformBlockIndex GLuint program, const
             * GLchar *uniformBlockName#strlen(uniformBlockName)+1" */
            /* func name: "glGetUniformBlockIndex" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLchar*', 'name': 'uniformBlockName', 'ptr': 'in', 'ptr_len':
             * 'strlen(uniformBlockName)+1', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "GLuint" */
            /* type: "112" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetUniformBlockIndex)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int uniformBlockName_flag = 0;
            int uniformBlockName_null_flag = 0;
            GLchar *uniformBlockName = (GLchar *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLuint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLuint *ret_ptr = (GLuint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLuint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLuint ret = glGetUniformBlockIndex(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                uniformBlockName);
            *ret_ptr = ret;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (uniformBlockName_flag == 1) {
                // g_free(uniformBlockName);
            }

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramResourceIndex: {
            /* readline: "GLuint glGetProgramResourceIndex GLuint program,
             * GLenum programInterface, const GLchar *name#strlen(name)+1" */
            /* func name: "glGetProgramResourceIndex" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'programInterface', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'const GLchar*', 'name': 'name',
             * 'ptr': 'in', 'ptr_len': 'strlen(name)+1', 'loc': 2, 'ptr_ptr':
             * False}] */
            /* ret: "GLuint" */
            /* type: "112" */

            /* Define variables */
            GLuint program;
            GLenum programInterface;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramResourceIndex)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            programInterface = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            int name_flag = 0;
            int name_null_flag = 0;
            GLchar *name = (GLchar *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLuint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLuint *ret_ptr = (GLuint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLuint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLuint ret = glGetProgramResourceIndex(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                programInterface, name);
            *ret_ptr = ret;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (name_flag == 1) {
                // g_free(name);
            }

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramResourceLocation: {
            /* readline: "GLint glGetProgramResourceLocation GLuint program,
             * GLenum programInterface, const GLchar *name#strlen(name)+1" */
            /* func name: "glGetProgramResourceLocation" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'programInterface', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'const GLchar*', 'name': 'name',
             * 'ptr': 'in', 'ptr_len': 'strlen(name)+1', 'loc': 2, 'ptr_ptr':
             * False}] */
            /* ret: "GLint" */
            /* type: "112" */

            /* Define variables */
            GLuint program;
            GLenum programInterface;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glGetProgramResourceLocation)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            programInterface = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            int name_flag = 0;
            int name_null_flag = 0;
            GLchar *name = (GLchar *)call->paras[0].para_ptr;

            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *ret_ptr = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLint ret = glGetProgramResourceLocation(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                programInterface, name);
            *ret_ptr = ret;

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (name_flag == 1) {
                // g_free(name);
            }

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetActiveAttrib: {
            /* readline: "glGetActiveAttrib GLuint program, GLuint index,
             * GLsizei bufSize, GLsizei *length#sizeof(GLsizei), GLint
             * *size#sizeof(GLint), GLenum *type#sizeof(GLenum), GLchar
             * *name#bufSize @{if(bufSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGetActiveAttrib" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'bufSize', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei*', 'name': 'length', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLsizei)', 'loc': 3, 'ptr_ptr': False}, {'type':
             * 'GLint*', 'name': 'size', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLint)', 'loc': 4, 'ptr_ptr': False}, {'type': 'GLenum*',
             * 'name': 'type', 'ptr': 'out', 'ptr_len': 'sizeof(GLenum)', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLchar*', 'name': 'name', 'ptr':
             * 'out', 'ptr_len': 'bufSize', 'loc': 6, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLuint index;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetActiveAttrib)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len =
                sizeof(GLsizei) + sizeof(GLint) + sizeof(GLenum) + bufSize;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLint *size = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            GLenum *type = (GLenum *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLenum);

            GLchar *name = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetActiveAttrib((GLuint)GLResource::get_host_program_id(
                                  opengl_context, (unsigned int)program),
                              index, bufSize, length, size, type, name);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetActiveUniform: {
            /* readline: "glGetActiveUniform GLuint program, GLuint index,
             * GLsizei bufSize, GLsizei *length#sizeof(GLsizei), GLint
             * *size#sizeof(GLint), GLenum *type#sizeof(GLenum), GLchar
             * *name#bufSize @{if(bufSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGetActiveUniform" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'bufSize', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei*', 'name': 'length', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLsizei)', 'loc': 3, 'ptr_ptr': False}, {'type':
             * 'GLint*', 'name': 'size', 'ptr': 'out', 'ptr_len':
             * 'sizeof(GLint)', 'loc': 4, 'ptr_ptr': False}, {'type': 'GLenum*',
             * 'name': 'type', 'ptr': 'out', 'ptr_len': 'sizeof(GLenum)', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLchar*', 'name': 'name', 'ptr':
             * 'out', 'ptr_len': 'bufSize', 'loc': 6, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLuint index;
            GLsizei bufSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetActiveUniform)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bufSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = call->paras[0].para_ptr;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *length = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLint *size = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            GLenum *type = (GLenum *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLenum);

            GLchar *name = (GLchar *)(ret_buf + out_buf_loc);
            out_buf_loc += bufSize;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetActiveUniform((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               index, bufSize, length, size, type, name);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetAttachedShaders: {
            /* readline: "glGetAttachedShaders GLuint program, GLsizei maxCount,
             * GLsizei *count#sizeof(GLsizei), GLuint
             * *shaders#maxCount*sizeof(GLuint)" */
            /* func name: "glGetAttachedShaders" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'maxCount', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei*', 'name': 'count', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLsizei)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLuint*', 'name': 'shaders', 'ptr': 'out', 'ptr_len':
             * 'maxCount*sizeof(GLuint)', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLsizei maxCount;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetAttachedShaders)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            maxCount = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLsizei) + maxCount * sizeof(GLuint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLsizei *count = (GLsizei *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLsizei);

            GLuint *shaders = (GLuint *)(ret_buf + out_buf_loc);
            out_buf_loc += maxCount * sizeof(GLuint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetAttachedShaders((GLuint)GLResource::get_host_program_id(
                                     opengl_context, (unsigned int)program),
                                 maxCount, count, shaders);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetProgramiv: {
            /* readline: "glGetProgramiv GLuint program, GLenum pname, GLint
             * *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetProgramiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint program;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetProgramiv((GLuint)GLResource::get_host_program_id(
                               opengl_context, (unsigned int)program),
                           pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetShaderiv: {
            /* readline: "glGetShaderiv GLuint shader, GLenum pname, GLint
             * *params#sizeof(GLint)" */
            /* func name: "glGetShaderiv" */
            /* args: [{'type': 'GLuint', 'name': 'shader', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLint)', 'loc': 2, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint shader;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetShaderiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetShaderiv((GLuint)GLResource::get_host_shader_id(
                              opengl_context, (unsigned int)shader),
                          pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetUniformfv: {
            /* readline: "glGetUniformfv GLuint program, GLint location, GLuint
             * param_len, GLfloat *params#param_len" */
            /* func name: "glGetUniformfv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'size_t', 'name': 'param_len', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLfloat*', 'name': 'params', 'ptr': 'out', 'ptr_len':
             * 'param_len', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLuint param_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetUniformfv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            param_len = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = (int)param_len;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfloat *params = (GLfloat *)(ret_buf + out_buf_loc);
            // out_buf_loc+=gl_get_program_uniform_size(context,program,location);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetUniformfv((GLuint)GLResource::get_host_program_id(
                               opengl_context, (unsigned int)program),
                           location, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetUniformiv: {
            /* readline: "glGetUniformiv GLuint program, GLint location, GLuint
             * param_len, GLint *params#param_len" */
            /* func name: "glGetUniformiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'size_t', 'name': 'param_len', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint*', 'name': 'params', 'ptr': 'out', 'ptr_len': 'param_len',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLuint param_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetUniformiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            param_len = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = (int)param_len;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            // out_buf_loc+=gl_get_program_uniform_size(context,program,location);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetUniformiv((GLuint)GLResource::get_host_program_id(
                               opengl_context, (unsigned int)program),
                           location, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetUniformuiv: {
            /* readline: "glGetUniformuiv GLuint program, GLint location, GLuint
             * *params#gl_get_program_uniform_size(context,program,location)" */
            /* func name: "glGetUniformuiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len':
             * 'gl_get_program_uniform_size(context,program,location)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLuint param_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetUniformuiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            param_len = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = (int)param_len;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLuint *params = (GLuint *)(ret_buf + out_buf_loc);
            // out_buf_loc+=gl_get_program_uniform_size(context,program,location);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetUniformuiv((GLuint)GLResource::get_host_program_id(
                                opengl_context, (unsigned int)program),
                            location, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetUniformIndices: {
            /* readline: "glGetUniformIndices GLuint program, GLsizei
             * uniformCount, const GLchar
             * *const*uniformNames#uniformCount|strlen(uniformNames[i])+1,
             * GLuint *uniformIndices#uniformCount*sizeof(GLuint)" */
            /* func name: "glGetUniformIndices" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'uniformCount', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLchar*', 'name':
             * 'uniformNames', 'ptr': 'in', 'ptr_len':
             * 'uniformCount|strlen(uniformNames[i])+1', 'loc': 2, 'ptr_ptr':
             * True}, {'type': 'GLuint*', 'name': 'uniformIndices', 'ptr':
             * 'out', 'ptr_len': 'uniformCount*sizeof(GLuint)', 'loc': 3,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint program;
            GLsizei uniformCount;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetUniformIndices)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            uniformCount = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            GLchar **uniformNames =
                (GLchar **)malloc(uniformCount * sizeof(const GLchar *));

            for (int i = 0; i < uniformCount; i++) {
                uniformNames[i] = (GLchar *)call->paras[i].para_ptr;
            }
            int out_buf_len = uniformCount * sizeof(GLuint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLuint *uniformIndices = (GLuint *)(ret_buf + out_buf_loc);
            out_buf_loc += uniformCount * sizeof(GLuint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetUniformIndices((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                uniformCount,
                                (const GLchar *const *)uniformNames,
                                uniformIndices);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            delete[] uniformNames;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetVertexAttribfv: {
            /* readline: "glGetVertexAttribfv_origin GLuint index, GLenum pname,
             * GLfloat *params#gl_pname_size(pname)*sizeof(GLfloat)" */
            /* func name: "glGetVertexAttribfv_origin" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfloat)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint index;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetVertexAttribfv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfloat *params = (GLfloat *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetVertexAttribfv(index, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetVertexAttribiv: {
            /* readline: "glGetVertexAttribiv_origin GLuint index, GLenum pname,
             * GLint *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetVertexAttribiv_origin" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint index;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetVertexAttribiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetVertexAttribiv(index, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetVertexAttribIiv: {
            /* readline: "glGetVertexAttribIiv_origin GLuint index, GLenum
             * pname, GLint *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetVertexAttribIiv_origin" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint index;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetVertexAttribIiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetVertexAttribIiv(index, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetVertexAttribIuiv: {
            /* readline: "glGetVertexAttribIuiv_origin GLuint index, GLenum
             * pname, GLuint *params#gl_pname_size(pname)*sizeof(GLuint)" */
            /* func name: "glGetVertexAttribIuiv_origin" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLuint)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLuint index;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetVertexAttribIuiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLuint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLuint *params = (GLuint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLuint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetVertexAttribIuiv(index, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetBufferParameteriv: {
            /* readline: "glGetBufferParameteriv GLenum target, GLenum pname,
             * GLint *params#sizeof(GLint)" */
            /* func name: "glGetBufferParameteriv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLint)', 'loc': 2, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetBufferParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *params = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            //            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
            //            MainRenderer::DSA_ENABLE == 0) {
            //                glGetBufferParameteriv(target, pname, params);
            //            } else {
            //                GLuint bind_buffer =
            //                GLState::get_guest_binding_buffer(opengl_context,
            //                target); glGetNamedBufferParameteriv(bind_buffer,
            //                pname, params); ALOGE("DSA not supported.");
            //            }
            // TODO Need Check HOST_OPENGL_VERSION
            glGetBufferParameteriv(target, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetBufferParameteri64v: {
            /* readline: "glGetBufferParameteri64v GLenum target, GLenum pname,
             * GLint64 *params#gl_pname_size(pname)*sizeof(GLint64)" */
            /* func name: "glGetBufferParameteri64v" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint64*', 'name': 'params', 'ptr':
             * 'out', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint64)', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetBufferParameteri64v)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint64);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint64 *params = (GLint64 *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint64);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            //            if (MainRenderer::HOST_OPENGL_VERSION < 45 ||
            //            MainRenderer::DSA_ENABLE == 0) {
            //                glGetBufferParameteri64v(target, pname, params);
            //            } else {
            //                //    GLuint bind_buffer =
            //                GLState::get_guest_binding_buffer(opengl_context,
            //                target);
            //                //    glGetNamedBufferParameteri64v(bind_buffer,
            //                pname, params); ALOGE("DSA not supported.");
            //            }
            // TODO Need Check HOST_OPENGL_VERSION
            glGetBufferParameteri64v(target, pname, params);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetBooleanv: {
            /* readline: "glGetBooleanv GLenum pname, GLboolean
             * *data#gl_pname_size(pname)*sizeof(GLboolean)" */
            /* func name: "glGetBooleanv" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLboolean*', 'name': 'data', 'ptr': 'out', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLboolean)', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetBooleanv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len =
                GLHelper::gl_pname_size(pname) * sizeof(GLboolean);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLboolean *data = (GLboolean *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLboolean);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetBooleanv(pname, data);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetBooleani_v: {
            /* readline: "glGetBooleani_v GLenum target, GLuint index, GLboolean
             * *data#sizeof(GLboolean)" */
            /* func name: "glGetBooleani_v" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean*', 'name': 'data', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLboolean)', 'loc': 2, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetBooleani_v)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLboolean);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLboolean *data = (GLboolean *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLboolean);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                ALOGD("@todo! glGetBooleani_v target %x index %x with dsa\n",
                      target, index);
            }

            glGetBooleani_v(target, index, data);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetFloatv: {
            /* readline: "glGetFloatv GLenum pname, GLfloat
             * *data#gl_pname_size(pname)*sizeof(GLfloat)" */
            /* func name: "glGetFloatv" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLfloat*', 'name': 'data', 'ptr': 'out', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLfloat)', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetFloatv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLfloat *data = (GLfloat *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfloat);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetFloatv(pname, data);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetIntegerv: {
            /* readline: "glGetIntegerv GLenum pname, GLint
             * *data#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glGetIntegerv" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint*',
             * 'name': 'data', 'ptr': 'out', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLint)', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetIntegerv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *data = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetIntegerv(pname, data);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetIntegeri_v: {
            /* readline: "glGetIntegeri_v GLenum target, GLuint index, GLint
             * *data#sizeof(GLint)" */
            /* func name: "glGetIntegeri_v" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'data', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLint)', 'loc': 2, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetIntegeri_v)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLint);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint *data = (GLint *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                ALOGD("@todo! glGetIntegeri_v target %x index %x with dsa\n",
                      target, index);
            }

            glGetIntegeri_v(target, index, data);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetInteger64v: {
            /* readline: "glGetInteger64v GLenum pname, GLint64
             * *data#gl_pname_size(pname)*sizeof(GLint64)" */
            /* func name: "glGetInteger64v" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLint64*', 'name': 'data', 'ptr': 'out', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLint64)', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetInteger64v)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = GLHelper::gl_pname_size(pname) * sizeof(GLint64);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint64 *data = (GLint64 *)(ret_buf + out_buf_loc);
            out_buf_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint64);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            glGetInteger64v(pname, data);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetInteger64i_v: {
            /* readline: "glGetInteger64i_v GLenum target, GLuint index, GLint64
             * *data#sizeof(GLint64)" */
            /* func name: "glGetInteger64i_v" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint64*', 'name': 'data', 'ptr':
             * 'out', 'ptr_len': 'sizeof(GLint64)', 'loc': 2, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "112" */

            /* Define variables */
            GLenum target;
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetInteger64i_v)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            int out_buf_len = sizeof(GLint64);

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            GLint64 *data = (GLint64 *)(ret_buf + out_buf_loc);
            out_buf_loc += sizeof(GLint64);

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }
            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                ALOGD("@todo! glGetInteger64i_v target %x index %x with dsa\n",
                      target, index);
            }
            glGetInteger64i_v(target, index, data);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

            /******* end of file '1-1-2', 69/91 functions*******/

            /******* file '1-2' *******/

        case FUNID_glMapBufferRange_read: {
            /* readline: "glMapBufferRange_read GLenum target, GLintptr offset,
             * GLsizeiptr length, GLbitfield access, void *mem_buf#length" */
            /* func name: "glMapBufferRange_read" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'offset', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'GLsizeiptr', 'name':
             * 'length', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'GLbitfield', 'name': 'access', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'void*',
             * 'name': 'mem_buf', 'ptr': 'out', 'ptr_len': 'length', 'loc': 4,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "120" */

            /* Define variables */
            GLenum target;
            GLintptr offset;
            GLsizeiptr length;
            GLbitfield access;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glMapBufferRange_read)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            length = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;

            access = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = length;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            void *mem_buf = ret_buf + out_buf_loc;
            out_buf_loc += length;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            Memory::d_glMapBufferRange_read(opengl_context, target, offset,
                                            length, access, mem_buf);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glReadPixels_without_bound: {
            /* readline: "glReadPixels_without_bound GLint x, GLint y, GLsizei
             * width, GLsizei height, GLenum format, GLenum type, GLint buf_len,
             * void *pixels#buf_len" */
            /* func name: "glReadPixels_without_bound" */
            /* args: [{'type': 'GLint', 'name': 'x', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'y',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'format', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'type', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'buf_len', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}, {'type':
             * 'void*', 'name': 'pixels', 'ptr': 'out', 'ptr_len': 'buf_len',
             * 'loc': 7, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "120" */

            /* Define variables */
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;
            GLenum format;
            GLenum type;
            GLint buf_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glReadPixels_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 28 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = buf_len;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            void *pixels = ret_buf + out_buf_loc;
            out_buf_loc += buf_len;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            Texture::d_glReadPixels_without_bound(opengl_context, x, y, width,
                                                  height, format, type, buf_len,
                                                  pixels);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glTestPointer3: {
            /* readline: "GLint glTestPointer3 GLint a, const GLint
             * *b#sizeof(GLint)*a, GLint *c#sizeof(GLint)*a" */
            /* func name: "glTestPointer3" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLint*',
             * 'name': 'b', 'ptr': 'in', 'ptr_len': 'sizeof(GLint)*a', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint*', 'name': 'c', 'ptr': 'out',
             * 'ptr_len': 'sizeof(GLint)*a', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "GLint" */
            /* type: "120" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            // GLint a;

            // int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            // if (unlikely(para_num < PARA_NUM_MIN_glTestPointer3))
            // {
            //     break;
            // }

            // size_t temp_len = 0;
            // unsigned char *temp = nullptr;

            // temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            // if (unlikely(temp_len < 4 * 1))
            // {
            //     break;
            // }

            // int null_flag = 0;
            // temp = get_direct_ptr(call->params[0].param_data_ptr,
            // &null_flag); if (unlikely(temp == NULL))
            // {
            //     if (temp_len != 0 && null_flag == 0)
            //     {
            //         temp = malloc(temp_len);no_ptr_buf=temp;
            //         guest_write(call->params[0].param_data_ptr, temp, 0,
            //         call->params[0].param_size);
            //     }
            //     else
            //     {
            //         break;
            //     }
            // }

            // unsigned int temp_loc = 0;

            // a = *(GLint *)(temp + temp_loc);
            // temp_loc += 4;

            // void *b = call->params[1].param_data_ptr;

            // GLint *c = call->params[2].param_data_ptr;

            // GLint ret = d_glTestPointer3(opengl_context, a, b, c);

            // guest_read(all_para[3].data, &ret, 0, sizeof(GLint));
        } break;

            /******* end of file '1-2', 4/94 functions*******/

            /******* file '2-1' *******/

        case FUNID_glFlush: {
            /* readline: "glFlush void" */
            /* func name: "glFlush" */
            /* args: [] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFlush)) {
                break;
            }
            ALOGD("glFlush");
            glFlush();
        } break;

        case FUNID_glFinish: {
            /* readline: "glFinish void" */
            /* func name: "glFinish" */
            /* args: [] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFinish)) {
                break;
            }
            ALOGD("glFinish");
            glFinish();
        } break;

        case FUNID_glBeginQuery: {
            /* readline: "glBeginQuery GLenum target, GLuint id" */
            /* func name: "glBeginQuery" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'id', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLuint id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBeginQuery)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            id = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBeginQuery(target, (GLuint)GLResource::get_host_query_id(
                                     opengl_context, (unsigned int)id));
            // ALOGD("glBeginQuery %llx %d -> %d target %llx r_context %llx\n",
            // (uint64_t)opengl_context, id,
            // (GLuint)GLResource::get_host_query_id(opengl_context, (unsigned
            // int)id), target, r_context);
        } break;

        case FUNID_glEndQuery: {
            /* readline: "glEndQuery GLenum target" */
            /* func name: "glEndQuery" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glEndQuery)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glEndQuery(target);
            // ALOGD("glEndQuery target %llx\n", target);
        } break;

        case FUNID_glViewport: {
            /* readline: "glViewport GLint x, GLint y, GLsizei width, GLsizei
             * height" */
            /* func name: "glViewport" */
            /* args: [{'type': 'GLint', 'name': 'x', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'y',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glViewport)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLState::d_glViewport_special(opengl_context, x, y, width, height);
        } break;

        case FUNID_glTexStorage2D: {
            /* readline: "glTexStorage2D GLenum target, GLsizei levels, GLenum
             * internalformat, GLsizei width, GLsizei height" */
            /* func name: "glTexStorage2D" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'levels', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLsizei levels;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexStorage2D)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            levels = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::GLState::get_guest_binding_texture(opengl_context,
                                                                target);
                //    glTextureStorage2D(bind_texture, levels, internalformat,
                //    width, height);
                ALOGE("DSA not supported.");
            } else {
                glTexStorage2D(target, levels, internalformat, width, height);
            }
        } break;

        case FUNID_glTexStorage3D: {
            /* readline: "glTexStorage3D GLenum target, GLsizei levels, GLenum
             * internalformat, GLsizei width, GLsizei height, GLsizei depth" */
            /* func name: "glTexStorage3D" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'levels', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLsizei levels;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;
            GLsizei depth;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexStorage3D)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            levels = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::GLState::get_guest_binding_texture(opengl_context,
                                                                target);
                //    glTextureStorage3D(bind_texture, levels, internalformat,
                //    width, height, depth);
                ALOGE("DSA not supported.");
            } else {
                glTexStorage3D(target, levels, internalformat, width, height,
                               depth);
            }
        } break;

        case FUNID_glTexImage2D_with_bound: {
            /* readline: "glTexImage2D_with_bound GLenum target, GLint level,
             * GLint internalformat, GLsizei width, GLsizei height, GLint
             * border, GLenum format, GLenum type, GLintptr pixels" */
            /* func name: "glTexImage2D_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'border', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'format', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'type', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 7, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'pixels', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 8, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint internalformat;
            GLsizei width;
            GLsizei height;
            GLint border;
            GLenum format;
            GLenum type;
            GLintptr pixels;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexImage2D_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 40 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pixels = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glTexImage2D_with_bound(opengl_context, target, level,
                                               internalformat, width, height,
                                               border, format, type, pixels);
        } break;

        case FUNID_glTexSubImage2D_with_bound: {
            /* readline: "glTexSubImage2D_with_bound GLenum target, GLint level,
             * GLint xoffset, GLint yoffset, GLsizei width, GLsizei height,
             * GLenum format, GLenum type, GLintptr pixels" */
            /* func name: "glTexSubImage2D_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'width', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'format', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}, {'type':
             * 'GLenum', 'name': 'type', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 7,
             * 'ptr_ptr': False}, {'type': 'GLintptr', 'name': 'pixels', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLsizei width;
            GLsizei height;
            GLenum format;
            GLenum type;
            GLintptr pixels;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexSubImage2D_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 40 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pixels = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glTexSubImage2D_with_bound(opengl_context, target, level,
                                                  xoffset, yoffset, width,
                                                  height, format, type, pixels);
        } break;

        case FUNID_glTexImage3D_with_bound: {
            /* readline: "glTexImage3D_with_bound GLenum target, GLint level,
             * GLint internalformat, GLsizei width, GLsizei height, GLsizei
             * depth, GLint border, GLenum format, GLenum type, GLintptr pixels"
             */
            /* func name: "glTexImage3D_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'border', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'format', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 7, 'ptr_ptr': False}, {'type':
             * 'GLenum', 'name': 'type', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 8,
             * 'ptr_ptr': False}, {'type': 'GLintptr', 'name': 'pixels', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 9, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint internalformat;
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLint border;
            GLenum format;
            GLenum type;
            GLintptr pixels;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexImage3D_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 44 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pixels = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glTexImage3D_with_bound(
                opengl_context, target, level, internalformat, width, height,
                depth, border, format, type, pixels);
        } break;

        case FUNID_glTexSubImage3D_with_bound: {
            /* readline: "glTexSubImage3D_with_bound GLenum target, GLint level,
             * GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width,
             * GLsizei height, GLsizei depth, GLenum format, GLenum type,
             * GLintptr pixels" */
            /* func name: "glTexSubImage3D_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'zoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'height',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 7, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'format', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'type', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 9, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'pixels', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 10, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLint zoffset;
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLenum format;
            GLenum type;
            GLintptr pixels;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexSubImage3D_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 48 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            zoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pixels = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glTexSubImage3D_with_bound(
                opengl_context, target, level, xoffset, yoffset, zoffset, width,
                height, depth, format, type, pixels);
        } break;

        case FUNID_glReadPixels_with_bound: {
            /* readline: "glReadPixels_with_bound GLint x, GLint y, GLsizei
             * width, GLsizei height, GLenum format, GLenum type, GLintptr
             * pixels" */
            /* func name: "glReadPixels_with_bound" */
            /* args: [{'type': 'GLint', 'name': 'x', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'y',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'format', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'type', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5,
             * 'ptr_ptr': False}, {'type': 'GLintptr', 'name': 'pixels', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;
            GLenum format;
            GLenum type;
            GLintptr pixels;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glReadPixels_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 32 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pixels = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glReadPixels_with_bound(opengl_context, x, y, width,
                                               height, format, type, pixels);
        } break;

        case FUNID_glCompressedTexImage2D_with_bound: {
            /* readline: "glCompressedTexImage2D_with_bound GLenum target, GLint
             * level, GLenum internalformat, GLsizei width, GLsizei height,
             * GLint border, GLsizei imageSize, GLintptr data @{if(imageSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glCompressedTexImage2D_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'border', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'imageSize', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'GLintptr', 'name': 'data', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 7, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;
            GLint border;
            GLsizei imageSize;
            GLintptr data;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glCompressedTexImage2D_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 36 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            data = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glCompressedTexImage2D_with_bound(
                opengl_context, target, level, internalformat, width, height,
                border, imageSize, data);
        } break;

        case FUNID_glCompressedTexSubImage2D_with_bound: {
            /* readline: "glCompressedTexSubImage2D_with_bound GLenum target,
             * GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei
             * height, GLenum format, GLsizei imageSize, GLintptr data
             * @{if(imageSize<0){ set_gl_error(context,GL_INVALID_VALUE);
             * return; }}" */
            /* func name: "glCompressedTexSubImage2D_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'width', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'format', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'imageSize', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 7, 'ptr_ptr': False}, {'type': 'GLintptr', 'name': 'data',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLsizei width;
            GLsizei height;
            GLenum format;
            GLsizei imageSize;
            GLintptr data;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glCompressedTexSubImage2D_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 40 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            data = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glCompressedTexSubImage2D_with_bound(
                opengl_context, target, level, xoffset, yoffset, width, height,
                format, imageSize, data);
        } break;

        case FUNID_glCompressedTexImage3D_with_bound: {
            /* readline: "glCompressedTexImage3D_with_bound GLenum target, GLint
             * level, GLenum internalformat, GLsizei width, GLsizei height,
             * GLsizei depth, GLint border, GLsizei imageSize, GLintptr data
             * @{if(imageSize<0){ set_gl_error(context,GL_INVALID_VALUE);
             * return; }}" */
            /* func name: "glCompressedTexImage3D_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'border', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'imageSize',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 7, 'ptr_ptr': False},
             * {'type': 'GLintptr', 'name': 'data', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 8, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLint border;
            GLsizei imageSize;
            GLintptr data;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glCompressedTexImage3D_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 40 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            data = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glCompressedTexImage3D_with_bound(
                opengl_context, target, level, internalformat, width, height,
                depth, border, imageSize, data);
        } break;

        case FUNID_glCompressedTexSubImage3D_with_bound: {
            /* readline: "glCompressedTexSubImage3D_with_bound GLenum target,
             * GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei
             * width, GLsizei height, GLsizei depth, GLenum format, GLsizei
             * imageSize, GLintptr data" */
            /* func name: "glCompressedTexSubImage3D_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'zoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'height',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 7, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'format', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'imageSize', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 9, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'data', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 10, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLint zoffset;
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLenum format;
            GLsizei imageSize;
            GLintptr data;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glCompressedTexSubImage3D_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 48 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            zoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            data = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glCompressedTexSubImage3D_with_bound(
                opengl_context, target, level, xoffset, yoffset, zoffset, width,
                height, depth, format, imageSize, data);
        } break;

        case FUNID_glCopyTexImage2D: {
            /* readline: "glCopyTexImage2D GLenum target, GLint level, GLenum
             * internalformat, GLint x, GLint y, GLsizei width, GLsizei height,
             * GLint border" */
            /* func name: "glCopyTexImage2D" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLint', 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'y',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'border', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 7, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLenum internalformat;
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;
            GLint border;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCopyTexImage2D)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 32 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    if (glCopyTextureImage2DEXT == NULL)
                //    {
                //    ALOGD("error! glCopyTextureImage2DEXT is NULL!\n");
                //    }
                //    glCopyTextureImage2DEXT(bind_texture, target, level,
                //    internalformat, x, y, width, height, border);
                ALOGE("DSA not supported.");
            } else {
                glCopyTexImage2D(target, level, internalformat, x, y, width,
                                 height, border);
            }
        } break;

        case FUNID_glCopyTexSubImage2D: {
            /* readline: "glCopyTexSubImage2D GLenum target, GLint level, GLint
             * xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei
             * height" */
            /* func name: "glCopyTexSubImage2D" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'x', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'y', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 7,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCopyTexSubImage2D)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 32 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    glCopyTextureSubImage2D(bind_texture, level, xoffset,
                //    yoffset, x, y, width, height);
                ALOGE("DSA not supported.");
            } else {
                glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y,
                                    width, height);
            }
        } break;

        case FUNID_glCopyTexSubImage3D: {
            /* readline: "glCopyTexSubImage3D GLenum target, GLint level, GLint
             * xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei
             * width, GLsizei height" */
            /* func name: "glCopyTexSubImage3D" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'zoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'y', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'width', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 7,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'height', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLint zoffset;
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCopyTexSubImage3D)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 36 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            zoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    glCopyTextureSubImage3D(bind_texture, level, xoffset,
                //    yoffset, zoffset, x, y, width, height);
                ALOGE("DSA not supported.");
            } else {
                glCopyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x,
                                    y, width, height);
            }
        } break;

        case FUNID_glVertexAttribPointer_with_bound: {
            /* readline: "glVertexAttribPointer_with_bound GLuint index, GLint
             * size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr
             * pointer" */
            /* func name: "glVertexAttribPointer_with_bound" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'type', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'normalized', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'stride', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLintptr', 'name': 'pointer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLuint index;
            GLint size;
            GLenum type;
            GLboolean normalized;
            GLsizei stride;
            GLintptr pointer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glVertexAttribPointer_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 28 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            normalized = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            stride = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            pointer = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glVertexAttribPointer_with_bound(
                opengl_context, index, size, type, normalized, stride, pointer);
        } break;

        case FUNID_glVertexAttribPointer_offset: {
            /* readline: "glVertexAttribPointer_offset GLuint index, GLuint
             * size, GLenum type, GLboolean normalized, GLsizei stride, GLuint
             * index_father, GLintptr offset" */
            /* func name: "glVertexAttribPointer_offset" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'type', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'normalized', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'stride', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'index_father', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'offset', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 6, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLuint index;
            GLuint size;
            GLenum type;
            GLboolean normalized;
            GLsizei stride;
            GLuint index_father;
            GLintptr offset;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glVertexAttribPointer_offset)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 32 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            normalized = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            stride = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            index_father = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glVertexAttribPointer_offset(opengl_context, index, size,
                                                   type, normalized, stride,
                                                   index_father, offset);
        } break;

        case FUNID_glMapBufferRange_write: {
            /* readline: "glMapBufferRange_write GLenum target, GLintptr offset,
             * GLsizeiptr length, GLbitfield access" */
            /* func name: "glMapBufferRange_write" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'offset', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'GLsizeiptr', 'name':
             * 'length', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'GLbitfield', 'name': 'access', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLintptr offset;
            GLsizeiptr length;
            GLbitfield access;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glMapBufferRange_write)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            length = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;

            access = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Memory::d_glMapBufferRange_write(opengl_context, target, offset,
                                             length, access);
        } break;

        case FUNID_glUnmapBuffer_special: {
            /* readline: "glUnmapBuffer_special GLenum target" */
            /* func name: "glUnmapBuffer_special" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUnmapBuffer_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Memory::d_glUnmapBuffer_special(opengl_context, target);
        } break;

        case FUNID_glWaitSync: {
            /* readline: "glWaitSync GLsync sync, GLbitfield flags, GLuint64
             * timeout" */
            /* func name: "glWaitSync" */
            /* args: [{'type': 'GLsync', 'name': 'sync', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLbitfield', 'name':
             * 'flags', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint64', 'name': 'timeout', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLsync sync;
            GLbitfield flags;
            GLuint64 timeout;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glWaitSync)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sync = *(GLsync *)(temp + temp_loc);
            temp_loc += 8;

            flags = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            timeout = *(GLuint64 *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            ALOGD("glWaitSync context %llx guest %u host %llu timeout %llu\n",
                  (uint64_t)opengl_context, (unsigned int)(uint64_t)sync,
                  (GLsync)GLResource::get_host_sync_id(
                      opengl_context, (unsigned int)(uint64_t)sync),
                  timeout);
            glWaitSync((GLsync)GLResource::get_host_sync_id(
                           opengl_context, (unsigned int)(uint64_t)sync),
                       flags, timeout);
            // glClientWaitSync((GLsync)GLResource::get_host_sync_id(opengl_context,
            // (unsigned int)(uint64_t)sync), GL_SYNC_FLUSH_COMMANDS_BIT,
            // 1000000000);
        } break;

        case FUNID_glShaderBinary: {
            /* readline: "glShaderBinary GLsizei count, const GLuint
             * *shaders#count*sizeof(GLuint), GLenum binaryFormat, const void
             * *binary#length, GLsizei length" */
            /* func name: "glShaderBinary" */
            /* args: [{'type': 'GLsizei', 'name': 'count', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLuint*', 'name': 'shaders', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLuint)', 'loc': 1, 'ptr_ptr': False}, {'type':
             * 'GLenum', 'name': 'binaryFormat', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}, {'type': 'const void*', 'name':
             * 'binary', 'ptr': 'in', 'ptr_len': 'length', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'length', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLsizei count;
            GLenum binaryFormat;
            GLsizei length;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glShaderBinary)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            binaryFormat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            length = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *shaders = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint);

            const void *binary = (const void *)(temp + temp_loc);
            temp_loc += length;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glShaderBinary(count, shaders, binaryFormat, binary, length);
        } break;

        case FUNID_glProgramBinary_special: {
            /* readline: "glProgramBinary GLuint program, GLenum binaryFormat,
             * const void *binary#length, GLsizei length, int *program_data_len"
             */
            /* func name: "glProgramBinary" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'binaryFormat', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const void*', 'name': 'binary',
             * 'ptr': 'in', 'ptr_len': 'length', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'length', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLuint program;
            GLenum binaryFormat;
            GLsizei length;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramBinary_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            binaryFormat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            length = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const void *binary = (const void *)(temp + temp_loc);
            temp_loc += length;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            ALOGD("context %llx program binary %u host %u\n",
                  (uint64_t)opengl_context, program,
                  (GLuint)GLResource::get_host_program_id(
                      opengl_context, (unsigned int)program));
            GLProgram::d_glProgramBinary_special(
                opengl_context,
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                binaryFormat, binary, length);
        } break;

        case FUNID_glDrawBuffers: {
            /* readline: "glDrawBuffers GLsizei n, const GLenum
             * *bufs#n*sizeof(GLenum)" */
            /* func name: "glDrawBuffers" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLenum*',
             * 'name': 'bufs', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLenum)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDrawBuffers)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLenum *bufs = (const GLenum *)(temp + temp_loc);
            temp_loc += n * sizeof(GLenum);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glDrawBuffers(n, bufs);
        } break;

        case FUNID_glDrawArrays_origin: {
            /* readline: "glDrawArrays_origin GLenum mode, GLint first, GLsizei
             * count" */
            /* func name: "glDrawArrays_origin" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name':
             * 'first', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'count', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum mode;
            GLint first;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDrawArrays_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            first = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glDrawArrays_origin(opengl_context, mode, first, count);
        } break;

        case FUNID_glDrawArraysInstanced_origin: {
            /* readline: "glDrawArraysInstanced_origin GLenum mode, GLint first,
             * GLsizei count, GLsizei instancecount" */
            /* func name: "glDrawArraysInstanced_origin" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name':
             * 'first', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'count', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'instancecount', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum mode;
            GLint first;
            GLsizei count;
            GLsizei instancecount;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawArraysInstanced_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            first = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            instancecount = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glDrawArraysInstanced_origin(opengl_context, mode, first,
                                                   count, instancecount);
        } break;

        case FUNID_glDrawElementsInstanced_with_bound: {
            /* readline: "glDrawElementsInstanced_with_bound GLenum mode,
             * GLsizei count, GLenum type, GLsizeiptr indices, GLsizei
             * instancecount" */
            /* func name: "glDrawElementsInstanced_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'type', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizeiptr', 'name': 'indices', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'instancecount', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum mode;
            GLsizei count;
            GLenum type;
            GLsizeiptr indices;
            GLsizei instancecount;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawElementsInstanced_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            indices = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;

            instancecount = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glDrawElementsInstanced_with_bound(
                opengl_context, mode, count, type, indices, instancecount);
        } break;

        case FUNID_glDrawElements_with_bound: {
            /* readline: "glDrawElements_with_bound GLenum mode, GLsizei count,
             * GLenum type, GLsizeiptr indices" */
            /* func name: "glDrawElements_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'type', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizeiptr', 'name': 'indices', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum mode;
            GLsizei count;
            GLenum type;
            GLsizeiptr indices;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDrawElements_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            indices = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glDrawElements_with_bound(opengl_context, mode, count,
                                                type, indices);
        } break;

        case FUNID_glDrawRangeElements_with_bound: {
            /* readline: "glDrawRangeElements_with_bound GLenum mode, GLuint
             * start, GLuint end, GLsizei count, GLenum type, GLsizeiptr
             * indices" */
            /* func name: "glDrawRangeElements_with_bound" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'start', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'end', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'type', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizeiptr', 'name': 'indices', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum mode;
            GLuint start;
            GLuint end;
            GLsizei count;
            GLenum type;
            GLsizeiptr indices;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawRangeElements_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 28 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            start = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            end = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            indices = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glDrawRangeElements_with_bound(
                opengl_context, mode, start, end, count, type, indices);
        } break;

        case FUNID_glTestIntAsyn: {
            /* readline: "glTestIntAsyn GLint a, GLuint b, GLfloat c, GLdouble
             * d" */
            /* func name: "glTestIntAsyn" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'b', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLfloat', 'name': 'c', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}, {'type': 'GLdouble', 'name': 'd',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLint a;
            GLuint b;
            GLfloat c;
            GLdouble d;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTestIntAsyn)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            c = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            d = *(GLdouble *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLHelper::glTestIntAsyn(a, b, c, d);
        } break;

        case FUNID_glPrintfAsyn: {
            /* readline: "glPrintfAsyn GLint a, GLuint size, GLdouble c, const
             * GLchar *out_string#strlen(out_string)+1" */
            /* func name: "glPrintfAsyn" */
            /* args: [{'type': 'GLint', 'name': 'a', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLdouble', 'name': 'c', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'const GLchar*',
             * 'name': 'out_string', 'ptr': 'in', 'ptr_len':
             * 'strlen(out_string)+1', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLint a;
            GLuint size;
            GLdouble c;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPrintfAsyn)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            a = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            c = *(GLdouble *)(temp + temp_loc);
            temp_loc += 8;

            const GLchar *out_string = (const GLchar *)(temp + temp_loc);
            temp_loc += strlen(out_string) + 1;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLHelper::glPrintfAsyn(a, size, c, out_string);
        } break;

        case FUNID_glEGLImageTargetTexture2DOES: {
            /* readline: "glEGLImageTargetTexture2DOES GLenum target,
             * GLeglImageOES imageSize" */
            /* func name: "glEGLImageTargetTexture2DOES" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLeglImageOES', 'name': 'imageSize', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLeglImageOES imageSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glEGLImageTargetTexture2DOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLeglImageOES *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLState::d_glEGLImageTargetTexture2DOES(opengl_context, target,
                                                    imageSize);
        } break;

        case FUNID_glEGLImageTargetRenderbufferStorageOES: {
            /* readline: "glEGLImageTargetRenderbufferStorageOES GLenum target,
             * GLeglImageOES image" */
            /* func name: "glEGLImageTargetRenderbufferStorageOES" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLeglImageOES', 'name': 'image', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            GLenum target;
            GLeglImageOES image;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glEGLImageTargetRenderbufferStorageOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            image = *(GLeglImageOES *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLState::d_glEGLImageTargetRenderbufferStorageOES(opengl_context,
                                                              target, image);
        } break;

            /******* end of file '2-1', 37/130 functions*******/

            /******* file '2-1-1' *******/

        case FUNID_glGenBuffers: {
            /* readline: "glGenBuffers GLsizei n, const GLuint
             * *buffers#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenBuffers" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'buffers', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenBuffers)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *buffers = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenBuffers(opengl_context, n, buffers);
        } break;

        case FUNID_glGenRenderbuffers: {
            /* readline: "glGenRenderbuffers GLsizei n, const GLuint
             * *renderbuffers#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenRenderbuffers" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'renderbuffers', 'ptr': 'in', 'ptr_len':
             * 'n*sizeof(GLuint)', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenRenderbuffers)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *renderbuffers = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenRenderbuffers(opengl_context, n, renderbuffers);
        } break;

        case FUNID_glGenTextures: {
            /* readline: "glGenTextures GLsizei n, const GLuint
             * *textures#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenTextures" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'textures', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenTextures)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *textures = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenTextures(opengl_context, n, textures);
        } break;

        case FUNID_glGenSamplers: {
            /* readline: "glGenSamplers GLsizei count, const GLuint
             * *samplers#count*sizeof(GLuint) @{if(count<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenSamplers" */
            /* args: [{'type': 'GLsizei', 'name': 'count', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLuint*', 'name': 'samplers', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLuint)', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenSamplers)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *samplers = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenSamplers(opengl_context, count, samplers);
        } break;

        case FUNID_glCreateProgram: {
            /* readline: "glCreateProgram GLuint program" */
            /* func name: "glCreateProgram" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCreateProgram)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glCreateProgram(opengl_context, program);
        } break;

        case FUNID_glCreateShader: {
            /* readline: "glCreateShader GLenum type, GLuint shader
             * @if(type!=GL_COMPUTE_SHADER&&type!=GL_VERTEX_SHADER&&type!=GL_FRAGMENT_SHADER){set_gl_error(context,GL_INVALID_ENUM);return
             * 0;}" */
            /* func name: "glCreateShader" */
            /* args: [{'type': 'GLenum', 'name': 'type', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'shader', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLenum type;
            GLuint shader;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCreateShader)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            GLResource::d_glCreateShader(opengl_context, type, shader);
        } break;

        case FUNID_glFenceSync: {
            /* readline: "glFenceSync GLenum condition, GLbitfield flags, GLsync
             * sync" */
            /* func name: "glFenceSync" */
            /* args: [{'type': 'GLenum', 'name': 'condition', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLbitfield', 'name': 'flags', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'GLsync', 'name': 'sync',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLenum condition;
            GLbitfield flags;
            GLsync sync;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFenceSync)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            condition = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            flags = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            sync = *(GLsync *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glFenceSync(opengl_context, condition, flags, sync);
        } break;

        case FUNID_glCreateShaderProgramv: {
            /* readline: "glCreateShaderProgramv GLenum type, GLsizei count,
             * const GLchar *const*strings#count|strlen(strings[i])+1, GLuint
             * program
             * @{if(type!=GL_COMPUTE_SHADER&&type!=GL_VERTEX_SHADER&&type!=GL_FRAGMENT_SHADER){set_gl_error(context,GL_INVALID_ENUM);return
             * 0;}if(count<0){set_gl_error(context,GL_INVALID_VALUE);return
             * 0;}}" */
            /* func name: "glCreateShaderProgramv" */
            /* args: [{'type': 'GLenum', 'name': 'type', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'const GLchar*', 'name': 'strings', 'ptr': 'in',
             * 'ptr_len': 'count|strlen(strings[i])+1', 'loc': 2, 'ptr_ptr':
             * True}, {'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLenum type;
            GLsizei count;
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCreateShaderProgramv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            GLchar **strings =
                (GLchar **)malloc(count * sizeof(const GLchar *));

            for (int i = 0; i < count; i++) {
                strings[i] = (GLchar *)(temp + temp_loc);
                temp_loc += strlen(strings[i]) + 1;
            }
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glCreateShaderProgramv_special(
                opengl_context, type, count, (const GLchar *const *)strings,
                program);

            free(strings);
        } break;

        case FUNID_glGenFramebuffers: {
            /* readline: "glGenFramebuffers GLsizei n, const GLuint
             * *framebuffers#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenFramebuffers" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'framebuffers', 'ptr': 'in', 'ptr_len':
             * 'n*sizeof(GLuint)', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenFramebuffers)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *framebuffers = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenFramebuffers(opengl_context, n, framebuffers);
        } break;

        case FUNID_glGenProgramPipelines: {
            /* readline: "glGenProgramPipelines GLsizei n, const GLuint
             * *pipelines#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenProgramPipelines" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'pipelines', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenProgramPipelines)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *pipelines = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenProgramPipelines(opengl_context, n, pipelines);
        } break;

        case FUNID_glGenTransformFeedbacks: {
            /* readline: "glGenTransformFeedbacks GLsizei n, const GLuint
             * *ids#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenTransformFeedbacks" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'ids', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)', 'loc':
             * 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenTransformFeedbacks)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *ids = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenTransformFeedbacks(opengl_context, n, ids);
        } break;

        case FUNID_glGenVertexArrays: {
            /* readline: "glGenVertexArrays GLsizei n, const GLuint
             * *arrays#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenVertexArrays" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'arrays', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenVertexArrays)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *arrays = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenVertexArrays(opengl_context, n, arrays);
        } break;

        case FUNID_glGenQueries: {
            /* readline: "glGenQueries GLsizei n, const GLuint
             * *ids#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glGenQueries" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'ids', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)', 'loc':
             * 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenQueries)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *ids = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glGenQueries(opengl_context, n, ids);
        } break;

        case FUNID_glDeleteBuffers_origin: {
            /* readline: "glDeleteBuffers_origin GLsizei n, const GLuint
             * *buffers#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteBuffers_origin" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'buffers', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteBuffers_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *buffers = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteBuffers(opengl_context, n, buffers);
        } break;

        case FUNID_glDeleteRenderbuffers: {
            /* readline: "glDeleteRenderbuffers GLsizei n, const GLuint
             * *renderbuffers#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteRenderbuffers" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'renderbuffers', 'ptr': 'in', 'ptr_len':
             * 'n*sizeof(GLuint)', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteRenderbuffers)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *renderbuffers = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteRenderbuffers(opengl_context, n,
                                                renderbuffers);
        } break;

        case FUNID_glDeleteTextures: {
            /* readline: "glDeleteTextures GLsizei n, const GLuint
             * *textures#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteTextures" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'textures', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteTextures)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *textures = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteTextures(opengl_context, n, textures);
        } break;

        case FUNID_glDeleteSamplers: {
            /* readline: "glDeleteSamplers GLsizei count, const GLuint
             * *samplers#count*sizeof(GLuint) @{if(count<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteSamplers" */
            /* args: [{'type': 'GLsizei', 'name': 'count', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLuint*', 'name': 'samplers', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLuint)', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteSamplers)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *samplers = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteSamplers(opengl_context, count, samplers);
        } break;

        case FUNID_glDeleteProgram_origin: {
            /* readline: "glDeleteProgram_origin GLuint program" */
            /* func name: "glDeleteProgram_origin" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteProgram_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteProgram(opengl_context, program);
        } break;

        case FUNID_glDeleteShader: {
            /* readline: "glDeleteShader GLuint shader" */
            /* func name: "glDeleteShader" */
            /* args: [{'type': 'GLuint', 'name': 'shader', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLuint shader;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteShader)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteShader(opengl_context, shader);
        } break;

        case FUNID_glDeleteSync: {
            /* readline: "glDeleteSync GLsync sync" */
            /* func name: "glDeleteSync" */
            /* args: [{'type': 'GLsync', 'name': 'sync', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsync sync;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteSync)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sync = *(GLsync *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteSync(opengl_context, sync);
        } break;

        case FUNID_glDeleteFramebuffers: {
            /* readline: "glDeleteFramebuffers GLsizei n, const GLuint
             * *framebuffers#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteFramebuffers" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'framebuffers', 'ptr': 'in', 'ptr_len':
             * 'n*sizeof(GLuint)', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteFramebuffers)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *framebuffers = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteFramebuffers(opengl_context, n, framebuffers);
        } break;

        case FUNID_glDeleteProgramPipelines: {
            /* readline: "glDeleteProgramPipelines GLsizei n, const GLuint
             * *pipelines#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteProgramPipelines" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'pipelines', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteProgramPipelines)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *pipelines = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteProgramPipelines(opengl_context, n,
                                                   pipelines);
        } break;

        case FUNID_glDeleteTransformFeedbacks: {
            /* readline: "glDeleteTransformFeedbacks GLsizei n, const GLuint
             * *ids#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteTransformFeedbacks" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'ids', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)', 'loc':
             * 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteTransformFeedbacks)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *ids = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteTransformFeedbacks(opengl_context, n, ids);
        } break;

        case FUNID_glDeleteVertexArrays_origin: {
            /* readline: "glDeleteVertexArrays_origin GLsizei n, const GLuint
             * *arrays#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteVertexArrays_origin" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'arrays', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteVertexArrays_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *arrays = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteVertexArrays(opengl_context, n, arrays);
        } break;

        case FUNID_glDeleteQueries: {
            /* readline: "glDeleteQueries GLsizei n, const GLuint
             * *ids#n*sizeof(GLuint) @{if(n<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glDeleteQueries" */
            /* args: [{'type': 'GLsizei', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const GLuint*',
             * 'name': 'ids', 'ptr': 'in', 'ptr_len': 'n*sizeof(GLuint)', 'loc':
             * 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "211" */

            /* Define variables */
            GLsizei n;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDeleteQueries)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *ids = (const GLuint *)(temp + temp_loc);
            temp_loc += n * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLResource::d_glDeleteQueries(opengl_context, n, ids);
        } break;

            /******* end of file '2-1-1', 26/155 functions*******/

            /******* file '2-1-2' *******/

        case FUNID_glLinkProgram_special: {
            /* readline: "glLinkProgram_origin GLuint program int
             * *program_data_len" */
            /* func name: "glLinkProgram_origin" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glLinkProgram_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            ALOGD("context %llx link program %u host %u\n",
                  (uint64_t)opengl_context, program,
                  (GLuint)GLResource::GLResource::get_host_program_id(
                      opengl_context, (unsigned int)program));
            GLProgram::d_glLinkProgram_special(
                opengl_context,
                (GLuint)GLResource::GLResource::get_host_program_id(
                    opengl_context, (unsigned int)program));
        } break;

        case FUNID_glPixelStorei_origin: {
            /* readline: "glPixelStorei_origin GLenum pname, GLint param" */
            /* func name: "glPixelStorei_origin" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'param', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum pname;
            GLint param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPixelStorei_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLState::d_glPixelStorei_origin(opengl_context, pname, param);
        } break;

        case FUNID_glDisableVertexAttribArray_origin: {
            /* readline: "glDisableVertexAttribArray_origin GLuint index" */
            /* func name: "glDisableVertexAttribArray_origin" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDisableVertexAttribArray_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glDisableVertexAttribArray_origin(opengl_context, index);
        } break;

        case FUNID_glEnableVertexAttribArray_origin: {
            /* readline: "glEnableVertexAttribArray_origin GLuint index" */
            /* func name: "glEnableVertexAttribArray_origin" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glEnableVertexAttribArray_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glEnableVertexAttribArray_origin(opengl_context, index);
        } break;

        case FUNID_glReadBuffer_special: {
            /* readline: "glReadBuffer_special GLenum src" */
            /* func name: "glReadBuffer_special" */
            /* args: [{'type': 'GLenum', 'name': 'src', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum src;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glReadBuffer_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            src = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Texture::d_glReadBuffer_special(opengl_context, src);
        } break;

        case FUNID_glVertexAttribDivisor_origin: {
            /* readline: "glVertexAttribDivisor_origin GLuint index, GLuint
             * divisor" */
            /* func name: "glVertexAttribDivisor_origin" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'divisor', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLuint divisor;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glVertexAttribDivisor_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            divisor = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glVertexAttribDivisor_origin(opengl_context, index,
                                                   divisor);
        } break;

        case FUNID_glShaderSource_special: {
            /* readline: "glShaderSource_origin GLuint shader, GLsizei count,
             * const GLint *length#count*sizeof(GLint), const GLchar
             * *const*string#count|length[i]" */
            /* func name: "glShaderSource_origin" */
            /* args: [{'type': 'GLuint', 'name': 'shader', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLint*', 'name': 'length',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}, {'type': 'const GLchar*', 'name': 'string',
             * 'ptr': 'in', 'ptr_len': 'count|length[i]', 'loc': 3, 'ptr_ptr':
             * True}] */
            /* ret: "" */
            /* type: "212" */

            /* TODO: More than one ptr, should check manually */
            /* Define variables */
            GLuint shader;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glShaderSource_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            GLint *length = (GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint);

            GLchar **string = (GLchar **)malloc(count * sizeof(GLchar *));

            for (int i = 0; i < count; i++) {
                string[i] = (GLchar *)(temp + temp_loc);
                temp_loc += length[i];
            }
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            GLProgram::d_glShaderSource_special(
                opengl_context,
                (GLuint)GLResource::get_host_shader_id(opengl_context,
                                                       (unsigned int)shader),
                count, length, string);
            free(string);
        } break;

        case FUNID_glVertexAttribIPointer_with_bound: {
            /* readline: "glVertexAttribIPointer_with_bound GLuint index, GLint
             * size, GLenum type, GLsizei stride, GLintptr pointer" */
            /* func name: "glVertexAttribIPointer_with_bound" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'type', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'stride', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLintptr', 'name': 'pointer',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLint size;
            GLenum type;
            GLsizei stride;
            GLintptr pointer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glVertexAttribIPointer_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            stride = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            pointer = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glVertexAttribIPointer_with_bound(
                opengl_context, index, size, type, stride, pointer);
        } break;

        case FUNID_glVertexAttribIPointer_offset: {
            /* readline: "glVertexAttribIPointer_offset GLuint index, GLint
             * size, GLenum type, GLsizei stride, GLuint index_father, GLintptr
             * offset" */
            /* func name: "glVertexAttribIPointer_offset" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'type', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'stride', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'index_father',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False},
             * {'type': 'GLintptr', 'name': 'offset', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLint size;
            GLenum type;
            GLsizei stride;
            GLuint index_father;
            GLintptr offset;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glVertexAttribIPointer_offset)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 28 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            stride = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            index_father = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glVertexAttribIPointer_offset(opengl_context, index, size,
                                                    type, stride, index_father,
                                                    offset);
        } break;

        case FUNID_glBindVertexArray_special: {
            /* readline: "glBindVertexArray_special GLuint array" */
            /* func name: "glBindVertexArray_special" */
            /* args: [{'type': 'GLuint', 'name': 'array', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint array;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindVertexArray_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            array = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            ALOGD("context %llx bind vertexArray guest %u host %u\n",
                  (uint64_t)opengl_context, array,
                  (GLuint)GLResource::get_host_array_id(opengl_context,
                                                        (unsigned int)array));
            GLState::d_glBindVertexArray_special(opengl_context, array);
        } break;

        case FUNID_glBindBuffer_origin: {
            /* readline: "glBindBuffer_origin GLenum target, GLuint buffer" */
            /* func name: "glBindBuffer_origin" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'buffer', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLuint buffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindBuffer_origin)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            buffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            ALOGD("glbindbuffer target %x %d\n", target, buffer);
            GLState::d_glBindBuffer_special(opengl_context, target, buffer);
        } break;

        case FUNID_glBeginTransformFeedback: {
            /* readline: "glBeginTransformFeedback GLenum primitiveMode" */
            /* func name: "glBeginTransformFeedback" */
            /* args: [{'type': 'GLenum', 'name': 'primitiveMode', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum primitiveMode;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBeginTransformFeedback)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            primitiveMode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBeginTransformFeedback(primitiveMode);
        } break;

        case FUNID_glEndTransformFeedback: {
            /* readline: "glEndTransformFeedback void" */
            /* func name: "glEndTransformFeedback" */
            /* args: [] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glEndTransformFeedback)) {
                break;
            }

            glEndTransformFeedback();
        } break;

        case FUNID_glPauseTransformFeedback: {
            /* readline: "glPauseTransformFeedback void" */
            /* func name: "glPauseTransformFeedback" */
            /* args: [] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPauseTransformFeedback)) {
                break;
            }

            glPauseTransformFeedback();
        } break;

        case FUNID_glResumeTransformFeedback: {
            /* readline: "glResumeTransformFeedback void" */
            /* func name: "glResumeTransformFeedback" */
            /* args: [] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glResumeTransformFeedback)) {
                break;
            }

            glResumeTransformFeedback();
        } break;

        case FUNID_glBindBufferRange: {
            /* readline: "glBindBufferRange GLenum target, GLuint index, GLuint
             * buffer, GLintptr offset, GLsizeiptr size" */
            /* func name: "glBindBufferRange" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'buffer', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'offset', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizeiptr', 'name':
             * 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLuint index;
            GLuint buffer;
            GLintptr offset;
            GLsizeiptr size;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindBufferRange)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 28 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            buffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            size = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            ALOGD(
                "mapbufferrange glbindbufferRange target %x index %d buffer %d "
                "offset %lld size %lld end %lld\n",
                target, index,
                (GLuint)GLResource::GLResource::get_host_buffer_id(
                    opengl_context, (unsigned int)buffer),
                offset, size, offset + size);

            GLState::d_glBindBufferRange_special(opengl_context, target, index,
                                                 buffer, offset, size);
        } break;

        case FUNID_glBindBufferBase: {
            /* readline: "glBindBufferBase GLenum target, GLuint index, GLuint
             * buffer" */
            /* func name: "glBindBufferBase" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'buffer', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLuint index;
            GLuint buffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindBufferBase)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            buffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLState::d_glBindBufferBase_special(opengl_context, target, index,
                                                buffer);
        } break;

        case FUNID_glBindTexture: {
            /* readline: "glBindTexture GLenum target, GLuint texture" */
            /* func name: "glBindTexture" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'texture', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLuint texture;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindTexture)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            texture = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            // GLint pre_texture;
            // glGetIntegerv(GL_TEXTURE_BINDING_2D,&pre_texture);
            // static int cnt = 0;
            // cnt ++;
            // ALOGD("context %llx cnt %d bindtexture target %x guest %u host %u
            // pre
            // %u\n",(uint64_t)opengl_context,cnt,target,texture,(GLuint)GLResource::get_host_texture_id(opengl_context,
            // (unsigned int)texture),pre_texture);
            GLState::d_glBindTexture_special(opengl_context, target, texture);
        } break;

        case FUNID_glBindRenderbuffer: {
            /* readline: "glBindRenderbuffer GLenum target, GLuint renderbuffer"
             */
            /* func name: "glBindRenderbuffer" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'renderbuffer', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLuint renderbuffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindRenderbuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            renderbuffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBindRenderbuffer(target,
                               (GLuint)GLResource::get_host_renderbuffer_id(
                                   opengl_context, (unsigned int)renderbuffer));
        } break;

        case FUNID_glBindSampler: {
            /* readline: "glBindSampler GLuint unit, GLuint sampler" */
            /* func name: "glBindSampler" */
            /* args: [{'type': 'GLuint', 'name': 'unit', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'sampler', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint unit;
            GLuint sampler;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindSampler)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            unit = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            sampler = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBindSampler(unit, (GLuint)GLResource::get_host_sampler_id(
                                    opengl_context, (unsigned int)sampler));
        } break;

        case FUNID_glBindFramebuffer: {
            /* readline: "glBindFramebuffer GLenum target, GLuint framebuffer"
             */
            /* func name: "glBindFramebuffer" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'framebuffer', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLuint framebuffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindFramebuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            framebuffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            ALOGD("context %llx glBindFramebuffer %x guest %d host %d\n",
                  (uint64_t)opengl_context, target, framebuffer,
                  (GLuint)GLResource::get_host_framebuffer_id(
                      opengl_context, (unsigned int)framebuffer));
            GLState::d_glBindFramebuffer_special(
                opengl_context, target,
                (GLuint)GLResource::get_host_framebuffer_id(
                    opengl_context, (unsigned int)framebuffer));
        } break;

        case FUNID_glBindProgramPipeline: {
            /* readline: "glBindProgramPipeline GLuint pipeline" */
            /* func name: "glBindProgramPipeline" */
            /* args: [{'type': 'GLuint', 'name': 'pipeline', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint pipeline;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindProgramPipeline)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pipeline = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBindProgramPipeline((GLuint)GLResource::get_host_pipeline_id(
                opengl_context, (unsigned int)pipeline));
        } break;

        case FUNID_glBindTransformFeedback: {
            /* readline: "glBindTransformFeedback GLenum target, GLuint
             * feedback_id" */
            /* func name: "glBindTransformFeedback" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'feedback_id', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLuint feedback_id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindTransformFeedback)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            feedback_id = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBindTransformFeedback(
                target, (GLuint)GLResource::get_host_feedback_id(
                            opengl_context, (unsigned int)feedback_id));
        } break;

        case FUNID_glBindEGLImage: {
            /* Define variables */
            GLenum target;
            uint64_t image;
            GLuint texture;
            GLuint share_texture;
            EGLContext share_ctx;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindEGLImage)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < (28) * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            image = *(uint64_t *)(temp + temp_loc);
            temp_loc += 8;

            texture = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            share_texture = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            share_ctx = *(EGLContext *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            
            GLState::d_glBindEGLImage(thread_context, target, image, texture,
                                      share_texture, share_ctx);
        } break;

        case FUNID_glActiveTexture: {
            /* readline: "glActiveTexture GLenum texture" */
            /* func name: "glActiveTexture" */
            /* args: [{'type': 'GLenum', 'name': 'texture', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum texture;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glActiveTexture)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            texture = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            // ALOGD("context %llx ActiveTexture %d\n",(uint64_t)opengl_context,
            // texture-GL_TEXTURE0);
            GLState::d_glActiveTexture_special(opengl_context, texture);
        } break;

        case FUNID_glAttachShader: {
            /* readline: "glAttachShader GLuint program, GLuint shader" */
            /* func name: "glAttachShader" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'shader', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLuint shader;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glAttachShader)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            ALOGD("attach shader %u to program %u\n",
                  (GLuint)GLResource::get_host_shader_id(opengl_context,
                                                         (unsigned int)shader),
                  (GLuint)GLResource::get_host_program_id(
                      opengl_context, (unsigned int)program));
            glAttachShader((GLuint)GLResource::get_host_program_id(
                               opengl_context, (unsigned int)program),
                           (GLuint)GLResource::get_host_shader_id(
                               opengl_context, (unsigned int)shader));
        } break;

        case FUNID_glBlendColor: {
            /* readline: "glBlendColor GLfloat red, GLfloat green, GLfloat blue,
             * GLfloat alpha" */
            /* func name: "glBlendColor" */
            /* args: [{'type': 'GLfloat', 'name': 'red', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'green', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'blue', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'alpha', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat red;
            GLfloat green;
            GLfloat blue;
            GLfloat alpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBlendColor)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            red = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            green = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            blue = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            alpha = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBlendColor(red, green, blue, alpha);
        } break;

        case FUNID_glBlendEquation: {
            /* readline: "glBlendEquation GLenum mode" */
            /* func name: "glBlendEquation" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum mode;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBlendEquation)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBlendEquation(mode);
        } break;

        case FUNID_glBlendEquationSeparate: {
            /* readline: "glBlendEquationSeparate GLenum modeRGB, GLenum
             * modeAlpha" */
            /* func name: "glBlendEquationSeparate" */
            /* args: [{'type': 'GLenum', 'name': 'modeRGB', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'modeAlpha', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum modeRGB;
            GLenum modeAlpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBlendEquationSeparate)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            modeRGB = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            modeAlpha = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBlendEquationSeparate(modeRGB, modeAlpha);
        } break;

        case FUNID_glBlendFunc: {
            /* readline: "glBlendFunc GLenum sfactor, GLenum dfactor" */
            /* func name: "glBlendFunc" */
            /* args: [{'type': 'GLenum', 'name': 'sfactor', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'dfactor', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum sfactor;
            GLenum dfactor;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBlendFunc)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sfactor = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            dfactor = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBlendFunc(sfactor, dfactor);
        } break;

        case FUNID_glBlendFuncSeparate: {
            /* readline: "glBlendFuncSeparate GLenum sfactorRGB, GLenum
             * dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha" */
            /* func name: "glBlendFuncSeparate" */
            /* args: [{'type': 'GLenum', 'name': 'sfactorRGB', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'dfactorRGB', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'sfactorAlpha',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLenum', 'name': 'dfactorAlpha', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum sfactorRGB;
            GLenum dfactorRGB;
            GLenum sfactorAlpha;
            GLenum dfactorAlpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBlendFuncSeparate)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sfactorRGB = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            dfactorRGB = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            sfactorAlpha = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            dfactorAlpha = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha,
                                dfactorAlpha);
        } break;

        case FUNID_glClear: {
            /* readline: "glClear GLbitfield mask" */
            /* func name: "glClear" */
            /* args: [{'type': 'GLbitfield', 'name': 'mask', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLbitfield mask;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClear)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mask = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glClear(mask);
        } break;

        case FUNID_glClearColor: {
            /* readline: "glClearColor GLfloat red, GLfloat green, GLfloat blue,
             * GLfloat alpha" */
            /* func name: "glClearColor" */
            /* args: [{'type': 'GLfloat', 'name': 'red', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'green', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'blue', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'alpha', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat red;
            GLfloat green;
            GLfloat blue;
            GLfloat alpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearColor)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            red = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            green = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            blue = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            alpha = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glClearColor(red, green, blue, alpha);
        } break;

        case FUNID_glClearDepthf: {
            /* readline: "glClearDepthf GLfloat d" */
            /* func name: "glClearDepthf" */
            /* args: [{'type': 'GLfloat', 'name': 'd', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat d;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearDepthf)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            d = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glClearDepthf(d);
        } break;

        case FUNID_glClearStencil: {
            /* readline: "glClearStencil GLint s" */
            /* func name: "glClearStencil" */
            /* args: [{'type': 'GLint', 'name': 's', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint s;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearStencil)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            s = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glClearStencil(s);
        } break;

        case FUNID_glColorMask: {
            /* readline: "glColorMask GLboolean red, GLboolean green, GLboolean
             * blue, GLboolean alpha" */
            /* func name: "glColorMask" */
            /* args: [{'type': 'GLboolean', 'name': 'red', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'green', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'GLboolean', 'name':
             * 'blue', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'GLboolean', 'name': 'alpha', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLboolean red;
            GLboolean green;
            GLboolean blue;
            GLboolean alpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glColorMask)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            red = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            green = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            blue = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            alpha = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glColorMask(red, green, blue, alpha);
        } break;

        case FUNID_glCompileShader: {
            /* readline: "glCompileShader GLuint shader" */
            /* func name: "glCompileShader" */
            /* args: [{'type': 'GLuint', 'name': 'shader', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint shader;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCompileShader)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLProgram::d_glCompileShader_special(opengl_context, shader);
        } break;

        case FUNID_glCullFace: {
            /* readline: "glCullFace GLenum mode" */
            /* func name: "glCullFace" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum mode;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCullFace)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glCullFace(mode);
        } break;

        case FUNID_glDepthFunc: {
            /* readline: "glDepthFunc GLenum func" */
            /* func name: "glDepthFunc" */
            /* args: [{'type': 'GLenum', 'name': 'func', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum func;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDepthFunc)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            func = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glDepthFunc(func);
        } break;

        case FUNID_glDepthMask: {
            /* readline: "glDepthMask GLboolean flag" */
            /* func name: "glDepthMask" */
            /* args: [{'type': 'GLboolean', 'name': 'flag', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLboolean flag;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDepthMask)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            flag = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glDepthMask(flag);
        } break;

        case FUNID_glDepthRangef: {
            /* readline: "glDepthRangef GLfloat n, GLfloat f" */
            /* func name: "glDepthRangef" */
            /* args: [{'type': 'GLfloat', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'f', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat n;
            GLfloat f;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDepthRangef)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            f = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glDepthRangef(n, f);
        } break;

        case FUNID_glDetachShader: {
            /* readline: "glDetachShader GLuint program, GLuint shader" */
            /* func name: "glDetachShader" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'shader', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLuint shader;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDetachShader)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            shader = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glDetachShader((GLuint)GLResource::GLResource::get_host_program_id(
                               opengl_context, (unsigned int)program),
                           (GLuint)GLResource::get_host_shader_id(
                               opengl_context, (unsigned int)shader));
        } break;

        case FUNID_glDisable: {
            /* readline: "glDisable GLenum cap" */
            /* func name: "glDisable" */
            /* args: [{'type': 'GLenum', 'name': 'cap', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum cap;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDisable)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            cap = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            if (cap == GL_SCISSOR_TEST && opengl_context != NULL) {
                opengl_context->enable_scissor = 0;
            }
            glDisable(cap);
        } break;

        case FUNID_glEnable: {
            /* readline: "glEnable GLenum cap" */
            /* func name: "glEnable" */
            /* args: [{'type': 'GLenum', 'name': 'cap', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum cap;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glEnable)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            cap = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (cap == GL_SCISSOR_TEST && opengl_context != NULL) {
                opengl_context->enable_scissor = 1;
            }

            glEnable(cap);
        } break;

        case FUNID_glFramebufferRenderbuffer: {
            /* readline: "glFramebufferRenderbuffer GLenum target, GLenum
             * attachment, GLenum renderbuffertarget, GLuint renderbuffer" */
            /* func name: "glFramebufferRenderbuffer" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'attachment', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'renderbuffertarget', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2,
             * 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'renderbuffer',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum attachment;
            GLenum renderbuffertarget;
            GLuint renderbuffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFramebufferRenderbuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            attachment = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            renderbuffertarget = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            renderbuffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glFramebufferRenderbuffer(
                target, attachment, renderbuffertarget,
                (GLuint)GLResource::GLResource::get_host_renderbuffer_id(
                    opengl_context, (unsigned int)renderbuffer));
        } break;

        case FUNID_glFramebufferTexture2D: {
            /* readline: "glFramebufferTexture2D GLenum target, GLenum
             * attachment, GLenum textarget, GLuint texture, GLint level" */
            /* func name: "glFramebufferTexture2D" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'attachment', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'textarget', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLuint', 'name': 'texture', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'level', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum attachment;
            GLenum textarget;
            GLuint texture;
            GLint level;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFramebufferTexture2D)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            attachment = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            textarget = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            texture = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            // GLuint t;
            // glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint *)&t);
            // ALOGD("#%llx FramebufferTexture2D %u target %x attachment %x
            // textarget %x guest %u texture %u level
            // %d\n",(uint64_t)opengl_context,t,target, attachment, textarget,
            // texture, (GLuint)GLResource::get_host_texture_id(opengl_context,
            // (unsigned int)texture), level);
            Texture::d_glFramebufferTexture2D_special(
                opengl_context, target, attachment, textarget, texture, level);
            // glFramebufferTexture2D(target, attachment, textarget,
            // (GLuint)GLResource::get_host_texture_id(opengl_context,
            // (unsigned int)texture), level);
        } break;

        case FUNID_glFrontFace: {
            /* readline: "glFrontFace GLenum mode" */
            /* func name: "glFrontFace" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum mode;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFrontFace)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glFrontFace(mode);
        } break;

        case FUNID_glGenerateMipmap: {
            /* readline: "glGenerateMipmap GLenum target" */
            /* func name: "glGenerateMipmap" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGenerateMipmap)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    glGenerateTextureMipmap(bind_texture);
                ALOGE("DSA not supported.");
            } else {
                glGenerateMipmap(target);
            }
        } break;

        case FUNID_glHint: {
            /* readline: "glHint GLenum target, GLenum mode" */
            /* func name: "glHint" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'mode', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum mode;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glHint)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glHint(target, mode);
        } break;

        case FUNID_glLineWidth: {
            /* readline: "glLineWidth GLfloat width" */
            /* func name: "glLineWidth" */
            /* args: [{'type': 'GLfloat', 'name': 'width', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat width;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glLineWidth)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            width = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glLineWidth(width);
        } break;

        case FUNID_glPolygonOffset: {
            /* readline: "glPolygonOffset GLfloat factor, GLfloat units" */
            /* func name: "glPolygonOffset" */
            /* args: [{'type': 'GLfloat', 'name': 'factor', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'units', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat factor;
            GLfloat units;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPolygonOffset)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            factor = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            units = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glPolygonOffset(factor, units);
        } break;

        case FUNID_glReleaseShaderCompiler: {
            /* readline: "glReleaseShaderCompiler void" */
            /* func name: "glReleaseShaderCompiler" */
            /* args: [] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glReleaseShaderCompiler)) {
                break;
            }

            glReleaseShaderCompiler();
        } break;

        case FUNID_glRenderbufferStorage: {
            /* readline: "glRenderbufferStorage GLenum target, GLenum
             * internalformat, GLsizei width, GLsizei height" */
            /* func name: "glRenderbufferStorage" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'internalformat', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'width', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glRenderbufferStorage)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glRenderbufferStorage(target, internalformat, width, height);
        } break;

        case FUNID_glSampleCoverage: {
            /* readline: "glSampleCoverage GLfloat value, GLboolean invert" */
            /* func name: "glSampleCoverage" */
            /* args: [{'type': 'GLfloat', 'name': 'value', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'invert', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat value;
            GLboolean invert;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glSampleCoverage)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            value = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            invert = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glSampleCoverage(value, invert);
        } break;

        case FUNID_glScissor: {
            /* readline: "glScissor GLint x, GLint y, GLsizei width, GLsizei
             * height" */
            /* func name: "glScissor" */
            /* args: [{'type': 'GLint', 'name': 'x', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'y',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glScissor)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glScissor(x, y, width, height);
        } break;

        case FUNID_glStencilFunc: {
            /* readline: "glStencilFunc GLenum func, GLint ref, GLuint mask" */
            /* func name: "glStencilFunc" */
            /* args: [{'type': 'GLenum', 'name': 'func', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name':
             * 'ref', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLuint', 'name': 'mask', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum func;
            GLint ref;
            GLuint mask;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glStencilFunc)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            func = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            ref = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            mask = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glStencilFunc(func, ref, mask);
        } break;

        case FUNID_glStencilFuncSeparate: {
            /* readline: "glStencilFuncSeparate GLenum face, GLenum func, GLint
             * ref, GLuint mask" */
            /* func name: "glStencilFuncSeparate" */
            /* args: [{'type': 'GLenum', 'name': 'face', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'func', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'ref', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'mask', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum face;
            GLenum func;
            GLint ref;
            GLuint mask;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glStencilFuncSeparate)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            face = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            func = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            ref = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            mask = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glStencilFuncSeparate(face, func, ref, mask);
        } break;

        case FUNID_glStencilMask: {
            /* readline: "glStencilMask GLuint mask" */
            /* func name: "glStencilMask" */
            /* args: [{'type': 'GLuint', 'name': 'mask', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint mask;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glStencilMask)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mask = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glStencilMask(mask);
        } break;

        case FUNID_glStencilMaskSeparate: {
            /* readline: "glStencilMaskSeparate GLenum face, GLuint mask" */
            /* func name: "glStencilMaskSeparate" */
            /* args: [{'type': 'GLenum', 'name': 'face', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'mask', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum face;
            GLuint mask;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glStencilMaskSeparate)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            face = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            mask = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glStencilMaskSeparate(face, mask);
        } break;

        case FUNID_glStencilOp: {
            /* readline: "glStencilOp GLenum fail, GLenum zfail, GLenum zpass"
             */
            /* func name: "glStencilOp" */
            /* args: [{'type': 'GLenum', 'name': 'fail', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'zfail', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'zpass', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum fail;
            GLenum zfail;
            GLenum zpass;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glStencilOp)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            fail = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            zfail = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            zpass = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glStencilOp(fail, zfail, zpass);
        } break;

        case FUNID_glStencilOpSeparate: {
            /* readline: "glStencilOpSeparate GLenum face, GLenum sfail, GLenum
             * dpfail, GLenum dppass" */
            /* func name: "glStencilOpSeparate" */
            /* args: [{'type': 'GLenum', 'name': 'face', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'sfail', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'dpfail', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'dppass', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum face;
            GLenum sfail;
            GLenum dpfail;
            GLenum dppass;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glStencilOpSeparate)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            face = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            sfail = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            dpfail = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            dppass = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glStencilOpSeparate(face, sfail, dpfail, dppass);
        } break;

        case FUNID_glTexParameterf: {
            /* readline: "glTexParameterf GLenum target, GLenum pname, GLfloat
             * param" */
            /* func name: "glTexParameterf" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;
            GLfloat param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexParameterf)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    glTextureParameterf(bind_texture, pname, param);
                ALOGE("DSA not supported.");
            } else {
                if (target == GL_TEXTURE_EXTERNAL_OES) {
                    TextureBindingStatus *texture_status =
                        &(opengl_context->texture_binding_status);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(GL_TEXTURE0);
                    }
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->current_texture_external);
                    glTexParameterf(GL_TEXTURE_2D, pname, param);
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->host_current_texture_2D[0]);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(
                            texture_status->host_current_active_texture +
                            GL_TEXTURE0);
                    }
                } else {
                    glTexParameterf(target, pname, param);
                }
            }
        } break;

        case FUNID_glTexParameteri: {
            /* readline: "glTexParameteri GLenum target, GLenum pname, GLint
             * param" */
            /* func name: "glTexParameteri" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;
            GLint param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexParameteri)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    glTextureParameteri(bind_texture, pname, param);
                ALOGE("DSA not supported.");
            } else {
                if (target == GL_TEXTURE_EXTERNAL_OES) {
                    TextureBindingStatus *texture_status =
                        &(opengl_context->texture_binding_status);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(GL_TEXTURE0);
                    }
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->current_texture_external);
                    glTexParameteri(GL_TEXTURE_2D, pname, param);
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->host_current_texture_2D[0]);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(
                            texture_status->host_current_active_texture +
                            GL_TEXTURE0);
                    }
                } else {
                    glTexParameteri(target, pname, param);
                }
            }
        } break;

        case FUNID_glUniform1f: {
            /* readline: "glUniform1f GLint location, GLfloat v0" */
            /* func name: "glUniform1f" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLfloat v0;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform1f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform1f(location, v0);
        } break;

        case FUNID_glUniform1i: {
            /* readline: "glUniform1i GLint location, GLint v0" */
            /* func name: "glUniform1i" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLint v0;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform1i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform1i(location, v0);
        } break;

        case FUNID_glUniform2f: {
            /* readline: "glUniform2f GLint location, GLfloat v0, GLfloat v1" */
            /* func name: "glUniform2f" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLfloat v0;
            GLfloat v1;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform2f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform2f(location, v0, v1);
        } break;

        case FUNID_glUniform2i: {
            /* readline: "glUniform2i GLint location, GLint v0, GLint v1" */
            /* func name: "glUniform2i" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLint v0;
            GLint v1;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform2i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform2i(location, v0, v1);
        } break;

        case FUNID_glUniform3f: {
            /* readline: "glUniform3f GLint location, GLfloat v0, GLfloat v1,
             * GLfloat v2" */
            /* func name: "glUniform3f" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'v2', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLfloat v0;
            GLfloat v1;
            GLfloat v2;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform3f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform3f(location, v0, v1, v2);
        } break;

        case FUNID_glUniform3i: {
            /* readline: "glUniform3i GLint location, GLint v0, GLint v1, GLint
             * v2" */
            /* func name: "glUniform3i" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLint', 'name':
             * 'v2', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLint v0;
            GLint v1;
            GLint v2;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform3i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform3i(location, v0, v1, v2);
        } break;

        case FUNID_glUniform4f: {
            /* readline: "glUniform4f GLint location, GLfloat v0, GLfloat v1,
             * GLfloat v2, GLfloat v3" */
            /* func name: "glUniform4f" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'v2', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLfloat', 'name': 'v3', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLfloat v0;
            GLfloat v1;
            GLfloat v2;
            GLfloat v3;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform4f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v3 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform4f(location, v0, v1, v2, v3);
        } break;

        case FUNID_glUniform4i: {
            /* readline: "glUniform4i GLint location, GLint v0, GLint v1, GLint
             * v2, GLint v3" */
            /* func name: "glUniform4i" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLint', 'name':
             * 'v2', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLint', 'name': 'v3', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLint v0;
            GLint v1;
            GLint v2;
            GLint v3;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform4i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v3 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform4i(location, v0, v1, v2, v3);
        } break;

        case FUNID_glUseProgram: {
            /* readline: "glUseProgram GLuint program" */
            /* func name: "glUseProgram" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUseProgram)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            ALOGD("context %llx use program guest %u host %u\n",
                  (uint64_t)opengl_context, program,
                  (GLuint)GLResource::get_host_program_id(
                      opengl_context, (unsigned int)program));
            GLProgram::d_glUseProgram_special(
                opengl_context, (GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program));
        } break;

        case FUNID_glValidateProgram: {
            /* readline: "glValidateProgram GLuint program" */
            /* func name: "glValidateProgram" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glValidateProgram)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glValidateProgram((GLuint)GLResource::get_host_program_id(
                opengl_context, (unsigned int)program));
        } break;

        case FUNID_glVertexAttrib1f: {
            /* readline: "glVertexAttrib1f GLuint index, GLfloat x" */
            /* func name: "glVertexAttrib1f" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLfloat x;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttrib1f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttrib1f(index, x);
        } break;

        case FUNID_glVertexAttrib2f: {
            /* readline: "glVertexAttrib2f GLuint index, GLfloat x, GLfloat y"
             */
            /* func name: "glVertexAttrib2f" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'y', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLfloat x;
            GLfloat y;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttrib2f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttrib2f(index, x, y);
        } break;

        case FUNID_glVertexAttrib3f: {
            /* readline: "glVertexAttrib3f GLuint index, GLfloat x, GLfloat y,
             * GLfloat z" */
            /* func name: "glVertexAttrib3f" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'y', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'z', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLfloat x;
            GLfloat y;
            GLfloat z;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttrib3f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            z = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttrib3f(index, x, y, z);
        } break;

        case FUNID_glVertexAttrib4f: {
            /* readline: "glVertexAttrib4f GLuint index, GLfloat x, GLfloat y,
             * GLfloat z, GLfloat w" */
            /* func name: "glVertexAttrib4f" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'y', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'z', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLfloat', 'name': 'w', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLfloat x;
            GLfloat y;
            GLfloat z;
            GLfloat w;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttrib4f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            z = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            w = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttrib4f(index, x, y, z, w);
        } break;

        case FUNID_glBlitFramebuffer: {
            /* readline: "glBlitFramebuffer GLint srcX0, GLint srcY0, GLint
             * srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint
             * dstY1, GLbitfield mask, GLenum filter" */
            /* func name: "glBlitFramebuffer" */
            /* args: [{'type': 'GLint', 'name': 'srcX0', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name':
             * 'srcY0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'srcX1', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'srcY1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'dstX0', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'dstY0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'dstX1', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'dstY1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 7,
             * 'ptr_ptr': False}, {'type': 'GLbitfield', 'name': 'mask', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr': False}, {'type':
             * 'GLenum', 'name': 'filter', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 9, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint srcX0;
            GLint srcY0;
            GLint srcX1;
            GLint srcY1;
            GLint dstX0;
            GLint dstY0;
            GLint dstX1;
            GLint dstY1;
            GLbitfield mask;
            GLenum filter;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBlitFramebuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 40 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            srcX0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            srcY0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            srcX1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            srcY1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            dstX0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            dstY0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            dstX1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            dstY1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            mask = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            filter = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            // ALOGD("context %llx glBlitFramebuffer\n");
            glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1,
                              dstY1, mask, filter);
        } break;

        case FUNID_glRenderbufferStorageMultisample: {
            /* readline: "glRenderbufferStorageMultisample GLenum target,
             * GLsizei samples, GLenum internalformat, GLsizei width, GLsizei
             * height" */
            /* func name: "glRenderbufferStorageMultisample" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'samples', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLsizei samples;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glRenderbufferStorageMultisample)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            samples = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glRenderbufferStorageMultisample(target, samples, internalformat,
                                             width, height);
        } break;

        case FUNID_glFramebufferTextureLayer: {
            /* readline: "glFramebufferTextureLayer GLenum target, GLenum
             * attachment, GLuint texture, GLint level, GLint layer" */
            /* func name: "glFramebufferTextureLayer" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'attachment', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'texture', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'layer', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum attachment;
            GLuint texture;
            GLint level;
            GLint layer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFramebufferTextureLayer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            attachment = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            texture = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            layer = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glFramebufferTextureLayer(
                target, attachment,
                (GLuint)GLResource::get_host_texture_id(opengl_context,
                                                        (unsigned int)texture),
                level, layer);
        } break;

        case FUNID_glVertexAttribI4i: {
            /* readline: "glVertexAttribI4i GLuint index, GLint x, GLint y,
             * GLint z, GLint w" */
            /* func name: "glVertexAttribI4i" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'y', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'z',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLint', 'name': 'w', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLint x;
            GLint y;
            GLint z;
            GLint w;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttribI4i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            z = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            w = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttribI4i(index, x, y, z, w);
        } break;

        case FUNID_glVertexAttribI4ui: {
            /* readline: "glVertexAttribI4ui GLuint index, GLuint x, GLuint y,
             * GLuint z, GLuint w" */
            /* func name: "glVertexAttribI4ui" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'y', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'z', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLuint', 'name': 'w', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;
            GLuint x;
            GLuint y;
            GLuint z;
            GLuint w;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttribI4ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            z = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            w = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttribI4ui(index, x, y, z, w);
        } break;

        case FUNID_glUniform1ui: {
            /* readline: "glUniform1ui GLint location, GLuint v0" */
            /* func name: "glUniform1ui" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLuint v0;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform1ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform1ui(location, v0);
        } break;

        case FUNID_glUniform2ui: {
            /* readline: "glUniform2ui GLint location, GLuint v0, GLuint v1" */
            /* func name: "glUniform2ui" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLuint v0;
            GLuint v1;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform2ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform2ui(location, v0, v1);
        } break;

        case FUNID_glUniform3ui: {
            /* readline: "glUniform3ui GLint location, GLuint v0, GLuint v1,
             * GLuint v2" */
            /* func name: "glUniform3ui" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'v2', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLuint v0;
            GLuint v1;
            GLuint v2;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform3ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform3ui(location, v0, v1, v2);
        } break;

        case FUNID_glUniform4ui: {
            /* readline: "glUniform4ui GLint location, GLuint v0, GLuint v1,
             * GLuint v2, GLuint v3" */
            /* func name: "glUniform4ui" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'v0', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'v1', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'v2', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLuint', 'name': 'v3', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLuint v0;
            GLuint v1;
            GLuint v2;
            GLuint v3;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform4ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v3 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform4ui(location, v0, v1, v2, v3);
        } break;

        case FUNID_glClearBufferfi: {
            /* readline: "glClearBufferfi GLenum buffer, GLint drawbuffer,
             * GLfloat depth, GLint stencil" */
            /* func name: "glClearBufferfi" */
            /* args: [{'type': 'GLenum', 'name': 'buffer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'drawbuffer', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'depth', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'stencil', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum buffer;
            GLint drawbuffer;
            GLfloat depth;
            GLint stencil;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearBufferfi)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            buffer = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            drawbuffer = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            stencil = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glClearBufferfi(buffer, drawbuffer, depth, stencil);
        } break;

        case FUNID_glCopyBufferSubData: {
            /* readline: "glCopyBufferSubData GLenum readTarget, GLenum
             * writeTarget, GLintptr readOffset, GLintptr writeOffset,
             * GLsizeiptr size" */
            /* func name: "glCopyBufferSubData" */
            /* args: [{'type': 'GLenum', 'name': 'readTarget', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'writeTarget', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLintptr', 'name': 'readOffset',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLintptr', 'name': 'writeOffset', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}, {'type':
             * 'GLsizeiptr', 'name': 'size', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum readTarget;
            GLenum writeTarget;
            GLintptr readOffset;
            GLintptr writeOffset;
            GLsizeiptr size;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glCopyBufferSubData)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 32 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            readTarget = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            writeTarget = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            readOffset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            writeOffset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            size = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_buffer1 = GLState::get_guest_binding_texture(
                    opengl_context, readTarget);
                GLuint bind_buffer2 = GLState::get_guest_binding_texture(
                    opengl_context, writeTarget);
                //    glCopyNamedBufferSubData(bind_buffer1, bind_buffer2,
                //    readOffset, writeOffset, size);
                ALOGE("DSA not supported.");
            } else {
                glCopyBufferSubData(readTarget, writeTarget, readOffset,
                                    writeOffset, size);
            }
        } break;

        case FUNID_glUniformBlockBinding: {
            /* readline: "glUniformBlockBinding GLuint program, GLuint
             * uniformBlockIndex, GLuint uniformBlockBinding" */
            /* func name: "glUniformBlockBinding" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'uniformBlockIndex', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'uniformBlockBinding', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLuint uniformBlockIndex;
            GLuint uniformBlockBinding;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformBlockBinding)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            uniformBlockIndex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            uniformBlockBinding = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformBlockBinding((GLuint)GLResource::get_host_program_id(
                                      opengl_context, (unsigned int)program),
                                  uniformBlockIndex, uniformBlockBinding);
        } break;

        case FUNID_glSamplerParameteri: {
            /* readline: "glSamplerParameteri GLuint sampler, GLenum pname,
             * GLint param" */
            /* func name: "glSamplerParameteri" */
            /* args: [{'type': 'GLuint', 'name': 'sampler', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint sampler;
            GLenum pname;
            GLint param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glSamplerParameteri)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sampler = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glSamplerParameteri((GLuint)GLResource::get_host_sampler_id(
                                    opengl_context, (unsigned int)sampler),
                                pname, param);
        } break;

        case FUNID_glSamplerParameterf: {
            /* readline: "glSamplerParameterf GLuint sampler, GLenum pname,
             * GLfloat param" */
            /* func name: "glSamplerParameterf" */
            /* args: [{'type': 'GLuint', 'name': 'sampler', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint sampler;
            GLenum pname;
            GLfloat param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glSamplerParameterf)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sampler = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glSamplerParameterf((GLuint)GLResource::get_host_sampler_id(
                                    opengl_context, (unsigned int)sampler),
                                pname, param);
        } break;

        case FUNID_glProgramParameteri: {
            /* readline: "glProgramParameteri GLuint program, GLenum pname,
             * GLint value" */
            /* func name: "glProgramParameteri" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'value', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLenum pname;
            GLint value;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramParameteri)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            value = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramParameteri((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                pname, value);
        } break;

        case FUNID_glAlphaFuncxOES: {
            /* readline: "glAlphaFuncxOES GLenum func, GLfixed ref" */
            /* func name: "glAlphaFuncxOES" */
            /* args: [{'type': 'GLenum', 'name': 'func', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'ref', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum func;
            GLfixed ref;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glAlphaFuncxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            func = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            ref = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glAlphaFuncxOES(func, ref);
            ALOGE("glAlphaFuncxOES not implemented.");
        } break;

        case FUNID_glClearColorxOES: {
            /* readline: "glClearColorxOES GLfixed red, GLfixed green, GLfixed
             * blue, GLfixed alpha" */
            /* func name: "glClearColorxOES" */
            /* args: [{'type': 'GLfixed', 'name': 'red', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'green', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfixed', 'name': 'blue', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfixed',
             * 'name': 'alpha', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed red;
            GLfixed green;
            GLfixed blue;
            GLfixed alpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearColorxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            red = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            green = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            blue = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            alpha = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glClearColorxOES(red, green, blue, alpha);
            ALOGE("glClearColorxOES not implemented.");
        } break;

        case FUNID_glClearDepthxOES: {
            /* readline: "glClearDepthxOES GLfixed depth" */
            /* func name: "glClearDepthxOES" */
            /* args: [{'type': 'GLfixed', 'name': 'depth', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed depth;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearDepthxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            depth = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glClearDepthxOES(depth);
            ALOGE("glClearDepthxOES not implemented.");
        } break;

        case FUNID_glColor4xOES: {
            /* readline: "glColor4xOES GLfixed red, GLfixed green, GLfixed blue,
             * GLfixed alpha" */
            /* func name: "glColor4xOES" */
            /* args: [{'type': 'GLfixed', 'name': 'red', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'green', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfixed', 'name': 'blue', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfixed',
             * 'name': 'alpha', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed red;
            GLfixed green;
            GLfixed blue;
            GLfixed alpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glColor4xOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            red = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            green = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            blue = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            alpha = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glColor4xOES(red, green, blue, alpha);
            ALOGE("glColor4xOES not implemented.");
        } break;

        case FUNID_glDepthRangexOES: {
            /* readline: "glDepthRangexOES GLfixed n, GLfixed f" */
            /* func name: "glDepthRangexOES" */
            /* args: [{'type': 'GLfixed', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'f', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed n;
            GLfixed f;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDepthRangexOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            f = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glDepthRangexOES(n, f);
            ALOGE("glDepthRangexOES not implemented.");
        } break;

        case FUNID_glFogxOES: {
            /* readline: "glFogxOES GLenum pname, GLfixed param" */
            /* func name: "glFogxOES" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed',
             * 'name': 'param', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum pname;
            GLfixed param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFogxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glFogxOES(pname, param);
            ALOGE("glFogxOES not implemented.");
        } break;

        case FUNID_glFrustumxOES: {
            /* readline: "glFrustumxOES GLfixed l, GLfixed r, GLfixed b, GLfixed
             * t, GLfixed n, GLfixed f" */
            /* func name: "glFrustumxOES" */
            /* args: [{'type': 'GLfixed', 'name': 'l', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'r', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLfixed', 'name': 'b', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfixed', 'name': 't',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLfixed', 'name': 'n', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}, {'type': 'GLfixed', 'name': 'f',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed l;
            GLfixed r;
            GLfixed b;
            GLfixed t;
            GLfixed n;
            GLfixed f;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFrustumxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            l = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            r = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            t = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            n = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            f = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glFrustumxOES(l, r, b, t, n, f);
            ALOGE("glFrustumxOES not implemented.");
        } break;

        case FUNID_glLightModelxOES: {
            /* readline: "glLightModelxOES GLenum pname, GLfixed param" */
            /* func name: "glLightModelxOES" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed',
             * 'name': 'param', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum pname;
            GLfixed param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glLightModelxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glLightModelxOES(pname, param);
            ALOGE("glLightModelxOES not implemented.");
        } break;

        case FUNID_glLightxOES: {
            /* readline: "glLightxOES GLenum light, GLenum pname, GLfixed param"
             */
            /* func name: "glLightxOES" */
            /* args: [{'type': 'GLenum', 'name': 'light', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfixed', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum light;
            GLenum pname;
            GLfixed param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glLightxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            light = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glLightxOES(light, pname, param);
            ALOGE("glLightxOES not implemented.");
        } break;

        case FUNID_glLineWidthxOES: {
            /* readline: "glLineWidthxOES GLfixed width" */
            /* func name: "glLineWidthxOES" */
            /* args: [{'type': 'GLfixed', 'name': 'width', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed width;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glLineWidthxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            width = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glLineWidthxOES(width);
            ALOGE("glLineWidthxOES not implemented.");
        } break;

        case FUNID_glMaterialxOES: {
            /* readline: "glMaterialxOES GLenum face, GLenum pname, GLfixed
             * param" */
            /* func name: "glMaterialxOES" */
            /* args: [{'type': 'GLenum', 'name': 'face', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfixed', 'name': 'param', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum face;
            GLenum pname;
            GLfixed param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glMaterialxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            face = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glMaterialxOES(face, pname, param);
            ALOGE("glMaterialxOES not implemented.");
        } break;

        case FUNID_glMultiTexCoord4xOES: {
            /* readline: "glMultiTexCoord4xOES GLenum texture, GLfixed s,
             * GLfixed t, GLfixed r, GLfixed q" */
            /* func name: "glMultiTexCoord4xOES" */
            /* args: [{'type': 'GLenum', 'name': 'texture', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed',
             * 'name': 's', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfixed', 'name': 't', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'r', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLfixed', 'name': 'q', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum texture;
            GLfixed s;
            GLfixed t;
            GLfixed r;
            GLfixed q;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glMultiTexCoord4xOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            texture = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            s = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            t = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            r = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            q = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glMultiTexCoord4xOES(texture, s, t, r, q);
            ALOGE("glMultiTexCoord4xOES not implemented.");
        } break;

        case FUNID_glNormal3xOES: {
            /* readline: "glNormal3xOES GLfixed nx, GLfixed ny, GLfixed nz" */
            /* func name: "glNormal3xOES" */
            /* args: [{'type': 'GLfixed', 'name': 'nx', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'ny', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLfixed', 'name': 'nz', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed nx;
            GLfixed ny;
            GLfixed nz;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glNormal3xOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            nx = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            ny = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            nz = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glNormal3xOES(nx, ny, nz);
            ALOGE("glNormal3xOES not implemented.");
        } break;

        case FUNID_glOrthoxOES: {
            /* readline: "glOrthoxOES GLfixed l, GLfixed r, GLfixed b, GLfixed
             * t, GLfixed n, GLfixed f" */
            /* func name: "glOrthoxOES" */
            /* args: [{'type': 'GLfixed', 'name': 'l', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'r', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLfixed', 'name': 'b', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfixed', 'name': 't',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLfixed', 'name': 'n', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}, {'type': 'GLfixed', 'name': 'f',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed l;
            GLfixed r;
            GLfixed b;
            GLfixed t;
            GLfixed n;
            GLfixed f;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glOrthoxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            l = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            r = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            t = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            n = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            f = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glOrthoxOES(l, r, b, t, n, f);
            ALOGE("glOrthoxOES not implemented.");
        } break;

        case FUNID_glPointSizexOES: {
            /* readline: "glPointSizexOES GLfixed size" */
            /* func name: "glPointSizexOES" */
            /* args: [{'type': 'GLfixed', 'name': 'size', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed size;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPointSizexOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            size = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glPointSizexOES(size);
            ALOGE("glPointSizexOES not implemented.");
        } break;

        case FUNID_glPolygonOffsetxOES: {
            /* readline: "glPolygonOffsetxOES GLfixed factor, GLfixed units" */
            /* func name: "glPolygonOffsetxOES" */
            /* args: [{'type': 'GLfixed', 'name': 'factor', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed',
             * 'name': 'units', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed factor;
            GLfixed units;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPolygonOffsetxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            factor = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            units = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glPolygonOffsetxOES(factor, units);
            ALOGE("glPolygonOffsetxOES not implemented.");
        } break;

        case FUNID_glRotatexOES: {
            /* readline: "glRotatexOES GLfixed angle, GLfixed x, GLfixed y,
             * GLfixed z" */
            /* func name: "glRotatexOES" */
            /* args: [{'type': 'GLfixed', 'name': 'angle', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed',
             * 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLfixed', 'name': 'y', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'z', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed angle;
            GLfixed x;
            GLfixed y;
            GLfixed z;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glRotatexOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            angle = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            z = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glRotatexOES(angle, x, y, z);
            ALOGE("glRotatexOES not implemented.");
        } break;

        case FUNID_glScalexOES: {
            /* readline: "glScalexOES GLfixed x, GLfixed y, GLfixed z" */
            /* func name: "glScalexOES" */
            /* args: [{'type': 'GLfixed', 'name': 'x', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'y', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLfixed', 'name': 'z', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed x;
            GLfixed y;
            GLfixed z;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glScalexOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            x = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            z = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glScalexOES(x, y, z);
            ALOGE("glScalexOES not implemented.");
        } break;

        case FUNID_glTexEnvxOES: {
            /* readline: "glTexEnvxOES GLenum target, GLenum pname, GLfixed
             * param" */
            /* func name: "glTexEnvxOES" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfixed', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;
            GLfixed param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexEnvxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glTexEnvxOES(target, pname, param);
            ALOGE("glTexEnvxOES not implemented.");
        } break;

        case FUNID_glTranslatexOES: {
            /* readline: "glTranslatexOES GLfixed x, GLfixed y, GLfixed z" */
            /* func name: "glTranslatexOES" */
            /* args: [{'type': 'GLfixed', 'name': 'x', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed', 'name':
             * 'y', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLfixed', 'name': 'z', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfixed x;
            GLfixed y;
            GLfixed z;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTranslatexOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            x = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;

            z = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glTranslatexOES(x, y, z);
            ALOGE("glTranslatexOES not implemented.");
        } break;

        case FUNID_glPointParameterxOES: {
            /* readline: "glPointParameterxOES GLenum pname, GLfixed param" */
            /* func name: "glPointParameterxOES" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfixed',
             * 'name': 'param', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum pname;
            GLfixed param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPointParameterxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glPointParameterxOES(pname, param);
            ALOGE("glPointParameterxOES not implemented.");
        } break;

        case FUNID_glSampleCoveragexOES: {
            /* readline: "glSampleCoveragexOES GLclampx value, GLboolean invert"
             */
            /* func name: "glSampleCoveragexOES" */
            /* args: [{'type': 'GLclampx', 'name': 'value', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'invert', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLclampx value;
            GLboolean invert;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glSampleCoveragexOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            value = *(GLclampx *)(temp + temp_loc);
            temp_loc += 4;

            invert = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glSampleCoveragexOES(value, invert);
            ALOGE("glSampleCoveragexOES not implemented.");
        } break;

        case FUNID_glTexGenxOES: {
            /* readline: "glTexGenxOES GLenum coord, GLenum pname, GLfixed
             * param" */
            /* func name: "glTexGenxOES" */
            /* args: [{'type': 'GLenum', 'name': 'coord', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfixed', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum coord;
            GLenum pname;
            GLfixed param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexGenxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            coord = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glTexGenxOES(coord, pname, param);
            ALOGE("glTexGenxOES not implemented.");
        } break;

        case FUNID_glClearDepthfOES: {
            /* readline: "glClearDepthfOES GLclampf depth" */
            /* func name: "glClearDepthfOES" */
            /* args: [{'type': 'GLclampf', 'name': 'depth', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLclampf depth;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearDepthfOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            depth = *(GLclampf *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glClearDepthfOES(depth);
            ALOGE("glClearDepthfOES not implemented.");
        } break;

        case FUNID_glDepthRangefOES: {
            /* readline: "glDepthRangefOES GLclampf n, GLclampf f" */
            /* func name: "glDepthRangefOES" */
            /* args: [{'type': 'GLclampf', 'name': 'n', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLclampf', 'name':
             * 'f', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLclampf n;
            GLclampf f;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDepthRangefOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            n = *(GLclampf *)(temp + temp_loc);
            temp_loc += 4;

            f = *(GLclampf *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glDepthRangefOES(n, f);
            ALOGE("glDepthRangefOES not implemented.");
        } break;

        case FUNID_glFrustumfOES: {
            /* readline: "glFrustumfOES GLfloat l, GLfloat r, GLfloat b, GLfloat
             * t, GLfloat n, GLfloat f" */
            /* func name: "glFrustumfOES" */
            /* args: [{'type': 'GLfloat', 'name': 'l', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'r', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLfloat', 'name': 'b', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 't',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLfloat', 'name': 'n', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'f',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat l;
            GLfloat r;
            GLfloat b;
            GLfloat t;
            GLfloat n;
            GLfloat f;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFrustumfOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            l = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            r = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            t = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            n = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            f = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glFrustumfOES(l, r, b, t, n, f);
            ALOGE("glFrustumfOES not implemented.");
        } break;

        case FUNID_glOrthofOES: {
            /* readline: "glOrthofOES GLfloat l, GLfloat r, GLfloat b, GLfloat
             * t, GLfloat n, GLfloat f" */
            /* func name: "glOrthofOES" */
            /* args: [{'type': 'GLfloat', 'name': 'l', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'r', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLfloat', 'name': 'b', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 't',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLfloat', 'name': 'n', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'f',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLfloat l;
            GLfloat r;
            GLfloat b;
            GLfloat t;
            GLfloat n;
            GLfloat f;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glOrthofOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            l = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            r = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            b = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            t = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            n = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            f = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glOrthofOES(l, r, b, t, n, f);
            ALOGE("glOrthofOES not implemented.");
        } break;

        case FUNID_glRenderbufferStorageMultisampleEXT: {
            /* readline: "glRenderbufferStorageMultisampleEXT GLenum target,
             * GLsizei samples, GLenum internalformat, GLsizei width, GLsizei
             * height" */
            /* func name: "glRenderbufferStorageMultisampleEXT" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'samples', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLsizei samples;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glRenderbufferStorageMultisampleEXT)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            samples = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glRenderbufferStorageMultisampleEXT(target, samples,
            //    internalformat, width, height);
            ALOGE("glRenderbufferStorageMultisampleEXT not implemented.");
        } break;

        case FUNID_glUseProgramStages: {
            /* readline: "glUseProgramStages GLuint pipeline, GLbitfield stages,
             * GLuint program" */
            /* func name: "glUseProgramStages" */
            /* args: [{'type': 'GLuint', 'name': 'pipeline', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLbitfield', 'name': 'stages', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'program', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint pipeline;
            GLbitfield stages;
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUseProgramStages)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pipeline = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            stages = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUseProgramStages((GLuint)GLResource::get_host_pipeline_id(
                                   opengl_context, (unsigned int)pipeline),
                               stages,
                               (GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program));
        } break;

        case FUNID_glActiveShaderProgram: {
            /* readline: "glActiveShaderProgram GLuint pipeline, GLuint program"
             */
            /* func name: "glActiveShaderProgram" */
            /* args: [{'type': 'GLuint', 'name': 'pipeline', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'program', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint pipeline;
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glActiveShaderProgram)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pipeline = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glActiveShaderProgram((GLuint)GLResource::get_host_pipeline_id(
                                      opengl_context, (unsigned int)pipeline),
                                  (GLuint)GLResource::get_host_program_id(
                                      opengl_context, (unsigned int)program));
        } break;

        case FUNID_glProgramUniform1i: {
            /* readline: "glProgramUniform1i GLuint program, GLint location,
             * GLint v0" */
            /* func name: "glProgramUniform1i" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLint v0;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform1i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform1i((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               location, v0);
        } break;

        case FUNID_glProgramUniform2i: {
            /* readline: "glProgramUniform2i GLuint program, GLint location,
             * GLint v0, GLint v1" */
            /* func name: "glProgramUniform2i" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLint v0;
            GLint v1;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform2i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform2i((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               location, v0, v1);
        } break;

        case FUNID_glProgramUniform3i: {
            /* readline: "glProgramUniform3i GLuint program, GLint location,
             * GLint v0, GLint v1, GLint v2" */
            /* func name: "glProgramUniform3i" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'v2', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLint v0;
            GLint v1;
            GLint v2;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform3i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform3i((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               location, v0, v1, v2);
        } break;

        case FUNID_glProgramUniform4i: {
            /* readline: "glProgramUniform4i GLuint program, GLint location,
             * GLint v0, GLint v1, GLint v2, GLint v3" */
            /* func name: "glProgramUniform4i" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'v2', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 4, 'ptr_ptr': False}, {'type': 'GLint', 'name':
             * 'v3', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLint v0;
            GLint v1;
            GLint v2;
            GLint v3;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform4i)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v3 = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform4i((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               location, v0, v1, v2, v3);
        } break;

        case FUNID_glProgramUniform1ui: {
            /* readline: "glProgramUniform1ui GLuint program, GLint location,
             * GLuint v0" */
            /* func name: "glProgramUniform1ui" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLuint v0;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform1ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform1ui((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, v0);
        } break;

        case FUNID_glProgramUniform2ui: {
            /* readline: "glProgramUniform2ui GLuint program, GLint location,
             * GLuint v0, GLuint v1" */
            /* func name: "glProgramUniform2ui" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLuint v0;
            GLuint v1;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform2ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform2ui((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, v0, v1);
        } break;

        case FUNID_glProgramUniform3ui: {
            /* readline: "glProgramUniform3ui GLuint program, GLint location,
             * GLuint v0, GLuint v1, GLuint v2" */
            /* func name: "glProgramUniform3ui" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'v2', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLuint v0;
            GLuint v1;
            GLuint v2;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform3ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform3ui((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, v0, v1, v2);
        } break;

        case FUNID_glProgramUniform4ui: {
            /* readline: "glProgramUniform4ui GLuint program, GLint location,
             * GLuint v0, GLuint v1, GLuint v2, GLuint v3" */
            /* func name: "glProgramUniform4ui" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'v2', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 4, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'v3', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLuint v0;
            GLuint v1;
            GLuint v2;
            GLuint v3;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform4ui)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            v3 = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform4ui((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, v0, v1, v2, v3);
        } break;

        case FUNID_glProgramUniform1f: {
            /* readline: "glProgramUniform1f GLuint program, GLint location,
             * GLfloat v0" */
            /* func name: "glProgramUniform1f" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLfloat v0;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform1f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform1f((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               location, v0);
        } break;

        case FUNID_glProgramUniform2f: {
            /* readline: "glProgramUniform2f GLuint program, GLint location,
             * GLfloat v0, GLfloat v1" */
            /* func name: "glProgramUniform2f" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLfloat v0;
            GLfloat v1;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform2f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform2f((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               location, v0, v1);
        } break;

        case FUNID_glProgramUniform3f: {
            /* readline: "glProgramUniform3f GLuint program, GLint location,
             * GLfloat v0, GLfloat v1, GLfloat v2" */
            /* func name: "glProgramUniform3f" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'v2', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLfloat v0;
            GLfloat v1;
            GLfloat v2;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform3f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform3f((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               location, v0, v1, v2);
        } break;

        case FUNID_glProgramUniform4f: {
            /* readline: "glProgramUniform4f GLuint program, GLint location,
             * GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3" */
            /* func name: "glProgramUniform4f" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'v0', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLfloat',
             * 'name': 'v1', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLfloat', 'name': 'v2', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 4, 'ptr_ptr': False}, {'type': 'GLfloat', 'name':
             * 'v3', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLfloat v0;
            GLfloat v1;
            GLfloat v2;
            GLfloat v3;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform4f)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            v0 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v1 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v2 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;

            v3 = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform4f((GLuint)GLResource::get_host_program_id(
                                   opengl_context, (unsigned int)program),
                               location, v0, v1, v2, v3);
        } break;

        case FUNID_glTransformFeedbackVaryings: {
            /* readline: "glTransformFeedbackVaryings GLuint program, GLsizei
             * count, const GLchar *const*varyings#count|strlen(varyings[i])+1,
             * GLenum bufferMode" */
            /* func name: "glTransformFeedbackVaryings" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLchar*', 'name': 'varyings',
             * 'ptr': 'in', 'ptr_len': 'count|strlen(varyings[i])+1', 'loc': 2,
             * 'ptr_ptr': True}, {'type': 'GLenum', 'name': 'bufferMode', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLsizei count;
            GLenum bufferMode;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTransformFeedbackVaryings)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            bufferMode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            GLchar **varyings = (GLchar **)malloc(count * sizeof(GLchar *));

            for (int i = 0; i < count; i++) {
                varyings[i] = (GLchar *)(temp + temp_loc);
                temp_loc += strlen(varyings[i]) + 1;
            }
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glTransformFeedbackVaryings(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                count, (const GLchar *const *)varyings, bufferMode);

            free(varyings);
        } break;

        case FUNID_glTexParameterfv: {
            /* readline: "glTexParameterfv GLenum target, GLenum pname, const
             * GLfloat *params#gl_pname_size(pname)*sizeof(GLfloat)" */
            /* func name: "glTexParameterfv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name': 'params',
             * 'ptr': 'in', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfloat)',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexParameterfv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *params = (const GLfloat *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfloat);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    glTextureParameterfv(bind_texture, pname, params);
                ALOGE("DSA not supported.");
            } else {
                if (target == GL_TEXTURE_EXTERNAL_OES) {
                    TextureBindingStatus *texture_status =
                        &(opengl_context->texture_binding_status);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(GL_TEXTURE0);
                    }
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->current_texture_external);
                    glTexParameterfv(GL_TEXTURE_2D, pname, params);
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->host_current_texture_2D[0]);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(
                            texture_status->host_current_active_texture +
                            GL_TEXTURE0);
                    }
                } else {
                    glTexParameterfv(target, pname, params);
                }
            }
        } break;

        case FUNID_glTexParameteriv: {
            /* readline: "glTexParameteriv GLenum target, GLenum pname, const
             * GLint *params#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glTexParameteriv" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLint*', 'name': 'params',
             * 'ptr': 'in', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *params = (const GLint *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    glTextureParameteriv(bind_texture, pname, params);
                ALOGE("DSA not supported.");
            } else {
                if (target == GL_TEXTURE_EXTERNAL_OES) {
                    TextureBindingStatus *texture_status =
                        &(opengl_context->texture_binding_status);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(GL_TEXTURE0);
                    }
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->current_texture_external);
                    glTexParameteriv(GL_TEXTURE_2D, pname, params);
                    glBindTexture(GL_TEXTURE_2D,
                                  texture_status->host_current_texture_2D[0]);
                    if (texture_status->host_current_active_texture != 0) {
                        glActiveTexture(
                            texture_status->host_current_active_texture +
                            GL_TEXTURE0);
                    }
                } else {
                    glTexParameteriv(target, pname, params);
                }
            }
        } break;

        case FUNID_glUniform1fv: {
            /* readline: "glUniform1fv GLint location, GLsizei count, const
             * GLfloat *value#count*sizeof(GLfloat)*1" */
            /* func name: "glUniform1fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*1', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform1fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 1;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform1fv(location, count, value);
        } break;

        case FUNID_glUniform1iv: {
            /* readline: "glUniform1iv GLint location, GLsizei count, const
             * GLint *value#count*sizeof(GLint)*1" */
            /* func name: "glUniform1iv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLint)*1', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform1iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint) * 1;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform1iv(location, count, value);
        } break;

        case FUNID_glUniform2fv: {
            /* readline: "glUniform2fv GLint location, GLsizei count, const
             * GLfloat *value#count*sizeof(GLfloat)*2" */
            /* func name: "glUniform2fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*2', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 2;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform2fv(location, count, value);
        } break;

        case FUNID_glUniform2iv: {
            /* readline: "glUniform2iv GLint location, GLsizei count, const
             * GLint *value#count*sizeof(GLint)*2" */
            /* func name: "glUniform2iv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLint)*2', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform2iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint) * 2;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform2iv(location, count, value);
        } break;

        case FUNID_glUniform3fv: {
            /* readline: "glUniform3fv GLint location, GLsizei count, const
             * GLfloat *value#count*sizeof(GLfloat)*3" */
            /* func name: "glUniform3fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*3', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 3;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform3fv(location, count, value);
        } break;

        case FUNID_glUniform3iv: {
            /* readline: "glUniform3iv GLint location, GLsizei count, const
             * GLint *value#count*sizeof(GLint)*3" */
            /* func name: "glUniform3iv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLint)*3', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform3iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint) * 3;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform3iv(location, count, value);
        } break;

        case FUNID_glUniform4fv: {
            /* readline: "glUniform4fv GLint location, GLsizei count, const
             * GLfloat *value#count*sizeof(GLfloat)*4" */
            /* func name: "glUniform4fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*4', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform4fv(location, count, value);
        } break;

        case FUNID_glUniform4iv: {
            /* readline: "glUniform4iv GLint location, GLsizei count, const
             * GLint *value#count*sizeof(GLint)*4" */
            /* func name: "glUniform4iv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLint)*4', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform4iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform4iv(location, count, value);
        } break;

        case FUNID_glVertexAttrib1fv: {
            /* readline: "glVertexAttrib1fv GLuint index, const GLfloat
             * *v#sizeof(GLfloat)*1" */
            /* func name: "glVertexAttrib1fv" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfloat*', 'name': 'v', 'ptr': 'in', 'ptr_len':
             * 'sizeof(GLfloat)*1', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttrib1fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *v = (const GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat) * 1;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttrib1fv(index, v);
        } break;

        case FUNID_glVertexAttrib2fv: {
            /* readline: "glVertexAttrib2fv GLuint index, const GLfloat
             * *v#sizeof(GLfloat)*2" */
            /* func name: "glVertexAttrib2fv" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfloat*', 'name': 'v', 'ptr': 'in', 'ptr_len':
             * 'sizeof(GLfloat)*2', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttrib2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *v = (const GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat) * 2;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttrib2fv(index, v);
        } break;

        case FUNID_glVertexAttrib3fv: {
            /* readline: "glVertexAttrib3fv GLuint index, const GLfloat
             * *v#sizeof(GLfloat)*3" */
            /* func name: "glVertexAttrib3fv" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfloat*', 'name': 'v', 'ptr': 'in', 'ptr_len':
             * 'sizeof(GLfloat)*3', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttrib3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *v = (const GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat) * 3;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttrib3fv(index, v);
        } break;

        case FUNID_glVertexAttrib4fv: {
            /* readline: "glVertexAttrib4fv GLuint index, const GLfloat
             * *v#sizeof(GLfloat)*4" */
            /* func name: "glVertexAttrib4fv" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfloat*', 'name': 'v', 'ptr': 'in', 'ptr_len':
             * 'sizeof(GLfloat)*4', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttrib4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *v = (const GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttrib4fv(index, v);
        } break;

        case FUNID_glUniformMatrix2fv: {
            /* readline: "glUniformMatrix2fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*4" */
            /* func name: "glUniformMatrix2fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*4', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix2fv(location, count, transpose, value);
        } break;

        case FUNID_glUniformMatrix3fv: {
            /* readline: "glUniformMatrix3fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*9" */
            /* func name: "glUniformMatrix3fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*9', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 9;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix3fv(location, count, transpose, value);
        } break;

        case FUNID_glUniformMatrix4fv: {
            /* readline: "glUniformMatrix4fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*16" */
            /* func name: "glUniformMatrix4fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*16', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 16;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix4fv(location, count, transpose, value);
        } break;

        case FUNID_glUniformMatrix2x3fv: {
            /* readline: "glUniformMatrix2x3fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*6" */
            /* func name: "glUniformMatrix2x3fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*6', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix2x3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 6;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix2x3fv(location, count, transpose, value);
        } break;

        case FUNID_glUniformMatrix3x2fv: {
            /* readline: "glUniformMatrix3x2fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*6" */
            /* func name: "glUniformMatrix3x2fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*6', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix3x2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 6;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix3x2fv(location, count, transpose, value);
        } break;

        case FUNID_glUniformMatrix2x4fv: {
            /* readline: "glUniformMatrix2x4fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*8" */
            /* func name: "glUniformMatrix2x4fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*8', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix2x4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix2x4fv(location, count, transpose, value);
        } break;

        case FUNID_glUniformMatrix4x2fv: {
            /* readline: "glUniformMatrix4x2fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*8" */
            /* func name: "glUniformMatrix4x2fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*8', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix4x2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix4x2fv(location, count, transpose, value);
        } break;

        case FUNID_glUniformMatrix3x4fv: {
            /* readline: "glUniformMatrix3x4fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*12" */
            /* func name: "glUniformMatrix3x4fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*12', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix3x4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 12;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix3x4fv(location, count, transpose, value);
        } break;

        case FUNID_glUniformMatrix4x3fv: {
            /* readline: "glUniformMatrix4x3fv GLint location, GLsizei count,
             * GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*12" */
            /* func name: "glUniformMatrix4x3fv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLboolean', 'name': 'transpose',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'const GLfloat*', 'name': 'value', 'ptr': 'in',
             * 'ptr_len': 'count*sizeof(GLfloat)*12', 'loc': 3, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniformMatrix4x3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 12;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniformMatrix4x3fv(location, count, transpose, value);
        } break;

        case FUNID_glVertexAttribI4iv: {
            /* readline: "glVertexAttribI4iv GLuint index, const GLint
             * *v#sizeof(GLint)*4" */
            /* func name: "glVertexAttribI4iv" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLint*', 'name': 'v', 'ptr': 'in', 'ptr_len': 'sizeof(GLint)*4',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttribI4iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *v = (const GLint *)(temp + temp_loc);
            temp_loc += sizeof(GLint) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttribI4iv(index, v);
        } break;

        case FUNID_glVertexAttribI4uiv: {
            /* readline: "glVertexAttribI4uiv GLuint index, const GLuint
             * *v#sizeof(GLuint)*4" */
            /* func name: "glVertexAttribI4uiv" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLuint*', 'name': 'v', 'ptr': 'in', 'ptr_len':
             * 'sizeof(GLuint)*4', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttribI4uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *v = (const GLuint *)(temp + temp_loc);
            temp_loc += sizeof(GLuint) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glVertexAttribI4uiv(index, v);
        } break;

        case FUNID_glUniform1uiv: {
            /* readline: "glUniform1uiv GLint location, GLsizei count, const
             * GLuint *value#count*sizeof(GLuint)*1" */
            /* func name: "glUniform1uiv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLuint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLuint)*1', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform1uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint) * 1;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform1uiv(location, count, value);
        } break;

        case FUNID_glUniform2uiv: {
            /* readline: "glUniform2uiv GLint location, GLsizei count, const
             * GLuint *value#count*sizeof(GLuint)*2" */
            /* func name: "glUniform2uiv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLuint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLuint)*2', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform2uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint) * 2;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform2uiv(location, count, value);
        } break;

        case FUNID_glUniform3uiv: {
            /* readline: "glUniform3uiv GLint location, GLsizei count, const
             * GLuint *value#count*sizeof(GLuint)*3" */
            /* func name: "glUniform3uiv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLuint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLuint)*3', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform3uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint) * 3;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform3uiv(location, count, value);
        } break;

        case FUNID_glUniform4uiv: {
            /* readline: "glUniform4uiv GLint location, GLsizei count, const
             * GLuint *value#count*sizeof(GLuint)*4" */
            /* func name: "glUniform4uiv" */
            /* args: [{'type': 'GLint', 'name': 'location', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLuint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len': 'count*sizeof(GLuint)*4', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glUniform4uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glUniform4uiv(location, count, value);
        } break;

        case FUNID_glClearBufferiv: {
            /* readline: "glClearBufferiv GLenum buffer, GLint drawbuffer, const
             * GLint *value#(buffer==GL_COLOR?4*sizeof(GLint):1*sizeof(GLint))"
             */
            /* func name: "glClearBufferiv" */
            /* args: [{'type': 'GLenum', 'name': 'buffer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'drawbuffer', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len':
             * '(buffer==GL_COLOR?4*sizeof(GLint):1*sizeof(GLint))', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum buffer;
            GLint drawbuffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearBufferiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            buffer = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            drawbuffer = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc +=
                (buffer == GL_COLOR ? 4 * sizeof(GLint) : 1 * sizeof(GLint));
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glClearBufferiv(buffer, drawbuffer, value);
        } break;

        case FUNID_glClearBufferuiv: {
            /* readline: "glClearBufferuiv GLenum buffer, GLint drawbuffer,
             * const GLuint
             * *value#(buffer==GL_COLOR?4*sizeof(GLuint):1*sizeof(GLuint))" */
            /* func name: "glClearBufferuiv" */
            /* args: [{'type': 'GLenum', 'name': 'buffer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'drawbuffer', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLuint*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len':
             * '(buffer==GL_COLOR?4*sizeof(GLuint):1*sizeof(GLuint))', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum buffer;
            GLint drawbuffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearBufferuiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            buffer = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            drawbuffer = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc +=
                (buffer == GL_COLOR ? 4 * sizeof(GLuint) : 1 * sizeof(GLuint));
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glClearBufferuiv(buffer, drawbuffer, value);
        } break;

        case FUNID_glClearBufferfv: {
            /* readline: "glClearBufferfv GLenum buffer, GLint drawbuffer, const
             * GLfloat
             * *value#(buffer==GL_COLOR?4*sizeof(GLfloat):1*sizeof(GLfloat))" */
            /* func name: "glClearBufferfv" */
            /* args: [{'type': 'GLenum', 'name': 'buffer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'drawbuffer', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name': 'value',
             * 'ptr': 'in', 'ptr_len':
             * '(buffer==GL_COLOR?4*sizeof(GLfloat):1*sizeof(GLfloat))', 'loc':
             * 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum buffer;
            GLint drawbuffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClearBufferfv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            buffer = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            drawbuffer = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += (buffer == GL_COLOR ? 4 * sizeof(GLfloat)
                                            : 1 * sizeof(GLfloat));
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glClearBufferfv(buffer, drawbuffer, value);
        } break;

        case FUNID_glSamplerParameteriv: {
            /* readline: "glSamplerParameteriv GLuint sampler, GLenum pname,
             * const GLint *param#gl_pname_size(pname)*sizeof(GLint)" */
            /* func name: "glSamplerParameteriv" */
            /* args: [{'type': 'GLuint', 'name': 'sampler', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLint*', 'name': 'param',
             * 'ptr': 'in', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLint)',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint sampler;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glSamplerParameteriv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sampler = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *param = (const GLint *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glSamplerParameteriv((GLuint)GLResource::get_host_sampler_id(
                                     opengl_context, (unsigned int)sampler),
                                 pname, param);
        } break;

        case FUNID_glSamplerParameterfv: {
            /* readline: "glSamplerParameterfv GLuint sampler, GLenum pname,
             * const GLfloat *param#gl_pname_size(pname)*sizeof(GLfloat)" */
            /* func name: "glSamplerParameterfv" */
            /* args: [{'type': 'GLuint', 'name': 'sampler', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name': 'param',
             * 'ptr': 'in', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfloat)',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint sampler;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glSamplerParameterfv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            sampler = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *param = (const GLfloat *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfloat);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glSamplerParameterfv((GLuint)GLResource::get_host_sampler_id(
                                     opengl_context, (unsigned int)sampler),
                                 pname, param);
        } break;

        case FUNID_glInvalidateFramebuffer: {
            /* readline: "glInvalidateFramebuffer GLenum target, GLsizei
             * numAttachments, const GLenum
             * *attachments#numAttachments*sizeof(GLenum)" */
            /* func name: "glInvalidateFramebuffer" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'numAttachments', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLenum*', 'name':
             * 'attachments', 'ptr': 'in', 'ptr_len':
             * 'numAttachments*sizeof(GLenum)', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLsizei numAttachments;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glInvalidateFramebuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            numAttachments = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            GLenum *attachments = (GLenum *)(temp + temp_loc);
            temp_loc += numAttachments * sizeof(GLenum);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            for (int i = 0; i < numAttachments; i++) {
                if (attachments[i] == GL_COLOR) {
                    attachments[i] = GL_COLOR_ATTACHMENT0;
                }
                if (attachments[i] == GL_DEPTH) {
                    attachments[i] = GL_DEPTH_ATTACHMENT;
                }
                if (attachments[i] == GL_STENCIL) {
                    attachments[i] = GL_DEPTH_STENCIL_ATTACHMENT;
                }
            }

            glInvalidateFramebuffer(target, numAttachments, attachments);
        } break;

        case FUNID_glInvalidateSubFramebuffer: {
            /* readline: "glInvalidateSubFramebuffer GLenum target, GLsizei
             * numAttachments, const GLenum
             * *attachments#numAttachments*sizeof(GLenum), GLint x, GLint y,
             * GLsizei width, GLsizei height" */
            /* func name: "glInvalidateSubFramebuffer" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'numAttachments', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLenum*', 'name':
             * 'attachments', 'ptr': 'in', 'ptr_len':
             * 'numAttachments*sizeof(GLenum)', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLint', 'name': 'x', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'y',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLsizei numAttachments;
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glInvalidateSubFramebuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            numAttachments = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLenum *attachments = (const GLenum *)(temp + temp_loc);
            temp_loc += numAttachments * sizeof(GLenum);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glInvalidateSubFramebuffer(target, numAttachments, attachments, x,
                                       y, width, height);
        } break;

        case FUNID_glClipPlanexOES: {
            /* readline: "glClipPlanexOES GLenum plane, const GLfixed
             * *equation#sizeof(GLfixed)*4" */
            /* func name: "glClipPlanexOES" */
            /* args: [{'type': 'GLenum', 'name': 'plane', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfixed*', 'name': 'equation', 'ptr': 'in', 'ptr_len':
             * 'sizeof(GLfixed)*4', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum plane;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClipPlanexOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            plane = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfixed *equation = (const GLfixed *)(temp + temp_loc);
            temp_loc += sizeof(GLfixed) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glClipPlanexOES(plane, equation);
            ALOGE("glClipPlanexOES not implemented.");
        } break;

        case FUNID_glFogxvOES: {
            /* readline: "glFogxvOES GLenum pname, const GLfixed
             * *param#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glFogxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfixed*', 'name': 'param', 'ptr': 'in', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFogxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfixed *param = (const GLfixed *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glFogxvOES(pname, param);
            ALOGE("glFogxvOES not implemented.");
        } break;

        case FUNID_glLightModelxvOES: {
            /* readline: "glLightModelxvOES GLenum pname, const GLfixed
             * *param#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glLightModelxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfixed*', 'name': 'param', 'ptr': 'in', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glLightModelxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfixed *param = (const GLfixed *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glLightModelxvOES(pname, param);
            ALOGE("glLightModelxvOES not implemented.");
        } break;

        case FUNID_glLightxvOES: {
            /* readline: "glLightxvOES GLenum light, GLenum pname, const GLfixed
             * *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glLightxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'light', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfixed*', 'name': 'params',
             * 'ptr': 'in', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum light;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glLightxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            light = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfixed *params = (const GLfixed *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glLightxvOES(light, pname, params);
            ALOGE("glLightxvOES not implemented");
        } break;

        case FUNID_glLoadMatrixxOES: {
            /* readline: "glLoadMatrixxOES const GLfixed *m#16*sizeof(GLfixed)"
             */
            /* func name: "glLoadMatrixxOES" */
            /* args: [{'type': 'const GLfixed*', 'name': 'm', 'ptr': 'in',
             * 'ptr_len': '16*sizeof(GLfixed)', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glLoadMatrixxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            // if (unlikely(temp_len < 0 * 1))
            // {
            //     break;
            // }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            const GLfixed *m = (const GLfixed *)(temp + temp_loc);
            temp_loc += 16 * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glLoadMatrixxOES(m);
            ALOGE("glLoadMatrixxOES not implemented");
        } break;

        case FUNID_glMaterialxvOES: {
            /* readline: "glMaterialxvOES GLenum face, GLenum pname, const
             * GLfixed *param#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glMaterialxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'face', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'const GLfixed*', 'name': 'param', 'ptr': 'in',
             * 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum face;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glMaterialxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            face = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfixed *param = (const GLfixed *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glMaterialxvOES(face, pname, param);
            ALOGE("glMaterialxvOES not implemented");
        } break;

        case FUNID_glMultMatrixxOES: {
            /* readline: "glMultMatrixxOES const GLfixed *m#16*sizeof(GLfixed)"
             */
            /* func name: "glMultMatrixxOES" */
            /* args: [{'type': 'const GLfixed*', 'name': 'm', 'ptr': 'in',
             * 'ptr_len': '16*sizeof(GLfixed)', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glMultMatrixxOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            // if (unlikely(temp_len < 0 * 1))
            // {
            //     break;
            // }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            const GLfixed *m = (const GLfixed *)(temp + temp_loc);
            temp_loc += 16 * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glMultMatrixxOES(m);
            ALOGE("glMultMatrixxOES not implemented");
        } break;

        case FUNID_glPointParameterxvOES: {
            /* readline: "glPointParameterxvOES GLenum pname, const GLfixed
             * *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glPointParameterxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'pname', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfixed*', 'name': 'params', 'ptr': 'in', 'ptr_len':
             * 'gl_pname_size(pname)*sizeof(GLfixed)', 'loc': 1, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPointParameterxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfixed *params = (const GLfixed *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glPointParameterxvOES(pname, params);
            ALOGE("glPointParameterxvOES not implemented");
        } break;

        case FUNID_glTexEnvxvOES: {
            /* readline: "glTexEnvxvOES GLenum target, GLenum pname, const
             * GLfixed *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glTexEnvxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfixed*', 'name': 'params',
             * 'ptr': 'in', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexEnvxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfixed *params = (const GLfixed *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glTexEnvxvOES(target, pname, params);
            ALOGE("glTexEnvxvOES not implemented");
        } break;

        case FUNID_glClipPlanefOES: {
            /* readline: "glClipPlanefOES GLenum plane, const GLfloat
             * *equation#sizeof(GLfloat)*4" */
            /* func name: "glClipPlanefOES" */
            /* args: [{'type': 'GLenum', 'name': 'plane', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLfloat*', 'name': 'equation', 'ptr': 'in', 'ptr_len':
             * 'sizeof(GLfloat)*4', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum plane;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glClipPlanefOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            plane = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *equation = (const GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glClipPlanefOES(plane, equation);
            ALOGE("glClipPlanefOES not implemented");
        } break;

        case FUNID_glTexGenxvOES: {
            /* readline: "glTexGenxvOES GLenum coord, GLenum pname, const
             * GLfixed *params#gl_pname_size(pname)*sizeof(GLfixed)" */
            /* func name: "glTexGenxvOES" */
            /* args: [{'type': 'GLenum', 'name': 'coord', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLfixed*', 'name': 'params',
             * 'ptr': 'in', 'ptr_len': 'gl_pname_size(pname)*sizeof(GLfixed)',
             * 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum coord;
            GLenum pname;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexGenxvOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            coord = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            const GLfixed *params = (const GLfixed *)(temp + temp_loc);
            temp_loc += GLHelper::gl_pname_size(pname) * sizeof(GLfixed);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            //    glTexGenxvOES(coord, pname, params);
            ALOGE("glTexGenxvOES not implemented");
        } break;

        case FUNID_glProgramUniform1iv: {
            /* readline: "glProgramUniform1iv GLuint program, GLint location,
             * GLsizei count, const GLint *value#count*sizeof(GLint)" */
            /* func name: "glProgramUniform1iv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLint*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLint)', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform1iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform1iv((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, count, value);
        } break;

        case FUNID_glProgramUniform2iv: {
            /* readline: "glProgramUniform2iv GLuint program, GLint location,
             * GLsizei count, const GLint *value#count*sizeof(GLint)*2" */
            /* func name: "glProgramUniform2iv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLint*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLint)*2', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform2iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint) * 2;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform2iv((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, count, value);
        } break;

        case FUNID_glProgramUniform3iv: {
            /* readline: "glProgramUniform3iv GLuint program, GLint location,
             * GLsizei count, const GLint *value#count*sizeof(GLint)*3" */
            /* func name: "glProgramUniform3iv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLint*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLint)*3', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform3iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint) * 3;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform3iv((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, count, value);
        } break;

        case FUNID_glProgramUniform4iv: {
            /* readline: "glProgramUniform4iv GLuint program, GLint location,
             * GLsizei count, const GLint *value#count*sizeof(GLint)*4" */
            /* func name: "glProgramUniform4iv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLint*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLint)*4', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform4iv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLint *value = (const GLint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLint) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform4iv((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, count, value);
        } break;

        case FUNID_glProgramUniform1uiv: {
            /* readline: "glProgramUniform1uiv GLuint program, GLint location,
             * GLsizei count, const GLuint *value#count*sizeof(GLuint)" */
            /* func name: "glProgramUniform1uiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLuint*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLuint)', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform1uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform1uiv((GLuint)GLResource::get_host_program_id(
                                     opengl_context, (unsigned int)program),
                                 location, count, value);
        } break;

        case FUNID_glProgramUniform2uiv: {
            /* readline: "glProgramUniform2uiv GLuint program, GLint location,
             * GLsizei count, const GLuint *value#count*sizeof(GLuint)*2" */
            /* func name: "glProgramUniform2uiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLuint*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLuint)*2', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform2uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint) * 2;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform2uiv((GLuint)GLResource::get_host_program_id(
                                     opengl_context, (unsigned int)program),
                                 location, count, value);
        } break;

        case FUNID_glProgramUniform3uiv: {
            /* readline: "glProgramUniform3uiv GLuint program, GLint location,
             * GLsizei count, const GLuint *value#count*sizeof(GLuint)*3" */
            /* func name: "glProgramUniform3uiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLuint*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLuint)*3', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform3uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint) * 3;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform3uiv((GLuint)GLResource::get_host_program_id(
                                     opengl_context, (unsigned int)program),
                                 location, count, value);
        } break;

        case FUNID_glProgramUniform4uiv: {
            /* readline: "glProgramUniform4uiv GLuint program, GLint location,
             * GLsizei count, const GLuint *value#count*sizeof(GLuint)*4" */
            /* func name: "glProgramUniform4uiv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLuint*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLuint)*4', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform4uiv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLuint *value = (const GLuint *)(temp + temp_loc);
            temp_loc += count * sizeof(GLuint) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform4uiv((GLuint)GLResource::get_host_program_id(
                                     opengl_context, (unsigned int)program),
                                 location, count, value);
        } break;

        case FUNID_glProgramUniform1fv: {
            /* readline: "glProgramUniform1fv GLuint program, GLint location,
             * GLsizei count, const GLfloat *value#count*sizeof(GLfloat)" */
            /* func name: "glProgramUniform1fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLfloat*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLfloat)', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform1fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform1fv((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, count, value);
        } break;

        case FUNID_glProgramUniform2fv: {
            /* readline: "glProgramUniform2fv GLuint program, GLint location,
             * GLsizei count, const GLfloat *value#count*sizeof(GLfloat)*2" */
            /* func name: "glProgramUniform2fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLfloat*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLfloat)*2', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 2;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform2fv((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, count, value);
        } break;

        case FUNID_glProgramUniform3fv: {
            /* readline: "glProgramUniform3fv GLuint program, GLint location,
             * GLsizei count, const GLfloat *value#count*sizeof(GLfloat)*3" */
            /* func name: "glProgramUniform3fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLfloat*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLfloat)*3', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 3;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform3fv((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, count, value);
        } break;

        case FUNID_glProgramUniform4fv: {
            /* readline: "glProgramUniform4fv GLuint program, GLint location,
             * GLsizei count, const GLfloat *value#count*sizeof(GLfloat)*4" */
            /* func name: "glProgramUniform4fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'const GLfloat*', 'name': 'value', 'ptr': 'in', 'ptr_len':
             * 'count*sizeof(GLfloat)*4', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniform4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniform4fv((GLuint)GLResource::get_host_program_id(
                                    opengl_context, (unsigned int)program),
                                location, count, value);
        } break;

        case FUNID_glProgramUniformMatrix2fv: {
            /* readline: "glProgramUniformMatrix2fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*4" */
            /* func name: "glProgramUniformMatrix2fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*4',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix2fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glProgramUniformMatrix3fv: {
            /* readline: "glProgramUniformMatrix3fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*9" */
            /* func name: "glProgramUniformMatrix3fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*9',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 9;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix3fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glProgramUniformMatrix4fv: {
            /* readline: "glProgramUniformMatrix4fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*16" */
            /* func name: "glProgramUniformMatrix4fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*16',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 16;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix4fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glProgramUniformMatrix2x3fv: {
            /* readline: "glProgramUniformMatrix2x3fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*6" */
            /* func name: "glProgramUniformMatrix2x3fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*6',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix2x3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 6;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix2x3fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glProgramUniformMatrix3x2fv: {
            /* readline: "glProgramUniformMatrix3x2fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*6" */
            /* func name: "glProgramUniformMatrix3x2fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*6',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix3x2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 6;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix3x2fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glProgramUniformMatrix2x4fv: {
            /* readline: "glProgramUniformMatrix2x4fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*8" */
            /* func name: "glProgramUniformMatrix2x4fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*8',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix2x4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix2x4fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glProgramUniformMatrix4x2fv: {
            /* readline: "glProgramUniformMatrix4x2fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*8" */
            /* func name: "glProgramUniformMatrix4x2fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*8',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix4x2fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix4x2fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glProgramUniformMatrix3x4fv: {
            /* readline: "glProgramUniformMatrix3x4fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*12" */
            /* func name: "glProgramUniformMatrix3x4fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*12',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix3x4fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 12;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix3x4fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glProgramUniformMatrix4x3fv: {
            /* readline: "glProgramUniformMatrix4x3fv GLuint program, GLint
             * location, GLsizei count, GLboolean transpose, const GLfloat
             * *value#count*sizeof(GLfloat)*12" */
            /* func name: "glProgramUniformMatrix4x3fv" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'location', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'count', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'transpose', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'const GLfloat*', 'name':
             * 'value', 'ptr': 'in', 'ptr_len': 'count*sizeof(GLfloat)*12',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLint location;
            GLsizei count;
            GLboolean transpose;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glProgramUniformMatrix4x3fv)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            location = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            transpose = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            const GLfloat *value = (const GLfloat *)(temp + temp_loc);
            temp_loc += count * sizeof(GLfloat) * 12;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glProgramUniformMatrix4x3fv(
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                location, count, transpose, value);
        } break;

        case FUNID_glBindAttribLocation: {
            /* readline: "glBindAttribLocation GLuint program, GLuint index,
             * const GLchar *name#strlen(name)+1" */
            /* func name: "glBindAttribLocation" */
            /* args: [{'type': 'GLuint', 'name': 'program', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'index', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const GLchar*', 'name': 'name',
             * 'ptr': 'in', 'ptr_len': 'strlen(name)+1', 'loc': 2, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLuint program;
            GLuint index;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindAttribLocation)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            const GLchar *name = (const GLchar *)(temp + temp_loc);
            temp_loc += strlen(name) + 1;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBindAttribLocation((GLuint)GLResource::get_host_program_id(
                                     opengl_context, (unsigned int)program),
                                 index, name);
        } break;

        case FUNID_glTexEnvf: {
            /* readline: "glTexEnvf GLenum target, GLenum pname, GLfloat param"
             */
            /* func name: "glTexEnvf" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfloat', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;
            GLfloat param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexEnvf)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfloat *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLv1::d_glTexEnvf_special(opengl_context, target, pname, param);
        } break;

        case FUNID_glTexEnvi: {
            /* readline: "glTexEnvi GLenum target, GLenum pname, GLint param" */
            /* func name: "glTexEnvi" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;
            GLint param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexEnvi)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLv1::d_glTexEnvi_special(opengl_context, target, pname, param);
        } break;

        case FUNID_glTexEnvx: {
            /* readline: "glTexEnvx GLenum target, GLenum pname, GLfixed param"
             */
            /* func name: "glTexEnvx" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLfixed', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;
            GLfixed param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexEnvx)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLfixed *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLv1::d_glTexEnvx_special(opengl_context, target, pname, param);
        } break;

        case FUNID_glTexParameterx: {
            /* readline: "glTexParameterx GLenum target, GLenum pname, GLint
             * param" */
            /* func name: "glTexParameterx" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'pname', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'param', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum target;
            GLenum pname;
            GLint param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexParameterx)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLv1::d_glTexParameterx_special(opengl_context, target, pname,
                                            param);
        } break;

        case FUNID_glShadeModel: {
            /* readline: "glShadeModel GLenum mode" */
            /* func name: "glShadeModel" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLenum mode;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glShadeModel)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLv1::d_glShadeModel_special(opengl_context, mode);
        } break;

        case FUNID_glDrawTexiOES: {
            /* readline: "glDrawTexiOES GLint x, GLint y, GLint z, GLint width,
             * GLint height, GLfloat left_x, GLfloat right_x, GLfloat bottom_y,
             * GLfloat top_y" */
            /* func name: "glDrawTexiOES" */
            /* args: [{'type': 'GLint', 'name': 'x', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'y',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False},
             * {'type': 'GLint', 'name': 'z', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 2, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'width',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'GLint', 'name': 'height', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "212" */

            /* Define variables */
            GLint x;
            GLint y;
            GLint z;
            GLint width;
            GLint height;
            GLfloat left_x;
            GLfloat right_x;
            GLfloat bottom_y;
            GLfloat top_y;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDrawTexiOES)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1 + 4 * sizeof(GLfloat))) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            x = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            y = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            z = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            left_x = *(GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat);

            right_x = *(GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat);

            bottom_y = *(GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat);

            top_y = *(GLfloat *)(temp + temp_loc);
            temp_loc += sizeof(GLfloat);

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            GLv1::d_glDrawTexiOES_special(opengl_context, x, y, z, width,
                                          height, left_x, right_x, bottom_y,
                                          top_y);
        } break;

            /******* end of file '2-1-2', 210/364 functions*******/

            /******* file '2-2' *******/

        case FUNID_glVertexAttribIPointer_without_bound: {
            /* readline: "glVertexAttribIPointer_without_bound GLuint index,
             * GLint size, GLenum type, GLsizei stride, GLuint offset, GLsizei
             * length, const void *pointer#length" */
            /* func name: "glVertexAttribIPointer_without_bound" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'type', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'stride', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLuint', 'name': 'offset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'length', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'const void*', 'name': 'pointer',
             * 'ptr': 'in', 'ptr_len': 'length', 'loc': 6, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLuint index;
            GLint size;
            GLenum type;
            GLsizei stride;
            GLuint offset;
            GLsizei length;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glVertexAttribIPointer_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 24 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            stride = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            length = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            void *pointer = (void *)call->paras[0].para_ptr;

            Vertex::d_glVertexAttribIPointer_without_bound(
                opengl_context, index, size, type, stride, offset, length,
                pointer);
        } break;

        case FUNID_glVertexAttribPointer_without_bound: {
            /* readline: "glVertexAttribPointer_without_bound GLuint index,
             * GLint size, GLenum type, GLboolean normalized, GLsizei stride,
             * GLuint offset, GLuint length, const void *pointer#length" */
            /* func name: "glVertexAttribPointer_without_bound" */
            /* args: [{'type': 'GLuint', 'name': 'index', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'type', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLboolean', 'name': 'normalized', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'stride', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'offset', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLuint',
             * 'name': 'length', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'const void*', 'name': 'pointer',
             * 'ptr': 'in', 'ptr_len': 'length', 'loc': 7, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLuint index;
            GLint size;
            GLenum type;
            GLboolean normalized;
            GLsizei stride;
            GLuint offset;
            GLuint length;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glVertexAttribPointer_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 28 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            index = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            normalized = *(GLboolean *)(temp + temp_loc);
            temp_loc += 4;

            stride = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            length = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            void *pointer = (void *)call->paras[0].para_ptr;

            Vertex::d_glVertexAttribPointer_without_bound(
                opengl_context, index, size, type, normalized, stride, offset,
                length, pointer);
        } break;

        case FUNID_glDrawElements_without_bound: {
            /* readline: "glDrawElements_without_bound GLenum mode, GLsizei
             * count, GLenum type, const void *indices#count*gl_sizeof(type)" */
            /* func name: "glDrawElements_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'type', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'const
             * void*', 'name': 'indices', 'ptr': 'in', 'ptr_len':
             * 'count*gl_sizeof(type)', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum mode;
            GLsizei count;
            GLenum type;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawElements_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            void *indices = (void *)call->paras[0].para_ptr;

            Vertex::d_glDrawElements_without_bound(opengl_context, mode, count,
                                                   type, indices);
        } break;

        case FUNID_glDrawElementsInstanced_without_bound: {
            /* readline: "glDrawElementsInstanced_without_bound GLenum mode,
             * GLsizei count, GLenum type, const void
             * *indices#count*gl_sizeof(type), GLsizei instancecount" */
            /* func name: "glDrawElementsInstanced_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'type', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'const
             * void*', 'name': 'indices', 'ptr': 'in', 'ptr_len':
             * 'count*gl_sizeof(type)', 'loc': 3, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'instancecount', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum mode;
            GLsizei count;
            GLenum type;
            GLsizei instancecount;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawElementsInstanced_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            instancecount = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            void *indices = (void *)call->paras[0].para_ptr;

            Vertex::d_glDrawElementsInstanced_without_bound(
                opengl_context, mode, count, type, indices, instancecount);
        } break;

        case FUNID_glDrawRangeElements_without_bound: {
            /* readline: "glDrawRangeElements_without_bound GLenum mode, GLuint
             * start, GLuint end, GLsizei count, GLenum type, const void
             * *indices#count*gl_sizeof(type)" */
            /* func name: "glDrawRangeElements_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'mode', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLuint', 'name':
             * 'start', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr':
             * False}, {'type': 'GLuint', 'name': 'end', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'count', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'type', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type': 'const
             * void*', 'name': 'indices', 'ptr': 'in', 'ptr_len':
             * 'count*gl_sizeof(type)', 'loc': 5, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum mode;
            GLuint start;
            GLuint end;
            GLsizei count;
            GLenum type;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawRangeElements_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            start = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            end = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            count = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            void *indices = (void *)call->paras[0].para_ptr;

            Vertex::d_glDrawRangeElements_without_bound(
                opengl_context, mode, start, end, count, type, indices);
        } break;

        case FUNID_glFlushMappedBufferRange_special: {
            /* readline: "glFlushMappedBufferRange_special GLenum target,
             * GLintptr offset, GLsizeiptr length, const void *data#length" */
            /* func name: "glFlushMappedBufferRange_special" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'offset', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'GLsizeiptr', 'name':
             * 'length', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'const void*', 'name': 'data', 'ptr': 'in',
             * 'ptr_len': 'length', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLintptr offset;
            GLsizeiptr length;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glFlushMappedBufferRange_special)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            length = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;

            void *data = (void *)call->paras[0].para_ptr;

            Memory::d_glFlushMappedBufferRange_special(opengl_context, target,
                                                       offset, length, data);
        } break;

        case FUNID_glBufferData_custom: {
            /* readline: "glBufferData_custom GLenum target, GLsizeiptr size,
             * const void *data#size, GLenum usage" */
            /* func name: "glBufferData_custom" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLsizeiptr', 'name': 'size', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'const void*', 'name':
             * 'data', 'ptr': 'in', 'ptr_len': 'size', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'usage', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLsizeiptr size;
            GLenum usage;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBufferData_custom)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;

            usage = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            void *data = (void *)call->paras[0].para_ptr;

            Memory::d_glBufferData_custom(opengl_context, target, size, data,
                                          usage);
        } break;

        case FUNID_glBufferSubData_custom: {
            /* readline: "glBufferSubData_custom GLenum target, GLintptr offset,
             * GLsizeiptr size, const void *data#size" */
            /* func name: "glBufferSubData_custom" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'GLintptr', 'name': 'offset', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'GLsizeiptr', 'name':
             * 'size', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'const void*', 'name': 'data', 'ptr': 'in',
             * 'ptr_len': 'size', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLintptr offset;
            GLsizeiptr size;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBufferSubData_custom)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            size = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;

            void *data = (void *)call->paras[0].para_ptr;

            Memory::d_glBufferSubData_custom(opengl_context, target, offset,
                                             size, data);
        } break;

        case FUNID_glCompressedTexImage2D_without_bound: {
            /* readline: "glCompressedTexImage2D_without_bound GLenum target,
             * GLint level, GLenum internalformat, GLsizei width, GLsizei
             * height, GLint border, GLsizei imageSize, const void
             * *data#imageSize  @{if(imageSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glCompressedTexImage2D_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'border', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLsizei',
             * 'name': 'imageSize', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'const void*', 'name': 'data', 'ptr':
             * 'in', 'ptr_len': 'imageSize', 'loc': 7, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;
            GLint border;
            GLsizei imageSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glCompressedTexImage2D_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 28 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            void *data = (void *)call->paras[0].para_ptr;

            Texture::d_glCompressedTexImage2D_without_bound(
                opengl_context, target, level, internalformat, width, height,
                border, imageSize, data);
        } break;

        case FUNID_glCompressedTexSubImage2D_without_bound: {
            /* readline: "glCompressedTexSubImage2D_without_bound GLenum target,
             * GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei
             * height, GLenum format, GLsizei imageSize, const void
             * *data#imageSize @{if(imageSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glCompressedTexSubImage2D_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'width', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'format', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'imageSize', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 7, 'ptr_ptr': False}, {'type': 'const void*', 'name':
             * 'data', 'ptr': 'in', 'ptr_len': 'imageSize', 'loc': 8, 'ptr_ptr':
             * False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLsizei width;
            GLsizei height;
            GLenum format;
            GLsizei imageSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(
                    para_num <
                    PARA_NUM_MIN_glCompressedTexSubImage2D_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 32 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            void *data = (void *)call->paras[0].para_ptr;

            Texture::d_glCompressedTexSubImage2D_without_bound(
                opengl_context, target, level, xoffset, yoffset, width, height,
                format, imageSize, data);
        } break;

        case FUNID_glCompressedTexImage3D_without_bound: {
            /* readline: "glCompressedTexImage3D_without_bound GLenum target,
             * GLint level, GLenum internalformat, GLsizei width, GLsizei
             * height, GLsizei depth, GLint border, GLsizei imageSize, const
             * void *data#imageSize @{if(imageSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glCompressedTexImage3D_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'border', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'imageSize',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 7, 'ptr_ptr': False},
             * {'type': 'const void*', 'name': 'data', 'ptr': 'in', 'ptr_len':
             * 'imageSize', 'loc': 8, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLint border;
            GLsizei imageSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glCompressedTexImage3D_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 32 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            void *data = (void *)call->paras[0].para_ptr;

            Texture::d_glCompressedTexImage3D_without_bound(
                opengl_context, target, level, internalformat, width, height,
                depth, border, imageSize, data);
        } break;

        case FUNID_glCompressedTexSubImage3D_without_bound: {
            /* readline: "glCompressedTexSubImage3D_without_bound GLenum target,
             * GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei
             * width, GLsizei height, GLsizei depth, GLenum format, GLsizei
             * imageSize, const void *data#imageSize @{if(imageSize<0){
             * set_gl_error(context,GL_INVALID_VALUE); return; }}" */
            /* func name: "glCompressedTexSubImage3D_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'zoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'height',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 7, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'format', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'imageSize', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 9, 'ptr_ptr': False}, {'type': 'const
             * void*', 'name': 'data', 'ptr': 'in', 'ptr_len': 'imageSize',
             * 'loc': 10, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLint zoffset;
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLenum format;
            GLsizei imageSize;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(
                    para_num <
                    PARA_NUM_MIN_glCompressedTexSubImage3D_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 40 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            zoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            imageSize = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            void *data = (void *)call->paras[0].para_ptr;

            Texture::d_glCompressedTexSubImage3D_without_bound(
                opengl_context, target, level, xoffset, yoffset, zoffset, width,
                height, depth, format, imageSize, data);
        } break;

        case FUNID_glTexImage2D_without_bound: {
            /* readline: "glTexImage2D_without_bound GLenum target, GLint level,
             * GLint internalformat, GLsizei width, GLsizei height, GLint
             * border, GLenum format, GLenum type, GLint buf_len, const void
             * *pixels#buf_len" */
            /* func name: "glTexImage2D_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLint', 'name': 'border', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLenum',
             * 'name': 'format', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'type', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 7, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'buf_len', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 8, 'ptr_ptr': False}, {'type': 'const void*', 'name': 'pixels',
             * 'ptr': 'in', 'ptr_len': 'buf_len', 'loc': 9, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint internalformat;
            GLsizei width;
            GLsizei height;
            GLint border;
            GLenum format;
            GLenum type;
            GLint buf_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexImage2D_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 36 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            void *pixels = (void *)call->paras[0].para_ptr;

            Texture::d_glTexImage2D_without_bound(
                opengl_context, target, level, internalformat, width, height,
                border, format, type, buf_len, pixels);
        } break;

        case FUNID_glTexImage3D_without_bound: {
            /* readline: "glTexImage3D_without_bound GLenum target, GLint level,
             * GLint internalformat, GLsizei width, GLsizei height, GLsizei
             * depth, GLint border, GLenum format, GLenum type, GLint buf_len,
             * const void *pixels#buf_len" */
            /* func name: "glTexImage3D_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'internalformat',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name':
             * 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}, {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 5, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'border', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6,
             * 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'format', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 7, 'ptr_ptr': False}, {'type':
             * 'GLenum', 'name': 'type', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 8,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'buf_len', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 9, 'ptr_ptr': False}, {'type':
             * 'const void*', 'name': 'pixels', 'ptr': 'in', 'ptr_len':
             * 'buf_len', 'loc': 10, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint internalformat;
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLint border;
            GLenum format;
            GLenum type;
            GLint buf_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexImage3D_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 40 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            border = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            void *pixels = (void *)call->paras[0].para_ptr;

            Texture::d_glTexImage3D_without_bound(
                opengl_context, target, level, internalformat, width, height,
                depth, border, format, type, buf_len, pixels);
        } break;

        case FUNID_glTexSubImage2D_without_bound: {
            /* readline: "glTexSubImage2D_without_bound GLenum target, GLint
             * level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei
             * height, GLenum format, GLenum type, GLint buf_len, const void
             * *pixels#buf_len" */
            /* func name: "glTexSubImage2D_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'width', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'height', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLenum', 'name': 'format', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False}, {'type':
             * 'GLenum', 'name': 'type', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 7,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'buf_len', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr': False}, {'type':
             * 'const void*', 'name': 'pixels', 'ptr': 'in', 'ptr_len':
             * 'buf_len', 'loc': 9, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLsizei width;
            GLsizei height;
            GLenum format;
            GLenum type;
            GLint buf_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glTexSubImage2D_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 36 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            void *pixels = (void *)call->paras[0].para_ptr;

            Texture::d_glTexSubImage2D_without_bound(
                opengl_context, target, level, xoffset, yoffset, width, height,
                format, type, buf_len, pixels);
        } break;

        case FUNID_glTexSubImage3D_without_bound: {
            /* readline: "glTexSubImage3D_without_bound GLenum target, GLint
             * level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei
             * width, GLsizei height, GLsizei depth, GLenum format, GLenum type,
             * GLint buf_len, const void *pixels#buf_len" */
            /* func name: "glTexSubImage3D_without_bound" */
            /* args: [{'type': 'GLenum', 'name': 'target', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'level', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'GLint', 'name': 'xoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'GLint', 'name': 'yoffset', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 3, 'ptr_ptr': False}, {'type': 'GLint', 'name': 'zoffset', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'GLsizei', 'name': 'width', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}, {'type': 'GLsizei', 'name': 'height',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 6, 'ptr_ptr': False},
             * {'type': 'GLsizei', 'name': 'depth', 'ptr': 'NA', 'ptr_len':
             * 'NA', 'loc': 7, 'ptr_ptr': False}, {'type': 'GLenum', 'name':
             * 'format', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 8, 'ptr_ptr':
             * False}, {'type': 'GLenum', 'name': 'type', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 9, 'ptr_ptr': False}, {'type': 'GLint',
             * 'name': 'buf_len', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 10,
             * 'ptr_ptr': False}, {'type': 'const void*', 'name': 'pixels',
             * 'ptr': 'in', 'ptr_len': 'buf_len', 'loc': 11, 'ptr_ptr': False}]
             */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLint zoffset;
            GLsizei width;
            GLsizei height;
            GLsizei depth;
            GLenum format;
            GLenum type;
            GLint buf_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glTexSubImage3D_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 44 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            xoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            yoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            zoffset = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            depth = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            void *pixels = (void *)call->paras[0].para_ptr;

            Texture::d_glTexSubImage3D_without_bound(
                opengl_context, target, level, xoffset, yoffset, zoffset, width,
                height, depth, format, type, buf_len, pixels);
        } break;

        case FUNID_glPrintf: {
            /* readline: "glPrintf GLint buf_len, const GLchar
             * *out_string#buf_len" */
            /* func name: "glPrintf" */
            /* args: [{'type': 'GLint', 'name': 'buf_len', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * GLchar*', 'name': 'out_string', 'ptr': 'in', 'ptr_len':
             * 'buf_len', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            GLint buf_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glPrintf)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            buf_len = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            void *out_string = (void *)call->paras[0].para_ptr;

            GLHelper::d_glPrintf(opengl_context, buf_len, (GLchar *)out_string);
        } break;

            /******* end of file '2-2', 18/375 functions*******/

        case FUNID_glGraphicBufferData: {
            EGLContext ctx;
            uint64_t g_buffer_id;
            int width;
            int height;
            int buf_len;
            int row_byte_len;
            int stride;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGraphicBufferData)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < (36) * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            ctx = *(EGLContext *)(temp + temp_loc);
            temp_loc += 8;

            g_buffer_id = *(uint64_t *)(temp + temp_loc);
            temp_loc += 8;

            width = *(int *)(temp + temp_loc);
            temp_loc += 4;

            height = *(int *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(int *)(temp + temp_loc);
            temp_loc += 4;

            row_byte_len = *(int *)(temp + temp_loc);
            temp_loc += 4;

            stride = *(int *)(temp + temp_loc);
            temp_loc += 4;

            void *real_buffer = (void *)call->paras[0].para_ptr;

            
            Texture::d_glGraphicBufferData(thread_context, ctx, g_buffer_id,
                                           width, height, buf_len, row_byte_len,
                                           stride, real_buffer);
        } break;

        case FUNID_glReadGraphicBuffer: {
            EGLContext ctx;
            uint64_t g_buffer_id;
            int width;
            int height;
            int buf_len;
            int row_byte_len;
            int stride;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glReadGraphicBuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < (8 * 2 + 4 * 5) * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            ctx = *(EGLContext *)(temp + temp_loc);
            temp_loc += 8;

            g_buffer_id = *(uint64_t *)(temp + temp_loc);
            temp_loc += 8;

            width = *(int *)(temp + temp_loc);
            temp_loc += 4;

            height = *(int *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(int *)(temp + temp_loc);
            temp_loc += 4;

            row_byte_len = *(int *)(temp + temp_loc);
            temp_loc += 4;

            stride = *(int *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = buf_len;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            void *real_buffer = ret_buf + out_buf_loc;
            out_buf_loc += buf_len;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            
            Texture::d_glReadGraphicBuffer(thread_context, ctx, g_buffer_id,
                                           width, height, buf_len, row_byte_len,
                                           stride, real_buffer);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glGetStaticValues: {
            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetStaticValues)) {
                break;
            }

            int read_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);

            if (unlikely(read_len > sizeof(Static_Context_Values))) {
                read_len = sizeof(Static_Context_Values);
            }

            while (!MainRenderer::get_instance()->static_context_values_initialized) {}

            FrameCallPoller::return_call(
                thread_context,
                (char *)MainRenderer::get_instance()->static_context_values,
                sizeof(Static_Context_Values) + 512 * 100 + 400);
        } break;

        case FUNID_glGetProgramDataSize: {
            GLuint program;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramDataSize)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp =
                (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            unsigned int temp_loc = 0;

            program = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = 4;
            int ret_int = 0;
            int *data_len = (int *)(&ret_int);

            GLProgram::d_glGetProgramDataSize(
                opengl_context,
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                data_len);
            FrameCallPoller::return_call(thread_context, (char *)data_len,
                                         out_buf_len);
        } break;

        case FUNID_glGetProgramData: {
            GLuint program;
            int buf_len;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glGetProgramData)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            program = *(int *)(temp + temp_loc);
            temp_loc += 4;

            buf_len = *(int *)(temp + temp_loc);
            temp_loc += 4;

            int out_buf_len = buf_len;

            unsigned char *ret_buf = nullptr;

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                ret_buf = new unsigned char[out_buf_len];
            } else {
                ret_buf = ret_local_buf;
            }
            int out_buf_loc = 0;

            void *real_buffer = ret_buf + out_buf_loc;
            out_buf_loc += buf_len;

            if (unlikely(out_buf_loc > out_buf_len)) {
                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                    delete[] ret_buf;
                }
                break;
            }

            GLProgram::d_glGetProgramData(
                opengl_context,
                (GLuint)GLResource::get_host_program_id(opengl_context,
                                                        (unsigned int)program),
                buf_len, real_buffer);

            FrameCallPoller::return_call(thread_context, (char *)ret_buf,
                                         out_buf_len);

            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
                delete[] ret_buf;
            }
        } break;

        case FUNID_glSync: {
            
            ALOGE("guest sync\n");
            // glFinish();
        } break;

        case FUNID_glBindImageTexture: {
            /* Define variables */
            GLuint unit;
            GLuint texture;
            GLint level;
            GLboolean layered;
            GLint layer;
            GLenum access;
            GLenum format;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindImageTexture)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 25 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            unit = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            texture = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            level = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            layered = *(GLboolean *)(temp + temp_loc);
            temp_loc += 1;

            layer = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            access = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            format = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glBindImageTexture(unit,
                               (GLuint)GLResource::get_host_texture_id(
                                   opengl_context, (unsigned int)texture),
                               level, layered, layer, access, format);
        } break;

        case FUNID_glBindVertexBuffer: {
            GLuint bindingindex;
            GLuint buffer;
            GLintptr offset;
            GLsizei stride;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindVertexBuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            bindingindex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            buffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            stride = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glBindVertexBuffer(bindingindex,
                               (GLuint)GLResource::get_host_buffer_id(
                                   opengl_context, (unsigned int)buffer),
                               offset, stride);
        } break;

        case FUNID_glVertexAttribFormat: {
            GLuint attribindex;
            GLint size;
            GLenum type;
            GLboolean normalized;
            GLuint relativeoffset;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttribFormat)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 17 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            attribindex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            normalized = *(GLboolean *)(temp + temp_loc);
            temp_loc += 1;

            relativeoffset = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glVertexAttribFormat(attribindex, size, type, normalized,
                                 relativeoffset);
        } break;

        case FUNID_glVertexAttribIFormat: {
            GLuint attribindex;
            GLint size;
            GLenum type;
            GLuint relativeoffset;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttribIFormat)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            attribindex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            size = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            relativeoffset = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glVertexAttribIFormat(attribindex, size, type, relativeoffset);
        } break;

        case FUNID_glVertexAttribBinding: {
            GLuint attribindex;
            GLuint bindingindex;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexAttribBinding)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            attribindex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            bindingindex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glVertexAttribBinding(attribindex, bindingindex);
        } break;

        case FUNID_glDispatchCompute: {
            GLuint num_groups_x;
            GLuint num_groups_y;
            GLuint num_groups_z;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDispatchCompute)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            num_groups_x = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            num_groups_y = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            num_groups_z = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
        } break;

        case FUNID_glDispatchComputeIndirect: {
            GLintptr indirect;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDispatchComputeIndirect)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            indirect = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glDispatchComputeIndirect(indirect);
        } break;

        case FUNID_glMemoryBarrier: {
            GLbitfield barriers;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glMemoryBarrier)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            barriers = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glMemoryBarrier(barriers);
        } break;

        case FUNID_glMemoryBarrierByRegion: {
            GLbitfield barriers;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glMemoryBarrierByRegion)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            barriers = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glMemoryBarrierByRegion(barriers);
        } break;

        case FUNID_glFramebufferParameteri: {
            GLenum target;
            GLenum pname;
            GLint param;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glFramebufferParameteri)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            pname = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            param = *(GLint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glFramebufferParameteri(target, pname, param);
        } break;

        case FUNID_glSampleMaski: {
            GLuint maskNumber;
            GLbitfield mask;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glSampleMaski)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            maskNumber = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            mask = *(GLbitfield *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            glSampleMaski(maskNumber, mask);
        } break;

        case FUNID_glTexStorage2DMultisample: {
            GLenum target;
            GLsizei samples;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;
            GLboolean fixedsamplelocations;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexStorage2DMultisample)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 21 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            samples = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            width = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            height = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            fixedsamplelocations = *(GLboolean *)(temp + temp_loc);
            temp_loc += 1;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_texture =
                    GLState::get_guest_binding_texture(opengl_context, target);
                //    glTextureStorage2DMultisample(bind_texture, samples,
                //    internalformat, width, height, fixedsamplelocations);
                ALOGE("DSA not supported.");
            } else {
                glTexStorage2DMultisample(target, samples, internalformat,
                                          width, height, fixedsamplelocations);
            }
        } break;

        case FUNID_glValidateProgramPipeline: {
            GLuint pipeline;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glValidateProgramPipeline)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            pipeline = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glValidateProgramPipeline((GLuint)GLResource::get_host_pipeline_id(
                opengl_context, (unsigned int)pipeline));
        } break;

        case FUNID_glVertexBindingDivisor: {
            GLuint bindingindex;
            GLuint divisor;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glVertexBindingDivisor)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            bindingindex = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            divisor = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glVertexBindingDivisor_special(opengl_context,
                                                     bindingindex, divisor);
        } break;

        case FUNID_glDrawArraysIndirect_with_bound: {
            GLenum mode;
            GLintptr indirect;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawArraysIndirect_with_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            indirect = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            Vertex::d_glDrawArraysIndirect_with_bound(opengl_context, mode,
                                                      indirect);
        } break;

        case FUNID_glDrawArraysIndirect_without_bound: {
            GLenum mode;
            void *indirect;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawArraysIndirect_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 4 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            
            indirect = (void *)(temp + temp_loc);

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            Vertex::d_glDrawArraysIndirect_without_bound(opengl_context, mode,
                                                         indirect);
        } break;

        case FUNID_glDrawElementsIndirect_with_bound: {
            GLenum mode;
            GLenum type;
            GLintptr indirect;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBindFramebuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 16 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            indirect = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            Vertex::d_glDrawElementsIndirect_with_bound(opengl_context, mode,
                                                        type, indirect);
        } break;

        case FUNID_glDrawElementsIndirect_without_bound: {
            GLenum mode;
            GLenum type;
            void *indirect;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num <
                         PARA_NUM_MIN_glDrawElementsIndirect_without_bound)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            mode = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            type = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            
            indirect = (void *)(temp + temp_loc);

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }
            // ALOGD("context %llx glBindFramebuffer %x guest %d host
            // %d\n",(uint64_t)opengl_context,target,framebuffer,(GLuint)get_host_framebuffer_id(opengl_context,
            // (unsigned int)framebuffer));
            Vertex::d_glDrawElementsIndirect_without_bound(opengl_context, mode,
                                                           type, indirect);
        }

        break;

        case FUNID_glDiscardFramebufferEXT: {
            /* Define variables */
            GLenum target;
            GLsizei numAttachments;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glDiscardFramebufferEXT)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            numAttachments = *(GLsizei *)(temp + temp_loc);
            temp_loc += 4;

            GLenum *attachments = (GLenum *)(temp + temp_loc);
            temp_loc += numAttachments * sizeof(GLenum);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            for (int i = 0; i < numAttachments; i++) {
                if (attachments[i] == GL_COLOR) {
                    attachments[i] = GL_COLOR_ATTACHMENT0;
                }
                if (attachments[i] == GL_DEPTH) {
                    attachments[i] = GL_DEPTH_ATTACHMENT;
                }
                if (attachments[i] == GL_STENCIL) {
                    attachments[i] = GL_DEPTH_STENCIL_ATTACHMENT;
                }
            }
            glInvalidateFramebuffer(target, numAttachments, attachments);
            // glDiscardFramebufferEXT(target, numAttachments, attachments);
        } break;

        case FUNID_glTexBuffer: {
            GLenum target;
            GLenum internalformat;
            GLuint buffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexBuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            buffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_buffer = GLState::GLState::get_guest_binding_buffer(
                    opengl_context, target);
                //    glTextureBuffer(bind_buffer, internalformat,
                //    (GLuint)GLResource::get_host_buffer_id(opengl_context,
                //    (unsigned int)buffer));
                ALOGE("DSA not supported.");
            } else {
                glTexBuffer(target, internalformat,
                            (GLuint)GLResource::get_host_buffer_id(
                                opengl_context, (unsigned int)buffer));
            }
        } break;

        case FUNID_glTexBufferRange: {
            GLenum target;
            GLenum internalformat;
            GLuint buffer;
            GLintptr offset;
            GLsizeiptr size;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glTexBufferRange)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 28 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            target = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            internalformat = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            buffer = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            offset = *(GLintptr *)(temp + temp_loc);
            temp_loc += 8;

            size = *(GLsizeiptr *)(temp + temp_loc);
            temp_loc += 8;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            if (DSA_LIKELY(MainRenderer::HOST_OPENGL_VERSION >= 45 &&
                           MainRenderer::DSA_ENABLE != 0)) {
                GLuint bind_buffer = GLState::GLState::get_guest_binding_buffer(
                    opengl_context, target);
                glTexBufferRange(bind_buffer, internalformat,
                                 (GLuint)GLResource::get_host_buffer_id(
                                     opengl_context, (unsigned int)buffer),
                                 offset, size);
                ALOGE("DSA not supported.");
            } else {
                glTexBufferRange(target, internalformat,
                                 (GLuint)GLResource::get_host_buffer_id(
                                     opengl_context, (unsigned int)buffer),
                                 offset, size);
            }
        } break;

        case FUNID_glColorMaski: {
            GLuint buf;
            GLboolean red;
            GLboolean green;
            GLboolean blue;
            GLboolean alpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glColorMaski)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 8 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            buf = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            red = *(GLboolean *)(temp + temp_loc);
            temp_loc += 1;

            green = *(GLboolean *)(temp + temp_loc);
            temp_loc += 1;

            blue = *(GLboolean *)(temp + temp_loc);
            temp_loc += 1;

            alpha = *(GLboolean *)(temp + temp_loc);
            temp_loc += 1;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glColorMaski(buf, red, green, blue, alpha);
        } break;

        case FUNID_glBlendFuncSeparatei: {
            GLuint buf;
            GLenum srcRGB;
            GLenum dstRGB;
            GLenum srcAlpha;
            GLenum dstAlpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBlendFuncSeparatei)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 20 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            buf = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            srcRGB = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            dstRGB = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            srcAlpha = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            dstAlpha = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBlendFuncSeparatei(buf, srcRGB, dstRGB, srcAlpha, dstAlpha);
        } break;

        case FUNID_glBlendEquationSeparatei: {
            GLuint buf;
            GLenum modeRGB;
            GLenum modeAlpha;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_glBlendEquationSeparatei)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 12 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            buf = *(GLuint *)(temp + temp_loc);
            temp_loc += 4;

            modeRGB = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            modeAlpha = *(GLenum *)(temp + temp_loc);
            temp_loc += 4;

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            glBlendEquationSeparatei(buf, modeRGB, modeAlpha);
        } break;

            // case FUNID_glBindSharedGLImage:

            // {

            //     /* Define variables */
            //     GLenum target;
            //     GLint texture;
            //     EGLContext share_ctx;

            //     int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            //     if (unlikely(para_num < PARA_NUM_MIN_glBindSharedGLImage))
            //     {
            //         break;
            //     }

            //     size_t temp_len = 0;
            //     unsigned char *temp = nullptr;

            //     temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            //     if (unlikely(temp_len < 4+4+8))
            //     {
            //         break;
            //     }

            //     int null_flag = 0;
            //     temp = get_direct_ptr(call->params[0].param_data_ptr,
            //     &null_flag); if (unlikely(temp == NULL))
            //     {
            //         if (temp_len != 0 && null_flag == 0)
            //         {
            //             temp = malloc(temp_len);no_ptr_buf=temp;
            //             guest_write(call->params[0].param_data_ptr, temp, 0,
            //             call->params[0].param_size);
            //         }
            //         else
            //         {
            //             break;
            //         }
            //     }

            //     unsigned int temp_loc = 0;

            //     target = *(GLenum *)(temp + temp_loc);
            //     temp_loc += 4;

            //     texture = *(GLenum *)(temp + temp_loc);
            //     temp_loc += 4;

            //     share_ctx = *(EGLContext *)(temp + temp_loc);
            //     temp_loc += 8;

            //     /* Check length */
            //     if (unlikely(temp_len < temp_loc))
            //     {
            //         break;
            //     }

            //     Opengl_Context *share_context = (Opengl_Context
            //     *)g_hash_table_lookup(thread_context->process_context->context_map,
            //     GUINT_TO_POINTER(share_ctx));

            // //    d_glBindSharedGLImage(opengl_context, target, texture,
            // share_context);
            // }
            // break;

            // case FUNID_glFramebufferSharedTexture2D:

            // {

            //     GLenum target;
            //     GLenum attachment;
            //     GLenum textarget;
            //     GLuint texture;
            //     GLint level;
            //     EGLContext share_ctx;

            //     int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            //     if (unlikely(para_num <
            //     PARA_NUM_MIN_glFramebufferSharedTexture2D))
            //     {
            //         break;
            //     }

            //     size_t temp_len = 0;
            //     unsigned char *temp = nullptr;

            //     temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            //     if (unlikely(temp_len < 20 * 1 + 8))
            //     {
            //         break;
            //     }

            //     int null_flag = 0;
            //     temp = get_direct_ptr(call->params[0].param_data_ptr,
            //     &null_flag); if (unlikely(temp == NULL))
            //     {
            //         if (temp_len != 0 && null_flag == 0)
            //         {
            //             temp = malloc(temp_len);no_ptr_buf=temp;
            //             guest_write(call->params[0].param_data_ptr, temp, 0,
            //             call->params[0].param_size);
            //         }
            //         else
            //         {
            //             break;
            //         }
            //     }

            //     unsigned int temp_loc = 0;

            //     target = *(GLenum *)(temp + temp_loc);
            //     temp_loc += 4;

            //     attachment = *(GLenum *)(temp + temp_loc);
            //     temp_loc += 4;

            //     textarget = *(GLenum *)(temp + temp_loc);
            //     temp_loc += 4;

            //     texture = *(GLuint *)(temp + temp_loc);
            //     temp_loc += 4;

            //     level = *(GLint *)(temp + temp_loc);
            //     temp_loc += 4;

            //     share_ctx = *(EGLContext *)(temp + temp_loc);
            //     temp_loc += 8;

            //     /* Check length */
            //     if (unlikely(temp_len < temp_loc))
            //     {
            //         break;
            //     }
            //     // GLuint t;
            //     // glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, (GLint *)&t);
            //     // ALOGD("#%llx FramebufferTexture2D %u target %x attachment
            //     %x textarget %x guest %u texture %u level
            //     %d\n",(uint64_t)opengl_context,t,target, attachment,
            //     textarget, texture,
            //     (GLuint)GLResource::get_host_texture_id(opengl_context,
            //     (unsigned int)texture), level); Opengl_Context *share_context
            //     = (Opengl_Context
            //     *)g_hash_table_lookup(thread_context->process_context->context_map,
            //     GUINT_TO_POINTER(share_ctx));

            // //    d_glFramebufferSharedTexture2D(opengl_context, target,
            // attachment, textarget, texture, level, share_context);
            // }
            // break;

            // case FUNID_glFramebufferEGLImage:

            // {

            //     /* Define variables */
            //     GLenum target;
            //     GLenum attachment;
            //     GLenum textarget;
            //     GLeglImageOES image;
            //     GLint level;

            //     int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            //     if (unlikely(para_num < PARA_NUM_MIN_glFramebufferEGLImage))
            //     {
            //         break;
            //     }

            //     size_t temp_len = 0;
            //     unsigned char *temp = nullptr;

            //     temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            //     if (unlikely(temp_len < 24 * 1))
            //     {
            //         break;
            //     }

            //     int null_flag = 0;
            //     temp = get_direct_ptr(call->params[0].param_data_ptr,
            //     &null_flag); if (unlikely(temp == NULL))
            //     {
            //         if (temp_len != 0 && null_flag == 0)
            //         {
            //             temp = malloc(temp_len);no_ptr_buf=temp;
            //             guest_write(call->params[0].param_data_ptr, temp, 0,
            //             call->params[0].param_size);
            //         }
            //         else
            //         {
            //             break;
            //         }
            //     }

            //     unsigned int temp_loc = 0;

            //     target = *(GLenum *)(temp + temp_loc);
            //     temp_loc += 4;

            //     attachment = *(GLenum *)(temp + temp_loc);
            //     temp_loc += 4;

            //     textarget = *(GLenum *)(temp + temp_loc);
            //     temp_loc += 4;

            //     image = *(uint64_t *)(temp + temp_loc);
            //     temp_loc += 8;

            //     level = *(GLint *)(temp + temp_loc);
            //     temp_loc += 4;
            //     /* Check length */
            //     if (unlikely(temp_len < temp_loc))
            //     {
            //         break;
            //     }
            // //    d_glFramebufferEGLImage(opengl_context, target, attachment,
            // textarget, image, level);
            // }
            // break;

        default: {
            ALOGD("error! invoke call id %llx not exist!\n", call->call_id);
        } break;
    }
}
