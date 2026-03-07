#ifndef CTREE_ALG_H
#define CTREE_ALG_H

#include "tree.h"

typedef CTreeKey WeightType;

CTreeNodePtr huffman_code(const WeightType weight[], int count);
CTreeNodePtr optimal_bst(const WeightType weight[], int count);
int reconstruct_turnpike(const WeightType distances[], WeightType points[],
                         int npts);

#endif //CTREE_ALG_H