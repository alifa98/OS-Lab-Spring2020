cmake_minimum_required(VERSION 3.15)
project(lab1p C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
add_executable(lab1p main.c)