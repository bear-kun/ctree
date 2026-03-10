add_library(ctree_core STATIC
        core/alloc.c
        core/bst.c
        core/avl.c
)

add_library(ctree_alg STATIC
        alg/Huffman.c
        alg/OBST.c
        alg/RT.c
)