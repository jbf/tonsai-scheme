#include "../cpp_vm.hpp"

#include <iostream>

class test_context {
public:
  static void test_frames() {
    context  c;
    context::frame *t;
    int error = 0;

    std::cout << "===test_frames===" << std::endl;

    c.push_new_frame(0);
    try {
      c.push_on(1);
      throw new vm_error();
    } catch (const context::frame_overflow& e) {
      ;
    }
    c.push_new_frame(2);
    c.push_on(22);
    c.push_on(33);

    int j = c.pop_off();
    int k = c.pop_off();
    
    if (j != 33 || k != 22) {
      throw new vm_error();
    }

    try {
      j = c.pop_off();
      error = 1;
    } catch (const context::frame_underflow& e) {
      ;
    }

    c.pop_frame();
    c.pop_frame();

    try {
      c.pop_frame();
      std::cout << "stack underflow allowed. we shoud not! be here" << std::endl;
      error = 1;
    } catch (const vm_error& e) {
      ;
    }
    if (error) {
      throw new vm_error();
    }
  }

  static void test_values() {
    context  c;
    context::frame *t;
    int i;

    std::cout << "===test_values==" << std::endl;

    c.push_new_frame(5);
    c.push_on(1);
    c.push_on(2);
    c.push_on(3);

    t = c.bottom;

    std::cout << *t << std::endl;
    if (t->size != 5) throw new vm_error();
    if (t->size - t->sp != 3) throw new vm_error();
    
    c.push_new_frame(10);
    c.push_on(11);
    c.push_on(12);

    t = c.bottom;

    std::cout << *t << std::endl;
    if (t->size != 10) throw new vm_error();
    if (t->size - t->sp != 2) throw new vm_error();

    c.pop_frame();
    c.pop_off(true);

    t = c.bottom;

    std::cout << *t << std::endl;
    if (t->size != 5) throw new vm_error();
    if (t->size - t->sp != 2) throw new vm_error();

    c.push_on(3);
    c.push_on(4);
    c.push_on(5);

    int error = 0;
    try {
      c.push_on(6);
      error = 1;
    } catch (const context::frame_overflow& e) { //could use context::frame_overflow
      ;
    }
    if (error) throw new vm_error();

    if (t->size != 5) throw new vm_error();
    if (t->size - t->sp != 5) throw new vm_error();

    c.pop_off(true);
    c.pop_off(true);
    c.pop_off(true);
    c.pop_off(true);
    c.pop_off(true);
    try {
      c.pop_off(true);
      std::cout << "frame underflow allowed. we shoud not! be here" << std::endl;
    } catch (const context::frame_underflow& e) { // could use vm_error
      ;
    }

    t = c.bottom;
    if (t->size != 5) throw new vm_error();
    if (t->size - t->sp != 0) throw new vm_error();

    std::cout << *t << std::endl;
  }
};

int main(void) {
  test_context::test_frames();
  test_context::test_values();
  std::cout << "All ok" << std::endl;
}
