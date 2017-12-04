#ifndef PRIORITY_H
#define PRIORITY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Lock priorities */
typedef enum {
  PRIORITY_GLOBAL = 1,
  PRIORITY_OBJECT = 3,
  PRIORITY_COLLECTOR = 2,
  PRIORITY_LIST = 0,
  PRIORITY_COLLECTCOUNTER = -1
} priority_t;

#ifdef __cplusplus
}
#endif

#endif /* PRIORITY_H */
