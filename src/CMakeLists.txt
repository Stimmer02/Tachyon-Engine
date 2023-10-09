set(BINARY ${CMAKE_PROJECT_NAME})

find_package(OpenCL REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.h *.cpp *.c)

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${glfw3_INCLUE_DIRS})

target_link_libraries(${BINARY}_lib ${OpenCL_LIBRARIES})
target_link_libraries(${BINARY}_lib glfw)
target_link_libraries(${BINARY}_run ${BINARY}_lib)
