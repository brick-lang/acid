#include <assert.h>

#include "collector.h"
#include "link.h"
#include "lockable.h"
#include "locker.h"
#include "memory.h"
#include "object.h"
#include "task.h"

// TODO: This isn't particularly elegant, but...
const uint64_t OBJECT_MAGIC_NUMBER = 10943156698286619218u;

#define BIT_FLIP(b) ((b) ^ 1)

atomic_uint_fast64_t acid_world_count;
atomic_uint_fast64_t acid_collect_count;

size_t hashtable_hash_offset(const void *key, int len, uint32_t seed) {
  (void)len;
  (void)seed;
  return (size_t)key;
}

void object_set_collector(Object *obj, collector_t *c) {
  locker_start1(obj);
  if (obj->collector != c) {
    if (c != NULL) {
      counter_inc_ref(c->count);
    }
    if (obj->collector != NULL) {
      counter_dec_ref(obj->collector->count);
    }
    obj->collector = c;
  }
  locker_end();
}

Object *object_init(Object *obj) {
  HashTableConf htc = {0};
  hashtable_conf_init(&htc);
  htc.hash = hashtable_hash_offset;
  htc.key_compare = cc_common_cmp_ptr;
  hashtable_new_conf(&htc, &obj->links);

  obj->lock = idlock_create();

  obj->which = 0;
  obj->phantomized = false;
  obj->count[0] = 0;
  obj->count[1] = 0;
  obj->count[2] = 0;
  obj->collector = NULL;
  obj->phantomization_complete = true;
  obj->queued = false;

  // Storage-related information
  obj->dtor = NULL;
  obj->data = NULL;
  obj->magic = OBJECT_MAGIC_NUMBER;

  acid_world_count++;
  return obj;
}

Object *object_init_strong(Object *o) {
  object_init(o);
  object_inc_strong(o);
  return o;
}

static void object_del(Object *obj) {
  idlock_t* objlock = obj->lock;
  locker_start1(&obj->lock);
  object_set_collector(obj, NULL);
  assert(hashtable_size(obj->links) == 0);  // make *sure* there are no links
  hashtable_destroy(obj->links);
  obj->magic = 0;
  if (obj->dtor != NULL) {
    obj->dtor(obj->data);
  }
  free(obj);  // free the memory
  locker_end();
  idlock_destroy(objlock);
  acid_collect_count++;
}

static void object_remove_links(Object *obj) {
  link_t **lks = NULL;  // Array<link_t*>
  size_t lks_size = 0;

  locker_start1(obj);
  if (hashtable_size(obj->links) > 0) {
    lks = xmalloc(sizeof(link_t *) * hashtable_size(obj->links),
                  "object_remove_links");
    HASHTABLE_FOREACH(entry, obj->links, { lks[lks_size++] = entry->value; })
  }
  hashtable_remove_all(obj->links);
  assert(obj->count[0] == 0);
  assert(obj->count[1] == 0);
  locker_end();

  if (lks != NULL && lks_size > 0) {
    for (size_t i = 0; i < lks_size; i++) {
      link_destroy(lks[i]);
    }
    free(lks);
  }
}

// Pseudo: Delete
static bool _object_request_delete_task_run(void *obj) {
  object_remove_links(obj);
  // assert(obj->count[2] == 0)
  object_del(obj);
  return false;  // Don't run this operation again.
}

static inline void object_request_delete(Object *const obj) {
  obj->queued = true;
  //_object_request_delete_task_run(obj);
  task_start(task_create(_object_request_delete_task_run, obj));
}

bool object_dec(Object *obj, bit_t w) {
  bool deleted = false;
  locker_start1(obj);
  if (obj->queued) {
    deleted = true;
    goto cleanup;
  }

  if (obj->count[w] > 0) obj->count[w]--;
  assert(obj->count[w] >= 0);  // : object_to_string(obj);
  if (obj->count[w] == 0 && w == obj->which) {
    if (obj->count[BIT_FLIP(obj->which)] == 0 && obj->count[2] == 0) {
      // No strong, weak, or phantom counts. Let's delete.
      object_request_delete(obj);
      deleted = true;
    } else {
      // Set up a collector if one doesn't exist
      if (obj->collector == NULL) {
        object_set_collector(obj, collector_create());
        task_start(task_create(collector_run, obj->collector));
      }
      collector_add_object(obj->collector, obj);
    }
  }
cleanup:
  locker_end();
  return deleted;
}

