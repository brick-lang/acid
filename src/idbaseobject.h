#ifndef IDBASEOBJECT_H
#define IDBASEOBJECT_H
#include <stdbool.h>

// An IdBaseObject is a structure defined by a unique ID.
typedef struct IdBaseObject {
  const unsigned int id;
} IdBaseObject;

IdBaseObject* idbaseobject_create();
void idbaseobject_init(IdBaseObject*);
int idbaseobject_hash_code(IdBaseObject* obj);
bool idbaseobject_equals(IdBaseObject* o1, IdBaseObject* o2);

#endif /* IDBASEOBJECT_H */
