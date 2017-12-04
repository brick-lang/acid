#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <here.h>

#include "complock.h"
#include "locker.h"
#include "lockable.h"

static _Thread_local locker_t *current_locks = NULL;

locker_t *locker_create() {
  locker_t *locker = malloc(sizeof(locker_t));
  // TODO: check CC_OK here
  treetable_new(complock_compare, &locker->locks);
  list_new(&locker->stack);
  return locker;
}

void locker_destroy(locker_t *locker) {
  treetable_destroy(locker->locks);
  list_destroy_cb(locker->stack, (void (*)(void *)) treeset_destroy);
  free(locker);
}

/**
 * Sort locks and lock them
 * @param locks_count the number of locks passed in
 * @param locks the locks to use
 */
void locker_start(int locks_count, void *locks[]) {
  locker_t *lk = current_locks;
  if (lk == NULL) {
    lk = locker_create();
    current_locks = lk;
  }

  // Determine the maximum priority of existing locks
  // All new locks must have lower priority than maxPriority.
  int max_priority = INT_MAX;
  if (treetable_size(lk->locks) > 0) {
    TREETABLE_FOREACH(locks_entry, lk->locks, {
      complock_t *const lock = locks_entry.key;
      if (lock->priority < max_priority) {
        max_priority = lock->priority;
      }
    })
  }

  // Sort the current set of locks
  TreeSet *tlocks = NULL; // TreeSet<complock_t>
  treeset_new(complock_compare, &tlocks);
  for (int i = 0; i < locks_count; i++) {
    void *lock = locks[i];
    if (lock != NULL && ((lockable_t *) lock)->cmplock != NULL) {
      treeset_add(tlocks, ((lockable_t *) lock)->cmplock);
    }
  }

  TreeSet *new_locks = NULL; // TreeSet<complock_t>
  treeset_new(complock_compare, &new_locks);
  list_add_last(lk->stack, new_locks);

  //TREESET_FOREACH(lock, tlocks, {
  TreeSetIter treeSetIter;
  treeset_iter_init(&treeSetIter, tlocks);
  complock_t *lock;
  while (treeset_iter_next(&treeSetIter, (void **) &lock) != CC_ITER_END) {
    if (treetable_contains_key(lk->locks, lock)) {
      size_t count;
      treetable_get(lk->locks, lock, (void **) &count);
      treetable_add(lk->locks, lock, (void *) (count + 1));
      treeset_add(new_locks, lock);
    } else if (!treetable_contains_key(lk->locks, lock)
        && lock->priority < max_priority) {
      treetable_add(lk->locks, lock, (void *) 1);
      complock_lock(lock);
      treeset_add(new_locks, lock);
    } else {
      int num = 0;
      TREETABLE_FOREACH(ll, lk->locks, fprintf(stderr, "NUM=%d\n", num++););
      char *str = calloc(sizeof(char), 500);
      sprintf(str, "Error! Possible deadlock %s\n", complock_to_string(lock)); // + " " + lk->locks
      HERE_MSG(str);
      quick_exit(-1);
    }
  }
  //});
  treeset_destroy(tlocks);
}

void locker_start1(void *lock) {
  void *locks[] = {lock};
  locker_start(1, locks);
}

void locker_start2(void *lock1, void *lock2) {
  void *locks[] = {lock1, lock2};
  locker_start(2, locks);
}

void locker_start3(void *lock1, void *lock2, void *lock3) {
  void *locks[] = {lock1, lock2, lock3};
  locker_start(3, locks);
}

void locker_start4(void *lock1, void *lock2, void *lock3, void *lock4) {
  void *locks[] = {lock1, lock2, lock3, lock4};
  locker_start(4, locks);
}

/**
 * Unlock the set of locks obtained by the last
 * call to start().
 */
void locker_end() {
  locker_t *lk = current_locks;
  TreeSet *locks = NULL;
  list_remove_last(lk->stack, (void **) &locks); // pop off the stack;
  TREESET_FOREACH(lock, locks, {
    size_t count;
    treetable_get(lk->locks, lock, (void **) &count);
    if (count == 1) {
      treetable_remove(lk->locks, lock, NULL);
      complock_unlock(lock);
    } else {
      treetable_add(lk->locks, lock, (void *) (count - 1));
    }
  });
  treeset_destroy(locks);
}
