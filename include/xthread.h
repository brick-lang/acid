#include <stdbool.h>

#ifndef XTHREAD_H
#define XTHREAD_H


#ifdef __cplusplus
extern "C" {
#endif

typedef struct xthread_t {
  bool (*run)(void*);
  void *runarg;
} xthread_t;

xthread_t *xthread_create();
void xthread_start(xthread_t *xthread);
int xthread_get_thread_count();
bool xthread_wait_for_zero_threads();
void xthread_run(xthread_t *xthread);
void xthread_destroy(xthread_t *xthread);

#ifdef __cplusplus
}
#endif

#endif //XTHREAD_H