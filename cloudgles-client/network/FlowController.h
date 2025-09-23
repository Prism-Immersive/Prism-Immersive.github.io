#ifndef FLOW_CONTROLLER_H
#define FLOW_CONTROLLER_H

#include <cstdint>

#define EWMA_ALPHA 0.3
#define EXPECTED_QUEUE_LATENCY 5

class ThreadContext;

class FlowController {
   public:
    FlowController() = default;
    ~FlowController() = default;

    void begin_service();
    void end_service();
    void end_frame_service();
    void set_inter_arrival_time(uint64_t inter_arrival_time);

    void decision(ThreadContext *context);

   private:
    uint64_t service_begin_ts = 0;
    uint64_t total_service_time = 0;

    double mean_service_time = 0;
    double service_time_variance = 0;
    double mean_inter_arrival_time = 0;
    double inter_arrival_time_variance = 0;

    uint64_t get_current_time();
};

#endif  // FLOW_CONTROLLER_H
