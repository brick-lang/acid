#include "../lib/collectc/include/treetable.h"
#include "../lib/collectc/include/list.h"
#ifndef LOCKER_H
#define LOCKER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The locker manages locks in a way that keeps the code safe
 * from deadlocks and other bad behavior. A thread can only obtain
 * more locks if (1) it doesn't have any yet, or (2) the new locks
 * have lower "priority" than any locks already obtained. */
typedef struct locker_t {
  TreeTable *locks;  // TreeMap<complock_t*,int>
  List *stack;       // List<locker_entry_t*>
} locker_t;

void locker_setup();
void locker_teardown();
locker_t *locker_create();
void locker_destroy(locker_t *);
void locker_start1(void *lock);
void locker_start2(void *lock1, void *lock2);
void locker_start3(void *lock1, void *lock2, void *lock3);
void locker_start4(void *lock1, void *lock2, void *lock3, void *lock4);
void locker_end();

#ifdef __cplusplus
}
#endif

#endif //LOCKER_H
