#include <stdlib.h>
#include <stdatomic.h>
#include "idlock.h"
#include "memory.h"

static atomic_uint_fast64_t id_count = 1;

idlock_t *idlock_create() {
  idlock_t *lock = xmalloc(sizeof(idlock_t), "idlock_create");
  idlock_init(lock);
  return lock;
}

void idlock_init(idlock_t *lock) {
  *(uint_fast64_t *)&lock->id = id_count++;

  // FIXME: check this return value to make sure everything's alright
  // (thrd_success vs thrd_error, etc.)
  mtx_init(&lock->mtx, mtx_plain | mtx_recursive);
}

void idlock_destroy(idlock_t* lock) {
  mtx_destroy(&lock->mtx);
  free(lock);
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
