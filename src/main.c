#define DEBUG
#include <assert.h>
#include "../lib/collectc/include/list.h"
#include "../lib/collectc/include/deque.h"
#include "../lib/collectc/include/hashset.h"
#include "here.h"
#include "worker.h"
#include "root.h"
#include "locker.h"
#include "link.h"

static List* keylist = NULL;

void mark_and_sweep();

void world_init() {
  list_new(&keylist);
  root_setup();
  object_system_setup();
  xthread_setup();
  workers_setup();
}

void world_teardown() {
  workers_teardown();
  xthread_teardown();
  list_destroy_cb(keylist, free);
}

void simplecycle() {
  root_t *a = root_create();
  root_t *b = root_create();
  root_alloc(a);
  root_alloc(b);

  object_set(a->ref, "x", b->ref);
  object_set(b->ref, "x", a->ref);
  object_set(a->ref, "y", a->ref);

  root_free(b);
  root_free(a);
}

void simplecycle2() {
  root_t *a = root_create();
  root_t *b = root_create();

  root_alloc(a);
  root_alloc(b);

  object_set(a->ref, "x", b->ref);
  object_set(b->ref, "x", a->ref);

  root_free(b);
  object_set(a->ref, "y", NULL);

  thrd_sleep(&(struct timespec) {.tv_nsec=3000000}, NULL);
  root_free(a);
}

void wheel() {
  root_t *wheel = root_create();
  root_alloc(wheel);

  root_t *prev = root_create();
  root_set(prev, wheel->ref);

  const int n = 499;
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
    locker_start1(wheel->ref);
    o = object_get(wheel->ref, "next");
    locker_end();

    locker_start2(wheel->ref, o);
    assert(o != NULL);
    root_set(wheel, o);
    locker_end();
  }
  root_free(wheel);
}

void multi_collect() {
  root_t *a1 = root_create();
  root_alloc(a1);
  root_t *prev = root_create();
  root_set(prev, a1->ref);

  root_t *x = NULL;
  const int n = 1;
  for (int i = 0; i <= n; i++) {
    x = root_create();
    root_alloc(x);
    object_set(prev->ref, "next", x->ref);
    root_set(prev, x->ref);
    root_free(x);
  }
  HERE_MSG("First chain created");

  root_t *a2 = root_create();
  root_alloc(a2);
  root_t *prev2 = root_create();
  root_set(prev2, a2->ref);
  const int nt = 1;
  for (int i = 0; i <= nt; i++) {
    x = root_create();
    root_alloc(x);
    object_set(prev2->ref, "next", x->ref);
    root_set(prev2, x->ref);
    root_free(x);
  }
  HERE_MSG("Second chain created");

  object_set(prev->ref, "n", a2->ref);
  object_set(prev2->ref, "m", a1->ref);
  HERE_MSG("Chains joined");

  root_free(prev);
  root_free(prev2);
  root_free(a1);
  root_free(a2);
}

root_t *dlink(Object *o, int n) {
  root_t *a = root_create();
  root_alloc(a);
  if (o != NULL) {
    object_set(o, "next", a->ref); // forward
    object_set(a->ref, "prev", o); // back
    if (n == 0) {
      return a; // base case
    }
  }
  root_t *b = dlink(a->ref, n-1);
  root_free(a);
  return b;
}

void doubly_linked_list() {
  root_t *dl = dlink(NULL, 30);
  Object *o = NULL;
  while (true) {
    o = object_get(dl->ref, "prev");
    if (o == NULL) break;
    root_set(dl, o);
  }
  root_free(dl);
}

void clique() {
  int n = 9;
  root_t *a[n];
  for (int i = 0; i < n; i++) {
    a[i] = root_create();
    root_alloc(a[i]);
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i!=j) {
        char *key = calloc(7, sizeof(char));
        snprintf(key, 6, "%dx%d", i+1, j+1);
        list_add(keylist, key);
        object_set(a[i]->ref, key, a[j]->ref);
      }
    }
  }

  for (int i = n - 1; i >= 0; i--) {
    root_free(a[i]);
  }
}

void benzene_ring_scalability(int size) {
  int n = size * 16;
  root_t *a[n][6];
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 6; ++j) {
      a[i][j] = root_create();
      root_alloc(a[i][j]);
    }
  }

  // create benzene
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 6; ++j) {
      char *key = calloc(5, sizeof(char));
      snprintf(key, 5, "%d", (j+1)%8);
      list_add(keylist, key);
      Object *target = a[i][(j+1)%6]->ref;
      object_set(a[i][j]->ref, key, target);
    }
  }

  for (int i = 0; i < n-1; ++i) {
    object_set(a[i][1]->ref, "next1", a[i+1][5]->ref);
  }
  for (int i = n-1; i > 0; --i) {
    object_set(a[i][4]->ref, "next2", a[i-1][2]->ref);
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 1; j < 6; ++j) {
      root_free(a[i][j]);
    }
  }

  printf("Benzene x%d constructed.\n", n);
  xthread_wait_for_zero_threads();
  clock_t start = clock() / (CLOCKS_PER_SEC / 1000);

  for (int i = 0; i < n; ++i ){
    root_free(a[i][0]);
  }
  xthread_wait_for_zero_threads();
  clock_t end = clock() / (CLOCKS_PER_SEC / 1000);
  printf("Collection time=%lf\n", 0.001*(end-start));
}

