cmake_minimum_required(VERSION 3.1)
project(cp3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")

set(SOURCE_FILES main.c)
add_executable(cp3 ${SOURCE_FILES} main.c)