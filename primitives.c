#include "primitives.h"
#include "cell.h"
#include "environment.h"
#include "eval.h"
#include "scheme-utils.h"

int proper_list_of_length(int length, cell_t *lst);

cell_t *prim_if(cell_t *rest, void *e) {
  environ_t *env = (environ_t *)e;
  cell_t *pred;

  if (!proper_list_of_length(3, rest)) return NULL;

  pred = evaluate(CAR(rest), env);
  
  if(NILP(pred) || FALSEP(pred)) {
    return evaluate(CADDR(rest), env);
  } else {
    return evaluate(CADR(rest), env);
  }
}

int proper_list_of_length(int length, cell_t *lst) {
  if (length < 0 || NULL == lst)
    return 0; /* error */

  for (;
       length > 0 && PAIRP(lst);
       length--, lst = CDR(lst)) {
    ;
  }

  if (length > 0) return FALSE; /* fail */
  
  return NILP(lst);
}
