#include <stdarg.h>
#include <errno.h>

#include "util.h"
#include "memory.h"

typedef struct frame_t {
  struct frame_t *next_frame;
  int nr_live;
  struct cell_t **live_cells; /* this is an array of 'nr_live' (cell_t *)s */
} frame_t;

#define _LIVENESS_C 1
#include "liveness.h"
#undef _LIVENESS_C

frame_t *live_root;

frame_t *new_liveframe(int nr_live, ...) {
  va_list ap;
  struct cell_t **list;
  frame_t *frame;

  if (nr_live < 1) {
    DEBUGPRINT_("Trying to make a frame_t with < 1 live cell_t:s. Aborting.\n");
    bail(1);
  } 

  list = (struct cell_t **)malloc_or_bail(sizeof(struct cell_t *)*nr_live/sizeof(char));
  frame = (frame_t *)malloc_or_bail(sizeof(frame_t)/sizeof(char));
  
  frame->nr_live = nr_live;
  frame->live_cells = list;

  va_start(ap, nr_live);
  for (; nr_live > 0; nr_live--) {
    list = va_arg(ap, void *);
    list++;
  }
  va_end(ap);

  return frame;
}

void push_liveness(frame_t **top, frame_t *new_top) {
  new_top->next_frame=*top;
  *top=new_top;
}

void pop_liveness(frame_t **top) {
  frame_t *t;
  free((*top)->live_cells);
  t = *top;
  *top = (*top)->next_frame;
  free(t);
}

void dealloc_frames(frame_t **top) {
  while (*top != NULL) {
    pop_liveness(top);
  }
}
