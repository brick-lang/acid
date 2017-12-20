#include <stdlib.h>
#include <stdatomic.h>
#include "idlock.h"

static atomic_uint_fast64_t id_count = 1;

void idlock_init(idlock_t *complock) {
  *(uint_fast64_t *)&complock->id = id_count++;

  // FIXME: check this return value to make sure everything's alright
  // (thrd_success vs thrd_error, etc.)
  mtx_init(&complock->mtx, mtx_plain | mtx_recursive);
}

int idlock_compare(const idlock_t *lock1, const idlock_t *lock2) {
  if (lock1->id < lock2->id)
    return -1;
  else if (lock1->id > lock2->id)
    return 1;
  else
    return 0;
}

int idlock_equals(idlock_t *c1, idlock_t *c2) {
  return c1->id == c2->id;
}

int idlock_lock(idlock_t *c) { return mtx_lock(&c->mtx); }

int idlock_unlock(idlock_t *c) { return mtx_unlock(&c->mtx); }
