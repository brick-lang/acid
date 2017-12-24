#include <acid.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "../src/task.h"

extern atomic_uint_fast64_t acid_world_count;
extern atomic_uint_fast64_t acid_collect_count;

stack_t* stack_build(int *arr, int c) {
  stack_t* last _cleanup_acid_dissolve_ = stack_empty();
  stack_t* curr = NULL;
  acid_set(curr, last);
  for (int i = c; i > 0; i--) {
    stack_t* cell _cleanup_acid_dissolve_ = stack_cons(&arr[i], curr);
    acid_set(curr, cell);
  }
  return curr;
}

#define acid_new_int(var, val) \
  int* (var) _cleanup_acid_dissolve_ = acid_malloc(sizeof(int)); \
  *(var) = val;


void stack_test() {
  acid_new_int(_five, 5);
  acid_new_int(_four, 4);
  acid_new_int(_three, 3);
  acid_new_int(_two, 2);
  acid_new_int(_one, 1);
  acid_new_int(_zero, 0);

  stack_t* first_end = stack_empty();
  struct acid_header_t* first_end_header = _acid_get_header(first_end);
  stack_t* five = stack_cons(_five, first_end);
  struct acid_header_t* five_header = _acid_get_header(five);
  stack_t* four = stack_cons(_four, five);
  struct acid_header_t* four_header = _acid_get_header(four);
  stack_t* three = stack_cons(_three, four);
  struct acid_header_t* three_header = _acid_get_header(three);
  acid_dissolve(first_end);
  acid_dissolve(five);
  acid_dissolve(four);
  stack_t* back_stack = three;

  (void)first_end_header;
  (void)five_header;
  (void)four_header;
  (void)three_header;

//  stack_t* snd_end = stack_empty();
//  stack_t* two = stack_cons(_two, snd_end);
//  stack_t* one = stack_cons(_one, two);
//  stack_t* zero = stack_cons(_zero, one);
//  acid_dissolve(snd_end);
//  acid_dissolve(two);
//  acid_dissolve(one);
//  stack_t* front_stack = zero;

  //stack_t* combo = stack_concat(front_stack, back_stack);

  acid_dissolve(back_stack);
  //acid_dissolve(front_stack);
  //acid_dissolve(combo);

  //int lst[] = {0, 1, 2, 3, 4};
  //int replacement = 7;
  //stack_t* full_stack = stack_build(lst, sizeof(lst) / sizeof(int));
  //stack_t* updated = stack_update(full_stack, 2, &replacement);

  //(void)full_stack;
  //acid_dissolve(full_stack);
  //acid_dissolve(updated);
}

int main(int argc, char** argv) {
  acid_setup();
  for (int i = 0; i < 4096; i++) {
    printf("\r[%d/4096]",i+1);
    fflush(stdout);
    stack_test();
  }
  printf("\n");
  acid_teardown();
  printf("Total number of objects created: %lu\n", acid_world_count);
  printf("Total number of objects collected: %lu\n", acid_collect_count);
  exit(0);
}
