#include "token.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  int i;
  token_t *t;

  while ((i = get_token(t = alloc_token())) == TOKEN_OK) {
    print_token(t);
    free_token(t);
  }

  printf("Error code is: %d\n", i);

  return 0;
}
