#define LOG_TAG "FlowController"
#define LOG_LEVEL LOG_LEVEL_DEBUG
#include "FlowController.h"

#include <renderer/RenderThread.h>

#include <cassert>
#include <cmath>

#include "Log.h"

void FlowController::begin_service() { service_begin_ts = get_current_time(); }

void FlowController::end_service() {
    total_service_time += get_current_time() - service_begin_ts;
}

void FlowController::end_frame_service() {
    double service_time = total_service_time / 1000.;
    total_service_time = 0;
    assert(service_time >= 0);

    if (mean_service_time == 0) {
        mean_service_time = service_time;
        return;
    }

    mean_service_time =
        (1 - EWMA_ALPHA) * mean_service_time + EWMA_ALPHA * service_time;
    int64_t variance = service_time - mean_service_time;
    service_time_variance = (1 - EWMA_ALPHA) * pow(service_time_variance, 2) +
                            EWMA_ALPHA * pow(variance, 2);
    service_time_variance = sqrt(service_time_variance);
    ALOGD("Service time: %f, mean: %f, variance: %f", service_time,
          mean_service_time, service_time_variance);
}

void FlowController::set_inter_arrival_time(uint64_t inter_arrival_time) {
    double inter_arrival_time_ms = inter_arrival_time / 1000.;
    assert(inter_arrival_time >= 0);

    if (mean_inter_arrival_time == 0) {
        mean_inter_arrival_time = inter_arrival_time_ms;
        return;
    }

    mean_inter_arrival_time = (1 - EWMA_ALPHA) * mean_inter_arrival_time +
                              EWMA_ALPHA * inter_arrival_time_ms;
    int64_t variance = inter_arrival_time_ms - mean_inter_arrival_time;
    inter_arrival_time_variance =
        (1 - EWMA_ALPHA) * pow(inter_arrival_time_variance, 2) +
        EWMA_ALPHA * pow(variance, 2);
    inter_arrival_time_variance = sqrt(inter_arrival_time_variance);
    ALOGD("Inter arrival time: %f, mean: %f, variance: %f",
          inter_arrival_time_ms, mean_inter_arrival_time,
          inter_arrival_time_variance);
}

void FlowController::decision(ThreadContext *context) {
    double queue_utilization = mean_service_time / mean_inter_arrival_time;
    if (queue_utilization >= 1) return;

    double coefficient_service = service_time_variance / mean_service_time;
    double coefficient_inter_arrival =
        inter_arrival_time_variance / mean_inter_arrival_time;

    double expected_frame_time =
        ((pow(coefficient_service, 2) + pow(coefficient_inter_arrival, 2)) /
             (2 * EXPECTED_QUEUE_LATENCY) +
         1) *
        mean_service_time;

    uint64_t expected_frame_time_us = expected_frame_time * 1000;
    if (context->remote_mem != 0) {
        context->remote_mem_writer->write_mem(
            context->remote_mem,
            reinterpret_cast<char *>(&expected_frame_time_us),
            sizeof(uint64_t));
        context->remote_mem = 0;
    }

    ALOGD("Expected frame time: %f", expected_frame_time);
}

uint64_t FlowController::get_current_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
}
