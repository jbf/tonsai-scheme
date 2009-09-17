#define _SYMBOL_C 1
#include "symbol.h"
#undef _SYMBOL_C
#include "util.h"
#include "error.h"
#include "cell.h"

#include <string.h>

symbol_entry_t nil = { (unsigned char *)"NIL", (void *)0};
cell_t nil_cell = {{.type = PAYLOAD_NIL}, {.symbol = &nil}};

/* 
 * This is the recursive function doing the actual work. Internal only.
 */
symbol_entry_t *intern2(unsigned char *sym, symtab_entry_t **tab, symtab_entry_t *orig_head);

symbol_entry_t *_lookup(unsigned char *sym, symtab_entry_t *tab);

cell_t *lookup(unsigned char *sym, symtab_entry_t *tab) {
  symbol_entry_t *s = _lookup(sym, tab);
  return (NULL == s ? NULL : s->symbol_cell);
}

/*
 * This visible function is just a wrapper around intern2.
 */
cell_t *intern(unsigned char *sym, symtab_entry_t **tab) {
  return intern2(sym, tab, *tab)->symbol_cell;
}

symbol_entry_t *_lookup(unsigned char *sym, symtab_entry_t *tab) {
  if (NULL == tab) {
    return NULL; /* sym not present. */
  } else if (strncmp((const char *)sym, (const char*)tab->symbol->symbol_name, MAX_SYMBOL_UNIQUENESS) == 0) {
    return tab->symbol;
  } else {
    return _lookup(sym, tab->next);
  }
}

symbol_entry_t *intern2(unsigned char *sym, symtab_entry_t **tab, symtab_entry_t *orig_head) {
  symbol_entry_t *existing;
  existing = _lookup(sym, *tab);
  
  if (NULL == existing) {
    symtab_entry_t *new_tab;
    symbol_entry_t *new_sym;
    cell_t *new_cell;
   
    new_tab = new(symtab_entry_t);
    new_sym = new(symbol_entry_t);
    new_cell = new(cell_t);
    
    if (NULL == new_tab || NULL == new_sym || NULL == new_cell) {
      free(new_tab);
      free(new_sym);
      free(new_cell);
      return 0; /* Out of memory. */
    }

    new_cell->slot1.type = PAYLOAD_SYMBOL;
    new_cell->slot2.symbol = new_sym;

    new_sym->symbol_name = sym; /* Keep old string. */
    new_sym->symbol_cell = new_cell;

    new_tab->symbol = new_sym;
    new_tab->next = orig_head;

    *tab = new_tab;

    return new_sym;
  } else {
    return existing;
  }
}

/* 
 * Create an intial symtab containing 'nil'. The returned symtab is ready
 * for interning into.
 */
int create_initial_symtab(symtab_entry_t **tab) {
  symtab_entry_t *symtab;

  if (nil.symbol_cell == NULL ) {
    nil.symbol_cell = &nil_cell;
  }

  symtab = new(symtab_entry_t);
  if(NULL == symtab) {
    return EOOM;
  }

  symtab->next = NULL;
  symtab->symbol = &nil;

  *tab = symtab;

  return SYMTAB_CREATED;
}
