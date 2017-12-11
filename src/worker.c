#include <limits.h>
#ifdef __STDC_NO_THREADS__
#include "../lib/tinycthread/tinycthread.h"
#else
#include <threads.h>
#endif

#include <stdatomic.h>
#include "../lib/collectc/include/list.h"
#include "locker.h"
#include "worker.h"

#define NUM_WORKERS 8

static thrd_t WORKERS[NUM_WORKERS];
static mtx_t work_sychro_mutex;
static List *work;  // List<task_t>
static atomic_bool endtime = false;

static int worker_run(void *);

void workers_setup() {
  list_new(&work);
  mtx_init(&work_sychro_mutex, mtx_plain | mtx_recursive);
  for (int i = 0; i < NUM_WORKERS; i++) {
    thrd_create(&WORKERS[i], worker_run, NULL);
  }
}

/**
 * Signal closing time, wait for all threads
 * to terminate, and clean up the workspace.
 */
void workers_teardown() {
  endtime = true;
  for (int i = 0; i < NUM_WORKERS; i++) {
    thrd_join(WORKERS[i], NULL);
  }

  mtx_lock(&work_sychro_mutex);
  list_destroy(work);
  mtx_unlock(&work_sychro_mutex);
  mtx_destroy(&work_sychro_mutex);
}

/**
 * Add a task to the work queue
 * @param task the task to add
 */
void worker_add_task(task_t *task) {
  mtx_lock(&work_sychro_mutex);
  list_add_last(work, task);
  mtx_unlock(&work_sychro_mutex);
}

/**
 * Get the next task in the work queue
 * @return
 */
static task_t *worker_get_next_task() {
  task_t *retval = NULL;
  mtx_lock(&work_sychro_mutex);
  if (list_size(work) > 0) {
    list_remove_first(work, (void **)&retval);
  }
  mtx_unlock(&work_sychro_mutex);
  return retval;
}

/**
 * The primary function of the worker threads
 * @return the exit status of the thread
 */
static int worker_run(void *ignored) {
  locker_setup();
  while (!endtime) {
    task_t *task = worker_get_next_task();
    if (task != NULL) {
      task_run(task);
    }
  }
  // destroy the thread-local locker (if it exists)
  locker_teardown();
  return 0;
}
