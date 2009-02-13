#include "token.h"

#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 256

int readstring(int, unsigned char[], token_t *);
int readnumber(int, unsigned char[], token_t *);
int readsymbol(int, unsigned char[], token_t *);

int issymb(int);
int isstop_pushback(int);

int get_token(token_t *tok) {
  int c, token_index = 0;
  unsigned char token[MAX_TOKEN_LENGTH+1];

  while ((c = fgetc(stdin)) != EOF &&
         token_index < MAX_TOKEN_LENGTH) {

    if (c < 0 || c > UCHAR_MAX) {
      return -1;
    }

    if(isspace(c)) {
      c = ' ';
    }

    if(c == '-' ||
       c == '+') {
      int next;

      next = fgetc(stdin);

      /* The actual symbols '+' or '-' */
      if(isspace(next)) {
        unsigned char *str;
        str = malloc(2);

        if (!str) {
          return -1;
        }
         
        *str = (unsigned char)c;
        *(str+1) = '\0';

        tok->type = SYMBOL;
        tok->atom_name = str;

        return 0;
      }

      /* Number with leading '+' or '-' */
      if(isdigit(next) &&
         next != '0') { /* can't start with 0 */
        token[token_index] = (unsigned char)c;
        token_index++;
        token[token_index] = (unsigned char)next;
        token_index++;
        return readnumber(token_index, token, tok);
      }

      /* Symbol starting with '+' or '-' */
      if(issymb(next)) {
        token[token_index] = (unsigned char)c;
        token_index++;
        token[token_index] = (unsigned char)next;
        token_index++;
        return readsymbol(token_index, token, tok);
      }

      return -1;
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
    case '(': tok->type = LPAREN;
      tok->atom_name = 0;
      return 0;
    case ')': tok->type = RPAREN;
      tok->atom_name = 0;
      return 0;
    case '"': return readstring(token_index, token, tok);
      return 0; /* never reached */
    case ' ':
      break;
    default: printf("%c\n", c);
      break;
    }

    token_index++;
  }
  return -1;
}

int readsymbol(int index, unsigned char token_string[], token_t *tok) {
  int c;
  
  while((c = fgetc(stdin)) != EOF &&
        index < MAX_TOKEN_LENGTH) {

    if (issymb(c)) {
      token_string[index] = (unsigned char)c;
      index++;
      continue;
    }

    if(isstop_pushback(c)) {
      char *tok_str;

      token_string[index] = '\0';

      tok->type = SYMBOL;
      
      tok_str = (char *)malloc(index);
      if (NULL == tok_str) {
        return -1;
      }
      
      strncpy(tok_str, (const char *)token_string, index + 1);

      tok->string_val = (unsigned char *)tok_str;

      return 0;
    }

    return -1;
  }
  
  return -1;
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

  while((c = fgetc(stdin)) != EOF &&
        index < MAX_TOKEN_LENGTH) {

    if(isstop_pushback(c)) {
      char *end;
      end = (char *)&token_string[index];

      /* TODO: Check for overflow by comparing with original string if
         result is LONG_MAX or INT_MAX. */
      tok->type = NUMBER;
      tok->i_val = strtol((const char *)token_string, &end, 10);

      return 0;
    }

    if (c < '0' || c > '9') {
      return -1;
    }

    token_string[index] = (unsigned char)c;
    index++;
  }

  return -1;
}


int readstring(int index, unsigned char token_string[], token_t *tok) {
  int c;
  token_string[index] = '"';
  
  while((c = fgetc(stdin)) != EOF &&
        index < MAX_TOKEN_LENGTH) {

    if (c < 0 || c > UCHAR_MAX) {
      return -1;
    }

    index++;
    token_string[index] = (unsigned char)c;

    if((unsigned char)c == '"') {
      char *tok_str;

      token_string[index + 1] = '\0';

      tok->type = STRING;
      
      tok_str = (char *)malloc(index + 1);
      if (NULL == tok_str) {
        return -1;
      }
      
      strncpy(tok_str, (const char *)token_string, index + 1);

      tok->string_val = (unsigned char *)tok_str;

      return 0;
    }
  }
  
  return -1;
}

int print_token(const token_t *tok) {
  switch (tok->type) {
  case LPAREN: printf("Token: LPAREN\n");
    return 0;
  case RPAREN: printf("Token: RPAREN\n");
    return 0;
  case STRING: printf("Token: STRING %s\n", tok->string_val);
    return 0;
  case NUMBER: printf("Token: NUMBER %d\n", tok->i_val);
    return 0;
  case SYMBOL: printf("Token: SYMBOL %s\n", tok->atom_name);
    return 0;
  default: printf("Unknown token.n");
    return -1;
  }
}
