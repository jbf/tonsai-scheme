#include "cell.h"
#include "function.h"
#include "primitives.h"

#include <stdio.h>

void pp_list(cell_t *cell);
void pp(cell_t *cell);

void print_cell(cell_t *cell) {
  if (NULL == cell) {
    printf ("C-NULL");
  } else if (NILP(cell)) {
    printf("NIL");
  } else if (PAIRP(cell)) {
    printf("PAIR");
  } else if (SYMBOLP(cell)) {
#ifdef DEBUG
    printf("SYMBOL ");
#endif /* DEBUG */
    printf("%s", SYMBOL_NAME(CELL_SYMBOL(cell)));
  } else if (NUMBERP(cell)) {
#ifdef DEBUG
    printf("NUMBER ");
#endif /* DEBUG */
    printf("%d", I_VAL(cell));
  } else if (STRINGP(cell)) {
#ifdef DEBUG
    printf("STRING ");
#endif /* DEBUG */
    printf("\"%s\"", STRING_VAL(cell));
  } else if (PRIMITIVEP(cell)) {
    printf("PRIMITIVE FUNCTION CELL %s ", CELL_PRIMITIVE_NAME(cell));
  } else if (FUNCTIONP(cell)) {
    print_fun(cell);
  } else {
    printf("UNKNOWN CELL %p, %p", CAR(cell), CDR(cell));
  }
}

void pretty_print(cell_t *cell) {
  pp(cell);
  printf("\n");
}

void pp(cell_t *cell) {
  if (NULL == cell) {
    printf("C-NULL");
    return;
  }

  if (PAIRP(cell)) {
    printf("(");
    pp_list(cell);
    printf(")");
  } else {
    print_cell(cell);
  }
}

void pp_list(cell_t *cell) {
  if (NULL != cell && PAIRP(cell)) {
      pp(CAR(cell));
      cell = CDR(cell);
  }

  while (!NILP(cell)) {
    if (PAIRP(cell)) {
      printf(" ");
      pp(CAR(cell));
      cell = CDR(cell);
    } else {
      printf("ERROR in list\n");
    }
  }
}
