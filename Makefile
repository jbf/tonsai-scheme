CFLAGS  = -Wall -DDEBUG -g
PROGRAM = test_cell test_token test_symbol test_reader \
          test_environment test_primitives test_eval
SOURCES = $(filter-out test_%,$(wildcard *.c))
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPS    = $(patsubst %.c,%.dep,$(SOURCES))

all: $(DEPS) $(OBJECTS) $(PROGRAM)

depend: $(DEPS)

test_cell : cell.o

test_token : token.o

test_symbol : symbol.o

test_reader : reader.o cell.o token.o symbol.o

test_environment : symbol.o environment.o cell.o bootstrap.o

test_primitives : primitives.o eval.o environment.o symbol.o cell.o bootstrap.o

test_eval : eval.o token.o symbol.o cell.o reader.o primitives.o \
            environment.o bootstrap.o

%.dep: %.c %.h
	@set -e; rm -f $@; \
         $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed -i -e 's,\:,\:\ Makefile,' $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

.PHONY: clean

clean:
	-rm -f -- *.o *.dep *~ core a.out test_token test_cell test_reader \
        test_symbol test_environment test_eval test_primitives

include $(SOURCES:.c=.dep)
