#include <assert.h>

#include "environment.h"
#include "errors.h"
#include "symbol.h"

int alloc_sym_value_pair(value_container_t **sym, value_container_t **val);

extern symbol_entry_t nil;
extern cell_t nil_cell;

int create_initial_environment(environ_t **env) { 
  environ_t *temp_env = new(environ_t);
  value_container_t *sym;
  value_container_t *val;
  int ok;

  ok = alloc_sym_value_pair(&sym, &val);

  if (NULL == temp_env || !ok) {
    *env = NULL;
    free(temp_env);
    free(sym);
    free(val);
    return EOOM;
  }

  sym->value = &nil;
  sym->next = NULL;

  val->value = &nil_cell;
  val->next = NULL;

  temp_env->parent = NULL;
  temp_env->values = val;
  temp_env->symbols = sym;

  *env = temp_env;
  return ENV_CREATED_OK;
}

int alloc_sym_value_pair(value_container_t **sym, value_container_t **val) {
  *sym = new(value_container_t);
  *val = new(value_container_t);
  int oom = FALSE;

  if (NULL == *sym) {
    oom = TRUE;
  } else if (NULL == val) {
    oom = TRUE;
  }
  
  return !oom; /* We want TRUE to be ok. */
}

int add_to_environment(environ_t *env, symbol_entry_t *symbol, cell_t *value) {
  value_container_t *sym;
  value_container_t *val;
  int ok;
  
  ok = alloc_sym_value_pair(&sym, &val);

  if (!ok) {
    free(sym);
    free(val);
    return EOOM;
  }

  sym->next = env->symbols;
  sym->value = symbol;
  env->symbols = sym;

  val->next = env->values;
  val->value = value;
  env->values = val;

  return ENV_ADDED_TO_OK;
}

cell_t *value(environ_t *env, symbol_entry_t *sym) {
  value_container_t *sym_list;
  value_container_t *val_list;

  if (NULL == env || NULL == sym) {
    return NULL;
  }
  
  for (sym_list = env->symbols, val_list = env->values;
       sym_list != NULL && val_list != NULL;
       sym_list = sym_list->next, val_list = val_list->next) {
    /* assert equal length */
    assert(sym_list);
    assert(val_list);
    
    if ((symbol_entry_t *)sym_list->value == sym) {
      return (cell_t *)val_list->value;
    }
  }
  return NULL;
}
