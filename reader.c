#include "reader.h"
#include "cell.h"
#include "token.h"
#include "symbol.h"
#include "util.h"

cell_t *read_intern(symtab_entry_t **symbol_table) {
  token_t tok;
  cell_t *cell; 
  int ret;

  ret = get_token(&tok);
  if (ret != TOKEN_OK) {
    return NULL;
  }

  cell = new(cell_t);
  if (NULL == cell) {
    return NULL;
  }

  switch(tok.type) {
  case TOKEN_LPAREN:
    free(cell);
    return read_list_intern(symbol_table);
  case TOKEN_RPAREN:
    free(cell);
    return NULL;
  case TOKEN_SYMBOL:
    /* We also need to free or steal the payload part. */
    cell->type = PAYLOAD_SYMBOL;
    cell->symbol = intern2(tok.atom_name, symbol_table, *symbol_table);
    return cell;
  case TOKEN_NUMBER:
    cell->type = PAYLOAD_NUMBER;
    cell->i_val = tok.i_val;
    return cell;
  case TOKEN_STRING:
    /* As with symbols, we need to free or steal the token payload. */
    cell->type = PAYLOAD_STRING;
    cell->string = tok.string_val;
    return cell;
  default:
    free(cell);
    return NULL;
  }
}

cell_t *read_list_intern(symtab_entry_t **symbol_table) {
  cell_t *last, *current, *first, *temp;
  token_t tok;
  int ret;

  last = current = first = temp = NULL;

  do {
    ret = get_token(&tok);
    if (ret != TOKEN_OK) {
      return NULL;
    }

    current = new(cell_t);

    /* Will leak all read token payloads' memory in OOM situation. */
    if (!current) {
      return NULL;
    }

    if (last) {
      last->cdr = current;
    } else { /* This is the first in the list. */
      first = current;
    }

    switch(tok.type) {
    case TOKEN_LPAREN:
      current->car = read_list_intern(symbol_table);
      break;
    case TOKEN_RPAREN:
      current->type = PAYLOAD_NIL;
      current->cdr = NULL;
      return first;
    case TOKEN_SYMBOL:
      /* We also need to free or steal the payload part. */
      temp = new(cell_t);
      if (!temp) {
        return NULL;
      }
      current->car = temp;
      temp->type = PAYLOAD_SYMBOL;
      temp->symbol = intern2(tok.atom_name, symbol_table, *symbol_table);
      break;
    case TOKEN_NUMBER:
      temp = new(cell_t);
      if (!temp) {
        return NULL;
      }
      current->car = temp;
      temp->type = PAYLOAD_NUMBER;
      temp->i_val = tok.i_val;
      break;
    case TOKEN_STRING:
      /* As with symbols, we need to free or steal the token payload. */
      temp = new(cell_t);
      if (!temp) {
        return NULL;
      }
      current->car = temp;
      temp->type = PAYLOAD_STRING;
      temp->string = tok.string_val;
      break;
    default:
      return NULL;
    }

    last = current;
  } while (1);

  return first;
}
