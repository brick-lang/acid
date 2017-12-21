#ifndef MEMORY_H
#define MEMORY_H
#include <stddef.h>

void* xmalloc(size_t size, char* func);
void xfree(void* ptr);

#endif /* MEMORY_H */
