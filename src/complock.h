#ifndef COMPLOCK_H
#define COMPLOCK_H

#include <limits.h>  // Primes __STDC_NO_THREADS__
#ifdef __STDC_NO_THREADS__
  #include "../lib/tinycthread/tinycthread.h"
#else
  #include <threads.h>
#endif
#include "priority.h"

typedef struct complock_t {
  const unsigned int id;
  priority_t priority;
  mtx_t mtx;
} complock_t;

complock_t* complock_create(priority_t priority, unsigned int id);
void complock_destroy(complock_t* c);
int complock_compare(const complock_t* c1, const complock_t* c2);
int complock_equals(complock_t* c1, complock_t* c2);
int complock_lock(complock_t* c);
int complock_unlock(complock_t* c);

#endif /* COMPLOCK_H */
