#include "bit.h"

Bit bit_flip(Bit b) {
  switch (b) {
    case ZERO:
      return ONE;
    case ONE:
      return ZERO;
  }
}

Bit bit_value_of(int n) {
  switch (n) {
    case 0:
      return ZERO;
    case 1:
      return ONE;
    default:
      printf("Fatal error: Bit.value_of of non-i1:%d", n);
      exit(-1);
  }
}
