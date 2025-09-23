#ifndef MAIN_CLIENT_H
#define MAIN_CLIENT_H

#include <pthread.h>
#include <sys/epoll.h>

#include <string>
#include <unordered_map>

#include "Network.h"

#define MAX_EVENTS 64

#define BUF_SIZE 4096

class MainClient {
   public:
    bool is_server_connected = false;
    bool client_should_running = false;
    static std::string ip;
    static int ports[5];

    MainClient();
    ~MainClient();

    void start_client(std::string server_ip = "127.0.0.1");
    void stop_server() const;
    void force_stop();

    static void send_control_packet(int client_fd, uint64_t server_tid,
                                    ControlMSG msg, int extra_data = 0);
    int try_connect();
    static int read_to_buf(int fd, char *buf, size_t len);
    static int write_buf_to(int fd, const char *buf, size_t len);

    void register_server_status_callback(void (*callback)(bool));
    void on_server_status_changed(bool server_status);

   private:
    pthread_t client_pthread{};

    int port = 0;
    int port_idx = 0;
    int client_controller_fd = 0;
    int epoll_fd = 0;
    struct epoll_event *events = nullptr;
    void (*server_status_callback)(bool) = nullptr;

    int create_epoll_events();
    static void *client_thread(void *data);
    static int non_blocking_socket(int sfd);
};

#endif  // MAIN_CLIENT_H
