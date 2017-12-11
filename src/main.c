#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include "acid.h"

#define HERE_MSG(msg)                                                       \
  (printf("here: %s(%s:%d) %s\n", __func__, strrchr("/" __FILE__, '/') + 1, \
          __LINE__, msg))

// static List *keylist = NULL;

typedef struct point_t {
  void* x;
  void* y;
} Point;

void simplecycle() {
  Point* acid_new(a, sizeof(Point));
  Point* acid_new(b, sizeof(Point));

  acid_set_field(a, x, b);
  acid_set_field(b, x, a);
  acid_set_field(a, y, a);
}

void simplecycle2() {
  Point* acid_new(a, sizeof(Point));
  Point* b = acid_malloc(sizeof(Point));

  acid_set_field(a, x, b);
  acid_set_field(b, x, a);

  acid_dissolve(b);

  acid_set_field(a, y, NULL);
}

typedef struct slink {
  struct slink* next;
} slink_t;

void wheel() {
  slink_t* acid_new(wheel, sizeof(slink_t));

  slink_t* prev = NULL;
  acid_set(prev, wheel);

  const int n = 499;
  for (int i = 0; i <= n; i++) {
    slink_t* acid_new(x, sizeof(slink_t));
    acid_set_field(prev, next, x);
    acid_set(prev, x);
    if (i == n) {
      acid_set_field(x, next, wheel);  // close the loop
    }
  }
  acid_dissolve(prev);
}

// void multi_collect() {
//  root_t *a1 = root_create();
//  root_alloc(a1);
//  root_t *prev = root_create();
//  root_set(prev, a1->ref);
//
//  root_t *x = NULL;
//  const int n = 1;
//  for (int i = 0; i <= n; i++) {
//    x = root_create();
//    root_alloc(x);
//    object_set(prev->ref, "next", x->ref);
//    root_set(prev, x->ref);
//    root_free(x);
//  }
//  HERE_MSG("First chain created");
//
//  root_t *a2 = root_create();
//  root_alloc(a2);
//  root_t *prev2 = root_create();
//  root_set(prev2, a2->ref);
//  const int nt = 1;
//  for (int i = 0; i <= nt; i++) {
//    x = root_create();
//    root_alloc(x);
//    object_set(prev2->ref, "next", x->ref);
//    root_set(prev2, x->ref);
//    root_free(x);
//  }
//  HERE_MSG("Second chain created");
//
//  object_set(prev->ref, "n", a2->ref);
//  object_set(prev2->ref, "m", a1->ref);
//  HERE_MSG("Chains joined");
//
//  root_free(prev);
//  root_free(prev2);
//  root_free(a1);
//  root_free(a2);
//}
//
// root_t *dlink(Object *o, int n) {
//  root_t *a = root_create();
//  root_alloc(a);
//  if (o != NULL) {
//    object_set(o, "next", a->ref);  // forward
//    object_set(a->ref, "prev", o);  // back
//    if (n == 0) {
//      return a;  // base case
//    }
//  }
//  root_t *b = dlink(a->ref, n - 1);
//  root_free(a);
//  return b;
//}
//
// void doubly_linked_list() {
//  root_t *dl = dlink(NULL, 30);
//  Object *o = NULL;
//  while (true) {
//    o = object_get(dl->ref, "prev");
//    if (o == NULL) break;
//    root_set(dl, o);
//  }
//  root_free(dl);
//}

// void clique() {
//  int n = 9;
//  root_t *a[n];
//  for (int i = 0; i < n; i++) {
//    a[i] = root_create();
//    root_alloc(a[i]);
//  }
//
//  for (int i = 0; i < n; i++) {
//    for (int j = 0; j < n; j++) {
//      if (i != j) {
//        char *key = calloc(7, sizeof(char));
//        snprintf(key, 6, "%dx%d", i + 1, j + 1);
//        list_add(keylist, key);
//        object_set(a[i]->ref, key, a[j]->ref);
//      }
//    }
//  }
//
//  for (int i = n - 1; i >= 0; i--) {
//    root_free(a[i]);
//  }
//}

