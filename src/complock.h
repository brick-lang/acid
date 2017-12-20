#ifndef COMPLOCK_H
#define COMPLOCK_H

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

typedef struct complock_t {
  const uint_fast64_t id;
  const priority_t priority;
  mtx_t mtx;
} complock_t;

complock_t* complock_create(priority_t priority);
void complock_destroy(complock_t* c);
int complock_compare(const complock_t* c1, const complock_t* c2);
int complock_equals(complock_t* c1, complock_t* c2);
int complock_lock(complock_t* c);
int complock_unlock(complock_t* c);

#endif /* COMPLOCK_H */
