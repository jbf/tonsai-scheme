#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"

#include "util.h"

extern symbol_table *global_symtab;

static void *mem_sys_heap1 = NULL;
static void *mem_sys_heap2 = NULL;
static void *red_pages = NULL;

static size_t mem_sys_heap1_size = 0;
static size_t mem_sys_heap2_size = 0;

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

/* Off-heap free. */
void free_malloced(void *ptr) {
  free(ptr);
}

/* On-heap malloc that only returns if it succeeds */
void *mem_sys_safe_alloc(size_t bytes) {
  void *t;
  assert(NULL != mem_sys_heap1 &&
         NULL != cur &&
         NULL != top);

#ifdef MEM_DEBUG
  DEBUGPRINT("Trying to alloc: %lu bytes at %p\n", (long) bytes, cur);
#endif /* MEM_DEBUG */

  if (bytes <= 0) {
    DEBUGPRINT_("Trying to allocate zero or a negative amount of memory. Aborting.\n");
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

u8_cell_t *u8_new(size_t bytes) {
  size_t orig = bytes;
  u8_cell_t *c;
  void *t;

  if (bytes <= 0) {
    DEBUGPRINT_("Trying to allocate an u8-vec of zero or negative size. Aborting.\n");
    exit(1);
  }

  bytes += sizeof(void *)*2; /* add space for tag word and size */
  bytes = (((bytes-1)/(sizeof(void*)))+1)*sizeof(void *); /* align to sizeof(void *) */

#ifdef MEM_DEBUG
  DEBUGPRINT("Trying to alloc u8-vec: %zu bytes became %zu after "
             "align, tag and size.\n",
             orig, bytes);
#endif /* MEM_DEBUG */

  t = mem_sys_safe_alloc(bytes);

  c = (u8_cell_t *)t;
  c->header.slot1.type = U8VEC; /* tag */
  c->header.slot2.u8_data_size = bytes-sizeof(cell_t);
  memset(c->data, 0, c->header.slot2.u8_data_size);
  return c;
}

/* Inits the managed memory subsytem. Should only be done once. */
void init_mem_sys__safe() {
  int page_size = getpagesize();
  size_t t;

  if (NULL != mem_sys_heap1 ||
      NULL != mem_sys_heap2) {
    DEBUGPRINT_("Memory subsystem is already initialized! aborting!\n");
    exit(1);
  }

  if (page_size < 4096) {
    DEBUGPRINT("page_size to weird: %d aborting!\n", page_size);
    exit(1);
  }
  cur = mem_sys_heap1 = mmap(NULL,
                            t = (size_t)(2 * page_size),
                            PROT_READ | PROT_WRITE,
                            MAP_ANON | MAP_PRIVATE,
                            -1,
                            0);
  if (mem_sys_heap1 == MAP_FAILED) {
    /* Find out what errno is stupid! */
    perror(AT);
    DEBUGPRINT_("Can't map memory heap, aborting!\n");
    exit(1);
  }
  top = mem_sys_heap1 + t;
  mem_sys_heap1_size = (size_t)(top-mem_sys_heap1);

  red_pages = mmap(NULL,
                   t = (size_t)(2 * page_size),
                   PROT_NONE,
                   MAP_ANON | MAP_PRIVATE,
                   -1,
                   0);
  mprotect(red_pages, mem_sys_heap2_size, PROT_NONE);

  mem_sys_heap2 = mmap(NULL,
                       t = (size_t)(2 * page_size),
                       PROT_NONE,
                       MAP_ANON | MAP_PRIVATE,
                       -1,
                       0);
  if (mem_sys_heap2 == MAP_FAILED) {
    /* Find out what errno is stupid! */
    perror(AT);
    DEBUGPRINT_("Can't map memory for heap, aborting!\n");
    exit(1);
  }
  mem_sys_heap2_size = (size_t)t;
  mprotect(mem_sys_heap2, mem_sys_heap2_size, PROT_NONE);

  /* Test that red pages are between the areas */

  fprintf(stderr,
          "\tmem_sys_heap1: %p, top: %p\n"
          "\theap size: %ld bytes\n"
          "\tsizeof(void *) is %d bytes\n"
          "\tmem_sys_heap2: %p\n"
          "\theap size: %ld bytes\n",
          mem_sys_heap1,
          top,
          (long)mem_sys_heap1_size,
          (int)sizeof (void *),
          mem_sys_heap2,
          (long)mem_sys_heap2_size);
}

void destroy_mem_sys__safe() {
  int ret;
  fprintf(stderr,
          "\tunmapping %ld bytes at page(s): %p\n",
          (long)mem_sys_heap1_size,
          mem_sys_heap1);
  ret = munmap(mem_sys_heap1, mem_sys_heap1_size);
  if (ret != 0) {
    fprintf(stderr, "\terror unmapping heap\n");
    perror(NULL);
  }
}

void scan_heap() {
  char *start, *base, *end;

  start = (char *)mem_sys_heap1;
  end = start + mem_sys_heap1_size;

  for (base = start; base < end; ) {
    cell_t *obj = (cell_t *)base;

    if (U8VECP(obj)) {
      base = base + sizeof(cell_t) + U8LEN(obj);
    } else {
      base += sizeof(cell_t);
    }
  }
}


void print_roots() {
  symtab_iterator_t sym_iter, *i;
  cell_t *c;

  i = &sym_iter;
  init_symtab_iterator(i, global_symtab);
  while(symtab_iter_has_next(i)) {
    c = symtab_iter_next_sym(i);
    pretty_print(c);
  }


}
