cmake_minimum_required(VERSION 3.0)
project(dissect)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address")

include(cmake/kissat.cmake)

add_executable(dissect-sat dissect-sat.cpp)
add_dependencies(dissect-sat KissatBuild)
