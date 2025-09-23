#ifndef TRACER_H
#define TRACER_H

#include <boost/lockfree/spsc_queue.hpp>
#include <string>
#include <pthread.h>

#define BUFFER_LENGTH 500 * 1024 * 1024
#define WRITE_LENGTH 1024
#define BASE_DIR "/data/user/0/"

class DataTracer {
   public:
    DataTracer();
    ~DataTracer();

    void trace_common(const char *fmt, ...);
    void trace_resource(const char *data, int64_t len);
    static std::string get_app_package_name();

   private:
    void trace(const char *data, int64_t len);
    static void *dump(void *data);
    void open_file();

    std::string file_name;
    FILE *file;
    boost::lockfree::spsc_queue<char, boost::lockfree::capacity<BUFFER_LENGTH>> buffer;
    pthread_t dump_thread;
};

#endif  // TRACER_H