// void benzene_ring_scalability(int size) {
//  int n = size * 16;
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
//  for (int i = 0; i < n - 1; ++i) {
//    object_set(a[i][1]->ref, "next1", a[i + 1][5]->ref);
//  }
//  for (int i = n - 1; i > 0; --i) {
//    object_set(a[i][4]->ref, "next2", a[i - 1][2]->ref);
//  }
//
//  for (int i = 0; i < n; ++i) {
//    for (int j = 1; j < 6; ++j) {
//      root_free(a[i][j]);
//      a[i][j] = NULL;
//    }
//  }
//
//  // printf("Benzene x%d constructed.\n", n);
//  printf(".");
//  fflush(stdout);
//  for (int i = 0; i < n; ++i) {
//    root_free(a[i][0]);
//    a[i][0] = NULL;
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

// int main(int argc, char **argv) {
//  struct timespec wall_start;
//  timespec_get(&wall_start, TIME_UTC);
//  clock_t start = clock() / (CLOCKS_PER_SEC / 1000);
//
//  world_init();
//  printf("hello world!\n");
//  simplecycle();
//  simplecycle2();
//  wheel();
//  multi_collect();
//  doubly_linked_list();
//  clique();
//  benzene_ring_ms_test();
//  object_set_test();
//
//  for (int x = 1; x <= 256; ++x) {
//    //    clock_t st = clock() / (CLOCKS_PER_SEC / 1000);
//    benzene_ring_scalability(x);
//    //    task_wait_for_zero_threads();
//    //    clock_t e = clock() / (CLOCKS_PER_SEC / 1000);
//    //    printf("Total time for %d is %lf\n", x, 0.001*(e-st));
//  }
//  printf("\n");

//  task_wait_for_zero_threads();
//  world_teardown();
//
//  clock_t end = clock() / (CLOCKS_PER_SEC / 1000);
//
//  struct timespec wall_end;
//  timespec_get(&wall_end, TIME_UTC);
//
//  struct timespec wall_diff = {
//      .tv_nsec = (wall_end.tv_nsec - wall_start.tv_nsec),
//      .tv_sec = (wall_end.tv_sec - wall_start.tv_sec)};

//  printf("Total cpu time elasped: %lf\n", 0.001 * (end - start));
//  double wall_diff_combined =
//      wall_diff.tv_sec + (0.000000001 * wall_diff.tv_nsec);
//  printf("Total wall time elapsed: %lf\n", wall_diff_combined);
//  printf("Total number of objects in system: %zu\n", world_count);
//  printf("Total number of collectors created: %zu\n", num_collector);
//  exit(0);
//}

void test1() {
  int acid_new(*i, sizeof(int));
  // int *i _cleanup_acid_dissolve_ = acid_malloc(sizeof(int));
  *i = 42;
}
//
// root_t *dlink(Object *o, int n) {
//  root_t *a = root_create();
//  root_alloc(a);
//  if (o != NULL) {
//    object_set(o, "next", a->ref);  // forward
//    object_set(a->ref, "prev", o);  // back
//    if (n == 0) {
//      return a;  // base case
//    }
//  }
//  root_t *b = dlink(a->ref, n - 1);
//  root_free(a);
//  return b;
//}
//
// void doubly_linked_list() {
//  root_t *dl = dlink(NULL, 30);
//  Object *o = NULL;
//  while (true) {
//    o = object_get(dl->ref, "prev");
//    if (o == NULL) break;
//    root_set(dl, o);
//  }
//  root_free(dl);

slink_t* slist_create() {
  slink_t* acid_new(front, sizeof(slink_t));
  slink_t* curr = front;
  for (int i = 0; i < 10; i++) {
    slink_t* acid_new(next, sizeof(slink_t));
    acid_set_field(curr, next, next);  // curr->next = next
    curr = curr->next;
  }
  return front;
}

extern atomic_size_t world_count;
extern atomic_size_t collect_count;

int main(int argc, char** argv) {
  acid_setup();
  test1();
  slist_create();
  simplecycle();
  simplecycle2();
  wheel();

  acid_teardown_blocking();
  printf("Total number of objects created: %zu\n", atomic_load(&world_count));
  printf("Total number of objects collected: %zu\n",
         atomic_load(&collect_count));
  exit(0);
}
