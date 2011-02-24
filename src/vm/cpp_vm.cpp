#include "cpp_vm.hpp"

#include <iostream>

/* Frame */
context::frame::frame(int s, frame *p) :
  prev(p),
  next(0),
  size(s),
  top(0),
  u(0)
{
  u = new stack_entry[size];
  for (int i = 0; i < size; i++) {
    *(u+i)=0;
  }
}

std::ostream& operator<<(std::ostream& os, const context::frame& f) {
  os << "Frame at: " << &f << ", size: " << f.size;
  return os;
}

context::frame::~frame() {
  delete [] u; // what happens to each element now?
}

/* Context */
context::context(int initial_frame_size) :
  top(0),
  bottom(0)
{ 
  frame *t = new frame(initial_frame_size, 0);
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
context::frame * context::push_new_frame(int size) {
  frame *f = new frame(size, bottom); //bottom is 0 if this is first frame

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
  if(bottom->top == bottom->size) {
    throw context::frame_overflow("");
    return;
  }
  *(bottom->u + bottom->top) = s;
  bottom->top++;
}

context::stack_entry context::pop_off(bool zero) {
  if (bottom->top < 1) {
    throw context::frame_underflow("");
  }

  bottom->top--;
  stack_entry t = *(bottom->u + bottom->top);
  if(zero) {
    *(bottom->u + bottom->top) = 0;
  }

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
}
