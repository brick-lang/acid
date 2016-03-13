#include "bit.h"
#include "cppspec.hpp"

describe bit_spec("Bit tests", _blk{
    it("ZERO flips to ONE", _blk{
        expect(bit_flip(ZERO)).to_equal(ONE)();
      });

    it("ONE flips to ZERO", _blk{
        expect(bit_flip(ONE)).to_equal(ZERO)();
      });

    it("0's value to be ZERO", _blk{
        expect(bit_value_of(0)).to_equal(ZERO)();
      });

    it("1's value to be ONE", _blk{
        expect(bit_value_of(1)).to_equal(ONE)();
      });
  });
