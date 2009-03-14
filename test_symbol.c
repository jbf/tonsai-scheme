#include "symbol.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp) {
  symbol_entry_t *nil_sym;
  symtab_entry_t *symtab;
  unsigned char *nil_str;

  unsigned char *my_nil = (unsigned char *)"NIL";
  unsigned char *my_foo = (unsigned char *)"FOO";
  unsigned char *my_bar = (unsigned char *)"BAR";
  unsigned char *new_bar;

  new_bar = malloc(4);
  new_bar[0] = 'B';
  new_bar[1] = 'A';
  new_bar[2] = 'R';
  new_bar[3] = '\0';
  
  nil_sym = new(symbol_entry_t);
  symtab = new(symtab_entry_t);
  nil_str = (unsigned char *)malloc(4);
  nil_str[0] = 'N';
  nil_str[1] = 'I';
  nil_str[2] = 'L';
  nil_str[3] = '\0';

  nil_sym->symbol_name = nil_str;
  nil_sym->symbol_value = NULL;
  symtab->next = NULL;
  symtab->symbol = nil_sym;

  printf("Looking up NIL, should return non-NULL address: %p. NIL used for lookup is at: %p\n",
         lookup(my_nil, symtab),
         my_nil);
  printf("Looking up FOO, should return (nil): %p. FOO used for lookup is at %p\n",
         lookup(my_foo, symtab),
         my_foo);

  printf("Interning BAR from\t%p. Should return non-NULL address:\t%p.\n",
         my_bar,
         intern2(my_bar, &symtab, symtab));
  printf("Looking up BAR from\t%p. Should return same non-NULL address:\t%p.\n",
         new_bar,
         lookup(new_bar, symtab));
  return 0;
}
