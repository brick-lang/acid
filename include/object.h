/* Copyright 2016 Katherine Whitlock */

#ifndef OBJECT_H
#define OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Object { const int id; } Object;

Object* object_create();
int object_hash_code(Object* obj);
int object_equals(Object* o1, Object* o2);
char* object_to_string(Object* o);

#ifdef __cplusplus
}
#endif

#endif /* OBJECT_H */
