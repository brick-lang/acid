#include <limits.h>
#ifdef __STDC_NO_THREADS__
#include "tinycthread/tinycthread.h"
#else
#include <threads.h>
#endif

#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>
#include "locker.h"
#include "task.h"
#include "worker.h"

static cnd_t zero_threads_cond;
static mtx_t count_mutex;
static atomic_size_t count = 0;

void task_setup() {
  mtx_init(&count_mutex, mtx_plain);
  cnd_init(&zero_threads_cond);
}

void task_teardown() {}

task_t *task_create() {
  task_t *task = malloc(sizeof(task_t));
  task->run = NULL;
  task->runarg = NULL;
  return task;
}

void task_destroy(task_t *task) { free(task); }

void task_start(task_t *task) {
  mtx_lock(&count_mutex);
  count++;
  mtx_unlock(&count_mutex);
  worker_add_task(task);
}

size_t task_get_thread_count() {
  size_t retval;
  mtx_lock(&count_mutex);
  retval = count;
  mtx_unlock(&count_mutex);
  return retval;
}

bool task_wait_for_zero_threads() {
  bool retval;
  mtx_lock(&count_mutex);
  if (count > 0) {
    cnd_wait(&zero_threads_cond, &count_mutex);
  }
  retval = (count == 0);
  mtx_unlock(&count_mutex);
  return retval;
}

void task_run(task_t *task) {
  bool again = task->run(task->runarg);
  // TODO: Error handle here (stack trace, etc.)
  if (again) {
    worker_add_task(task);
  } else {
    task_destroy(task);
    mtx_lock(&count_mutex);
    count--;
    if (count == 0) {
      cnd_broadcast(&zero_threads_cond);
    }
    mtx_unlock(&count_mutex);
  }
}
