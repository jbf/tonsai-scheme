#ifndef _MEMORY_H
#define _MEMORY_H 1

#include <stdlib.h>

void *mem_sys_safe_alloc(size_t bytes);

#define new(type) ((type *)mem_sys_safe_alloc(sizeof(type)/sizeof(char)))

void init_mem_sys();

#endif /* _MEMORY_H */
