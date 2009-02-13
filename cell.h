#ifndef _CELL_H
#define _CELL_H 1

#include "errors.h"

#include <stdint.h>
#include <stdlib.h>

typedef enum {
  PAYLOAD_NIL = 0,
  PAYLOAD_SYMBOL = 1,
  PAYLOAD_STRING = 3,
  PAYLOAD_NUMBER = 5
} cell_type_t;

typedef struct cell_t {
  union {
    cell_type_t type;
    struct cell_t *car;
  };
  union {
    int32_t i_val;
    void *symbol_val;
    unsigned char *string_val;
    struct cell_t *cdr;
  };
} cell_t;

#define PAIRP(c) (((c)->type & 1) == 0)
#define NILP(c) ((c)->type == 0)
#define CAR(c) ((c)->car)
#define CDR(c) ((c)->cdr)
#define SYMBOLP(c) ((c)->type == PAYLOAD_SYMBOL)
#define NUMBERP(c) ((c)->type == PAYLOAD_NUMBER)
#define STRINGP(c) ((c)->type == PAYLOAD_STRING)
#define CONS(target, src1, src2)   \
  do {                             \
    cell_t *__CONS_TMP = (c4);     \
    __CONS_TMP->car = (src1);      \
    __CONS_TMP->cdr = (src2);      \
  } while (0)

int print_cell(cell_t *cell);

#define alloc_cell() (((cell_t *)malloc(sizeof(cell_t)/sizeof(char))))

#endif /* _CELL_H */
