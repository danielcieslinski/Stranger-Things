cmake_minimum_required(VERSION 3.12)
project(Flattop C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Flattop main.c)