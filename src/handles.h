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

/* Iteration */
typedef struct handle_iterator {
  struct handle_t *current;
} handle_iterator_t;

void init_handle_iterator(handle_iterator_t *iter);
int handle_iter_has_next(handle_iterator_t *iter);
struct handle_t *handle_iter_next_handle(handle_iterator_t *iter);

#endif /* _HANDLES_H */
