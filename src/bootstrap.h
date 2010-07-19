#ifndef _BOOTSTRAP_H
#define _BOOTSTRAP_H 1

#include "cell.h"
#include "symbol.h"
#include "environment.h"

int boot(symbol_table *tab, environ_t **env);
int load_lib_scm(symbol_table *symtab, environ_t *lib, environ_t *internal);

#ifndef _BOOTSTRAP_C
extern cell_t *nil_cell;
extern cell_t *false_cell;
extern cell_t *t_cell;
#endif /* _BOOTSTRAP_C */

#endif /* _BOOTSTRAP_H */
