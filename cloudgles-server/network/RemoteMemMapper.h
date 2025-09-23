#ifndef REMOTE_MEM_MAPPER_H
#define REMOTE_MEM_MAPPER_H

#include <pthread.h>

#include <boost/asio.hpp>
#include <cstdint>
#include <unordered_set>

#include "Network.h"

using boost::asio::ip::tcp;

class RemoteMemMapper;

class RemoteMem {
   public:
    explicit RemoteMem(uint64_t mem_size);
    ~RemoteMem();
    void write_mem(char *data, uint32_t data_len) const;

    char *mem = nullptr;
    uint64_t size = 0;
    std::atomic_flag lock = ATOMIC_FLAG_INIT;
};

class RemoteMemMapper {
   public:
    RemoteMem *map_memory(uint64_t length);
    void unmap_memory(const RemoteMem *mem);

    RemoteMemMapper();
    ~RemoteMemMapper();

   private:
    tcp::socket *client_socket = nullptr;
    pthread_t read_pthread{};
    bool mapper_should_run = true;
    std::unordered_set<uint64_t> mapped_memories;
    std::atomic_flag map_lock = ATOMIC_FLAG_INIT;
    char static_read_buffer[BUF_SIZE] = {0};

    static void *async_read_thread(void *data);
};

#endif  // REMOTE_MEM_MAPPER_H
