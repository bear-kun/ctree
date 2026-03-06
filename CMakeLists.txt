cmake_minimum_required(VERSION 3.10)
project(ctree)

set(CMAKE_C_STANDARD 11)

add_library(ctree_core STATIC
        src/core/alloc.c
)

target_include_directories(ctree_core PUBLIC include/ctree)