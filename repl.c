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
extern cell_t *orig_sexpr;

int main(int argc, char **argv, char **envp) {
  cell_t *cell;
  cell_t *res;

  init_eval();
  if (setjmp(__jmp_env)) {
    __tl_eval_level = 0;
    orig_sexpr = NULL;
  }
  while ((cell = read_intern(stdin, global_symtab))) {
    orig_sexpr = cell;
    res = evaluate(cell, special_forms);
    if (res) {
      pretty_print(res);
    } else {
      printf("Got NULL from eval of: ");
      pretty_print(orig_sexpr);
    }
    orig_sexpr = NULL;
  }

  return 0;
}
