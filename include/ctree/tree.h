#ifndef CTREE_TREE_H
#define CTREE_TREE_H

typedef int CTreeKey;
typedef int CTreeInt;
typedef struct CTreeNode_ CTreeNode, *CTreeNodePtr;

struct CTreeNode_ {
  CTreeKey key;
  CTreeInt height; // AVL balance factor
  CTreeNodePtr left, right;
};

CTreeNodePtr ctree_new_node();
CTreeNodePtr ctree_new_node_init(CTreeKey key);
void ctree_delete_node(CTreeNodePtr node);
void ctree_delete_recursive(CTreeNodePtr node);

int ctree_bst_have(CTreeNodePtr root, CTreeKey key);
void ctree_bst_update(CTreeNodePtr root, CTreeKey from, CTreeKey to);

void ctree_bst_insert(CTreeNodePtr *root, CTreeKey key);
void ctree_bst_delete(CTreeNodePtr *root, CTreeKey key);
CTreeKey ctree_bst_delete_max(CTreeNodePtr *root);
CTreeKey ctree_bst_delete_min(CTreeNodePtr *root);

void ctree_avl_insert(CTreeNodePtr *root, CTreeKey key);
void ctree_avl_delete(CTreeNodePtr *root, CTreeKey key);
CTreeKey ctree_avl_delete_min(CTreeNodePtr *root);
CTreeKey ctree_avl_delete_max(CTreeNodePtr *root);

#endif //CTREE_TREE_H