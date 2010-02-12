#define _BOOTSTRAP_C 1
#include "bootstrap.h"
#undef _BOOTSTRAP_C

#include "cell.h"
#include "symbol.h"
#include "errors.h"

static cell_t nil_c;
static cell_t false_c;
static cell_t t_c;

static symbol_entry_t nil_s = {(unsigned char *)"NIL", &nil_c};
static symbol_entry_t false_s = {(unsigned char *)"FALSE", &false_c};
static symbol_entry_t t_s = {(unsigned char *)"T", &t_c};

static cell_t nil_c = {{.type = PAYLOAD_NIL}, {.symbol = &nil_s}};
static cell_t false_c = {{.type = PAYLOAD_SYMBOL}, {.symbol = &false_s}};
static cell_t t_c = {{.type = PAYLOAD_SYMBOL}, {.symbol = &t_s}};

cell_t *nil_cell = &nil_c;
cell_t *false_cell = &false_c;
cell_t *t_cell = &t_c;

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
