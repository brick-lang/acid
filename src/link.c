#include <assert.h>
#include <here.h>
#include "link.h"
#include "object.h"
#include "locker.h"
#include "collector.h"

#ifdef DEBUG
static char linkmsg[200];
#endif

link_t *link_create(Object *s) {
  link_t *link = (link_t*)malloc(sizeof(link_t));
  *(Object **)&link->src = s;
  link->target = NULL;
  link->which = BIT_ZERO;
  link->phantomized = false;
  return link;
}

/**
 * Destroys a link_t. This frees the memory allocated to the link,
 * but does not effect the link's src or target.
 *
 * Generally, destroying a link only decrements the reference count.
 * @param link the link to destroy
 */
void link_destroy(link_t *link) {
  sprintf(linkmsg, "before: #<Object:%p id:%d> => #<Object:%p id:%d>",
          (void*)link->src, link->src->id, (void*)link->target, link->target->id);
  HERE_MSG(linkmsg);

  locker_start1(link->src);   // lock on the source object
  bool target_null = (link->target == NULL);
  locker_end();
  if (!target_null) {
    link_dec(link, false);

    sprintf(linkmsg, "after: %s", object_to_string(link->target));
    HERE_MSG(linkmsg);
  }
  free(link);
}

// Pseudo: PhantomizeLink
void link_phantomize(link_t *link) {
  locker_start2((void *) link->target, (void *) link->src);
  if (link->target == NULL || link->phantomized) {
    locker_end();
    // assert Here.here("locks remaining=" + Locker.currentLocks.get().locks.size());
    return;
  }
  link->target->count[2]++;
  link_dec(link, false);
  // assert link->target->collector != NULL || link->src->collector != NULL
  object_merge_collectors(link->src, link->target);
  link->phantomized = true;
  // assert target.collector.equals(src.collector) : "diff1: t="+target.collector+" s="+src.collector;
  locker_end();
  // assert Here.here("locks remaining=" + Locker.currentLocks.get().locks.size());
}

void link_dec_phantomizing(link_t* link) {
  bool merge = false;
  locker_start2((void *) link->target, (void *) link->src);
  if (link->phantomized) {
    object_dec_phantom(link->target);
  } else {
    int ncount = --link->target->count[link->which];
    if (ncount == 0 && link->target->count[bit_flip(link->which)] == 0 && link->target->count[2] == 0) {
      collector_request_delete(link->target);
      locker_end();
      return;
    }
    assert (link->target->count[link->which] >= 0); // : target.toString()
    merge = false;
    if (ncount == 0 && link->which == link->target->which) {
      collector_add_object(link->target->collector, link->target);
    }
  }
  locker_end();
  // assert Here.here("locks remaining=" + Locker.currentLocks.get().locks.size());
  // assert Here.here("until now");
  if (merge) {
    object_merge_collectors(link->src, link->target);
  }
}

void link_dec(link_t* link, bool phantomizing) {
  if (link->target == NULL) return;
  if (phantomizing) {
    link_dec_phantomizing(link);
    return;
  }
  // Pseudo: LinkFree
  if (link->target == NULL) return;

  locker_start2(link->target, link->src);
  if (link->phantomized) {
    object_dec_phantom(link->target);
  } else {
    object_dec(link->target, link->which);
  }
  locker_end();
  // assert Here.here("locks remaining=" + Locker.currentLocks.get().locks.size());
}