/**
 * dec_phantom is responsible for removing any
 * reference to the collector.
 *
 * Pseudo: DecPhantom
 */
bool object_dec_phantom(Object *obj) {
  bool deleted = false;
  locker_start1(obj);
  obj->count[2]--;  // decrease the phantom count
  assert(obj->count[2] >= 0);

  if (obj->count[2] == 0) {
    if (obj->count[0] == 0 && obj->count[1] == 0) {
      object_request_delete(obj);
      deleted = true;
    } else {
      object_set_collector(obj, NULL);
    }
  }
  locker_end();
  return deleted;
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
  locker_start1(obj);
  object_set_collector(obj, collector_update(cptr));

  if (obj->count[obj->which] > 0) obj->count[obj->which]--;
  //assert(obj->count[obj->which] >= 0);  //: object_to_string(obj);

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
      phantoms = xmalloc(sizeof(link_t *) * hashtable_size(obj->links),
                         "object_phantomize_node");
      HASHTABLE_FOREACH(entry, obj->links, {
        phantoms[phantoms_size++] = entry->value;
      })
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
      if (hashtable_size(obj->links) == 0) break;
      locker_start2(obj, lk->target);
      if (hashtable_size(obj->links) == 0) {
        locker_end();
        break;
      }
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
  Object *src = lk->src;
  locker_start2(src, lk->target);
  if (hashtable_size(src->links) == 0)
    goto cleanup;

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
cleanup:
  locker_end();
}

void object_recover_node(Object *obj, collector_t *cptr) {
  if (obj->queued) return;
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
      rebuild = xmalloc(sizeof(link_t *) * hashtable_size(obj->links),
                        "object_recover_node");
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
      if (lk->target != NULL)
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
 * by the collector itself). When the last phantom
 * count is gone, the object is cleaned up (in decPhantom)
 *
 * @param obj the Object to clean
 */
void object_clean_node(Object *obj) {
  bool in_use = true;
  locker_start1(obj);
  assert(obj->count[0] >= 0);
  assert(obj->count[1] >= 0);
  in_use = (obj->count[0] != 0 || obj->count[1] != 0);
  locker_end();

  if (!in_use) {
    object_remove_links(obj);
  }
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

Object *object_get(Object *obj, size_t field_offset) {
  Object *retval = NULL;

  locker_start1(obj);
  link_t *lk = NULL;
  hashtable_get(obj->links, (void *)field_offset, (void **)&lk);
  if (lk != NULL) retval = lk->target;
  locker_end();

  return retval;
}

/**
 * Create a new link, and decide the type of link to set.
 *
 * Pseudo: LinkSet
 * @param obj the "source" ojbect
 * @param field_offset the offset of the field holding the reference,
 *                     used as a hash key.
 * @param referent the object to link to
 */
void object_set(Object *obj, size_t field_offset, Object *referent) {
  link_t *old_link = NULL;
  locker_start2(obj, referent);

  // The new reference is NULL, clear the link
  if (referent == NULL) {
    hashtable_remove(obj->links, (void *)field_offset, (void**)&old_link);
    goto cleanup;
  }

  hashtable_get(obj->links, (void *)field_offset, (void **)&old_link);

  // Old link's target is the same as new target.
  // No changes needed, just return.
  if (old_link != NULL && old_link->target == referent) {
    old_link = NULL;
    goto cleanup;
  }

  // Otherwise, let's create a new link...
  link_t *lk = link_create(obj, referent);
  hashtable_add(obj->links, (void *)field_offset, lk);
  lk->target = referent;

  if (obj->phantomized) {
    object_merge_collectors(obj, referent);
    assert(lk->src->collector != NULL);
    lk->target->count[2]++;
    lk->phantomized = true;
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
cleanup:
  locker_end();
  if (old_link != NULL) {
    link_dec(old_link);
  }
}
