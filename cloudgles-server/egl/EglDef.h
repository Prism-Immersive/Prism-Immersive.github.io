
#ifndef DEFINE_EGL_H
#define DEFINE_EGL_H

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "EGL/eglplatform.h"

#define MAX_OUT_BUF_LEN 4096

EGLBoolean d_eglGetConfigAttrib(void *context, EGLDisplay dpy, EGLConfig config,
                                EGLint attribute, EGLint *value);

EGLBoolean d_eglGetConfigs(void *context, EGLDisplay dpy, EGLConfig *configs,
                           EGLint config_size, EGLint *num_config);

EGLDisplay d_eglGetCurrentDisplay(void *context);

EGLSurface d_eglGetCurrentSurface(void *context, EGLint readdraw);

EGLDisplay d_eglGetPlatformDisplay(void *context, EGLenum platform,
                                   void *native_display,
                                   const EGLAttrib *attrib_list);

EGLDisplay d_eglGetDisplay(void *context, EGLNativeDisplayType display_id);

EGLint d_eglGetError(void *context);

EGLBoolean d_eglInitialize(void *context, EGLDisplay dpy, EGLint *major,
                           EGLint *minor);

EGLBoolean d_eglQueryContext(void *context, EGLDisplay dpy, EGLContext ctx,
                             EGLint attribute, EGLint *value);

EGLBoolean d_eglQuerySurface(void *context, EGLDisplay dpy, EGLSurface surface,
                             EGLint attribute, EGLint *value);

const char *d_eglQueryString(void *context, EGLDisplay dpy, EGLint name);

__eglMustCastToProperFunctionPointerType d_eglGetProcAddress(
    void *context, const char *procname);

EGLBoolean d_eglBindAPI(void *context, EGLenum api);

EGLenum d_eglQueryAPI(void *context);

EGLContext d_eglGetCurrentContext(void *context);

EGLBoolean r_eglMakeCurrent(void *context, EGLDisplay dpy, EGLSurface draw,
                            EGLSurface read, EGLContext ctx,
                            uint64_t gbuffer_id, int width, int height,
                            int hal_format, int64_t mc_fence);

EGLBoolean d_eglMakeCurrent(void *context, EGLDisplay dpy, EGLSurface draw,
                            EGLSurface read, EGLContext ctx);

EGLBoolean d_eglSwapBuffers(void *context, EGLDisplay dpy, EGLSurface surface);

EGLBoolean d_eglSwapBuffersWithDamageKHR(void *context, EGLDisplay dpy, EGLSurface surface,
                                         const EGLint *rects, EGLint n_rects);
EGLBoolean d_eglSetDamageRegionKHR(void *context, EGLDisplay dpy, EGLSurface surface,
                                 EGLint *rects, EGLint n_rects);

EGLBoolean r_eglWaitGL(void *context);

EGLBoolean d_eglWaitGL(void *context);

EGLBoolean r_eglWaitNative(void *context, EGLint engine);

EGLBoolean d_eglWaitNative(void *context, EGLint engine);

EGLBoolean r_eglWaitClient(void *context);

EGLBoolean d_eglWaitClient(void *context);

EGLint r_eglClientWaitSync(void *context, EGLDisplay dpy, EGLSync sync,
                           EGLint flags, EGLTime timeout);

EGLint d_eglClientWaitSync(void *context, EGLDisplay dpy, EGLSync sync,
                           EGLint flags, EGLTime timeout);

// EGLBoolean
// r_eglGetSyncAttrib(void *context, EGLDisplay dpy, EGLSync sync, EGLint
// attribute, EGLint *value);

EGLBoolean d_eglGetSyncAttrib(void *context, EGLDisplay dpy, EGLSync sync,
                              EGLint attribute, EGLAttrib *value);

EGLBoolean d_eglChooseConfig(void *context, EGLDisplay dpy,
                             const EGLint *attrib_list, EGLConfig *configs,
                             EGLint config_size, EGLint *num_config);

void r_eglCreatePbufferSurface(void *context, EGLDisplay dpy, EGLConfig config,
                               const EGLint *attrib_list, EGLSurface surface);

EGLSurface d_eglCreatePbufferSurface(void *context, EGLDisplay dpy,
                                     EGLConfig config,
                                     const EGLint *attrib_list);

void r_eglCreateWindowSurface(void *context, EGLDisplay dpy, EGLConfig config,
                              EGLNativeWindowType win,
                              const EGLint *attrib_list, EGLSurface surface);

EGLSurface d_eglCreateWindowSurface(void *context, EGLDisplay dpy,
                                    EGLConfig config, EGLNativeWindowType win,
                                    const EGLint *attrib_list);

void r_eglCreateContext(void *context, EGLDisplay dpy, EGLConfig config,
                        EGLContext share_context, const EGLint *attrib_list,
                        EGLContext egl_context);

EGLContext d_eglCreateContext(void *context, EGLDisplay dpy, EGLConfig config,
                              EGLContext share_context,
                              const EGLint *attrib_list);

void r_eglDestroyContext(void *context, EGLDisplay dpy, EGLContext ctx);

EGLBoolean d_eglDestroyContext(void *context, EGLDisplay dpy, EGLContext ctx);

