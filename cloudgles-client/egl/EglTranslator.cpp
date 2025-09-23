#define LOG_TAG "EglTranslator"
#define LOG_LEVEL LOG_LEVEL_ERROR

#include "EglTranslator.h"

#include "Config.h"
#include "Context.h"
#include "EglFuncID.h"
#include "GPU.h"
#include "Log.h"
#include "Surface.h"
#include "Sync.h"
#include "Utils.h"
#include "renderer/MainRenderer.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

void EglTranslator::translate(ThreadContext *thread_context,
                              CompressGLCall *call) {
    if (unlikely(thread_context == nullptr)) {
        return;
    }

    unsigned char ret_local_buf[1024 * 4];

    unsigned char *no_ptr_buf = nullptr;
    EGLint error = eglGetError();
    if (error != EGL_SUCCESS) error = -1;

    ALOGD("Thread %" PRIu64 ": %s has been called", thread_context->remote_tid,
          get_function_name(call->call_id).c_str());

    switch (call->call_id) {
            /******* file '1-1' *******/

            /******* end of file '1-1', 15/14 functions*******/

            /******* file '1-2' *******/

        case FUNID_eglMakeCurrent: {
            /* readline: "EGLBoolean eglMakeCurrent EGLDisplay dpy, EGLSurface
             * draw, EGLSurface read, EGLContext ctx, uint64_t gbuffer_id" */
            /* func name: "eglMakeCurrent" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLSurface', 'name': 'draw', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'EGLSurface', 'name':
             * 'read', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'EGLContext', 'name': 'ctx', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "120" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSurface draw;
            EGLSurface read;
            EGLContext ctx;
            uint64_t gbuffer_id;
            int64_t mc_fence;
            int width;
            int height;
            int hal_format;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglMakeCurrent)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < 60 * 1)) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            draw = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            read = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            ctx = *(EGLContext *)(temp + temp_loc);
            temp_loc += 8;

            mc_fence = *(int64_t *)(temp + temp_loc);
            temp_loc += 8;

            gbuffer_id = *(uint64_t *)(temp + temp_loc);
            temp_loc += 8;

            width = *(int *)(temp + temp_loc);
            temp_loc += 4;

            height = *(int *)(temp + temp_loc);
            temp_loc += 4;

            hal_format = *(int *)(temp + temp_loc);
            temp_loc += 4;

            EglContext::d_eglMakeCurrent(thread_context, dpy, draw, read, ctx,
                                         gbuffer_id, width, height, hal_format,
                                         mc_fence);

        } break;

        case FUNID_eglSwapBuffers_sync: {
            /* readline: "EGLBoolean eglSwapBuffers_sync EGLDisplay dpy,
             * EGLSurface surface" */
            /* func name: "eglSwapBuffers_sync" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLSurface', 'name': 'surface', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "120" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSurface surface;
            uint64_t gbuffer_id;
            int width;
            int height;
            int hal_format;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglSwapBuffers_sync)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            gbuffer_id = *(uint64_t *)(temp + temp_loc);
            temp_loc += 8;

            width = *(int *)(temp + temp_loc);
            temp_loc += 4;

            height = *(int *)(temp + temp_loc);
            temp_loc += 4;

            hal_format = *(int *)(temp + temp_loc);
            temp_loc += 4;

            EGLBoolean ret = EglGPU::d_eglSwapBuffers_sync(
                thread_context, dpy, surface, gbuffer_id, width, height,
                hal_format);

            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(EGLBoolean));
        } break;

        case FUNID_eglWaitGL: {
            /* readline: "EGLBoolean eglWaitGL void" */
            /* func name: "eglWaitGL" */
            /* args: [] */
            /* ret: "EGLBoolean" */
            /* type: "120" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglWaitGL)) {
                break;
            }

            EGLBoolean ret = EglSync::d_eglWaitGL(thread_context);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(EGLBoolean));
        } break;

        case FUNID_eglWaitNative: {
            /* readline: "EGLBoolean eglWaitNative EGLint engine" */
            /* func name: "eglWaitNative" */
            /* args: [{'type': 'EGLint', 'name': 'engine', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "120" */

            /* Define variables */
            EGLint engine;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglWaitNative)) {
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

            engine = *(EGLint *)(temp + temp_loc);

            EGLBoolean ret = EglSync::d_eglWaitNative(thread_context, engine);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(EGLBoolean));
        } break;

        case FUNID_eglWaitClient: {
            /* readline: "EGLBoolean eglWaitClient void" */
            /* func name: "eglWaitClient" */
            /* args: [] */
            /* ret: "EGLBoolean" */
            /* type: "120" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglWaitClient)) {
                break;
            }

            EGLBoolean ret = EglSync::d_eglWaitClient(thread_context);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(EGLBoolean));
        } break;

        case FUNID_eglClientWaitSync: {
            /* readline: "EGLint eglClientWaitSync EGLDisplay dpy, EGLSync sync,
             * EGLint flags, EGLTime timeout" */
            /* func name: "eglClientWaitSync" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'EGLSync',
             * 'name': 'sync', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'EGLint', 'name': 'flags', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}, {'type':
             * 'EGLTime', 'name': 'timeout', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "EGLint" */
            /* type: "120" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSync sync;
            EGLint flags;
            EGLTime timeout;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglClientWaitSync)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            sync = *(EGLSync *)(temp + temp_loc);
            temp_loc += 8;

            flags = *(EGLint *)(temp + temp_loc);
            temp_loc += 4;

            timeout = *(EGLTime *)(temp + temp_loc);
            temp_loc += 8;

            EGLint ret = EglSync::d_eglClientWaitSync(thread_context, dpy, sync,
                                                      flags, timeout);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(EGLint));
        } break;

            //        case FUNID_eglGetSyncAttrib: {
            //
            //            /* readline: "EGLBoolean eglGetSyncAttrib EGLDisplay
            //            dpy, EGLSync sync, EGLint attribute, EGLint
            //            *value#sizeof(EGLint)" */
            //            /* func name: "eglGetSyncAttrib" */
            //            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr':
            //            'NA', 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False},
            //            {'type': 'EGLSync', 'name': 'sync', 'ptr': 'NA',
            //            'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}, {'type':
            //            'EGLint', 'name': 'attribute', 'ptr': 'NA', 'ptr_len':
            //            'NA', 'loc': 2, 'ptr_ptr': False}, {'type': 'EGLint*',
            //            'name': 'value', 'ptr': 'out', 'ptr_len':
            //            'sizeof(EGLint)', 'loc': 3, 'ptr_ptr': False}] */
            //            /* ret: "EGLBoolean" */
            //            /* type: "120" */
            //
            //            /* Define variables */
            //            EGLDisplay dpy;
            //            EGLSync sync;
            //            EGLint attribute;
            //
            //            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            //            if (unlikely(para_num <
            //            PARA_NUM_MIN_eglGetSyncAttrib)) {
            //                break;
            //            }
            //
            //            size_t temp_len = 0;
            //            unsigned char *temp = nullptr;
            //
            //            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            //            if (unlikely(temp_len < 20 * 1)) {
            //                break;
            //            }
            //
            //            int null_flag = 0;
            //            temp = (unsigned char *)
            //            COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            //
            //
            //            unsigned int temp_loc = 0;
            //
            //            dpy = *(EGLDisplay *) (temp + temp_loc);
            //            temp_loc += 8;
            //
            //            sync = *(EGLSync *) (temp + temp_loc);
            //            temp_loc += 8;
            //
            //            attribute = *(EGLint *) (temp + temp_loc);
            //            temp_loc += 4;
            //            int out_buf_len = call->params[1].param_size;
            //
            //            unsigned char *ret_buf = nullptr;
            //
            //            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
            //                ret_buf = new unsigned char[out_buf_len];
            //            } else {
            //                ret_buf = ret_local_buf;
            //            }
            //            int out_buf_loc = 0;
            //
            //            EGLint *value = (EGLint *) (ret_buf + out_buf_loc);
            //            out_buf_loc += sizeof(EGLint);
            //
            //            EGLBoolean *ret_ptr = (EGLBoolean *) (ret_buf +
            //            out_buf_loc); out_buf_loc += sizeof(EGLBoolean);
            //
            //            if (unlikely(out_buf_loc > out_buf_len)) {
            //                if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
            //                    delete[] ret_buf;
            //                }
            //                break;
            //            }
            //
            //            EGLBoolean ret =
            //            EglSync::d_eglGetSyncAttrib(thread_context, dpy,
            //            sync, attribute,
            //                                                                 value);
            //            *ret_ptr = ret;
            //
            //            memcpy(call->params[1].param_data_ptr, ret_buf,
            //            out_buf_len);
            //
            //            if (unlikely(out_buf_len > MAX_OUT_BUF_LEN)) {
            //                delete[] ret_buf;
            //            }
            //        }
            //            break;

            /******* end of file '1-2', 10/23 functions*******/

            /******* file '2-1' *******/

        case FUNID_eglChooseConfig: {
            /* readline: "EGLBoolean eglChooseConfig EGLDisplay dpy, const
             * EGLint *attrib_list#EglConfig::get_attrib_list_len(attrib_list),
             * EGLConfig *configs#config_size*sizeof(EGLConfig), EGLint
             * config_size, EGLint *num_config#sizeof(EGLint)" */
            /* func name: "eglChooseConfig" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'const
             * EGLint*', 'name': 'attrib_list', 'ptr': 'in', 'ptr_len':
             * 'EglConfig::get_attrib_list_len(attrib_list)', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'EGLConfig*', 'name': 'configs',
             * 'ptr': 'out', 'ptr_len': 'config_size*sizeof(EGLConfig)', 'loc':
             * 2, 'ptr_ptr': False}, {'type': 'EGLint', 'name': 'config_size',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False},
             * {'type': 'EGLint*', 'name': 'num_config', 'ptr': 'out',
             * 'ptr_len': 'sizeof(EGLint)', 'loc': 4, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* TODO: More than one ptr, should check mannually */
            /* Define variables */
            EGLDisplay dpy;
            EGLint config_size;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglChooseConfig)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            config_size = *(EGLint *)(temp + temp_loc);
            temp_loc += 4;

            const EGLint *attrib_list = (const EGLint *)(temp + temp_loc);
            temp_loc += EglConfig::get_attrib_list_len(attrib_list);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglConfig::d_eglChooseConfig(thread_context, dpy, attrib_list,
                                         nullptr, config_size, nullptr);
        } break;

        case FUNID_eglCreatePbufferSurface: {
            /* readline: "EGLSurface eglCreatePbufferSurface EGLDisplay dpy,
             * EGLConfig config, const EGLint
             * *attrib_list#EglConfig::get_attrib_list_len(attrib_list),
             * EGLSurface surface" */
            /* func name: "eglCreatePbufferSurface" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLConfig', 'name': 'config', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'const EGLint*', 'name':
             * 'attrib_list', 'ptr': 'in', 'ptr_len':
             * 'EglConfig::get_attrib_list_len(attrib_list)', 'loc': 2,
             * 'ptr_ptr': False}, {'type': 'EGLSurface', 'name': 'surface',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "EGLSurface" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLConfig config;
            EGLSurface surface;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglCreatePbufferSurface)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            config = *(EGLConfig *)(temp + temp_loc);
            temp_loc += 8;

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            const EGLint *attrib_list = (const EGLint *)(temp + temp_loc);
            temp_loc += EglConfig::get_attrib_list_len(attrib_list);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSurface::d_eglCreatePbufferSurface(thread_context, dpy, config,
                                                  attrib_list, surface);
        } break;

        case FUNID_eglCreateWindowSurface: {
            /* readline: "EGLSurface eglCreateWindowSurface EGLDisplay dpy,
             * EGLConfig config, EGLNativeWindowType win, const EGLint
             * *attrib_list#EglConfig::get_attrib_list_len(attrib_list),
             * EGLSurface surface" */
            /* func name: "eglCreateWindowSurface" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLConfig', 'name': 'config', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'EGLNativeWindowType',
             * 'name': 'win', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'const EGLint*', 'name': 'attrib_list', 'ptr':
             * 'in', 'ptr_len': 'EglConfig::get_attrib_list_len(attrib_list)',
             * 'loc': 3, 'ptr_ptr': False}, {'type': 'EGLSurface', 'name':
             * 'surface', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4, 'ptr_ptr':
             * False}] */
            /* ret: "EGLSurface" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLConfig config;
            EGLNativeWindowType win;
            EGLSurface surface;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglCreateWindowSurface)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            config = *(EGLConfig *)(temp + temp_loc);
            temp_loc += 8;

            win = *(EGLNativeWindowType *)(temp + temp_loc);
            temp_loc += 8;

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            const EGLint *attrib_list = (const EGLint *)(temp + temp_loc);
            temp_loc += EglConfig::get_attrib_list_len(attrib_list);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSurface::d_eglCreateWindowSurface(thread_context, dpy, config,
                                                 win, attrib_list, surface);
        } break;

        case FUNID_eglCreateContext: {
            /* readline: "EGLContext eglCreateContext EGLDisplay dpy, EGLConfig
             * config, EGLContext share_context, const EGLint
             * *attrib_list#EglConfig::get_attrib_list_len(attrib_list),
             * EGLContext thread_context" */
            /* func name: "eglCreateContext" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLConfig', 'name': 'config', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'EGLContext', 'name':
             * 'share_context', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2,
             * 'ptr_ptr': False}, {'type': 'const EGLint*', 'name':
             * 'attrib_list', 'ptr': 'in', 'ptr_len':
             * 'EglConfig::get_attrib_list_len(attrib_list)', 'loc': 3,
             * 'ptr_ptr': False}, {'type': 'EGLContext', 'name':
             * 'thread_context', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 4,
             * 'ptr_ptr': False}] */
            /* ret: "EGLContext" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLConfig config;
            EGLContext share_context;
            EGLContext guest_egl_context;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglCreateContext)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            config = *(EGLConfig *)(temp + temp_loc);
            temp_loc += 8;

            share_context = *(EGLContext *)(temp + temp_loc);
            temp_loc += 8;

            guest_egl_context = *(EGLContext *)(temp + temp_loc);
            temp_loc += 8;

            const EGLint *attrib_list = (const EGLint *)(temp + temp_loc);
            temp_loc += EglConfig::get_attrib_list_len(attrib_list);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglContext::d_eglCreateContext(thread_context, dpy, config,
                                           share_context, attrib_list,
                                           guest_egl_context);
        } break;

        case FUNID_eglDestroyContext: {
            /* readline: "EGLBoolean eglDestroyContext EGLDisplay dpy,
             * EGLContext ctx" */
            /* func name: "eglDestroyContext" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLContext', 'name': 'ctx', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLContext ctx;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglDestroyContext)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            ctx = *(EGLContext *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglContext::d_eglDestroyContext(thread_context, dpy, ctx);
        } break;

        case FUNID_eglDestroySurface: {
            /* readline: "EGLBoolean eglDestroySurface EGLDisplay dpy,
             * EGLSurface surface" */
            /* func name: "eglDestroySurface" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLSurface', 'name': 'surface', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSurface surface;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglDestroySurface)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSurface::d_eglDestroySurface(thread_context, dpy, surface);
        } break;

        case FUNID_eglTerminate: {
            /* readline: "EGLBoolean eglTerminate EGLDisplay dpy" */
            /* func name: "eglTerminate" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglTerminate)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglContext::d_eglTerminate(thread_context, dpy);
        } break;

        case FUNID_eglIamComposer: {
            /* readline: "eglIamComposer EGLSurface surface" */
            /* func name: "eglIamComposer" */
            /* args: [{'type': 'EGLSurface', 'name': 'surface', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "210" */

            /* Define variables */
            EGLSurface surface;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglIamComposer)) {
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

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            unsigned int pid = thread_context->process_context->remote_pid;

            EglSurface::d_eglIamComposer(thread_context, surface, pid);
        } break;

        case FUNID_eglGetDisplay: {
            /* readline: "EGLDisplay eglGetDisplay EGLNativeDisplayType
             * display_id, EGLDisplay guest_display" */
            /* func name: "eglGetDisplay" */
            /* args: [{'type': 'EGLNativeDisplayType', 'name': 'display_id',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False},
             * {'type': 'EGLDisplay', 'name': 'guest_display', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 1, 'ptr_ptr': False}] */
            /* ret: "EGLDisplay" */
            /* type: "210" */

            /* Define variables */
            EGLNativeDisplayType display_id;
            EGLDisplay guest_display;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglGetDisplay)) {
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

            display_id = *(EGLNativeDisplayType *)(temp + temp_loc);
            temp_loc += 8;

            guest_display = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglConfig::d_eglGetDisplay(thread_context, display_id,
                                       guest_display);
        } break;

        case FUNID_eglBindTexImage: {
            /* readline: "EGLBoolean eglBindTexImage EGLDisplay dpy, EGLSurface
             * surface, EGLint buffer" */
            /* func name: "eglBindTexImage" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLSurface', 'name': 'surface', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'EGLint', 'name': 'buffer',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSurface surface;
            EGLint buffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglBindTexImage)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            buffer = *(EGLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglGPU::d_eglBindTexImage(thread_context, dpy, surface, buffer);
        } break;

        case FUNID_eglReleaseTexImage: {
            /* readline: "EGLBoolean eglReleaseTexImage EGLDisplay dpy,
             * EGLSurface surface, EGLint buffer" */
            /* func name: "eglReleaseTexImage" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLSurface', 'name': 'surface', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'EGLint', 'name': 'buffer',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSurface surface;
            EGLint buffer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglReleaseTexImage)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            buffer = *(EGLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglGPU::d_eglReleaseTexImage(thread_context, dpy, surface, buffer);
        } break;

        case FUNID_eglSurfaceAttrib: {
            /* readline: "EGLBoolean eglSurfaceAttrib EGLDisplay dpy, EGLSurface
             * surface, EGLint attribute, EGLint value" */
            /* func name: "eglSurfaceAttrib" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLSurface', 'name': 'surface', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'EGLint', 'name':
             * 'attribute', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr':
             * False}, {'type': 'EGLint', 'name': 'value', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSurface surface;
            EGLint attribute;
            EGLint value;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglSurfaceAttrib)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            attribute = *(EGLint *)(temp + temp_loc);
            temp_loc += 4;

            value = *(EGLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSurface::d_eglSurfaceAttrib(thread_context, dpy, surface,
                                           attribute, value);
        } break;

        case FUNID_eglSwapInterval: {
            /* readline: "EGLBoolean eglSwapInterval EGLDisplay dpy, EGLint
             * interval" */
            /* func name: "eglSwapInterval" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'EGLint',
             * 'name': 'interval', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLint interval;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglSwapInterval)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            interval = *(EGLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglGPU::d_eglSwapInterval(thread_context, dpy, interval);
        } break;

        case FUNID_eglReleaseThread: {
            /* readline: "EGLBoolean eglReleaseThread void" */
            /* func name: "eglReleaseThread" */
            /* args: [] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglReleaseThread)) {
                break;
            }

            EglContext::d_eglReleaseThread(thread_context);
        } break;

        case FUNID_eglCreateSync: {
            /* readline: "EGLSync eglCreateSync EGLDisplay dpy, EGLenum type,
             * const EGLint *attrib_list#sizeof(EGLint), EGLSync sync" */
            /* func name: "eglCreateSync" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'EGLenum',
             * 'name': 'type', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'const EGLint*', 'name':
             * 'attrib_list', 'ptr': 'in', 'ptr_len': 'sizeof(EGLint)', 'loc':
             * 2, 'ptr_ptr': False}, {'type': 'EGLSync', 'name': 'sync', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}] */
            /* ret: "EGLSync" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLenum type;
            EGLSync sync;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglCreateSync)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            type = *(EGLenum *)(temp + temp_loc);
            temp_loc += 4;

            sync = *(EGLSync *)(temp + temp_loc);
            temp_loc += 8;

            const EGLint *attrib_list = (const EGLint *)(temp + temp_loc);
            temp_loc += EglConfig::get_attrib_list_len(attrib_list);
            ;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSync::d_eglCreateSync(thread_context, dpy, type, attrib_list,
                                     sync);
        } break;

        case FUNID_eglDestroySync: {
            /* readline: "EGLBoolean eglDestroySync EGLDisplay dpy, EGLSync
             * sync" */
            /* func name: "eglDestroySync" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'EGLSync',
             * 'name': 'sync', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSync sync;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglDestroySync)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            sync = *(EGLSync *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSync::d_eglDestroySync(thread_context, dpy, sync);
        } break;

        case FUNID_eglCreateImage: {
            /* readline: "EGLImage eglCreateImage EGLDisplay dpy, EGLContext
             * ctx, EGLenum target, EGLClientBuffer buffer, const EGLint
             * *attrib_list#sizeof(EGLint), EGLImage image" */
            /* func name: "eglCreateImage" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLContext', 'name': 'ctx', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}, {'type': 'EGLenum', 'name': 'target',
             * 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False},
             * {'type': 'EGLClientBuffer', 'name': 'buffer', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 3, 'ptr_ptr': False}, {'type': 'const
             * EGLint*', 'name': 'attrib_list', 'ptr': 'in', 'ptr_len':
             * 'sizeof(EGLint)', 'loc': 4, 'ptr_ptr': False}, {'type':
             * 'EGLImage', 'name': 'image', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 5, 'ptr_ptr': False}] */
            /* ret: "EGLImage" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLContext ctx;
            EGLenum target;
            EGLClientBuffer buffer;
            EGLImage image;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglCreateImage)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            ctx = *(EGLContext *)(temp + temp_loc);
            temp_loc += 8;

            target = *(EGLenum *)(temp + temp_loc);
            temp_loc += 4;

            buffer = *(EGLClientBuffer *)(temp + temp_loc);
            temp_loc += 8;

            image = *(EGLImage *)(temp + temp_loc);
            temp_loc += 8;

            const EGLint *attrib_list = (const EGLint *)(temp + temp_loc);
            temp_loc += EglConfig::get_attrib_list_len(attrib_list);
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSurface::d_eglCreateImage(thread_context, dpy, ctx, target,
                                         buffer, attrib_list, image);
        } break;

        case FUNID_eglDestroyImage: {
            /* readline: "EGLBoolean eglDestroyImage EGLDisplay dpy, EGLImage
             * image" */
            /* func name: "eglDestroyImage" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLImage', 'name': 'image', 'ptr': 'NA', 'ptr_len': 'NA', 'loc':
             * 1, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLImage image;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglDestroyImage)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            image = *(EGLImage *)(temp + temp_loc);
            temp_loc += 8;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSurface::d_eglDestroyImage(thread_context, dpy, image);
        } break;

        case FUNID_eglWaitSync: {
            /* readline: "EGLBoolean eglWaitSync EGLDisplay dpy, EGLSync sync,
             * EGLint flags" */
            /* func name: "eglWaitSync" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type': 'EGLSync',
             * 'name': 'sync', 'ptr': 'NA', 'ptr_len': 'NA', 'loc': 1,
             * 'ptr_ptr': False}, {'type': 'EGLint', 'name': 'flags', 'ptr':
             * 'NA', 'ptr_len': 'NA', 'loc': 2, 'ptr_ptr': False}] */
            /* ret: "EGLBoolean" */
            /* type: "210" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSync sync;
            EGLint flags;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglWaitSync)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            sync = *(EGLSync *)(temp + temp_loc);
            temp_loc += 8;

            flags = *(EGLint *)(temp + temp_loc);
            temp_loc += 4;
            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglSync::d_eglWaitSync(thread_context, dpy, sync, flags);
        } break;

        case FUNID_eglQueueBuffer: {
            /* Define variables */
            uint64_t gbuffer_id;
            int is_composer;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglQueueBuffer)) {
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

            gbuffer_id = *(uint64_t *)(temp + temp_loc);
            temp_loc += sizeof(uint64_t);

            is_composer = *(int *)(temp + temp_loc);
            temp_loc += sizeof(int);

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglGPU::d_eglQueueBuffer(thread_context, gbuffer_id, is_composer);
        } break;

            // case FUNID_eglGetNativeClientBufferANDROID:

            // {

            //     /* readline: "EGLClientBuffer eglGetNativeClientBufferANDROID
            //     AHardwareBuffer buffer" */
            //     /* func name: "eglGetNativeClientBufferANDROID" */
            //     /* args: [{'type': 'AHardwareBuffer', 'name': 'buffer',
            //     'ptr': 'NA', 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}] */
            //     /* ret: "EGLClientBuffer" */
            //     /* type: "210" */

            //     /* Define variables */
            //     AHardwareBuffer buffer;

            //     int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            //     if (unlikely(para_num <
            //     PARA_NUM_MIN_eglGetNativeClientBufferANDROID))
            //     {
            //         break;
            //     }

            //     size_t temp_len = 0;
            //     unsigned char *temp = nullptr;

            //     temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            //     if (unlikely(temp_len < 8 * 1))
            //     {
            //         break;
            //     }

            //     int null_flag = 0;
            //     temp = (unsigned char *)
            //     COMPRESS_GL_CALL_COMMON_PARA_PTR(call); if (unlikely(temp ==
            //     NULL))
            //     {
            //         if (temp_len != 0 && null_flag == 0)
            //         {
            //             temp = g_malloc(temp_len);no_ptr_buf=temp;
            //             guest_write(COMPRESS_GL_CALL_COMMON_PARA_PTR(call),
            //             temp, 0, COMPRESS_GL_CALL_COMMON_PARA_LEN(call));
            //         }
            //         else
            //         {
            //             break;
            //         }
            //     }

            //     unsigned int temp_loc = 0;

            //     buffer = *(AHardwareBuffer *)(temp + temp_loc);
            //     temp_loc += 8;
            //     /* Check length */
            //     if (unlikely(temp_len < temp_loc))
            //     {
            //         break;
            //     }

            //     // d_eglGetNativeClientBufferANDROID(thread_context, buffer);
            // }
            // break;

            /******* end of file '2-1', 19/41 functions*******/

            /******* file '2-2' *******/

        case FUNID_eglSwapBuffers: {
            /* readline: "eglSwapBuffers EGLDisplay dpy, EGLSurface surface,
             * int64_t invoke_time, int64_t *ret_invoke_time#sizeof(int64_t), ,
             * int64_t *swap_time#sizeof(int64_t)" */
            /* func name: "eglSwapBuffers" */
            /* args: [{'type': 'EGLDisplay', 'name': 'dpy', 'ptr': 'NA',
             * 'ptr_len': 'NA', 'loc': 0, 'ptr_ptr': False}, {'type':
             * 'EGLSurface', 'name': 'surface', 'ptr': 'NA', 'ptr_len': 'NA',
             * 'loc': 1, 'ptr_ptr': False}, {'type': 'EGLint*', 'name':
             * 'ret_flag', 'ptr': 'out', 'ptr_len': 'sizeof(GLint)', 'loc': 2,
             * 'ptr_ptr': False}] */
            /* ret: "" */
            /* type: "220" */

            /* Define variables */
            EGLDisplay dpy;
            EGLSurface surface;
            uint64_t gbuffer_id;
            uint64_t remote_mem_addr;
            int width;
            int height;
            int hal_format;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglSwapBuffers)) {
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

            dpy = *(EGLDisplay *)(temp + temp_loc);
            temp_loc += 8;

            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            gbuffer_id = *(uint64_t *)(temp + temp_loc);
            temp_loc += 8;

            remote_mem_addr = *(uint64_t *)(temp + temp_loc);
            temp_loc += 8;

            width = *(int *)(temp + temp_loc);
            temp_loc += 4;

            height = *(int *)(temp + temp_loc);
            temp_loc += 4;

            hal_format = *(int *)(temp + temp_loc);
            temp_loc += 4;

            EglGPU::d_eglSwapBuffers(thread_context, dpy, surface,
                                     gbuffer_id, width, height, hal_format,
                                     remote_mem_addr);
        } break;

            /******* end of file '2-2', 2/42 functions*******/

            /******* file '3' *******/

            /******* end of file '3', 7/48 functions*******/
        case FUNID_eglTP: {
            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);

            if (para_num != 1) {
                break;
            }
            // printf("enter egltp \n");
            void *data = COMPRESS_GL_CALL_COMMON_PARA_PTR(call);
            size_t len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            //            test_no_copy(data, len);
            break;
        }

        case FUNID_eglTerminateThread: {
            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);

            if (para_num != 0) {
                break;
            }
            GLint ret = 1;
            EglContext::d_eglTerminateThread(thread_context);
            break;
        }

        case FUNID_eglSetGraphicBufferID: {
            /* Define variables */
            EGLSurface surface;
            uint64_t gbuffer_id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglQueueBuffer)) {
                break;
            }

            size_t temp_len = 0;
            unsigned char *temp = nullptr;

            temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            if (unlikely(temp_len < sizeof(uint64_t) + sizeof(EGLSurface))) {
                break;
            }

            int null_flag = 0;
            temp = (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;
            surface = *(EGLSurface *)(temp + temp_loc);
            temp_loc += 8;

            gbuffer_id = *(uint64_t *)(temp + temp_loc);
            temp_loc += sizeof(uint64_t);

            /* Check length */
            if (unlikely(temp_len < temp_loc)) {
                break;
            }

            EglGPU::d_eglSetGraphicBufferID(thread_context, surface,
                                            gbuffer_id);
        } break;

        case FUNID_eglGetGBufferType: {
            uint64_t gbuffer_id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            if (unlikely(para_num < PARA_NUM_MIN_eglGetSyncAttrib)) {
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

            gbuffer_id = *(uint64_t *)(temp + temp_loc);
            temp_loc += 8;

            int ret = MainRenderer::get_instance()->get_global_gbuffer_type(
                gbuffer_id);
            FrameCallPoller::return_call(thread_context, (char *)&ret,
                                         sizeof(int));
        } break;

        case FUNID_eglFenceWaitSync: {
            int64_t fence_id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);

            size_t temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            unsigned char *temp =
                (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            fence_id = *(int64_t *)(temp + temp_loc);
            temp_loc += 8;

            ALOGD("Wait fence_id = %lld", (long long)fence_id);
            MainRenderer::get_instance()->eglFence.wait_fence(fence_id);
        } break;

        case FUNID_eglFenceSetSync: {
            int64_t fence_id;
            int64_t reset_fence_id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);

            size_t temp_len = COMPRESS_GL_CALL_COMMON_PARA_LEN(call);
            unsigned char *temp =
                (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;

            fence_id = *(int64_t *)(temp + temp_loc);
            temp_loc += 8;
            reset_fence_id = *(int64_t *)(temp + temp_loc);

            ALOGD("Set fence_id = %lld, reset id %lld", (long long)fence_id, (long long)reset_fence_id);
            MainRenderer::get_instance()->eglFence.set_fence(fence_id,
                                                             reset_fence_id);
        } break;

        case FUNID_eglGBufferDestroy: {
            int64_t gbuffer_id;

            int para_num = COMPRESS_GL_CALL_PARA_NUM(call);
            unsigned char *temp =
                (unsigned char *)COMPRESS_GL_CALL_COMMON_PARA_PTR(call);

            unsigned int temp_loc = 0;
            gbuffer_id = *(int64_t *)(temp + temp_loc);
            temp_loc += 8;

            GraphicBuffer *gbuffer =
                MainRenderer::get_instance()->get_gbuffer_from_global_map(
                    gbuffer_id);
            if (gbuffer) {
                gbuffer->prepare_to_die();
                ALOGD("Destroy gbuffer id = %lld", (long long)gbuffer_id);
            } else {
                ALOGE("Destroy gbuffer id = %lld, but not found",
                      (long long)gbuffer_id);
            }
        }

        default:
            break;
    }
}
