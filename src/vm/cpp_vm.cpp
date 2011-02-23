#include "cpp_vm.hpp"

Context::Frame::Frame(int s, Frame *p) :
  prev(p),
  next(0),
  size(s),
  u(0)
{
  ;
}

Context::Context(int initial_frame_size) :
  top(0),
  bottom(0)
{ 
  Frame *t = new Frame(initial_frame_size, 0);
  top = t;
  bottom = t;
}

/* Frame invariants:
 *
 * Either
 * top == bottom == 0
 * or
 * top AND bottom != 0
 *
 * and
 *
 * top->prev == 0
 * bottom->next == 0
 *
 * no other prev/next == 0
 * except if top == bottom then 
 * top->next == bottom->next == 0
 * and
 * bottom->prev == top->prev == 0
 */
Context::Frame * Context::push_new_frame(int size) {
  Frame *f = new Frame(size, bottom); //bottom is 0 if this is first frame

  if (top == 0) {
    top = f;
    bottom = f;
  } else {
    bottom->next = f;
    bottom = f;
  }
  return f;
}

void Context::pop_frame() {
  // Check for underflow/null

  Frame *f = bottom;
  
  if (top == bottom) {
    top = bottom = 0;
  } else {
    bottom = bottom->prev;
    bottom->next = 0;
  }
  delete f;
}

Context::Context() :
  top(0),
  bottom(0)
{ }

Context::~Context() {
  while(bottom != top) {
    Frame *t = bottom->prev;
    delete bottom;
    bottom = t;
  }

  delete top;
}
