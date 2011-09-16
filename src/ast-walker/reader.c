#include "reader.h"
#include "cell.h"
#include "token.h"
#include "symbol.h"
#include "memory.h"
#include "liveness.h"

#include <string.h>

extern frame_t *live_root;
extern cell_t *nil_cell;

cell_t *read_list_intern(STREAM *stream, symbol_table *symbol_table);

cell_t *read_intern(STREAM *stream, symbol_table *symbol_table) {
  token_t tok;
  cell_t *cell; 
  int ret;
  size_t slen;
  u8_cell_t* string;

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
    cell->slot1.type = PAYLOAD_NUMBER;
    cell->slot2.i_val = tok.i_val;
    return cell;
  case TOKEN_STRING:
    /* As with symbols, we need to free or steal the token payload. */
    cell = new(cell_t); // nothing extra live
    if (NULL == cell) {
      return NULL;
    }
    slen = strlen((char *)tok.string_val)+1; /* Add '\0' sigh */
    string = u8_new(slen);
    strncpy((char*)U8DATA(string), (char*)tok.string_val, slen);
    cell->slot1.type = PAYLOAD_STRING;
    cell->slot2.string = (cell_t *)string;
    free(tok.string_val);
    return cell;
  default:
    return NULL;
  }
}

cell_t *read_list_intern(STREAM *stream, symbol_table *symbol_table) {
  cell_t *last, *current, *first, *temp;
  token_t tok;
  int ret;
  size_t slen;
  u8_cell_t* string;

  last = current = first = temp = NULL;

  do {
    ret = get_token(stream, &tok);
    if (ret != TOKEN_OK) {
      if (first) { // if first, this is the second time in the loop,
                   // pop the liveframe on error
        pop_liveness(&live_root);
      }
      return NULL;
    }

    // special case nil
    if(tok.type == TOKEN_RPAREN) {
      current = nil_cell;
    } else {
      current = new(cell_t); // liveness tracked through 'first'
      current->slot2.cdr = nil_cell; /* pretend this is a proper list so we can
                                        print liveness */
    }

    /* Will leak all read token payloads' memory in OOM situation. */
    if (!current) {
      if (first) { // if first, this is the second time in the loop,
                   // pop the liveframe on error
        pop_liveness(&live_root); 
      }
      return NULL;
    }

    if (last) {
      last->slot2.cdr = current;
    } else { /* This is the first in the list. */
      first = current;
      push_liveness(&live_root, new_liveframe(1, first));
    }

    /* All liveness tracked through first */
    switch(tok.type) {
    case TOKEN_LPAREN:
      current->slot1.car = read_list_intern(stream, symbol_table);
      break;
    case TOKEN_RPAREN:
      // see special case above
      pop_liveness(&live_root);
      return first;
    case TOKEN_SYMBOL:
      /* We also need to free or steal the payload part. */
      temp = intern(tok.atom_name, symbol_table);
      if (!temp) {
        if (first) {  // if first, this is the second time in the loop,
                      // pop the liveframe on error
          pop_liveness(&live_root); 
        }
        return NULL;
      }
      current->slot1.car = temp;
      break;
    case TOKEN_NUMBER:
      temp = new(cell_t); // liveness ok
      if (!temp) {
        if (first) {  // if first, this is the second time in the loop,
                      // pop the liveframe on error
          pop_liveness(&live_root); 
        }
        return NULL;
      }
      current->slot1.car = temp;
      temp->slot1.type = PAYLOAD_NUMBER;
      temp->slot2.i_val = tok.i_val;
      break;
    case TOKEN_STRING:
      /* As with symbols, we need to free or steal the token payload. */
      temp = new(cell_t); // liveness ok
      if (!temp) {
        if (first) {
          pop_liveness(&live_root); 
        }
        return NULL;
      }
      slen = strlen((char *)tok.string_val)+1; /* Add '\0' sigh */
      string = u8_new(slen);
      strncpy((char*)U8DATA(string), (char*)tok.string_val, slen);
      current->slot1.car = temp;
      temp->slot1.type = PAYLOAD_STRING;
      temp->slot2.string = (cell_t *)string;
      free(tok.string_val);
      break;
    default:
      if (first) { // if first, this is the second time in the loop,
                   // pop the liveframe on error
        pop_liveness(&live_root); 
      }
      return NULL;
    }

    last = current;
  } while (1);

  if (first) {
    pop_liveness(&live_root); 
  }
  return first;
}
