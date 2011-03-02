DEBUG   = -DDEBUG
#DEBUG   = -DDEBUG -DEVAL_DEBUG -DMEM_DEBUG -DLOOKUP_DEBUG -DLIVENESS_DEBUG
CFLAGS  = -Wall $(DEBUG) -g
CXXFLAGS_EXTRA = -Wall -g
PROGRAM = repl
SOURCES = $(wildcard src/ast-walker/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPS    = $(patsubst %.c,%.dep,$(SOURCES))

all: depend $(PROGRAM) src/vm/cpp_vm.o test/test_context

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
	-rm -f -- TAGS GPATH  GRTAGS  GSYMS  GTAGS
	-rm -f -- src/vm/*.o src/vm/*.dep src/vm/*.dep.* src/vm/*~
	-rm -f -- src/vm/test/*.o src/vm/test/*.dep src/vm/test/*.dep.* \
                  src/vm/test/*~ test/test_context

tags:
	find . -name "*.[c|h]" | xargs etags
	gtags -v

cloc: clean
	cloc src Makefile dev-tools lib test/test_primitives.c test.rb

src/vm/cpp_vm.o: src/vm/cpp_vm.cpp src/vm/cpp_vm.hpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_EXTRA) -c -o $@ $<

src/vm/test/test_context.o: src/vm/test/test_context.cpp src/vm/cpp_vm.hpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_EXTRA) -c -o $@ $<

test/test_context: src/vm/cpp_vm.o src/vm/test/test_context.o
	$(CXX) $(LDFLAGS) -o $@ $^

.PHONY: clean
