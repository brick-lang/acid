#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "complock.h"
#include "locker.h"
#include "lockable.h"

static _Thread_local locker_t *current_locks = NULL;

void locker_setup() {
  assert(current_locks == NULL);
  current_locks = locker_create();
}

void locker_teardown() {
  assert(current_locks != NULL);
  locker_destroy(current_locks);
}

locker_t *locker_create() {
  locker_t *locker = malloc(sizeof(locker_t));
  // TODO: check CC_OK here
  treetable_new(complock_compare, &locker->locks);
  list_new(&locker->stack);
  return locker;
}

void locker_destroy(locker_t *locker) {
  treetable_destroy(locker->locks);
  list_destroy(locker->stack);
  free(locker);
}

typedef struct locker_entry_t {
  size_t size;
  complock_t *ary[4]; // we lock on at max four objects at once
} locker_entry_t;

static locker_entry_t* locker_entry_create() {
  locker_entry_t* le = malloc(sizeof(locker_entry_t));
  le->size = 0;
  return le;
}

static void locker_entry_add(locker_entry_t *le, complock_t* elem) {
  assert(le->size < 4);
  le->ary[le->size++] = elem;
}

static size_t remove_duplicate_complocks(complock_t **nums, size_t count) {
  if (count == 0) return 0;
  size_t r = 0;
  for (size_t i = 1; i < count; i++) {
    if (complock_compare(nums[r], nums[i]) != 0) {
      nums[++r] = nums[i]; // copy-in next unique number
    }
  }
  return r + 1;
}

/**
 * Sort locks and lock them
 * @param locks_count the number of locks passed in
 * @param locks the locks to use
 */
static void locker_start(int locks_count, void *locks[]) {
  locker_t *lk = current_locks;
  assert(lk != NULL);

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

  // Sort the current set of lockables and extract the complocks
  complock_t* filtered_locks[locks_count];
  size_t filtered_locks_count = 0;
  for (int i = 0; i < locks_count; i++) {
    if (locks[i] != NULL) { // remove any nulls
      filtered_locks[filtered_locks_count++] = ((lockable_t *)locks[i])->cmplock;
    }
  }
  qsort(filtered_locks, filtered_locks_count, sizeof(complock_t*), complock_compare);
  filtered_locks_count = remove_duplicate_complocks(filtered_locks, filtered_locks_count);

  locker_entry_t *new_locks = locker_entry_create();
  for (int i = 0; i < filtered_locks_count; i++) {
    complock_t *lock = filtered_locks[i];
    if (treetable_contains_key(lk->locks, lock)) {
      size_t count;
      treetable_get(lk->locks, lock, (void **) &count);
      treetable_add(lk->locks, lock, (void *) (count + 1));
      locker_entry_add(new_locks, lock);
    } else if (!treetable_contains_key(lk->locks, lock)
        && lock->priority < max_priority) {
      treetable_add(lk->locks, lock, (void *) 1);
      complock_lock(lock);
      locker_entry_add(new_locks, lock);
    } else {
      printf("Error! Possible deadlock with group of %zu locks\n", treetable_size(lk->locks));
      abort();
    }
  }
  list_add_last(lk->stack, new_locks);
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
  assert(lk != NULL);

  locker_entry_t *locks = NULL;
  list_remove_last(lk->stack, (void **) &locks); // pop off the stack;

  complock_t *lock = NULL;
  for (int i = 0; i < locks->size; ++i) {
    lock = locks->ary[i];
    size_t count = 0;
    treetable_get(lk->locks, lock, (void **) &count);
    if (count == 1) {
      treetable_remove(lk->locks, lock, NULL);
      complock_unlock(lock);
    } else {
      treetable_add(lk->locks, lock, (void *) (count - 1));
    }
  }
  free(locks);
}
