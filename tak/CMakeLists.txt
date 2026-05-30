cmake_minimum_required(VERSION 4.2)
project(tak C)

set(CMAKE_C_STANDARD 99)

add_library(tak STATIC
        src/helpers.c
        src/m44.c
        src/v2.c
        src/v3.c
        src/v4.c)

target_include_directories(tak PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Platform specific definitions
if(WIN32)
    target_compile_definitions(tak PRIVATE WIN32)
elseif(APPLE)
    target_compile_definitions(tak PRIVATE MACOS)
elseif(UNIX)
    target_compile_definitions(tak PRIVATE UNIX)
endif()

# Compiler specific definitions
target_compile_definitions(tak PRIVATE $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>)