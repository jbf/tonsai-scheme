#include "token.h"

#include <stdlib.h>

int main(int argc, char **argv, char **envp) {
  int i;
  token_t t;

  while ((i = get_token(&t)) != -1) {

    print_token(&t);

    if(t.type == STRING ||
       t.type == SYMBOL) {
      free(t.string_val);
    }
  }

  return 0;
}
