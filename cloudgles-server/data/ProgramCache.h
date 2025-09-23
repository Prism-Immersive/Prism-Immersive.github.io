#ifndef PROGRAM_CACHE_H
#define PROGRAM_CACHE_H

#include <GLES2/gl2.h>

#include <cstdint>
#include <string>

#include "Cache.h"

class ProgramCache : public Cache {
   public:
    ProgramCache();
    ~ProgramCache() final = default;

#ifdef USE_LOCAL_COMPILER
    void get_program(GLenum mock_binary_format, const GLchar *mock_binary,
             uint64_t mock_binary_size, GLenum *real_binary_format,
             std::string *real_binary, uint64_t *real_binary_size);
    void put_program(GLenum mock_binary_format, const GLchar *mock_binary,
             uint64_t mock_binary_size, GLenum real_binary_format,
             const GLchar *real_binary, uint64_t real_binary_size);
    void remove_program(GLenum mock_binary_format, const GLchar *mock_binary,
                uint64_t mock_binary_size);
#else
    void get_program_data(const GLchar *binary, uint64_t binary_size, std::string *program_data);
    void put_program_data(const GLchar *binary, uint64_t binary_size, const GLchar *program_data, uint64_t program_data_size);
    void remove_program(const GLchar *binary, uint64_t binary_size);
#endif

   private:
#ifdef USE_LOCAL_COMPILER
    void gen_key(GLenum binary_format, const GLchar *binary,
                 uint64_t binary_size, std::string *key) const;
#endif

    constexpr static const char *CACHE_FILE = "ProgramCache.db";
};

#endif  // PROGRAM_CACHE_H