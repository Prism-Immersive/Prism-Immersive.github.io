#define LOG_TAG "ServiceProxy"

#include "ServiceProxy.h"

#include "Log.h"
#include "MainClient.h"

void ServiceProxy::connect(int port) {
    if (is_connected) return;

    if (port == 0) {
        port = ports[port_idx];
    }

    try {
        auto ip = boost::asio::ip::address::from_string(MainClient::ip);
        service_socket.connect(tcp::endpoint(ip, port));
        boost::asio::write(service_socket,
                           boost::asio::buffer(&type, sizeof(ServiceType)));
        is_connected = true;
    } catch (std::exception &e) {
        ALOGE("client cannot connect to %s:%d, %d, %s",
              MainClient::ip.c_str(), ports[port_idx], errno,
              strerror(errno));
    }
}
