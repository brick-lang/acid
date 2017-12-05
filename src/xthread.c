#include <stdlib.h>
#include <assert.h>
#include "locker.h"
#include "wrappedlock.h"
#include "xthread.h"
#include "worker.h"

static wrappedlock_t *lock = NULL;
static volatile int count = 0;

void xthread_setup() {
  assert(lock == NULL);
  //lock = wrappedlock_create(PRIORITY_LIST);
  lock = malloc(sizeof(wrappedlock_t));
  lock->cmplock = complock_create(PRIORITY_LIST, 0);
  cnd_init(&lock->cond);
}

void xthread_teardown() {
  wrappedlock_destroy(lock);
}

xthread_t *xthread_create(){
  xthread_t *xthread = malloc(sizeof(xthread_t));
  xthread->run = NULL;
  xthread->runarg = NULL;
  return xthread;
}

void xthread_destroy(xthread_t *xthrd) {
  free(xthrd);
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
  bool again = xthrd->run(xthrd->runarg);
  // TODO: Error handle here (stack trace, etc.)
  if (again) {
    worker_add(xthrd);
  } else {
    xthread_destroy(xthrd);
    locker_start1(lock);
    count--;
    if(count == 0) {
      cnd_broadcast(&lock->cond);
    }
    locker_end();
  }
}
