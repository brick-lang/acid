#include "object.h"
#include <assert.h>
#include <stdatomic.h>
#include "collector.h"
#include "idbaseobject.h"
#include "link.h"
#include "lockable.h"
#include "locker.h"

#define BIT_FLIP(b) ((b) ^ 1)

atomic_size_t world_count;

void object_set_collector(Object *obj, collector_t *c) {
  locker_start1(obj);
  if (obj->collector != c) {
    if (c != NULL) {
      (&c->count)->ref_count++;
    }
    if (obj->collector != NULL) {
      (&obj->collector->count)->ref_count--;
    }
    obj->collector = c;
  }
  locker_end();
}

Object *object_init(Object *obj) {
  idbaseobject_init((IdBaseObject *)obj);
  hashtable_new(&obj->links);
  obj->lock = complock_create(PRIORITY_OBJECT, obj->id);
  obj->which = 0;
  obj->phantomized = false;
  obj->count[0] = 0;
  obj->count[1] = 0;
  obj->count[2] = 0;
  obj->collector = NULL;
  obj->phantomization_complete = true;
  obj->data = NULL;
  world_count++;
  return obj;
}

Object *object_create() {
  Object *obj = malloc(sizeof(Object));
  return object_init(obj);
}

Object *object_create_strong() {
  Object *o = object_create();
  object_inc_strong(o);
  return o;
}

Object *object_init_strong(Object *o) {
  object_init(o);
  object_inc_strong(o);
  return o;
}


static void object_del(Object *obj) {
  lockable_t lockobj = {.id = 0, .cmplock = obj->lock};
  locker_start1(&lockobj);
  object_set_collector(obj, NULL);
  assert(hashtable_size(obj->links) == 0);  // make *sure* there are no links
  hashtable_destroy(obj->links);
  //if (obj->data != NULL) { free(obj->data); }
  free(obj);  // free the memory
//  world_count--;
  locker_end();
  complock_destroy(lockobj.cmplock);
}

// Pseudo: Delete
static void object_die(Object *obj) {
  // TODO: Get rid of needing this (do destruction in the HASHTABLE_FOREACH)
  link_t **lks = NULL;  // Array<link_t*>
  size_t lks_size = 0;

  locker_start1(obj);
  if (hashtable_size(obj->links) > 0) {
    lks = malloc(sizeof(link_t *) * hashtable_size(obj->links));
    HASHTABLE_FOREACH(entry, obj->links, { lks[lks_size++] = entry->value; })
  }
  hashtable_remove_all(obj->links);
  assert(obj->count[0] == 0);
  assert(obj->count[1] == 0);
  bool has_phantoms = (obj->count[2] > 0);
  locker_end();

  if (lks != NULL && lks_size > 0) {
    for (size_t i = 0; i < lks_size; i++) {
      link_destroy(lks[i]);
    }
    free(lks);
  }

  if (!has_phantoms) {
    object_del(obj);
  }
}

static bool __object_request_delete_task_run(void *obj) {
  object_die(obj);
  return false;  // Don't run this operation again.
}

static void object_request_delete(Object *const obj) {
  task_t *task = malloc(sizeof(task_t));
  task->run = __object_request_delete_task_run;
  task->runarg = obj;
  task_start(task);
}

void object_dec(Object *obj, bit_t w) {
  locker_start1(obj);
  obj->count[w]--;
  assert(obj->count[w] >= 0);  // : object_to_string(obj);
  if (obj->count[w] == 0 && w == obj->which) {
    if (obj->count[BIT_FLIP(obj->which)] == 0 && obj->count[2] == 0) {
      object_request_delete(obj);
    } else {
      if (obj->collector == NULL) {
        object_set_collector(obj, collector_create());
        collector_add_object(obj->collector, obj);

        // create and start a new collection task
        task_t *task = task_create();
        task->run = collector_run;
        task->runarg = obj->collector;
        task_start(task);
      } else {
        collector_add_object(obj->collector, obj);
      }
    }
  }
  locker_end();
}

/**
 * dec_phantom is responsible for removing any
 * reference to the collector.
 *
 * Pseudo: DecPhantom
 */
void object_dec_phantom(Object *obj) {
  locker_start1(obj);

  obj->count[2]--;  // decrease the phantom count
  assert(obj->count[2] >= 0);

  if (obj->count[2] == 0) {
    if (obj->count[0] == 0 && obj->count[1] == 0) {
      object_request_delete(obj);
    } else {
      object_set_collector(obj, NULL);
    }
  }
  locker_end();
}

void object_dec_strong(Object *obj) { object_dec(obj, obj->which); }

void object_inc_strong(Object *obj) {
  locker_start1(obj);
  obj->count[obj->which]++;
  locker_end();
}

