#ifndef LINK_H
#define LINK_H

#include <stdbool.h>
#include <stdint.h>
#include "object.h"

typedef struct link_t {
  uint_fast8_t which;
  bool phantomized;
  Object* target;
  Object* const src;
} link_t;

link_t* link_create(Object* src);
void link_destroy(link_t* l);
void link_phantomize(link_t* l);
void link_dec(link_t* l);

#endif  // LINK_H
