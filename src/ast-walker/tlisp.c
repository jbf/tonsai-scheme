#include "tlisp.h"
#include "threads.h"
#include "eval.h"
#include "liveness.h"
#include "memory.h"

#define destroy_vm() do { destroy_mem_sys__safe(); } while(0)

int main(int argc, char **argv, char **envp) {
  int repl_thread;

  init_mem_sys__safe();
  init_eval__safe();

  /* Handle error here */
  repl_thread = run_in_thread("(repl)");
    
  destroy_vm();
  return 0;
}
