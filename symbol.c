#include "symbol.h"
#include "util.h"

#include <string.h>

symbol_entry_t *lookup(unsigned char *sym, symtab_entry_t *tab) {
  if (NULL == tab) {
    return NULL; /* sym not present. */
  } else if (strncmp((const char *)sym, (const char*)tab->symbol->symbol_name, MAX_SYMBOL_UNIQUENESS) == 0) {
    return tab->symbol;
  } else {
    return lookup(sym, tab->next);
  }
}

symbol_entry_t *intern2(unsigned char *sym, symtab_entry_t **tab, symtab_entry_t *orig_head) {
  symbol_entry_t *existing;
  existing = lookup(sym, *tab);
  
  if (NULL == existing) {
    symtab_entry_t *new_tab;
    symbol_entry_t *new_sym;
    
    new_tab = new(symtab_entry_t);
    
    if (NULL == new_tab) {
      return 0; /* Out of memory. */
    }
    
    new_sym = new(symbol_entry_t);

    if (NULL == new_sym) {
      free(new_tab);
      return 0; /* Out of memory. */
    }

    new_sym->symbol_name = sym; /* Keep old string. */
    new_sym->symbol_value = NULL;

    new_tab->symbol = new_sym;
    new_tab->next = orig_head;

    *tab = new_tab;

    return new_sym;
  } else {
    return existing;
  }
}
