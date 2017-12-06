#include "task.h"

#ifndef WORKER_H
#define WORKER_H

#ifdef __cplusplus
extern "C" {
#endif

void workers_setup();
void workers_teardown();
int worker_run(void*);
task_t* worker_get();
void worker_add(task_t *task);

#ifdef __cplusplus
}
#endif

#endif //WORKER_H
