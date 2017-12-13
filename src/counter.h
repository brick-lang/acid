#ifndef COUNTER_H
#define COUNTER_H
#include <stdatomic.h>

typedef struct counter_t {
  atomic_size_t ref_count;
  atomic_size_t store_count;
} counter_t;

#endif  // COUNTER_H
