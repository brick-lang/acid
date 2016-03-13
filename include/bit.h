#include <stdlib.h>
#include <stdio.h>

#ifndef BIT_H
#define BIT_H

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Keeps track of the state of the "which" bit. */
typedef enum {
  ZERO = 0,
  ONE = 1
} Bit;


Bit bit_flip(Bit b);
Bit bit_value_of(int n);

#ifdef __cplusplus
}
#endif

#endif /* BIT_H */