/**
 * Pseudo: PhantomizeNode
 *
 * The collector takes away the strong link it made in add_to_collector()
 * @param obj
 * @param cptr
 */
void object_phantomize_node(Object *obj, struct collector_t *cptr) {
  // Determine if merge needs to be done.
  collector_t *c = cptr;
  collector_t *t = obj->collector;
  if (t != NULL) {
    while (true) {
      locker_start2(t, c);
      if (c == t) {
        locker_end();
        break;
      }
      assert(c->forward != NULL || t->forward != NULL);
      if (c->forward != NULL) c = c->forward;
      if (t->forward != NULL) t = t->forward;
      locker_end();
    }
  }

  locker_start1(obj);
  object_set_collector(obj, collector_update(obj->collector));

  assert(t != NULL);

  obj->count[obj->which]--;
  assert(obj->count[obj->which] >= 0);  //: object_to_string(obj);

  if (obj->count[obj->which] > 0) {
    locker_end();
    return;  // fast exit
  }

  bool phantomize = false;
  link_t **phantoms = NULL;  // Array<link_t*>
  size_t phantoms_size = 0;

  if (obj->count[BIT_FLIP(obj->which)] > 0) {
    obj->which ^= 1;  /// flip the which bit atomically.
  }
  // phantom count is > 0, but weak and strong are zero.
  if (!obj->phantomized) {
    phantomize = true;
    obj->phantomized = true;
    assert(obj->collector != NULL);
    obj->phantomization_complete = false;
    if (hashtable_size(obj->links) > 0) {
      phantoms = malloc(sizeof(link_t *) * hashtable_size(obj->links));
      HASHTABLE_FOREACH(entry, obj->links,
                        { phantoms[phantoms_size++] = entry->value; })
    }
  }
  locker_end();

  if (phantomize) {
    // Between here and the following syncs
    // the actual contents of the links could
    // be changed. New links could be added
    // and existing links could be redirected
    // or cleared.
    link_t *lk = NULL;
    for (size_t i = 0; i < phantoms_size; i++) {
      lk = phantoms[i];
      assert(lk != NULL);
      locker_start2(obj, lk->target);
      assert(obj->phantomized);
      if (!lk->phantomized) {
        assert(lk->src->phantomized);
        assert(lk->src->collector != NULL);
        link_phantomize(lk);
      }
      locker_end();
    }
    free(phantoms);

    locker_start1(obj);
    obj->phantomization_complete = true;
    locker_end();
  }
}

/**
 * Pseudo: Rebuild
 * @param rebuildNext
 * @param lk
 */
static void object_rebuild_link(safelist_t *rebuild_next, link_t *const lk) {
  locker_start2(lk->src, lk->target);
  if (lk->phantomized) {
    if (lk->target == lk->src) {
      lk->which = BIT_FLIP(lk->target->which);
    } else if (lk->target->phantomized ||
               hashtable_size(lk->target->links) == 0) {
      lk->which = lk->target->which;
    } else {
      lk->which = BIT_FLIP(lk->target->which);
    }

    lk->target->count[lk->which]++;
    lk->target->count[2]--;

    if (lk->target->count[2] == 0) {
      object_set_collector(lk->target, NULL);
    }
    lk->phantomized = false;

    assert(lk->target->count[2] >= 0);
    if (lk->target->collector != NULL) {
      safelist_add(rebuild_next, lk->target);
    }
  }
  locker_end();
}

void object_recover_node(Object *obj, collector_t *cptr) {
  // Determine if merge needs to be done.
  collector_t *c = cptr;
  collector_t *t = obj->collector;
  if (t != NULL) {
    while (true) {
      locker_start2(t, c);
      if (c == t) {
        locker_end();
        break;
      }
      assert(c->forward != NULL || t->forward != NULL);
      if (c->forward != NULL) c = c->forward;
      if (t->forward != NULL) t = t->forward;
      locker_end();
    }
  }

  char **rebuild = NULL;  // Array<String>
  size_t rebuild_size = 0;

  locker_start1(obj);
  if (obj->collector != NULL)
    object_set_collector(obj, collector_update(obj->collector));

  if (obj->count[obj->which] > 0) {
    int wcount = 0;
    while (obj->phantomized && !obj->phantomization_complete) {
      locker_end();
      thrd_sleep(&(struct timespec){.tv_nsec = 1000000},
                 NULL);  // sleep for 1ms TODO: maybe change duration?
      locker_start1(obj);
      wcount++;
      assert(wcount < 10000);
    }
    assert(obj->phantomization_complete);
    obj->phantomized = false;
    if (hashtable_size(obj->links) > 0) {
      rebuild = malloc(sizeof(link_t *) * hashtable_size(obj->links));
      HASHTABLE_FOREACH(entry, obj->links,
                        { rebuild[rebuild_size++] = entry->key; })
    }
  } else {
    assert(obj->count[BIT_FLIP(obj->which)] == 0);
  }
  locker_end();

  if (rebuild != NULL) {
    link_t *lk = NULL;
    for (size_t i = 0; i < rebuild_size; i++) {
      hashtable_get(obj->links, rebuild[i], (void **)&lk);
      assert(lk != NULL);
      object_rebuild_link(&cptr->rebuild_list, lk);
    }
    free(rebuild);
  }

  locker_start1(obj);
  if (obj->count[2] == 0) {
    assert(obj->collector == NULL);
  }
  locker_end();
}

