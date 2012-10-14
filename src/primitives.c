#include <assert.h>

#include "primitives.h"

#include "cell.h"
#include "environment.h"
#include "eval.h"
#include "errors.h"
#include "bootstrap.h"
#include "scheme-utils.h"
#include "function.h"
#include "memory.h"
#include "handles.h"
#include "util.h"

void inner_prim_error(cell_t *string_cell);
void string_error(const char *err_msg);

extern environ_t *internal; /* Lib of vm-internal scheme functions. */
extern environ_t *lib; /* Lib of public shceme functions. */

extern symbol_table *global_symtab;
extern cell_t *nil_cell;

/*
 * Helpers.
 */

/* Is this aloc-safe? */
int scheme_to_c_truth(cell_t *c, environ_t *env) {
  if (NULL == c) return EINVALID_TRUTH; /* error */

  if (NILP(c)) {
    return FALSE;
  }
  
  if (ATOMP(c)) {
    if (STRINGP(c)) {
      return TRUE;
    }
    
    cell_t *tmp = find_value(env, c);
    if (tmp == false_cell ||tmp == nil_cell) {
      return FALSE;
    }
  }
  return TRUE;
}

/*
 * Checks that 'lst' is a proper list, and not cyclic.
 *
 * If target_length is != 0 it checks max target_length + 1 cons-cells, and
 * returns 0 if length != target_length or target_length if lenght ==
 * target_length.
 *
 * If target_length is 0 it returns:
 *
 * length for a proper list
 * 0 for a 0-length list (NIL)
 * -1 if lst is NULL
 * -2 if lst i a cyclic list
 * -3 if lst is not a list
 * -4 if lst is not a proper list (ie not NIL as last elem)
 * -5 if last elem is a NULL-pointer
 * -6 if target_length < 0
 */
int proper_list_length(cell_t *lst, int target_length) {
  int length = 1;
  cell_t *runner = lst;

  if (target_length < 0) {
    return -6;
  }

  if (NULL == lst)
    return -1; /* error */

  if (NILP(lst)) {
    return 0;
  }

  if (PAIRP(lst)) {
    if (NILP(CDR(lst))) {
      return length; /* single cell*/
    } else {
      runner = CDR(lst);
    }
  } else {
    return -3; /* not a list */
  }

  while(NULL != lst &&
        NULL != runner &&
        (!target_length || length <= target_length )) {
    if (runner == lst) return -2; /* cyclic list */
    if (PAIRP(runner)) { /* safe to step both one step */
      runner = CDR(runner);
      lst = CDR(lst);
      length++;
      
      if (NULL != runner && PAIRP(runner)) { /* second step */
        runner = CDR(runner);
        length++;
      }
    } else if (NILP(runner)) { /* proper list, return lenght */
      return length;
    } else { /* improper list, return -1 */
      return -4;
    }
  }
  
  if (target_length && length > target_length) {
    return 0;
  }

  return -5;
}

/*
 * Do not use on symbol.
 */
int list_of(cell_type_t type, cell_t *lst) {
  assert(type != SYMBOL);
  if (NULL == lst) return -1;
  
  for (; lst != NULL && PAIRP(lst); lst = CDR(lst)) {
    if (!(CAR(lst) != NULL && CAR(lst)->slot1.type == type)) return 0;
  }

  return 1;
}

/*
 * Primitives.
 */
cell_t *prim_list(cell_t *rest, environ_t *env) {
  cell_t *first = NULL, *tmp = NULL, *next = NULL;
  handle_t *hf, *ht, *htt, *hr;

  hf = handle_push(first);
  hr = handle_push(rest);

  if (NILP(rest)) {
    first = nil_cell;
  } else {
    first = new(cell_t); // rest protected by handle
    handle_set(hf, first);
    tmp = evaluate(CAR(rest), env); // rest, first protected by handle
    first = handle_get(hf);
    rest = handle_get(hr);

    first->slot1.car = tmp;

    rest = CDR(rest);
    tmp = first;
    ht = handle_push(tmp);

    handle_set(hf, first);
    handle_set(hr, rest);
    
    while (!NILP(rest)) {
      cell_t *t = evaluate(CAR(rest), env); //rest, first, tmp handled
      htt = handle_push(t);
      next = new(cell_t); //rest, first, tmp,t handled
      t = handle_get(htt);
      handle_pop(htt);
      next->slot1.car = t;

      rest = handle_get(hr);
      tmp = handle_get(ht);

      tmp->slot2.cdr = next;
      tmp = next;
      rest = CDR(rest);

      handle_set(hr, rest);
      handle_set(ht, tmp);
    }

    tmp->slot2.cdr = nil_cell;
    handle_pop(ht);

    first = handle_get(hf);
  }

  handle_pop(hr);
  handle_pop(hf);
  return first;
}

