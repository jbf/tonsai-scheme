#include "environment.h"
#include "symbol.h"
#include "util.h"
#include "cell.h"

#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv, char **envp) {
  symtab_entry_t *symtab;
  environ_t *env;

  unsigned char *my_nil = (unsigned char *)"NIL";
  unsigned char *my_bar = (unsigned char *)"BAR";
  unsigned char *my_foo = (unsigned char *)"FOO";
  unsigned char *new_bar;

  symbol_entry_t *my_nil_sym, *my_bar_sym, *my_foo_sym;

  cell_t num_10 = {{.type = PAYLOAD_NUMBER}, {.i_val = 10}};
  cell_t str_hej = {{.type = PAYLOAD_STRING}, {.string = (unsigned char *)"Hej\t..."}};

  new_bar = malloc(4);
  new_bar[0] = 'B';
  new_bar[1] = 'A';
  new_bar[2] = 'R';
  new_bar[3] = '\0';
  
  create_initial_symtab(&symtab);
  create_initial_environment(&env);

  my_nil_sym =lookup(my_nil, symtab);
  my_bar_sym = intern2(my_bar, &symtab, symtab);
  my_foo_sym = intern2(my_foo, &symtab, symtab);

  printf("Value of %p is ", my_nil_sym);
  print_cell(value(env, my_nil_sym));
  printf("\n\n");

  printf("Value of %p is ", my_bar_sym);
  print_cell(value(env, my_bar_sym));
  printf("\n");
  printf("Adding BAR => 10 binding\n");
  add_to_environment(env, my_bar_sym, &num_10);

  printf("Value of %p is ", my_bar_sym);
  print_cell(value(env, my_bar_sym));
  printf("\n\n");

  printf("Value of %p is ", my_foo_sym);
  print_cell(value(env, my_foo_sym));
  printf("\n");
  printf("Adding FOO => \"Hej\\t...\" binding\n");
  add_to_environment(env, my_foo_sym, &str_hej);

  printf("Value of %p is ", my_foo_sym);
  print_cell(value(env, my_foo_sym));
  printf("\n\n");

  printf("Value of %p is (still) ", my_nil_sym);
  print_cell(value(env, my_nil_sym));
  printf("\n\n");

  return 0;
}
