#ifndef LOCKABLE_H
#define LOCKABLE_H
#include "complock.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct lockable_t {
  const unsigned int id;  // placeholder as lockable inherits Object
  complock_t *cmplock;
} lockable_t;
#ifdef __cplusplus
}
#endif
#endif  // LOCKABLE_H
