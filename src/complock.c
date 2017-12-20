#include <stdlib.h>
#include <stdatomic.h>
#include "complock.h"

static atomic_uint_fast64_t id_count = 1;

void complock_init(complock_t* complock, priority_t priority) {
  *(uint_fast64_t *)&complock->id = id_count++;
  *(priority_t *)&complock->priority = priority;

  // FIXME: check this return value to make sure everything's alright
  // (thrd_success vs thrd_error, etc.)
  mtx_init(&complock->mtx, mtx_plain | mtx_recursive);
}

int complock_compare(const complock_t* lock1, const complock_t* lock2) {
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

int complock_equals(complock_t* c1, complock_t* c2) {
  return c1->id == c2->id && c1->priority == c2->priority;
}

int complock_lock(complock_t* c) { return mtx_lock(&c->mtx); }

int complock_unlock(complock_t* c) { return mtx_unlock(&c->mtx); }
