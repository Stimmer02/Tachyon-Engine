cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

project(TachyonEngine VERSION 1.1)

include(ExternalProject)

set(GTEST_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/lib/googletest" CACHE PATH "Installation directory for Google Test")

ExternalProject_Add(
    googletest
    PREFIX "${CMAKE_BINARY_DIR}/googletest"
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0 
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_PREFIX}
)

add_library(gtest INTERFACE IMPORTED GLOBAL)
add_dependencies(gtest googletest)

target_include_directories(gtest INTERFACE ${GTEST_INSTALL_PREFIX}/include)
target_link_libraries(gtest INTERFACE ${GTEST_INSTALL_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})

add_library(gtest_main INTERFACE IMPORTED GLOBAL)
add_dependencies(gtest_main googletest)
target_link_libraries(gtest_main INTERFACE ${GTEST_INSTALL_PREFIX}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})


include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)