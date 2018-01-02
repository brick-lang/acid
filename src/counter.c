#include <stdlib.h>
#include "counter.h"
#include "memory.h"

counter_t *counter_create() {
  counter_t* counter = xmalloc(sizeof(counter_t), "counter_create");
  // TODO: Make sure this needs to be recursive
  mtx_init(&counter->_mtx, mtx_plain);
  counter->ref_count = 0;
  counter->store_count = 0;
  return counter;
}

void counter_destroy(counter_t *counter) {
  xfree(counter);
}

void counter_inc_ref(counter_t* counter) {
  mtx_lock(&counter->_mtx);
  counter->ref_count++;
  mtx_unlock(&counter->_mtx);
}

void counter_dec_ref(counter_t* counter) {
  mtx_lock(&counter->_mtx);
  counter->ref_count--;
  mtx_unlock(&counter->_mtx);
}

void counter_inc_store(counter_t* counter) {
  mtx_lock(&counter->_mtx);
  counter->store_count++;
  mtx_unlock(&counter->_mtx);
}

void counter_dec_store(counter_t* counter) {
  mtx_lock(&counter->_mtx);
  counter->store_count--;
  mtx_unlock(&counter->_mtx);
}

bool counter_continue_waiting(counter_t *counter) {
  bool retval;
  mtx_lock(&counter->_mtx);
  retval = (counter->ref_count > 0 && counter->store_count == 0);
  mtx_unlock(&counter->_mtx);
  return retval;
}

bool counter_done(counter_t *counter) {
  bool retval;
  mtx_lock(&counter->_mtx);
  retval = (counter->ref_count == 0 && counter->store_count == 0);
  mtx_unlock(&counter->_mtx);
  return retval;
}
