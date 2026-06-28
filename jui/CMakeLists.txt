cmake_minimum_required(VERSION 3.22)
project(jui C)

set(CMAKE_C_STANDARD 99)

add_library(jui STATIC
        src/transient.c
        src/core.c
        src/core_memory.c)

target_include_directories(jui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(jui PUBLIC common_platform_config SDL3::SDL3)

target_compile_options(jui PRIVATE
    $<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-Wall -Wextra -Wpedantic>
    $<$<C_COMPILER_ID:MSVC>:/W4>
)