#ifndef THREADS_H
#define THREADS_H

#include <limits.h>  // Primes __STDC_NO_THREADS__
#ifdef __STDC_NO_THREADS__
#  include "tinycthread/tinycthread.h"
#else
#  include <threads.h>
#endif

#endif  // THREADS_H
