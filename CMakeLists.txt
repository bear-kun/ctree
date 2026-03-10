cmake_minimum_required(VERSION 3.13)
project(ctree)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src)

target_include_directories(ctree_core PRIVATE include/ctree)
target_include_directories(ctree_alg PUBLIC include)
target_link_libraries(ctree_alg PUBLIC ctree_core)