cmake_minimum_required(VERSION 3.10.2)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        src/main.cpp
        src/all_command.cpp
        src/unrolled_linked_list.cpp
        src/all_header.cpp)
