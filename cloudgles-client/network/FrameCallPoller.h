#ifndef FRAME_CALL_POLLER_H
#define FRAME_CALL_POLLER_H

#include <unistd.h>

#include "ThreadPacketPoller.h"
#include "data/Decompressor.h"
#include "renderer/RenderThread.h"

#define MAX_BUF_LEN 4096

class GLFrameData {
   public:
    uint64_t *call_offsets_in_data;  // The offset of the gl_call in call_data
    int max_num_calls;               // The max size of call_offsets_in_data
    int num_calls;  // The index of the next call to be added in
                    // call_offsets_in_data
    int read_cursor;

    char *call_data;  // An array of CompressGLCall
    int call_data_max_len;
    size_t call_data_len;

    size_t resource_para_len;  // The total length of all resource
                               // parameters in call_data

    GLFrameData();

    ~GLFrameData();

    void add_calls(ThreadContext *context, char *buf_data, size_t buf_data_len,
                   int num_added_calls);
    CompressGLCall *read_one_call();
    void parse_resource_data(ThreadContext *context, CompressGLCall *call,
                             int res_index);
};

class FrameCallPoller {
   public:
    FrameCallPoller();
    FrameCallPoller(const FrameCallPoller &poller) = delete;
    ~FrameCallPoller();
    FrameCallPoller &operator=(const FrameCallPoller &) = delete;

    void poll_and_call(ThreadContext *context);
    void end_one_frame();

    static void return_call(ThreadContext *context, char *buf, size_t buf_len);

   private:
    GLFrameData *pre_frame_data;
    GLFrameData *cur_frame_data;

    void process_frame_calls(ThreadContext *context, DataPacketHeader *header, std::string *data);
    void process_context_less_call(ThreadContext *context, std::string *data);
};

#endif  // FRAME_CALL_POLLER_H
