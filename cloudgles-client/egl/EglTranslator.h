#ifndef EGL_TRANSLATOR_H
#define EGL_TRANSLATOR_H

#include "renderer/MainController.h"

#define MAX_OUT_BUF_LEN 4096

class EglTranslator {
   public:
    static void translate(ThreadContext *thread_context, CompressGLCall *call);
};

#endif  // EGL_TRANSLATOR_H
