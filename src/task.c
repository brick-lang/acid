#include <limits.h>
#ifdef __STDC_NO_THREADS__
#include "../lib/tinycthread/tinycthread.h"
#else
#include <threads.h>
#endif

#include <stdlib.h>
#include <assert.h>
#include "locker.h"
#include "wrappedlock.h"
#include "task.h"
#include "worker.h"

static wrappedlock_t *lock = NULL;
static volatile int count = 0;

void task_setup() {
  assert(lock == NULL);
  //lock = wrappedlock_create(PRIORITY_LIST);
  lock = malloc(sizeof(wrappedlock_t));
  lock->cmplock = complock_create(PRIORITY_LIST, 0);
  cnd_init(&lock->cond);
}

void task_teardown() {
  wrappedlock_destroy(lock);
}

task_t *task_create(){
  task_t *task = malloc(sizeof(task_t));
  task->run = NULL;
  task->runarg = NULL;
  return task;
}

void task_destroy(task_t *task) {
  free(task);
}

void task_start(task_t* task) {
  locker_start1(lock);
  count++;
  locker_end();
  worker_add(task);
}

int task_get_thread_count() {
  int retval;
  locker_start1(lock);
  retval = count;
  locker_end();
  return retval;
}

bool task_wait_for_zero_threads() {
  bool retval;
  locker_start1(lock);
  if (count > 0) {
    cnd_wait(&lock->cond, &lock->cmplock->mtx);
  }
  retval = (count == 0);
  locker_end();
  return retval;
}

void task_run(task_t *task) {
  bool again = task->run(task->runarg);
  // TODO: Error handle here (stack trace, etc.)
  if (again) {
    worker_add(task);
  } else {
    task_destroy(task);
    locker_start1(lock);
    count--;
    if(count == 0) {
      cnd_broadcast(&lock->cond);
    }
    locker_end();
  }
}
