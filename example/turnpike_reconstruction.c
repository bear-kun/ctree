#include "binary_search_tree.h"
#include <stdio.h>

#define STACK_ELEM_TYPE TreeNodePtr
#include "stack.h"

typedef int DistanceType;
typedef struct {
  TreeNodePtr *tree;
  DistanceType *points;
  Stack *stack;
  int npts;
} Package;

static int RT_Delete(TreeNodePtr *tree, Stack *stack, const DistanceType points[],
              const DistanceType point, const int left, const int right,
              const int end) {
  int i;
  for (i = 0; i < left; i++) {
    const TreeNodePtr node = bstUnlinkWithData(tree, point - points[i]);
    if (node == NULL) return i;
    stackPush(stack, node);
  }
  for (i = right + 1; i < end; i++) {
    const TreeNodePtr node = bstUnlinkWithData(tree, points[i] - point);
    if (node == NULL) return i;
    stackPush(stack, node);
  }
  return end;
}

static void RT_Insert(TreeNodePtr *tree, Stack *stack, int left, const int right,
               const int end) {
  int i;
  if (end < left) left = end;

  for (i = right + 1; i < end; i++) bstInsertNode(tree, stackPop(stack));
  for (i = 0; i < left; i++) bstInsertNode(tree, stackPop(stack));
}

static int RT_step(Package *pkg, const int left, const int right) {
  int success = 0, end;
  if (left > right) return 1;
  DistanceType max = (*bstFindMax(pkg->tree))->data;

  if (pkg->npts == (end = RT_Delete(pkg->tree, pkg->stack, pkg->points, max,
                                    left, right, pkg->npts))) {
    pkg->points[right] = max;
    success = RT_step(pkg, left, right - 1);
  }

  if (!success) {
    RT_Insert(pkg->tree, pkg->stack, left, right, end);

    if (pkg->npts == (end = RT_Delete(pkg->tree, pkg->stack, pkg->points,
                                      max = pkg->points[pkg->npts - 1] - max,
                                      left, right, pkg->npts))) {
      pkg->points[left] = max;
      success = RT_step(pkg, left + 1, right);
    }

    if (!success) {
      RT_Insert(pkg->tree, pkg->stack, left, right, end);
    }
  }

  return success;
}

static TreeNodePtr buildBST(const DataType data[], const int len,
                            TreeNode *const buffer) {
  TreeNodePtr tree = NULL;
  if (buffer == NULL) return NULL;

  for (int i = 0; i < len; i++) {
    buffer[i].data = data[i];
    bstInsertNode(&tree, buffer + i);
  }

  return tree;
}

void reconstructTurnpike(DistanceType distances[], DistanceType points[],
                         const int npts) {
  const int numOfDistances = npts * (npts - 1) / 2;
  const TreeNodePtr buffer = calloc(numOfDistances, sizeof(TreeNode));
  TreeNodePtr tree = buildBST(distances, numOfDistances, buffer);
  Stack *stack = stackCreate(numOfDistances);
  Package package = (Package){&tree, points, stack, npts};

  points[0] = 0;
  points[npts - 1] = bstUnlinkMax(&tree)->data; // buffer，不用free单个节点
  points[npts - 2] = bstUnlinkMax(&tree)->data;

  if (bstUnlinkWithData(&tree, points[npts - 1] - points[npts - 2]) != NULL) {
    if (RT_step(&package, 1, npts - 3)) {
      puts("reconstruct successfully!");
      goto END;
    }
  }

  puts("reconstruct failed!");
END:
  stackRelease(stack);
  free(buffer);
  free(tree);
}

int main() {
  int npts;
  DistanceType dist[45], pts[10];

  printf("input npts:");
  scanf("%d", &npts);

  const int numOfDists = npts * (npts - 1) / 2;
  printf("input distance:");
  for (int i = 0; i < numOfDists; i++) scanf("%d", dist + i);

  reconstructTurnpike(dist, pts, npts);
  printf("turnpike points:");
  for (int i = 0; i < npts; i++) printf("%d ", pts[i]);

  return 0;
}
