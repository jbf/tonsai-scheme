#include "eval.h"
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "environment.h"
#include "primitives.h"
#include "bootstrap.h"
#include "function.h"

environ_t *special_forms;
environ_t *toplevel;
static symbol_table __gs;
symbol_table *global_symtab = &__gs;
int __tl_eval_level = 0;
cell_t *find_value(environ_t *env, cell_t *sym);

#ifdef DEBUG
#define DEBUG_PRINT_AND_RETURN(x)                   \
  do {                                              \
    cell_t *__my_ret = (x);                         \
    printf("Eval (%d) returns: ", __tl_eval_level); \
    pretty_print(__my_ret);                         \
    --__tl_eval_level;                              \
    return __my_ret;                                \
  } while (0)
#define DEBUG_PRIM_PRINT_AND_RETURN(x)                   \
  do {                                                   \
    cell_t *__my_ret = (x);                              \
    printf("Primitive (%d) returns: ", __tl_eval_level); \
    pretty_print(__my_ret);                              \
    --__tl_eval_level;                                   \
    return __my_ret;                                     \
  } while (0)
#define DEBUG_INVOKE_PRINT_AND_RETURN(x)              \
  do {                                                \
    cell_t *__my_ret = (x);                           \
    printf("Invoke (%d) returns: ", __tl_eval_level); \
    pretty_print(__my_ret);                           \
    --__tl_eval_level;                                \
    return __my_ret;                                  \
  } while (0)
#else
#define DEBUG_PRINT_AND_RETURN(x) do {--__tl_eval_level; return(x);} while (0)
#define DEBUG_PRIM_PRINT_AND_RETURN(x) do {--__tl_eval_level; return(x);} while (0)  
#define DEBUG_INVOKE_PRINT_AND_RETURN(x) do {--__tl_eval_level; return(x);} while (0)
#endif /* DEBUG */

#ifdef DEBUG
#define DFLAG 1
#else
#define DFLAG 0
#endif /* DEBUG */

void init_eval() {
  boot(global_symtab, &special_forms);
  create_empty_environment(&toplevel);

#define DECLARE_PRIMITIVE(name, prim_op) do {                           \
    cell_t *s, *v = new(cell_t);                                        \
    v->slot1.type = PRIMITIVE;                                          \
    v->slot2.prim = &prim_op;                                           \
    s = intern((unsigned char *)name, global_symtab);                   \
    add_to_environment(special_forms, s, v);                            \
  } while (0)

  DECLARE_PRIMITIVE("if", prim_if);
  DECLARE_PRIMITIVE("+", prim_plus);
  DECLARE_PRIMITIVE("lambda", prim_lambda);
  DECLARE_PRIMITIVE("quote", prim_quote);
  DECLARE_PRIMITIVE("define", prim_define);
  DECLARE_PRIMITIVE("error", prim_error);
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
      DEBUG_PRINT_AND_RETURN(find_value(env, exp));
    } else if (STRINGP(exp) || NUMBERP(exp)) {
      DEBUG_PRINT_AND_RETURN(exp);
    } else {
      --__tl_eval_level;
      return NULL; /* error */
    }
  } else { /* list */
    cell_t *first = evaluate(CAR(exp), env);
    cell_t *rest = CDR(exp);
    
#ifdef DEBUG
    printf("First is: ");
    pretty_print(first);
    printf("Rest is: ");
    pretty_print(rest);
#endif /* DEBUG */

    if (NULL == first) {
      DEBUG_PRINT_AND_RETURN(NULL);
    } else if (PRIMITIVEP(first)) {
      cell_t *(*f)(cell_t *, environ_t *) = CELL_PRIMITIVE(first);
      DEBUG_PRIM_PRINT_AND_RETURN((*f)(rest, env));
    } else if (FUNCTIONP(first)) { /* function call */
      DEBUG_INVOKE_PRINT_AND_RETURN(invoke(first, evargs(rest, env), env));
    }
      --__tl_eval_level;
      return NULL; /* Not primitive or funcall, error.*/
  }
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

cell_t *invoke(cell_t *fun, cell_t *args, environ_t *env) {
  int argslen, paramlen;
  environ_t *new_env;
  function_t *func = fun->slot2.fun;
  cell_t *ret;
  cell_t *code;

  argslen = proper_list_length(args);
  paramlen = proper_list_length(func->param_list);
  
  if (argslen != paramlen) return NULL; /* error */

  create_empty_environment(&new_env);
  extend(func->lexical_env, new_env, func->param_list, args);

  code = func->code;
  while (NULL != code && !NILP(code)) {
    ret = evaluate(CAR(code), new_env);
    code = CDR(code);
  }

  return ret;
}

cell_t *find_value(environ_t *env, cell_t *sym) {
  cell_t *ct = value(special_forms, sym);
  DEBUGPRINT("In special forms, sym is %p\n", ct);

  if (ct) {
    return ct;
  }

  ct = value(env, sym);
  DEBUGPRINT("In env, sym is %p\n", ct);
  
  if (NULL == ct) {
    DEBUGPRINT("In toplevel, sym is %p\n", value(toplevel, sym));
    return value(toplevel, sym);
  }
  
  return ct;
}
