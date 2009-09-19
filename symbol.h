/*
 * symbol.h and symbol.c contains the code for a symbol table.
 *
 * intern will modify the symtab_entry_t, this is part of adding the symbol
 * to the table.
 *
 * Note that intern will not make a copy of the string, that is the
 * responsibility of the tokenizer (intern will steal the payload).
 */

#ifndef _SYMBOL_H
#define _SYMBOL_H 1

#include "cell.h"
#include "token.h"

typedef struct symbol_entry_t {
  unsigned char *symbol_name;
  struct cell_t *symbol_cell;
} symbol_entry_t;

typedef struct symbol_table {
  struct symtab_entry_t *head;
} symbol_table;

/* Accessors. */
#define SYMBOL_NAME(s) ((s)->symbol_name)
#define SYMBOL_CELL(s) ((s)->symbol_cell)

struct cell_t *intern(unsigned char *sym, symbol_table *tab);
struct cell_t *lookup(unsigned char *sym, symbol_table *tab);
int init_symbol_table(symbol_table *tab);

/* need free_symtab() */
#define free_symtab(t)

#define MAX_SYMBOL_UNIQUENESS MAX_TOKEN_LENGTH
#define SYMTAB_CREATED 1

#endif /* _SYMBOL_H */
