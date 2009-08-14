#include "token.h"

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int read_plus_minus(FILE *stream, int first, int index, unsigned char token_string[], token_t *tok);
int readstring(FILE *stream, int index, unsigned char token_string[], token_t *tok);
int readnumber(FILE *stream, int index, unsigned char token_string[], token_t *tok);
int readsymbol(FILE *stream, int index, unsigned char token_string[], token_t *tok);

int issymb(int c);
int isstop_pushback(FILE *stream, int c);

/*
 * Frees any extra memory possibly associated with 'tok'.
 */
void free_token_payload(token_t *tok) {
  if (tok->type == TOKEN_SYMBOL ||
      tok->type == TOKEN_STRING) {
    free(tok->payload);
  }
}

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
int get_token(FILE *stream, token_t *tok) {
  int c, token_index = 0;
  unsigned char token[MAX_TOKEN_LENGTH+1];

  while ((c = fgetc(stream)) != EOF &&
         token_index < MAX_TOKEN_LENGTH) {

    if (c < 0 || c > UCHAR_MAX) {
      return ERROR_UNREDABLE_CHAR;
    }

    /* So we can use a switch on whitespace. */
    if(isspace(c)) {
      c = ' ';
    }

    /* '-' and '+' need lookahead, thus special treatment. */
    if(c == '-' ||
       c == '+') {

       return read_plus_minus(stream, c, token_index, token, tok);
    }

    /* 
     * Numbers and symbols are (possibly) multichar. 
     *
     * Read the first char and dispatch to special handler for the rest.
     */
    if(isdigit(c) && 
       c != '0') { /* can't start with 0 */
      token[token_index] = (unsigned char)c;
      token_index++;
      return readnumber(stream, token_index, token, tok);
    }

    /* Se above. */
    if(issymb(c)) {
      token[token_index] = (unsigned char)c;
      token_index++;
      return readsymbol(stream, token_index, token, tok);
    }

    /* Handle the last cases. */
    switch (c) {
    case '(': tok->type = TOKEN_LPAREN;
      tok->atom_name = 0;
      return TOKEN_OK;
    case ')': tok->type = TOKEN_RPAREN;
      tok->atom_name = 0;
      return TOKEN_OK;
    case '"': return readstring(stream, token_index, token, tok);
      return ERROR_UNREACHABLE_STATEMENT; /* never reached */
    case ' ':
      continue;
    default: printf("%c\n", c);
      break;
    }

    token_index++;
  }

  /* What to return? */
  if (token_index == 0 &&
      c == EOF) {
    return NO_TOKEN;
  } else if (c == EOF) {
    return TOKEN_OK_EOF;
  } else {
    return ERROR_TOKEN_TO_LONG;
  }
}

/* First char was '+'/'-'. */
int read_plus_minus(FILE *stream,
                    int first,
                    int token_index,
                    unsigned char token[],
                    token_t *tok) {
  int next;

  next = fgetc(stream);

  /* If the next char is a whitespace, the symbol was '+' or '-'. */
  if(isspace(next) ||
     next == EOF) {
    unsigned char *str;
    str = malloc(2);

    if (!str) {
      return EOOM;
    }
         
    *str = (unsigned char)first;
    *(str+1) = '\0';

    tok->type = TOKEN_SYMBOL;
    tok->atom_name = str;

    return TOKEN_OK;
  }

  /* Number with leading '+' or '-' */
  if(isdigit(next) &&
     next != '0') { /* can't start with 0 */
    token[token_index] = (unsigned char)first;
    token_index++;
    token[token_index] = (unsigned char)next;
    token_index++;
    return readnumber(stream, token_index, token, tok);
  }

  /* Symbol starting with '+' or '-' */
  if(issymb(next)) {
    token[token_index] = (unsigned char)first;
    token_index++;
    token[token_index] = (unsigned char)next;
    token_index++;
    return readsymbol(stream, token_index, token, tok);
  }

  return ERROR_UNPARSABLE_TOKEN; /* '-' or '+' followed by weired
                                    shit. */
}

