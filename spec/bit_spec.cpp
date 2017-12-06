#include "bit.h"
#include "cppspec.hpp"

describe bit_spec("Bit tests", $ {
  it("BIT_ZERO flips to BIT_ONE", _ {
    expect(bit_flip(BIT_ZERO)).to_equal(BIT_ONE);
  });

  it("BIT_ONE flips to BIT_ZERO",_ {
    expect(bit_flip(BIT_ONE)).to_equal(BIT_ZERO);
  });
});

int main() {
  return CppSpec::Runner(CppSpec::Formatters::verbose)
             .add_spec(bit_spec)
             .exec() ? EXIT_SUCCESS : EXIT_FAILURE;
}

