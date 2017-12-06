#include "../lib/collectc/include/list.h"
#include "wrappedlock.h"
#include "counter.h"

#ifndef SAFELIST_H
#define SAFELIST_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A list of Objects that can only be accessed with Locks.
 */
typedef struct safelist_t {
  wrappedlock_t *const lock;
  counter_t *const count;
  List *const data; // List<Object>
  struct safelist_t *_forward;
} safelist_t;

safelist_t *safelist_create(counter_t *c);
void safelist_destroy(safelist_t *sl);
void safelist_add(safelist_t *sl, void* datum);
size_t safelist_size(safelist_t *sl);
bool safelist_is_empty(safelist_t *sl);
void *safelist_poll(safelist_t *sl);
void safelist_forward(safelist_t *sl, safelist_t *forward_to);
bool safelist_forwards_to(safelist_t *sl, safelist_t *forward);

#ifdef __cplusplus
}
#endif

#endif //SAFELIST_H
