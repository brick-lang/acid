#include "acid.h"
#include <stdio.h>
#include "locker.h"
#include "memory.h"
#include "object.h"
#include "task.h"
#include "worker.h"

void acid_start(int num_threads) {
  locker_setup();
  task_setup();
  workers_setup(num_threads);
}

void acid_stop_nonblocking() {
  workers_teardown();
  task_teardown();
  locker_teardown();
}

void acid_stop() {
  task_wait_for_zero_threads();
  acid_stop_nonblocking();
}

void *acid_malloc(size_t alloc_size) {
  // | Object (header) | datum...
  void *fullspace = xmalloc(sizeof(Object) + alloc_size, "acid_malloc");

  Object *o = object_init_strong(fullspace);
  void *data_section = (void *)((intptr_t)fullspace + sizeof(Object));
  o->data = data_section;
  return data_section;
}

void *acid_malloc_dtor(size_t alloc_size, void (*dtor)(void *)) {
  void *mem = acid_malloc(alloc_size);
  _acid_get_header(mem)->dtor = dtor;
  return mem;
}

void acid_harden(const void *ptr) {
  Object *obj_addr = (Object *)((intptr_t)ptr - sizeof(Object));
  locker_start1(obj_addr);
  object_inc_strong(obj_addr);
  locker_end();
}

void acid_dissolve(const void *ptr) {
  Object *obj_addr = (Object *)((intptr_t)ptr - sizeof(Object));
  locker_start1(obj_addr);
  object_dec_strong(obj_addr);
  locker_end();
}

inline void _acid_dissolve_cleanup(const void *ptr) {
  acid_dissolve(*(const void **)ptr);
}

// var = val;
void _acid_set_raw(const void **var, const void *val) {
  Object *varobj = NULL;
  Object *valobj = _acid_get_header(val);
  if (acid_is_managed(*var)) {
    varobj = _acid_get_header(*var);
  }

  locker_start2(valobj, varobj);
  if (val != NULL) {
    object_inc_strong(valobj);
  }
  if (varobj != NULL) {
    object_dec_strong(varobj);
  }
  *var = val;
  locker_end();
}

extern const uint64_t OBJECT_MAGIC_NUMBER;
bool acid_is_managed(const void *ptr) {
  return ptr != NULL &&
         OBJECT_MAGIC_NUMBER == *(uint64_t *)((intptr_t)ptr - sizeof(Object) +
                                              offsetof(Object, magic));
}

inline Object *_acid_get_header(const void *ptr) {
  return (Object *)((intptr_t)ptr - sizeof(Object));
}
