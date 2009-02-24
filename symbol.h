#ifndef _SYMBOL_H
#define _SYMBOL_H 1

#include "cell.h"
#include "token.h"

typedef struct symbol_entry_t {
  unsigned char *symbol_name;
  struct cell_t *symbol_value;
} symbol_entry_t;

typedef struct symtab_entry_t {
  struct symbol_entry_t *symbol;
  struct symtab_entry_t *next;
} symtab_entry_t;

symbol_entry_t *intern2(unsigned char *sym, symtab_entry_t **tab, symtab_entry_t *orig_head);
symbol_entry_t *lookup(unsigned char *sym, symtab_entry_t *tab);

#define alloc_symbol_entry() (((symbol_entry_t *)malloc(sizeof(symbol_entry_t)/sizeof(char))))
#define alloc_symtab_entry() (((symtab_entry_t *)malloc(sizeof(symtab_entry_t)/sizeof(char))))

#define MAX_SYMBOL_UNIQUENESS MAX_TOKEN_LENGTH

#endif /* _SYMBOL_H */
