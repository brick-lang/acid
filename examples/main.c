#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/locker.h"
#include "acid.h"
#include "../lib/collectc/hashtable.h"

#define HERE_MSG(msg)                                                       \
  (printf("here: %s(%s:%d) %s\n", __func__, strrchr("/" __FILE__, '/') + 1, \
          __LINE__, msg))

typedef struct point_t {
  void* x;
  void* y;
} Point;

void simplecycle() {
  Point* a _cleanup_acid_dissolve_ = acid_malloc(sizeof(Point));
  Point* b _cleanup_acid_dissolve_ = acid_malloc(sizeof(Point));

  acid_set_field(a, x, b);
  acid_set_field(b, x, a);
  acid_set_field(a, y, a);
}

void simplecycle2() {
  Point* a = acid_malloc(sizeof(Point));
  Point* b = acid_malloc(sizeof(Point));

  acid_set_field(a, x, b);
  acid_set_field(b, x, a);

  acid_dissolve(b);

  acid_set_field(a, y, NULL);
  acid_dissolve(a);
}

typedef struct slink {
  struct slink* next;
} slink_t;

void wheel() {
  slink_t* wheel = acid_malloc(sizeof(slink_t));

  slink_t* prev = NULL;
  acid_set(prev, wheel);

  const int n = 499;
  for (int i = 0; i <= n; i++) {
    slink_t* x _cleanup_acid_dissolve_ = acid_malloc(sizeof(slink_t));
    acid_set_field(prev, next, x);
    acid_set(prev, x);
    if (i == n) {
      acid_set_field(x, next, wheel);  // close the loop
    }
  }
  acid_dissolve(prev);
  acid_dissolve(wheel);
}
//
//void multi_collect() {
//  slink_t* a1 = acid_malloc(sizeof(slink_t));
//  slink_t* prev _cleanup_acid_dissolve_ = NULL;
//  acid_set(prev, a1);
//
//  const int n = 6;
//  for (int i = 0; i <= n; i++) {
//    slink_t* x _cleanup_acid_dissolve_ = acid_malloc(sizeof(slink_t));
//    acid_set_field(prev, next, x);
//    acid_set(prev, x);
//  }
//  HERE_MSG("First chain created");
//
//  slink_t* a2 = acid_malloc(sizeof(slink_t));
//  slink_t* prev2 _cleanup_acid_dissolve_ = NULL;
//  acid_set(prev2, a2);
//
//  const int nt = 6;
//  for (int i = 0; i <= nt; i++) {
//    slink_t* x _cleanup_acid_dissolve_ = acid_malloc(sizeof(slink_t));
//    acid_set_field(prev2, next, x);
//    acid_set(prev2, x);
//  }
//  HERE_MSG("Second chain created");
//
//  object_set(_acid_get_header(prev), "n", _acid_get_header(a2));
//  object_set(_acid_get_header(prev), "m", _acid_get_header(a1));
//  HERE_MSG("Chains joined");
//
//  acid_dissolve(a1);
//  acid_dissolve(a2);
//}

#include "../src/object.h"

#define EDGE_COUNT 9
typedef struct Vertex {
  struct Vertex* edges[EDGE_COUNT];
} Vertex;
void clique() {
  Vertex* a[EDGE_COUNT];
  for (int i = 0; i < EDGE_COUNT; i++) {
    a[i] = acid_malloc(sizeof(Vertex));
  }

  for (int i = 0; i < EDGE_COUNT; i++) {
    for (int j = 0; j < EDGE_COUNT; j++) {
      if (i != j) {
        acid_set_field(a[i], edges[j], a[j]);
        //printf("%zu\n",(uintptr_t)&(a[i]->edges[j]) - (uintptr_t)(a[i]));
      }
    }
  }

//  HASHTABLE_FOREACH(entry, _acid_get_header(a[0])->links, { printf("%zu\n", (size_t)entry->key); })

  for (int i = EDGE_COUNT - 1; i >= 0; i--) {
    acid_dissolve(a[i]);
  }
}

