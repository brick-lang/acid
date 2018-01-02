# Acid [![Build Status](https://travis-ci.org/brick-lang/acid.svg?branch=master)](https://travis-ci.org/brick-lang/acid)
### A multithreaded, concurrent GC with industrial-strength object-dissolving action

Acid is an implementation of the Brandt reference-counting GC algorithm in C, specifically developed for
[Brick](https://github.com/brick-lang), but also intended to be usable in any generic C project.

## Usage

At its heart, Acid provides these functions for use:

```c
// Global functions
void acid_start(int num_threads);
void acid_stop();
void acid_stop_nonblocking();

// Memory functions
void *acid_malloc(size_t alloc_size);
void *acid_malloc_dtor(size_t alloc_size, void (*dtor)(void*));
void acid_harden(void *ptr);
void acid_dissolve(void *ptr);

// Assignment operators
#define acid_set(lhs, rhs);
#define acid_set_field(struct, field, rhs);

// Accessory functions
bool acid_is_managed(void *ptr);
```  

### Global functions
Acid's world is composed of a number of worker threads, a lock coordinator
for each thread, a global tasks queue, and a total running-tasks counter. Constructing the world is as easy
as calling `acid_start(int)` at the beginning of your program.

Deconstructing the world is achieved through `acid_stop()`. It is a blocking
call, waiting on any remaining tasks to complete before `join`ing the worker threads
back to the main thread and returning. There is also an `acid_stop_nonblocking()`
that does *not* complete any remaining collections and destroys the
environment immediately.

### Memory functions
`acid_malloc(size_t)` is intended to be used as a drop-in replacement for libc's own `malloc`.
When called with a size, it returns a pointer to a matching allocated memory space. In reality,
this memory space also has a hidden header that is used for storing reference information.

`acid_malloc_dtor(size_t, void(*)(void*))` is used to allocate memory with an explicit
destructor function tied to its lifetime. Note: the destructor *must not* call free
on the input pointer, but is free to do so on any non-Acid-managed reference in
the structure it is destroying.

##### Warning! Never call `free()` on an Acid-allocated pointer.

...So if you can't call `free()` how is memory released?

Every block of memory allocated by Acid starts off with a strong reference count of 1.
This is to represent the calling context's claim. Other than this single count, all
reference counting is done internally and automatically.

From this point, there are two options: 1) the reference goes out of scope, or 2) the
reference is passed up the stack, still in use.

In the case of 1), the context's claim on the reference must be *dissolved*, which is
achieved by calling `acid_dissolve(void*)` before the context exits. It basically
says "I'm done using this thing myself, you can collect it when nothing else is using
it too." This can be automated inside a block on compilers that support
`__atribute__((cleanup))` (such as Clang and GCC) by using the the
`_cleanup_acid_dissolve_` tag on a variable.

```c
typedef struct { int x; int y; } Point;
typedef struct Line { Point* point; struct Line *next; } Line;
void scenario1() {
  Line* l = acid_malloc(sizeof(Line));
  // do some work...
  acid_dissolve(l);
}

// Returns a managed pointer
Point* scenario2() {
  Point* p = acid_malloc(sizeof(Point));
  // do some work...
  return p;
}

void scenario1_attr() {
  Line* l _cleanup_acid_dissolve_ = acid_malloc(sizeof(Line));
  // do some work
  // acid_dissolve(l) is automatically called
}
```

More precisely, `acid_dissolve(void*)` decrements the reference's strong count.
To manually increment the reference count, `acid_harden(void*)` is provided. This
is useful in certain circumstances, such as recursive algorithms.

### Assignment operators
These functions are what make Acid truly powerful. They operate on an Acid-managed
pointer, replacing the standard assignment operator `=`.

`acid_set_field(struct, field, acid_val)` is used to set a field of an Acid-managed
reference to another Acid-managed reference. Under the hood, it creates a link between
the two, incrementing the receiver's reference count.

For something like `Obj *o1; o1 = o2;`, `acid_set(acid_l, acid_r)` is used. It decrements
the existing reference (if there is one), sets the variable, and increments the new
reference to match.

### Accessory functions
The utility function `acid_is_managed(void*)` is provided for testing if a reference
is managed by Acid or not.

## Example

A simple example looks like

```c
#include <stdio.h>
#include <acid.h>

typedef struct { void* x; void* y;} Point;

Point* simplecycle() {
  Point* a = acid_malloc(sizeof(Point));
  Point* b _cleanup_acid_dissolve_ = acid_malloc(sizeof(Point));

  acid_set_field(a, x, b);
  acid_set_field(b, y, a);
  acid_set_field(a, y, NULL);
  return a;
}

int main(int argc, char** argv) {
  acid_start();

  Point* p = simplecycle();
  printf("{%p| x:{%p| x:%p y:%p} y:%p}\n",
         (void*)p, p->x, ((Point*)p->x)->x, ((Point*)p->x)->y, p->y);
  acid_dissolve(p);

  acid_stop();
  exit(0);
}
```

## About

For the reference implementation, see: [MultiThreadBrownbridge](https://github.com/stevenrbrandt/MultiThreadBrownbridge)

The full paper behind this collector is based on is in the docs folder.

## Known Issues
* There's a race condition in the `object_phantomize_node` over an object's links that
  occurs when an object is simulateously being curated by the collector and deleted
  (as they are two individual tasks able to be executed by two separate threads).
  Current working solution: Use a small number of collector threads (1 or 2).

--------------------------

Copyright (c) 2017 Katherine Whitlock  
Original Java code Copyright (c) 2014, Steven Brandt et al.
