#ifndef _CELL_H
#define _CELL_H 1

#include "errors.h"
#include "symbol.h"

#include <stdint.h>

typedef enum {
  PAYLOAD_SYMBOL = 1,
  PAYLOAD_STRING = 3,
  PAYLOAD_NUMBER = 5,
  PAYLOAD_NIL = 7,
  PRIMITIVE = 31,
  FUNCTION = 33,
  FALSE_CELL = 101,
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
    struct cell_t *(*prim)(struct cell_t *, void *);
  } slot2;
} cell_t;

/* Predicates. */
#define PAIRP(c) (((c)->slot1.type & 1) == 0)
#define ATOMP(c) (((c)->slot1.type & 1) == 1)
#define NILP(c) ((c)->slot1.type == PAYLOAD_NIL)
#define SYMBOLP(c) ((c)->slot1.type == PAYLOAD_SYMBOL)
#define NUMBERP(c) ((c)->slot1.type == PAYLOAD_NUMBER)
#define STRINGP(c) ((c)->slot1.type == PAYLOAD_STRING)
#define PRIMITIVEP(c) ((c)->slot1.type == PRIMITIVE)
#define FUNCTIONP(c) ((c)->slot1.type == FUNCTION)
#define FALSEP(c) ((c)->slot1.type == FALSE_CELL)

/* Accessors. */
#define CAR(c) ((c)->slot1.car)
#define CDR(c) ((c)->slot2.cdr)
#define CELL_SYMBOL(c) ((c)->slot2.symbol)
#define I_VAL(c) ((c)->slot2.i_val)
#define STRING_VAL(c) ((c)->slot2.string)
#define CELL_PRIMITIVE(c) ((c)->slot2.prim);

/* Constructor. */
#define CONS(target, src1, src2)   \
  do {                             \
    cell_t *__CONS_TMP = (target);     \
    __CONS_TMP->slot1.car = (src1);      \
    __CONS_TMP->slot2.cdr = (src2);      \
  } while (0)

void print_cell(cell_t *cell);

void pretty_print(cell_t *cell);

#ifndef _CELL_C
extern cell_t the_false_cell;
#endif /* _CELL_C */

#define NIL &nil_cell

#endif /* _CELL_H */
