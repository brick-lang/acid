#include "stack.h"
#include <acid.h>
#include <assert.h>
#include <stdlib.h>

stack_t *stack_empty() { return stack_cons(NULL, NULL); }

bool stack_is_empty(stack_t *stack) { return stack->val == NULL; }

stack_t *stack_cons(void *elem, const stack_t *stack) {
  struct stack_t *cell = acid_malloc(sizeof(stack_t));
  acid_set_field(cell, val, elem);
  acid_set_field(cell, next, (void *)stack);
  return cell;
}

void *stack_head(const stack_t *stack) {
  assert(stack->val != NULL);
  return stack->val;
}

stack_t *stack_tail(const stack_t *stack) {
  assert(stack->val != NULL);
  return stack->next;
}

stack_t *stack_concat(const stack_t *stack1, const stack_t *stack2) {
  if (stack_is_empty(stack1)) {
    // increment the strong count in preparation for decrementing it
    acid_harden((acid_t)stack2);
    return stack2;
  } else {
    void *const x = stack_head(stack1);
    stack_t *const xs = stack_tail(stack1);
    stack_t *const zs = stack_concat(xs, stack2);
    stack_t *const xzs = stack_cons(x, zs);  // x :: (xs ++ ys)
    acid_dissolve((acid_t)zs);
    return xzs;
  }
}

stack_t *stack_update(stack_t *stack, size_t idx, void *const y) {
  assert(!stack_is_empty(stack));
  stack_t *const xs = stack_tail(stack);
  if (idx == 0) {  // base case
    return stack_cons(y, xs);
  } else {
    void *const x = stack_head(stack);
    stack_t *const new_tail = stack_update(xs, idx - 1, y);

    // x :: update (xs, idx-1, y)
    stack_t *const new_stack = stack_cons(x, new_tail);
    acid_dissolve((acid_t)new_tail);
    return new_stack;
  }
}
