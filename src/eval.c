#include "eval.h"
#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "memory.h"
#include "environment.h"
#include "primitives.h"
#include "bootstrap.h"
#include "function.h"
#include "errors.h"
#include "liveness.h"

#include <string.h>

/* Some of theses should be per thread, and some global but protected. */
environ_t *special_forms;
environ_t *toplevel;
environ_t *primitives;
environ_t *lib;
environ_t *internal;
static symbol_table __gs;
symbol_table *global_symtab = &__gs;
int __tl_eval_level = 0;
cell_t *orig_sexpr;
extern frame_t *live_root;

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

cell_t *evaluate(cell_t *exp, environ_t *env) {
  ++__tl_eval_level;
  if (DFLAG) {
    printf("Eval (%d) got : ", __tl_eval_level);
    pretty_print(exp);
  }

  if (NULL == exp) {
    DRETURN(RET_VAL, NULL);
  } else if (NILP(exp)) {
    DRETURN(RET_VAL, nil_cell);
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
#define MAX_LISP_ARGS 16
  cell_t *argsarray[MAX_LISP_ARGS];
  int length;
  int i;
  cell_t *tmp, *head = nil_cell, *tail;

  if ((length = proper_list_length(args, 0)) < 0) return NULL; /* error */
  if (length > MAX_LISP_ARGS) return NULL; /* can only handle 16 args atm */

  for (i = 0; i < length; i++, args = CDR(args)) {
    argsarray[i] = evaluate(CAR(args), env);
  }

  for (i = length - 1; i >= 0; i--) {
    tmp = new(cell_t); // liveness ok
    if (head != nil_cell) {
      pop_liveness(&live_root);
    }
    tail = head;
    head = argsarray[i];
    CONS(tmp, head, tail);
    head = tmp;
    
    push_liveness(&live_root, new_liveframe(1, head));
  }

  if (length > 0) {
    pop_liveness(&live_root);
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

/* This fixes the lookup order. If S is defined in an env looked into
 * earlier, it will override later environments.
 *
 * For example library code overrides primitives.
 */
cell_t *find_value(environ_t *env, cell_t *sym) {
  cell_t *ct = value(special_forms, sym);
#ifdef LOOKUP_DEBUG
  DEBUGPRINT("In special forms, sym is %p\n", ct);
#endif /* LOOKUP_DEBUG */
  if (ct) {
    return ct;
  }

  ct = value(env, sym);
#ifdef LOOKUP_DEBUG
  DEBUGPRINT("In env, sym is %p\n", ct);
#endif /* LOOKUP_DEBUG */
  if (ct) {
    return ct;
  }
 
  ct = value(toplevel, sym);
#ifdef LOOKUP_DEBUG
  DEBUGPRINT("In toplevel, sym is %p\n", ct);
#endif /* LOOKUP_DEBUG */
  if (ct) {
    return ct;
  }

  ct = value(lib, sym);
#ifdef LOOKUP_DEBUG
  DEBUGPRINT("In library, sym is %p\n", ct);
#endif /* LOOKUP_DEBUG */
  if (ct) {
    return ct;
  }

  ct = value(primitives, sym);
#ifdef LOOKUP_DEBUG
  DEBUGPRINT("In primitives, sym is %p\n", ct);
#endif /* LOOKUP_DEBUG */
  if (ct) {
    return ct;
  }

  undef_ident_error(sym);
  GOTO_TOPLEVEL();
}

typedef struct prim_container {
  struct prim_container *next;
  primitive_t *prim;
} prim_container;

static prim_container *all_prims = NULL;

void init_eval__safe() {
  boot(global_symtab, &special_forms);
  create_empty_environment(&toplevel);
  create_empty_environment(&primitives);
  create_empty_environment(&internal);
  create_empty_environment(&lib);

#include "primitives_eval_pre.h"
#include "primitives_declare.h"
#include "primitives_eval_post.h"

  if (load_lib_scm(global_symtab, lib, internal) == 0) {
    DEBUGPRINT_("Can not load \"lib/lib_boot.scm\". Exiting.\n");
    exit(1);
  }
}

void destroy_eval__safe() {
  prim_container *c = all_prims;

  while (c != NULL) {
    prim_container *t;
    free_malloced(c->prim);
    t = c;
    c = c->next;
    free_malloced(t);
  }

  /* Frees containers and non heap values (?) held in
     environments. */
  destroy_env(&special_forms);
  destroy_env(&toplevel);
  destroy_env(&primitives);
  destroy_env(&lib);
  destroy_env(&internal);


  //orig_sexpr == NULL;
  //*live_root;

  /* This will free resources held by symbol representation. */
  free_symtab(global_symtab);
  global_symtab = NULL;
  __gs.head = NULL;
}