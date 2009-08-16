#include "cell.h"
#include "symbol.h"
#include "reader.h"
#include "util.h"

cell_t *evaluate(cell_t *exp /* later add env */) {
  /* if (NULL == exp) { */
  /*   return NULL; */
  /* } else if (ATOMP(exp)) { */
  /*   /\*if (SYMBOLP(exp)) { */
  /*     return lookup(exp->symbol, env); */
  /*     } else *\/ if (STRINGP(exp) || NUMBERP(exp)) { */
  /*     return exp; */
  /*   } else { */
  /*     return NULL; /\* error *\/ */
  /*   } */
  /* } else if (PAIRP(exp)) { */
  /*   return NULL; /\* evaluate car, invoke on cdr *\/ */
  /* } else { */
  /*   return NULL; */
  /* } */

  /* temp */ return NULL;
}