void r_eglDestroySurface(void *context, EGLDisplay dpy, EGLSurface surface);

EGLBoolean d_eglDestroySurface(void *context, EGLDisplay dpy,
                               EGLSurface surface);

void r_eglTerminate(void *context, EGLDisplay dpy);

EGLBoolean d_eglTerminate(void *context, EGLDisplay dpy);

void r_eglIamComposer(void *context, EGLSurface surface);

// void d_eglIamComposer(void *context, EGLSurface surface);

void r_eglBindTexImage(void *context, EGLDisplay dpy, EGLSurface surface,
                       EGLint buffer);

EGLBoolean d_eglBindTexImage(void *context, EGLDisplay dpy, EGLSurface surface,
                             EGLint buffer);

void r_eglReleaseTexImage(void *context, EGLDisplay dpy, EGLSurface surface,
                          EGLint buffer);

EGLBoolean d_eglReleaseTexImage(void *context, EGLDisplay dpy,
                                EGLSurface surface, EGLint buffer);

void r_eglSurfaceAttrib(void *context, EGLDisplay dpy, EGLSurface surface,
                        EGLint attribute, EGLint value);

EGLBoolean d_eglSurfaceAttrib(void *context, EGLDisplay dpy, EGLSurface surface,
                              EGLint attribute, EGLint value);

void r_eglSwapInterval(void *context, EGLDisplay dpy, EGLint interval);

EGLBoolean d_eglSwapInterval(void *context, EGLDisplay dpy, EGLint interval);

void r_eglReleaseThread(void *context);

EGLBoolean d_eglReleaseThread(void *context);

void r_eglCreateSync(void *context, EGLDisplay dpy, EGLenum type,
                     const EGLint *attrib_list, EGLSync sync);

EGLSync d_eglCreateSync(void *context, EGLDisplay dpy, EGLenum type,
                        const EGLAttrib *attrib_list);

void r_eglDestroySync(void *context, EGLDisplay dpy, EGLSync sync);

EGLBoolean d_eglDestroySync(void *context, EGLDisplay dpy, EGLSync sync);

void r_eglCreateImage(void *context, EGLDisplay dpy, EGLContext ctx,
                      EGLenum target, EGLClientBuffer buffer,
                      const EGLint *attrib_list, uint64_t gbuffer_id);

EGLImage d_eglCreateImage(void *context, EGLDisplay dpy, EGLContext ctx,
                          EGLenum target, EGLClientBuffer buffer,
                          const EGLAttrib *attrib_list);

void r_eglDestroyImage(void *context, EGLDisplay dpy, uint64_t gbuffer_id);

EGLBoolean d_eglDestroyImage(void *context, EGLDisplay dpy, EGLImage image);

void r_eglWaitSync(void *context, EGLDisplay dpy, EGLSync sync, EGLint flags);

EGLBoolean d_eglWaitSync(void *context, EGLDisplay dpy, EGLSync sync,
                         EGLint flags);

EGLBoolean d_eglWaitSyncKHR(void *context, EGLDisplay dpy, EGLSyncKHR sync,
                            EGLint flags);

// EGLClientBuffer r_eglGetNativeClientBufferANDROID(void *context,
// AHardwareBuffer buffer);
//
// EGLClientBuffer d_eglGetNativeClientBufferANDROID(void *context,
// AHardwareBuffer buffer);

void r_eglSwapBuffers(void *context, const int64_t *swap_data);

EGLBoolean d_eglDestroySyncKHR(void *context, EGLDisplay dpy, EGLSyncKHR sync);

EGLint d_eglClientWaitSyncKHR(void *context, EGLDisplay dpy, EGLSyncKHR sync,
                              EGLint flags, EGLTimeKHR timeout);

EGLImageKHR d_eglCreateImageKHR(void *context, EGLDisplay dpy, EGLContext ctx,
                                EGLenum target, EGLClientBuffer buffer,
                                const EGLint *attrib_list);

EGLSyncKHR d_eglCreateSyncKHR(void *context, EGLDisplay dpy, EGLenum type,
                              const EGLAttrib *attrib_list);

EGLBoolean d_eglDestroyImageKHR(void *context, EGLDisplay dpy, EGLImageKHR img);

EGLBoolean d_eglGetSyncAttribKHR(void *context, EGLDisplay dpy, EGLSyncKHR sync,
                                 EGLint attribute, EGLint *value);

void d_eglQueueBuffer(void *context, EGLImage gbuffer_id, int is_composer);
void r_eglQueueBuffer(void *context, uint64_t gbuffer_id, int is_composer);

EGLint d_eglDupNativeFenceFDANDROID(void *context, EGLDisplay dpy,
                                    EGLSync sync);

void d_eglTP(void *context, char *buf, int len);
void r_eglTP(void *context, char *buf, int len);

void d_eglTerminateThread(void *context);
void r_eglTerminateThread(void *context);

void r_eglFenceWaitSync(void *context, int64_t fence_id);
void r_eglFenceSetSync(void *context, int64_t fence_id, int64_t reset_fence_id);
void r_eglGBufferDestroy(void *context, uint64_t gbuffer_id);

// int r_eglGetGBufferType(void *context, uint64_t gbuffer_id);

#endif
