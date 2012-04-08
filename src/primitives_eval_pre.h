#define DECLARE_NAMED_SPECIAL(nn, prim_op) do {                         \
    /* liveness tracked through environment */                          \
    char *n = malloc_or_bail(sizeof(nn));                               \
    cell_t *s, *v = new(cell_t);                                        \
    primitive_t *p = new_malloc(primitive_t);                           \
    prim_container *c = new_malloc(prim_container);                     \
    c->next = NULL;                                                     \
    strcpy(n, nn);                                                      \
                                                                        \
    if (all_prims == NULL) {                                            \
      all_prims = c;                                                    \
    } else {                                                            \
      c->next = all_prims;                                              \
      all_prims = c;                                                    \
    }                                                                   \
    all_prims->prim = p;                                                \
                                                                        \
    p->fun = &prim_op;                                                  \
    p->name = (unsigned char *)n;                                       \
    v->slot1.type = PRIMITIVE;                                          \
    v->slot2.prim = p;                                                  \
    s = intern((unsigned char *)n, global_symtab);                      \
    add_to_environment(special_forms, s, v);                            \
  } while (0)

#define DECLARE_SPECIAL(name) DECLARE_NAMED_SPECIAL(#name, prim_ ## name)

#define DECLARE_NAMED_PRIMITIVE(nn, prim_op) do {                       \
    /* liveness tracked through environment */                          \
    char *n = malloc_or_bail(sizeof(nn));                               \
    cell_t *s, *v = new(cell_t);                                        \
    primitive_t *p = new_malloc(primitive_t);                           \
    prim_container *c = new_malloc(prim_container);                     \
    c->next = NULL;                                                     \
    strcpy(n, nn);                                                      \
                                                                        \
    if (all_prims == NULL) {                                            \
      all_prims = c;                                                    \
    } else {                                                            \
      c->next = all_prims;                                              \
      all_prims = c;                                                    \
    }                                                                   \
    all_prims->prim = p;                                                \
                                                                        \
    p->fun = &prim_op;                                                  \
    p->name = (unsigned char *)n;                                       \
    v->slot1.type = PRIMITIVE;                                          \
    v->slot2.prim = p;                                                  \
    s = intern((unsigned char *)n, global_symtab);                      \
    add_to_environment(primitives, s, v);                               \
  } while (0)

#define DECLARE_PRIMITIVE(name) DECLARE_NAMED_PRIMITIVE(#name, prim_ ## name)

#define DECLARE_NAMED_INTERNAL(nn, prim_op) do {                        \
    /* liveness tracked through environment */                          \
    char *n = malloc_or_bail(sizeof(nn));                               \
    cell_t *s, *v = new(cell_t);                                        \
    primitive_t *p = new_malloc(primitive_t);                           \
    prim_container *c = new_malloc(prim_container);                     \
    c->next = NULL;                                                     \
    strcpy(n, nn);                                                      \
                                                                        \
    if (all_prims == NULL) {                                            \
      all_prims = c;                                                    \
    } else {                                                            \
      c->next = all_prims;                                              \
      all_prims = c;                                                    \
    }                                                                   \
    all_prims->prim = p;                                                \
                                                                        \
    p->fun = &prim_op;                                                  \
    p->name = (unsigned char *)n;                                       \
    v->slot1.type = PRIMITIVE;                                          \
    v->slot2.prim = p;                                                  \
    s = intern((unsigned char *)n, global_symtab);                      \
    add_to_environment(internal, s, v);                                 \
  } while (0)

#define DECLARE_INTERNAL(name) DECLARE_NAMED_INTERNAL(#name, prim_ ## name)
