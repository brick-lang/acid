#ifndef IDBASEOBJECT_H
#define IDBASEOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

// An IdBaseObject is a structure defined by a unique ID.
typedef struct IdBaseObject { const unsigned int id; } IdBaseObject;

IdBaseObject* idbaseobject_create();
void idbaseobject_init(IdBaseObject *);
int idbaseobject_hash_code(IdBaseObject* obj);
int idbaseobject_equals(IdBaseObject* o1, IdBaseObject* o2);
//char* idbaseobject_to_string(IdBaseObject* o);

#ifdef __cplusplus
}
#endif

#endif /* IDBASEOBJECT_H */
