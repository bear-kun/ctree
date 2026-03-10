#include "ctree/alg.h"
#include <stdlib.h>

typedef struct {
  int size;
  CTreeNodePtr elems[0];
} Heap;

static Heap *new_heap(const int capacity) {
  Heap *heap = malloc(sizeof(Heap) + (capacity + 1) * sizeof(CTreeNodePtr));
  heap->size = 0;
  return heap;
}

static void delete_heap(Heap *heap) { free(heap); }

static void heapify_down(Heap *heap, int top) {
  const CTreeNodePtr topValue = heap->elems[top];

  for (int child; (child = top << 1) <= heap->size; top = child) {
    if (child != heap->size &&
        heap->elems[child + 1]->key < heap->elems[child]->key) {
      ++child;
    }
    if (heap->elems[child]->key < topValue->key) {
      heap->elems[top] = heap->elems[child];
    } else {
      break;
    }
  }
  heap->elems[top] = topValue;
}

static CTreeNodePtr *heap_top(Heap *heap) { return heap->elems + 1; }

static CTreeNodePtr heap_pop(Heap *heap) {
  const CTreeNodePtr ret = heap->elems[1];
  heap->elems[1] = heap->elems[heap->size--];
  heapify_down(heap, 1);
  return ret;
}

static void heap_pre_build(Heap *heap, const CTreeNodePtr elem) {
  heap->elems[++heap->size] = elem;
}

static void heap_build(Heap *heap) {
  for (int i = heap->size >> 1; i; --i) heapify_down(heap, i);
}

typedef CTreeKey WeightType;

/*
 * minimize(sum(deep[i] * weight[i]))，仅树叶
 */
CTreeNodePtr huffman_code(const WeightType weight[], const int count) {
  if (count == 1) return ctree_new_node_init(weight[0]);

  Heap *heap = new_heap(count);
  for (int i = 0; i < count; ++i) {
    heap_pre_build(heap, ctree_new_node_init(weight[i]));
  }
  heap_build(heap);

  CTreeNodePtr root = NULL;
  while (heap->size > 1) {
    // 弹出heap的top2，作为root的left/right，然后将root重新入堆，root的weight为left、right之和
    // 这里合并了弹出right和插入root
    const CTreeNodePtr first = heap_pop(heap);
    CTreeNodePtr *second = heap_top(heap);

    root = ctree_new_node();
    root->key = first->key + (*second)->key;
    root->left = first;
    root->right = *second;

    *second = root;
    heapify_down(heap, 1);
  }

  delete_heap(heap);
  return root;
}