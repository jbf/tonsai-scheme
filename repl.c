#include "repl.h"
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "eval.h"
#include "environment.h"

#include <stdio.h>
#include <setjmp.h>

extern symbol_table *global_symtab;
extern environ_t *special_forms;
extern jmp_buf __jmp_env;
extern int __tl_eval_level;

int main(int argc, char **argv, char **envp) {
  cell_t *cell;
  cell_t *res;

  init_eval();
  if (setjmp(__jmp_env)) {
    __tl_eval_level = 0;
  }
  while ((cell = read_intern(stdin, global_symtab))) {
    res = evaluate(cell, special_forms);
    if (res) pretty_print(res);
  }

  return 0;
}
