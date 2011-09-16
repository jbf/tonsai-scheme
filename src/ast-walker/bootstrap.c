#define _BOOTSTRAP_C 1
#include "bootstrap.h"
#undef _BOOTSTRAP_C

#include <setjmp.h>
#include <stdio.h>

#include "cell.h"
#include "symbol.h"
#include "errors.h"
#include "reader.h"
#include "eval.h"
#include "t_stream.h"
#include "filestream.h"

static cell_t nil_c;
static cell_t false_c;
static cell_t t_c;

static symbol_entry_t nil_s = {(unsigned char *)"NIL", &nil_c};
static symbol_entry_t false_s = {(unsigned char *)"FALSE", &false_c};
static symbol_entry_t t_s = {(unsigned char *)"T", &t_c};

static cell_t nil_c = {{.type = PAYLOAD_NIL}, {.symbol = &nil_s}};
static cell_t false_c = {{.type = PAYLOAD_SYMBOL}, {.symbol = &false_s}};
static cell_t t_c = {{.type = PAYLOAD_SYMBOL}, {.symbol = &t_s}};

extern jmp_buf __jmp_env;
extern cell_t *orig_sexpr;
extern environ_t *internal;

extern symbol_table *global_symtab;

cell_t *nil_cell = &nil_c;
cell_t *false_cell = &false_c;
cell_t *t_cell = &t_c;

int fill_initial_environment(environ_t *env);

int fill_initial_environment(environ_t *env) { 
  if (NULL == env) {
    return ERROR_OTHER_ERROR;
  }

  /* Add lots of stuff */
  add_to_environment(env, nil_cell, nil_cell);
  add_to_environment(env, false_cell, false_cell);
  add_to_environment(env, t_cell, t_cell);

  return ENV_CREATED_OK;
}

/*
 * push is defined in symbol.c why do I do this?
 */
int push(symbol_table *tab, symbol_entry_t *s);

int boot(symbol_table *tab, environ_t **env) {
  init_symbol_table(tab);
  push(tab, &nil_s);
  push(tab, &false_s);
  push(tab, &t_s);

  create_empty_environment(env);
  fill_initial_environment(*env);
  return 1;
}

/*
 * Load shceme ibrary from "lib/lib_boot.scm" into lib or internal environment.
 */
int load_lib_scm(symbol_table *symtab, environ_t *lib, environ_t *internal) {
  cell_t *cell, *res;
  STREAM s;
  FILE *f;
  
  if (((f = fopen("lib/lib_boot.scm", "r")) == NULL) &&
      ((f = fopen("lib/lib_boot_test.scm", "r")) == NULL)) {
    perror(AT);
    DEBUGPRINT_("Can't open \"lib/lib_boot.scm\".\n");
    return 0;
  }

  /* set up a mini-repl here that aborts on any error */
  if (setjmp(__jmp_env)) {
    DEBUGPRINT_("Error evaluating \"lib/lib_boot.scm\". Exiting. \n");
    fclose(f); /* this is bad, but works for now */
    return 0;
  }

  make_filestream(&s, f);
  while ((cell = read_intern(&s, global_symtab))) {
    DEBUGPRINT_("Found form in lib/lib_boot.scm\n");

    /* mini-eval each form with internal as initial env*/
    orig_sexpr = cell;
    res = evaluate(cell, internal);
    if (res) {
#ifdef DEBUG_BOOT
      pretty_print(res);
#endif /* DEBUG_BOOT */
    } else {
      DEBUGPRINT_("Got NULL from eval of: ");
      pretty_print(orig_sexpr);
      DEBUGPRINT_("Error evaluating \"lib/lib_boot.scm\". Exiting. \n");
      fclose(f); /* this is bad, but works for now */
      return 0;
    }
    orig_sexpr = NULL;
  }

  stream_close(&s); /* this closes FILE *f as well */
  return 1;
}
