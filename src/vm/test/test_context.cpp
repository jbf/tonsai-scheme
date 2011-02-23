#include "../cpp_vm.hpp"

#include <iostream>

class TestContext {
public:
  static void test() {
    Context  c;
    Context::Frame *t;

    c.push_new_frame(0);
    std::cout << "Frame 0" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << t << std::endl;
    }

    c.push_new_frame(1);
    std::cout << "Frame 1" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << t << std::endl;
    }

    c.push_new_frame(2);
    std::cout << "Frame 2" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << t << std::endl;
    }

    c.push_new_frame(3);
    std::cout << "Frame 3" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << t << std::endl;
    }

    c.pop_frame();
    std::cout << "Frame 2" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << t << std::endl;
    }

    c.pop_frame();
    std::cout << "Frame 1" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << t << std::endl;
    }

    c.pop_frame();
    std::cout << "Frame 0" << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << t << std::endl;
    }

    c.pop_frame();
    std::cout << "Frame NULL" << std::endl;
    std::cout << "top: " << c.top << ", bottom: " << c.bottom << std::endl;
    for(t = c.top; t != 0; t = t->next) {
      std::cout << t << std::endl;
    }
  }
};

int main(void) {
  TestContext::test();
}
