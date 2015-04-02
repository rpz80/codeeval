cmake_minimum_required(VERSION 2.8)
project(bn)

set(SOURCE_FILES bn.cpp)

add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
)

add_executable(bn ${SOURCE_FILES})