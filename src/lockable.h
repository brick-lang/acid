#ifndef LOCKABLE_H
#define LOCKABLE_H
#include "complock.h"

typedef struct lockable_t {
  const unsigned int id;  // placeholder as lockable inherits Object
  complock_t *cmplock;
} lockable_t;

#endif  // LOCKABLE_H
