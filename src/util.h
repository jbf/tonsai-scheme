#ifndef __UTIL_H
#define __UTIL_H 1

#include <stdio.h>

#include "symbol.h"

typedef struct value_container_t {
  void *value;
  struct value_container_t *next;
} value_container_t;

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

#define AT "["__FILE__ ": " QUOTEME(__LINE__) "]"

#define WHERESTR  "[file %s, line %d]: "
#define WHEREARG  __FILE__, __LINE__
#define DEBUGPRINT2(...) fprintf(stderr, __VA_ARGS__)
#define DEBUGPRINT(_fmt, ...)  DEBUGPRINT2(WHERESTR _fmt, WHEREARG, __VA_ARGS__)
#define DEBUGPRINT_(_fmt)  DEBUGPRINT2(WHERESTR _fmt, WHEREARG)

#define bail(x) exit(x)

#define CHECK(x,y) ({                                   \
      typeof(x) _x = (x);                               \
      typeof(y) _y = (y);                               \
      (_x != _y) ? ({ bail(1); NULL; }) : _x; })

#define CHECK_NOT(x,y) ({                               \
      typeof(x) _x = (x);                               \
      typeof(y) _y = (y);                               \
      (_x == _y) ? ({ bail(1); NULL; }) : _x; })

#define CHECK_NULL(x) CHECK_NOT(x, NULL)

void print_symtab(symbol_table *tab);
void print_handles();
void print_environment(struct environ_t *env);
void print_roots(struct environ_t *env);

#endif /* __UTIL_H */
