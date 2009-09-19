#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"

#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  cell_t *cell;
  symbol_table s, *symtab = &s;

  init_symbol_table(symtab);

  while ((cell = read_intern(stdin, symtab))) {
    pretty_print(cell);
  }

  return 0;
}
