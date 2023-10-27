set(BINARY ${CMAKE_PROJECT_NAME})

find_package(OpenCL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB SOURCES LIST_DIRECTORIES true  *.cpp *.c)
file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

add_executable(${BINARY}_run ${SOURCES})

set_property(TARGET ${BINARY}_run PROPERTY CXX_STANDARD 17)

add_library(${BINARY}_lib SHARED ${SOURCES} ${HEADER_FILES} ${SOURCE_FILES})

include_directories(${OpenCL_INCLUDE_DIRS})
include_directories(${glfw3_INCLUE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(${BINARY}_lib ${OpenCL_LIBRARIES})
target_link_libraries(${BINARY}_lib glfw)
target_link_libraries(${BINARY}_lib OpenGL::GL)
target_link_libraries(${BINARY}_lib GLEW::GLEW)
target_link_libraries(${BINARY}_run ${BINARY}_lib)
