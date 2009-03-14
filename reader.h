#ifndef _READER_H
#define _READER_H 1

#include "cell.h"
#include "symbol.h"

cell_t *read_intern(symtab_entry_t **symbol_table);
cell_t *read_list_intern(symtab_entry_t **symbol_table);

#endif /* _READER_H */
