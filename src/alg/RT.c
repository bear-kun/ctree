#include "ctree/alg.h"
#include <stdlib.h>

typedef struct {
  CTreeNodePtr tree;
  WeightType *points;
  int npts;
} Package;

static int rt_delete(Package *pkg, const WeightType point, const int left,
                     const int right, const int end) {
  for (int i = 1; i < left; i++) {
    const WeightType dist = point - pkg->points[i];
    if (!ctree_bst_have(pkg->tree, dist)) return i;
    ctree_bst_delete(&pkg->tree, dist);
  }
  for (int i = right + 1; i < end; i++) {
    const WeightType dist = pkg->points[i] - point;
    if (!ctree_bst_have(pkg->tree, dist)) return i;
    ctree_bst_delete(&pkg->tree, dist);
  }
  return end;
}

static void rt_insert(Package *pkg, const WeightType point, int left,
                      const int right, const int end) {
  if (end < left) left = end;

  for (int i = 0; i < left; i++) {
    ctree_bst_insert(&pkg->tree, point - pkg->points[i]);
  }
  for (int i = right + 1; i < end; i++) {
    ctree_bst_insert(&pkg->tree, pkg->points[i]);
  }
}

static int rt_step(Package *pkg, const int left, const int right) {
  if (left > right) return 1;

  int success = 0;
  WeightType max = ctree_bst_delete_max(&pkg->tree);

  int end = rt_delete(pkg, max, left, right, pkg->npts);
  if (end == pkg->npts) {
    pkg->points[right] = max;
    success = rt_step(pkg, left, right - 1);
  }

  if (!success) {
    rt_insert(pkg, max, left, right, end);

    end = rt_delete(pkg, max = pkg->points[pkg->npts - 1] - max, left, right,
                    pkg->npts);
    if (end == pkg->npts) {
      pkg->points[left] = max;
      success = rt_step(pkg, left + 1, right);
    }

    if (!success) {
      rt_insert(pkg, max, left, right, end);
    }
  }

  return success;
}

static CTreeNodePtr build_bst(const WeightType weight[], const int len) {
  CTreeNodePtr tree = NULL;
  for (int i = 0; i < len; i++) ctree_bst_insert(&tree, weight[i]);
  return tree;
}

int reconstruct_turnpike(const WeightType distances[], WeightType points[],
                         const int npts) {
  const int count_dist = npts * (npts - 1) / 2;
  CTreeNodePtr tree = build_bst(distances, count_dist);

  points[0] = 0;
  points[npts - 1] = ctree_bst_delete_max(&tree);
  points[npts - 2] = ctree_bst_delete_max(&tree);

  const WeightType dist = points[npts - 1] - points[npts - 2];
  int success = ctree_bst_have(tree, dist);
  if (success) {
    ctree_bst_delete(&tree, dist);
    Package package = (Package){tree, points, npts};
    success = rt_step(&package, 1, npts - 3);
  }

  ctree_delete_recursive(tree);
  return success;
}