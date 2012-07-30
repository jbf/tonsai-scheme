#include "reader.h"
#include "cell.h"
#include "token.h"
#include "symbol.h"
#include "memory.h"
#include "handles.h"

#include <string.h>

extern cell_t *nil_cell;

cell_t *read_list_intern(STREAM *stream, symbol_table *symbol_table);

cell_t *read_intern(STREAM *stream, symbol_table *symbol_table) {
  token_t tok;
  cell_t *cell; 
  int ret;
  size_t slen;
  u8_cell_t* string;
  handle_t *hcell;

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
    cell = intern(tok.atom_name, symbol_table);
    if (NULL == cell) {
      return NULL;
    }
    return cell;
  case TOKEN_NUMBER:
    cell = new(cell_t); // nothing extra live
    if (NULL == cell) {
      return NULL;
    }
    cell->slot1.type = NUMBER;
    cell->slot2.i_val = tok.i_val;
    return cell;
  case TOKEN_STRING:
    /* As with symbols, we need to free or steal the token payload. */
    cell = new(cell_t); // nothing extra live
    if (NULL == cell) {
      return NULL;
    }
    slen = strlen((char *)tok.string_val)+1; /* Add '\0' sigh */
    hcell = handle_push(cell);
    string = u8_new(slen); // cell handled
    cell = handle_get(hcell);
    handle_pop(hcell);
    strncpy((char*)U8DATA(string), (char*)tok.string_val, slen);
    cell->slot1.type = STRING;
    cell->slot2.string = (cell_t *)string;
    free_token_payload(&tok);
    return cell;
  default:
    return NULL;
  }
}

cell_t *read_list_intern(STREAM *stream, symbol_table *symbol_table) {
  cell_t *last, *current, *first, *temp;
  handle_t *hlast, *hcurrent, *hfirst, *htemp;
  token_t tok;
  int ret;
  size_t slen;
  u8_cell_t* string;

  last = current = first = temp = NULL;

#define LOCAL_POP_HANDLES()                      \
  do {                                           \
    handle_pop(hlast);                           \
    handle_pop(htemp);                           \
    handle_pop(hfirst);                          \
    handle_pop(hcurrent);                        \
  } while (0)                                    \

  hcurrent = handle_push(current);
  hfirst = handle_push(first);
  htemp = handle_push(temp);
  hlast = handle_push(last);

  do {
    ret = get_token(stream, &tok);
    if (ret != TOKEN_OK) {
      LOCAL_POP_HANDLES();
      return NULL;
    }

    // special case nil
    if(tok.type == TOKEN_RPAREN) {
      current = nil_cell;
    } else {
      handle_set(hfirst, first);
      handle_set(htemp, temp);
      handle_set(hlast, last);
      current = new(cell_t); // all protected
      last = handle_get(hlast);
      temp = handle_get(htemp);
      first = handle_get(hfirst);

      current->slot2.cdr = nil_cell; 
    }

    /* TODO */
    /* Will leak all read token payloads' memory in OOM situation. */
    if (!current) {
      LOCAL_POP_HANDLES();
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
      LOCAL_POP_HANDLES();
      return first;
    case TOKEN_SYMBOL:
      /* We also need to free or steal the payload part. */
      temp = intern(tok.atom_name, symbol_table); // UNSAFE??
      if (!temp) {
        LOCAL_POP_HANDLES();
        return NULL;
      }
      current->slot1.car = temp;
      break;
    case TOKEN_NUMBER:
      handle_set(hcurrent, current);
      handle_set(hfirst, first);
      handle_set(hlast, last);
      temp = new(cell_t); // first, last, current handled
      last = handle_get(hlast);
      first = handle_get(hfirst);
      current = handle_get(hcurrent);

      if (!temp) {
        LOCAL_POP_HANDLES();      
        return NULL;
      }
      current->slot1.car = temp;
      temp->slot1.type = NUMBER;
      temp->slot2.i_val = tok.i_val;
      break;
    case TOKEN_STRING:
      /* As with symbols, we need to free or steal the token payload. */
      slen = strlen((char *)tok.string_val)+1; /* Add '\0' sigh */

      handle_set(hcurrent, current);
      handle_set(hfirst, first);
      handle_set(hlast, last);
      temp = new(cell_t); // first, last, current handled
      
      handle_set(htemp, temp);
      string = u8_new(slen); // first, temp, last, current handled
      temp = handle_get(htemp);

      last = handle_get(hlast);
      first = handle_get(hfirst);
      current = handle_get(hcurrent);

      if (!temp) {
        LOCAL_POP_HANDLES();
        return NULL;
      }

      strncpy((char*)U8DATA(string), (char*)tok.string_val, slen);
      current->slot1.car = temp;
      temp->slot1.type = STRING;
      temp->slot2.string = (cell_t *)string;
      free_token_payload(&tok);
      break;
    default:
      LOCAL_POP_HANDLES();
      return NULL;
    }

    last = current;
  } while (1);

  LOCAL_POP_HANDLES();
  return first;
}
