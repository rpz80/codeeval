cmake_minimum_required(VERSION 2.8)
project(ch)

set(SOURCE_FILES main.cpp)

add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
)

add_executable(ch ${SOURCE_FILES})