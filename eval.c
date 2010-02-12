#include "eval.h"
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"
#include "environment.h"
#include "primitives.h"
#include "bootstrap.h"
#include "function.h"
#include "errors.h"

/* Some of theses should be per thread, and some global but protected. */
environ_t *special_forms;
environ_t *toplevel;
static symbol_table __gs;
symbol_table *global_symtab = &__gs;
int __tl_eval_level = 0;
cell_t *orig_sexpr;

cell_t *find_value(environ_t *env, cell_t *sym);

enum {
  RET_VAL     = 1,
  RET_PRIM    = 2,
  RET_FUNCALL = 3,
};

#ifdef EVAL_DEBUG
#define DRETURN(type,x)                                         \
  do {                                                          \
    cell_t *__my_ret = (x);                                     \
    if (type == RET_VAL) {                                      \
      printf("Eval (%d) returns: ", __tl_eval_level);           \
    } else if (type == RET_PRIM) {                              \
      printf("Primitive (%d) returns: ", __tl_eval_level);      \
    } else {                                                    \
      printf("Invoke (%d) returns: ", __tl_eval_level);         \
    }                                                           \
    pretty_print(__my_ret);                                     \
    --__tl_eval_level;                                          \
    return __my_ret;                                            \
  } while (0)

#define DFLAG 1
#else /* != EVAL_DEBUG */
#define DRETURN(type, x)          \
  do {                            \
    cell_t *__my_ret = (x);       \
    --__tl_eval_level;            \
    return(__my_ret);             \
  } while (0)
#define DFLAG 0
#endif /* EVAL_DEBUG */

void init_eval() {
  boot(global_symtab, &special_forms);
  create_empty_environment(&toplevel);

#define DECLARE_PRIMITIVE(n, prim_op) do {                              \
    cell_t *s, *v = new(cell_t);                                        \
    primitive_t *p = new(primitive_t);                                  \
                                                                        \
    p->fun = &prim_op;                                                  \
    p->name = (unsigned char *)n;                                       \
    v->slot1.type = PRIMITIVE;                                          \
    v->slot2.prim = p;                                                  \
    s = intern((unsigned char *)n, global_symtab);                      \
    add_to_environment(special_forms, s, v);                            \
  } while (0)

  DECLARE_PRIMITIVE("if", prim_if);
  DECLARE_PRIMITIVE("+", prim_plus);
  DECLARE_PRIMITIVE("*", prim_mul);
  DECLARE_PRIMITIVE("=", prim_number_equals);
  DECLARE_PRIMITIVE("-", prim_minus);
  DECLARE_PRIMITIVE("lambda", prim_lambda);
  DECLARE_PRIMITIVE("quote", prim_quote);
  DECLARE_PRIMITIVE("define", prim_define);
  DECLARE_PRIMITIVE("error", prim_error);
  DECLARE_PRIMITIVE("length", prim_length);
  DECLARE_PRIMITIVE("eq?", prim_eq);
#undef DECLARE_PRIMITIVE
}

cell_t *evaluate(cell_t *exp, environ_t *env) {
  ++__tl_eval_level;
  if (DFLAG) {
    printf("Eval (%d) got : ", __tl_eval_level);
    pretty_print(exp);
  }

  if (NULL == exp) {
    DRETURN(RET_VAL, NULL);
  } else if (NILP(exp)) {
    fast_error("() is not autoquoting.");
    return NULL; /* Unreachable fast_error() does not return. */
  } else if (ATOMP(exp)) {
    if (SYMBOLP(exp)) {
      DRETURN(RET_VAL, find_value(env, exp));
    } else if (STRINGP(exp) || NUMBERP(exp)) {
      DRETURN(RET_VAL, exp);
    } else {
      DEBUGPRINT_("Expression not valid.\n");
      pretty_print(orig_sexpr);
      GOTO_TOPLEVEL();
      return NULL; /* unreachable */
    }
  } else { /* list */
    cell_t *first = evaluate(CAR(exp), env);
    cell_t *rest = CDR(exp);
    
    if (DFLAG) {
      printf("First is: ");
      pretty_print(first);
      printf("Rest is: ");
      pretty_print(rest);
    }

    if (NULL == first) {
      fast_error(" malformed expression.");
      /* This is unreachable */
    } else if (PRIMITIVEP(first)) {
      cell_t *(*f)(cell_t *, environ_t *) = CELL_PRIMITIVE(first);
      DRETURN(RET_PRIM, (*f)(rest, env));
    } else if (FUNCTIONP(first)) { /* function call */
      DRETURN(RET_FUNCALL, invoke(first, evargs(rest, env), env));
    }
    undefun_error(first, exp); /* Not primitive or funcall, error.*/
    return NULL; /* Unreachable, undefun_error() does not return. */
  }
}

cell_t *evargs(cell_t *args, environ_t *env) {
  cell_t *argsarray[16];
  int length;
  int i;
  cell_t *tmp, *head = nil_cell, *tail;

  if (!(length = proper_list_length(args, 0))) return NULL; /* error */
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

  argslen = proper_list_length(args,0);
  paramlen = proper_list_length(func->param_list, 0);
  
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
  if (ct) {
    return ct;
  }
 
  ct = value(toplevel, sym);
  DEBUGPRINT("In toplevel, sym is %p\n", ct);
  if (ct) {
    return ct;
  }

  DEBUGPRINT_("Value not bound.\n");
  GOTO_TOPLEVEL();
}
