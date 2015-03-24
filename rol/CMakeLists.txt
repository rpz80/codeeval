cmake_minimum_required(VERSION 3.1)
project(ray_of_light)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c)
add_executable(ray_of_light ${SOURCE_FILES})