#include "idbaseobject.h"
#include <stdatomic.h>
#include <stdlib.h>

static atomic_uint object_id_count = 1;

IdBaseObject* idbaseobject_create() {
  IdBaseObject* obj = malloc(sizeof *obj);
  *(unsigned*)&obj->id = object_id_count++;
  return obj;
}

void idbaseobject_init(IdBaseObject* obj) {
  *(unsigned*)&obj->id = object_id_count++;
}

int idbaseobject_hash_code(IdBaseObject* obj) { return obj->id; }

bool idbaseobject_equals(IdBaseObject* o1, IdBaseObject* o2) {
  return o1->id == o2->id;
}
