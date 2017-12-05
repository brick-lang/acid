#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <here.h>
#include "counter.h"
#include "locker.h"

static char countstr[50];

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
  assert (counter->store_count >= 0); // : this
  locker_end();
}

char *counter_to_string(counter_t *counter) {
  sprintf(countstr, "COUNT: store=%d / ref=%d", counter->store_count, counter->ref_count);
  return countstr;
}

bool counter_action(counter_t *counter) {
  bool retval;
  locker_start1(counter->lock_counter);
  while (true) {
    if (counter->store_count > 0) { retval = true; break; }
    if (counter->ref_count == 0) { retval = false; break; }
    HERE_MSG(counter_to_string(counter));
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

