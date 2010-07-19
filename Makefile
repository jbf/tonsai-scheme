DEBUG   = -DDEBUG -DLIVENESS_DEBUG
#DEBUG   = -DDEBUG -DEVAL_DEBUG -DMEM_DEBUG -DLOOKUP_DEBUG -DLIVENESS_DEBUG
CFLAGS  = -Wall $(DEBUG) -g
PROGRAM = repl
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPS    = $(patsubst %.c,%.dep,$(SOURCES))

all: $(DEPS) $(OBJECTS) $(PROGRAM)

depend: $(DEPS)

repl: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.dep: %.c %.h
	@set -e; rm -f $@; \
         $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed -i -e 's,\:,\:\ Makefile,' $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

clean:
	-rm -f -- *.o *.dep *.dep.* *~ core a.out repl
	-rm -rf -- *.dSYM
	-rm -f -- src/*.o src/*.dep src/*.dep.* src/*~

include $(SOURCES:.c=.dep)

.PHONY: clean
