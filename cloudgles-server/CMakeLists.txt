cmake_minimum_required(VERSION 3.4.1)

include_directories("include")
include_directories("./")

FILE(GLOB_RECURSE OPENGL_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/opengl/*.cpp)
FILE(GLOB_RECURSE EGL_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/egl/*.cpp)
FILE(GLOB_RECURSE NETWORK_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/network/*.cpp)
FILE(GLOB_RECURSE COMPRESS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp)

add_library( # Sets the name of the library.
             glserver

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
        ${OPENGL_SRC}
        ${EGL_SRC}
        ${NETWORK_SRC}
        ${COMPRESS_SRC}
        ThreadContext.cpp
        )

find_package(ZLIB)
find_library(log-lib log)
find_library(android-lib android )
target_link_libraries(glserver ZLIB::ZLIB)
target_link_libraries( glserver ${log-lib} )
target_link_libraries(glserver ${android-lib} )
