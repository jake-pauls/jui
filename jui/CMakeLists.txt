cmake_minimum_required(VERSION 3.22)
project(jui C)

set(CMAKE_C_STANDARD 99)

add_library(jui STATIC
        src/transient.c
        src/core.c
        include/jui/core_memory.h
        src/core_memory.c)

target_include_directories(jui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(jui PUBLIC common_platform_config SDL3::SDL3)
