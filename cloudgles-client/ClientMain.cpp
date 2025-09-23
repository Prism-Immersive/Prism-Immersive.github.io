#include <android/native_window_jni.h>
#include <jni.h>

#include <utility>

#include "Utils.h"
#include "renderer/MainController.h"

jobject main_activity_obj;
jmethodID on_server_status_change_method;
JavaVM *jvm = nullptr;

extern "C" {

void on_server_status_change(bool server_status) {
    if (on_server_status_change_method) {
        JNIEnv *env;
        jvm->AttachCurrentThread(&env, nullptr);
        env->CallVoidMethod(main_activity_obj, on_server_status_change_method,
                            server_status);
    }
}

MainController *controller = nullptr;

void client_run(ANativeWindow *window, int max_refresh_rate,
                std::string server_ip = "127.0.0.1") {
    controller = MainController::get_instance();
    controller->run(window, max_refresh_rate, std::move(server_ip),
                    on_server_status_change);
}

JNIEXPORT void JNICALL Java_com_cloud_client_1app_MainActivity_run(
    JNIEnv *env, jobject thiz, jobject surface, jstring ip) {
    main_activity_obj = env->NewGlobalRef(thiz);
    env->GetJavaVM(&jvm);
    jclass main_activity = env->GetObjectClass(thiz);
    on_server_status_change_method =
        env->GetMethodID(main_activity, "onServerStatusChange", "(Z)V");

    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
    const char *ip_addr = env->GetStringUTFChars(ip, nullptr);
    client_run(window, 60, ip_addr);
    env->ReleaseStringUTFChars(ip, ip_addr);
}

JNIEXPORT void JNICALL
Java_com_cloud_client_1app_MainActivity_stop(JNIEnv *env, jobject thiz) {
    if (likely(controller))
        controller->force_stop();
    else
        MainController::get_instance()->force_stop();
}
}
extern "C" JNIEXPORT void JNICALL
Java_com_cloud_client_1app_RenderSurfaceView_injectTouchEvent(
    JNIEnv *env, jobject thiz, jint action, jfloat x, jfloat y) {
#if defined(INPUT_SERVICE)
    if (likely(MainController::get_instance()->is_running)) {
        MainController::get_instance()
            ->input_service_proxy->inject_touch_events(
                static_cast<short>(action), static_cast<short>(x),
                static_cast<short>(y));
    }
#endif
}
extern "C"
JNIEXPORT void JNICALL
Java_com_cloud_client_1app_RenderSurfaceView_setNativeRotation(JNIEnv *env, jobject thiz,
                                                               jint rotation) {
    MainController::get_instance()->set_rotation(rotation);
}