void benzene_ring_ms_test() {
  int n = 8;
  
  root_t *a[n][6];
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 6; ++j) {
      a[i][j] = root_create();
      root_alloc(a[i][j]);
    }
  }
  
  // create benzene
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 6; ++j) {
      char *key = calloc(5, sizeof(char));
      snprintf(key, 5, "%d", (j+1)%8);
      list_add(keylist, key);
      Object *target = a[i][(j+1)%6]->ref;
      object_set(a[i][j]->ref, key, target);
    }
  }

  for (int j = n-1; j > 0; --j) {
    object_set(a[j][4]->ref, "next2", a[j-1][2]->ref);
  }

  for (int i = 0; i < n; ++i) {
    for (int j = 1; j < 6; ++j) {
      root_free(a[i][j]);
    }
  }

  HERE_MSG("Benzene finished");

  for (int i = 0; i < n; ++i) {
    root_free(a[i][0]);
  }
}

void object_set_test() {
  int n = 2;

  root_t *a[n][6];
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 6; ++j) {
      a[i][j] = root_create();
      root_alloc(a[i][j]);
    }
  }
  
  // create benzene
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 6; ++j) {
      object_set(a[i][j]->ref, "next", a[i][(j+1)%6]->ref);
    }
  }
  
  // partially free
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < 5; ++j) {
      root_free(a[i][j]);
    }
  }

  HERE_MSG("Benzene finished");

  int race_checks = 0;
  for (int i = 0; i < n; ++i) {
    bool phantomized = false;
    locker_start2(a[i][5]->ref, object_get(a[i][5]->ref, "next"));
    if (object_get(a[i][5]->ref, "next") != NULL &&
        object_get(a[i][5]->ref, "next")->phantomized) {
      root_set(a[i][5], object_get(a[i][5]->ref, "next"));
      race_checks++;
      phantomized = true;
    }
    locker_end();

    if (!phantomized) {
      int ncount = 0;
      while(!object_get(a[i][5]->ref, "next")->phantomized) {
        thrd_sleep(&(struct timespec) {.tv_nsec=1000000}, NULL);
        if (ncount++ == 10) break;
      }
      root_set(a[i][5], object_get(a[i][5]->ref, "next"));
      race_checks++;
    }
  }
  if (race_checks == n) {
    for (int i = 1; i < n; ++i) {
      object_set(a[i][5]->ref, "next1", a[i-1][5]->ref);
    }
    for (int i = 0; i < n; ++i) {
      root_free(a[i][5]);
    }
  }
}

extern List *roots;
extern HashSet *objects;
extern volatile int objectslive;
void mark_and_sweep(){
  printf("Mark and Sweep\n");

  Deque *pqueue = NULL;
  deque_new(&pqueue);

  if (roots == NULL) {
    printf("null queue\n");
  } else {
    printf("size %zu\n", list_size(roots));
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
  if (hashset_size(copy) > 0) {
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
  }
  hashset_destroy(copy);

  printf("After Mark and Sweep, live = %d\n", live);
  printf("live = %d; slive = %d\n", live, objectslive);

  if (live == objectslive) {
    printf(" MS == B\n");
  } else {
    printf(" MS != B\n");
  }
}

void status() {
  xthread_wait_for_zero_threads();
  object_status();
  mark_and_sweep();
}

int main(int argc, char** argv) {
  clock_t start = clock() / (CLOCKS_PER_SEC / 1000);
  world_init();
  printf("hello world!\n");
  simplecycle();
  simplecycle2();
  wheel();
  multi_collect();
  doubly_linked_list();
  clique();
  benzene_ring_ms_test();
  object_set_test();

  for (int x = 1; x <= 16; ++x) {
    clock_t st = clock() / (CLOCKS_PER_SEC / 1000);
    benzene_ring_scalability(x);
//    status();
    clock_t e = clock() / (CLOCKS_PER_SEC / 1000);
    printf("Total time for %d is %lf\n", x, 0.001*(e-st));
  }

  status();
  world_teardown();
  clock_t end = clock() / (CLOCKS_PER_SEC / 1000);
  printf("Total time elasped: %lf\n", 0.001*(end-start));
  exit(0);
}
