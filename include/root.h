#include "object.h"

#ifndef ROOT_H
#define ROOT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct root_t {
  Object *ref;
} root_t;

void root_setup();
root_t *root_create();
void root_alloc(root_t *root);
void root_free(root_t *root);
void root_set(root_t *root, Object *o);
//Object *root_gets(root_t *root, char* field);
//void root_sets(root_t *root, char* field, Object* obj);

#ifdef __cplusplus
}
#endif

#endif //ROOT_H
