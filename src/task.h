#ifndef TASK_H
#define TASK_H
#include <stdbool.h>

typedef struct task_t {
  bool (*run)(void *);
  void *runarg;
} task_t;

void task_setup();
void task_teardown();
task_t *task_create();
void task_start(task_t *task);
size_t task_get_thread_count();
bool task_wait_for_zero_threads();
void task_run(task_t *task);
void task_destroy(task_t *task);

#endif  // TASK_H
