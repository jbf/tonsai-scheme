#ifndef _PRIMITIVES_H
#define _PRIMITIVES_H 1

#include "cell.h"
#include "environment.h"

int scheme_to_c_truth(cell_t *c, environ_t *env);

cell_t *prim_if(cell_t* rest, void *e);

#endif /* _PRIMITIVES_H */
