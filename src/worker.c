#include <c11threads.h>
#include <assert.h>
#include "../lib/collectc/include/list.h"
#include "worker.h"
#define NUM_WORKERS 1

static thrd_t WORKERS[NUM_WORKERS];
static mtx_t worker_synchro_mutex;
static List *work; // List<xthread_t>
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

xthread_t *worker_get() {
  xthread_t *retval;
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
  while(true) {
    xthread_t *xthread = worker_get();
    if (xthread != NULL) {
      xthread_run(xthread);
    }
    if (endtime) return 0;
  }
}

void worker_add(xthread_t *xthrd) {
  mtx_lock(&worker_synchro_mutex);
  list_add_last(work, xthrd);
  mtx_unlock(&worker_synchro_mutex);
}

