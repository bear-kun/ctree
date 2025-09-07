#include <stdlib.h>
#include <tree.h>

TreeNodePtr treeNewNode(const DataType data) {
  const TreeNodePtr node = malloc(sizeof(TreeNode));
  node->next = NULL;
  node->left = node->right = NULL;
  node->data = data;
  return node;
}

void treeInsertData(TreeNodePtr *const node2child, const DataType data) {
  const TreeNodePtr node = malloc(sizeof(TreeNode));
  node->data = data;
  node->left = node->right = NULL;
  treeInsertNode(node2child, node);
}

void treeDestroy(TreeNodePtr root) {
  while (root) {
    treeDestroy(root->left);
    treeDestroy(root->right);
    const TreeNodePtr node = root;
    root = root->next;
    free(node);
  }
}
