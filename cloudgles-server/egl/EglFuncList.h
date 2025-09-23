#ifndef EGL_FUNC_LIST_H
#define EGL_FUNC_LIST_H

#define VOID_DEFAULT return
#define RET_DEFAULT return 0

#define LIST_IMPL_EGL_FUNCTIONS(X)                                             \
    X(EGLDisplay, eglGetDisplay, (EGLNativeDisplayType display_id),            \
      RET_DEFAULT, thread_ctx, display_id)                                     \
    X(EGLDisplay, eglGetPlatformDisplay,                                       \
      (EGLenum platform, void *native_display, const EGLAttrib *attrib_list),  \
      RET_DEFAULT, thread_ctx, platform, native_display, attrib_list)          \
    X(EGLBoolean, eglDestroyImageKHR, (EGLDisplay dpy, EGLImageKHR img),       \
      RET_DEFAULT, thread_ctx, dpy, img)                                       \
    X(EGLBoolean, eglGetSyncAttribKHR,                                         \
      (EGLDisplay dpy, EGLSyncKHR sync, EGLint attribute, EGLint * value),     \
      RET_DEFAULT, thread_ctx, dpy, sync, attribute, value)                    \
    X(EGLBoolean, eglInitialize,                                               \
      (EGLDisplay dpy, EGLint * major, EGLint * minor), RET_DEFAULT,           \
      thread_ctx, dpy, major, minor)                                           \
    X(__eglMustCastToProperFunctionPointerType, eglGetProcAddress,             \
      (const char *procname), RET_DEFAULT, thread_ctx, procname)               \
    X(EGLBoolean, eglQuerySurface,                                             \
      (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint * value),  \
      RET_DEFAULT, thread_ctx, dpy, surface, attribute, value)                 \
    X(EGLBoolean, eglReleaseThread, (void), RET_DEFAULT, thread_ctx)           \
    X(EGLBoolean, eglDestroySyncKHR, (EGLDisplay dpy, EGLSyncKHR sync),        \
      RET_DEFAULT, thread_ctx, dpy, sync)                                      \
    X(EGLBoolean, eglChooseConfig,                                             \
      (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs,          \
       EGLint config_size, EGLint *num_config),                                \
      RET_DEFAULT, thread_ctx, dpy, attrib_list, configs, config_size,         \
      num_config)                                                              \
    X(EGLSync, eglCreateSync,                                                  \
      (EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list),            \
      RET_DEFAULT, thread_ctx, dpy, type, attrib_list)                         \
    X(EGLSurface, eglCreatePbufferSurface,                                     \
      (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list),           \
      RET_DEFAULT, thread_ctx, dpy, config, attrib_list)                       \
    X(EGLenum, eglQueryAPI, (void), RET_DEFAULT, thread_ctx)                   \
    X(EGLDisplay, eglGetCurrentDisplay, (void), RET_DEFAULT, thread_ctx)       \
    X(const char *, eglQueryString, (EGLDisplay dpy, EGLint name),             \
      RET_DEFAULT, thread_ctx, dpy, name)                                      \
    X(EGLint, eglClientWaitSyncKHR,                                            \
      (EGLDisplay dpy, EGLSyncKHR sync, EGLint flags, EGLTimeKHR timeout),     \
      RET_DEFAULT, thread_ctx, dpy, sync, flags, timeout)                      \
    X(EGLBoolean, eglQueryContext,                                             \
      (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint * value),      \
      RET_DEFAULT, thread_ctx, dpy, ctx, attribute, value)                     \
    X(EGLBoolean, eglDestroyContext, (EGLDisplay dpy, EGLContext ctx),         \
      RET_DEFAULT, thread_ctx, dpy, ctx)                                       \
    X(EGLBoolean, eglWaitSync, (EGLDisplay dpy, EGLSync sync, EGLint flags),   \
      RET_DEFAULT, thread_ctx, dpy, sync, flags)                               \
    X(EGLBoolean, eglWaitSyncKHR,                                              \
      (EGLDisplay dpy, EGLSyncKHR sync, EGLint flags), RET_DEFAULT,            \
      thread_ctx, dpy, sync, flags)                                            \
    X(EGLBoolean, eglSwapBuffers, (EGLDisplay dpy, EGLSurface surface),        \
      RET_DEFAULT, thread_ctx, dpy, surface)                                   \
    X(EGLBoolean, eglSwapBuffersWithDamageKHR,                                 \
      (EGLDisplay dpy, EGLSurface draw, EGLint * rects, EGLint n_rects),       \
      RET_DEFAULT, thread_ctx, dpy, draw, rects, n_rects)                      \
    X(EGLBoolean, eglSetDamageRegionKHR,                                       \
      (EGLDisplay dpy, EGLSurface surface, EGLint * rects, EGLint n_rects),    \
      RET_DEFAULT, thread_ctx, dpy, surface, rects, n_rects)                   \
    X(EGLBoolean, eglWaitNative, (EGLint engine), RET_DEFAULT, thread_ctx,     \
      engine)                                                                  \
    X(EGLBoolean, eglWaitGL, (void), RET_DEFAULT, thread_ctx)                  \
    X(EGLSyncKHR, eglCreateSyncKHR,                                            \
      (EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list),            \
      RET_DEFAULT, thread_ctx, dpy, type, attrib_list)                         \
    X(EGLImage, eglCreateImage,                                                \
      (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, \
       const EGLAttrib *attrib_list),                                          \
      RET_DEFAULT, thread_ctx, dpy, ctx, target, buffer, attrib_list)          \
    X(EGLBoolean, eglGetConfigAttrib,                                          \
      (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint * value),    \
      RET_DEFAULT, thread_ctx, dpy, config, attribute, value)                  \
    X(EGLBoolean, eglBindAPI, (EGLenum api), RET_DEFAULT, thread_ctx, api)     \
    X(EGLBoolean, eglBindTexImage,                                             \
      (EGLDisplay dpy, EGLSurface surface, EGLint buffer), RET_DEFAULT,        \
      thread_ctx, dpy, surface, buffer)                                        \
    X(EGLContext, eglCreateContext,                                            \
      (EGLDisplay dpy, EGLConfig config, EGLContext share_context,             \
       const EGLint *attrib_list),                                             \
      RET_DEFAULT, thread_ctx, dpy, config, share_context, attrib_list)        \
    X(EGLBoolean, eglDestroyImage, (EGLDisplay dpy, EGLImage image),           \
      RET_DEFAULT, thread_ctx, dpy, image)                                     \
    X(EGLBoolean, eglWaitClient, (void), RET_DEFAULT, thread_ctx)              \
    X(EGLContext, eglGetCurrentContext, (void), RET_DEFAULT, thread_ctx)       \
    X(EGLBoolean, eglDestroySync, (EGLDisplay dpy, EGLSync sync), RET_DEFAULT, \
      thread_ctx, dpy, sync)                                                   \
    X(EGLImageKHR, eglCreateImageKHR,                                          \
      (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, \
       const EGLint *attrib_list),                                             \
      RET_DEFAULT, thread_ctx, dpy, ctx, target, buffer, attrib_list)          \
    X(EGLBoolean, eglMakeCurrent,                                              \
      (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx),      \
      RET_DEFAULT, thread_ctx, dpy, draw, read, ctx)                           \
    X(EGLint, eglGetError, (void), RET_DEFAULT, thread_ctx)                    \
    X(EGLBoolean, eglReleaseTexImage,                                          \
      (EGLDisplay dpy, EGLSurface surface, EGLint buffer), RET_DEFAULT,        \
      thread_ctx, dpy, surface, buffer)                                        \
    X(EGLSurface, eglGetCurrentSurface, (EGLint readdraw), RET_DEFAULT,        \
      thread_ctx, readdraw)                                                    \
    X(EGLBoolean, eglSurfaceAttrib,                                            \
      (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value),    \
      RET_DEFAULT, thread_ctx, dpy, surface, attribute, value)                 \
    X(EGLBoolean, eglGetConfigs,                                               \
      (EGLDisplay dpy, EGLConfig * configs, EGLint config_size,                \
       EGLint * num_config),                                                   \
      RET_DEFAULT, thread_ctx, dpy, configs, config_size, num_config)          \
    X(EGLBoolean, eglTerminate, (EGLDisplay dpy), RET_DEFAULT, thread_ctx,     \
      dpy)                                                                     \
    X(EGLBoolean, eglDestroySurface, (EGLDisplay dpy, EGLSurface surface),     \
      RET_DEFAULT, thread_ctx, dpy, surface)                                   \
    X(EGLBoolean, eglSwapInterval, (EGLDisplay dpy, EGLint interval),          \
      RET_DEFAULT, thread_ctx, dpy, interval)                                  \
    X(EGLint, eglClientWaitSync,                                               \
      (EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout),           \
      RET_DEFAULT, thread_ctx, dpy, sync, flags, timeout)                      \
    X(EGLBoolean, eglGetSyncAttrib,                                            \
      (EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib * value),     \
      RET_DEFAULT, thread_ctx, dpy, sync, attribute, value)                    \
    X(EGLSurface, eglCreateWindowSurface,                                      \
      (EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win,              \
       const EGLint *attrib_list),                                             \
      RET_DEFAULT, thread_ctx, dpy, config, win, attrib_list)                  \
    X(EGLint, eglDupNativeFenceFDANDROID, (EGLDisplay dpy, EGLSyncKHR sync),   \
      RET_DEFAULT, thread_ctx, dpy, sync)                                      \
    X(void, eglTP, (char *buf, int len), VOID_DEFAULT, thread_ctx, buf, len)   \
    X(void, eglTerminateThread, (void), VOID_DEFAULT, thread_ctx)

