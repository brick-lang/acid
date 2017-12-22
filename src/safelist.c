#include "safelist.h"
#include "locker.h"
#include "object.h"

#include <stdio.h>
#include "task.h"

safelist_t *safelist_init(safelist_t *sl, counter_t *c) {
  idlock_init(&sl->lock);
  *(counter_t **)&sl->count = c;
  list_new((List **)&sl->data);
  sl->_forward = NULL;
  return sl;
}

void safelist_deinit(safelist_t *sl) { list_destroy(sl->data); }

void safelist_add(safelist_t *sl, void *datum) {
  safelist_t *f = NULL;

  locker_start1(sl);
  if (sl->_forward == NULL) {
    counter_inc_store(sl->count);
    list_add(sl->data, datum);
  } else {
    f = sl->_forward;
  }
  locker_end();

  if (f != NULL) {
    while (f->_forward != NULL) {
      f = f->_forward;
    }
    safelist_add(f, datum);
  }
}
size_t safelist_size(safelist_t *sl) {
  size_t retval;
  locker_start1(sl);
  retval = list_size(sl->data);
  locker_end();
  return retval;
}

bool safelist_is_empty(safelist_t *sl) { return safelist_size(sl) == 0; }

void *safelist_poll(safelist_t *sl) {
  void *retval;
  locker_start1(sl);
  if (list_size(sl->data) == 0) {
    retval = NULL;
  } else {
    counter_dec_store(sl->count);
    list_remove_first(sl->data, &retval);
  }
  locker_end();
  return retval;
}

void safelist_forward(safelist_t *sl, safelist_t *forward_to) {
  locker_start2(sl, forward_to);
  while (true) {
    Object *obj = safelist_poll(sl);
    if (obj == NULL) break;
    safelist_add(forward_to, obj);
  }
  sl->_forward = forward_to;
  locker_end();
}

bool safelist_forwards_to(safelist_t *sl, safelist_t *forward) {
  return sl->_forward == forward;
}
