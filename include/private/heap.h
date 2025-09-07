#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>

#ifndef HEAP_ELEM_TYPE
#define HEAP_ELEM_TYPE int
#endif

#ifndef HEAP_LESS_THAN
#define HEAP_LESS_THAN(x, y) ((x) < (y))
#endif

typedef HEAP_ELEM_TYPE HeapElemType;
typedef struct {
  int capacity, size;
  HeapElemType elems[1];
  // CGraphId dummy;
} Heap;

static Heap *heapCreate(const int capacity) {
  Heap *heap = malloc(sizeof(Heap) + capacity * sizeof(HeapElemType));
  heap->capacity = capacity;
  heap->size = 0;
  return heap;
}

static void heapRelease(Heap *heap) { free(heap); }

static inline int heapEmpty(const Heap *const heap) { return heap->size == 0; }

static inline HeapElemType *heapTop(Heap *const heap) {
  return heap->elems + 1;
}

static void heapPush(Heap *heap, const HeapElemType elem) {
  int child = ++heap->size;
  for (int father;
       ((father = child >> 1)) && HEAP_LESS_THAN(elem, heap->elems[father]);
       child = father) {
    heap->elems[child] = heap->elems[father];
  }
  heap->elems[child] = elem;
}

static void heapifyDown(Heap *heap, int father) {
  const HeapElemType topValue = heap->elems[father];

  for (int child; (child = father << 1) <= heap->size; father = child) {
    if (child != heap->size &&
        HEAP_LESS_THAN(heap->elems[child + 1], heap->elems[child]))
      ++child;

    if (HEAP_LESS_THAN(heap->elems[child], topValue))
      heap->elems[father] = heap->elems[child];
    else
      break;
  }
  heap->elems[father] = topValue;
}

static HeapElemType heapPop(Heap *heap) {
  const HeapElemType ret = heap->elems[1];
  heap->elems[1] = heap->elems[heap->size--];
  heapifyDown(heap, 1);
  return ret;
}

static void heapPreBuild(Heap *heap, const HeapElemType item) {
  heap->elems[++heap->size] = item;
}

static void heapBuild(Heap *heap) {
  for (int i = heap->size >> 1; i; --i) heapifyDown(heap, i);
}

#endif // HEAP_H
