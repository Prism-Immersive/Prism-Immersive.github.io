#include "MainController.h"

#include <EGL/egl.h>
#include <EGL/eglplatform.h>

#include <utility>

#include "EglFuncID.h"
#include "GLFuncID.h"
#include "Log.h"
#include "MainRenderer.h"
#include "RenderThread.h"
#include "Utils.h"
#include "egl/EglTranslator.h"
#include "egl/Surface.h"
#include "opengl/GLTranslator.h"
#include "opengl/Resource.h"

#define LOG_TAG "MainController"

MainController *MainController::instance = nullptr;

void MainController::run(ANativeWindow *window, int max_refresh_rate,
                         std::string ip, void (*server_status_callback)(bool)) {
    if (is_running) force_stop();
    if (!main_client->client_should_running) {
        main_client->register_server_status_callback(server_status_callback);
        main_client->start_client(ip);
    }
#if defined(INPUT_SERVICE)
    input_service_proxy->start(ip);
#endif
    if (!main_renderer->main_renderer_should_run) main_renderer->start(window);
    MainRenderer::COMPOSER_REFRESH_RATE = max_refresh_rate;
    is_running = true;
}

void MainController::force_stop() {
    if (!is_running) return;

    SPIN_LOCK(render_threads_lock);
    for (auto &pid_it : render_threads) {
        for (auto &tid_it : pid_it.second) {
            tid_it.second->stop();
        }
    }
    render_threads.clear();
    SPIN_UNLOCK(render_threads_lock);

    main_client->force_stop();
#if defined(INPUT_SERVICE)
    input_service_proxy->stop();
#endif

    is_running = false;
}

void MainController::stop_server() const {
    if (main_client) main_client->stop_server();
}

MainController *MainController::get_instance() {
    if (instance == nullptr) {
        instance = new MainController();
    }
    return instance;
}

MainController::MainController() {
    main_renderer = MainRenderer::get_instance();
    main_client = new MainClient();
#if defined(INPUT_SERVICE)
    input_service_proxy = new InputServiceProxy();
#endif
}

void MainController::create_render_thread(pid_t pid, pid_t tid) {
    SPIN_LOCK(render_threads_lock);
    auto &thread_map = render_threads[pid];
    if (!thread_map.empty()) {
        if (thread_map.find(tid) == thread_map.end())
            thread_map[tid] = new RenderThread(
                thread_map.begin()->second->thread_context->process_context,
                tid);
    } else {
        thread_map[tid] = new RenderThread(pid, tid);
    }
    SPIN_UNLOCK(render_threads_lock);
}

MainController::~MainController() {
    delete main_renderer;
    delete main_client;
#if defined(INPUT_SERVICE)
    delete input_service_proxy;
#endif
}

void MainController::destroy_render_thread(pid_t pid, pid_t tid) {
    SPIN_LOCK(render_threads_lock);
    auto thread_map_it = render_threads.find(pid);
    if (thread_map_it == render_threads.end()) return;
    auto thread_map = &render_threads[pid];

    auto render_thread_it = thread_map->find(tid);
    if (render_thread_it == thread_map->end()) return;
    auto render_thread = render_thread_it->second;
    thread_map->erase(tid);
    size_t thread_map_size = thread_map->size();

    render_thread->stop();
    if (thread_map_size == 0) render_threads.erase(pid);
    SPIN_UNLOCK(render_threads_lock);
}

void MainController::set_rotation(int rotation) {
    MainRenderer::get_instance()->set_rotation(rotation);
}
