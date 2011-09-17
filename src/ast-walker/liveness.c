#include <stdarg.h>
#include <errno.h>

#include "util.h"
#include "memory.h"
#include "cell.h"

typedef struct frame_t {
  struct frame_t *next_frame;
  int nr_live;
  cell_t **live_cells; /* this is an array of 'nr_live' (cell_t *)s */
} frame_t;

#define _LIVENESS_C 1
#include "liveness.h"
#undef _LIVENESS_C

frame_t *live_root;

void print_single_frame(frame_t *frame);

frame_t *new_liveframe(int nr_live, ...) {
  va_list ap;
  cell_t **list, **arr;
  frame_t *frame;
  int i = 0;

  if (nr_live < 1) {
    DEBUGPRINT_("Trying to make a frame_t with < 1 live cell_t:s. Aborting.\n");
    bail(1);
  } 

  list = (cell_t **)malloc_or_bail(sizeof(cell_t *)*nr_live/sizeof(char));
  frame = (frame_t *)malloc_or_bail(sizeof(frame_t)/sizeof(char));
  
  frame->nr_live = nr_live;
  frame->live_cells = list;
  arr = list;

  va_start(ap, nr_live);
  for (i = 0; i < nr_live; i++) {
    cell_t *foo = va_arg(ap, cell_t *);
    if (NULL != foo) {
      *arr = foo;
      arr++;
    } else {
      frame->nr_live--;
    }  
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
  free_malloced((*top)->live_cells);
  t = *top;
  *top = (*top)->next_frame;
  free_malloced(t);
}

void dealloc_frames(frame_t **top) {
  while (*top != NULL) {
    pop_liveness(top);
  }
}

void print_frames(frame_t *first) {
  frame_t *f = first;
  while(f != NULL) {
    print_single_frame(f);
    f=f->next_frame;
  }
}

void print_single_frame(frame_t *f) {
  cell_t **cp = f->live_cells;
  int i = 0;
  while (i < f->nr_live) {
    DEBUGPRINT("Live cell_t at: %p ", *cp);
    pp(*cp);
    printf("\n");
    i++;
    cp++;
  }
}
