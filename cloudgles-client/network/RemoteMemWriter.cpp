#define LOG_TAG "RemoteMemWriter"

#include "RemoteMemWriter.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

#include <cerrno>

#include "Log.h"
#include "MainClient.h"
#include "Utils.h"
#include "renderer/MainController.h"

RemoteMemWriter::RemoteMemWriter(uint64_t tid) {
    server_tid = tid;
    server_fd = -1;
    connect_server();
}

RemoteMemWriter::~RemoteMemWriter() {
    if (server_fd > 0) close(server_fd);
}

void RemoteMemWriter::connect_server() {
    if (server_fd != -1) close(server_fd);
    server_fd = FAILURE_RETRY(
        MainController::get_instance()->main_client->try_connect());
    if (server_fd > 0) {
        int val = 1;
        if (setsockopt(server_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val)) ==
            -1) {
            ALOGE("Failed to disable the TCP Nagle algorithm in client");
        }
    }
    MainClient::send_control_packet(server_fd, server_tid,
                                    MSG_CREATE_MEM_MAPPER);
}

void RemoteMemWriter::write_mem(uint64_t addr, char *data, uint32_t data_len) {
    RemoteMemHeader header{.mem_addr = addr, .size = data_len};
    struct iovec iov[2];
    iov[0].iov_base = &header;
    iov[0].iov_len = sizeof(RemoteMemHeader);
    iov[1].iov_base = data;
    iov[1].iov_len = data_len;

    int len;
    size_t total_len = sizeof(RemoteMemHeader) + data_len;
    do {
        len = writev(this->server_fd, iov, 2);
        int val = 1;
        setsockopt(server_fd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
        if (len < total_len) {
            connect_server();
            len = -1;
        }
    } while (len == -1);
}
