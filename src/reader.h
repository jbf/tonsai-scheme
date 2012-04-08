#ifndef _READER_H
#define _READER_H 1

#include "cell.h"
#include "symbol.h"
#include "t_stream.h"

cell_t *read_intern(STREAM *stream, symbol_table *symbol_table);

#endif /* _READER_H */
