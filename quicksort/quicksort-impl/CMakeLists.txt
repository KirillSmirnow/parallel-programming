cmake_minimum_required(VERSION 3.15)
project(quicksort)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER /usr/bin/mpic++)

include_directories(/usr/include/mpi)

add_executable(quicksort main.cpp)