//void benzene_ring_scalability(int size) {
//  int n = size * 16;
//  slink_t *a[n][6];
//  for (int i = 0; i < n; ++i) {
//    for (int j = 0; j < 6; ++j) {
//      a[i][j] = acid_malloc(sizeof(slink_t));
//    }
//  }
//
//  // create benzene
//  for (int i = 0; i < n; ++i) {
//    for (int j = 0; j < 6; ++j) {
//      slink_t *target = a[i][(j + 1) % 6];
//      acid_set_field(a[i][j], next, target);
//    }
//  }
//
//  for (int i = 0; i < n - 1; ++i) {
//    object_set(_acid_get_header(a[i][1]), "next1", _acid_get_header(a[i + 1][5]));
//  }
//  for (int i = n - 1; i > 0; --i) {
//    object_set(_acid_get_header(a[i][4]), "next2", _acid_get_header(a[i - 1][2]));
//  }
//
//  for (int i = 0; i < n; ++i) {
//    for (int j = 1; j < 6; ++j) {
//      acid_dissolve(a[i][j]);
//      //a[i][j] = NULL;
//    }
//  }
//
//  // printf("Benzene x%d constructed.\n", n);
//  printf(".");
//  fflush(stdout);
//  for (int i = 0; i < n; ++i) {
//    acid_dissolve(a[i][0]);
//    //a[i][0] = NULL;
//  }
//}
//
// void benzene_ring_ms_test() {
//  int n = 8;
//
//  root_t *a[n][6];
//  for (int i = 0; i < n; ++i) {
//    for (int j = 0; j < 6; ++j) {
//      a[i][j] = root_create();
//      root_alloc(a[i][j]);
//    }
//  }
//
//  // create benzene
//  for (int i = 0; i < n; ++i) {
//    for (int j = 0; j < 6; ++j) {
//      char *key = calloc(5, sizeof(char));
//      snprintf(key, 5, "%d", (j + 1) % 8);
//      list_add(keylist, key);
//      Object *target = a[i][(j + 1) % 6]->ref;
//      object_set(a[i][j]->ref, key, target);
//    }
//  }
//
//  for (int j = n - 1; j > 0; --j) {
//    object_set(a[j][4]->ref, "next2", a[j - 1][2]->ref);
//  }
//
//  for (int i = 0; i < n; ++i) {
//    for (int j = 1; j < 6; ++j) {
//      root_free(a[i][j]);
//    }
//  }
//
//  HERE_MSG("Benzene finished");
//
//  for (int i = 0; i < n; ++i) {
//    root_free(a[i][0]);
//  }
//}
//
// void object_set_test() {
//  int n = 2;
//
//  root_t *a[n][6];
//  for (int i = 0; i < n; ++i) {
//    for (int j = 0; j < 6; ++j) {
//      a[i][j] = root_create();
//      root_alloc(a[i][j]);
//    }
//  }
//
//  // create benzene
//  for (int i = 0; i < n; ++i) {
//    for (int j = 0; j < 6; ++j) {
//      object_set(a[i][j]->ref, "next", a[i][(j + 1) % 6]->ref);
//    }
//  }
//
//  // partially free
//  for (int i = 0; i < n; ++i) {
//    for (int j = 0; j < 5; ++j) {
//      root_free(a[i][j]);
//    }
//  }
//
//  HERE_MSG("Benzene finished");
//
//  int race_checks = 0;
//  for (int i = 0; i < n; ++i) {
//    bool phantomized = false;
//    locker_start2(a[i][5]->ref, object_get(a[i][5]->ref, "next"));
//    if (object_get(a[i][5]->ref, "next") != NULL &&
//        object_get(a[i][5]->ref, "next")->phantomized) {
//      root_set(a[i][5], object_get(a[i][5]->ref, "next"));
//      race_checks++;
//      phantomized = true;
//    }
//    locker_end();
//
//    if (!phantomized) {
//      int ncount = 0;
//      while (!object_get(a[i][5]->ref, "next")->phantomized) {
//        thrd_sleep(&(struct timespec){.tv_nsec = 1000000}, NULL);
//        if (ncount++ == 10) break;
//      }
//      root_set(a[i][5], object_get(a[i][5]->ref, "next"));
//      race_checks++;
//    }
//  }
//  if (race_checks == n) {
//    for (int i = 1; i < n; ++i) {
//      object_set(a[i][5]->ref, "next1", a[i - 1][5]->ref);
//    }
//    for (int i = 0; i < n; ++i) {
//      root_free(a[i][5]);
//    }
//  }
//}

void test1() {
  int* i _cleanup_acid_dissolve_ = acid_malloc(sizeof(int));
  *i = 42;
}

typedef struct dlink_t {
  void* data;
  struct dlink_t* prev;
  struct dlink_t* next;
} dlink_t;

void* wrapper(void* arg) {
  printf("Eval return!\n");
  return arg;
}

dlink_t* dlink(dlink_t* o, int n) {
  dlink_t* a = acid_malloc(sizeof(dlink_t));
  a->prev = NULL; // init vars
  a->next = NULL;

  if (o != NULL) {
    acid_set_field(o, next, a);  // forward
    acid_set_field(a, prev, o);  // back
    if (n == 0) {
      return a;  // base case
    }
  }
  dlink_t* b = dlink(a, n - 1);
  acid_dissolve(a);
  return b;
}

void doubly_linked_list() {
  dlink_t *dl = dlink(NULL, 30);
  dlink_t *o = NULL;
  while (true) {
    o = dl->prev;
    if (o == NULL) break;
    acid_set(dl, o);
  }
  acid_dissolve(dl);
}

void slist_create() {
  slink_t* front _cleanup_acid_dissolve_ = acid_malloc(sizeof(slink_t));
  slink_t* curr = front;
  for (int i = 0; i < 10; i++) {
    slink_t* next _cleanup_acid_dissolve_ = acid_malloc(sizeof(slink_t));
    acid_set_field(curr, next, next);  // curr->next = next
    curr = curr->next;
  }
}

extern atomic_uint_fast64_t acid_world_count;
extern atomic_uint_fast64_t acid_collect_count;

int main(int argc, char** argv) {
  struct timespec wall_start;
  timespec_get(&wall_start, TIME_UTC);
  clock_t start = clock() / (CLOCKS_PER_SEC / 1000);

  acid_setup();

  test1();
  slist_create();
  simplecycle();
  simplecycle2();
  wheel();
  doubly_linked_list();
//  multi_collect();
  clique();
//  benzene_ring_ms_test();
//  object_set_test();

//  for (int x = 1; x <= 8; ++x) {
//    benzene_ring_scalability(x);
//  }
  printf("\n");

  acid_teardown();

  clock_t end = clock() / (CLOCKS_PER_SEC / 1000);

  struct timespec wall_end;
  timespec_get(&wall_end, TIME_UTC);

  struct timespec wall_diff = {
      .tv_nsec = (wall_end.tv_nsec - wall_start.tv_nsec),
      .tv_sec = (wall_end.tv_sec - wall_start.tv_sec)
  };
  printf("Total cpu time elasped: %lf\n", 0.001 * (end - start));
  printf("Total wall time elapsed: %lf\n",  wall_diff.tv_sec + (0.000000001 * wall_diff.tv_nsec));
  printf("Total number of objects created: %zu\n", acid_world_count);
  printf("Total number of objects collected: %zu\n", acid_collect_count);
  exit(0);
}
