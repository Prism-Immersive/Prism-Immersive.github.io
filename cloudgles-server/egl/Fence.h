#ifndef EGL_FENCE_H
#define EGL_FENCE_H

#include <stdint.h>

#include <atomic>
#include <cstdint>
#include <memory>
#include <random>
#include <unordered_map>
#include <unordered_set>

#define SYNC_FLAGS_SIZE 256

#define IS_FENCE_BIT_SET(flags, id) ((flags)[(id) / 64].load() & (((uint64_t)1) << ((id) % 64)))

#define RESET_FENCE_BIT(flags, id)                       \
    do {                                                 \
        uint64_t _old_flag = (flags)[(id) / 64];         \
        (flags)[(id) / 64].compare_exchange_weak(        \
            _old_flag, _old_flag & ~(((uint64_t)1) << ((id) % 64))); \
    } while (IS_FENCE_BIT_SET((flags), (id)))

class EglFence {
   public:
    EglFence();
    uint64_t gen_key();
    int64_t set_fence(uint64_t fence_key);
    void wait_fence(int64_t fence_id) const;
    int64_t get_fence(uint64_t fence_key);
    void destroy_fence(uint64_t fence_key);

   private:
    int64_t reset_fence(uint64_t fence_key);
    std::random_device rd;
    std::unique_ptr<std::mt19937> engine;
    std::unique_ptr<std::uniform_int_distribution<uint64_t>> distribution;

    std::atomic_uint64_t fence_flags[SYNC_FLAGS_SIZE];
    std::atomic_int64_t cur_fence_id;
    std::unordered_set<uint64_t> fence_key_set;
    std::atomic_flag fence_key_set_lock = ATOMIC_FLAG_INIT;
    std::unordered_map<uint64_t, int64_t> fence_map;
    std::atomic_flag fence_map_lock = ATOMIC_FLAG_INIT;
};

#endif  // EGL_FENCE_H