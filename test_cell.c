#include "cell.h"

#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
  cell_t *c1, *c2, *c3, *c4;
  unsigned char* str = (unsigned char *)"Hej";
  unsigned char* smbl =(unsigned char *)"QUOTE";

  c1 = alloc_cell();
  c2 = alloc_cell();
  c3 = alloc_cell();
  c4 = alloc_cell();

  c1->type = PAYLOAD_NUMBER;
  c1->i_val = 47;
  print_cell(c1);

  c2->type = PAYLOAD_STRING;
  c2->string_val = str;
  print_cell(c2);

  c3->type = PAYLOAD_SYMBOL;
  c3->symbol_val = smbl;
  print_cell(c3);

  CONS(c4, c1, c2);
  print_cell(c4);

  return 0;
}
