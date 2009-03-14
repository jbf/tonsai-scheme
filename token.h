#ifndef _TOKEN_H
#define _TOKEN_H 1

#include "errors.h"

#include <stdint.h>

typedef enum {
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_SYMBOL,
  TOKEN_NUMBER,
  TOKEN_STRING
} token_type_t;

typedef struct {
  token_type_t type;
  union {
    void *payload;
    unsigned char *atom_name;
    unsigned char *string_val;
    int32_t i_val;
  };
} token_t;

int get_token(token_t *tok);
int print_token(const token_t *tok);

void free_token(token_t *token);
void *free_token_reuse_payload(token_t *token);

#define TOKEN_OK 1
#define TOKEN_OK_EOF 2

#define NO_TOKEN 10

#define MAX_TOKEN_LENGTH 256

/* tokenizer recoverable errors -2000 to -2999 */
#define ERROR_UNREDABLE_CHAR -2100
#define ERROR_UNPARSABLE_TOKEN -2101
#define ERROR_TOKEN_TO_LONG -2102
#define ERROR_MALFORMED_SYMBOL -2103
#define ERROR_MALFORMED_NUMBER -2104
#define ERROR_TOKEN_OVERFLOW -2105
#define ERROR_MALFORMED_STRING -2106

#endif /* _TOKEN_H */
