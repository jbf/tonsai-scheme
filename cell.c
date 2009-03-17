#include "cell.h"

#include <stdio.h>

void pp_list(cell_t *cell);
void pp(cell_t *cell);

void print_cell(cell_t *cell) {
  if (NILP(cell)) {
    printf("NIL");
  } else if (PAIRP(cell)) {
    printf("PAIR");
  } else if (SYMBOLP(cell)) {
#ifdef __DEBUG
    printf("SYMBOL ");
#endif /* __DEBUG */
    printf("%s", cell->symbol->symbol_name);
  } else if (NUMBERP(cell)) {
#ifdef __DEBUG
    printf("NUMBER ");
#endif /* __DEBUG */
    printf("%d", cell->i_val);
  } else if (STRINGP(cell)) {
#ifdef __DEBUG
    printf("STRING ");
#endif /* __DEBUG */
    printf("\"%s\"", cell->string);
  } else {
    printf("UNKNOWN CELL %p, %p", cell->car, cell->cdr);
  }
}

void pretty_print(cell_t *cell) {
  pp(cell);
  printf("\n");
}

void pp(cell_t *cell) {
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
