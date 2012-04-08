  /*SPECIALS*/
  DECLARE_SPECIAL(if);
  DECLARE_SPECIAL(lambda);
  DECLARE_SPECIAL(quote);
  DECLARE_SPECIAL(define);

  /*PRIMITIVES*/
  DECLARE_NAMED_PRIMITIVE("+", prim_plus);
  DECLARE_NAMED_PRIMITIVE("*", prim_mul);
  DECLARE_NAMED_PRIMITIVE("=", prim_number_equals);
  DECLARE_NAMED_PRIMITIVE("-", prim_minus);
  DECLARE_PRIMITIVE(error);
  DECLARE_PRIMITIVE(length);
  DECLARE_NAMED_PRIMITIVE("eq?", prim_eq);
  DECLARE_PRIMITIVE(cons);
  DECLARE_NAMED_PRIMITIVE("set-car!", prim_setcar);
  DECLARE_NAMED_PRIMITIVE("set-cdr!", prim_setcdr);
  DECLARE_NAMED_PRIMITIVE("list", prim_list);

  /*INTERNAL*/
  DECLARE_INTERNAL(definternal);
  DECLARE_INTERNAL(deflibrary);
