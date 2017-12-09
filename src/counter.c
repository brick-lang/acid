#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "here.h"
#include "counter.h"

void counter_inc_ref(counter_t* counter) {
  counter->ref_count++;
}

void counter_dec_ref(counter_t* counter) {
  counter->ref_count--;
  assert(counter->ref_count >= 0); // : this
}

void counter_inc_store(counter_t *counter) {
  counter->store_count++;
}

void counter_dec_store(counter_t *counter) {
  counter->store_count--;
  assert (counter->store_count >= 0); // : this
}

//char *counter_to_string(counter_t *counter) {
//  debug_snprintf(countstr, COUNTSTRLEN, "COUNT: store=%zu / ref=%zu", atomic_load(&counter->store_count), atomic_load(&counter->ref_count));
//  return countstr;
//}

size_t counter_get_ref_count(counter_t *counter) {
  return atomic_load(&counter->ref_count);
}

bool counter_continue_waiting(counter_t *counter) {
  return (counter->ref_count > 0 && counter->store_count == 0);
}

bool counter_done(counter_t *counter) {
  return (counter->ref_count == 0 && counter->store_count == 0);
}

