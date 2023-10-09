cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(TachyonEngine VERSION 1.1)

find_package(GTest REQUIRED)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)