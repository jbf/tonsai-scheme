#include <stdio.h>
#include <sys/types.h>

#include "handles.h"

#include "memory.h"
#include "util.h"

static handle_t *bottom;
static handle_t *top;
static size_t handle_frame_size;

void init_handles() {
  size_t initial_handles_bytes = 1024;
  bottom = top = (handle_t *)malloc_or_bail(initial_handles_bytes);
  handle_frame_size = initial_handles_bytes / sizeof(handle_t);
}

void destroy_handles() {
  DEBUGPRINT("HANDLES: BOTTOM: %p, TOP: %p, DIFF: %lu\n", bottom, top, top - bottom);
}

handle_t *handle_push(cell_t *protected) {
  if (top + 1 > bottom + handle_frame_size) {
    printf("Out of handles!\n");
    fflush(NULL);
    exit(1);
  }

  top++;
  top->content = protected;

  return top;
}

cell_t *handle_get(handle_t *handle) {
  return handle->content;
}

cell_t *handle_set(handle_t *handle, cell_t *value) {
  handle->content = value;
  return value;
}

void handle_pop(handle_t *handle) {
  if (handle != top) {
    printf("Freeing: %p, Top is: %p, Bottom: %p\n", handle, top, bottom);
  }
  assert(handle == top);
  handle->content = NULL;
  top--;
}

handle_t *get_mark() {
  return top;
}

void pop_to_mark(handle_t *mark) {
  while (top > mark) {
    top->content = NULL;
    top--;
  }
}
