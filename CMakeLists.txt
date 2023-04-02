cmake_minimum_required(VERSION 3.24)
project(OS C)

set(CMAKE_C_STANDARD 11)

add_executable(OS main.c)

target_link_libraries(OS pthread m)

target_compile_options(OS PRIVATE -g)


