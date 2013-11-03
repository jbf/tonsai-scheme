#include "util.h"
#include "cell.h"
#include "handles.h"
#include "environment.h"

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
      printf("%p : cell type: %u ", sym, (sym->slot1).type);
      pp(sym);
    }
    printf("\n");
  }
}

void print_handles() {
  handle_iterator_t handle_iter, *hi;

  hi = &handle_iter;
  init_handle_iterator(hi);
  while(handle_iter_has_next(hi)) {
    cell_t *c;
    handle_t *h;
    h = handle_iter_next_handle(hi);
    c = handle_get(h);
    printf("HANDLE: %p : CELL : %p :", h, c);
    pretty_print(c);
  }
}

void print_environment(struct environ_t *env) {
    iterate_print(env);
}


extern symbol_table *global_symtab;
void print_roots(struct environ_t *env) {
  print_symtab(global_symtab);
  print_handles();
}
