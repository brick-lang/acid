#ifndef ACID_H
#define ACID_H
#include <stdbool.h>
#include <stddef.h>
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

void acid_setup();
void acid_teardown();
void acid_teardown_blocking();

#define acid_new(v, s) \
  (v) __attribute__((cleanup(acid_dissolve_cleanup))) = acid_malloc((s))
#define _cleanup_acid_dissolve_ __attribute((cleanup(acid_dissolve_cleanup)))
#define acid_set_field(p, f, v)                               \
  object_set(acid_get_base_object(p), #f,                     \
             ((v) == NULL) ? NULL : acid_get_base_object(v)); \
  (p)->f = v;

#define acid_set(p, v) acid_set_raw((void **)&(p), v);

void *acid_malloc(size_t alloc_size);
void acid_dissolve(void *ptr);
void acid_dissolve_cleanup(void *ptr);

void acid_set_raw(void **var, void *val);
bool acid_is_managed(void *ptr);
struct Object *acid_get_base_object(void *ptr);

#ifdef __cplusplus
}
#endif

#endif  // ACID_H
