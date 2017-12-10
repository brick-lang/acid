#ifndef PRIORITY_H
#define PRIORITY_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Lock priorities */
typedef enum {
  PRIORITY_LIST = 0,
  PRIORITY_GLOBAL = 1,
  PRIORITY_COLLECTOR = 2,
  PRIORITY_OBJECT = 3,
} priority_t;

#ifdef __cplusplus
}
#endif

#endif /* PRIORITY_H */
