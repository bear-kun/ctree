cmake_minimum_required(VERSION 3.10)
project(ctree)

set(CMAKE_C_STANDARD 11)


file(GLOB SOURCES "src/*.c")
add_library(bst STATIC ${SOURCES})
target_include_directories(bst PRIVATE include)
target_include_directories(bst PRIVATE include/public)

add_executable(example example/turnpike_reconstruction.c)
target_include_directories(example PRIVATE include/public)
target_link_libraries(example PRIVATE bst)