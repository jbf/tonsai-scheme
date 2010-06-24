#ifndef _LIVENESS_H
#define _LIVENESS_H 1

#include <stdlib.h>

#ifndef _LIVENESS_C
typedef struct frame_t frame_t;
#endif /* _LIVENESS_C */

frame_t *new_liveframe(int nr_live, ...);
void push_liveness(frame_t **top, frame_t *new_top);
void pop_liveness(frame_t **top);
void dealloc_frames(frame_t **top);

void print_frames(frame_t *first);

#endif /* _LIVENESS_H */
