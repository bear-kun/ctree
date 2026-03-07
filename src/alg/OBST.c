#include "ctree/alg.h"
#include <stdlib.h>

typedef struct {
  int left, right;
} LeftRight;

typedef struct {
  int capacity, size;
  int front, rear;
  LeftRight elems[0];
} Queue;

static Queue *new_queue(const int capacity) {
  Queue *queue = malloc(sizeof(Queue) + capacity * sizeof(LeftRight));
  queue->capacity = capacity;
  queue->size = queue->front = queue->rear = 0;
  return queue;
}

static void delete_queue(Queue *queue) { free(queue); }

static int queue_empty(const Queue *queue) { return queue->size == 0; }

static void queue_push(Queue *queue, const LeftRight item) {
  queue->elems[queue->front] = item;
  if (++queue->front == queue->capacity) queue->front = 0;
  ++queue->size;
}

static LeftRight queue_pop(Queue *queue) {
  const LeftRight item = queue->elems[queue->rear];
  if (++queue->rear == queue->capacity) queue->rear = 0;
  --queue->size;
  return item;
}

typedef struct {
  WeightType sub; // left~right 内的 OBST 的 sum(d*w)
  WeightType sum; // sum(w[i]) left <= i <= right
  int root;       // left!right 内的 OBST 的 root
} DpData;

/*
 * 最优二叉搜索树
 * minimize(sum(deep[i] * weight[i]))
 */

// 输入data应该升序排列，以满足搜索二叉树的性质
CTreeNodePtr optimal_bst(const WeightType weight[],const int count) {
  DpData **dp = malloc(count * sizeof(DpData *) +
                       count * (count + 1) / 2 * sizeof(DpData));
  DpData *array = (DpData *)(dp + count);

  /*
   *          00  01  02 ...
   *      ->  11  12  13 ...
   *  ->  -   22  23  24 ...
   *  -   -   ...
   */
  for (int i = 0, j = 0, k = count; i != count; ++i) {
    array[j].sub = weight[i];
    array[j].sum = weight[i];
    array[j].root = i;
    dp[i] = array + j - i;
    j += k--;
  }

  for (int width = 1; width < count; width++) {
    for (int left = 0, right; (right = width + left) < count; left++) {
      dp[left][right].sum = weight[left] + dp[left + 1][right].sum;

      WeightType min; // min(left-sub-OBST + right-sub-OBST)
      if (dp[left + 1][right].sub < dp[left][right - 1].sub) {
        min = dp[left + 1][right].sub;
        dp[left][right].root = left;
      } else {
        min = dp[left][right - 1].sub;
        dp[left][right].root = right;
      }

      for (int root = left + 1; root != right; ++root) {
        if (dp[left][root - 1].sub + dp[root + 1][right].sub < min) {
          min = dp[left][root - 1].sub + dp[root + 1][right].sub;
          dp[left][right].root = root;
        }
      }
      dp[left][right].sub = min + dp[left][right].sum; // 递推公式
    }
  }

  CTreeNodePtr *nodes = malloc(count * sizeof(CTreeNodePtr));
  for (int i = 0; i != count; ++i) nodes[i] = ctree_new_node_init(weight[i]);
  const CTreeNodePtr tree = nodes[dp[0][count - 1].root];

  // N个节点的二叉树最多有(N+1)/2个树叶
  Queue *queue = new_queue((count + 1) / 2);
  queue_push(queue, (LeftRight){0, count - 1});

  while (!queue_empty(queue)) {
    const LeftRight lr = queue_pop(queue);
    const int left = lr.left;
    const int right = lr.right;
    const int root = dp[left][right].root;
    const CTreeNodePtr node = nodes[root];

    if (root != left) {
      queue_push(queue, (LeftRight){left, root - 1});
      node->left = nodes[dp[left][root - 1].root];
    }

    if (root != right) {
      queue_push(queue, (LeftRight){root + 1, right});
      node->right = nodes[dp[root + 1][right].root];
    }
  }

  free(dp);
  free(nodes);
  delete_queue(queue);
  return tree;
}
