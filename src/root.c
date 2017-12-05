#include "../lib/collectc/include/list.h"
#include "here.h"
#include "locker.h"
#include "root.h"

static mtx_t root_synchro_mutex;

List *roots;

void root_setup() {
  mtx_init(&root_synchro_mutex, mtx_plain | mtx_recursive);
  list_new(&roots);
}

root_t *root_create(){
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

  mtx_lock(&root_synchro_mutex);
  list_add(roots, root->ref);
  mtx_unlock(&root_synchro_mutex);
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

  mtx_lock(&root_synchro_mutex);
  list_remove(roots, root->ref, NULL);
  if (obj != NULL) list_add(roots, obj);
  mtx_unlock(&root_synchro_mutex);

  root->ref = obj;
  locker_end();
}

void root_free(root_t *root) {
  HERE();
  root_set(root, NULL);
  free(root);
}
