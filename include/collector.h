#include <stdbool.h>
#include "complock.h"
#include "counter.h"
#include "object.h"
#include "safelist.h"
#include "task.h"

#ifndef COLLECTOR_H
#define COLLECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct collector_t {
  const unsigned int id; // placeholder as lockable inherits Object
  complock_t *const lock;
  volatile bool terminated;
  struct collector_t* forward;

  counter_t *const count;
  safelist_t *const collect;
  safelist_t *const merged_list;
  safelist_t *const recovery_list;
  safelist_t *const rebuild_list;
  safelist_t *const clean_list;
} collector_t;

collector_t *collector_create();
void collector_check_not_terminated(collector_t *collector);
void collector_set_forward(collector_t* collector, collector_t *f);
//char* collector_to_string(collector_t* collector);
void collector_add_object(collector_t* add_to, Object *o);
bool collector_run(collector_t *collector);
void collector_merge(collector_t* collector, collector_t *s);
bool collector_equals(collector_t *c1, collector_t *c2);
collector_t* collector_update(collector_t *collector);

#ifdef __cplusplus
}
#endif

#endif //COLLECTOR_H
