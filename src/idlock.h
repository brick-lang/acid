#ifndef IDLOCK_H
#define IDLOCK_H

#include <stdint.h>
#include "threads.h"

/**
 * Lock priorities */
typedef struct idlock_t {
  const uint_fast64_t id;
  mtx_t mtx;
} idlock_t;

idlock_t* idlock_create();
void idlock_init(idlock_t *lock);
void idlock_destroy(idlock_t* lock);
int idlock_compare(const idlock_t *lock1, const idlock_t *lock2);
int idlock_equals(idlock_t *lock1, idlock_t *lock2);
int idlock_lock(idlock_t *lock);
int idlock_unlock(idlock_t *lock);

#endif /* IDLOCK_H */
