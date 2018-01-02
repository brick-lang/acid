#include "collector.h"
#include <stdio.h>
#include "locker.h"
#include "memory.h"
#include "object.h"
#include "task.h"

atomic_size_t num_collector = 0;

collector_t *collector_create() {
  collector_t *collector = xmalloc(sizeof(collector_t), "collector_create");
  collector->forward = NULL;
  *(counter_t**)&collector->count = counter_create();
  safelist_init(&collector->collect, collector->count);
  safelist_init(&collector->merged_list, collector->count);
  safelist_init(&collector->recovery_list, collector->count);
  safelist_init(&collector->rebuild_list, collector->count);
  safelist_init(&collector->clean_list, collector->count);

  idlock_init(&collector->lock);
  num_collector++;
  return collector;
}

static void collector_destroy(collector_t *collector) {
  safelist_deinit(&collector->collect);
  safelist_deinit(&collector->merged_list);
  safelist_deinit(&collector->recovery_list);
  safelist_deinit(&collector->rebuild_list);
  safelist_deinit(&collector->clean_list);
  counter_destroy(collector->count);
  xfree(collector);
}

static void collector_terminate(collector_t *collector) {
  locker_start2(collector, collector->forward);
  collector_set_forward(collector, NULL);
  locker_end();
}

void collector_set_forward(collector_t *collector, collector_t *f) {
  locker_start3(collector, collector->forward, f);
  if (f == collector->forward) {  // check equality based on address, not value
    locker_end();
    return;
  }

  if (f != NULL) {
    counter_inc_ref(f->count);
  }
  if (collector->forward != NULL) {
    counter_dec_ref(collector->forward->count);
  }

  collector->forward = f;
  locker_end();
}

void collector_add_object(collector_t *collector, Object *obj) {
  collector_t *add_to = collector;
  while (true) {
    locker_start2(add_to, obj);
    // fix collector pointers in objects on the fly
    if (add_to->forward != NULL) {
      add_to = add_to->forward;
      locker_end();
      continue;
    }
    obj->count[obj->which]++;
    obj->count[2]++;  // do this here rather than phantomize_node
    // the reason is because a phantom count of one prevents
    // the collector from being set to null
    safelist_add(&add_to->collect, obj);

    // TODO: What if it has a collector already?
    object_set_collector(obj, add_to);
    locker_end();
    break;
  }
}

bool collector_run(void *c) {
  collector_t *collector = c;

  if (counter_continue_waiting(collector->count)) {
    return true;
  }

  // TODO: Change all these while loops to for loops
  locker_start1(collector);
  while (true) {
    Object *obj = safelist_poll(&collector->merged_list);
    if (obj == NULL) break;
    safelist_add(&collector->recovery_list, obj);
  }
  locker_end();

  // Perform the collection
  while (true) {
    Object *obj = safelist_poll(&collector->collect);
    if (obj == NULL) break;
    object_phantomize_node(obj, collector);
    safelist_add(&collector->recovery_list, obj);
  }

  while (true) {
    Object *obj = safelist_poll(&collector->recovery_list);
    if (obj == NULL) break;
    object_recover_node(obj, collector);  // don't need to pass rebuild
    safelist_add(&collector->clean_list, obj);
  }

  while (true) {
    Object *obj = safelist_poll(&collector->rebuild_list);
    if (obj == NULL) break;
    object_recover_node(obj, collector);
  }

  while (true) {
    Object *obj = safelist_poll(&collector->clean_list);
    if (obj == NULL) break;
    object_clean_node(obj);
  }

  locker_start2(collector, collector->forward);
  if (counter_done(collector->count)) {
    collector_terminate(collector);
    locker_end();  // release the collector->lock *before* destruction
    collector_destroy(collector);
    return false;
  }
  locker_end();

  // catch exception, log handling, stack trace, etc. in orig file here.
  return true;
}

static void collector_forward_safelists_to(collector_t *collector,
                                           collector_t *s) {
  // s is forwarding
  // collector is receiving
  locker_start2(collector, s);
  safelist_forward(&collector->collect, &s->collect);
  safelist_forward(&collector->merged_list, &s->merged_list);

  // Don't send to the merged list, that's only for links
  // The collector holds a phantom count too
  safelist_forward(&collector->rebuild_list, &s->rebuild_list);

  safelist_forward(&collector->clean_list, &s->merged_list);
  safelist_forward(&collector->recovery_list, &s->merged_list);
  locker_end();
}

void collector_merge(collector_t *collector, collector_t *s) {
  locker_start2(collector, s);
  collector_forward_safelists_to(s, collector);
  collector_set_forward(s, collector);
  locker_end();
}

/** The only safe way to compare whether two collectors are equal */
bool collector_equals(collector_t *c1, collector_t *c2) {
  bool retval;
  while (true) {
    locker_start2(c1, c2);
    if (c1 == c2) {
      retval = true;
      locker_end();
      break;
    } else if (c1 != NULL && c1->forward != NULL) {
      c1 = c1->forward;
    } else if (c2 != NULL && c2->forward != NULL) {
      c2 = c2->forward;
    } else {
      retval = false;
      locker_end();
      break;
    }
  }
  return retval;
}

collector_t *collector_update(collector_t *collector) {
  collector_t *retval;
  collector_t *c = collector;
  while (true) {
    locker_start1(c);
    if (c->forward == NULL) {
      retval = c;
      locker_end();
      break;
    } else {
      c = c->forward;
    }
    locker_end();
  }
  return retval;
}
