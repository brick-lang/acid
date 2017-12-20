#ifndef IDLOCK_H
#define IDLOCK_H

#include <limits.h>  // Primes __STDC_NO_THREADS__
#ifdef __STDC_NO_THREADS__
  #include "tinycthread/tinycthread.h"
#else
  #include <threads.h>
#endif
#include <stdint.h>

/**
 * Lock priorities */
typedef struct idlock_t {
  const uint_fast64_t id;
  mtx_t mtx;
} idlock_t;

void idlock_init(idlock_t *lock);
int idlock_compare(const idlock_t *lock1, const idlock_t *lock2);
int idlock_equals(idlock_t *lock1, idlock_t *lock2);
int idlock_lock(idlock_t *lock);
int idlock_unlock(idlock_t *lock);

#endif /* IDLOCK_H */
