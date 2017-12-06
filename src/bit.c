#include "bit.h"

/**
 * Invert the bit.
 * @param b the bit to flip
 */
bit_t bit_flip(bit_t b) {
  switch (b) {
    case BIT_ZERO:
      return BIT_ONE;
    case BIT_ONE:
      return BIT_ZERO;
    default:
      printf("Fatal error: Bit.flip of non-bit");
      abort();
  }
}
