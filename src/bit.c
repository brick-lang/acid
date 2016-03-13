#include "bit.h"

bit_t bit_flip(bit_t b) {
  switch (b) {
    case BIT_ZERO:
      return BIT_ONE;
    case BIT_ONE:
      return BIT_ZERO;
    default:
      printf("Fatal error: Bit.flip of non-bit");
      exit(-1);
  }
}

bit_t bit_value_of(int n) {
  switch (n) {
    case 0:
      return BIT_ZERO;
    case 1:
      return BIT_ONE;
    default:
      printf("Fatal error: Bit.value_of of non-i1:%d", n);
      exit(-1);
  }
}
