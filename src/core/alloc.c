#include "tree.h"
#include <stdlib.h>

#define BLOCK_MAX_CAPACITY 256
#define BLOCK_ELEMS_CAPACITY 256
#define BLOCK_ALIGN 4096

typedef struct {
  int size;
  int free_head;
  int free_next[BLOCK_ELEMS_CAPACITY];
  CTreeNode elements[BLOCK_ELEMS_CAPACITY];
} Block;

static struct {
  int block_count;
  Block *blocks[BLOCK_MAX_CAPACITY];
} allocator = {0};

static Block *new_block() {
  Block *blk = _aligned_malloc(sizeof(Block), BLOCK_ALIGN);
  blk->size = 0;
  blk->free_head = 0;
  for (int i = 0; i < BLOCK_ELEMS_CAPACITY; i++) {
    blk->free_next[i] = i + 1;
  }
  return blk;
}

static CTreeNodePtr block_alloc(Block *blk) {
  const int idx = blk->free_head;
  blk->free_head = blk->free_next[idx];
  blk->size++;
  return blk->elements + idx;
}

static void block_delete(Block *blk, const int idx) {
  blk->free_next[idx] = blk->free_head;
  blk->free_head = idx;
  blk->size--;
}

CTreeNodePtr ctree_new_node() {
  for (size_t i = 0; i < allocator.block_count; i++) {
    Block *blk = allocator.blocks[i];
    if (blk->size == BLOCK_ELEMS_CAPACITY) continue;

    Block *head = allocator.blocks[0];
    allocator.blocks[0] = blk;
    allocator.blocks[i] = head;
    return block_alloc(blk);
  }

  Block *new_blk = new_block();
  allocator.blocks[allocator.block_count++] = allocator.blocks[0];
  allocator.blocks[0] = new_blk;
  return block_alloc(new_blk);
}

void ctree_delete_node(const CTreeNodePtr node) {
  Block *blk = (Block *)((long long)node & ~(BLOCK_ALIGN - 1));
  block_delete(blk, (int)(node - blk->elements));
}