#include "safelist.h"
#include "locker.h"
#include "object.h"

safelist_t *safelist_create(counter_t *c) {
  safelist_t *sl = malloc(sizeof(safelist_t));
  return safelist_init(sl, c);
}

safelist_t *safelist_init(safelist_t* sl, counter_t *c) {
  complock_init(&sl->lock, PRIORITY_LIST);
  *(counter_t **)&sl->count = c;
  list_new((List **)&sl->data);
  sl->_forward = NULL;
  return sl;
}

void safelist_deinit(safelist_t *sl) {
  list_destroy(sl->data);
}

void safelist_add(safelist_t *sl, void *datum) {
  safelist_t *f = NULL;

  locker_start1(sl);
  if (sl->_forward == NULL) {
    sl->count->store_count++;
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
    sl->count->store_count--;
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
