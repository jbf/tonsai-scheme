#ifndef __UTIL_H
#define __UTIL_H 1

#include <stdlib.h>

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

#endif /* __UTIL_H */
