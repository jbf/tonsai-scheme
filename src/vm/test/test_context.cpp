#include "../cpp_vm.hpp"

#include <iostream>

int main(void) {
  Context  c;

  c.push_new_frame(0);
  c.push_new_frame(1);
  c.push_new_frame(2);
  c.pop_frame();
  c.pop_frame();
  c.pop_frame();
}
