(definternal (foo bar baz) "aaa")
(definternal (noo c) (+ 1 c))
(deflibrary (baz mun) "ooo")
(deflibrary (gaa) (foo 10 20))
(deflibrary (maa a) (noo a))
