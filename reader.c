#include "reader.h"
#include "cell.h"
#include "token.h"
#include "symbol.h"
#include "util.h"

cell_t *read_intern(FILE *stream, symtab_entry_t **symbol_table) {
  token_t tok;
  cell_t *cell; 
  int ret;

  ret = get_token(stream, &tok);
  if (ret != TOKEN_OK) {
    return NULL;
  }


  switch(tok.type) {
  case TOKEN_LPAREN:
    return read_list_intern(stream, symbol_table);
  case TOKEN_RPAREN:
    return NULL;
  case TOKEN_SYMBOL:
    /* We also need to free or steal the payload part. */
    cell = new(cell_t);
    if (NULL == cell) {
      return NULL;
    }
    cell->slot1.type = PAYLOAD_SYMBOL;
    cell->slot2.symbol = intern(tok.atom_name, symbol_table);
    return cell;
  case TOKEN_NUMBER:
    cell = new(cell_t);
    if (NULL == cell) {
      return NULL;
    }
    cell->slot1.type = PAYLOAD_NUMBER;
    cell->slot2.i_val = tok.i_val;
    return cell;
  case TOKEN_STRING:
    /* As with symbols, we need to free or steal the token payload. */
    cell = new(cell_t);
    if (NULL == cell) {
      return NULL;
    }
    cell->slot1.type = PAYLOAD_STRING;
    cell->slot2.string = tok.string_val;
    return cell;
  default:
    return NULL;
  }
}

cell_t *read_list_intern(FILE *stream, symtab_entry_t **symbol_table) {
  cell_t *last, *current, *first, *temp;
  token_t tok;
  int ret;

  last = current = first = temp = NULL;

  do {
    ret = get_token(stream, &tok);
    if (ret != TOKEN_OK) {
      return NULL;
    }

    current = new(cell_t);

    /* Will leak all read token payloads' memory in OOM situation. */
    if (!current) {
      return NULL;
    }

    if (last) {
      last->slot2.cdr = current;
    } else { /* This is the first in the list. */
      first = current;
    }

    switch(tok.type) {
    case TOKEN_LPAREN:
      current->slot1.car = read_list_intern(stream, symbol_table);
      break;
    case TOKEN_RPAREN:
      current->slot1.type = PAYLOAD_NIL;
      current->slot2.cdr = NULL;
      return first;
    case TOKEN_SYMBOL:
      /* We also need to free or steal the payload part. */
      temp = new(cell_t);
      if (!temp) {
        return NULL;
      }
      current->slot1.car = temp;
      temp->slot1.type = PAYLOAD_SYMBOL;
      temp->slot2.symbol = intern(tok.atom_name, symbol_table);
      break;
    case TOKEN_NUMBER:
      temp = new(cell_t);
      if (!temp) {
        return NULL;
      }
      current->slot1.car = temp;
      temp->slot1.type = PAYLOAD_NUMBER;
      temp->slot2.i_val = tok.i_val;
      break;
    case TOKEN_STRING:
      /* As with symbols, we need to free or steal the token payload. */
      temp = new(cell_t);
      if (!temp) {
        return NULL;
      }
      current->slot1.car = temp;
      temp->slot1.type = PAYLOAD_STRING;
      temp->slot2.string = tok.string_val;
      break;
    default:
      return NULL;
    }

    last = current;
  } while (1);

  return first;
}
