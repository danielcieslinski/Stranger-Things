cmake_minimum_required(VERSION 3.12)
project(Beehive C)

set(CMAKE_C_STANDARD 90)

add_executable(Beehive main.c Utilities.c Utilities.h)