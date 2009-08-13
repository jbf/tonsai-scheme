#ifndef _TOKEN_H
#define _TOKEN_H 1

#include "errors.h"

#include <stdint.h>
#include <stdio.h>

/*
 * These are the tokens recognized by this parser.
 */
typedef enum {
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_SYMBOL,
  TOKEN_NUMBER,
  TOKEN_STRING
} token_type_t;

/*
 * TOKEN - union member used
 *
 * LPAREN - atom_name (= NULL)
 * RPAREN - atom_name (= NULL)
 * SYMBOL - atom_name (= null-terminated string repr. of symbol)
 * NUMBER - i_val (= integer value of input)
 * STRING - string_val (= null-terminated string)
 */
typedef struct {
  token_type_t type;
  union {
    void *payload;
    unsigned char *atom_name;
    unsigned char *string_val;
    int32_t i_val;
  };
} token_t;

/* 
 * Read one token from 'stream' storing it in the memory pointed to by
 * 'tok'.
 *
 * No more than 'MAX_TOKEN_LENGTH' will be read, before the read is aborted
 * and an error is retuned.
 *
 * Some tokens require additional memory to be allocated. Use
 * 'free_token_payload' to free any extra memory.
 */
int get_token(FILE *stream, token_t *tok);

/*
 * Frees any extra memory possibly associated with 'tok'.
 */
void free_token_payload(token_t *tok);

/*
 * Prints token 'tok' to FILE 'stream'. Since tokens are not first class
 * objects this is mostly for (VM) debugging purposes.
 */
int print_token(const token_t *tok);

/* Return codes. */
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
