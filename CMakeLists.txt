cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(TestApp VERSION 1.1)

# Include Google Test using ExternalProject
include(ExternalProject)

ExternalProject_Add(
    googletest
    PREFIX ${CMAKE_BINARY_DIR}/googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0 
)

ExternalProject_Get_Property(googletest SOURCE_DIR)
set(GTEST_INCLUDE_DIRS ${SOURCE_DIR}/googletest/include)
set(GTEST_LIBRARIES_DIR ${CMAKE_BINARY_DIR}/googletest/lib)

add_library(gtest UNKNOWN IMPORTED)
set_target_properties(gtest PROPERTIES
    IMPORTED_LOCATION ${GTEST_LIBRARIES_DIR}/libgtest.a
    INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIRS}
)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
