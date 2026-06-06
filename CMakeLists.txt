cmake_minimum_required(VERSION 3.22)
project(jui C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Common compile definitions across targets
add_library(common_platform_config INTERFACE)
target_compile_definitions(common_platform_config INTERFACE
    $<$<PLATFORM_ID:Windows>:WIN32>
    $<$<PLATFORM_ID:Darwin>:MACOS>
    $<$<PLATFORM_ID:Linux>:LINUX>
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>)

add_subdirectory(third_party)

add_subdirectory(jui)
add_subdirectory(jui_sandbox)
