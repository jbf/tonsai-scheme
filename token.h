#ifndef _TOKEN_H
#define _TOKEN_H	1

#include <stdint.h>

typedef enum {
  LPAREN,
  RPAREN,
  SYMBOL,
  NUMBER,
  STRING
} token_type_t;

typedef struct {
  token_type_t type;
  union {
    unsigned char *atom_name;
    unsigned char *string_val;
    int32_t i_val;
  };
} token_t;

int get_token(token_t *tok);
int print_token(const token_t *tok);

token_t *alloc_token();
int free_token(token_t *token);
void *free_token_reuse_payload(token_t *token);

#endif /* _TOKEN_H */
