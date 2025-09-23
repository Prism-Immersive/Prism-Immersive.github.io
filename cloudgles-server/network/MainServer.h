#ifndef MAIN_SERVER_H
#define MAIN_SERVER_H

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>

#include "Network.h"
#include "pthread.h"
#include "Utils.h"

using boost::asio::ip::tcp;

/**
 * Every thread has a socket to communicate with the client thread
 */
class MainServer {
   public:
    MainServer();
    ~MainServer();
    void start();
    void stop();

    void wait_for_client();

    tcp::socket *get_thread_socket(pid_t tid, ControlMSG map_msg);

    void send_control_packet(ControlMSG msg, int extra_data = 0);
    void recv_control_packet(std::unique_ptr<tcp::socket> socket);
    static int read_to_buf(int fd, char *buf, size_t len);
    static int write_buf_to(int fd, const char *buf, size_t len);
    static int read_to_buf(tcp::socket *socket, char *buf, size_t len);
    static int write_buf_to(tcp::socket *socket, const char *buf, size_t len);
    void on_client_status_changed(int client_status);
    void register_client_status_callback(ClientStatusCallback &callback);

   private:
    pthread_mutex_t thread_lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t socket_map_lock = PTHREAD_MUTEX_INITIALIZER;
    std::unordered_map<pid_t, std::unique_ptr<tcp::socket>> thread_socket_map;
    std::unordered_map<pid_t, std::unique_ptr<tcp::socket>> thread_mapper_socket_map;

    bool is_server_running = false;
    bool is_client_connected = false;

    unsigned short ports[5] = {8890, 8891, 8892, 8893, 8894};

    std::unique_ptr<boost::asio::io_context> server_io_context = nullptr;
    std::unique_ptr<tcp::acceptor> server_acceptor = nullptr;
    std::unique_ptr<tcp::socket> main_client_socket = nullptr;
    int port_idx = 0;
    ClientStatusCallback client_status_callback = nullptr;

    int create_server();
    std::unique_ptr<tcp::socket> wait_for_connect() const;
};

#endif  // MAIN_SERVER_H