/**
 * Pseudo: CleanNode
 *
 * After deleting all the outgoing links, decrement
 * the phantom count by one (i.e. the reference held
 * by the collector itself). Wehn the last phantom
 * count is gone, the object is cleaned up.
 *
 * @param obj the Object to clean
 */
void object_clean_node(Object *obj) {
  bool die = false;
  locker_start1(obj);
  assert(obj->count[0] >= 0);
  assert(obj->count[1] >= 0);
  if (obj->count[0] == 0 && obj->count[1] == 0) {
    die = true;
  }
  locker_end();

  if (die) object_die(obj);

  object_dec_phantom(obj);
}

/**
 * Pseudo: MergeCollectors
 *
 * When two collector threads realize they are
 * managing a common subset of objects, one defers
 * to the other.
 *
 * @param obj
 * @param target
 * @return true if a merge really happened
 */
bool object_merge_collectors(Object *const obj, Object *target) {
  collector_t *s = obj->collector;
  collector_t *t = target->collector;
  if (t == NULL && s != NULL) {
    object_set_collector(target, s);
  }
  if (t != NULL && s == NULL) {
    object_set_collector(obj, t);
  }
  if (t == s || t == NULL || s == NULL) {
    return false;
  }

  assert(t != NULL);
  assert(s != NULL);

  while (true) {
    locker_start4(t, s, obj, target);
//    assert(!t->terminated);
//    assert(!s->terminated);
    if (t->forward == s && s->forward == NULL) {
      object_set_collector(target, s);
      object_set_collector(obj, s);
      locker_end();
      return false;
    } else if (s->forward == t && t->forward == NULL) {
      object_set_collector(target, t);
      object_set_collector(obj, t);
      locker_end();
      assert(t != NULL);
      return false;
    } else if (t->forward != NULL) {
      t = t->forward;
    } else if (s->forward != NULL) {
      s = s->forward;
    } else if (s == t) {
      locker_end();
      assert(t != NULL);
      return false;
    } else if (s != t) {
      assert(s->forward == NULL);
      assert(t->forward == NULL);
      // We could do this the other way around
      collector_merge(t, s);

      assert(s->forward == t);
      object_set_collector(target, t);
      object_set_collector(obj, t);
      locker_end();
      assert(t != NULL);
      return true;
    }
    locker_end();
  }
}

Object *object_get(Object *obj, char *field) {
  Object *retval = NULL;

  locker_start1(obj);
  link_t *lk = NULL;
  hashtable_get(obj->links, field, (void **)&lk);
  if (lk != NULL) retval = lk->target;
  locker_end();

  return retval;
}

// Pseudo: LinkSet
void object_set(Object *obj, char *field, Object *referent) {
  link_t *old_link = NULL;

  locker_start2(obj, referent);
  hashtable_get(obj->links, field, (void **)&old_link);

  if (referent == NULL) {
    hashtable_remove(obj->links, field, NULL);
    locker_end();
    if (old_link != NULL) link_dec(old_link);
    return;
  }

  if (old_link != NULL && old_link->target == referent) {
    free(old_link);
    old_link = NULL;
    locker_end();
    return;
  }

  link_t *lk = link_create(obj);
  hashtable_add(obj->links, field, lk);
  lk->target = referent;

  if (obj->phantomized) {
    object_merge_collectors(obj, referent);
    assert(lk->src->collector != NULL);
    lk->phantomized = true;
    lk->target = referent;
    lk->target->count[2]++;
  } else if (lk->target == obj) {
    // self-references *must* be weak
    lk->which = BIT_FLIP(referent->which);
    lk->target->count[lk->which]++;
  } else if (hashtable_size(referent->links) == 0) {
    lk->which = referent->which;
    lk->target->count[lk->which]++;
  } else {
    lk->which = BIT_FLIP(referent->which);
    lk->target->count[lk->which]++;
  }
  locker_end();
  if (old_link != NULL) link_dec(old_link);
}
