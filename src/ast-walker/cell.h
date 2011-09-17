#ifndef _CELL_H
#define _CELL_H 1

#include "symbol.h"

#include <stdint.h>

struct environ_t;
struct function_t;
struct primitive_t;

/* cell type enum */
#define DECLARE_OBJECT_TYPE(x, y) x = y,
typedef enum {
#include "heap_objects_declare.h"
} cell_type_t;
#undef DECLARE_OBJECT_TYPE

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

/* declare content predicates */
#define SYMBOLP(c) ((c)->slot1.type == SYMBOL)
#define NUMBERP(c) ((c)->slot1.type == NUMBER)
#define STRINGP(c) ((c)->slot1.type == STRING)
#define PRIMITIVEP(c) ((c)->slot1.type == PRIMITIVE)
#define FUNCTIONP(c) ((c)->slot1.type == FUNCTION)
#define U8VECP(c) ((c)->slot1.type == U8VEC)

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

const char *object_type_str(cell_t *cell);
#endif /* _CELL_H */
