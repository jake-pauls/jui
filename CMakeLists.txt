cmake_minimum_required(VERSION 3.20)
project(jui C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(third_party)

add_subdirectory(tak)

add_subdirectory(jui)
add_subdirectory(jui_sandbox)
