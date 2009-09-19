#include "symbol.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp) {
  symbol_table s, *symtab=&s;
  
  unsigned char *my_foo = (unsigned char *)"FOO";
  unsigned char *my_bar = (unsigned char *)"BAR";
  unsigned char *new_bar;

  init_symbol_table(symtab);

  new_bar = malloc(4);
  new_bar[0] = 'B';
  new_bar[1] = 'A';
  new_bar[2] = 'R';
  new_bar[3] = '\0';
  
  printf("Looking up FOO, should return (nil): %p. FOO used for lookup is at %p\n",
         lookup(my_foo, symtab),
         my_foo);

  printf("Interning BAR from\t%p. Should return non-NULL address:\t%p.\n",
         my_bar,
         intern(my_bar, symtab));
  printf("Looking up BAR from\t%p. Should return same non-NULL address:\t%p.\n",
         new_bar,
         lookup(new_bar, symtab));
  return 0;
}
