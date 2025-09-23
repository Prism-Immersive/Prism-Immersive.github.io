#ifndef THREAD_PACKET_POLLER_H
#define THREAD_PACKET_POLLER_H

#include "Compression.h"
#include "MainClient.h"

class ThreadPacketPoller {
   public:
    explicit ThreadPacketPoller(uint64_t tid);
    ~ThreadPacketPoller();

    void connect_server();

    void send_packet(DataPacketHeader *header, char *payload);
    void recv_packet(char *buf, size_t buf_len);
    void recv_packet(size_t buf_len);
    char *get_packet() { return call_buf; }

   private:
    int server_fd;
    uint64_t server_tid;
    MainClient *main_client;
    char *call_buf;
    size_t call_buf_len;
};

#endif  // THREAD_PACKET_POLLER_H
