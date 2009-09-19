#ifndef _CELL_H
#define _CELL_H 1

#include "errors.h"
#include "symbol.h"

#include <stdint.h>

/*
 *                  binary position
 * symbol payloads: 9876543210
 *
 *        cons cell xxxxxxxxx0 both car and cdr point to other cells.
 *          symbols 0000000011
 *       nil-symbol 0000000111
 *           string 0000000101
 *           number 0000001101 
 *        PRIMITIVE 0000010101
 *         FUNCTION 0000011101 
 */

typedef enum {
  PAYLOAD_SYMBOL = 0b00011, /*  3 */
  PAYLOAD_NIL    = 0b00111, /*  7 */
  PAYLOAD_STRING = 0b00101, /*  5 */
  PAYLOAD_NUMBER = 0b01101, /* 13 */
  PRIMITIVE      = 0b10101, /* 21 */
  FUNCTION       = 0b11101, /* 29 */
} cell_type_t;

struct environ_t;

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
    struct cell_t *(*prim)(struct cell_t *, struct environ_t *);
  } slot2;
} cell_t;

/* Predicates. */
#define PAIRP(c) (((c)->slot1.type & 1) == 0)
#define ATOMP(c) (((c)->slot1.type & 1) == 1)
#define NILP(c) ((c)->slot1.type == PAYLOAD_NIL)
#define SYMBOLP(c) (((c)->slot1.type & 2) == 2)
#define NUMBERP(c) ((c)->slot1.type == PAYLOAD_NUMBER)
#define STRINGP(c) ((c)->slot1.type == PAYLOAD_STRING)
#define PRIMITIVEP(c) ((c)->slot1.type == PRIMITIVE)
#define FUNCTIONP(c) ((c)->slot1.type == FUNCTION)

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
