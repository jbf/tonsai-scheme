#include "primitives.h"
#include "cell.h"
#include "environment.h"
#include "eval.h"
#include "bootstrap.h"
#include "scheme-utils.h"

#include <assert.h>

int proper_list_of_length(int length, cell_t *lst);

int scheme_to_c_truth(cell_t *c, environ_t *env) {
  if (NULL == c) return EINVALID_TRUTH; /* error */
  
  if (ATOMP(c) && value(env, c) == false_cell) {
    return FALSE;
  }
  
  return TRUE;
}

cell_t *prim_if(cell_t *rest, environ_t *env) {
  cell_t *pred;

  if (!proper_list_of_length(3, rest)) return NULL;

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

int proper_list_of_length(int length, cell_t *lst) {
  if (length < 0 || NULL == lst)
    return 0; /* error */

  for (;
       length > 0 && NULL != lst && PAIRP(lst);
       length--, lst = CDR(lst)) {
    ;
  }

  if (NULL == lst || length > 0) return FALSE; /* fail */
  
  return NILP(lst);
}

/*
 * Do not use on symbol.
 */
int list_of(cell_type_t type, cell_t *lst) {
  assert(type != PAYLOAD_SYMBOL);
  if (NULL == lst) return -1;
  
  for (lst; lst != NULL && PAIRP(lst); lst = CDR(lst))
    if (!(CAR(lst) != NULL && CAR(lst)->slot1.type == type)) return 0;

  return 1;
}

cell_t *prim_plus(cell_t *rest, environ_t *env) {
  cell_t *args = evargs(rest, env);
  int ok = list_of(PAYLOAD_NUMBER, args);

  if (ok < 1) {
    return NULL; /* ERROR */
  } else {
    int tmp = 0;
    cell_t *rc = new(cell_t);

    for (args; args != NULL && PAIRP(args); args = CDR(args))
      tmp += I_VAL(CAR(args));

    rc->slot1.type = PAYLOAD_NUMBER;
    rc->slot2.i_val = tmp;
    return rc;
  }
}