cell_t *prim_setcar(cell_t *rest, environ_t *env) {
  cell_t *tmp, *res;
  handle_t *hr, *ht;

  if (proper_list_length(rest, 2) != 2) {
    fast_error("wrong arity in call to (set-car! ...) expected 2 arguments.");
  }
  
  hr = handle_push(rest);
  tmp = evaluate(CAR(rest), env); // rest handled
  if (!PAIRP(tmp)) {
    handle_pop(hr);
    fast_error("first operand to set-car! must be a pair.");
  }
  
  ht = handle_push(tmp);
  rest = handle_get(hr);
  res = evaluate(CAR(CDR(rest)), env); // tmp, rest handled

  tmp = handle_get(ht);
  tmp->slot1.car = res;
  
  handle_pop(ht);
  handle_pop(hr);
  return tmp;
}

cell_t *prim_setcdr(cell_t *rest, environ_t *env) {
  cell_t *tmp, *res;
  handle_t *hr, *ht;

  if (proper_list_length(rest, 2) != 2) {
    fast_error("wrong arity in call to (set-cdr! ...) expected 2 arguments.");
  }
  
  hr = handle_push(rest);
  tmp = evaluate(CAR(rest), env); // rest handled
  if (!PAIRP(tmp)) {
    handle_pop(hr);
    fast_error("first operand to set-cdr! must be a pair.");
  }

  ht = handle_push(tmp);
  rest = handle_get(hr);
  res = evaluate(CAR(CDR(rest)), env); // tmp, rest handled
  
  tmp = handle_get(ht);
  tmp->slot2.cdr = res;

  handle_pop(ht);
  handle_pop(hr);
  return tmp;
}

cell_t *prim_cons(cell_t *rest, environ_t *env) {
  cell_t *tmp;
  handle_t *hr;

  if (proper_list_length(rest, 2) != 2) {
    fast_error("wrong arity in call to (cons ...) expected 2 arguments.");
  }

  hr = handle_push(rest);
  tmp = new(cell_t); //rest handled
  rest = handle_get(hr);

  tmp->slot1.car = evaluate(CAR(rest), env); //rest handled
  rest = handle_get(hr);

  tmp->slot2.cdr = evaluate(CAR(CDR(rest)), env); //rest handeld

  handle_pop(hr);
  return tmp;
}

cell_t *prim_length(cell_t *rest, environ_t *env) {
  cell_t *tmp;
  int length;

  if (proper_list_length(rest, 1) != 1) {
    fast_error("wrong arity in call to (length ...) expected 1 argument of type list.");
  }

  tmp = evaluate(CAR(rest), env); //no handles needed
  length = proper_list_length(tmp, 0);
  if (length >= 0) {
    tmp = new(cell_t); //no handles neede
    tmp->slot1.type = NUMBER;
    tmp->slot2.i_val = length;
    return tmp;
  } else {
    fast_error("not a proper list.");
  }
  return NULL; /* unreachable */
}

cell_t *prim_plus(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env);
  int ok = list_of(NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    handle_t *ha;
    int tmp = 0;

    ha = handle_push(args);
    cell_t *rc = new(cell_t); // args handled
    args = handle_get(ha);
    handle_pop(ha);

    for (; args != NULL && PAIRP(args); args = CDR(args)) {
      tmp += I_VAL(CAR(args)); //this might overflow :)
    }

    rc->slot1.type = NUMBER;
    rc->slot2.i_val = tmp;
    return rc;
  }
}

