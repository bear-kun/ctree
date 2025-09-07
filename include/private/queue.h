#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#ifndef QUEUE_ELEM_TYPE
#define QUEUE_ELEM_TYPE int
#endif

typedef QUEUE_ELEM_TYPE QueueElemType;
typedef struct {
  int capacity, size;
  int front, rear;
  QueueElemType elems[0];
} Queue;

static inline Queue *queueCreate(const int capacity) {
  Queue *queue = malloc(sizeof(Queue) + capacity * sizeof(QueueElemType));
  queue->capacity = capacity;
  queue->size = queue->front = queue->rear = 0;
  return queue;
}

static inline void queueRelease(Queue *queue) { free(queue); }

static inline void cgraphQueueClear(Queue *queue) {
  queue->size = queue->front = queue->rear = 0;
}

static inline int queueEmpty(const Queue *queue) { return queue->size == 0; }

static inline void queuePush(Queue *queue, const QueueElemType item) {
  queue->elems[queue->front] = item;
  if (++queue->front == queue->capacity) queue->front = 0;
  ++queue->size;
}

static inline QueueElemType queuePop(Queue *queue) {
  const QueueElemType item = queue->elems[queue->rear];
  if (++queue->rear == queue->capacity) queue->rear = 0;
  --queue->size;
  return item;
}

#endif // QUEUE_H