/* First char was symbol-constituent. */
int readsymbol(FILE *stream, int index, unsigned char token_string[], token_t *tok) {
  int c;
  
  while(index < MAX_TOKEN_LENGTH) {
    c = fgetc(stream);

    if (issymb(c)) {
      token_string[index] = (unsigned char)c;
      index++;
      continue;
    }

    /* If we are done, prepare the token and return. */
    if(isstop_pushback(stream, c) ||
       c == EOF) {
      char *tok_str;

      token_string[index] = '\0';

      tok->type = TOKEN_SYMBOL;

      tok_str = (char *)malloc(index+1);
      if (NULL == tok_str) {
        return EOOM;
      }
      
      strncpy(tok_str, (const char *)token_string, index + 1);

      tok->atom_name = (unsigned char *)tok_str;

      if (c == EOF) {
        return TOKEN_OK_EOF;
      } else {
        return TOKEN_OK;
      }
    }

    return ERROR_MALFORMED_SYMBOL;
  }

  return ERROR_TOKEN_TO_LONG;
}

/* Returns true if 'c' is symbol-constituent. */
int issymb(int c) {
  return isalpha(c) ||
    c == '-' ||
    c == '+' ||
    c == '!' ||
    c == '?' ||
    c == '_' ||
    c == '<' ||
    c == '>' ||
    c == '=' ||
    c == '|' ||
    c == '/' ||
    c == '*';
}

/* Peek at next char in 'FILE', return true if we should stop reading this symbol. */
int isstop_pushback(FILE *stream, int c) {
  if (isspace(c)) {
    return 1;
  } else if (c == ')') {
    ungetc(c, stream);
    return 1;
  } else {
    return 0;
  }
}

int readnumber(FILE *stream, int index, unsigned char token_string[], token_t *tok) {
  int c;

  while(index < MAX_TOKEN_LENGTH) {
    c = fgetc(stream);

    if(isstop_pushback(stream, c) ||
       c == EOF) {
      token_string[index] = '\0';

      char *end;
      end = (char *)&token_string[index];

      tok->type = TOKEN_NUMBER;
      tok->i_val = strtol((const char *)token_string, &end, 10);

      if (tok->i_val == LONG_MAX) {
        char * long_max_string = "2147483647";
        char *start;

        if (token_string[0] == '+') {
          start = ((char *)token_string)+1;
        } else {
          start = (char *)token_string;
        }

        if (strncmp(long_max_string, start, index+1)) {
          return ERROR_TOKEN_OVERFLOW;
        } else {
          return TOKEN_OK;
        }
      }

      if (tok->i_val == LONG_MIN) {
        char * long_min_string = "-2147483648";

        if (strncmp(long_min_string, (const char *)token_string, index+1)) {
          return ERROR_TOKEN_OVERFLOW;
        } else {
          return TOKEN_OK;
        }
      }

      if (c == EOF) {
        return TOKEN_OK_EOF;
      } else {
        return TOKEN_OK;
      }
    }

    if (c < '0' || c > '9') {
      return ERROR_MALFORMED_NUMBER;
    }

    token_string[index] = (unsigned char)c;
    index++;
  }

  return ERROR_TOKEN_TO_LONG;
}

int readstring(FILE *stream, int index, unsigned char token_string[], token_t *tok) {
  int c;

  /* The loop works, but is off by one. Don't care to fix it yet.*/
  index--;
  
  while(index < MAX_TOKEN_LENGTH) {
    c = fgetc(stream);

    if (c < 0 || c > UCHAR_MAX) {
      return ERROR_UNREDABLE_CHAR;
    }

    if (c == EOF) {
      return ERROR_MALFORMED_STRING;
    }

    index++;
    token_string[index] = (unsigned char)c;

    if((unsigned char)c == '"') {
      char *tok_str;

      token_string[index] = '\0';

      tok->type = TOKEN_STRING;
      
      tok_str = (char *)malloc(index+1);
      if (NULL == tok_str) {
        return EOOM;
      }
      
      strncpy(tok_str, (const char *)token_string, index+1);

      tok->string_val = (unsigned char *)tok_str;

      return TOKEN_OK;
    }
  }
  
  return ERROR_TOKEN_TO_LONG;
}

/*
 * Prints token 'tok' to FILE 'stream'. Since tokens are not first class
 * objects this is mostly for (VM) debugging purposes.
 */
int print_token(const token_t *tok) {
  switch (tok->type) {
  case TOKEN_LPAREN: printf("Token: LPAREN\n");
    return 0;
  case TOKEN_RPAREN: printf("Token: RPAREN\n");
    return 0;
  case TOKEN_STRING: printf("Token: STRING %s\n", tok->string_val);
    return 0;
  case TOKEN_NUMBER: printf("Token: NUMBER %d\n", tok->i_val);
    return 0;
  case TOKEN_SYMBOL: printf("Token: SYMBOL %s\n", tok->atom_name);
    return 0;
  default: printf("Unknown token.\n");
    return -1;
  }
}
