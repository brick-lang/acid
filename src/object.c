#include "object.h"
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static atomic_uint object_id_count = 1;

Object* object_create() {
  Object init = {.id = object_id_count++};
  Object* obj = malloc(sizeof *obj);

  if (obj == NULL) abort();
  memcpy(obj, &init, sizeof *obj);

  return obj;
}

int object_hash_code(Object* obj) { return obj->id; }

int object_equals(Object* o1, Object* o2) { return o1->id == o2->id; }

char* object_to_string(Object* o) {
  int max_len = 20;  // being generous
  char* str = (char*)calloc(max_len, sizeof(char));
  sprintf(str, "Obj[%d]", o->id);
  return str;
}
