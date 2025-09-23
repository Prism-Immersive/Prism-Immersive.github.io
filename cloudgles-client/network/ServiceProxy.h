#ifndef SERVICE_PROXY_H
#define SERVICE_PROXY_H

#include <boost/asio.hpp>

#include "CloudServices/ServiceType.h"

using boost::asio::ip::tcp;

class ServiceProxy {
   public:
    virtual ~ServiceProxy() = default;
    virtual void start(std::string ip) = 0;
    virtual void stop() = 0;

   protected:
    ServiceType type;
    bool is_connected = false;

    unsigned short ports[5] = {8880, 8881, 8882, 8883, 8884};
    int port_idx = 0;

    boost::asio::io_context io_context;
    tcp::socket service_socket = tcp::socket(io_context);

    void connect(int port);
};

#endif  // SERVICE_PROXY_H
