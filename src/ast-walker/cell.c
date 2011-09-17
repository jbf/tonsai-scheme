#include <stdio.h>

#include "cell.h"
#include "function.h"
#include "primitives.h"
#include "util.h"

void pp_list(cell_t *cell);
void pp(cell_t *cell);

#undef PP_DEBUG

void print_cell(cell_t *cell) {
  if (NULL == cell) {
    printf ("C-NULL");
  } else if (NILP(cell)) {
    printf("()");
  } else if (PAIRP(cell)) {
    printf("PAIR");
  } else if (SYMBOLP(cell)) {
#ifdef PP_DEBUG
    printf("SYMBOL ");
#endif /* PP_DEBUG */
    printf("%s", SYMBOL_NAME(CELL_SYMBOL(cell)));
  } else if (NUMBERP(cell)) {
#ifdef PP_DEBUG
    printf("NUMBER ");
#endif /* PP_DEBUG */
    printf("%d", I_VAL(cell));
  } else if (STRINGP(cell)) {
#ifdef PP_DEBUG
    printf("STRING ");
#endif /* PP_DEBUG */
    printf("\"%s\"", STRING_VAL(cell));
  } else if (PRIMITIVEP(cell)) {
    printf("PRIMITIVE FUNCTION CELL %s ", CELL_PRIMITIVE_NAME(cell));
  } else if (FUNCTIONP(cell)) {
    print_fun(cell);
  } else {
    printf("UNKNOWN CELL %p, %p", CAR(cell), CDR(cell));
  }
}

void pretty_print(cell_t *cell) {
  pp(cell);
  printf("\n");
}

void pp(cell_t *cell) {
  if (NULL == cell) {
    printf("C-NULL");
    return;
  }

  if (PAIRP(cell)) {
    printf("(");
    pp_list(cell);
    printf(")");
  } else {
    print_cell(cell);
  }
}

void pp_list(cell_t *cell) {
  if (NULL != cell && PAIRP(cell)) {
      pp(CAR(cell));
      cell = CDR(cell);
  }

  while (!NILP(cell)) {
    if (PAIRP(cell)) {
      printf(" ");
      pp(CAR(cell));
      cell = CDR(cell);
    } else {
      printf(" . ");
      pp(cell); /* Improper list */
      break;
    }
  }
}

const char *object_type_str(cell_t *cell) {
#define DECLARE_OBJECT_TYPE(x, y) if ( x ## P (cell)) { return QUOTEME(x) ; } else 
#include "heap_objects_declare.h"
#undef DECLARE_OBJECT_TYPE
  { /* this continues the last else from macro expansion */
    return "Unknwn object";
  }

  return "Unknown object";
}