cell_t *prim_mul(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env);
  int ok = list_of(NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    handle_t *ha;
    int tmp = 1;

    ha = handle_push(args);
    cell_t *rc = new(cell_t); // args handled
    args = handle_get(ha);
    handle_pop(ha);

    for (; args != NULL && PAIRP(args); args = CDR(args)) {
      tmp *= I_VAL(CAR(args)); // overflow!
    }

    rc->slot1.type = NUMBER;
    rc->slot2.i_val = tmp;
    return rc;
  }
}

cell_t *prim_number_equals(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env); //this implies strictness
  int ok = list_of(NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    int tmp = 1;
    cell_t *last = CAR(args);

    for (; args != NULL && PAIRP(args) && tmp; args = CDR(args)) {
      tmp = I_VAL(CAR(args)) == I_VAL(last);
      if (!tmp) break;
      last = CAR(args);
    }

    return tmp ? t_cell : false_cell;
  }
}

cell_t *prim_minus(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env);
  int ok = list_of(NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    handle_t *ha;
    int tmp = 2*I_VAL(CAR(args));

    ha = handle_push(args);
    cell_t *rc = new(cell_t); // args handled
    args = handle_get(ha);
    handle_pop(ha);

    for (; args != NULL && PAIRP(args); args = CDR(args)) {
      tmp -= I_VAL(CAR(args));
    }

    rc->slot1.type = NUMBER;
    rc->slot2.i_val = tmp;
    return rc;
  }
}

cell_t *prim_if(cell_t *rest, environ_t *env) {
  cell_t *pred;
  handle_t *hrest;

  if (proper_list_length(rest, 3) != 3) return NULL;

  hrest = handle_push(rest);
  pred = evaluate(CAR(rest), env); //rest handled
  int truth_value = scheme_to_c_truth(pred, env);
  rest = handle_get(hrest);
  handle_pop(hrest);

  if(truth_value < 0) {
    return NULL;
  } else if (truth_value) {
    return evaluate(CADR(rest), env); // safe due to return
  } else {
    return evaluate(CADDR(rest), env); // safe due to return
  }
}


cell_t *prim_lambda(cell_t *rest, environ_t *paren_lexical_env) {
  cell_t *tmp;
  function_t *fun;
  handle_t *hrest, *htmp;

  if (proper_list_length(rest, 0) < 2) return NULL;
  if (!(NILP(CAR(rest)) || proper_list_length(CAR(rest),0) > 0)) return NULL;

  /* asert params only symbols */

  hrest = handle_push(rest);
  tmp = new(cell_t); // rest handled

  htmp = handle_push(tmp);
  fun = new(function_t); // rest, tmp handled

  tmp = handle_get(htmp);
  rest = handle_get(hrest);
  handle_pop(htmp);
  handle_pop(hrest);

  fun->fun_cell = tmp;
  fun->lexical_env = paren_lexical_env;
  fun->param_list = CAR(rest);
  fun->code = CDR(rest);

  tmp->slot1.type = FUNCTION;
  tmp->slot2.fun = fun;
  return tmp;
}

/* add fun-name to internal env, do as if rest is lambda */
extern environ_t *internal;
cell_t *prim_definternal(cell_t *rest, environ_t *paren_lexical_env) {
  cell_t *val, *name_param = CAR(CAR(rest));
  rest->slot1.car = rest->slot1.car->slot2.cdr;
  
  if (!SYMBOLP(name_param)) {
    fast_error("1:st argument to 'define' must evaluate to a symbol.");
  }
    
  if (NULL == paren_lexical_env) {
    val = prim_lambda(rest, internal); /* internal is the global internal env */
  } else {
    val = prim_lambda(rest, paren_lexical_env);
  }

  add_to_environment(internal, name_param, val);
  return val;
}

/* add fun-name to lib env, do as if rest is lambda */
extern environ_t *lib;
cell_t *prim_deflibrary(cell_t *rest, environ_t *paren_lexical_env) {
  cell_t *val, *name_param = CAR(CAR(rest));
  rest->slot1.car = rest->slot1.car->slot2.cdr;
  
  if (!SYMBOLP(name_param)) {
    fast_error("1:st argument to 'define' must evaluate to a symbol.");
  }
    
  if (NULL == paren_lexical_env) {
    val = prim_lambda(rest, internal); /* lib also got internal env internal env */
  } else {
    val = prim_lambda(rest, paren_lexical_env);
  }

  add_to_environment(lib, name_param, val);
  return val;
}


