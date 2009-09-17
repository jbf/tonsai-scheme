#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "eval.h"
#include "environment.h"

#include <stdio.h>

void init_eval();
extern symtab_entry_t *global_symtab;
extern environ_t *special_forms;

int main(int argc, char **argv, char **envp) {
  cell_t *cell;
  cell_t *res;

  init_eval();
  while ((cell = read_intern(stdin, &global_symtab))) {
    pretty_print(cell);

     res = evaluate(cell, special_forms);

     if (res) pretty_print(res); else printf("Error\n");
  }

  return 0;
}
