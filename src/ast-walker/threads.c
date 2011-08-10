#include "threads.h"
#include "eval.h"
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "eval.h"
#include "environment.h"
#include "memory.h"
#include "liveness.h"
#include "errors.h"

#include <stdio.h>
#include <setjmp.h>

extern symbol_table *global_symtab;
extern environ_t *special_forms;
extern jmp_buf __jmp_env;
 
int run_in_thread(const char *code) {
  int __tl_eval_level = 0;
  cell_t *orig_sexpr = NULL;
  frame_t *live_root = NULL;
  cell_t *cell=NULL;
  cell_t *res=NULL;

  if (setjmp(__jmp_env)) {
    __tl_eval_level = 0;
    orig_sexpr = NULL;
    res = NULL;
    orig_sexpr = NULL;
    dealloc_frames(&live_root);
    
    /* Handle thread cleanup */
    return 0;
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
