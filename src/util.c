#include "util.h"
#include "cell.h"

#include <stdio.h>

void print_symtab(symbol_table *tab) {
  symtab_iterator_t _private, *iter;

  if (tab == NULL) return;

  iter = &_private;
  init_symtab_iterator(iter,  tab);

  while (symtab_iter_has_next(iter)) {
    cell_t *sym = symtab_iter_next_sym(iter);
    if(NILP(sym)) {
      printf("%p : NIL \"'()\" ", sym);
    } else {
      printf("%p : ", sym);
      pp(sym);
    }
    printf("\n");
  }
}
