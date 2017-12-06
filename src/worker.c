#include <limits.h>
#ifdef __STDC_NO_THREADS__
#include "../lib/tinycthread/tinycthread.h"
#else
#include <threads.h>
#endif

#include <assert.h>
#include "../lib/collectc/include/list.h"
#include "worker.h"
#include "locker.h"

#define NUM_WORKERS 1

static thrd_t WORKERS[NUM_WORKERS];
static mtx_t worker_synchro_mutex;
static List *work; // List<task_t>
static volatile bool endtime = false;

void workers_setup() {
  list_new(&work);
  mtx_init(&worker_synchro_mutex, mtx_plain | mtx_recursive);
  for (int i = 0; i < NUM_WORKERS; i++) {
    thrd_create(&WORKERS[i], worker_run, NULL);
  }
}

void workers_teardown() {
  endtime = true;
  for (int i = 0; i < NUM_WORKERS; i++) {
    thrd_join(WORKERS[i], NULL);
  }

  mtx_lock(&worker_synchro_mutex);
  assert(list_size(work) == 0);
  list_destroy(work);
  mtx_unlock(&worker_synchro_mutex);
  mtx_destroy(&worker_synchro_mutex);
}

task_t *worker_get() {
  task_t *retval;
  mtx_lock(&worker_synchro_mutex);
  if (list_size(work) == 0) {
    retval = NULL;
  } else {
    list_remove_first(work, (void **) &retval);
  }
  mtx_unlock(&worker_synchro_mutex);
  return retval;
}

int worker_run(void*_) {
  locker_setup();
  while (!endtime) {
    task_t *task = worker_get();
    if (task != NULL) {
      task_run(task);
    }
  }
  // destroy the thread-local locker (if it exists)
  locker_teardown();
  return 0;
}

void worker_add(task_t *task) {
  mtx_lock(&worker_synchro_mutex);
  list_add_last(work, task);
  mtx_unlock(&worker_synchro_mutex);
}

