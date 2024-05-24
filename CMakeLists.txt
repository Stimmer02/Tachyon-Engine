cmake_minimum_required(VERSION 3.21)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(TachyonEngine VERSION 1.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D DEBUG -msse")

if(WIN32)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lopengl32 -lglfw3 -lglew32 ")

endif()

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIB_NAME ${PROJECT_NAME})

add_subdirectory(src)

target_include_directories(${LIB_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(tests)

add_subdirectory(editor)

add_dependencies(${LIB_NAME}_run ${LIB_NAME}_lib)

target_compile_definitions(${LIB_NAME}_run PRIVATE PROJECT_ROOT_DIR="${PROJECT_ROOT_DIR}")

target_link_libraries(${LIB_NAME}_run ${LIB_NAME}_lib)
