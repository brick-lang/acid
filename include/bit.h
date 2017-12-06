#include <stdio.h>
#include <stdlib.h>

#ifndef BIT_H
#define BIT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Keeps track of the state of the "which" bit. */
typedef enum { BIT_ZERO = 0, BIT_ONE = 1 } bit_t;

bit_t bit_flip(bit_t b);

#ifdef __cplusplus
}
#endif

#endif /* BIT_H */
