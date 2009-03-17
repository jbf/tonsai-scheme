#include "token.h"

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int read_plus_minus(int first, int index, unsigned char token_string[], token_t *tok);
int readstring(int index, unsigned char token_string[], token_t *tok);
int readnumber(int index, unsigned char token_string[], token_t *tok);
int readsymbol(int index, unsigned char token_string[], token_t *tok);

int issymb(int c);
int isstop_pushback(int c);

void free_token(token_t *tok) {
  if (tok->type == TOKEN_SYMBOL ||
      tok->type == TOKEN_STRING) {
    free(tok->payload);
  }

  free(tok);
}

void *free_token_reuse_payload(token_t *tok) {
  void *payload;

  if (tok->type == TOKEN_SYMBOL ||
      tok->type == TOKEN_STRING) {
    payload = tok->payload;
    tok->type = TOKEN_NUMBER;
  }

  free_token(tok);
  return payload;
}

int get_token(token_t *tok) {
  int c, token_index = 0;
  unsigned char token[MAX_TOKEN_LENGTH+1];

  while ((c = fgetc(stdin)) != EOF &&
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

      return read_plus_minus(c, token_index, token, tok);
    }

    if(isdigit(c) && 
       c != '0') { /* can't start with 0 */
      token[token_index] = (unsigned char)c;
      token_index++;
      return readnumber(token_index, token, tok);
    }

    if(issymb(c)) {
      token[token_index] = (unsigned char)c;
      token_index++;
      return readsymbol(token_index, token, tok);
    }

    switch (c) {
    case '(': tok->type = TOKEN_LPAREN;
      tok->atom_name = 0;
      return TOKEN_OK;
    case ')': tok->type = TOKEN_RPAREN;
      tok->atom_name = 0;
      return TOKEN_OK;
    case '"': return readstring(token_index, token, tok);
      return ERROR_UNREACHABLE_STATEMENT; /* never reached */
    case ' ':
      continue;
    default: printf("%c\n", c);
      break;
    }

    token_index++;
  }

  if (token_index == 0 &&
      c == EOF) {
    return NO_TOKEN;
  } else if (c == EOF) {
    return TOKEN_OK_EOF;
  } else {
    return ERROR_TOKEN_TO_LONG;
  }
}

int read_plus_minus(int first, int token_index, unsigned char token[], token_t *tok) {
  int next;

  next = fgetc(stdin);

  /* The actual symbols '+' or '-' */
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
    return readnumber(token_index, token, tok);
  }

  /* Symbol starting with '+' or '-' */
  if(issymb(next)) {
    token[token_index] = (unsigned char)first;
    token_index++;
    token[token_index] = (unsigned char)next;
    token_index++;
    return readsymbol(token_index, token, tok);
  }

  return ERROR_UNPARSABLE_TOKEN; /* '-' or '+' followed by weired
                                    shit. */
}


int readsymbol(int index, unsigned char token_string[], token_t *tok) {
  int c;
  
  while(index < MAX_TOKEN_LENGTH) {
    c = fgetc(stdin);

    if (issymb(c)) {
      token_string[index] = (unsigned char)c;
      index++;
      continue;
    }

    if(isstop_pushback(c) ||
       c == EOF) {
      char *tok_str;

      token_string[index] = '\0';

      tok->type = TOKEN_SYMBOL;

      tok_str = (char *)malloc(index+1);
      if (NULL == tok_str) {
        return EOOM;
      }
      
      strncpy(tok_str, (const char *)token_string, index + 1);

      tok->string_val = (unsigned char *)tok_str;

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

int isstop_pushback(int c) {
  if (isspace(c)) {
    return 1;
  } else if (c == '(' ||
             c == ')') {
    ungetc(c, stdin);
    return 1;
  } else {
    return 0;
  }
}

int readnumber(int index, unsigned char token_string[], token_t *tok) {
  int c;

  while(index < MAX_TOKEN_LENGTH) {
    c = fgetc(stdin);

    if(isstop_pushback(c) ||
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

int readstring(int index, unsigned char token_string[], token_t *tok) {
  int c;

  /* The loop works, but is off by one. Don't care to fix it yet.*/
  index--;
  
  while(index < MAX_TOKEN_LENGTH) {
    c = fgetc(stdin);

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
