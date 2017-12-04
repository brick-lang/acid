#include <limits.h>
#include <assert.h>
#include <callback.h>
#include "idbaseobject.h"
#include "link.h"
#include "here.h"
#include "lockable.h"
#include "object.h"
#include "collector.h"
#include "locker.h"
#include "../lib/collectc/include/hashset.h"

static complock_t *objects_lock = NULL;
static HashSet *objects = NULL; // Set<Object>
volatile static int objectlive = 0;
static lockable_t *objects_lockable = NULL;

static char objstr[120];

void object_set_collector(Object *obj, collector_t *c) {
  locker_start1(obj);
  if (obj->collector != c) {
    if (c != NULL) counter_inc_ref(c->count);
    if (obj->collector != NULL) counter_dec_ref(obj->collector->count);
    obj->collector = c;
  }
  locker_end();
}

void object_system_setup() {
  if (objects_lock != NULL || objects != NULL || objects_lock != NULL) {
    return;
  }
  objects_lock = complock_create(PRIORITY_GLOBAL, UINT_MAX);
  hashset_new(&objects);
  objectlive = 0;
  objects_lockable = malloc(sizeof(lockable_t));
  objects_lockable->cmplock = objects_lock;
}

Object *object_create() {
  Object *obj = malloc(sizeof(Object));
  idbaseobject_init((IdBaseObject *) obj);
  hashtable_new(&obj->links);
  obj->lock = complock_create(PRIORITY_OBJECT, obj->id);
  obj->which = BIT_ZERO;
  obj->phantomized = false;
  obj->deleted = false;
  obj->count[0] = 0;
  obj->count[1] = 0;
  obj->count[2] = 0;
  obj->collector = NULL;
  obj->mark = false;
  obj->recovered = false;
  obj->phantomization_complete = true;

  locker_start1(objects_lockable);
  hashset_add(objects, obj);
  locker_end();
  return obj;
}

void object_inc_strong(Object *obj) {
  locker_start1(obj);
  assert(!obj->deleted);
  obj->count[obj->which]++;

  HERE_MSG(object_to_string(obj));

  locker_end();
}

void object_dec(Object *obj, bit_t w) {
  locker_start1(obj);
  assert(!obj->deleted);
  obj->count[w]--;
  assert(obj->count[w] >= 0); // : object_to_string(obj);
  if (obj->count[w] == 0 && w == obj->which) {
    if (obj->count[bit_flip(obj->which)] == 0 && obj->count[2] == 0) {
      collector_request_delete(obj);
    } else {
      if (obj->collector == NULL) {
        object_set_collector(obj, collector_create());
        HERE();
        collector_add_object(obj->collector, obj);
        obj->collector->th = xthread_create();
        obj->collector->th->run = alloc_callback(collector_run, obj->collector);
        HERE();
        xthread_start(obj->collector->th);
        HERE_MSG("collector created");
      } else {
        HERE_MSG(object_to_string(obj));
        collector_add_object(obj->collector, obj);
      }
    }
  }
  locker_end();
}

void object_dec_phantom(Object *obj) {
  locker_start1(obj);
  assert(!obj->deleted);
  obj->count[2]--; // decrease the object's phantom count
  assert(obj->count[2] >= 0);
  if (obj->count[2] == 0) {
    if (obj->count[0] == 0 && obj->count[1] == 0) {
      collector_request_delete(obj);
    } else {
      object_set_collector(obj, NULL);
    }
  }
  locker_end();
}

void object_dec_strong(Object *obj) {
  object_dec(obj, obj->which);
}

//#ifdef DEBUG
void object_del(Object *obj) {
  locker_start1(obj);
//  if (obj->deleted && obj->where != NULL) {
//    where.printStackTrace();
//  }
  assert(!obj->deleted);
  obj->deleted = true;
  object_set_collector(obj, NULL);

  HERE_MSG("Deleted:");
  HERE_MSG(object_to_string(obj));
  locker_end();
}
//#else
//void object_del(Object *obj) {
//  lockable_t lockobj = { .id = 0, .cmplock = obj->lock};
//  locker_start1(&lockobj);
//  assert(hashtable_size(obj->links) == 0); // make *sure* there are no links
//  free(obj); // free the memory
//  locker_end();
//  complock_destroy(lockobj.cmplock);
//}

void object_die(Object *obj) {
  List *lks; // List<link_t>
  list_new(&lks);

  locker_start1(obj);
  assert(!obj->deleted);
  HASHTABLE_FOREACH(entry, obj->links, {
    list_add(lks, entry->value);
  });
  hashtable_remove_all(obj->links);
  assert(obj->count[0] == 0);
  assert(obj->count[1] == 0);
  if (obj->count[2] > 0) {
    HERE_MSG("You have phantoms! Spooky!");
    quick_exit(-1); // fail fast and hard
  }
  locker_end();

  // HERE_MSG("die=%d", list_size(lks)
  LIST_FOREACH(lk, lks, { link_destroy(lk); });
  object_del(obj);
}

