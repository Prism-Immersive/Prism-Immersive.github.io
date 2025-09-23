cmake_minimum_required(VERSION 3.4.1)

include_directories("./" "../third_party/zstd/lib")
include_directories("./" "../third_party/xxHash")
include_directories("./" "../third_party/fpzip/include")
include_directories("./" "../third_party/FP16/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DINPUT_SERVICE")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FILE(GLOB_RECURSE OPENGL_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.cpp)
FILE(GLOB_RECURSE EGL_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/egl/*.cpp)
FILE(GLOB_RECURSE NETWORK_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/network/*.cpp)
FILE(GLOB_RECURSE DECOMPRESS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp)
FILE(GLOB_RECURSE RENDERER_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/renderer/*.cpp)

add_library( # Sets the name of the library.
        glclient

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${OPENGL_SRC}
        ${EGL_SRC}
        ${NETWORK_SRC}
        ${DECOMPRESS_SRC}
        ${RENDERER_SRC}
        ClientMain.cpp
        )

find_package(ZLIB)
find_library(log-lib log)
find_library(android-lib android)
find_library(egl-lib EGL)
find_library(gl-lib GLESv1_CM)
find_library(gl2-lib GLESv2)
find_library(gl3-lib GLESv3)

if(${SANITIZE})
    # For asan and ubsan, we need to copy some files from the NDK into our APK.
    get_filename_component(HINT_PATH ${ANDROID_C_COMPILER} DIRECTORY)
    set(ARCH_STR ${CMAKE_ANDROID_ARCH})
    if ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "arm64-v8a")
        set(ARCH_STR "aarch64")
    elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "armeabi")
        set(ARCH_STR "arm")
    elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "armeabi-v7a")
        set(ARCH_STR "arm")
    elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "x86")
        set(ARCH_STR "i686")
    elseif ("${CMAKE_ANDROID_ARCH_ABI}" STREQUAL "x86_64")
        set(ARCH_STR "x86_64")
    endif()

    message("Using asan")

    target_compile_options(glclient PUBLIC -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(glclient PUBLIC -fsanitize=address)

    # Grab libclang_rt.asan-${ARCH_STR}-android.so from the NDK.
    file(GLOB ASAN_GLOB "${HINT_PATH}/../lib64/clang/*/lib/linux")
    find_file(ASAN
            NAMES libclang_rt.asan-${ARCH_STR}-android.so
            PATHS ${ASAN_GLOB})
    get_filename_component(ASAN_NAME ${ASAN} NAME)
    set(ASAN_NAME ${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/${ASAN_NAME})
    add_custom_command(
            TARGET glclient PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${ASAN} ${ASAN_NAME})
    message("ASAN_NAME: ${ASAN_NAME}")
    message("ASAN: ${ASAN}")

    # Grab the asan wrapper script from the NDK.
    find_file(WRAP
            NAMES asan.sh
            HINTS ${HINT_PATH}/../../../../../wrap.sh)
    set(WRAP_NAME ${CMAKE_SOURCE_DIR}/../resources/lib/${CMAKE_ANDROID_ARCH_ABI}/wrap.sh)
    add_custom_command(
            TARGET glclient PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${WRAP} ${WRAP_NAME})
else()
    file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/../jniLibs)
    file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/../resources)
endif()

target_link_libraries(glclient ZLIB::ZLIB)
target_link_libraries(glclient ${egl-lib})
target_link_libraries(glclient ${gl-lib})
target_link_libraries(glclient ${gl2-lib})
target_link_libraries(glclient ${gl3-lib})
target_link_libraries(glclient ${log-lib})
target_link_libraries(glclient ${android-lib})
# If the library has already been add_subdirectory, just link to its name
# Don't use find_library to find it, because you'll never do
target_link_libraries(glclient zstd)
target_link_libraries(glclient xxhash)
target_link_libraries(glclient fpzip)
target_link_libraries(glclient psimd)
target_link_libraries(glclient fp16)


