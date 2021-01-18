cmake_minimum_required(VERSION 3.16)
project(template_hack)

set(CMAKE_CXX_STANDARD 20)

add_executable(gen reflect_static/main.cpp)
add_executable(quick_sort quick_sort/main.cpp)
add_executable(args args/main.cpp)

find_package(Boost 1.72.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(tlist list/main.cpp)



add_executable(01-Selection-Sort algorithms/02-Sorting-Basic/01-Selection-Sort/main.cpp)

file(GLOB EXTENDED_02 "algorithms/02-Sorting-Basic/02-Selection-Sort-Using-Template/*")
message(${EXTENDED_02})
add_executable(02-Selection-Sort-Using-Template ${EXTENDED_02})

file(GLOB EXTENDED_03 "algorithms/02-Sorting-Basic/03-Selection-Sort-Generate-Test-Cases/*")
message(${EXTENDED_03})
add_executable(03-Selection-Sort-Generate-Test-Cases ${EXTENDED_03})