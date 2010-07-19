#ifndef _MEMORY_H
#define _MEMORY_H 1

#include <stdlib.h>

void *mem_sys_safe_alloc(size_t bytes);
void *malloc_or_bail(size_t bytes);
void free(void *);

#define new(type) ((type *)mem_sys_safe_alloc(sizeof(type)/sizeof(char)))

void init_mem_sys();

#endif /* _MEMORY_H */
