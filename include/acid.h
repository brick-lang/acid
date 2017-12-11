#ifndef ACID_H
#define ACID_H
#include <stddef.h>

void acid_setup();
void acid_teardown();
void acid_teardown_blocking();

#define acid_new(v, s) (v)__attribute__((cleanup(acid_dissolve))) = acid_malloc((s))
#define _cleanup_acid_dissolve_ __attribute((cleanup(acid_dissolve)))
void *acid_malloc(size_t alloc_size);
void acid_dissolve(void *ptr);

#endif //ACID_H
