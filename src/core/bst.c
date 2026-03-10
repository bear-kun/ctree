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

void ctree_bst_update(CTreeNodePtr root, const CTreeKey from,
                      const CTreeKey to) {
  const CTreeNodePtr *ptr = bst_find_ptr(&root, from);
  if (*ptr == NULL) return;
  (*ptr)->key = to;
}

static void bst_insert_right_min(const CTreeNodePtr node, const CTreeKey key) {
  CTreeNodePtr *ptr2child = &node->right;
  for (CTreeNodePtr child = *ptr2child; child; child = *ptr2child) {
    ptr2child = &child->left;
  }
  *ptr2child = ctree_new_node_init(key);
}

void ctree_bst_insert(CTreeNodePtr *root, const CTreeKey key) {
  CTreeNodePtr *ptr2child = bst_find_ptr(root, key);
  if (*ptr2child == NULL) {
    *ptr2child = ctree_new_node_init(key);
  } else {
    bst_insert_right_min(*ptr2child, key);
  }
}

void ctree_bst_delete(CTreeNodePtr *root, const CTreeKey key) {
  CTreeNodePtr *ptr2child = bst_find_ptr(root, key);
  const CTreeNodePtr node = *ptr2child;

  if (node == NULL) return;

  if (node->left == NULL) {
    *ptr2child = node->right;
    ctree_delete_node(node);
  } else if (node->right == NULL) {
    *ptr2child = node->left;
    ctree_delete_node(node);
  } else {
    node->key = ctree_bst_delete_min(&node->right);
  }
}

CTreeKey ctree_bst_delete_min(CTreeNodePtr *root) {
  CTreeNodePtr *ptr2child = root;
  CTreeNodePtr child = *ptr2child;
  while (child->left) {
    ptr2child = &child->left;
    child = child->left;
  }

  const CTreeKey key = child->key;
  *ptr2child = child->right;
  ctree_delete_node(child);
  return key;
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