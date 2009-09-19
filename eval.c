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
int __tl_eval_level = 0;

int proper_list_length(cell_t *lst);

cell_t pops[] = {
  {{.type = PRIMITIVE}, {.prim = &prim_if}},
  {{.type = PRIMITIVE}, {.prim = &prim_plus}},
};

#ifdef DEBUG
#define DEBUG_PRINT_AND_RETURN(x)                   \
  do {                                              \
    cell_t *__my_ret = (x);                         \
    printf("Eval (%d) returns: ", __tl_eval_level); \
    pretty_print(__my_ret);                         \
    --__tl_eval_level;                              \
    return __my_ret;                                \
  } while (0)
#else
#define DEBUG_PRINT_AND_RETURN(x) do {--__tl_eval_level; return(x);} while (0)
#endif /* DEBUG */

#ifdef DEBUG
#define DFLAG 1
#else
#define DFLAG 0
#endif /* DEBUG */

void init_eval() {
  cell_t *s;
  boot(global_symtab, &special_forms);

  s = intern((unsigned char *)"if", global_symtab);
  add_to_environment(special_forms, s, &pops[0]);

  s = intern((unsigned char *)"+", global_symtab);
  add_to_environment(special_forms, s, &pops[1]);
}

cell_t *evaluate(cell_t *exp, environ_t *env) {
  ++__tl_eval_level;
  if (DFLAG) {
    printf("Eval (%d) got : ", __tl_eval_level);
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
      --__tl_eval_level;
      return NULL; /* error */
    }
  } else { /* list */
    cell_t *first = evaluate(CAR(exp), env);

    if (NULL == first) {
      DEBUG_PRINT_AND_RETURN(NULL);
    } else if (PRIMITIVEP(first)) {
      cell_t *(*f)(cell_t *, environ_t *) = CELL_PRIMITIVE(first);

      DEBUG_PRINT_AND_RETURN((*f)(CDR(exp), env));
    } else if (FUNCTIONP(first)) { /* function call */
      --__tl_eval_level;
      return NULL; /* not implemented yet */
    }
      --__tl_eval_level;
    return NULL;
  }
}

int proper_list_length(cell_t *lst) {
  int length;

  if (NULL == lst)
    return -1; /* error */

  for (length = 0;
       NULL != lst && PAIRP(lst);
       length++, lst = CDR(lst));
  
  return NULL == lst ? -1 : (NILP(lst) ? length : -1);
}

cell_t *evargs(cell_t *args, environ_t *env) {
  cell_t *argsarray[16];
  int length;
  int i;
  cell_t *tmp, *head = nil_cell, *tail;

  if (!(length = proper_list_length(args))) return NULL; /* error */
  if (length > 16) return NULL; /* can only handle 16 args atm */

  for (i = 0; i < length; i++, args = CDR(args)) {
    argsarray[i] = evaluate(CAR(args), env);
  }

  for (i = length - 1; i >= 0; i--) {
    tmp = new(cell_t);
    tail = head;
    head = argsarray[i];
    CONS(tmp, head, tail);
    head = tmp;
  }

  return head;
}
