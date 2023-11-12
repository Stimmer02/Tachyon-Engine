find_package(OpenCL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB_RECURSE HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.h"
)

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
)

# message(STATUS "Headers:")
# foreach(header ${HEADERS})
#     message(STATUS "  ${header}")
# endforeach()

# message(STATUS "Sources:")
# foreach(source ${SOURCES})
#     message(STATUS "  ${source}")
# endforeach()

add_library(${LIB_NAME}_lib SHARED ${SOURCES} ${HEADERS})

set_property(TARGET ${LIB_NAME}_lib PROPERTY CXX_STANDARD 17)

target_include_directories(${LIB_NAME}_lib PUBLIC
    ${OpenCL_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${glfw3_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/GUIRenderer  
    ${CMAKE_CURRENT_SOURCE_DIR}/PhysicsProcessor 
)

target_link_libraries(${LIB_NAME}_lib 
    ${OpenCL_LIBRARIES}
    OpenGL::GL 
    glfw 
    GLEW::GLEW
)

add_executable(${LIB_NAME}_run main.cpp)

