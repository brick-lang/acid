#ifndef COUNTER_H
#define COUNTER_H
#include <stdatomic.h>
#include <stdbool.h>
#include "idlock.h"

typedef struct counter_t {
  mtx_t _mtx;
  uint_fast64_t ref_count;
  uint_fast64_t store_count;
} counter_t;

counter_t* counter_create();
void counter_destroy(counter_t* c);

void counter_inc_ref(counter_t* c);
void counter_dec_ref(counter_t* c);
void counter_inc_store(counter_t* c);
void counter_dec_store(counter_t* c);

bool counter_done(counter_t* c);
bool counter_continue_waiting(counter_t* c);

#endif  // COUNTER_H
