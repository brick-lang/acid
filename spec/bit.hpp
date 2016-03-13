#include "bit.h"
#include "cppspec.hpp"

describe bit_spec("Bit tests", _blk {
  it("BIT_ZERO flips to BIT_ONE",
     _blk { expect(bit_flip(BIT_ZERO)).to_equal(BIT_ONE)(); });

  it("BIT_ONE flips to BIT_ZERO",
     _blk { expect(bit_flip(BIT_ONE)).to_equal(BIT_ZERO)(); });

  it("0's value to be BIT_ZERO",
     _blk { expect(bit_value_of(0)).to_equal(BIT_ZERO)(); });

  it("1's value to be BIT_ONE",
     _blk { expect(bit_value_of(1)).to_equal(BIT_ONE)(); });
});
