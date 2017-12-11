#include "acid.h"
#include "locker.h"
#include "task.h"
#include "worker.h"
#include "object.h"

void acid_setup() {
  locker_setup();
  task_setup();
  workers_setup();
}

void acid_teardown() {
  workers_teardown();
  task_teardown();
  locker_teardown();
}

void acid_teardown_blocking() {
  task_wait_for_zero_threads();
  acid_teardown();
}

void *acid_malloc(size_t alloc_size) {
  // | Object (header) | datum...
  void *fullspace = malloc(sizeof(Object) + alloc_size);
  Object *o = object_init_strong(fullspace);
  void *data_section = (void *) ((intptr_t)fullspace + sizeof(Object));
  o->data = data_section;
  return data_section;
}

void acid_dissolve(void *ptr) {
  void* addr = (void *) ((intptr_t)ptr - sizeof(Object));
  object_dec_strong(addr);
}
