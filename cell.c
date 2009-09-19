#define _CELL_C 1
#include "cell.h"
#undef _CELL_C

#include <stdio.h>

void pp_list(cell_t *cell);
void pp(cell_t *cell);

void print_cell(cell_t *cell) {
  if (NULL == cell) {
    printf ("C-NULL");
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
    printf("PRIMITIVE FUNCTION CELL ");
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
  while (!NILP(cell)) {
    if (PAIRP(cell)) {
      pp(CAR(cell));
      printf(" ");
      cell = CDR(cell);
    } else {
      printf("ERROR in list\n");
    }
  }
}
