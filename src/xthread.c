#include <stdlib.h>
#include <callback.h>
#include "locker.h"
#include "wrappedlock.h"
#include "xthread.h"
#include "worker.h"

static volatile int count = 0;
static wrappedlock_t *lock = NULL;

xthread_t *xthread_create(){
  if (lock == NULL) {
    lock = wrappedlock_create(PRIORITY_LIST);
  }
  xthread_t *xthread = malloc(sizeof(xthread_t));
  xthread->run = NULL;
  return xthread;
}

int xthread_xrun(xthread_t *xthrd) {
  return xthrd->run();
}

void xthread_start(xthread_t* xthrd) {
  locker_start1(lock);
  count++;
  locker_end();
  worker_add(xthrd);
}

int xthread_get_thread_count() {
  int retval;
  locker_start1(lock);
  retval = count;
  locker_end();
  return retval;
}

bool xthread_wait_for_zero_threads() {
  bool retval;
  locker_start1(lock);
  if (count > 0) {
    cnd_wait(&lock->cond, &lock->cmplock->mtx);
  }
  retval = (count == 0);
  locker_end();
  return retval;
}

void xthread_run(xthread_t *xthrd) {
  bool again = false;
  again = (bool)xthread_xrun(xthrd);
  // TODO: Error handle here (stack trace, etc.)
  if (again) {
    worker_add(xthrd);
  } else {
    locker_start1(lock);
    count--;
    if(count == 0) {
      cnd_broadcast(&lock->cond);
    }
    locker_end();
  }
}

void xthread_destroy(xthread_t *xthrd) {
  if (xthrd->run != NULL) {
    free_callback(xthrd->run);
  }
  free(xthrd);
}
