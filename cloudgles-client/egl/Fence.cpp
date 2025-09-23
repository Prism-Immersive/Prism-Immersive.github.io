#include "Fence.h"

#include <memory>

#include "Utils.h"
#include "renderer/MainRenderer.h"

EglFence::EglFence() {
    event = create_event(0, 0);
    event_wait_cnt = 0;
    for (int i = 0; i < SYNC_FLAGS_SIZE; i++) {
        fence_flags[i].store(0);
    }
}

EglFence::~EglFence() { delete_event(event); }

void EglFence::set_fence(int64_t fence_id, int64_t reset_fence_id) {
    if (reset_fence_id != -1) {
        RESET_FENCE_BIT(fence_flags, reset_fence_id);
    }

    SET_FENCE_BIT(fence_flags, fence_id);
    if (event_wait_cnt.exchange(0) > 0) {
        set_event(event);
    }
}

void EglFence::wait_fence(int64_t fence_id) {
    while (!IS_FENCE_BIT_SET(fence_flags, fence_id)) {
        event_wait_cnt++;
        wait_event(event, 1);
    }
}
