#include "cpp_vm.hpp"

#include <iostream>

/* Frame */
context::frame::frame(context::context *prnt, frame *p, int s) :
  parent(prnt),
  prev(p),
  next(0),
  size(s),
  sp(s),
  u(0)
{
  u = new stack_entry[size];
  for (int i = 0; i < size; i++) {
    *(u+i)=0;
  }
}

std::ostream& operator<<(std::ostream& os, const context::frame& f) {
  os << "Frame at: " << &f 
     << ", size: " << f.size 
     << ", occupied: " << (f.size - f.sp);
  return os;
}

context::frame::~frame() {
  delete [] u; // what happens to each element now?
  size = sp = 0;
  parent = 0;
  prev = next = 0;
  u = 0;
}

/* Context */
context::context(int initial_frame_size) :
  top(0),
  bottom(0)
{ 
  frame *t = new frame(this, 0, initial_frame_size);
  top = t;
  bottom = t;
}

/* Context invariants:
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
context::frame * context::push_new_frame(int size) {
  frame *f = new frame(this, bottom, size); //bottom is 0 if this is first frame

  if (top == 0) {
    top = f;
    bottom = f;
  } else {
    bottom->next = f;
    bottom = f;
  }
  return f;
}

void context::pop_frame() {
  if (bottom == 0) {
    throw vm_error(); //pop one frame to manny, ey?
  }

  frame *f = bottom;
  
  if (top == bottom) {
    top = bottom = 0;
  } else {
    bottom = bottom->prev;
    bottom->next = 0;
  }
  delete f;
}

void context::push_on(stack_entry s) {
  if (bottom == 0) {
    throw vm_error(); //stack is not initialized, we have no frame
  }
  if(bottom->sp < 1) {
    throw context::frame_overflow("");
  }
  *(bottom->u + --bottom->sp) = s;
}

context::stack_entry context::pop_off(bool zero) {
  if (bottom->sp == bottom->size) {
    throw context::frame_underflow("");
  }

  stack_entry t = *(bottom->u + bottom->sp);
  if(zero) {
    *(bottom->u + bottom->sp) = 0;
  }
  bottom->sp++;
  return t;
}

context::context() :
  top(0),
  bottom(0)
{ }

context::~context() {
  while(bottom != top) {
    frame *t = bottom->prev;
    delete bottom;
    bottom = t;
  }

  delete top;
  top = bottom = 0;
}
