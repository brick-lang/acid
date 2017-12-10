#include "wrappedlock.h"
#include <assert.h>
#include <stdlib.h>
#include "idbaseobject.h"

wrappedlock_t* wrappedlock_create(priority_t p) {
  wrappedlock_t* wrappedlock = malloc(sizeof(wrappedlock_t));
  idbaseobject_init((IdBaseObject*)wrappedlock);
  wrappedlock->cmplock = complock_create(p, wrappedlock->id);
  cnd_init(&wrappedlock->cond);
  return wrappedlock;
}

void wrappedlock_destroy(wrappedlock_t *wl) {
  complock_destroy(wl->cmplock);
  cnd_destroy(&wl->cond);
  free(wl);
}
