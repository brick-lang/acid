#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <stdbool.h>
#include "complock.h"
#include "counter.h"
#include "safelist.h"
#include "object.h"

typedef struct collector_t {
  complock_t lock; // part of the 'lockable' interface.
  struct collector_t *forward;

  counter_t count;
  safelist_t collect;
  safelist_t merged_list;
  safelist_t recovery_list;
  safelist_t rebuild_list;
  safelist_t clean_list;
} collector_t;

collector_t *collector_create();
void collector_set_forward(collector_t *collector, collector_t *f);
void collector_add_object(collector_t *add_to, Object *o);
bool collector_run(void *collector);
void collector_merge(collector_t *collector, collector_t *s);
bool collector_equals(collector_t *c1, collector_t *c2);
collector_t *collector_update(collector_t *collector);

#endif  // COLLECTOR_H
