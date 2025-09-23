#include <functional>
#include <unordered_map>

#include "leveldb/options.h"
#define LOG_TAG "ProgramCache"

#include <leveldb/cache.h>
#include <sys/stat.h>
#include <xxhash.h>

#include <cstdint>
#include <memory>
#include <string>

#include "Log.h"
#include "ProgramCache.h"
#include "data/Cache.h"

ProgramCache::ProgramCache() {
    leveldb::Options options;
    options.block_cache = leveldb::NewLRUCache(100 * 1024 * 1024);
    init_leveldb(options, CACHE_FILE);
}

#ifdef USE_LOCAL_COMPILER
void ProgramCache::get_program(GLenum mock_binary_format,
                               const GLchar *mock_binary,
                               uint64_t mock_binary_size,
                               GLenum *real_binary_format,
                               std::string *real_binary,
                               uint64_t *real_binary_size) {
    std::string key;
    gen_key(mock_binary_format, mock_binary, mock_binary_size, &key);

    leveldb::ReadOptions options;
    Cache::get(options, key, real_binary);
    if (real_binary->empty()) {
        leveldb::Iterator *it =
            Cache::get_db()->NewIterator(leveldb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            std::string it_key = it->key().ToString();
            const char *it_key_chars = it_key.c_str();
            ALOGE("%s: Key in cache is %s", __FUNCTION__, it_key_chars);
        }
        delete it;

        ALOGE("%s: Real binary not found in cache, key is %s", __FUNCTION__,
              key.c_str());
        return;
    }
    unsigned long size = real_binary->size();
    char *data = real_binary->data();
    if (size < sizeof(GLenum)) {
        ALOGE("%s: Real binary size is too small: %lu", __FUNCTION__, size);
        return;
    }
    *real_binary_format = *(GLenum *)(data + size - sizeof(GLenum));
    *real_binary_size = real_binary->size() - sizeof(GLenum);
}

void ProgramCache::put_program(GLenum mock_binary_format,
                               const GLchar *mock_binary,
                               uint64_t mock_binary_size,
                               GLenum real_binary_format,
                               const GLchar *real_binary,
                               uint64_t real_binary_size) {
    std::string key;
    // They key is formed as mock_binary_format-mock_binary_hash
    gen_key(mock_binary_format, mock_binary, mock_binary_size, &key);

    leveldb::WriteOptions options;
    auto data = std::make_unique<char[]>(real_binary_size + sizeof(GLenum));
    memcpy(data.get(), real_binary, real_binary_size);
    memcpy(data.get() + real_binary_size, &real_binary_format, sizeof(GLenum));
    Cache::put(options, key, data.get(), real_binary_size + sizeof(GLenum));

    ALOGD("%s: Put real binary to cache, key is %s", __FUNCTION__, key.c_str());
}

void ProgramCache::remove_program(GLenum mock_binary_format,
                                  const GLchar *mock_binary,
                                  uint64_t mock_binary_size) {
    std::string key;
    gen_key(mock_binary_format, mock_binary, mock_binary_size, &key);

    leveldb::WriteOptions options;
    Cache::remove(options, key);
}

void ProgramCache::gen_key(GLenum binary_format, const GLchar *binary,
                           uint64_t binary_size, std::string *key) const {
    *key = "";
    *key += std::to_string(binary_format);
    *key += "-";

    uint64_t binary_hash = XXH3_64bits(binary, binary_size);
    *key += std::to_string(binary_hash);
}
#endif

void ProgramCache::get_program_data(const GLchar *binary, uint64_t binary_size,
                                    std::string *program_data) {
    if (binary == nullptr || binary_size == 0) {
        return;
    }
    std::string binary_hash = std::to_string(XXH3_64bits(binary, binary_size));

    leveldb::ReadOptions options;
    Cache::get(options, binary_hash, program_data);
    if (program_data->empty()) {
        ALOGE("%s: Real binary not found in cache, key is %s", __FUNCTION__,
              binary_hash.c_str());
    }
}

void ProgramCache::put_program_data(const GLchar *binary, uint64_t binary_size,
                                    const GLchar *program_data,
                                    uint64_t program_data_size) {
    std::string binary_hash = std::to_string(XXH3_64bits(binary, binary_size));

    leveldb::WriteOptions options;
    Cache::put(options, binary_hash, program_data, program_data_size);
}

void ProgramCache::remove_program(const GLchar *binary, uint64_t binary_size) {
    std::string binary_hash = std::to_string(XXH3_64bits(binary, binary_size));

    leveldb::WriteOptions options;
    Cache::remove(options, binary_hash);
}