#ifndef _EVAL_H
#define _EVAL_H 1

#include "cell.h"
#include "environment.h"

void init_eval__safe();
cell_t *evaluate(cell_t *exp, environ_t *env);
cell_t *evargs(cell_t *args, environ_t *env);
cell_t *invoke(cell_t *fun, cell_t *args, environ_t *env);
cell_t *find_value(environ_t *env, cell_t *sym);

void destroy_eval__safe();

#define EINVALID_TRUTH -1

#endif /* _EVAL_H */
