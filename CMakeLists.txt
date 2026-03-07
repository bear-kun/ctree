cmake_minimum_required(VERSION 3.10)
project(ctree)

set(CMAKE_C_STANDARD 11)

add_library(ctree_core STATIC
        src/core/alloc.c
        src/core/bst.c
)

add_library(ctree_alg STATIC
        src/alg/Huffman.c
        src/alg/OBST.c
        src/alg/RT.c
)

target_include_directories(ctree_core PRIVATE include/ctree)
target_include_directories(ctree_alg PUBLIC include)
target_link_libraries(ctree_alg PUBLIC ctree_core)