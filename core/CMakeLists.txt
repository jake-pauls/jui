cmake_minimum_required(VERSION 4.2)
project(core C)

set(CMAKE_C_STANDARD 99)

add_library(core STATIC
        src/core/hash.c
        src/tak/helpers.c
        src/tak/m44.c
        src/tak/v2.c
        src/tak/v3.c
        src/tak/v4.c)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(core PRIVATE common_platform_config)