// Pseudo: PhantomizeNode
void object_phantomize_node(Object *obj, struct collector_t *cptr) {
  HERE_MSG(object_to_string(obj));
  bool phantomize = false;
  List *phantoms; //List<TableEntry<char*, link_t*>>
  list_new(&phantoms);

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
      if (c->forward == NULL && t->forward == NULL) {
        locker_end();
        assert(false);
      }
      if (c->forward != NULL) c = c->forward;
      if (t->forward != NULL) t = t->forward;
    }
  }

  locker_start1(obj);
  object_set_collector(obj, collector_update(obj->collector));
  assert(t != NULL);
  assert(!obj->deleted);
  obj->count[obj->which]--;
  assert(obj->count[obj->which] >= 0); //: object_to_string(obj);
  if (obj->count[obj->which] > 0) {
    locker_end();
    return;
  } else if (obj->count[bit_flip(obj->which)] > 0) {
    obj->which = bit_flip(obj->which);
    if (!obj->phantomized) {
      phantomize = true;
      obj->phantomized = true;
      assert(obj->collector != NULL);
      obj->phantomization_complete = false;
      HASHTABLE_FOREACH(entry, obj->links, { list_add(phantoms, entry); });
    }
  } else { // phantom count is > 0, but weak and strong are zero.
    if (!obj->phantomized) {
      phantomize = true;
      obj->phantomized = true;
      assert(obj->collector != NULL);
      obj->phantomization_complete = false;
      HASHTABLE_FOREACH(entry, obj->links, { list_add(phantoms, entry); });
    }
  }
  locker_end();

  sprintf(objstr, "phantomize=%s/%s => #<Object:%p id:%d>/%zu",
          (phantomize ? "true" : "false"),
          (obj->phantomized ? "true" : "false"),
          (void*)obj, obj->id, list_size(phantoms));
  HERE_MSG(objstr);

  if (phantomize) {
    sprintf(objstr, "%zu", list_size(phantoms));

    // Between here and the following syncs
    // the actual contents of the links could
    // be changed. New links could be added
    // and existing links could be redirected
    // or cleared.
    LIST_FOREACH(en, phantoms, {
      link_t *lk = ((TableEntry *) en)->value;

      HERE_MSG("ph=");
      HERE_MSG(object_to_string(lk->target));

      locker_start2(obj, lk->target);
      assert(obj->phantomized);
      assert(cptr != NULL);
      if (!lk->phantomized) {
        assert(lk->src->phantomized);
        assert(lk->src->collector != NULL);
        link_phantomize(lk);
        HERE_MSG("phantom: ");
        HERE_MSG(object_to_string(lk->target));
      }

      //sprintf(objstr,"locks remaining=%d", Locker.current_locks.locks.size());
      locker_end();
    })

    locker_start1(obj);
    obj->phantomization_complete = true;
    locker_end();
  }
}

void object_rebuild_link(safelist_t *rebuildNext, link_t *const lk) {
  assert(!lk->src->deleted);
  if (lk->phantomized) {
    if (lk->target == lk->src) {
      lk->which = bit_flip(lk->target->which);
    } else if (lk->target->phantomized) {
      lk->which = lk->target->which;
    } else if (hashtable_size(lk->target->links) == 0) {
      lk->which = lk->target->which;
    } else {
      lk->which = bit_flip(lk->target->which);
    }

    lk->target->count[lk->which]++;
    lk->target->count[2]--;

    if (lk->target->count[2] == 0) {
      object_set_collector(lk->target, NULL);
      lk->target->recovered = false;
    }
    lk->phantomized = false;

    if (lk->target->count[lk->target->which] == 0 &&
        lk->target->count[bit_flip(lk->target->which)] > 0) {
      //assert(lk->target->phantomized); // : "t="+lk.target.collector.update()+" s="+lk.src.collector.update();
      //assert(!lk->target->phantomized); // : "t="+lk.target.collector.update()+" s="+lk.src.collector.update();
    }
    assert(lk->target->count[2] >= 0);
    if (lk->target->collector != NULL) {
      safelist_add(rebuildNext, lk->target);
    }
  }
}

