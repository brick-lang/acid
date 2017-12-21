#include "memory.h"

#include <limits.h>
#ifdef __STDC_NO_THREADS__
#include "tinycthread/tinycthread.h"
#else
#include <threads.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "task.h"

void *xmalloc(size_t size, char *func) {
  void *ptr = malloc(size);
  // Out-of-memory error handling
  while (ptr == NULL) {
    if (task_get_running_count() == 0) {
      fprintf(stderr, "%s: ERROR! Out of memory. Attempted recovery failed.\n",
              func);
      exit(-1);
    }
    fprintf(stderr,
            "%s: ERROR! No memory available. Waiting for collection...\n",
            func);
    thrd_sleep(&(struct timespec){.tv_nsec = 10000000}, NULL);  // 10ms
    ptr = malloc(size);                                         // try again
  }
  return ptr;
}
