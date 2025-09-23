#ifndef EGL_FENCE_H
#define EGL_FENCE_H

#include <stdint.h>

#include <atomic>

#include "renderer/Event.h"

#define SYNC_FLAGS_SIZE 256

#define IS_FENCE_BIT_SET(flags, id) ((flags)[(id) / 64] & (((uint64_t)1) << (((uint64_t)id) % 64)))

#define SET_FENCE_BIT(flags, id)                        \
    do {                                                \
        uint64_t _old_flag = (flags)[(id) / 64];        \
        (flags)[(id) / 64].compare_exchange_weak(       \
            _old_flag, _old_flag | (((uint64_t)1) << (((uint64_t)id) % 64))); \
    } while (!IS_FENCE_BIT_SET((flags), (id)))

#define RESET_FENCE_BIT(flags, id)                       \
    do {                                                 \
        uint64_t _old_flag = (flags)[(id) / 64];         \
        (flags)[(id) / 64].compare_exchange_weak(        \
            _old_flag, _old_flag & ~(((uint64_t)1) << (((uint64_t)id) % 64))); \
    } while (IS_FENCE_BIT_SET((flags), (id)))

class EglFence {
   public:
    void set_fence(int64_t fence_id, int64_t reset_fence_id);
    void wait_fence(int64_t fence_id);

    EglFence();
    ~EglFence();

   private:
    std::atomic_uint64_t fence_flags[SYNC_FLAGS_SIZE];
    void *event;
    std::atomic_int64_t event_wait_cnt;
};

#endif  // EGL_FENCE_H
