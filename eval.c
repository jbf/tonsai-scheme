#define _EVAL_C 1
#include "eval.h"
#undef _EVAL_C
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "environment.h"
#include "primitives.h"

environ_t *special_forms;
symtab_entry_t *global_symtab;
cell_t pops[] = {
  {{.type = PRIMITIVE}, {.prim = &prim_if}},
};

void init_eval() {
  cell_t *s;
  create_initial_symtab(&global_symtab);
  s = intern((unsigned char *)"if", &global_symtab);
  create_empty_environment(&special_forms);

  add_to_environment(special_forms, s->slot2.symbol, &pops[0]);
}

cell_t *evaluate(cell_t *exp, environ_t *env) {
  if (NULL == exp) {
    return NULL;
  } else if (NILP(exp)) {
      return NIL;
  } else if (ATOMP(exp)) {
    if (SYMBOLP(exp)) {
      return value(env, exp);
    } else if (STRINGP(exp) || NUMBERP(exp)) {
      return exp;
    } else {
      return NULL; /* error */
    }
  } else { /* list */
    cell_t *first = evaluate(CAR(exp), env);

    if (PRIMITIVEP(first)) {
      cell_t *(*f)(cell_t *, void *) = CELL_PRIMITIVE(first);

      return (*f)(CDR(exp), env);
    } else if (FUNCTIONP(first)) { /* function call */
      return NULL; /* not implemented yet */
    }
    return NULL;
  }
}
