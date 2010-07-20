#include "memory.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>

#include "util.h"
#include "cell.h"

#ifdef LIVENESS_DEBUG
#include "liveness.h"
extern frame_t *live_root;
#endif /* LIVENESS_DEBUG */

static void *mem_sys_heap = NULL;
static void *cur = NULL;
static void *top = NULL;

/* Off-heap malloc that only returns if it succeeds */
void *malloc_or_bail(size_t bytes) {
  void *tmp;
  tmp = malloc(bytes);

  if (NULL == tmp) {
    DEBUGPRINT_("Out ouf memory. Aborting.\n");
    bail(1);
  }

  return tmp;
}

/* On-heap malloc that only returns if it succeeds */
void *mem_sys_safe_alloc(size_t bytes) {
  void *t;
  assert(NULL != mem_sys_heap &&
         NULL != cur &&
         NULL != top);
  
#ifdef MEM_DEBUG
  DEBUGPRINT("Trying to alloc: %lu bytes at %p\n", (long) bytes, cur);
#endif /* MEM_DEBUG */

#ifdef LIVENESS_DEBUG
  print_frames(live_root);
#endif /* LIVENESS_DEBUG */

  if (bytes < 0) {
    DEBUGPRINT_("Trying to allocate a negative amount of memory. Aborting.\n");
    exit(1);
  } 

  bytes = (((bytes-1)/(sizeof(void*)))+1)*sizeof(void *); /* align to sizeof(void *) */

  if (cur + bytes > top) {
    DEBUGPRINT_("Out of memory. Aborting.\n");
    exit(1);
  }
  
  t = cur;
  cur = cur + bytes;
  return t;
}
  
void *u8_new(size_t bytes) {
#ifdef MEM_DEBUG
  int orig = bytes;
#endif /* MEM_DEBUG */
  cell_t *c;
  void *t;
  char *b;
  long *l;

  if (bytes < 0) {
    DEBUGPRINT_("Trying to allocate an u8-vec of negative size. Aborting.\n");
    exit(1);
  }

  bytes += sizeof(void *)*2; /* add space for tag word and size */
  bytes = (((bytes-1)/(sizeof(void*)))+1)*sizeof(void *); /* align to sizeof(void *) */

#ifdef MEM_DEBUG
  DEBUGPRINT("Trying to alloc u8-vec: %u bytes became %u after " +
             "align, tag and size.\n",
             orig, bytes);
#endif /* MEM_DEBUG */

  t = mem_sys_safe_alloc(bytes);

  c = (cell_t *)t;
  c->slot1.type = U8VEC; /* tag */

  b = (char *)t;
  b += sizeof(void *);
  l = (long *)b;
  *l = bytes; /* size */

  b = (char *)l;
  b += sizeof(void *);
  t = (void *)b; /* return pointer to free mem 2x(void *) in */
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
                            t = (size_t)(2 * page_size),
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
