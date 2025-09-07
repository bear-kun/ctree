#ifndef GRAPH_STACK_H
#define GRAPH_STACK_H

#include <stdlib.h>

#ifndef STACK_ELEM_TYPE
#define STACK_ELEM_TYPE int
#endif

typedef STACK_ELEM_TYPE StackElemType;
typedef struct {
  int size;
  StackElemType elems[0];
} Stack;

static Stack *stackCreate(const int capacity) {
  Stack *stack = malloc(sizeof(Stack) + capacity * sizeof(StackElemType));
  stack->size = 0;
  return stack;
}

static void stackRelease(Stack *const stack) { free(stack); }

static inline int stackEmpty(const Stack *const stack) {
  return stack->size == 0;
}

static inline void stackPush(Stack *const stack, const StackElemType item) {
  stack->elems[stack->size++] = item;
}

static inline StackElemType stackPop(Stack *const stack) {
  return stack->elems[--stack->size];
}

#endif // GRAPH_STACK_H
