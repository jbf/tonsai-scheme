#ifndef _CELL_H
#define _CELL_H 1

#include "errors.h"
#include "symbol.h"

#include <stdint.h>

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
  } slot1;
  union {
    int32_t i_val;
    struct symbol_entry_t *symbol;
    unsigned char *string;
    struct cell_t *cdr;
  } slot2;
} cell_t;

/* Predicates. */
#define PAIRP(c) (((c)->slot1.type & 1) == 0)
#define ATOMP(c) (!PAIRP(C))
#define NILP(c) ((c)->slot1.type == 0)
#define SYMBOLP(c) ((c)->slot1.type == PAYLOAD_SYMBOL)
#define NUMBERP(c) ((c)->slot1.type == PAYLOAD_NUMBER)
#define STRINGP(c) ((c)->slot1.type == PAYLOAD_STRING)

/* Accessors. */
#define CAR(c) ((c)->slot1.car)
#define CDR(c) ((c)->slot2.cdr)
#define CELL_SYMBOL(c) ((c)->slot2.symbol)
#define I_VAL(c) ((c)->slot2.i_val)
#define STRING_VAL(c) ((c)->slot2.string)

/* Constructor. */
#define CONS(target, src1, src2)   \
  do {                             \
    cell_t *__CONS_TMP = (c4);     \
    __CONS_TMP->slot1.car = (src1);      \
    __CONS_TMP->slot2.cdr = (src2);      \
  } while (0)

void print_cell(cell_t *cell);

void pretty_print(cell_t *cell);

#endif /* _CELL_H */