cell_t *prim_quote(cell_t *rest, environ_t *env) {
  if (PAIRP(rest) && proper_list_length(rest,1) == 1) { 
    return CAR(rest);
  } else {
    fast_error("malformed quote.");
    GOTO_TOPLEVEL();
    return NULL; /* unreachable */
  }
}


cell_t *prim_eq(cell_t *rest, environ_t *env) {
  /* Rest Need to be a two-element list. */
  if(!proper_list_length(rest, 2)) {
    fast_error("wrong arity in call to (eq ...).");
  }

  return evaluate(CAR(rest),env) == evaluate(CADR(rest),env) // protected by return
    ? t_cell
    : false_cell;
}

extern int __tl_eval_level;
extern environ_t *toplevel;
cell_t *prim_define(cell_t *rest, environ_t *env) {
  cell_t *val;
  handle_t *hrest;
 
  /* Rest must be a symbol and an expression. */
  if (proper_list_length(rest,2) != 2) {
    fast_error("wrong arity in call to (define ...).");
  }
  if (!SYMBOLP(CAR(rest))) {
    fast_error("1:st argument to 'define' must evaluate to a symbol.");
  }
  
  /* Can only define at toplevel. */
  if (__tl_eval_level != 1) {
    fast_error("(define ...) only at toplevel.");
  }
  
  hrest = handle_push(rest);
  val = evaluate(CADR(rest), env); // rest protected
  rest = handle_get(hrest);
  handle_pop(hrest);

  add_to_environment(toplevel, CAR(rest), val);
  
  return CAR(rest);
}

/*
 * (error "string") or (error (form)) where 'form' must be printable.
 *
 * When properly implemented, this primitive won't return, but longjump to
 * toplevel and restart repl.
 */
cell_t *prim_error(cell_t *rest, environ_t *env) {
  int arity = 0;
  /*
   * (error ...) must be a form of two elements, error and a second
   * printable element.
   */
  if (proper_list_length(rest,1) != 1) {
    DEBUGPRINT("rest=%p is not a proper list of length 1.\n", rest);
    if (0 != arity) { pretty_print(rest); }
    printf("error: wrong arity in call to (error ...)\n");
    GOTO_TOPLEVEL();
  }
  
  if (STRINGP(CAR(rest))) {
    inner_prim_error(CAR(rest));
  } else {
    cell_t *tmp;
    tmp = evaluate(CAR(rest), env); // no protection, only tmp used
    printf("error: ");
    pretty_print(tmp);
    GOTO_TOPLEVEL();
  }
  
  DEBUGPRINT_("Not a valid error form.\n");
  pretty_print(rest);
  GOTO_TOPLEVEL();
  return NULL; /* unreachable */
}

void undefun_error(cell_t *first, cell_t *exp) {
  printf("error: undefined function '");
  pp(first);
  printf("' in expression: ");
  pretty_print(exp);
  GOTO_TOPLEVEL();
}

void undef_ident_error(cell_t *sym) {
  printf("error: undefined identifier: ");
  pretty_print(sym);
  GOTO_TOPLEVEL();
}

void inner_prim_error(cell_t *string_cell) {
  printf("error: ");
  pretty_print(string_cell);
  GOTO_TOPLEVEL();
}

void fast_error(const char *err_msg) {
  printf("error: %s\n", err_msg);
  GOTO_TOPLEVEL();
}

/* Debug stuff */
cell_t *prim_dump_symtab(cell_t* rest, environ_t *env) {
  print_symtab(global_symtab);
  return nil_cell;
}

cell_t *prim_dump_handles(cell_t* rest, environ_t *env) {
  print_handles();
  return nil_cell;
}

cell_t *prim_dump_roots(cell_t* rest, environ_t *env) {
  print_roots();
  return nil_cell;
}
