// Copyright 2016 Katherine Whitlock
#include "cppspec.hpp"
#include "../src/idbaseobject.h"

describe object_spec("An IdBaseObject", $ {
  IdBaseObject *o = idbaseobject_create();
  context("must have a unique id", _ {
    it("starting at 1", _ {
      expect(o->id).to_equal((unsigned)1);
    });

    it("incremented every creation", _ {
      IdBaseObject *o2 = idbaseobject_create();
      expect(o->id).not_().to_equal(o2->id);
      expect(o->id).to_be_less_than(o2->id);
      free(o2);
    });
  });

  it("is equivalent when compared to itself", _{
    expect((bool)idbaseobject_equals(o,o)).to_be_true();
  });

  it("is not equivalent to another object", _{
    IdBaseObject *o2 = idbaseobject_create();
    expect((bool)idbaseobject_equals(o,o2)).to_be_false();
    free(o2);
  });
});

int main() {
  return CppSpec::Runner(CppSpec::Formatters::verbose)
             .add_spec(object_spec)
             .exec() ? EXIT_SUCCESS : EXIT_FAILURE;
}
