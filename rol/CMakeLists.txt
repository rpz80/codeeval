cmake_minimum_required(VERSION 3.0)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

project(ray_of_light)    

set(SOURCE_FILES main.c)
add_executable(ray_of_light ${SOURCE_FILES})