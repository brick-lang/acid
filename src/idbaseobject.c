#include "idbaseobject.h"
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static atomic_uint object_id_count = 1;

IdBaseObject* idbaseobject_create() {
  IdBaseObject init = {.id = object_id_count++};
  IdBaseObject* obj = malloc(sizeof *obj);

  if (obj == NULL) abort();
  memcpy(obj, &init, sizeof *obj);

  return obj;
}

void idbaseobject_init(IdBaseObject* obj) {
  *(unsigned*)&obj->id = object_id_count++;
}

int idbaseobject_hash_code(IdBaseObject* obj) { return obj->id; }

int idbaseobject_equals(IdBaseObject* o1, IdBaseObject* o2) {
  return o1->id == o2->id;
}

// char* idbaseobject_to_string(IdBaseObject* o) {
//  double max_len = floor(log10(o->id)) + 1 + 6;  // being generous
//  char* str = (char*)calloc((size_t) max_len, sizeof(char));
//  sprintf(str, "Obj[%d]", o->id);
//  return str;
//}
