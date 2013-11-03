#include <assert.h>

#include "environment.h"
#include "symbol.h"
#include "errors.h"
#include "memory.h"
#include "util.h"

int alloc_sym_value_pair(value_container_t **sym, value_container_t **val);
cell_t *_value(environ_t *env, symbol_entry_t *sym);

void free_environ(environ_t *env);
void free_values(value_container_t *t);
void free_symbol_container(value_container_t *t);

cell_t *value(environ_t *env, cell_t *sym) {
  return _value(env, CELL_SYMBOL(sym));
}

int create_empty_environment(environ_t **env) {
  *env = new_malloc(environ_t);

  if (NULL == *env) return EOOM;

  (*env)->parent = NULL;
  (*env)->symbols = NULL;
  (*env)->values = NULL;
  return ENV_CREATED_OK;
}

void destroy_env(environ_t **env) {
  free_environ(*env);
  *env = NULL;
}

void free_environ(environ_t *env) {
  environ_t *n;
  while(env != NULL)  {
    n = env->parent;
    free_values(env->values);
    free_symbol_container(env->symbols);
    free_malloced(env);
    env = n;
  }
}

void free_values(value_container_t *t) {
  value_container_t *n;
  while(t != NULL) {
    n = t->next;
    // t's payload is a cell, will get gc:d
    free_malloced(t);
    t = n;
  }
}

void free_symbol_container(value_container_t *t) {
  value_container_t *n;
  while(t != NULL) {
    n = t->next;
    // t's payload is a symbol, will be freed through symtab free
    free_malloced(t);
    t = n;
  }
}

/*
 * Short alloc of containers. Does not clean up on failure. Caller should
 * do that.
 */
int alloc_sym_value_pair(value_container_t **sym, value_container_t **val) {
  *sym = new_malloc(value_container_t);
  *val = new_malloc(value_container_t);
  int oom = FALSE;

  if (NULL == *sym) {
    oom = TRUE;
  } else if (NULL == val) {
    oom = TRUE;
  }

  return !oom; /* We want TRUE to be ok. */
}

int add_to_environment(environ_t *env, cell_t *symbol, cell_t *value) {
  value_container_t *sym;
  value_container_t *val;
  int ok;

  ok = alloc_sym_value_pair(&sym, &val);

  if (!ok) {
    free_malloced(sym);
    free_malloced(val);
    return EOOM;
  }

  sym->next = env->symbols;
  sym->value = CELL_SYMBOL(symbol);
  env->symbols = sym;

  val->next = env->values;
  val->value = value;
  env->values = val;

  return ENV_ADDED_TO_OK;
}

cell_t *_value(environ_t *env, symbol_entry_t *sym) {
  value_container_t *sym_list;
  value_container_t *val_list;

  if (NULL == env) return NULL; /* not found */

  if (NULL == sym) {
    DEBUGPRINT("sym=%p\n", sym);
    vm_exit();
    return NULL; /* unreachable */
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
  return _value(env->parent, sym);
}

int extend(environ_t *parent,
           environ_t *new,
           cell_t *symbols,
           cell_t *values) {
  new->parent = parent;
  for (;
       NULL != symbols &&
         !NILP(symbols) &&
         NULL != values &&
         !NILP(values);
       symbols = CDR(symbols),
         values = CDR(values)) {
    add_to_environment(new, CAR(symbols), CAR(values));
  }

  return 1;
}

void iterate(environ_t *env, void (*fun)(struct symbol_entry_t *, cell_t *)) {
  while (env != NULL) {
    value_container_t *sym_list;
    value_container_t *val_list;

    for (sym_list = env->symbols, val_list = env->values;
        sym_list != NULL && val_list != NULL;
        sym_list = sym_list->next, val_list = val_list->next) {
      (*fun)((struct symbol_entry_t *)sym_list->value, (cell_t *)val_list->value);
    }
    env = env->parent;
  }
}

void print_sym_val(struct symbol_entry_t *sym, cell_t *val);
void iterate_print(environ_t *env) {
  iterate(env, &print_sym_val);
}

void print_sym_val(struct symbol_entry_t *sym, cell_t *val) {
  printf("ENV ENTRY: sym: %p (%s) value: %p : ", sym, sym->symbol_name, val);
  pretty_print(val);
}
