#include <stdio.h>
#include <stdlib.h>
#include "idbaseobject.h"
#include "complock.h"

complock_t* complock_create(priority_t priority, unsigned int id) {
  complock_t* complock = (complock_t*)malloc(sizeof(complock_t));
  *(unsigned int*)&complock->id = id;
  complock->priority = priority;

  // FIXME: check this return value to make sure everything's alright
  // (thrd_success vs thrd_error, etc.)
  mtx_init(&complock->mtx, mtx_plain | mtx_recursive);

  return complock;
}

inline void complock_destroy(complock_t *c) { free(c); }

int complock_compare(complock_t *lock1, complock_t* lock2) {
  if (lock1->priority < lock2->priority)
    return -1;
  else if (lock1->priority > lock2->priority)
    return 1;
  else if (lock1->id < lock2->id)
    return -1;
  else if (lock1->id > lock2->id)
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
  size_t max_len = 30;  // being generous
  char* str = (char*)calloc(max_len, sizeof(char));
  sprintf(str, "CompLock[prio=%d, id=%d]", c->priority, c->id);
  return str;
}

int complock_lock(complock_t* c) {
  return mtx_lock(&c->mtx);
}

int complock_unlock(complock_t* c) {
  return mtx_unlock(&c->mtx);
}
