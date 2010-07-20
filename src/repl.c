#include "repl.h"
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "eval.h"
#include "environment.h"
#include "memory.h"
#include "liveness.h"

#include <stdio.h>
#include <setjmp.h>

extern symbol_table *global_symtab;
extern environ_t *special_forms;
extern jmp_buf __jmp_env;
extern int __tl_eval_level;
extern cell_t *orig_sexpr;
extern frame_t *live_root;

int main(int argc, char **argv, char **envp) {
  cell_t *cell=NULL;
  cell_t *res=NULL;

  init_mem_sys();
  init_eval();
  if (setjmp(__jmp_env)) {
    __tl_eval_level = 0;
    orig_sexpr = NULL;
    res = NULL;
    orig_sexpr = NULL;
    dealloc_frames(&live_root);
  }
  while (1) {
    cell = read_intern(stdin, global_symtab);

    if(!cell) {
      break;
    }

    orig_sexpr = cell;

    push_liveness(&live_root, new_liveframe(2, cell, orig_sexpr));
    res = evaluate(cell, NULL);
    pop_liveness(&live_root);

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
