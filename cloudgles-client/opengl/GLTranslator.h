#ifndef GL_TRANSLATOR_H
#define GL_TRANSLATOR_H

#include "renderer/MainRenderer.h"
#include "renderer/RenderThread.h"

#define MAX_OUT_BUF_LEN 4096

class GLTranslator {
   public:
    static void translate(ThreadContext *thread_context, CompressGLCall *call);
};

#endif  // GL_TRANSLATOR_H