#define LIST_NOT_IMPL_EGL_FUNCTIONS(X)                                        \
    X(EGLStreamKHR, eglCreateStreamKHR,                                       \
      (EGLDisplay dpy, const EGLint *attrib_list), RET_DEFAULT, thread_ctx,   \
      dpy, attrib_list)                                                       \
    X(EGLSurface, eglCreatePlatformWindowSurface,                             \
      (EGLDisplay dpy, EGLConfig config, void *native_window,                 \
       const EGLAttrib *attrib_list),                                         \
      RET_DEFAULT, thread_ctx, dpy, config, native_window, attrib_list)       \
    X(EGLSurface, eglCreateStreamProducerSurfaceKHR,                          \
      (EGLDisplay dpy, EGLConfig config, EGLStreamKHR stream,                 \
       const EGLint *attrib_list),                                            \
      RET_DEFAULT, thread_ctx, dpy, config, stream, attrib_list)              \
    X(EGLSurface, eglCreatePlatformPixmapSurface,                             \
      (EGLDisplay dpy, EGLConfig config, void *native_pixmap,                 \
       const EGLAttrib *attrib_list),                                         \
      RET_DEFAULT, thread_ctx, dpy, config, native_pixmap, attrib_list)       \
    X(EGLStreamKHR, eglCreateStreamFromFileDescriptorKHR,                     \
      (EGLDisplay dpy, EGLNativeFileDescriptorKHR file_descriptor),           \
      RET_DEFAULT, thread_ctx, dpy, file_descriptor)                          \
    X(EGLBoolean, eglCopyBuffers,                                             \
      (EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target),       \
      RET_DEFAULT, thread_ctx, dpy, surface, target)                          \
    X(EGLBoolean, eglDestroyStreamKHR, (EGLDisplay dpy, EGLStreamKHR stream), \
      RET_DEFAULT, thread_ctx, dpy, stream)                                   \
    X(EGLSurface, eglCreatePixmapSurface,                                     \
      (EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap,          \
       const EGLint *attrib_list),                                            \
      RET_DEFAULT, thread_ctx, dpy, config, pixmap, attrib_list)              \
    X(EGLSurface, eglCreatePbufferFromClientBuffer,                           \
      (EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer,               \
       EGLConfig config, const EGLint *attrib_list),                          \
      RET_DEFAULT, thread_ctx, dpy, buftype, buffer, config, attrib_list)     \
    X(EGLBoolean, eglSignalSyncKHR,                                           \
      (EGLDisplay dpy, EGLSyncKHR sync, EGLenum mode), RET_DEFAULT,           \
      thread_ctx, dpy, sync, mode)                                            \
    X(EGLSurface, eglCreatePlatformWindowSurfaceEXT,                          \
      (EGLDisplay dpy, EGLConfig config, void *native_window,                 \
       const EGLint *attrib_list),                                            \
      RET_DEFAULT, thread_ctx, dpy, config, native_window, attrib_list)       \
    X(EGLSurface, eglCreatePlatformPixmapSurfaceEXT,                          \
      (EGLDisplay dpy, EGLConfig config, void *native_pixmap,                 \
       const EGLint *attrib_list),                                            \
      RET_DEFAULT, thread_ctx, dpy, config, native_pixmap, attrib_list)
// X(EGLDisplay, eglGetPlatformDisplay,
// (EGLenum platform, void
// *native_display, const EGLint
// *attrib_list), RET_DEFAULT,
// thread_ctx, platform, native_display,
// attrib_list)

#endif  // EGL_FUNC_LIST_H
