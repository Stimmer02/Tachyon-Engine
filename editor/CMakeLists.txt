set(BINARY ${CMAKE_PROJECT_NAME}_editor)

file(GLOB_RECURSE EDITOR_SOURCES LIST_DIRECTORIES false *.hpp *.h *.cpp *.c)

set(SOURCES ${EDITOR_SOURCES})

add_executable(${BINARY} ${EDITOR_SOURCES})

set_property(TARGET ${BINARY} PROPERTY CXX_STANDARD 17)

target_link_libraries(${BINARY} ${LIB_NAME}_lib)