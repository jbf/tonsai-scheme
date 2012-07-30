#ifndef _HANDLES_H
#define _HANDLES_H 1

#include "cell.h"

/* Don't forget to make longjump on error clear handles */

typedef struct handle_t {
  cell_t *content;
} handle_t;

void init_handles();
handle_t *handle_push(cell_t *protected);
void handle_pop(handle_t *handle);
cell_t *handle_get(handle_t *handle);
cell_t *handle_set(handle_t *handle, cell_t *value);
void destroy_handles();

handle_t *get_mark();
void pop_to_mark(handle_t *handle);

#endif /* _HANDLES_H */
