#ifndef THREAD_PACKET_STREAMER_H
#define THREAD_PACKET_STREAMER_H

#include <sys/types.h>
#include <sys/uio.h>

#include <boost/asio.hpp>
#include <memory>

#include "Compression.h"

using boost::asio::ip::tcp;

class ThreadPacketStreamer {
    tcp::socket *client_socket = nullptr;
    pid_t tid = 0;

    void set_socket();

   public:
    ThreadPacketStreamer();
    ~ThreadPacketStreamer();

    void send_packet(DataPacketHeader *header, const char *payload,
                     struct iovec *extra_data = nullptr,
                     int extra_data_num = 0);

    void send_packet_async(DataPacketHeader *header, const char *payload,
                           void *callback = nullptr);

    void recv_packet(char *buf, size_t buf_len);
};

#endif  // THREAD_PACKET_STREAMER_H
