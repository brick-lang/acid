#include <stdbool.h>
#include "wrappedlock.h"

#ifndef COUNTER_H
#define COUNTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct counter_t {
  int ref_count;
  int store_count;
  wrappedlock_t *lock_counter;
} counter_t;

counter_t *counter_create();
void counter_destroy(counter_t *counter);
void counter_inc_ref(counter_t *counter);
void counter_dec_ref(counter_t *counter);
void counter_inc_store(counter_t *counter);
void counter_dec_store(counter_t *counter);
char *counter_to_string(counter_t *counter);
bool counter_action(counter_t *counter);
int counter_get_ref_count(counter_t *counter);
bool counter_continue_waiting(counter_t *counter);
bool counter_done(counter_t *counter);

#ifdef __cplusplus
}
#endif

#endif //COUNTER_H
