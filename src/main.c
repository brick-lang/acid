#define DEBUG
#include <here.h>
#include "worker.h"
#include "root.h"

char debugmsg[200];

void world_init() {
  root_setup();
  object_system_setup();
  workers_setup();
}

void simplecycle() {
  root_t a = {0}, b = {0};
  root_alloc(&a);
  root_alloc(&b);

  object_set(a.ref, "x", b.ref);
  object_set(b.ref, "x", a.ref);
  object_set(a.ref, "y", a.ref);


  sprintf(debugmsg, "a = %s", object_to_string(a.ref));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(b.ref));
  HERE_MSG(debugmsg);

  Object *pa = a.ref;
  Object *pb = b.ref;

  root_free(&b);

  sprintf(debugmsg, "a = %s", object_to_string(pa));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(pb));
  HERE_MSG(debugmsg);

  root_free(&a);

  sprintf(debugmsg, "a = %s", object_to_string(pa));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(pb));
  HERE_MSG(debugmsg);
}

void simplecycle2() {
  root_t a = {0}, b = {0};
  root_alloc(&a);
  root_alloc(&b);

  object_set(a.ref, "x", b.ref);
  object_set(b.ref, "x", a.ref);

  sprintf(debugmsg, "a = %s", object_to_string(a.ref));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(b.ref));
  HERE_MSG(debugmsg);

  Object *pa = a.ref;
  Object *pb = b.ref;

  root_free(&b);

  sprintf(debugmsg, "a = %s", object_to_string(pa));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(pb));
  HERE_MSG(debugmsg);

  object_set(a.ref, "y", NULL);

  thrd_sleep(&(struct timespec) {.tv_nsec=3000000}, NULL);

  root_free(&a);

  sprintf(debugmsg, "a = %s", object_to_string(pa));
  HERE_MSG(debugmsg);
  sprintf(debugmsg, "b = %s", object_to_string(pb));
  HERE_MSG(debugmsg);
}

void status() {
  xthread_wait_for_zero_threads();
  object_status();
//  mark_and_sweep();
}

int main(int argc, char** argv) {
  world_init();
  printf("hello world!\n");
  simplecycle();
  status();
}
