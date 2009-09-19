#include <stdio.h>

#include "cell.h"
#include "primitives.h"
#include "util.h"
#include "bootstrap.h"

int proper_list_of_length(int length, cell_t *lst);

#define test(str, expected) printf(#str); printf(": %d, expected %d\n", (str), (expected))

int main(int argc, char **argv, char **envp) {
  cell_t *c1, *c2, *c3, *c4;
  unsigned char* str = (unsigned char *)"Hej";
  unsigned char* smbl =(unsigned char *)"QUOTE";
  symbol_entry_t symbol;

  test(proper_list_of_length(0, NULL), 0);
  test(proper_list_of_length(1, NULL), 0);


  c1 = new(cell_t);
  c2 = new(cell_t);
  c3 = new(cell_t);
  c4 = new(cell_t);

  c1->slot1.type = PAYLOAD_NUMBER;
  c1->slot2.i_val = 47;

  c2->slot1.type = PAYLOAD_STRING;
  c2->slot2.string = str;

  symbol.symbol_name = smbl;
  symbol.symbol_cell = c3;
  c3->slot1.type = PAYLOAD_SYMBOL;
  c3->slot2.symbol = &symbol;

  CONS(c4, c1, c2);

  test(proper_list_of_length(0, c4), 0);
  test(proper_list_of_length(1, c4), 0);
  test(proper_list_of_length(10, c4), 0);

  CONS(c4, c1, nil_cell);
  CONS(c3, c1, c4);
  CONS(c2, c1, c3);

  pretty_print(c4);
  pretty_print(c3);
  pretty_print(c2);
  pretty_print(c1);
  
  test(proper_list_of_length(0, c4), 0);
  test(proper_list_of_length(1, c4), TRUE);
  test(proper_list_of_length(2, c4), FALSE);
  test(proper_list_of_length(10, c4), FALSE);

  test(proper_list_of_length(2, c3), TRUE);
  test(proper_list_of_length(3, c2), TRUE);

  return 0;
}
