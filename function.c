#include "function.h"

#include <stdio.h>

void print_fun(cell_t *fun) {
  printf("(lambda ");
  pp(fun->slot2.fun->param_list);
  printf(" ");
  pp(fun->slot2.fun->code);
  printf(")");
}
