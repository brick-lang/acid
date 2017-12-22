#ifndef GYPSUM_STACK_H
#define GYPSUM_STACK_H

#include <stdbool.h>
#include <stddef.h>

typedef const struct stack_t {
  void* val;
  const struct stack_t* next;
} stack_t;

stack_t* stack_empty();
bool stack_is_empty(stack_t* stack);
stack_t* stack_cons(void *elem, stack_t *stack);
void* stack_head(stack_t *stack);
stack_t* stack_tail(stack_t *stack);
stack_t* stack_concat(stack_t *stack1, stack_t* stack2);
stack_t* stack_update(stack_t *stack, size_t idx, void* e);

#endif  // GYPSUM_STACK_H
