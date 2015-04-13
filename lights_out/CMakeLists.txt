cmake_minimum_required(VERSION 3.1)
project(lights_out)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")

set(SOURCE_FILES main.c)
add_executable(lights_out ${SOURCE_FILES} main.c)