#ifndef _READER_H
#define _READER_H 1

#include "cell.h"
#include "symbol.h"

#include <stdio.h>

cell_t *read_intern(FILE *stream, symbol_table *symbol_table);
cell_t *read_list_intern(FILE *stream, symbol_table *symbol_table);

#endif /* _READER_H */
