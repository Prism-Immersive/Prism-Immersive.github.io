#define LOG_TAG "Tracer"

#include "DataTracer.h"

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>

#include "Log.h"

DataTracer::DataTracer() {
#if TRACE_DATA
    pthread_create(&dump_thread, nullptr, dump, this);
    pthread_setname_np(dump_thread, "dump");
#else
    // Make compiler happy
    (void) dump_thread;
#endif
}

DataTracer::~DataTracer() {
#if TRACE_DATA
    pthread_join(dump_thread, nullptr);
    if (file != nullptr) {
        fclose(file);
    }
#endif
}

void DataTracer::trace_common(const char *fmt, ...) {
#if TRACE_DATA
    char buf[WRITE_LENGTH];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, WRITE_LENGTH, fmt, args);
    va_end(args);

    trace(buf, strlen(buf));
    trace("\t\t", 2);
#endif
}

void DataTracer::trace_resource(const char *data, int64_t len) {
#if TRACE_DATA
    trace(data, len);
    trace("\t\t", 2);
#endif
}

void DataTracer::trace(const char *data, int64_t len) {
    if (file == nullptr) {
        open_file();
    }

    while (len > 0) {
        size_t write_len = WRITE_LENGTH > len ? len : WRITE_LENGTH;
        size_t written_len = buffer.push(data, write_len);
        len -= written_len;
        data += written_len;
    }
}

void *DataTracer::dump(void *data) {
    auto tracer = (DataTracer *)data;

    char buf[WRITE_LENGTH];
    while (true) {
        if (tracer->buffer.read_available() > 0) {
            size_t read_len = tracer->buffer.pop(buf, WRITE_LENGTH);
            fwrite(buf, 1, read_len, tracer->file);
        } else {
            fflush(tracer->file);
            usleep(1);
        }
    }
}

std::string DataTracer::get_app_package_name() {
    char buf[512] = {0};
    std::string package_name;
    std::string cmdline = "/proc/" + std::to_string(getpid()) + "/cmdline";
    FILE *fp = fopen(cmdline.c_str(), "r");
    if (fp != nullptr) {
        fgets(buf, 512, fp);
        fclose(fp);
        package_name = buf;
    }
    return package_name;
}

void DataTracer::open_file() {
    std::string base_file_name = "trace";
    std::string dir_path =
        std::string(BASE_DIR) + get_app_package_name() + "/" + "traces";
    if (access(dir_path.c_str(), F_OK) == -1 &&
        mkdir(dir_path.c_str(), 0777) == -1) {
        ALOGE("mkdir %s failed", dir_path.c_str());
    }

    pid_t pid = getpid();
    pid_t tid = gettid();
    file_name = dir_path + "/" + base_file_name + "-" + std::to_string(pid) +
                "-" + std::to_string(tid) + ".dat";
    file = fopen(file_name.c_str(), "w");
    if (file == nullptr) {
        ALOGE("open file %s failed", file_name.c_str());
    }
}
