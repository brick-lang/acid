#include <stdio.h>
#include <stdlib.h>
#include "complock.h"

complock_t* complock_create(priority_t priority, unsigned int id) {
  complock_t* complock = malloc(sizeof(complock_t));
  *(unsigned int*)&complock->id = id;
  complock->priority = priority;

  // FIXME: check this return value to make sure everything's alright
  // (thrd_success vs thrd_error, etc.)
  mtx_init(&complock->mtx, mtx_plain | mtx_recursive);

  return complock;
}

inline void complock_destroy(complock_t *c) { free(c); }

char* complock_to_string(const complock_t* c, char* str) {
  sprintf(str, "#<CompLock:%p prio:%d, id:%d>", (void*)c, (int)c->priority, c->id);
  return str;
}

int complock_compare(const void*c1, const void* c2) {
  const complock_t *lock1 = c1;
  const complock_t *lock2 = c2;
  if (lock1->priority < lock2->priority)
    return -1;
  else if (lock1->priority > lock2->priority)
    return 1;
  else if (lock1->id < lock2->id)
    return -1;
  else if (lock1->id > lock2->id)
    return 1;
  else {
    if (c1 != c2) {
      char* str1 = calloc(100, sizeof(char));
      char* str2 = calloc(100, sizeof(char));
      printf("Equal! %s == %s\n", complock_to_string(c1, str1), complock_to_string(c2, str2));
      free(str1);
      free(str2);
    }
    return 0;
  }

}

// TODO: Improve this. Seriously.
int complock_hash_code(complock_t* c) { return 100 * c->id * c->priority; }

int complock_equals(complock_t* c1, complock_t* c2) {
  return c1->id == c2->id && c1->priority == c2->priority;
}

int complock_lock(complock_t* c) {
  return mtx_lock(&c->mtx);
}

int complock_unlock(complock_t* c) {
  return mtx_unlock(&c->mtx);
}
