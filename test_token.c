#include "token.h"
#include "util.h"

#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  int i;
  token_t *t;

  while ((i = get_token(t = new(token_t))) == TOKEN_OK) {
    print_token(t);
    free_token(t);
  }

  printf("Error code is: %d\n", i);

  return 0;
}
