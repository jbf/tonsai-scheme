#ifndef _MEMORY_H
#define _MEMORY_H 1

#include <stdlib.h>

void *mem_sys_safe_alloc(size_t bytes);
void *malloc_or_bail(size_t bytes);
void *u8_new(size_t bytes);

#define new(type) ((type *)mem_sys_safe_alloc(sizeof(type)/sizeof(char)))
#define new_malloc(type) ((type *)malloc_or_bail(sizeof(type)/sizeof(char)))

void init_mem_sys__safe();

#endif /* _MEMORY_H */
