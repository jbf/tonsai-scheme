#ifndef _ERRORS_H
#define _ERRORS_H 1

#include <setjmp.h>

/* FATAL errors -100 to -999*/
#define EOOM -100
#define ERROR_UNREACHABLE_STATEMENT -101

#define ERROR_OTHER_ERROR -1000

/*
 * Kill the vm in a controlled fashion.
 */
void vm_exit();

/*
 * Kill the vm.
 */
void vm_abort();

#ifndef GOTO_TOPLEVEL
jmp_buf __jmp_env;
#define GOTO_TOPLEVEL() longjmp(__jmp_env, 1)
#endif /* GOTO_TOPLEVEL */

#endif /* _ERRORS_H */
