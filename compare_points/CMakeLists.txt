cmake_minimum_required(VERSION 2.8)
project(cp)

set(SOURCE_FILES main.cpp)

add_definitions(
    -D_CRT_SECURE_NO_WARNINGS
)

add_executable(
	cp 
	${SOURCE_FILES}
)