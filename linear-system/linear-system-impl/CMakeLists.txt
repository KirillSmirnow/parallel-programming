cmake_minimum_required(VERSION 3.15)
project(linear_system)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER /usr/bin/mpic++)

include_directories(/usr/include/mpi)

add_executable(linear_system main.cpp)
