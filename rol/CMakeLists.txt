cmake_minimum_required(VERSION 2.8)
project(ray_of_light)

if (MSVC)
  add_definitions(
	  -D_CRT_SECURE_NO_WARNINGS
  )
endif()

set(SOURCE_FILES main.cpp)
add_executable(ray_of_light ${SOURCE_FILES})