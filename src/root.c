#include "root.h"
#include "../lib/collectc/include/list.h"
#include "locker.h"

root_t *root_create() {
  root_t *root = malloc(sizeof(root));
  root->ref = NULL;
  return root;
}

void root_alloc(root_t *root) {
  if (root->ref != NULL) {
    object_dec_strong(root->ref);
  }
  root->ref = object_create();
  object_inc_strong(root->ref);
}

/**
 * Set the root object.
 * @param root
 * @param obj the object to become the new root.
 */
void root_set(root_t *root, Object *obj) {
  locker_start2(obj, root->ref);
  if (obj != NULL) object_inc_strong(obj);
  if (root->ref != NULL) object_dec_strong(root->ref);

  root->ref = obj;
  locker_end();
}

void root_free(root_t *root) {
  root_set(root, NULL);
  free(root);
}
