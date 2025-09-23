#define LOG_TAG "ThreadPacketPoller"

#include "ThreadPacketPoller.h"

#include <sys/select.h>
#include <sys/uio.h>
#include <unistd.h>

#include <cerrno>

#include "Log.h"
#include "Utils.h"
#include "renderer/MainController.h"

ThreadPacketPoller::ThreadPacketPoller(uint64_t tid) {
    server_tid = tid;
    server_fd = -1;
    main_client = MainController::get_instance()->main_client;
    call_buf_len = 4096;
    call_buf = reinterpret_cast<char *>(malloc(call_buf_len));
    connect_server();
}

void ThreadPacketPoller::send_packet(DataPacketHeader *header, char *payload) {
    struct iovec iov[2];
    iov[0].iov_base = header;
    iov[0].iov_len = sizeof(DataPacketHeader);
    iov[1].iov_base = payload;
    iov[1].iov_len = header->payload_len;

    int len;
    size_t total_len = sizeof(DataPacketHeader) + header->payload_len;
    do {
        len = writev(this->server_fd, iov, 2);
        if (len < total_len) {
            connect_server();
            len = -1;
        }
    } while (len == -1 && errno == EINTR );

    if (len == -1 && errno != EINTR) {
        ALOGE("Failed to send packet, error is %d: %s", errno, strerror(errno));
        main_client->on_server_status_changed(false);
    }
}

void ThreadPacketPoller::recv_packet(char *buf, size_t buf_len) {
    int len;
    do {
        len = MainClient::read_to_buf(server_fd, buf, buf_len);
        if (len < 0) {
            ALOGD("read packet error, %d, %s", errno, strerror(errno));
        }
    } while (len == -1);
}

void ThreadPacketPoller::recv_packet(size_t buf_len) {
    if (call_buf_len < buf_len) {
        size_t len = ALIGN(buf_len / 2 * 3);
        void *temp = realloc(call_buf, len);
        if (temp) {
            call_buf = reinterpret_cast<char *>(temp);
            call_buf_len = len;
        } else {
            ALOGE("server failed to realloc call_buf");
            return;
        }
    }
    memset(call_buf, 0, buf_len);
    recv_packet(call_buf, buf_len);
}

void ThreadPacketPoller::connect_server() {
    if (server_fd != -1) close(server_fd);
    server_fd = FAILURE_RETRY(
        MainController::get_instance()->main_client->try_connect());
    MainClient::send_control_packet(server_fd, server_tid, MSG_CREATE_THREAD);
}

ThreadPacketPoller::~ThreadPacketPoller() {
    if (server_fd > 0) close(server_fd);
    if (call_buf) free(call_buf);
}
