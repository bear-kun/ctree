#ifndef CTREE_TREE_H
#define CTREE_TREE_H

typedef int CTreeKey;
typedef struct CTreeNode_ CTreeNode, *CTreeNodePtr;

struct CTreeNode_ {
  CTreeKey key;
  CTreeNodePtr left, right;
};

CTreeNodePtr ctree_new_node();
void ctree_delete_node(CTreeNodePtr node);

#endif //CTREE_TREE_H