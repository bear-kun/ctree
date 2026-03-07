#include "tree.h"
#include <stddef.h>

static CTreeNodePtr *bst_find_ptr(CTreeNodePtr *root, const CTreeKey key) {
  CTreeNodePtr *ptr2child = root;
  for (CTreeNodePtr node = *ptr2child; node; node = *ptr2child) {
    if (key < node->key) {
      ptr2child = &node->left;
    } else if (key > node->key) {
      ptr2child = &node->right;
    } else {
      break;
    }
  }
  return ptr2child;
}

int ctree_bst_have(CTreeNodePtr root, const CTreeKey key) {
  return *bst_find_ptr(&root, key) != NULL;
}

void ctree_bst_insert(CTreeNodePtr *root, const CTreeKey key) {
  CTreeNodePtr *ptr2child = bst_find_ptr(root, key);
  const CTreeNodePtr new_node = ctree_new_node_init(key);

  if (*ptr2child == NULL) {
    *ptr2child = new_node;
  } else {
    const CTreeNodePtr parent = *ptr2child;
    new_node->right = parent->right;
    parent->right = new_node;
  }
}

static CTreeNodePtr unlink_right_min(const CTreeNodePtr node) {
  CTreeNodePtr *ptr2child = &node->right;
  CTreeNodePtr child = *ptr2child;
  while (child->left) {
    ptr2child = &child->left;
    child = child->left;
  }

  *ptr2child = child->right;
  return child;
}

void ctree_bst_delete(CTreeNodePtr *root, const CTreeKey key) {
  CTreeNodePtr *ptr2child = bst_find_ptr(root, key);
  const CTreeNodePtr node = *ptr2child;

  if (node->left == NULL) {
    *ptr2child = node->right;
  } else if (node->right == NULL) {
    *ptr2child = node->left;
  } else {
    const CTreeNodePtr replace = unlink_right_min(node);
    replace->left = node->left;
    replace->right = node->right;
    *ptr2child = replace;
  }
  ctree_delete_node(node);
}

CTreeKey ctree_bst_delete_max(CTreeNodePtr *root) {
  CTreeNodePtr *ptr2child = root;
  CTreeNodePtr child = *ptr2child;
  while (child->right) {
    ptr2child = &child->right;
    child = *ptr2child;
  }

  const CTreeKey key = child->key;
  *ptr2child = child->left;
  ctree_delete_node(child);
  return key;
}