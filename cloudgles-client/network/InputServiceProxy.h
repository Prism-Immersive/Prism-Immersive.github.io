#ifndef INPUT_SERVICE_PROXY_H
#define INPUT_SERVICE_PROXY_H

#include <pthread.h>

#include <boost/lockfree/queue.hpp>

#include "CloudServices/IInputService.h"
#include "Network.h"
#include "ServiceProxy.h"

using namespace android;

class InputServiceProxy : public IInputService, public ServiceProxy {
   public:
    InputServiceProxy();
    ~InputServiceProxy() override;

    virtual void start(std::string ip) override;
    virtual void stop() override;

    // Input control
    void inject_touch_events(short action, short x, short y) override;

   private:
    int port = 0;
    bool proxy_should_run = true;
    pthread_t proxy_pthread{};
    pthread_cond_t proxy_cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t proxy_mutex = PTHREAD_MUTEX_INITIALIZER;
    boost::lockfree::queue<CloudInputEvent> input_event_queue{128};

    static void *proxy_thread(void *proxy);
};

#endif  // INPUT_SERVICE_PROXY_H
