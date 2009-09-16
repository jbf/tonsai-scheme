/*
 * symbol.h and symbol.c contains the code for a symbol table.
 *
 * intern :: "string", **symtab_entry_t -> *symbol_entry_t
 *
 * intern will modify the symtab_entry_t, this is part of adding the symbol
 * to the table.
 *
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
  struct cell_t *symbol_value;
} symbol_entry_t;

typedef struct symtab_entry_t {
  struct symbol_entry_t *symbol;
  struct symtab_entry_t *next;
} symtab_entry_t;

#ifndef _SYMBOL_C
extern struct symbol_entry_t nil;
extern struct cell_t nil_cell;
#endif /* _SYMBOL_C */

/* Accessors. */
#define SYMBOL_NAME(s) ((s)->symbol_name)
#define SYMBOL_VALUE(s) ((s)->symbol_value)

symbol_entry_t *intern(unsigned char *sym, symtab_entry_t **tab);
symbol_entry_t *lookup(unsigned char *sym, symtab_entry_t *tab);
int create_initial_symtab(symtab_entry_t **tab);

/* need free_symtab() */
#define free_symtab(t)

#define MAX_SYMBOL_UNIQUENESS MAX_TOKEN_LENGTH
#define SYMTAB_CREATED 1

#endif /* _SYMBOL_H */
