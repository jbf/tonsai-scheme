#ifndef __UTIL_H
#define __UTIL_H 1

#include <stdlib.h>
#include <stdio.h>

#define new(type) ((type *)malloc(sizeof(type)/sizeof(char)))

#define min(x,y) ({                             \
      typeof(x) _x = (x);                       \
      typeof(y) _y = (y);                       \
      (void) (&_x == &_y);                      \
      _x < _y ? _x : _y; })

#define max(x,y) ({                             \
      typeof(x) _x = (x);                       \
      typeof(y) _y = (y);                       \
      (void) (&_x == &_y);                      \
      _x > _y ? _x : _y; })

typedef struct value_container_t {
  void  *value;
  struct value_container_t *next;
} value_container_t;

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

#ifndef TRUE
#define TRUE (!FALSE)
#endif /* TRUE */

#ifndef NULL
#define NULL (void *)0
#endif /* NULL */

#define QUOTEME_(x) #x
#define QUOTEME(x) QUOTEME_(x)

#define WHERESTR  "[file %s, line %d]: "
#define WHEREARG  __FILE__, __LINE__
#define DEBUGPRINT2(...)       fprintf(stderr, __VA_ARGS__)
#define DEBUGPRINT(_fmt, ...)  DEBUGPRINT2(WHERESTR _fmt, WHEREARG, __VA_ARGS__)
#define DEBUGPRINT_(_fmt)  DEBUGPRINT2(WHERESTR _fmt, WHEREARG)

#endif /* __UTIL_H */
