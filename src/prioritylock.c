#include <stdlib.h>
#include <stdatomic.h>
#include "prioritylock.h"

static atomic_uint_fast64_t id_count = 1;

void prioritylock_init(prioritylock_t *complock, priority_t priority) {
  *(uint_fast64_t *)&complock->id = id_count++;
  *(priority_t *)&complock->priority = priority;

  // FIXME: check this return value to make sure everything's alright
  // (thrd_success vs thrd_error, etc.)
  mtx_init(&complock->mtx, mtx_plain | mtx_recursive);
}

int prioritylock_compare(const prioritylock_t *lock1, const prioritylock_t *lock2) {
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

int prioritylock_equals(prioritylock_t *c1, prioritylock_t *c2) {
  return c1->id == c2->id && c1->priority == c2->priority;
}

int prioritylock_lock(prioritylock_t *c) { return mtx_lock(&c->mtx); }

int prioritylock_unlock(prioritylock_t *c) { return mtx_unlock(&c->mtx); }
