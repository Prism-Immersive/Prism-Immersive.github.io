#ifndef FLOWCONTROLLER_H
#define FLOWCONTROLLER_H

#include <cstdint>

#include "egl/Surface.h"

class ThreadContext;

class FlowController {
   public:
    uint64_t last_sleep_time = 0;

    static void decision(ThreadContext *context, EglSurface *surface,
                         int64_t cur_time);
    void decision_fixed(ThreadContext *context, EglSurface *surface,
                               int64_t cur_time);
    void decision_kingman(ThreadContext *context, EglSurface *surface,
                                    int64_t cur_time);
};

#endif  // FLOWCONTROLLER_H