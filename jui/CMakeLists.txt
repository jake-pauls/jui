cmake_minimum_required(VERSION 4.2)
project(jui C)

set(CMAKE_C_STANDARD 99)

add_library(jui STATIC
        src/transient.c)

target_include_directories(jui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(jui PRIVATE common_platform_config tak SDL3::SDL3)