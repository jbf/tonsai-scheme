#ifndef _EVAL_H
#define _EVAL_H 1

#include "cell.h"
#include "environment.h"

cell_t *evaluate(cell_t *exp, environ_t *env);

#endif /* _EVAL_H */
