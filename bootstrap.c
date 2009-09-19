#define _BOOTSTRAP_C 1
#include "bootstrap.h"
#undef _BOOTSTRAP_C

#include "cell.h"
#include "symbol.h"

static cell_t nil_c;
static cell_t false_c;

static symbol_entry_t nil_s = {(unsigned char *)"NIL", &nil_c};
static symbol_entry_t false_s = {(unsigned char *)"FALSE", &false_c};

static cell_t nil_c = {{.type = PAYLOAD_NIL}, {.symbol = &nil_s}};
static cell_t false_c = {{.type = PAYLOAD_SYMBOL}, {.symbol = &false_s}};

cell_t *nil_cell = &nil_c;
cell_t *false_cell = &false_c;

/*
 * Creates an initial environment containing nil. Sets '**env' to NULL in
 * case of errror (typically OOM).
 */

int fill_initial_environment(environ_t *env);

int fill_initial_environment(environ_t *env) { 
  environ_t *temp_env;
  int ok = create_empty_environment(&temp_env);

  if (!ok) return EOOM;

  /* Add lots of stuff */
  add_to_environment(env, nil_cell, nil_cell);
  add_to_environment(env, false_cell, false_cell);

  return ENV_CREATED_OK;
}

int push(symbol_table *tab, symbol_entry_t *s);
int boot(symbol_table *tab, environ_t **env) {
  init_symbol_table(tab);
  push(tab, &nil_s);
  push(tab, &false_s);

  create_empty_environment(env);
  fill_initial_environment(*env);
  return 1;
}
