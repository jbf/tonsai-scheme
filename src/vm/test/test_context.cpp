#include "../cpp_vm.hpp"

#include <iostream>

class TestContext {
public:
  static void test_frames() {
    Context  c;
    Context::Frame *t;

    std::cout << "===test_frames===" << std::endl;

    c.push_new_frame(0);
    std::cout << "Frame 0" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.push_new_frame(1);
    std::cout << "Frame 1" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.push_new_frame(2);
    std::cout << "Frame 2" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.push_new_frame(3);
    std::cout << "Frame 3" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.pop_frame();
    std::cout << "Frame 2" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.pop_frame();
    std::cout << "Frame 1" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.pop_frame();
    std::cout << "Frame 0" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }

    c.pop_frame();
    std::cout << "Frame NULL" << std::endl;
    std::cout << "top: " << c.top << ", bottom: " << c.bottom << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << *t << std::endl;
    }
  }

  static void test_values() {
    Context  c;
    Context::Frame *t;
    
    std::cout << std::endl << "===test_values==" << std::endl;

    c.push_new_frame(5);
    c.push_on(1);
    c.push_on(2);
    c.push_on(3);

    std::cout << "Frame 0" << std::endl;
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

    std::cout << "Frame 1" << std::endl;
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

    std::cout << "Frame 0" << std::endl;
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
  TestContext::test_frames();
  TestContext::test_values();
}
