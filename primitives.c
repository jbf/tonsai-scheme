#include "primitives.h"
#include "cell.h"
#include "environment.h"
#include "eval.h"
#include "errors.h"
#include "bootstrap.h"
#include "scheme-utils.h"
#include "function.h"

#include <assert.h>

void inner_prim_error(cell_t *string_cell);
void string_error(const char *err_msg);

/*
 * Helpers.
 */

int scheme_to_c_truth(cell_t *c, environ_t *env) {
  if (NULL == c) return EINVALID_TRUTH; /* error */
  
  if (ATOMP(c) && value(env, c) == false_cell) {
    return FALSE;
  } else if (ATOMP(c) && value(env, c) == nil_cell) {
    return FALSE;
  }
  return TRUE;
}

/*
 * Checks that 'lst' is a proper list, and not cyclic.
 *
 * If target_lenght is != 0 it checks max target_length + 1 cons-cells, and
 * returns 0 if length != target_length or target_lenght if that is lenght.
 *
 * If target_lenght is 0 it returns:
 *
 * length for a list
 * 0 for a 0-length list (NIL)
 * -1 if lst is NULL
 * -2 if lst i a cyclic list
 * -3 if lst is not a list
 * -4 if lst is not a proper list (ie not NIL as last elem)
 * -5 if last elem is a NULL-pointer
 * -6 if target_lenght < 0
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
  assert(type != PAYLOAD_SYMBOL);
  if (NULL == lst) return -1;
  
  for (; lst != NULL && PAIRP(lst); lst = CDR(lst)) {
    if (!(CAR(lst) != NULL && CAR(lst)->slot1.type == type)) return 0;
  }

  return 1;
}

/*
 * Primitives.
 */
cell_t *prim_length(cell_t *rest, environ_t *env) {
  cell_t *tmp;
  int length;

  if (proper_list_length(rest, 1) != 1) {
    fast_error("wrong arity in call to (length ...) expected 1 argument of type list.");
  }

  tmp = evaluate(CAR(rest), env);
  length = proper_list_length(tmp, 0);
  if (length >= 0) {
    tmp = new(cell_t);
    tmp->slot1.type = PAYLOAD_NUMBER;
    tmp->slot2.i_val = length;
    return tmp;
  } else {
    fast_error("not a proper list.");
  }
  return NULL; /* unreachable */
}

cell_t *prim_plus(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env);
  int ok = list_of(PAYLOAD_NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    int tmp = 0;
    cell_t *rc = new(cell_t);

    for (; args != NULL && PAIRP(args); args = CDR(args)) {
      tmp += I_VAL(CAR(args));
    }

    rc->slot1.type = PAYLOAD_NUMBER;
    rc->slot2.i_val = tmp;
    return rc;
  }
}

cell_t *prim_mul(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env);
  int ok = list_of(PAYLOAD_NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    int tmp = 1;
    cell_t *rc = new(cell_t);

    for (; args != NULL && PAIRP(args) && tmp; args = CDR(args)) {
      tmp *= I_VAL(CAR(args));
    }

    rc->slot1.type = PAYLOAD_NUMBER;
    rc->slot2.i_val = tmp;
    return rc;
  }
}

cell_t *prim_number_equals(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env);
  int ok = list_of(PAYLOAD_NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    int tmp = 1;
    cell_t *last = CAR(args);

    for (; args != NULL && PAIRP(args) && tmp; args = CDR(args)) {
      tmp = I_VAL(CAR(args)) == I_VAL(last);
      last = CAR(args);
    }

    return tmp ? t_cell : false_cell;
  }
}

cell_t *prim_minus(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env);
  int ok = list_of(PAYLOAD_NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    int tmp = 2*I_VAL(CAR(args));
    cell_t *rc = new(cell_t);

    for (; args != NULL && PAIRP(args); args = CDR(args)) {
      tmp -= I_VAL(CAR(args));
    }

    rc->slot1.type = PAYLOAD_NUMBER;
    rc->slot2.i_val = tmp;
    return rc;
  }
}

cell_t *prim_if(cell_t *rest, environ_t *env) {
  cell_t *pred;

  if (proper_list_length(rest, 3) != 3) return NULL;

  pred = evaluate(CAR(rest), env);
  int truth_value = scheme_to_c_truth(pred, env);

  if(truth_value < 0) {
    return NULL;
  } else if (truth_value) {
    return evaluate(CADR(rest), env);
  } else {
    return evaluate(CADDR(rest), env);
  }
}


cell_t *prim_lambda(cell_t *rest, environ_t *env) {
  cell_t *tmp;
  function_t *fun;

  if (proper_list_length(rest, 0) < 2) return NULL;
  if (!(NILP(CAR(rest)) || proper_list_length(CAR(rest),0) > 0)) return NULL;

  /* asert params only symbols */

  tmp = new(cell_t);
  fun = new(function_t);

  fun->fun_cell = tmp;
  fun->lexical_env = env;
  fun->param_list = CAR(rest);
  fun->code = CDR(rest);

  tmp->slot1.type = FUNCTION;
  tmp->slot2.fun = fun;
  return tmp;
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
    tmp = evaluate(CAR(rest), env);
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

void inner_prim_error(cell_t *string_cell) {
  printf("error: ");
  pretty_print(string_cell);
  GOTO_TOPLEVEL();
}

void fast_error(const char *err_msg) {
  printf("error: %s\n", err_msg);
  GOTO_TOPLEVEL();
}

cell_t *prim_eq(cell_t *rest, environ_t *env) {
  /* Rest Need to be a two-element list. */
  if(!proper_list_length(rest, 2)) {
    fast_error("wrong arity in call to (eq ...).");
  }

  return CAR(rest) == CADR(rest) ? t_cell : false_cell;
}

extern int __tl_eval_level;
extern environ_t *toplevel;
cell_t *prim_define(cell_t *rest, environ_t *env) {
  cell_t *val;
  
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
  
  val = evaluate(CADR(rest), env);
  add_to_environment(toplevel, CAR(rest), val);
  
  return CAR(rest);
}
