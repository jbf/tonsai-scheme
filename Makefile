DEBUG           = -DDEBUG
# All defined debug levels
#DEBUG          = -DDEBUG -DEVAL_DEBUG -DMEM_DEBUG -DLOOKUP_DEBUG -DLIVENESS_DEBUG
CFLAGS          = -Wall $(DEBUG) -g
CXXFLAGS_EXTRA  = -Wall -g
PROGRAM         = tlisp
SOURCES         = $(wildcard src/ast-walker/*.c)
OBJECTS         = $(patsubst %.c,%.o,$(SOURCES))
DEPS            = $(patsubst %.c,%.dep,$(SOURCES))

all: depend $(PROGRAM) vm

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
	-rm -f -- src/ast-walker/*.o src/ast-walker/*.dep \
                  src/ast-walker/*.dep.* src/ast-walker/*~
	-rm -f -- src/vm/*.o src/vm/*.dep src/vm/*.dep.* src/vm/*~ \
                  src/vm/cpp_vm.d
	-rm -f -- src/vm/test/*.o src/vm/test/*.dep src/vm/test/*.dep.* \
                  src/vm/test/*~ test/test_context src/vm/test/test_context.d

tags:
	-rm -f -- TAGS GTAGS GRTAGS GSYMS GPATH
	find . -name "*.[c|h]" | xargs etags -I
	gtags -v

cloc: clean
	cloc src Makefile dev-tools lib test/test_primitives.c test.rb

vm:
	@PATH=../redo:$(PATH) redo src/vm/vm

.PHONY: clean tags cloc
