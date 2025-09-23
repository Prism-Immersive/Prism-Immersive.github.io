#ifndef REMOTE_MEM_WRITER_H
#define REMOTE_MEM_WRITER_H

#include <cstddef>
#include <cstdint>

class RemoteMemWriter {
   public:
    explicit RemoteMemWriter(uint64_t tid);
    ~RemoteMemWriter();

    void connect_server();
    void write_mem(uint64_t addr, char *data, uint32_t data_len);

   private:
    int server_fd;
    uint64_t server_tid;
};

#endif  // REMOTE_MEM_WRITER_H
