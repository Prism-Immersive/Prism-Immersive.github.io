#define LOG_TAG "InputServiceProxy"

#include "InputServiceProxy.h"
#include "network/MainClient.h"

#include "Log.h"

InputServiceProxy::InputServiceProxy() { type = SERVICE_TYPE_INPUT; }

InputServiceProxy::~InputServiceProxy() {
    proxy_should_run = false;
    pthread_cond_signal(&proxy_cond);
    pthread_join(proxy_pthread, nullptr);
}

void InputServiceProxy::stop() {
    if (!proxy_should_run) return;
    proxy_should_run = false;
    pthread_cond_signal(&proxy_cond);
    pthread_join(proxy_pthread, nullptr);
    service_socket.close();
}

void InputServiceProxy::start(std::string server_ip) {
    auto pos = server_ip.find(':');
    if (pos != std::string::npos) {
        port = std::stoi(server_ip.substr(pos + 1));
        port = port - MainClient::ports[0] + ports[0];
    }
    pthread_create(&proxy_pthread, nullptr, proxy_thread, this);
}

void InputServiceProxy::inject_touch_events(short action, short x, short y) {
    CloudInputEvent event{.action = action, .x = x, .y = y};
    input_event_queue.push(event);
    ALOGD("inject_touch_events %d, %d, %d", action, x, y);
    pthread_cond_signal(&proxy_cond);
}

void *InputServiceProxy::proxy_thread(void *proxy) {
    auto *service_proxy = static_cast<InputServiceProxy *>(proxy);
    if (!service_proxy->is_connected) {
        service_proxy->connect(service_proxy->port);
    }
    if (!service_proxy->is_connected) {
        return nullptr;
    }

    CloudInputEvent event;
    while (service_proxy->proxy_should_run) {
        while (service_proxy->input_event_queue.pop(event)) {
            ALOGE("Write input event action: %d, x: %d, y: %d", event.action,
                  event.x, event.y);
            boost::asio::write(service_proxy->service_socket,
                               boost::asio::buffer(&event, sizeof(event)));
        }

        pthread_mutex_lock(&service_proxy->proxy_mutex);
        pthread_cond_wait(&service_proxy->proxy_cond,
                          &service_proxy->proxy_mutex);
        pthread_mutex_unlock(&service_proxy->proxy_mutex);
    }

    return nullptr;
}
