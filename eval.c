#define _EVAL_C 1
#include "eval.h"
#undef _EVAL_C
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "environment.h"
#include "primitives.h"
#include "bootstrap.h"

environ_t *special_forms;
static symbol_table __gs;
symbol_table *global_symtab = &__gs;

cell_t pops[] = {
  {{.type = PRIMITIVE}, {.prim = &prim_if}},
};

#ifdef DEBUG
#define DEBUG_PRINT_AND_RETURN(x) \
  do {                            \
    cell_t *__my_ret = (x);       \
    printf("Eval returns: ");     \
    pretty_print(__my_ret);       \
    return __my_ret;              \
  } while (0)
#else
#define DEBUG_PRINT_AND_RETURN(x) return(x)
#endif /* DEBUG */

void init_eval() {
  cell_t *s;
  boot(global_symtab, &special_forms);
  s = intern((unsigned char *)"if", global_symtab);
  add_to_environment(special_forms, s, &pops[0]);
}

cell_t *evaluate(cell_t *exp, environ_t *env) {
  if (DEBUG) {
    printf("Eval got: ");
    pretty_print(exp);
  }

  if (NULL == exp) {
    DEBUG_PRINT_AND_RETURN(NULL);
  } else if (NILP(exp)) {
    DEBUG_PRINT_AND_RETURN(nil_cell);
  } else if (ATOMP(exp)) {
    if (SYMBOLP(exp)) {
      DEBUG_PRINT_AND_RETURN(value(env, exp));
    } else if (STRINGP(exp) || NUMBERP(exp)) {
      DEBUG_PRINT_AND_RETURN(exp);
    } else {
      return NULL; /* error */
    }
  } else { /* list */
    cell_t *first = evaluate(CAR(exp), env);

    if (PRIMITIVEP(first)) {
      cell_t *(*f)(cell_t *, void *) = CELL_PRIMITIVE(first);

      DEBUG_PRINT_AND_RETURN((*f)(CDR(exp), env));
    } else if (FUNCTIONP(first)) { /* function call */
      return NULL; /* not implemented yet */
    }
    return NULL;
  }
}
