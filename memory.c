#include "memory.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>

#include "util.h"

static void *mem_sys_heap = NULL;
static void *cur = NULL;
static void *top = NULL;

void *mem_sys_safe_alloc(size_t bytes) {
  void *t;
  assert(NULL != mem_sys_heap &&
         NULL != cur &&
         NULL != top);
  
#ifdef MEM_DEBUG
  DEBUGPRINT("Trying to alloc: %lu bytes at %p\n", (long) bytes, cur);
#endif /* MEM_DEBUG */

  if (bytes < 0) {
    DEBUGPRINT_("Trying to allocate a negative amount of memory. Aborting.\n");
    exit(1);
  } else if (cur + bytes > top) {
    DEBUGPRINT_("Out of memory. Aborting.\n");
    exit(1);
  }
  
  t = cur;
  cur = cur + bytes;
  return t;
}
  

/* Inits the managed memory subsytem. Should only be done once. */
void init_mem_sys() {
  int page_size = getpagesize();
  size_t t;
  
  if (NULL != mem_sys_heap) {
    DEBUGPRINT_("Memory subsystem is already initialized! aborting!\n");
    exit(1);
  }

  if (page_size < 4096) {
    DEBUGPRINT("page_size to weird: %d aborting!\n", page_size);
    exit(1);
  }
  cur = mem_sys_heap = mmap(NULL,
                            t = (size_t)(1 * page_size),
                            PROT_READ | PROT_WRITE,
                            MAP_ANON | MAP_PRIVATE,
                            -1,
                            0);

  if (mem_sys_heap == MAP_FAILED) {
    /* Find out what errno is stupid! */
    perror(AT);
    DEBUGPRINT_("Can't map memory, aborting!\n");
    exit(1);
  }
  
  top = mem_sys_heap + t;

  DEBUGPRINT("\tmem_sys_heap: %p, top: %p\n"
             "\t\t\t\theap size: %d bytes\n"
             "\t\t\t\tsizeof(void *) is %lu bytes\n",
             mem_sys_heap,
             top,
             (int)(top-mem_sys_heap),
             sizeof (void *));
}