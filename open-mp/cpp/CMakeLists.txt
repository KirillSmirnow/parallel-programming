cmake_minimum_required(VERSION 3.15)
project(open_mp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(open_mp Main.cpp)