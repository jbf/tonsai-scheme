#ifndef _EVAL_H
#define _EVAL_H 1

#include "cell.h"
#include "environment.h"

cell_t *evaluate(cell_t *exp, environ_t *env);
cell_t *evargs(cell_t *args, environ_t *env);
cell_t *invoke(cell_t *fun, cell_t *args, environ_t *env);

#define EINVALID_TRUTH -1

#endif /* _EVAL_H */
