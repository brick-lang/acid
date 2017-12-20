#ifndef TASK_H
#define TASK_H
#include <stdbool.h>

typedef struct task_t {
  bool (*run)(void *);
  void *runarg;
} task_t;

void task_setup();
void task_teardown();
task_t *task_create(bool(*fn)(void*), void* arg);
void task_start(task_t *task);
unsigned task_get_running_count();
void task_wait_for_zero_threads();
void task_run(task_t *task);
void task_destroy(task_t *task);

#endif  // TASK_H
