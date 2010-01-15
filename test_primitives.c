#include <stdio.h>

#include "cell.h"
#include "primitives.h"
#include "util.h"
#include "bootstrap.h"

int proper_list_of_length(int length, cell_t *lst);

#define test(str, expected) printf(#str); printf(":%5d, expected: %5d\t\t... %s\n", (str), (expected), ((str)==(expected)?"SUCCESS":"FAIL"))

int main(int argc, char **argv, char **envp) {
  cell_t *c1, *c2, *c3, *c4, *c5, *c6, *c7, *c8, *c9;
  cell_t *c10, *c11, *c12, *c13, *c14, *c15, *c16, *c17;
  unsigned char* str = (unsigned char *)"Hej";
  unsigned char* smbl =(unsigned char *)"QUOTE";
  symbol_entry_t symbol;

  test(proper_list_length(NULL, 0), -1);

  c1 = new(cell_t);
  c2 = new(cell_t);
  c3 = new(cell_t);
  c4 = new(cell_t);
  c5 = new(cell_t);
  c6 = new(cell_t);
  c7 = new(cell_t);
  c8 = new(cell_t);
  c9 = new(cell_t);
  c10 = new(cell_t);
  c11 = new(cell_t);
  c12 = new(cell_t);
  c13 = new(cell_t);
  c14 = new(cell_t);
  c15 = new(cell_t);
  c16 = new(cell_t);
  c17 = new(cell_t);

  c1->slot1.type = PAYLOAD_NUMBER;
  c1->slot2.i_val = 47;

  c2->slot1.type = PAYLOAD_STRING;
  c2->slot2.string = str;

  symbol.symbol_name = smbl;
  symbol.symbol_cell = c3;
  c3->slot1.type = PAYLOAD_SYMBOL;
  c3->slot2.symbol = &symbol;

  test(proper_list_length(c1, 0), -3);
  test(proper_list_length(c2, 0), -3);
  test(proper_list_length(c3, 0), -3);

  CONS(c4, c1, c2);

  test(proper_list_length(c4, 0), -4);

  CONS(c4, c1, nil_cell);
  CONS(c3, c1, c4);
  CONS(c2, c1, c3);
  CONS(c5, c1, c2);
  CONS(c6, c1, c5);
  CONS(c7, c1, c6);
  CONS(c8, c1, c7);
  CONS(c9, c1, c8);
  CONS(c10, c1, c9);
  CONS(c11, c1, c10);
  CONS(c12, c1, c11);
  CONS(c13, c1, c12);
  CONS(c14, c1, c13);
  CONS(c15, c1, c14);
  CONS(c16, c1, c15);
  CONS(c17, c1, c16);

  test(proper_list_length(c4, 0), 1);
  test(proper_list_length(c3, 0), 2);
  test(proper_list_length(c2, 0), 3);

  test(proper_list_length(c17, 0), 16);

  CONS(c4, c1, c17);
  test(proper_list_length(c17, 0), -2);

  CONS(c4, c1, c1);
  test(proper_list_length(c17, 0), -4);

  c4->slot2.cdr = NULL;
  test(proper_list_length(c17, 0), -5);

  CONS(c3, c1, nil_cell);
  test(proper_list_length(c17, 0), 15);

  c3->slot2.cdr = NULL;
  test(proper_list_length(c17, 0), -5);

  return 0;
}
