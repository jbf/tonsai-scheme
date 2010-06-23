DEBUG   = -DDEBUG
#DEBUG   = -DDEBUG -DEVAL_DEBUG -DMEM_DEBUG -DLOOKUP_DEBUG
CFLAGS  = -Wall $(DEBUG) -g
PROGRAM = repl
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPS    = $(patsubst %.c,%.dep,$(SOURCES))

all: $(DEPS) $(OBJECTS) $(PROGRAM)

depend: $(DEPS)

repl : eval.o token.o symbol.o cell.o reader.o primitives.o \
       environment.o bootstrap.o function.o memory.o errors.o \
       liveness.o


%.dep: %.c %.h
	@set -e; rm -f $@; \
         $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed -i -e 's,\:,\:\ Makefile,' $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

clean:
	-rm -f -- *.o *.dep *.dep.* *~ core a.out test_token test_cell \
	test_reader test_symbol test_environment test_eval test_primitives \
	repl
	-rm -rf -- *.dSYM

include $(SOURCES:.c=.dep)

.PHONY: clean
