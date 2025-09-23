#define LOG_TAG "FlowController"
#define LOG_LEVEL LOG_LEVEL_DEBUG
#include "FlowController.h"

#include <algorithm>
#include <cstdint>

#include "Log.h"
#include "ThreadContext.h"

void FlowController::decision(ThreadContext *context, EglSurface *surface,
                              int64_t cur_time) {
    surface->swap_time_write_cursor++;
    if (surface->swap_time_write_cursor == MAX_SWAP_DATA_NUM)
        surface->swap_time_write_cursor = 0;

    auto *server_swap_time_addr =
        reinterpret_cast<int64_t volatile *>(context->remote_mem->mem);
    auto *remote_avg_frame_duration_addr = reinterpret_cast<int64_t volatile *>(
        context->remote_mem->mem + sizeof(int64_t));

    while (surface->swap_time_read_cursor != surface->swap_time_write_cursor &&
           surface->swap_time_data[surface->swap_time_read_cursor] <=
               *server_swap_time_addr &&
           *server_swap_time_addr < cur_time) {
        surface->swap_time_read_cursor++;
        if (surface->swap_time_read_cursor == MAX_SWAP_DATA_NUM)
            surface->swap_time_read_cursor = 0;
    }

    // Need to account for the actual network latency
    int64_t frame_num_gap =
        (surface->swap_time_write_cursor + MAX_SWAP_DATA_NUM -
         surface->swap_time_read_cursor) %
        MAX_SWAP_DATA_NUM;

    int64_t server_swap_time = *server_swap_time_addr;
    int64_t remote_avg_frame_duration = *remote_avg_frame_duration_addr;

    ALOGD("cur_time %lld server_swap_time %lld remote_avg_frame_duration %lld",
          (long long)cur_time, (long long)server_swap_time,
          (long long)remote_avg_frame_duration);
    if (remote_avg_frame_duration == 0) {
        remote_avg_frame_duration = 1;
    }

    int swap_interval = context->swap_interval;

    int64_t server_frame_duration = cur_time - surface->frame_end_time;

    if (surface->frame_end_time == 0) {
        server_frame_duration = cur_time - surface->frame_server_start_time;
    }

    if (server_frame_duration < 0) {
        ALOGE(
            "error! server_frame_duration %lld cur_time %lld frame_end_time "
            "%lld",
            (long long)server_frame_duration, (long long)cur_time,
            (long long)surface->frame_end_time);
        server_frame_duration = 1000;
        frame_num_gap = 1;
    }

    int64_t should_sleep_time_of_gsync = 0;

    if (swap_interval != 0 && process_static_context_values->composer_HZ != 0) {
        int64_t ideal_gsync_time = 1000000 /
                                   process_static_context_values->composer_HZ /
                                   swap_interval;

        if (ideal_gsync_time > server_frame_duration) {
            should_sleep_time_of_gsync =
                ideal_gsync_time - server_frame_duration;
        }
    }

    // Need to account for the actual network latency
    int remote_transport_delay = 20;
    double ideal_frame_gap =
        static_cast<double>(surface->frame_client_start_time -
                            surface->frame_server_start_time +
                            remote_transport_delay) *
        1.0f / static_cast<double>(remote_avg_frame_duration);

    ideal_frame_gap = ceil(ideal_frame_gap);

    int64_t should_sleep_time_of_flow_control =
        (cur_time - surface->frame_server_start_time) -
        remote_avg_frame_duration;

    ALOGD(
        "should_sleep_time parameters: cur_time - "
        "surface->frame_server_start_time %lld, remote_avg_frame_duration "
        "%lld, ideal_frame_gap %f, frame_num_gap %lld",
        (long long)(cur_time - surface->frame_server_start_time),
        (long long)remote_avg_frame_duration, ideal_frame_gap,
        (long long)frame_num_gap);
    if (should_sleep_time_of_flow_control < 0) {
        should_sleep_time_of_flow_control = -should_sleep_time_of_flow_control;
        // MIMD
        should_sleep_time_of_flow_control =
            (int64_t)(static_cast<float>(should_sleep_time_of_flow_control *
                                         frame_num_gap) *
                      1.0f / ideal_frame_gap);
    } else {
        if (static_cast<double>(frame_num_gap) * 1.0f > ideal_frame_gap) {
            should_sleep_time_of_flow_control =
                (int64_t)((static_cast<double>(frame_num_gap) * 1.0f -
                           ideal_frame_gap - 0.5) *
                          static_cast<double>(remote_avg_frame_duration));
        } else {
            should_sleep_time_of_flow_control = 0;
        }
    }

    int64_t should_sleep_time =
        should_sleep_time_of_flow_control > should_sleep_time_of_gsync
            ? should_sleep_time_of_flow_control
            : should_sleep_time_of_gsync;
    should_sleep_time -= server_frame_duration;

    int64_t frame_no_sleep_end_time = cur_time;

    struct timeval end_t {};

    int sleep_cnt = 0;
    int change_cnt = 0;
    int64_t remain_sleep_time = should_sleep_time + surface->remain_sleep_time;
    int64_t real_sleep_time = 0;

    if (my_pid == composer_pid) {
        remain_sleep_time = 0;
        r_glSync(context);
    }

    if (surface->frame_end_time == 0) {
        remain_sleep_time = 0;
    }

    while (remain_sleep_time > 1000) {
        usleep(1000);

        gettimeofday(&end_t, nullptr);

        int64_t wake_time = (int64_t)end_t.tv_sec * 1000000 + end_t.tv_usec;

        remain_sleep_time -= wake_time - cur_time;
        real_sleep_time += wake_time - cur_time;
        ALOGD("guest has sleep %lld cnt %d cur_time %lld",
              (long long)wake_time - cur_time, sleep_cnt, (long long)wake_time);

        cur_time = wake_time;

        sleep_cnt += 1;

        int64_t new_server_swap_time;
        new_server_swap_time = *server_swap_time_addr;
        if (new_server_swap_time != server_swap_time) {
            change_cnt += 1;
            if (should_sleep_time_of_flow_control >
                should_sleep_time_of_gsync) {
                if (change_cnt == 1 && frame_num_gap == 1) {
                    remain_sleep_time = 0;
                    break;
                }
                if (change_cnt > 1) {
                    remain_sleep_time = 0;
                    break;
                }
            }
            server_swap_time = new_server_swap_time;
        }
    }

    surface->remain_sleep_time = remain_sleep_time;

    ALOGD(
        "gbuffer %llx remote average frame time %lld sleep time %lld/%lld "
        "remain %lld ideal gap %.2f current gap %lld ",
        (long long)surface->gbuffer_id, (long long)remote_avg_frame_duration,
        (long long)real_sleep_time, (long long)should_sleep_time,
        (long long)remain_sleep_time, ideal_frame_gap,
        (long long)frame_num_gap);

    ALOGD(
        "gbuffer %llx frame start time gap %lld frame no-sleep real time %lld "
        "gsync-sleep %lld flow-control-sleep %lld ideal %dHz ",
        (long long)surface->gbuffer_id,
        (long long)(surface->frame_client_start_time -
                    surface->frame_server_start_time),
        (long long)(frame_no_sleep_end_time - surface->frame_server_start_time),
        (long long)should_sleep_time_of_gsync,
        (long long)should_sleep_time_of_flow_control,
        swap_interval == 0
            ? 0
            : process_static_context_values->composer_HZ / swap_interval);
}

