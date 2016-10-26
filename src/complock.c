#include "complock.h"
#include <stdio.h>
#include <stdlib.h>
#include "c11threads.h"
#include "priority.h"

complock_t* complock_create(priority_t priority, int id) {
  complock_t* complock = (complock_t*)malloc(sizeof(complock_t));
  complock->priority = priority;
  complock->id = id;

  // FIXME: check this return value to make sure everything's alright
  // (thrd_success vs thrd_error, etc.)
  mtx_init(&complock->mtx, mtx_plain | mtx_recursive);

  return complock;
}

int complock_compare(complock_t* c1, complock_t* c2) {
  if (c1->priority < c2->priority)
    return -1;
  else if (c1->priority > c2->priority)
    return 1;
  else if (c1->id < c2->id)
    return -1;
  else if (c1->id > c2->id)
    return 1;
  else
    return 0;
}

// TODO: Improve this. Seriously.
int complock_hash_code(complock_t* c) { return 100 * c->id * c->priority; }

int complock_equals(complock_t* c1, complock_t* c2) {
  return c1->id == c2->id && c1->priority == c2->priority;
}

char* complock_to_string(complock_t* c) {
  int max_len = 20;  // being generous
  char* str = (char*)calloc(max_len, sizeof(char));
  sprintf(str, "Lock[%d, %d]", c->priority, c->id);
  return str;
}
