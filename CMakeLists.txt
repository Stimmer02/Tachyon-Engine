cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(TestApp VERSION 1.1)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(lib/googletest)