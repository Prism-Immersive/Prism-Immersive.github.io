#ifndef EXT_FUNC_H
#define EXT_FUNC_H

extern "C" {
typedef struct {
    const char *func_name;
    void *func_address;
} GLEXTFunc;

extern const GLEXTFunc gl_ext_func_table[];
extern const int GL_EXT_FUNC_TABLE_SIZE;
}

#endif  // EXT_FUNC_H
