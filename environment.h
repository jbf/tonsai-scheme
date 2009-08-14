#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H 1

#include "symbol.h"

typedef struct environ_t {
  struct environ_t *parent;
  symtab_entry_t *symtab;
  
  struct value_container_t {
    cell_t *value;
    struct value_container_t *next;
  } *values;
} environ_t;

int create_initial_environment(environ_t **env);
int extend_environment(environ_t **env, symtab_entry_t *symtab, cell_t *list_of_values);
cell_t *value(environ_t *env, symbol_entry_t *sym);

#endif /* _ENVIRONMENT_H */
