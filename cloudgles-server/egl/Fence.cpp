#include "Fence.h"

#include <cstdint>

#include "EglDef.h"
#include "ThreadContext.h"

EglFence::EglFence() {
    engine = std::make_unique<std::mt19937>(rd());
    distribution = std::make_unique<std::uniform_int_distribution<uint64_t>>(
        0, UINT64_MAX);
    for (int i = 0; i < SYNC_FLAGS_SIZE; i++) {
        fence_flags[i].store(0);
    }
}

// Used when an object does not have a UUID
uint64_t EglFence::gen_key() {
    uint64_t key = 0;
    do {
        key = (*distribution)(*engine);
    } while (fence_key_set.find(key) != fence_key_set.end());

    SPIN_LOCK(fence_key_set_lock);
    fence_key_set.insert(key);
    SPIN_UNLOCK(fence_key_set_lock);
    return key;
}

void EglFence::wait_fence(int64_t fence_id) const {
    r_eglFenceWaitSync(egl_thread_context, fence_id);
}

int64_t EglFence::get_fence(uint64_t fence_key) {
    auto it = fence_map.find(fence_key);
    if (it == fence_map.end()) {
        return -1;
    }
    return it->second;
}

int64_t EglFence::reset_fence(uint64_t fence_key) {
    auto it = fence_map.find(fence_key);
    if (it == fence_map.end()) {
        return -1;
    }
    int64_t old_fence_id = it->second;
    RESET_FENCE_BIT(fence_flags, old_fence_id);
    return old_fence_id;
}

void EglFence::destroy_fence(uint64_t fence_key) {
    int64_t reset_fence_id = reset_fence(fence_key);
    if (reset_fence_id != -1) {
        SPIN_LOCK(fence_map_lock);
        fence_map.erase(fence_key);
        SPIN_UNLOCK(fence_map_lock);
    }

    SPIN_LOCK(fence_key_set_lock);
    fence_key_set.erase(fence_key);
    SPIN_UNLOCK(fence_key_set_lock);
}

int64_t EglFence::set_fence(uint64_t fence_key) {
    int64_t reset_fence_id = reset_fence(fence_key);
    int64_t id = cur_fence_id.load();

    do {
        id = cur_fence_id.fetch_add(1);
        id %= (SYNC_FLAGS_SIZE * 64);

        if (IS_FENCE_BIT_SET(fence_flags, id)) {
            continue;
        }

        uint64_t cur_gen_val = fence_flags[id / 64].load();

        fence_flags[id / 64].compare_exchange_weak(
            cur_gen_val, cur_gen_val | (1 << (id % 64)));
    } while (!IS_FENCE_BIT_SET(fence_flags, id));

    r_eglFenceSetSync(egl_thread_context, id, reset_fence_id);

    SPIN_LOCK(fence_map_lock);
    fence_map[fence_key] = id;
    SPIN_UNLOCK(fence_map_lock);

    return id;
}
