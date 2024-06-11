cmake_minimum_required(VERSION 3.5)
project(dissect)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address")

include(cmake/kissat.cmake)
include(cmake/glpk.cmake)

add_executable(dissect-sat dissect-sat.cpp)
add_dependencies(dissect-sat KissatBuild)

add_executable(dissect-linprog dissect-linprog.cpp)
add_dependencies(dissect-linprog KissatBuild)
target_link_libraries(dissect-linprog PRIVATE LibGLPK)

add_executable(fixed-isometries fixed-isometries.cpp)
add_dependencies(fixed-isometries KissatBuild)
target_link_libraries(fixed-isometries PRIVATE LibGLPK)
