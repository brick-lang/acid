#include <assert.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include "locker.h"
#include "memory.h"
#include "task.h"
#include "worker.h"
#include "threads.h"

static cnd_t task_zero_threads_cond;
static mtx_t task_count_mutex;
static unsigned task_count = 0;

void task_setup() {
  mtx_init(&task_count_mutex, mtx_plain);
  cnd_init(&task_zero_threads_cond);
}

void task_teardown() {
  cnd_destroy(&task_zero_threads_cond);
  mtx_destroy(&task_count_mutex);
}

task_t *task_create(bool (*fn)(void *), void *runarg) {
  task_t *task = xmalloc(sizeof(task_t), "task_create");
  task->run = fn;
  task->runarg = runarg;
  return task;
}

void task_destroy(task_t *task) { xfree(task); }

void task_start(task_t *task) {
  mtx_lock(&task_count_mutex);
  task_count++;
  mtx_unlock(&task_count_mutex);
  worker_add_task(task);
}

unsigned int task_get_running_count() {
  unsigned int retval;
  mtx_lock(&task_count_mutex);
  retval = task_count;
  mtx_unlock(&task_count_mutex);
  return retval;
}

void task_wait_for_zero_threads() {
  mtx_lock(&task_count_mutex);
  if (task_count > 0) {
    cnd_wait(&task_zero_threads_cond, &task_count_mutex);
  }
  mtx_unlock(&task_count_mutex);
}

void task_run(task_t *task) {
  bool again = task->run(task->runarg);
  // TODO: Error handle here (stack trace, etc.)
  if (again) {
    worker_add_task(task);
  } else {
    task_destroy(task);
    mtx_lock(&task_count_mutex);
    task_count--;
    if (task_count == 0) {
      cnd_broadcast(&task_zero_threads_cond);
    }
    mtx_unlock(&task_count_mutex);
  }
}
