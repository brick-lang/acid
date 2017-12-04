#include "locker.h"
#include "safelist.h"
#include "object.h"

safelist_t *safelist_create(counter_t *c) {
  safelist_t *safelist = (safelist_t*)malloc(sizeof(safelist_t));
  *(wrappedlock_t**)&safelist->lock = wrappedlock_create(PRIORITY_LIST);
  *(counter_t**)&safelist->count = c;
  array_new((Array **) &safelist->data);
  safelist->_forward = NULL;
  return safelist;
}

void safelist_destroy(safelist_t *sl) {
  wrappedlock_destroy(sl->lock);
  array_destroy(sl->data);
  free(sl);
}

void safelist_add(safelist_t *sl, void *datum) {
  safelist_t *f = NULL;

  locker_start1(sl->lock);
  if (sl->_forward == NULL) {
    counter_inc_store(sl->count);
    array_add(sl->data, datum);
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
  locker_start1(sl->lock);
  retval = array_size(sl->data);
  locker_end();
  return retval;
}

bool safelist_is_empty(safelist_t *sl) {
  return safelist_size(sl) == 0;
}

void *safelist_poll(safelist_t *sl) {
  void *retval;
  locker_start1(sl->lock);
  if (array_size(sl->data) == 0) {
    retval = NULL;
  } else {
    counter_dec_store(sl->count);
    array_remove_at(sl->data, 0, &retval);
  }
  locker_end();
  return retval;
}

void safelist_forward(safelist_t *sl, safelist_t *forward_to) {
  locker_start2(sl->lock, forward_to->lock);
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



