#include <tree_alg.h>

typedef struct HeapData_ {
  TreeNodePtr node;
  WeightType weight;
} HeapData;

#define HEAP_ELEM_TYPE HeapData
#define HEAP_LESS_THAN(a, b) (a.weight < b.weight)
#include "private/heap.h"

#define NO_DATA (-1)

/*
 * minimize(deep[i] * weight[i])，且数据都在树叶
 */
TreeNodePtr HuffmanCode(const DataType data[], const WeightType weight[],
                        const int number) {
  if (number == 1) return treeNewNode(*data);

  Heap *heap = heapCreate(number);
  for (int i = 0; i < number; ++i) {
    heapPreBuild(heap, (HeapData){treeNewNode(data[i]), weight[i]});
  }
  heapBuild(heap);

  TreeNodePtr root = NULL;

  while (heap->size > 1) {
    // 弹出heap的top2，作为root的left/right，然后将root重新入堆，root的weight为left、right之和
    // 这里合并了弹出right和插入root
    const HeapData first = heapPop(heap);
    HeapData *second = heapTop(heap);

    root = treeNewNode(NO_DATA);
    root->left = first.node;
    root->right = second->node;

    second->weight += first.weight;
    second->node = root;
    heapifyDown(heap, 1);
  }

  heapRelease(heap);
  return root;
}