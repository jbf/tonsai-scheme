#include "symbol.h"
#include "memory.h"
#include "cell.h"
#include "bootstrap.h"

#include <string.h>

typedef struct symtab_entry_t {
  struct symbol_entry_t *symbol;
  struct symtab_entry_t *next;
} symtab_entry_t;


cell_t *_lookup(unsigned char *sym, symtab_entry_t *tab);
int push(symbol_table *tab, symbol_entry_t *s);

cell_t *lookup(unsigned char *sym, symbol_table *tab) {
  return (NULL == tab ? NULL : _lookup(sym, tab->head));
}

cell_t *_lookup(unsigned char *sym, symtab_entry_t *tab) {
  if (NULL == tab || NULL == tab->symbol) {
    return NULL; /* sym not present. */
  } else if (strncmp((const char *)sym, (const char*)SYMBOL_NAME(tab->symbol), MAX_SYMBOL_UNIQUENESS) == 0) {
    return SYMBOL_CELL(tab->symbol);
  } else {
    return _lookup(sym, tab->next);
  }
}

cell_t *intern(unsigned char *sym, symbol_table *tab) {
  cell_t *existing;
  existing = lookup(sym, tab);

  if (NULL == tab) return NULL;
  
  if (NULL == existing) {
    symtab_entry_t *new_tab;
    symbol_entry_t *new_sym;
    cell_t *new_cell;
   
    new_tab = new_malloc(symtab_entry_t);
    new_sym = new_malloc(symbol_entry_t);
    new_cell = new(cell_t); // nothing extra live at call to new
    
    if (NULL == new_tab || NULL == new_sym || NULL == new_cell) {
      free_malloced(new_tab);
      free_malloced(new_sym);
      /* new_cell is on-heap, gc:d */
      return 0; /* Out of memory. */
    }

    new_cell->slot1.type = SYMBOL;
    new_cell->slot2.symbol = new_sym;

    new_sym->symbol_name = sym; /* Keep old string. */
    new_sym->symbol_cell = new_cell;

    new_tab->symbol = new_sym;
    new_tab->next = tab->head;

    tab->head = new_tab;

    return new_cell;
  } else {
    free_malloced(sym);
    return existing;
  }
}

int init_symbol_table(symbol_table *tab) {
  if (NULL == tab) return 0;

  tab->head = NULL;
  return SYMTAB_CREATED;
}

int push(symbol_table *tab, symbol_entry_t *s) {
  symtab_entry_t *t = new_malloc(symtab_entry_t);
  t->next = tab->head;
  t->symbol = s;
  tab->head = t;

  return 1;
}

void free_symtab(symbol_table *t) {
  symtab_entry_t *n, *e = t->head;
  symbol_entry_t *s;
  while(e != NULL) {
    n = e->next;
    s = e->symbol;

    if (freeable_symbol(s)) {
      /* 1) Free symbol_entry_t (sym part) */
      free_malloced(s->symbol_name);
      /* 2) Free symbol_entry_t */
      free_malloced(s);
    }

    /* 3) in all cases, free symtab_entry_t */
    free_malloced(e);
    e = n;
  }
}

/* Iteration */
void init_symtab_iterator(symtab_iterator_t *iter, symbol_table *tab) {
  iter->current = tab->head;
}
int symtab_iter_has_next(symtab_iterator_t *iter) {
  assert(iter != NULL);
  return iter->current != NULL;
}

struct cell_t *symtab_iter_next_sym(symtab_iterator_t *iter) {
  assert(symtab_iter_has_next(iter));
  struct cell_t *ret = iter->current->symbol->symbol_cell;
  iter->current = iter->current->next;
  return ret;
}
