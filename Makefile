DEBUG   = -DDEBUG
#DEBUG   = -DDEBUG -DEVAL_DEBUG -DMEM_DEBUG -DLOOKUP_DEBUG -DLIVENESS_DEBUG
CFLAGS  = -Wall $(DEBUG) -g
CXXFLAGS_EXTRA = -Wall -g
PROGRAM = repl
SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))
DEPS    = $(patsubst %.c,%.dep,$(SOURCES))

all: depend $(PROGRAM) src/vm/cpp_vm.o

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
	-rm -f -- TAGS GPATH  GRTAGS  GSYMS  GTAGS
	-rm -f -- src/vm/*.o src/vm/*.dep src/vm/*.dep.* src/vm/*~
tags:
	find . -name "*.[c|h]" | xargs etags
	gtags -v

src/vm/cpp_vm.o: src/vm/cpp_vm.cpp src/vm/cpp_vm.hpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_EXTRA) -c -o $@ $<

.PHONY: clean
