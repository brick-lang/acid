#include "complock.h"

#ifndef WRAPPEDLOCK_H
#define WRAPPEDLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * wrappedlock_t conforms to the 'Lockable' interface, i.e. it has a complock_t*
 * at 'cmplock';
 */
typedef struct wrappedlock_t {
  const unsigned int id;
  complock_t *cmplock;
  cnd_t cond;
} wrappedlock_t;

wrappedlock_t *wrappedlock_create(priority_t p);
void wrappedlock_destroy(wrappedlock_t *wl);

#ifdef __cplusplus
}
#endif

#endif  // WRAPPEDLOCK_H
