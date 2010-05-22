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
cell_t *prim_mul(cell_t* rest, environ_t *env);
cell_t *prim_number_equals(cell_t *rest, environ_t *env);
cell_t *prim_minus(cell_t* rest, environ_t *env);
cell_t *prim_lambda(cell_t* rest, environ_t *paren_lexical_env);
cell_t *prim_quote(cell_t *rest, environ_t *env);
cell_t *prim_define(cell_t *rest, environ_t *env);
cell_t *prim_error(cell_t *rest, environ_t *env);
cell_t *prim_length(cell_t *rest, environ_t *env);
cell_t *prim_eq(cell_t *rest, environ_t *env);
cell_t *prim_cons(cell_t *rest, environ_t *env);
cell_t *prim_setcar(cell_t *rest, environ_t *env);
cell_t *prim_setcdr(cell_t *rest, environ_t *env);
cell_t *prim_list(cell_t *rest, environ_t *env);

cell_t *prim_definternal(cell_t* rest, environ_t *paren_lexical_env);
cell_t *prim_deflibrary(cell_t* rest, environ_t *paren_lexical_env);

int proper_list_length(cell_t *lst, int target_length);
void fast_error(const char *err_msg);
void undefun_error(cell_t *first, cell_t *exp);
void undef_ident_error(cell_t *sym);

#endif /* _PRIMITIVES_H */