void object_recover_node(Object *obj, safelist_t *rebuildNext, collector_t *cptr) {
  List *rebuild = NULL; //List<Entry<String,Link>>

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
      if (c->forward == NULL && t->forward == NULL) {
        locker_end();
        assert(false);
      }
      if (c->forward != NULL) c = c->forward;
      if (t->forward != NULL) t = t->forward;
    }
  }

  locker_start1(obj);
  assert(!obj->deleted);
  if (obj->collector != NULL) object_set_collector(obj, collector_update(obj->collector));
  //HERE_MSG("rebuild:" + obj);
  if (obj->count[obj->which] > 0) {
    int wcount = 0;
    while (obj->phantomized && !obj->phantomization_complete) {
      locker_end();
      thrd_sleep(&(struct timespec) {.tv_nsec=1000000}, NULL); //sleep for 1ms TODO: maybe change duration?
      locker_start1(obj);
      wcount++;
      assert(wcount < 10000);
    }
    assert(obj->phantomization_complete);
    obj->phantomized = false;
    list_new(&rebuild);
    HASHTABLE_FOREACH(entry, obj->links, { list_add(rebuild, entry); });
    //HERE_MSG("rebuild " + this);
  } else if (obj->count[bit_flip(obj->which)] > 0) {
    assert(false);
  }
  locker_end();

  if (rebuild != NULL) {
    link_t *lk;
    LIST_FOREACH(e, rebuild, {
      hashtable_get(obj->links, ((TableEntry *) e)->key, (void **) &lk);
      locker_start2(lk->src, lk->target);
      if (lk->phantomized) {
        object_rebuild_link(rebuildNext, lk);
      }
      locker_end();
    })
  }

  locker_start1(obj);
  if (obj->count[2] == 0) {
    assert(obj->collector == NULL);
  } else {
    obj->recovered = true;
  }
  locker_end();
}

void object_clean_node(Object *obj, struct collector_t *c) {
  bool die = false;
  locker_start1(obj);
  //HERE_MSG("on clean-up " + object_to_string(obj));
  assert(obj->count[0] >= 0);
  assert(obj->count[1] >= 0);
  if (obj->count[0] == 0 && obj->count[1] == 0) {
    die = true;
  }
  locker_end();

  if (die) object_die(obj);

  locker_start1(obj);
  object_dec_phantom(obj);
  locker_end();
}

/**
 * Pseudo: MergeCollectors
 * @param obj
 * @param target
 * @return true if a merge really happened
 */
bool object_merge_collectors(Object *const obj, Object *target) {
  collector_t *s = obj->collector;
  collector_t *t = target->collector;
  if (t == NULL && s != NULL) {
    object_set_collector(target, s);
  } else if (t != NULL && s == NULL) {
    object_set_collector(obj, t);
  } else if (t == s || t == NULL || s == NULL) {
    return false;
  }

  assert(t != NULL);
  assert(s != NULL);

  while (true) {
    locker_start4(t, s, obj, target);
    collector_check_terminated(t);
    collector_check_terminated(s);
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
  }
}

char *object_to_string(Object *obj) {
  locker_start1(obj);
  int cid = obj->collector == NULL ? -1 : (int)obj->collector->id;
  sprintf(objstr,"#<Object:%p id:%d count:[%d,%d,%d] which:%d collector_id:%d links:%zu deleted:%s marked:%s phantomized:%s>", //|%s",
          (void*)obj, obj->id, obj->count[0], obj->count[1], obj->count[2],
          obj->which, cid, hashtable_size(obj->links),
          (obj->deleted ? "true" : "false"), (obj->mark ? "true" : "false"),
          (obj->phantomized ? "yes" : "no")); //obj->links->keys());
  locker_end();
  return objstr;
}

Object *object_get(Object *obj, char *field) {
  Object *retval = NULL;

  locker_start1(obj);
  assert(!obj->deleted);
  link_t *lk;
  hashtable_get(obj->links, field, (void **) &lk);
  if (lk != NULL) retval = lk->target;
  locker_end();

  return retval;
}

// Pseudo: LinkSet
void object_set(Object *obj, char *field, Object *referent) {
  link_t *old_link = NULL;

  locker_start2(obj, referent);
  hashtable_get(obj->links, field, (void **) &old_link);

  if  (referent == NULL) {
    hashtable_remove(obj->links,field,NULL);
    locker_end();
    if (old_link != NULL) link_dec(old_link, false);
    return;
  }

  if (old_link != NULL && old_link->target == referent) {
    free(old_link);
    old_link = NULL;
    locker_end();
    return;
  }

  HERE_MSG(object_to_string(obj));

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
    lk->which = bit_flip(referent->which);
    lk->target->count[lk->which]++;
  } else if (hashtable_size(referent->links) == 0) {
    lk->which = referent->which;
    lk->target->count[lk->which]++;
  } else {
    lk->which = bit_flip(referent->which);
    lk->target->count[lk->which]++;
  }
  HERE_MSG(object_to_string(obj));
  locker_end();
  if (old_link != NULL) link_dec(old_link, false);
}

void object_status() {
  HashSet *copy;
  hashset_new(&copy);

  locker_start1(objects_lockable);
  HASHSET_FOREACH(obj, objects, { hashset_add(copy, obj); });
  locker_end();

  int live = 0;
  HASHSET_FOREACH(val, copy, {
    Object *obj = val;
    locker_start1(obj);
    assert(obj->count[0] >= 0); // : object_to_string(obj);
    assert(obj->count[1] >= 0); // : object_to_string(obj);
    assert(obj->count[2] == 0); // : object_to_string(obj);
    if (obj->deleted) { ; }
    else {
      assert(!obj->phantomized);
      assert(obj->collector == NULL); // : obj
    }

    if (!obj->deleted) live++;
    // assert Here.here(obj);
    locker_end();
  });
  // assert(Here.here("live=" + live);
  objectlive = live;
}

