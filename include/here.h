#ifndef HERE_H
#define HERE_H

#define DEBUG

#ifdef DEBUG
#define HERE() (printf("here: %s(%s:%d)\n", __func__, strrchr("/" __FILE__, '/') + 1, __LINE__))
#define HERE_MSG(msg) (printf("here: %s(%s:%d) %s\n", __func__, strrchr("/" __FILE__, '/') + 1, __LINE__, msg))
#define HERE_PREFIX_MSG(pre,msg) (printf("here: %s(%s:%d) %s%s\n", __func__, strrchr("/" __FILE__, '/') + 1, __LINE__, pre, msg))
#define debug_snprintf(...) (snprintf(__VA_ARGS__))
#else
#define HERE()
#define HERE_MSG(msg)
#define HERE_PREFIX_MSG(pre,msg)
#define debug_snprintf(...)
#endif // DEBUG

#endif //HERE_H