void FlowController::decision_fixed(ThreadContext *context, EglSurface *surface,
                                    int64_t cur_time) {
    int swap_interval = context->swap_interval;
    last_sleep_time = 0;
    if (swap_interval != 0 && process_static_context_values->composer_HZ != 0) {
        int64_t ideal_gsync_time = 1000000 /
                                   process_static_context_values->composer_HZ /
                                   swap_interval;

        int64_t server_frame_duration =
            cur_time - surface->frame_server_start_time;
        int64_t should_sleep_time = ideal_gsync_time - server_frame_duration +
                                    surface->remain_sleep_time;

        if (should_sleep_time <= 0) return;

        int64_t remain_sleep_time = should_sleep_time;
        struct timeval end_t {};
        while (remain_sleep_time > 1000) {
            usleep(1000);

            gettimeofday(&end_t, nullptr);
            int64_t wake_time = end_t.tv_sec * 1000000 + end_t.tv_usec;
            last_sleep_time += wake_time - cur_time;
            remain_sleep_time -= wake_time - cur_time;
            cur_time = wake_time;
        }

        surface->remain_sleep_time = remain_sleep_time;
    }
}

void FlowController::decision_kingman(ThreadContext *context,
                                      EglSurface *surface, int64_t cur_time) {
    int swap_interval = context->swap_interval;
    last_sleep_time = 0;
    if (process_static_context_values->composer_HZ != 0) {
        uint64_t ideal_sleep_time = 0;
        if (swap_interval != 0) {
            ideal_sleep_time = 1000000 /
                               120 /
                               swap_interval;
        }
        auto *kingman_sleep_time_addr =
            reinterpret_cast<uint64_t volatile *>(context->remote_mem->mem);
        uint64_t kingman_sleep_time = *kingman_sleep_time_addr;

        int64_t server_frame_duration =
            cur_time - surface->frame_server_start_time;

        int64_t should_sleep_time =
            std::max(ideal_sleep_time, kingman_sleep_time) -
            server_frame_duration + surface->remain_sleep_time;

        if (should_sleep_time <= 0) return;

        int64_t remain_sleep_time = should_sleep_time;
        struct timeval end_t {};
        while (remain_sleep_time > 1000) {
            usleep(1000);

            gettimeofday(&end_t, nullptr);
            int64_t wake_time = end_t.tv_sec * 1000000 + end_t.tv_usec;
            last_sleep_time += wake_time - cur_time;
            remain_sleep_time -= wake_time - cur_time;
            cur_time = wake_time;
        }

        surface->remain_sleep_time = remain_sleep_time;
        ALOGD(
            "Flow control has slept for %lld, should sleep for %lld, kingman "
            "sleep time is %lld, ideal "
            "sleep time is %lld, server frame duration is %lld, remain sleep "
            "time is %lld",
            (long long)last_sleep_time, (long long)should_sleep_time,
            (long long)kingman_sleep_time, (long long)ideal_sleep_time,
            (long long)server_frame_duration, (long long)remain_sleep_time);
    }
}
