#include <stdlib.h>
#include "idbaseobject.h"
#include "wrappedlock.h"

wrappedlock_t* wrappedlock_create(priority_t p) {
  wrappedlock_t* wrappedlock = (wrappedlock_t*)malloc(sizeof(wrappedlock_t));
  idbaseobject_init((IdBaseObject *) wrappedlock);
  wrappedlock->cmplock = complock_create(p, wrappedlock->id);
  // TODO: Check thrd_success here
  cnd_init(&wrappedlock->cond);
  return wrappedlock;
}

void wrappedlock_destroy(wrappedlock_t *wl) {
  complock_destroy(wl->cmplock);
  cnd_destroy(&wl->cond);
  free(wl);
}
