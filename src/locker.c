#include <stdio.h>
#include <stdlib.h>

#include "complock.h"
#include "lockable.h"
#include "locker.h"

static _Thread_local locker_t *current_locks = NULL;

void locker_setup() { current_locks = locker_create(); }

void locker_teardown() { locker_destroy(current_locks); }

locker_t *locker_create() {
  locker_t *locker = malloc(sizeof(locker_t));
  // TODO: check CC_OK here
  list_new(&locker->stack);
  return locker;
}

void locker_destroy(locker_t *locker) {
  list_destroy(locker->stack);
  free(locker);
}

typedef struct locker_entry_t {
  size_t size;
  complock_t *ary[4];  // we lock on at max four objects at once
} locker_entry_t;

static locker_entry_t *locker_entry_create() {
  locker_entry_t *le = malloc(sizeof(locker_entry_t));
  le->size = 0;
  return le;
}

static inline void locker_entry_add(locker_entry_t *le, complock_t *elem) {
  le->ary[le->size++] = elem;
}

static inline size_t remove_duplicate_complocks(complock_t **nums, size_t count) {
  if (count == 0) return 0;
  size_t r = 0;
  for (size_t i = 1; i < count; i++) {
    if (complock_compare(nums[r], nums[i]) != 0) {
      nums[++r] = nums[i]; // copy-in next unique number
    }
  }
  return r + 1;
}

static inline void isort(complock_t *arr[], size_t count) {
  for (int_fast8_t i = 1; i < count; i++) {
    complock_t *key = arr[i];
    int_fast8_t j = i-1;
    while (j >= 0 && complock_compare(arr[j], key) == 1) {
      arr[j+1] = arr[j];
      j--;
    }
    arr[j+1] = key;
  }
}

/**
 * Sort locks and lock them
 * @param locks_count the number of locks passed in (should not ever be more
 * than 4)
 * @param locks the locks to use
 */
static void locker_start(size_t locks_count, void *locks[]) {
  // Sort the current set of lockables and extract the complocks
  complock_t *filtered_locks[locks_count];
  size_t filtered_locks_count = 0;
  for (int i = 0; i < locks_count; i++) {
    if (locks[i] != NULL) {  // remove any nulls
      lockable_t *lockable = locks[i];
      filtered_locks[filtered_locks_count++] = &lockable->lock;
    }
  }
  if (filtered_locks_count > 1) {
    isort(filtered_locks, filtered_locks_count);
    filtered_locks_count = remove_duplicate_complocks(filtered_locks, filtered_locks_count);
  }

  locker_entry_t *new_locks = locker_entry_create();
  for (uint_fast8_t i = 0; i < filtered_locks_count; i++) {
    complock_t *lock = filtered_locks[i];
    complock_lock(lock);
    locker_entry_add(new_locks, lock);
  }
  list_add_last(current_locks->stack, new_locks);
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

  locker_entry_t *locks = NULL;
  list_remove_last(lk->stack, (void **)&locks);  // pop off the stack;

  complock_t *lock = NULL;
  for (int i = 0; i < locks->size; ++i) {
    lock = locks->ary[i];
    complock_unlock(lock);
  }
  free(locks);
}
