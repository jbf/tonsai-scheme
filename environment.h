#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H 1

#include "symbol.h"
#include "cell.h"
#include "util.h"

typedef struct environ_t {
  struct environ_t *parent;
  value_container_t *symbols;
  value_container_t *values;
} environ_t;

#define ENV_CREATED_OK 1
#define ENV_ADDED_TO_OK 2

/*
 * Creates an initial environment containing nil. Sets '**env' to NULL in
 * case of errror (typically OOM).
 */
int create_initial_environment(environ_t **env);

/*
 * Creates a new empty environment.
 */
int create_empty_environment(environ_t **env);

/*
 * Adds a new binding to 'env'.
 *
 * Binding should be a symbol_entry_t 'symbol' and a cell_t 'value'.
 */
int add_to_environment(environ_t *env, symbol_entry_t *symbol, cell_t *value);

/*
 * Looks up the value of 'sym' in 'env' (or any parent environment of
 * 'env'). Returns NULL if if 'sym' is unbound in 'env'.
 */
cell_t *value(environ_t *env, cell_t *sym);

#endif /* _ENVIRONMENT_H */
