#include "c11threads.h"
#include "priority.h"

#ifndef COMPLOCK_H
#define COMPLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct complock_t {
  const unsigned int id;
  priority_t priority;
  mtx_t mtx;
} complock_t;

complock_t* complock_create(priority_t priority, unsigned int id);
void complock_destroy(complock_t* c);
int complock_compare(complock_t* c1, complock_t* c2);
int complock_hash_code(complock_t* c);
int complock_equals(complock_t* c1, complock_t* c2);
char* complock_to_string(complock_t* c);
int complock_lock(complock_t* c);
int complock_unlock(complock_t* c);

#ifdef __cplusplus
}
#endif

#endif /* COMPLOCK_H */
