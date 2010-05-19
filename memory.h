#ifndef _MEMORY_H
#define _MEMORY_H 1

#include <stdlib.h>

#define new(type) ((type *)malloc(sizeof(type)/sizeof(char)))

void init_mem_sys();

#endif /* _MEMORY_H */
