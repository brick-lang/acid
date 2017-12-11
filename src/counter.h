#ifndef COUNTER_H
#define COUNTER_H
#include <stdatomic.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct counter_t {
  atomic_size_t ref_count;
  atomic_size_t store_count;
} counter_t;
#ifdef __cplusplus
}
#endif
#endif  // COUNTER_H
