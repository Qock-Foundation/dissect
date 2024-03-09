cmake_minimum_required(VERSION 3.20)
project(dissect)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address")

add_executable(dissect main.cpp)
