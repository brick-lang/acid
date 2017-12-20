#include <stdlib.h>
#include "wrappedlock.h"

wrappedlock_t* wrappedlock_create(priority_t p) {
  wrappedlock_t* wrappedlock = malloc(sizeof(wrappedlock_t));
  wrappedlock->cmplock = complock_create(p);
  cnd_init(&wrappedlock->cond);
  return wrappedlock;
}

void wrappedlock_destroy(wrappedlock_t *wl) {
  complock_destroy(wl->cmplock);
  cnd_destroy(&wl->cond);
  free(wl);
}
