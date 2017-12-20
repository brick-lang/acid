#ifndef PRIORITYLOCK_H
#define PRIORITYLOCK_H

#include <limits.h>  // Primes __STDC_NO_THREADS__
#ifdef __STDC_NO_THREADS__
  #include "tinycthread/tinycthread.h"
#else
  #include <threads.h>
#endif
#include <stdint.h>

/**
 * Lock priorities */
typedef enum {
  PRIORITY_LIST = 0,
  PRIORITY_GLOBAL = 1,
  PRIORITY_COLLECTOR = 2,
  PRIORITY_OBJECT = 3,
} priority_t;

typedef struct prioritylock_t {
  const uint_fast64_t id;
  const priority_t priority;
  mtx_t mtx;
} prioritylock_t;

void prioritylock_init(prioritylock_t *lock, priority_t priority);
int prioritylock_compare(const prioritylock_t *lock1, const prioritylock_t *lock2);
int prioritylock_equals(prioritylock_t *lock1, prioritylock_t *lock2);
int prioritylock_lock(prioritylock_t *lock);
int prioritylock_unlock(prioritylock_t *lock);

#endif /* PRIORITYLOCK_H */
