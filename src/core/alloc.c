#include "tree.h"
#include <stdint.h>
#include <stdlib.h>

#define BLOCK_MAX_CAPACITY 258
#define BLOCK_ELEMS_CAPACITY 128
#define BLOCK_ALIGN 4096

typedef struct {
  int size;
  uint64_t bitmap[BLOCK_ELEMS_CAPACITY / 64];
  CTreeNode elements[BLOCK_ELEMS_CAPACITY];
} Block;

static struct {
  int block_count;
  Block *blocks[BLOCK_MAX_CAPACITY];
} allocator = {0};

#if defined(__GNUC__) || defined(__clang__)
static int ctz(const uint64_t x) {
  return __builtin_ctzll(x);
}
#elif defined(_MSC_VER)
#include <intrin.h>

static int ctz(const uint64_t x) {
  unsigned long n;
  _BitScanForward64(&n, x);
  return (int)n;
}
#endif

static Block *new_block() {
  Block *blk = _aligned_malloc(sizeof(Block), BLOCK_ALIGN);
  blk->size = 0;
  for (int i = 0; i < BLOCK_ELEMS_CAPACITY / 64; i++) blk->bitmap[i] = 0;
  return blk;
}

static void block_swap_head(const int idx) {
  Block *head = allocator.blocks[0];
  allocator.blocks[0] = allocator.blocks[idx];
  allocator.blocks[idx] = head;
}

static CTreeNodePtr block_alloc(Block *blk) {
  for (int i = 0; i < BLOCK_ELEMS_CAPACITY / 64; ++i) {
    const uint64_t x = blk->bitmap[i];
    if (x + 1 == 0) continue;

    const int idx = ctz(~x);
    blk->bitmap[i] |= 1ull << idx;
    blk->size++;
    return blk->elements + i * 64 + idx;
  }
  return NULL;
}

static void block_delete(Block *blk, const int idx) {
  blk->bitmap[idx >> 6] ^= 1ull << (idx & 63);
  blk->size--;
}

CTreeNodePtr ctree_new_node() {
  for (int i = 0; i < allocator.block_count; i++) {
    Block *blk = allocator.blocks[i];
    if (blk->size == BLOCK_ELEMS_CAPACITY) continue;

    block_swap_head(i);
    return block_alloc(blk);
  }

  Block *new_blk = new_block();
  allocator.blocks[allocator.block_count] = new_blk;
  block_swap_head(allocator.block_count++);
  return block_alloc(new_blk);
}

CTreeNodePtr ctree_new_node_init(const CTreeKey key) {
  const CTreeNodePtr node = ctree_new_node();
  node->key = key;
  node->height = 0;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void ctree_delete_node(const CTreeNodePtr node) {
  if (node == NULL) return;
  Block *blk = (Block *)((uint64_t)node & ~(BLOCK_ALIGN - 1));
  block_delete(blk, (int)(node - blk->elements));
}

void ctree_delete_recursive(const CTreeNodePtr node) {
  if (node == NULL) return;
  ctree_delete_recursive(node->left);
  ctree_delete_recursive(node->right);
  ctree_delete_node(node);
}