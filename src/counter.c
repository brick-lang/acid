#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "counter.h"
#include "locker.h"

counter_t* counter_create() {
  counter_t* counter = (counter_t*)malloc(sizeof(counter_t));
  counter->ref_count = 0;
  counter->store_count = 0;
  counter->lock_counter = wrappedlock_create(PRIORITY_COLLECTCOUNTER);
  return counter;
}

void counter_destroy(counter_t *counter) {
  wrappedlock_destroy(counter->lock_counter);
  free(counter);
}

void counter_inc_ref(counter_t* counter) {
  locker_start1(counter->lock_counter);
  counter->ref_count++;
  locker_end();
}

void counter_dec_ref(counter_t* counter) {
  locker_start1(counter->lock_counter);
  counter->ref_count--;
  if (counter->ref_count == 0) {
    cnd_broadcast(&counter->lock_counter->cond);
  }
  assert(counter->ref_count >= 0); // : this
  locker_end();
}

void counter_inc_store(counter_t *counter) {
  locker_start1(counter->lock_counter);
  counter->store_count++;
  if (counter->store_count == 1) {
    cnd_broadcast(&counter->lock_counter->cond);
  }
  locker_end();
}

void counter_dec_store(counter_t *counter) {
  locker_start1(counter->lock_counter);
  counter->store_count--;
  // assert (store_count >= 0) : this
  locker_end();
}

char *counter_to_string(counter_t *counter) {
  char* str = (char*)calloc(40, sizeof(char));
  sprintf(str, "COUNT: store=%d / ref=%d", counter->store_count, counter->ref_count);
  return str;
}

bool counter_action(counter_t *counter) {
  bool retval;
  locker_start1(counter->lock_counter);
  while (true) {
    if (counter->store_count > 0) { retval = true; break; }
    if (counter->ref_count == 0) { retval = false; break; }
    // assert Here.here(this);
    cnd_wait(&counter->lock_counter->cond, &counter->lock_counter->cmplock->mtx);
    // TODO: check thrd_error here
  }
  locker_end();
  return retval;
}

int counter_get_ref_count(counter_t *counter) {
  int retval;
  locker_start1(counter->lock_counter);
  retval = counter->ref_count;
  locker_end();
  return retval;
}

bool counter_continue_waiting(counter_t *counter) {
  bool retval;
  locker_start1(counter->lock_counter);
  retval = (counter->ref_count > 0 && counter->store_count == 0);
  locker_end();
  return retval;
}

bool counter_done(counter_t *counter) {
  bool retval;
  locker_start1(counter->lock_counter);
  retval = (counter->ref_count == 0 && counter->store_count == 0);
  locker_end();
  return retval;
}

