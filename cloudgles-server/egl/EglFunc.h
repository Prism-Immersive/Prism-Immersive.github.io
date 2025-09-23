#ifndef EGL_FUNC_H
#define EGL_FUNC_H
extern "C" {
typedef struct {
    const char *func_name;
    void *func_address;
} Egl_Func;

extern const Egl_Func egl_func_table[];
extern const int EGL_FUNC_TABLE_SIZE;
}

#endif  // EGL_FUNC_H
