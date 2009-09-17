#include "util.h"
#include "cell.h"
#include "symbol.h"

#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
  cell_t *c1, *c2, *c3, *c4;
  unsigned char* str = (unsigned char *)"Hej";
  unsigned char* smbl =(unsigned char *)"QUOTE";
  symbol_entry_t symbol;

  c1 = new(cell_t);
  c2 = new(cell_t);
  c3 = new(cell_t);
  c4 = new(cell_t);

  c1->slot1.type = PAYLOAD_NUMBER;
  c1->slot2.i_val = 47;
  print_cell(c1);
  printf("\n");

  c2->slot1.type = PAYLOAD_STRING;
  c2->slot2.string = str;
  print_cell(c2);
  printf("\n");

  symbol.symbol_name = smbl;
  c3->slot1.type = PAYLOAD_SYMBOL;
  c3->slot2.symbol = &symbol;
  print_cell(c3);
  printf("\n");

  CONS(c4, c1, c2);
  print_cell(c4);
  printf("\n");
  printf("CAR: "); print_cell(CAR(c4)); printf("\n");
  printf("CDR: "); print_cell(CDR(c4)); printf("\n");

  return 0;
}
