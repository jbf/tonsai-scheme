#ifndef _FUNCTION_H
#define _FUNCTION_H 1

#include "cell.h"
#include "environment.h"

typedef struct function_t {
  cell_t *fun_cell;
  environ_t *lexical_env;
  cell_t *param_list;
  cell_t *code;
} function_t;

void print_fun(cell_t *fun);

#endif /* _FUNCTION_H */
