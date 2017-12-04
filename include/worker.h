#include "xthread.h"

#ifndef WORKER_H
#define WORKER_H

#ifdef __cplusplus
extern "C" {
#endif

void workers_setup();
int worker_run(void*);
xthread_t* worker_get();
void worker_add(xthread_t *xthrd);

#ifdef __cplusplus
}
#endif

#endif //WORKER_H
