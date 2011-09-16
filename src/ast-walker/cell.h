#ifndef _CELL_H
#define _CELL_H 1

#include "symbol.h"

#include <stdint.h>

/*
 *                  binary position:
 *                  9876543210
 * symbol payloads: 
 *
 *        cons cell xxxxxxxxx0 both car and cdr point to other cells
 *          symbols 0000000011
 *       nil-symbol 0000000111 only symbols have bit 1 (zero-indexed) set
 *           string 0000000101
 *           number 0000001101 
 *        PRIMITIVE 0000010101
 *         FUNCTION 0000011101
 *             u8[] 0000100101 tag word for u8[] alloced on heap
 */

typedef enum {
  PAYLOAD_SYMBOL = 3,
  PAYLOAD_NIL    = 7,
  PAYLOAD_STRING = 5,
  PAYLOAD_NUMBER = 13,
  PRIMITIVE      = 21,
  FUNCTION       = 29,
  U8VEC          = 37,
} cell_type_t;

struct environ_t;
struct function_t;
struct primitive_t;

typedef struct cell_t {
  union {
    cell_type_t type;
    struct cell_t *car;
  } slot1;
  union {
    int32_t i_val;
    long u8_data_size; /* this is size exclusive of the cell_t header */
    struct symbol_entry_t *symbol;
    struct cell_t *string;
    struct cell_t *cdr;
    struct primitive_t *prim;
    struct function_t *fun;
  } slot2;
} cell_t;

/* remember, size in header refers to size of data */ 
typedef struct u8_cell_t {
  cell_t header;
  unsigned char data[];
} u8_cell_t;

/* Predicates. */
#define PAIRP(c) (((c)->slot1.type & 1) == 0)
#define ATOMP(c) (((c)->slot1.type & 1) == 1)
extern cell_t *nil_cell;
#define NILP(c) ((c) == nil_cell)
#define SYMBOLP(c) (((c)->slot1.type & 2) == 2)
#define NUMBERP(c) ((c)->slot1.type == PAYLOAD_NUMBER)
#define STRINGP(c) ((c)->slot1.type == PAYLOAD_STRING)
#define PRIMITIVEP(c) ((c)->slot1.type == PRIMITIVE)
#define FUNCTIONP(c) ((c)->slot1.type == FUNCTION)
#define U8VECP(c) ((c)->-slot1.type == U8VEC)

/* Accessors. */
#define CAR(c) ((c)->slot1.car)
#define CDR(c) ((c)->slot2.cdr)
#define CELL_SYMBOL(c) ((c)->slot2.symbol)
#define I_VAL(c) ((c)->slot2.i_val)

/* u8_vectors */
#define U8LEN(v) ((v)->slot2.u8_data_size)
#define U8DATA(v) (((u8_cell_t *)v)->data)
#define STRING_VAL(c) U8DATA(((c)->slot2.string))

/* Constructor. */
#define CONS(target, src1, src2)    \
  do {                              \
    cell_t *__CONS_TMP = (target);  \
    __CONS_TMP->slot1.car = (src1); \
    __CONS_TMP->slot2.cdr = (src2); \
  } while (0)

void print_cell(cell_t *cell);
void pretty_print(cell_t *cell);
void pp(cell_t *cell);

#endif /* _CELL_H */
