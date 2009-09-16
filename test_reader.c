#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"

#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  cell_t *cell;
  symtab_entry_t *symtab;

  symtab = NULL;

  while ((cell = read_intern(stdin, &symtab))) {
    pretty_print(cell);
  }

  return 0;
}