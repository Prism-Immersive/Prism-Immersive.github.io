#define ATRACE_TAG ATRACE_TAG_GRAPHICS
#define LOG_TAG "ThreadPacketStreamer"
#include "ThreadPacketStreamer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

#include <boost/asio/buffer.hpp>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "Log.h"
#include "ThreadContext.h"
#include "network/MainServer.h"

ThreadPacketStreamer::ThreadPacketStreamer() {
    tid = gettid();
    client_socket = main_server->get_thread_socket(gettid(), MSG_CREATE_THREAD);
    set_socket();
}

void ThreadPacketStreamer::set_socket() {
    if (client_socket == nullptr) return;

    int native_handle = client_socket->native_handle();
    int val = 1;
    if (setsockopt(native_handle, SOL_TCP, TCP_NODELAY, &val, sizeof(val)) ==
        -1) {
        ALOGE("Failed to disable the TCP Nagle algorithm in server");
    }
    // set socket buffer to 4MB
    int buf_size = 4 * 1024 * 1024;
    if (setsockopt(native_handle, SOL_SOCKET, SO_SNDBUF, &buf_size,
                   sizeof(buf_size)) == -1) {
        ALOGE("Failed to set socket buffer size in server");
    }
}

void ThreadPacketStreamer::send_packet(DataPacketHeader *header,
                                       const char *payload,
                                       struct iovec *extra_data,
                                       int extra_data_num) {
    ATRACE_CALL();
    do {
        client_socket = main_server->get_thread_socket(tid, MSG_CREATE_THREAD);
    } while (client_socket == nullptr);
    set_socket();

    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(header, sizeof(DataPacketHeader)));
    buffers.push_back(boost::asio::buffer(payload, header->payload_len));

    for (int i = 0; i < extra_data_num; i++) {
        header->payload_len += extra_data[i].iov_len;
        buffers.push_back(
            boost::asio::buffer(extra_data[i].iov_base, extra_data[i].iov_len));
    }

    try {
        boost::asio::write(*client_socket, buffers);
    } catch (boost::system::system_error &e) {
        ALOGE("Failed to write to client, %d, %s", e.code().value(), e.what());
        main_server->on_client_status_changed(-1);
    }
}

void ThreadPacketStreamer::send_packet_async(DataPacketHeader *header,
                                             const char *payload,
                                             void *callback) {
    ATRACE_CALL();
    do {
        client_socket = main_server->get_thread_socket(tid, MSG_CREATE_THREAD);
    } while (client_socket == nullptr);
    set_socket();

    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(header, sizeof(DataPacketHeader)));
    buffers.push_back(boost::asio::buffer(payload, header->payload_len));

    boost::asio::async_write(
        *client_socket, buffers,
        [callback](boost::system::error_code ec, std::size_t length) {
            (void)length;
            if (ec) {
                ALOGE("Failed to write to client, %s", __FUNCTION__);
                main_server->on_client_status_changed(-1);
                return;
            }
            if (callback) {
                ((void (*)(void))callback)();
            }
        });
}

void ThreadPacketStreamer::recv_packet(char *buf, size_t buf_len) {
    do {
        client_socket = main_server->get_thread_socket(tid, MSG_CREATE_THREAD);
    } while (client_socket == nullptr);
    set_socket();

    DataPacketHeader header;
    int read_len = MainServer::read_to_buf(client_socket, (char *)&header,
                                           sizeof(DataPacketHeader));
    if (read_len < 0) return;

    // Read payload to buffer
    if (header.payload_len != buf_len) {
        ALOGE("Payload length %d does not match buffer length %zu, %s",
              header.payload_len, buf_len, __FUNCTION__);
    }
    MainServer::read_to_buf(
        client_socket, buf,
        std::min(header.payload_len, (unsigned int)buf_len));
}

ThreadPacketStreamer::~ThreadPacketStreamer() {
    if (client_socket != nullptr) {
        try {
            client_socket->close();
        } catch (boost::system::system_error &e) {
            ALOGE("Failed to close client socket, %d, %s", e.code().value(),
                  e.what());
        }
    }
}
