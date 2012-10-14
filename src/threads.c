#include "threads.h"
#include "eval.h"
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "eval.h"
#include "environment.h"
#include "memory.h"
#include "errors.h"
#include "t_stream.h"
#include "filestream.h"
#include "handles.h"

#include <stdio.h>
#include <setjmp.h>

extern symbol_table *global_symtab;
extern environ_t *special_forms;
extern jmp_buf __jmp_env;
extern  int __tl_eval_level;
 
int run_in_thread(const char *code) {
  cell_t *orig_sexpr = NULL;
  cell_t *cell=NULL;
  cell_t *res=NULL;
  handle_t *handle_orig, *mark;

  STREAM s;
  make_filestream(&s, stdin);

  handle_orig = handle_push(orig_sexpr);
  mark = get_mark();
  __tl_eval_level = 0;

  if (setjmp(__jmp_env)) {
    __tl_eval_level = 0;
    res = NULL;
    orig_sexpr = NULL;
    pop_to_mark(mark);
    /* Handle thread cleanup */
  }
  while (1) {
    cell = read_intern(&s, global_symtab);
    
    if(!cell) {
      break;
    }
    
    orig_sexpr = cell;
    handle_set(handle_orig, orig_sexpr);
    
    res = evaluate(cell, NULL);  // orig_sexpr handled
    orig_sexpr = handle_get(handle_orig);

    if (res) {
      pretty_print(res);
   } else {
      printf("Got NULL from eval of: ");
      pretty_print(orig_sexpr);
    }
    orig_sexpr = NULL;
  }

  handle_pop(handle_orig);
  return 0;
}
