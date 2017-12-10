#include "task.h"

#ifndef WORKER_H
#define WORKER_H

#ifdef __cplusplus
extern "C" {
#endif

void workers_setup();
void workers_teardown();
void worker_add_task(task_t *task);

#ifdef __cplusplus
}
#endif

#endif  // WORKER_H
