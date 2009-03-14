#include "cell.h"

#include <stdio.h>

int print_cell(cell_t *cell) {
  if (NILP(cell)) {
    printf("NIL\n");
  } else if (PAIRP(cell)) {
    printf("PAIR\n");
  } else if (SYMBOLP(cell)) {
    printf("SYMBOL %s\n", cell->symbol->symbol_name);
  } else if (NUMBERP(cell)) {
    printf("NUMBER %d\n", cell->i_val);
  } else if (STRINGP(cell)) {
    printf("STRING %s\n", cell->string);
  } else {
    printf("UNKNOWN CELL %p, %p\n", cell->car, cell->cdr);
  }

  return 0;
}

