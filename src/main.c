#define DEBUG
#include <here.h>
#include <link.h>
#include <assert.h>
#include <locker.h>
#include "../lib/collectc/include/list.h"
#include "../lib/collectc/include/deque.h"
#include "worker.h"
#include "root.h"
#include "../lib/collectc/include/hashset.h"

char debugmsg[200];

void mark_and_sweep();

void world_init() {
  root_setup();
  object_system_setup();
  workers_setup();
}

void world_teardown() {
  workers_teardown();
}

void simplecycle() {
  root_t *a = root_create();
  root_t *b = root_create();
  root_alloc(a);
  root_alloc(b);

  object_set(a->ref, "x", b->ref);
  object_set(b->ref, "x", a->ref);
  object_set(a->ref, "y", a->ref);


  sprintf(debugmsg, "a = %s", object_to_string(a->ref));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(b->ref));
  HERE_MSG(debugmsg);

  Object *pa = a->ref;
  Object *pb = b->ref;

  root_free(b);

  sprintf(debugmsg, "a = %s", object_to_string(pa));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(pb));
  HERE_MSG(debugmsg);

  root_free(a);

  sprintf(debugmsg, "a = %s", object_to_string(pa));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(pb));
  HERE_MSG(debugmsg);
}

void simplecycle2() {
  root_t *a = root_create();
  root_t *b = root_create();

  root_alloc(a);
  root_alloc(b);

  object_set(a->ref, "x", b->ref);
  object_set(b->ref, "x", a->ref);

  sprintf(debugmsg, "a = %s", object_to_string(a->ref));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(b->ref));
  HERE_MSG(debugmsg);

  Object *pa = a->ref;
  Object *pb = b->ref;

  root_free(b);

  sprintf(debugmsg, "a = %s", object_to_string(pa));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(pb));
  HERE_MSG(debugmsg);

  object_set(a->ref, "y", NULL);

  thrd_sleep(&(struct timespec) {.tv_nsec=3000000}, NULL);

  root_free(a);

  sprintf(debugmsg, "a = %s", object_to_string(pa));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(pb));
  HERE_MSG(debugmsg);
}

void wheel() {
  root_t *wheel = root_create();
  root_alloc(wheel);

  root_t *prev = root_create();
  root_set(prev, wheel->ref);

  const int n = 500;
  root_t *x = NULL;
  for (int i = 0; i <= n; i++) {
    x = root_create();
    root_alloc(x);
    object_set(prev->ref, "next", x->ref);
    root_set(prev, x->ref);
    if (i == n) object_set(x->ref, "next", wheel->ref);  // close the loop
    root_free(x);
  }
  root_free(prev);

  HERE_MSG("Finished creating cycle");

  Object *o = NULL;
  for (int i = 0; i < 300; i++) {
    // HERE_PREFIX_MSG("i=", i);
    locker_start1(wheel->ref);
    o = object_get(wheel->ref, "next");
    locker_end();

    locker_start2(wheel->ref, o);
    assert(o != NULL);
    root_set(wheel, o);
    locker_end();
  }
  xthread_wait_for_zero_threads();
  mark_and_sweep();
  for (int i = 0; i < 300; i++) {
    // HERE_PREFIX_MSG("i=", i);
    locker_start1(wheel->ref);
    o = object_get(wheel->ref, "next");
    locker_end();

    locker_start2(wheel->ref, o);
    assert(o != NULL);
    root_set(wheel, o);
    locker_end();
  }
}


extern List *roots;
extern HashSet *objects;
extern volatile int objectslive;
void mark_and_sweep(){
  HERE_MSG("Mark and Sweep");

  Deque *pqueue = NULL;
  deque_new(&pqueue);

  if (roots == NULL) {
    HERE_MSG("null queue");
  } else {
    sprintf(debugmsg, "size %zu", list_size(roots));
    HERE_MSG(debugmsg);
  }

  if (list_size(roots) > 0) {
    LIST_FOREACH(item, roots, { deque_add_last(pqueue, item); });
  }

  Object *obj;
  while (deque_size(pqueue) > 0) {
    deque_remove_first(pqueue, (void **) &obj);
    if (!obj->mark) {
      obj->mark = true;
      if (hashtable_size(obj->links) > 0) {
        HASHTABLE_FOREACH(l, obj->links, {
          link_t *li = ((TableEntry*)l)->value;
          deque_add_last(pqueue, li->target);
        })
      }
    }
  }
  deque_destroy(pqueue);

  HashSet *copy;
  hashset_new(&copy);
  if (hashset_size(objects) > 0) {
    HASHSET_FOREACH(orig, objects, { hashset_add(copy, orig); })
  }
  int live = 0;
  objectslive = object_live();
  HASHSET_FOREACH(so, copy, {
    Object *o = so;
    if (o->mark) {
      assert(o->count[o->which] > 0);
      assert(o->count[bit_flip(o->which)] >= 0);
      assert(o->count[2] == 0); // No ghosts allowed!
      assert(!o->deleted);
      assert(o->collector == NULL);
      live++;
      o->mark = false;
    } else {
      assert(o->deleted);
    }
  })
  hashset_destroy(copy);

  sprintf(debugmsg, "After Mark and Sweep, live = %d", live);
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "live = %d; slive = %d", live, objectslive);
  HERE_MSG(debugmsg);
  if (live == objectslive) {
    HERE_MSG(" MS == B");
  } else {
    HERE_MSG(" MS != B");
  }
}

void status() {
  xthread_wait_for_zero_threads();
  object_status();
  mark_and_sweep();
}

int main(int argc, char** argv) {
  world_init();
  printf("hello world!\n");
//  simplecycle();
//  simplecycle2();
  wheel();
//  multi_collect();
//  doubly_linked_list;
//  clique();
//  benzene_ring_scalability(6);
//  object_set_test;

  status();
  world_teardown();
}
