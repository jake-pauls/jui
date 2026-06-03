cmake_minimum_required(VERSION 4.2)
project(jui_sandbox C)

set(CMAKE_C_STANDARD 99)

add_executable(jui_sandbox
        src/main.c)

target_link_libraries(jui_sandbox PRIVATE common_platform_config core jui SDL3::SDL3)