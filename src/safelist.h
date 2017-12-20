#ifndef SAFELIST_H
#define SAFELIST_H

#include "../lib/collectc/list.h"
#include "idlock.h"
#include "counter.h"

/**
 * A list of Objects that can only be accessed with Locks.
 * Conforms to lockable_t interface
 */
typedef struct safelist_t {
  idlock_t lock;
  counter_t *const count;
  List *const data;  // List<Object>
  struct safelist_t *_forward;
} safelist_t;

safelist_t *safelist_create(counter_t *c);
safelist_t *safelist_init(safelist_t* sl, counter_t *c);
void safelist_deinit(safelist_t *sl);
void safelist_add(safelist_t *sl, void *datum);
size_t safelist_size(safelist_t *sl);
bool safelist_is_empty(safelist_t *sl);
void *safelist_poll(safelist_t *sl);
void safelist_forward(safelist_t *sl, safelist_t *forward_to);
bool safelist_forwards_to(safelist_t *sl, safelist_t *forward);

#endif  // SAFELIST_H
