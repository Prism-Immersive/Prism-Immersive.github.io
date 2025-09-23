#define LOG_TAG "MainServer"
#include "MainServer.h"

#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <memory>
#include <utility>

#include "Log.h"
#include "Network.h"
#include "boost/asio/ip/tcp.hpp"

#define LOG_TAG "Main_Server"

MainServer::MainServer() {
    server_io_context = std::make_unique<boost::asio::io_context>();
    pthread_mutex_init(&thread_lock, nullptr);
}

void MainServer::start() {
    pthread_mutex_lock(&thread_lock);
    if (!is_server_running) {
        is_server_running = true;
        wait_for_client();
    }
    pthread_mutex_unlock(&thread_lock);
}

void MainServer::stop() { is_server_running = false; }

void MainServer::wait_for_client() {
    if (is_client_connected) return;
    FAILURE_RETRY_SLEEP(create_server());
    std::unique_ptr<tcp::socket> client_socket = wait_for_connect();
    if (client_socket == nullptr) {
        ALOGE("failed to create server, %d, %s", errno, strerror(errno));
        return;
    }

    if (client_status_callback) on_client_status_changed(false);
    recv_control_packet(std::move(client_socket));
}

int MainServer::create_server() {
    server_acceptor = std::make_unique<tcp::acceptor>(*server_io_context);
    server_acceptor->open(tcp::v4());
    int acceptor_fd = server_acceptor->native_handle();
    if (acceptor_fd < 0) {
        ALOGE("cannot create server acceptor, %d, %s", errno, strerror(errno));
        return -1;
    }

    // Allow reuse of the address and port
    int opt = 1;
    if (setsockopt(acceptor_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        ALOGE("server cannot set socket option, %d, %s", errno,
              strerror(errno));
        return -1;
    }

    try {
        server_acceptor->bind(tcp::endpoint(tcp::v4(), ports[port_idx]));
        server_acceptor->listen();
    } catch (boost::system::system_error &e) {
        ALOGE("server cannot bind/listen, %d, %s", e.code().value(), e.what());
        port_idx++;
        port_idx %= sizeof(ports) / sizeof(unsigned short);
        return -1;
    }
    return 0;
}

std::unique_ptr<tcp::socket> MainServer::wait_for_connect() const {
    auto socket = std::make_unique<tcp::socket>(*server_io_context);
    try {
        server_acceptor->accept(*socket);
    } catch (boost::system::system_error &e) {
        ALOGE("server accept failed, %d, %s", e.code().value(), e.what());
        return nullptr;
    }
    return socket;
}

int MainServer::read_to_buf(int fd, char *buf, size_t len) {
    int read_len = 0;
    size_t ret;
    do {
        ret = read(fd, buf,
                   BUF_SIZE < len - read_len ? BUF_SIZE : len - read_len);
        buf += ret;
        read_len += ret;
    } while (read_len < len && ret > 0);

    return read_len == len ? read_len : -1;
}

int MainServer::write_buf_to(int fd, const char *buf, size_t len) {
    size_t write_len = len;
    size_t ret;
    do {
        ret = write(fd, buf, BUF_SIZE < write_len ? BUF_SIZE : write_len);
        buf += ret;
        write_len -= ret;
    } while (write_len > 0 && ret > 0);
    return write_len == 0 ? 0 : -1;
}

int MainServer::read_to_buf(tcp::socket *socket, char *buf, size_t len) {
    if (socket == nullptr) return -1;
    boost::system::error_code ec;
    size_t read_len =
            boost::asio::read(*socket, boost::asio::buffer(buf, len), ec);
    if (!ec || ec == boost::asio::error::eof)
        return read_len == len ? (int)read_len : -1;
    return -1;
}

int MainServer::write_buf_to(tcp::socket *socket, const char *buf, size_t len) {
    if (socket == nullptr) return -1;
    boost::system::error_code ec;
    size_t write_len =
            boost::asio::write(*socket, boost::asio::buffer(buf, len), ec);
    if (!ec || ec == boost::asio::error::eof)
        return write_len == len ? 0 : -1;
    return -1;
}

tcp::socket *MainServer::get_thread_socket(pid_t tid, ControlMSG map_msg) {
    tcp::socket *socket;
    std::unordered_map<pid_t, std::unique_ptr<tcp::socket>> *map;
    switch (map_msg) {
        case MSG_CREATE_THREAD:
            map = &thread_socket_map;
            break;
        case MSG_CREATE_MEM_MAPPER:
            map = &thread_mapper_socket_map;
            break;
        default:
            return nullptr;
    }

    pthread_mutex_lock(&socket_map_lock);
    auto result = map->find(tid);
    socket = result == map->end() ? nullptr : result->second.get();
    pthread_mutex_unlock(&socket_map_lock);
    return socket;
}

void MainServer::send_control_packet(ControlMSG msg, int extra_data) {
    ControlPacket packet;
    packet.thread_id = (((uint64_t)getpid()) << 32) | gettid();
    packet.msg = msg;
    packet.extra_data = extra_data;
    ALOGD("server send message %d", msg);

    if (!is_client_connected) {
        ALOGE("server sends control packet before client connection");
        wait_for_client();
    }
    write_buf_to(main_client_socket.get(), (char *)&packet,
                 sizeof(ControlPacket));

    if (msg == MSG_CREATE_THREAD) {
        
        std::unique_ptr<tcp::socket> thread_socket = wait_for_connect();
        if (thread_socket == nullptr) return;
        recv_control_packet(std::move(thread_socket));

        
        std::unique_ptr<tcp::socket> mem_mapper_socket = wait_for_connect();
        if (mem_mapper_socket == nullptr) return;
        recv_control_packet(std::move(mem_mapper_socket));
    }
}

MainServer::~MainServer() {
    if (main_client_socket != nullptr) {
        try {
            main_client_socket->close();
        } catch (boost::system::system_error &e) {
            ALOGE("Failed to close main client socket, %d, %s",
                  e.code().value(), e.what());
        }
    }
}

void MainServer::recv_control_packet(std::unique_ptr<tcp::socket> socket) {
    ControlPacket msg{};
    read_to_buf(socket.get(), (char *)&msg, sizeof(ControlPacket));
    auto tid = (int)(msg.thread_id & 0x000000FFFFFFFF);
    switch (msg.msg) {
        case MSG_CREATE_THREAD:
            pthread_mutex_lock(&socket_map_lock);
            thread_socket_map[tid] = std::move(socket);
            pthread_mutex_unlock(&socket_map_lock);
            break;
        case MSG_MAIN_CLIENT_CONNECT:
            if (!is_client_connected) {
                main_client_socket = std::move(socket);
                on_client_status_changed(true);
            }
            break;
        case MSG_CREATE_MEM_MAPPER:
            pthread_mutex_lock(&socket_map_lock);
            thread_mapper_socket_map[tid] = std::move(socket);
            pthread_mutex_unlock(&socket_map_lock);
            break;
        default:
            break;
    }
}

void MainServer::register_client_status_callback(
    ClientStatusCallback &callback) {
    client_status_callback = callback;
}

void MainServer::on_client_status_changed(int client_status) {
    is_client_connected = client_status > 0;
    if (client_status_callback) client_status_callback(is_client_connected);
}
