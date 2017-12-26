#ifndef ACID_H
#define ACID_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef const void *acid_t;

struct acid_header_t;  // forward declaration
extern void object_set(struct acid_header_t *, size_t, struct acid_header_t *);

void acid_setup(int num_threads);
void acid_teardown_nonblocking();
void acid_teardown();

#define _cleanup_acid_dissolve_ __attribute__((cleanup(_acid_dissolve_cleanup)))

#define acid_set_field(var, field, acid_val)                              \
  if (acid_is_managed(acid_val)) {                                        \
    object_set(_acid_get_header(var),                                     \
               ((uintptr_t) & ((var)->field) - (uintptr_t)(var)),         \
               ((acid_val) == NULL) ? NULL : _acid_get_header(acid_val)); \
  }                                                                       \
  (var)->field = acid_val;

#define acid_set(acid_l, acid_r) _acid_set_raw((acid_t *)&(acid_l), acid_r);

void *acid_malloc(size_t alloc_size);
void *acid_malloc_dtor(size_t alloc_size, void (*dtor)(void *));

void acid_harden(acid_t acid_ptr);
void acid_dissolve(acid_t acid_ptr);
void _acid_dissolve_cleanup(void *acid_ptr);

bool acid_is_managed(const void *ptr);
void _acid_set_raw(acid_t *var, acid_t acid_val);
struct acid_header_t *_acid_get_header(acid_t acid_ptr);

#endif  // ACID_H
