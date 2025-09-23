#define LOG_TAG "MainClient"

#include "network/MainClient.h"

#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <utility>

#include "Log.h"
#include "Utils.h"
#include "renderer/MainController.h"

std::string MainClient::ip = "127.0.0.1";
int MainClient::ports[5] = {8890, 8891, 8892, 8893, 8894};

MainClient::MainClient() = default;

MainClient::~MainClient() {
    force_stop();
    delete events;
}

void MainClient::start_client(std::string server_ip) {
    // Split IP string into IP and port
    auto pos = server_ip.find(':');
    if (pos != std::string::npos) {
        ip = server_ip.substr(0, pos);
        port = std::stoi(server_ip.substr(pos + 1));
    } else {
        ip = std::move(server_ip);
    }
    pthread_create(&client_pthread, nullptr, client_thread, this);
    pthread_setname_np(client_pthread, "main_client");
}

void MainClient::force_stop() {
    if (!client_should_running) return;
    client_should_running = false;
    pthread_join(client_pthread, nullptr);
    on_server_status_changed(false);
}

void MainClient::send_control_packet(int client_fd, uint64_t server_tid,
                                     ControlMSG msg, int extra_data) {
    ControlPacket packet = {
        .thread_id = server_tid, .msg = msg, .extra_data = extra_data};
    write_buf_to(client_fd, (char *)&packet, sizeof(ControlPacket));
}

int MainClient::try_connect() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        ALOGE("client cannot create socket, %d, %s", errno, strerror(errno));
        return -1;
    }

    struct sockaddr_in server_addr {};

    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    int num_ports = sizeof(ports) / sizeof(int);
    int port_num = ports[port_idx];
    if (port != 0) {
        port_num = port;
    }
    server_addr.sin_port = htons(port_num);
    if (connect(fd, (struct sockaddr *)&server_addr,
                sizeof(struct sockaddr_in)) < 0) {
        ALOGE("client cannot connect to %s:%d, %d, %s", ip.c_str(),
              port_num, errno, strerror(errno));
        close(fd);
        fd = -1;
    }

    if (fd == -1) {
        port_idx++;
        port_idx %= num_ports;
    }

    return fd;
}

void *MainClient::client_thread(void *data) {
    auto *client = reinterpret_cast<MainClient *>(data);
    ControlPacket control_packet;

    client->client_controller_fd = FAILURE_RETRY_SLEEP(client->try_connect());
    FAILURE_RETRY(client->create_epoll_events());
    send_control_packet(client->client_controller_fd, (uint64_t)gettid(),
                        MSG_MAIN_CLIENT_CONNECT);

    while (client->client_should_running) {
        int num_events, i;
        num_events =
            epoll_wait(client->epoll_fd, client->events, MAX_EVENTS, 1);
        for (i = 0; i < num_events; i++) {
            if (!(client->events[i].events & EPOLLIN))
                continue;
            else if (client->client_controller_fd ==
                     client->events[i].data.fd) {
                read_to_buf(client->client_controller_fd,
                            (char *)&control_packet, sizeof(ControlPacket));
                auto tid = (int)(control_packet.thread_id & 0x000000FFFFFFFF);
                auto pid =
                    (int)((control_packet.thread_id & 0xFFFFFF00000000) >> 32);
                switch (control_packet.msg) {
                    case MSG_CREATE_THREAD:
                        
                        client->on_server_status_changed(true);
                        MainController::get_instance()->create_render_thread(
                            pid, tid);
                        ALOGD("client create thread pid %d tid %d", pid, tid);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    close(client->epoll_fd);
    close(client->client_controller_fd);
    return nullptr;
}

void MainClient::stop_server() const {
    if (is_server_connected) {
        send_control_packet(client_controller_fd, (uint64_t)gettid(),
                            MSG_SERVER_STOP);
    }
}

int MainClient::read_to_buf(int fd, char *buf, size_t len) {
    int read_len = 0;
    int ret;
    do {
        ret = read(fd, buf,
                   BUF_SIZE < len - read_len ? BUF_SIZE : len - read_len);
        buf += ret;
        read_len += ret;
    } while (read_len < len && ret > 0);

    return read_len == len ? read_len : -1;
}

int MainClient::write_buf_to(int fd, const char *buf, size_t len) {
    size_t write_len = len;
    int ret;
    do {
        ret = write(fd, buf, BUF_SIZE < write_len ? BUF_SIZE : write_len);
        buf += ret;
        write_len -= ret;
    } while (write_len > 0 && ret > 0);
    return write_len == 0 ? 0 : -1;
}

int MainClient::non_blocking_socket(int sfd) {
    int flags;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        ALOGE("fcntl failed");
        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(sfd, F_SETFL, flags) == -1) {
        ALOGE("fcntl nonblocking failed");
        return -1;
    }

    return 0;
}

int MainClient::create_epoll_events() {
    if (non_blocking_socket(client_controller_fd) < 0) {
        ALOGE("failed to make socket nonblocking");
        return -1;
    }

    
    
    struct epoll_event event {};
    if ((epoll_fd = epoll_create1(0)) < 0) {
        ALOGE("epoll_create failed");
        return -1;
    }
    event.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
    event.data.fd = client_controller_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_controller_fd, &event) < 0) {
        ALOGE("epoll_ctl failed");
        return -1;
    }
    events = reinterpret_cast<struct epoll_event *>(
        calloc(MAX_EVENTS, sizeof event));
    client_should_running = true;
    return 0;
}

void MainClient::register_server_status_callback(void (*callback)(bool)) {
    server_status_callback = callback;
}

void MainClient::on_server_status_changed(bool server_status) {
    is_server_connected = server_status;
    server_status_callback(is_server_connected);
}
