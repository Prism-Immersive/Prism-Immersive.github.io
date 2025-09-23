#include "Event.h"

#include <cstdlib>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else

#include <cerrno>
#include <ctime>

#endif

/**
 * @brief Create an event
 *
 * @param manual_reset Whether you need to manually reset the state after wait
 * @param initial_state The initial state of event
 * @return void* Event handle
 */
void *create_event(int manual_reset, int initial_state) {
#ifdef _WIN32
    HANDLE event = CreateEvent(NULL, manual_reset, initial_state, NULL);
#else
    POSIX_HANDLE event = (POSIX_HANDLE)malloc(HANDLE_SIZE);
    if (event == nullptr) return nullptr;
    memset(event, 0, HANDLE_SIZE);
    event->manual_reset = manual_reset;
    event->signal_state = initial_state;
    if (pthread_mutex_init(&event->event_lock, nullptr)) {
        free(event);
        return nullptr;
    }
    if (pthread_cond_init(&event->event_cond, nullptr)) {
        free(event);
        return nullptr;
    }
#endif
    return event;
}

/**
 * @brief Blocking on an event with timeout
 *
 * @param event Event handle
 * @param milliseconds Timeout value
 * @return int 0 for success, 1 for timeout, -1 for error
 */
int wait_event(void *event, long milliseconds) {
#ifdef _WIN32
    DWORD ret = WaitForSingleObject(event, milliseconds);
    if (ret == WAIT_OBJECT_0)
        return 0;
    else if (ret == WAIT_TIMEOUT)
        return 1;
    return -1;
#else
    POSIX_HANDLE pevent = (POSIX_HANDLE)event;
    struct timespec target_time {};
    struct timeval current_time {};
    int timeout = 0;
    gettimeofday(&current_time, nullptr);
    target_time.tv_sec = current_time.tv_sec + milliseconds / 1000;
    target_time.tv_nsec =
        current_time.tv_usec * 1000 + (milliseconds % 1000) * 1000000;
    if (target_time.tv_nsec >= 1000000000) {
        target_time.tv_nsec -= 1000000000;
        target_time.tv_sec++;
    }

    if (pthread_mutex_lock(&pevent->event_lock)) return -1;
    while (!pevent->signal_state) {
        if ((timeout = pthread_cond_timedwait(
                 &pevent->event_cond, &pevent->event_lock, &target_time))) {
            if (timeout == ETIMEDOUT) break;
            pthread_mutex_unlock(&pevent->event_lock);
            return -1;
        }
    }

    if (!timeout && !pevent->manual_reset) pevent->signal_state = 0;
    if (pthread_mutex_unlock(&pevent->event_lock)) return -1;
    if (timeout == ETIMEDOUT) return 1;
    return 0;
#endif
}

/**
 * @brief Blocking on an event without timeout
 *
 * @param event Event handle
 * @return int 0 for success, -1 for error
 */
int wait_event(void *event) {
#ifdef _WIN32
    DWORD ret = WaitForSingleObject(event, 100000);
    if (ret == WAIT_OBJECT_0)
        return 0;
    else if (ret == WAIT_TIMEOUT)
        return -1;
    return -1;
#else
    POSIX_HANDLE pevent = (POSIX_HANDLE)event;

    if (pthread_mutex_lock(&pevent->event_lock)) return -1;
    while (!pevent->signal_state) {
        if ((pthread_cond_wait(&pevent->event_cond, &pevent->event_lock))) {
            pthread_mutex_unlock(&pevent->event_lock);
            return -1;
        }
    }

    if (!pevent->manual_reset) pevent->signal_state = 0;
    if (pthread_mutex_unlock(&pevent->event_lock)) return -1;
    return 0;
#endif
}

/**
 * @brief Trigger the event
 *
 * @param event Event handle
 * @return int 0 for success, -1 for error
 */
int set_event(void *event) {
#ifdef _WIN32
    return SetEvent(event) ? 0 : -1;
#else
    POSIX_HANDLE pevent = (POSIX_HANDLE)event;
    if (pthread_mutex_lock(&pevent->event_lock)) {
        return -1;
    }
    pevent->signal_state = 1;
    if (pevent->manual_reset) {
        if (pthread_cond_broadcast(&pevent->event_cond)) {
            return -1;
        }
    } else {
        if (pthread_cond_signal(&pevent->event_cond)) {
            return -1;
        }
    }

    if (pthread_mutex_unlock(&pevent->event_lock)) {
        return -1;
    }
    return 0;
#endif
}

/**
 * Reset an event's state
 * @param event Event handle
 * @return 0 for success, -1 for error
 */
int reset_event(void *event) {
#ifdef _WIN32
    if (ResetEvent(event)) {
        return 0;
    }
    return -1;
#else
    POSIX_HANDLE pevent = (POSIX_HANDLE)event;
    if (pthread_mutex_lock(&pevent->event_lock) != 0) {
        return -1;
    }
    pevent->signal_state = 0;
    if (pthread_mutex_unlock(&pevent->event_lock) != 0) {
        return -1;
    }
    return 0;
#endif
}

/**
 * @brief Delete an event
 *
 * @param event Event handle
 */
void delete_event(void *event) {
#ifdef _WIN32
    CloseHandle(event);
#else
    POSIX_HANDLE pevent = (POSIX_HANDLE)event;
    pthread_mutex_destroy(&pevent->event_lock);
    pthread_cond_destroy(&pevent->event_cond);
    free(pevent);
#endif
}
