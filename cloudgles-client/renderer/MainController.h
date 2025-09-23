#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include "MainRenderer.h"
#include "RenderThread.h"
#include "egl/Display.h"
#include "network/InputServiceProxy.h"
#include "network/MainClient.h"

class MainController {
   public:
    MainRenderer *main_renderer;
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, RenderThread *>>
        render_threads;
    std::atomic_flag render_threads_lock = ATOMIC_FLAG_INIT;

    MainClient *main_client;
#if defined(INPUT_SERVICE)
    InputServiceProxy *input_service_proxy;
#endif

    static MainController *get_instance();

    void run(ANativeWindow *window, int max_refresh_rate, std::string ip,
             void (*server_status_callback)(bool));
    void force_stop();
    void stop_server() const;
    void create_render_thread(pid_t pid, pid_t tid);
    void destroy_render_thread(pid_t pid, pid_t tid);
    void set_rotation(int rotation);
    bool is_running = false;

   private:
    static MainController *instance;
    MainController();
    ~MainController();
};

#endif  // IN_APP_TEST_MAIN_CONTROLLER_H
