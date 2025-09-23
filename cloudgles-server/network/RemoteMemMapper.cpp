#define LOG_TAG "RemoteMemMapper"
#include "RemoteMemMapper.h"

#include <cinttypes>

#include "Log.h"
#include "ThreadContext.h"
#include "Utils.h"

RemoteMem::RemoteMem(uint64_t mem_size) : size(mem_size) {
    mem = reinterpret_cast<char *>(calloc(1, size));
}

RemoteMem::~RemoteMem() { free(mem); }

void RemoteMem::write_mem(char *data, uint32_t data_len) const {
    if (likely(data_len <= size))
        memcpy(mem, data, data_len);
    else
        ALOGE("Remote mem %llx (size is %llu) cannot hold data (size is %d)",
              reinterpret_cast<unsigned long long>(mem),
              static_cast<unsigned long long>(size), data_len);
}

RemoteMem *RemoteMemMapper::map_memory(uint64_t length) {
    auto *mem = new RemoteMem(length);
    SPIN_LOCK(map_lock);
    mapped_memories.insert(reinterpret_cast<uint64_t>(mem));
    SPIN_UNLOCK(map_lock);
    return mem;
}

void RemoteMemMapper::unmap_memory(const RemoteMem *mem) {
    // Remote_Mem should live in thread context rather than surface.
    // Otherwise, when EGL_Surface_Status is not destroyed when the thread that
    // maps it dies, Remote_Mem_Mapper is deleted before Remote_Mem, causing
    // SIGSEGV in unmap, When Surface is detached from the thread context (make
    // current null), its mapped memory should also detach When Surface is
    // connected to another thread, its mapped memory should transfer to the new
    // thread
    if (mapped_memories.find(reinterpret_cast<uint64_t>(mem)) !=
        mapped_memories.end())
        mapped_memories.erase(reinterpret_cast<uint64_t>(mem));
    delete mem;
}

RemoteMemMapper::RemoteMemMapper() {
    client_socket =
        main_server->get_thread_socket(gettid(), MSG_CREATE_MEM_MAPPER);
    pthread_create(&read_pthread, nullptr, async_read_thread, this);
}

RemoteMemMapper::~RemoteMemMapper() {
    mapper_should_run = false;
    pthread_join(read_pthread, nullptr);
    if (client_socket != nullptr) {
        try {
            client_socket->close();
        } catch (boost::system::system_error &e) {
            ALOGE("Failed to close client socket, %d, %s", e.code().value(),
                  e.what());
        }
    }
}

void *RemoteMemMapper::async_read_thread(void *data) {
    auto *mem_mapper = reinterpret_cast<RemoteMemMapper *>(data);
    RemoteMemHeader header;
    while (mem_mapper->mapper_should_run) {
        if (mem_mapper->client_socket == nullptr) {
            do {
                mem_mapper->client_socket = main_server->get_thread_socket(
                    gettid(), MSG_CREATE_MEM_MAPPER);
                usleep(1000);
            } while (mem_mapper->client_socket == nullptr &&
                     mem_mapper->mapper_should_run);
        }
        int len = MainServer::read_to_buf(mem_mapper->client_socket,
                                          reinterpret_cast<char *>(&header),
                                          sizeof(RemoteMemHeader));
        if (len < 0) {
            mem_mapper->client_socket =
                main_server->get_thread_socket(gettid(), MSG_CREATE_MEM_MAPPER);
            continue;
        }

        SPIN_LOCK(mem_mapper->map_lock);
        if (mem_mapper->mapped_memories.find(header.mem_addr) ==
            mem_mapper->mapped_memories.end()) {
            ALOGE("Writing to remote mem %" PRIu64 "which no longer exists!",
                  reinterpret_cast<uint64_t>(header.mem_addr));
            continue;
        }
        SPIN_UNLOCK(mem_mapper->map_lock);
        auto *mem = reinterpret_cast<RemoteMem *>(header.mem_addr);
        if (header.size > mem->size) {
            ALOGE("Remote mem %" PRIu64 "(size is %" PRIu64
                  ") cannot hold data sent by client (size is %d)",
                  reinterpret_cast<uint64_t>(mem->mem), mem->size, header.size);
            continue;
        }

        char *read_buf;
        if (unlikely(header.size > BUF_SIZE))
            read_buf = new char[header.size];
        else
            read_buf = mem_mapper->static_read_buffer;
        len = MainServer::read_to_buf(mem_mapper->client_socket, read_buf,
                                      header.size);
        if (len < 0) {
            if (unlikely(header.size > BUF_SIZE &&
                         read_buf != mem_mapper->static_read_buffer))
                delete[] read_buf;
            mem_mapper->client_socket =
                main_server->get_thread_socket(gettid(), MSG_CREATE_MEM_MAPPER);
            continue;
        }
        mem->write_mem(read_buf, header.size);
        if (unlikely(header.size > BUF_SIZE &&
                     read_buf != mem_mapper->static_read_buffer))
            delete[] read_buf;
    }
    return nullptr;
}
