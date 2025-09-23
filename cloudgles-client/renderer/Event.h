#ifndef EVENT_H
#define EVENT_H

#ifndef _WIN32
#include <pthread.h>
typedef struct {
    int signal_state;  // 1 for event triggering, 0 for not
    int manual_reset;  // 0 for event auto reset after wait, 1 for manual
    pthread_mutex_t event_lock;
    pthread_cond_t event_cond;
} POSIXEvent;
#define POSIX_HANDLE POSIXEvent *
#define HANDLE_SIZE sizeof(POSIXEvent)
#endif

void *create_event(int manual_reset, int initial_state);

int wait_event(void *event, long milliseconds);

int wait_event(void *event);

int set_event(void *event);

int reset_event(void *event);

void delete_event(void *event);

#endif  // EVENT_H
