DEBUG   = -DDEBUG
#DEBUG   = -DDEBUG -DEVAL_DEBUG -DMEM_DEBUG -DLOOKUP_DEBUG -DLIVENESS_DEBUG
CFLAGS  = -Wall $(DEBUG) -g
PROGRAM = repl
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPS    = $(patsubst %.c,%.dep,$(SOURCES))

all: depend $(PROGRAM)

depend: $(DEPS)

include $(SOURCES:.c=.dep)

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJECTS) $(DEPS): Makefile

%.dep: %.c %.h
	@echo "$(CC) -MM $(CPPFLAGS) $< > $@"
	@set -e; rm -f $@; \
         $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

clean:
	-rm -f -- *.o *.dep *.dep.* *~ core a.out repl
	-rm -rf -- *.dSYM
	-rm -f -- src/*.o src/*.dep src/*.dep.* src/*~
	-rm -f -- TAGS

tags:
	find . -name "*.[c|h]" | xargs etags
	gtags -v

.PHONY: clean
