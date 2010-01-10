#ifndef _PRIMITIVES_H
#define _PRIMITIVES_H 1

#include "cell.h"
#include "environment.h"
#include "repl.h"

typedef struct primitive_t {
  unsigned char *name;
  cell_t *(*fun)(struct cell_t *, struct environ_t *);
} primitive_t;

#define CELL_PRIMITIVE(c) ((c)->slot2.prim->fun)
#define CELL_PRIMITIVE_NAME(c) ((c)->slot2.prim->name)

int scheme_to_c_truth(cell_t *c, environ_t *env);

cell_t *prim_if(cell_t* rest, environ_t *env);
cell_t *prim_plus(cell_t* rest, environ_t *env);
cell_t *prim_lambda(cell_t* rest, environ_t *env);
cell_t *prim_quote(cell_t *rest, environ_t *env);
cell_t *prim_define(cell_t *rest, environ_t *env);
cell_t *prim_error(cell_t *rest, environ_t *env);

int proper_list_length(cell_t *lst);

#endif /* _PRIMITIVES_H */
