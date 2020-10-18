cmake_minimum_required(VERSION 3.15)
project(rw C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(rw main.c)