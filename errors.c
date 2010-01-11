#include "errors.h"

#include <stdlib.h>

void vm_exit() {
  exit(EXIT_FAILURE);  
}

void vm_abort() {
  abort();
}
