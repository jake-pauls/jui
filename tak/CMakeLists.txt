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
target_link_libraries(tak PRIVATE common_platform_config)