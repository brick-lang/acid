#include <stdatomic.h>
#include <stdbool.h>

#ifndef COUNTER_H
#define COUNTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct counter_t {
  atomic_size_t ref_count;
  atomic_size_t store_count;
} counter_t;

void counter_inc_ref(counter_t *counter);
void counter_dec_ref(counter_t *counter);
void counter_inc_store(counter_t *counter);
void counter_dec_store(counter_t *counter);
char *counter_to_string(counter_t *counter);
size_t counter_get_ref_count(counter_t *counter);
bool counter_continue_waiting(counter_t *counter);
bool counter_done(counter_t *counter);

#ifdef __cplusplus
}
#endif

#endif //COUNTER_H
