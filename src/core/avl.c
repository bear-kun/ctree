#include "tree.h"
#include <stddef.h>

static CTreeInt avl_height(const CTreeNodePtr node) {
  const CTreeInt lh = node->left ? node->left->height : -1;
  const CTreeInt rh = node->right ? node->right->height : -1;
  return (lh > rh ? lh : rh) + 1;
}

static CTreeInt avl_bf(const CTreeNodePtr node) {
  const CTreeInt lh = node->left ? node->left->height : -1;
  const CTreeInt rh = node->right ? node->right->height : -1;
  return lh - rh;
}

static void rotate_right(CTreeNodePtr *root) {
  const CTreeNodePtr node = *root;
  const CTreeNodePtr left = node->left;
  node->left = left->right;
  left->right = node;
  *root = left;
  node->height = avl_height(node);
  left->height = avl_height(left);
}

static void rotate_left(CTreeNodePtr *root) {
  const CTreeNodePtr node = *root;
  const CTreeNodePtr right = node->right;
  node->right = right->left;
  right->left = node;
  *root = right;
  node->height = avl_height(node);
  right->height = avl_height(right);
}

static int avl_fix_balance_left_i(CTreeNodePtr *root) {
  const CTreeNodePtr node = *root;
  const CTreeInt bf = avl_bf(node);
  if (bf == 0) return 1;
  if (bf == 2) {
    if (avl_bf(node->left) == -1) rotate_left(&node->left);
    rotate_right(root);
    return 1;
  }
  node->height++;
  return 0;
}

static int avl_fix_balance_right_i(CTreeNodePtr *root) {
  const CTreeNodePtr node = *root;
  const CTreeInt bf = avl_bf(node);
  if (bf == 0) return 1;
  if (bf == -2) {
    if (avl_bf(node->right) == 1) rotate_right(&node->right);
    rotate_left(root);
    return 1;
  }
  node->height++;
  return 0;
}

static int avl_insert(CTreeNodePtr *root, const CTreeKey key) {
  const CTreeNodePtr node = *root;
  if (node == NULL) {
    *root = ctree_new_node_init(key);
    return 0;
  }

  if (key < node->key) {
    if (avl_insert(&node->left, key)) return 1;
    return avl_fix_balance_left_i(root);
  }

  // key >= node->key
  if (avl_insert(&node->right, key)) return 1;
  return avl_fix_balance_right_i(root);
}

static int avl_fix_balance_left_d(CTreeNodePtr *root) {
  const CTreeNodePtr node = *root;
  const CTreeInt bf = avl_bf(node);
  if (bf == -1) return 1;
  if (bf == -2) {
    const CTreeInt old_h = node->height;
    if (avl_bf(node->right) == 1) rotate_right(&node->right);
    rotate_left(root);
    return (*root)->height == old_h;
  }
  node->height--;
  return 0;
}

static int avl_fix_balance_right_d(CTreeNodePtr *root) {
  const CTreeNodePtr node = *root;
  const CTreeInt bf = avl_bf(node);
  if (bf == 1) return 1;
  if (bf == 2) {
    const CTreeInt old_h = node->height;
    if (avl_bf(node->left) == -1) rotate_left(&node->left);
    rotate_right(root);
    return (*root)->height == old_h;
  }
  node->height--;
  return 0;
}

static int avl_delete_min(CTreeNodePtr *root, CTreeKey *key) {
  const CTreeNodePtr node = *root;
  if (node->left == NULL) {
    *root = node->right;
    *key = node->key;
    ctree_delete_node(node);
    return 0;
  }

  if (avl_delete_min(&node->left, key)) return 1;
  return avl_fix_balance_left_d(root);
}

static int avl_delete_max(CTreeNodePtr *root, CTreeKey *key) {
  const CTreeNodePtr node = *root;
  if (node->right == NULL) {
    *root = node->left;
    *key = node->key;
    ctree_delete_node(node);
    return 0;
  }

  if (avl_delete_max(&node->right, key)) return 1;
  return avl_fix_balance_right_d(root);
}

static int avl_delete(CTreeNodePtr *root, const CTreeKey key) {
  const CTreeNodePtr node = *root;
  if (node == NULL) return 1; // no to delete

  if (key < node->key) {
    if (avl_delete(&node->left, key)) return 1;
    return avl_fix_balance_left_d(root);
  }

  if (key > node->key) {
    if (avl_delete(&node->right, key)) return 1;
  } else {
    // key == node->key
    if (node->left == NULL) {
      *root = node->right;
      ctree_delete_node(node);
      return 0;
    }
    if (node->right == NULL) {
      *root = node->left;
      ctree_delete_node(node);
      return 0;
    }
    if (avl_delete_min(&node->right, &node->key)) return 1;
  }
  return avl_fix_balance_right_d(root);
}

void ctree_avl_insert(CTreeNodePtr *root, const CTreeKey key) {
  avl_insert(root, key);
}

void ctree_avl_delete(CTreeNodePtr *root, const CTreeKey key) {
  avl_delete(root, key);
}

CTreeKey ctree_avl_delete_min(CTreeNodePtr *root) {
  CTreeInt key;
  avl_delete_min(root, &key);
  return key;
}

CTreeKey ctree_avl_delete_max(CTreeNodePtr *root) {
  CTreeInt key;
  avl_delete_max(root, &key);
  return key;
}