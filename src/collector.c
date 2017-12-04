#include <assert.h>
#include <object.h>
#include "collector.h"
#include "locker.h"
#include "idbaseobject.h"
#include "here.h"

char collstr[200];

collector_t *collector_create() {
  collector_t *collector = (collector_t *) malloc(sizeof(collector_t));
  idbaseobject_init((IdBaseObject *) collector);
  collector->terminated = false;
  collector->forward = NULL;
  *(counter_t **) &collector->count = counter_create();
  *(safelist_t **) &collector->collect = safelist_create(collector->count);
  *(safelist_t **) &collector->merged_list = safelist_create(collector->count);
  *(safelist_t **) &collector->recovery_list = safelist_create(collector->count);
  *(safelist_t **) &collector->rebuild_list = safelist_create(collector->count);
  *(safelist_t **) &collector->clean_list = safelist_create(collector->count);

  HERE();
  *(complock_t **) &collector->lock = complock_create(PRIORITY_COLLECTOR, collector->id);
  HERE();

  return collector;
}

void collector_destroy(collector_t *collector) {
  counter_destroy(collector->count);
  safelist_destroy(collector->collect);
  safelist_destroy(collector->merged_list);
  safelist_destroy(collector->recovery_list);
  safelist_destroy(collector->rebuild_list);
  safelist_destroy(collector->clean_list);

  HERE();
  complock_destroy(collector->lock);
  HERE();

  free(collector);
}

void collector_check_terminated(collector_t *collector) {
  locker_start1(collector);
  assert(!collector->terminated);
  locker_end();
}

void collector_terminate(collector_t *collector) {
  locker_start2(collector, collector->forward);
  collector->terminated = true;
  collector_set_forward(collector, NULL);
}

void collector_set_forward(collector_t *collector, collector_t *f) {
  locker_start3(collector, collector->forward, f);
  if (collector_equals(f, collector->forward)) {
    locker_end();
    return;
  }

  if (f!=NULL)
    counter_inc_ref(f->count);
  if (collector->forward!=NULL)
    counter_dec_ref(collector->forward->count);
  if (f!=NULL)
    assert(collector->forward==NULL); // TODO: Make sure this is *actually* needed

  collector->forward = f;
  locker_end();
}

char *collector_to_string(collector_t *collector) {
  sprintf(collstr, "#<Collector:%p id:%d collector_size:%zu rec_size:%zu forward_id:%d>",
          (void*)collector, collector->id, safelist_size(collector->collect),
          safelist_size(collector->merged_list),
          (collector->forward == NULL ? -1 : (int)collector->forward->id));
  return (char*)collstr;
}

void collector_add_object(collector_t *collector, Object *obj) {
  collector_t *add_to = collector;
  while (true) {
    locker_start2(add_to, obj);
    assert (!add_to->terminated); //: count
    //fix collector pointers in objects on the fly
    if (add_to->forward!=NULL) {
      add_to = add_to->forward;
      locker_end();
      continue;
    }
    // Here.here();
    obj->count[obj->which]++;
    obj->count[2]++; // to this here rather than phantomize_node
    // the reason is because a phantom count of one prevents
    // the collector from being set to null
    safelist_add(add_to->collect, obj);
    object_set_collector(obj, add_to); // TODO: What if it has a collector already?
    locker_end();
    break;
  }
}

bool __collector_request_delete_xthread_run(Object *obj) {
  object_die(obj);
  return false;
}

void collector_request_delete(Object *const obj) {
  // obj->where = new Throwable;
  // assert so.where == null;
  xthread_t *xthrd = (xthread_t*)malloc(sizeof(xthread_t));
  xthrd->run = (bool (*)(void *)) __collector_request_delete_xthread_run;
  xthrd->runarg = obj;
  xthread_start(xthrd);
}

bool collector_run(collector_t* collector) {
  if (counter_continue_waiting(collector->count)) {
    return true;
  }

  // TODO: Change all these while loops to for loops
  locker_start1(collector);
  while (true) {
    Object *obj = safelist_poll(collector->merged_list);
    if (obj == NULL) break;
    safelist_add(collector->recovery_list, obj);
  }
  locker_end();

  // Perform the collection
  while(true) {
    Object *obj = safelist_poll(collector->collect);
    if (obj == NULL) break;
    object_phantomize_node(obj, collector);
    safelist_add(collector->recovery_list, obj);
  }

  while (true) {
    Object *obj = safelist_poll(collector->recovery_list);
    if (obj == NULL) break;
    object_recover_node(obj,collector->rebuild_list,collector); // don't need to pass rebuild
    safelist_add(collector->clean_list, obj);
  }

  while (true) {
    Object *obj = safelist_poll(collector->rebuild_list);
    if (obj == NULL) break;
    object_recover_node(obj,collector->rebuild_list,collector);
  }

  while (true) {
    Object *obj = safelist_poll(collector->clean_list);
    if (obj == NULL) break;
    object_clean_node(obj, collector);
  }

  sprintf(collstr, "Done one cycle %s m=%zu,rc=%zu,cl=%zu,co=%zu,r=%zu",
          counter_to_string(collector->count),
          safelist_size(collector->merged_list),
          safelist_size(collector->recovery_list),
          safelist_size(collector->clean_list),
          safelist_size(collector->collect),
          safelist_size(collector->rebuild_list));

  locker_start2(collector, collector->forward);
  bool done = counter_done(collector->count);
  if (done) {
    collector_terminate(collector);
    locker_end();
    HERE_PREFIX_MSG("Terminated ", collector_to_string(collector));
    return false;
  }
  locker_end();

  // catch exception, log handling, stack trace, etc. in orig file here.
  HERE_PREFIX_MSG("Terminated ", collector_to_string(collector));
  return true;
}

void collector_forward_safelists_to(collector_t *collector, collector_t *s) {
  // s is forwarding
  // collector is receiving
  locker_start2(collector,s);
  safelist_forward(collector->collect, s->collect);
  safelist_forward(collector->merged_list, s->merged_list);

  // Don't send to the merged list, that's only for links
  // The collector holds a phantom count too
  safelist_forward(collector->rebuild_list, s->rebuild_list);

  safelist_forward(collector->clean_list, s->merged_list);
  safelist_forward(collector->recovery_list, s->merged_list);
  locker_end();
}

void collector_merge(collector_t *collector, collector_t *s) {
  locker_start2(collector, s);
  HERE_PREFIX_MSG("S=", collector_to_string(s));
  HERE_PREFIX_MSG("T=", collector_to_string(collector));
  collector_forward_safelists_to(s, collector);
  collector_set_forward(s,collector);
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

collector_t *collector_update(collector_t *collector){
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

