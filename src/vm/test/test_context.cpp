#include "../cpp_vm.hpp"

#include <iostream>

class test_context {
public:
  static void test_frames() {
    context  c;
    context::frame *t;

    std::cout << "===test_frames===" << std::endl;

    c.push_new_frame(0);
    std::cout << "frame 0" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.push_new_frame(1);
    std::cout << "frame 1" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.push_new_frame(2);
    std::cout << "frame 2" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.push_new_frame(3);
    std::cout << "frame 3" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.pop_frame();
    std::cout << "frame 2" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.pop_frame();
    std::cout << "frame 1" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.pop_frame();
    std::cout << "frame 0" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.pop_frame();
    std::cout << "frame NULL" << std::endl;
    std::cout << "top: " << c.top << ", bottom: " << c.bottom << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    try {
      c.pop_frame();
      std::cout << "stack underflow allowed. we shoud not! be here" << std::endl;
    } catch (const vm_error& e) {
      std::cout << "Stack underflow detected which is good!" << std::endl;
    }
  }

  static void test_values() {
    context  c;
    context::frame *t;
    
    std::cout << std::endl << "===test_values==" << std::endl;

    c.push_new_frame(5);
    c.push_on(1);
    c.push_on(2);
    c.push_on(3);

    std::cout << "frame 0" << std::endl;
    t = c.bottom;
    std::cout << *t << std::endl;
    for (int i = 0; i < t->top; i++) {
      std::cout << "i: " << i << ", content: " << (t->u + i) <<
	", value: " << *(t->u + i) << std::endl;
    }
    std::cout << "unused stack:" << std::endl;    
    for (int i = t->top; i < t->size; i++) {
      std::cout << "i: " << i << ", content: " << (t->u + i) <<
	", value: " << *(t->u + i) << std::endl;
    }
    
    c.push_new_frame(10);
    c.push_on(11);
    c.push_on(12);

    std::cout << "frame 1" << std::endl;
    t = c.bottom;
    std::cout << *t << std::endl;
    for (int i = 0; i < t->top; i++) {
      std::cout << "i: " << i << ", content: " << (t->u + i) <<
	", value: " << *(t->u + i) << std::endl;
    }
    std::cout << "unused stack:" << std::endl;
    for (int i = t->top; i < t->size; i++) {
      std::cout << "i: " << i << ", content: " << (t->u + i) <<
	", value: " << *(t->u + i) << std::endl;
    }
    
    c.pop_frame();
    c.pop_off(true);

    std::cout << "frame 0" << std::endl;
    t = c.bottom;
    std::cout << *t << std::endl;
    for (int i = 0; i < t->top; i++) {
      std::cout << "i: " << i << ", content: " << (t->u + i) <<
	", value: " << *(t->u + i) << std::endl;
    }
    std::cout << "unused stack:" << std::endl;    
    for (int i = t->top; i < t->size; i++) {
      std::cout << "i: " << i << ", content: " << (t->u + i) <<
	", value: " << *(t->u + i) << std::endl;
    }

    c.push_on(3);
    c.push_on(4);
    c.push_on(5);
    try {
      c.push_on(6);
      std::cout << "frame overflow allowed. we shoud not! be here" << std::endl;
    } catch (const vm_error& e) { //could use context::frame_overflow
      std::cout << "Frame overflow detected which is good!" << std::endl;
    }

    c.pop_off(true);
    c.pop_off(true);
    c.pop_off(true);
    c.pop_off(true);
    c.pop_off(true);
    try {
      c.pop_off(true);
      std::cout << "frame underflow allowed. we shoud not! be here" << std::endl;
    } catch (const context::frame_underflow& e) { // could use vm_error
      std::cout << "Frame underflow error detected which is good!" << std::endl;
    }

    std::cout << "frame 0" << std::endl;
    t = c.bottom;
    std::cout << *t << std::endl;
    for (int i = 0; i < t->top; i++) {
      std::cout << "i: " << i << ", content: " << (t->u + i) <<
	", value: " << *(t->u + i) << std::endl;
    }
    std::cout << "unused stack:" << std::endl;    
    for (int i = t->top; i < t->size; i++) {
      std::cout << "i: " << i << ", content: " << (t->u + i) <<
	", value: " << *(t->u + i) << std::endl;
    }
  }
};

int main(void) {
  test_context::test_frames();
  test_context::test_values();
}
