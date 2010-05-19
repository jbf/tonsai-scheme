#include "memory.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>

#include "util.h"

static void *mem_sys_heap = NULL;
static size_t heap_left = 0;

/* Inits the managed memory subsytem. Should only be done once. */
void init_mem_sys() {
  int page_size = getpagesize();
  
  if (NULL != mem_sys_heap) {
    DEBUGPRINT_("Memory subsystem is already initialized! aborting!\n");
    exit(1);
  }

  if (page_size < 4096) {
    DEBUGPRINT("page_size to weird: %d aborting!\n", page_size);
    exit(1);
  }
  mem_sys_heap = mmap((void *)(long)page_size,
                      heap_left = (size_t)(1 * page_size),
                      PROT_READ | PROT_WRITE,
                      MAP_ANON | MAP_PRIVATE,
                      -1,
                      0);

  if (mem_sys_heap == MAP_FAILED) {
    /* Find out what errno is stupid! */
    perror("Trying to map memory");
    DEBUGPRINT_("Can't map memory, aborting!\n");
    exit(1);
  }
}
