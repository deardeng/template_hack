cmake_minimum_required(VERSION 3.16)
project(template_hack)

set(CMAKE_CXX_STANDARD 20)

add_executable(gen reflect_static/main.cpp)
add_executable(quick_sort quick_sort/main.cpp)
add_executable(args args/main.cpp)