CFLAGS = -Wall -DDEBUG -g
PROGRAM = test_cell test_token test_symbol test_reader
EXTRAS = errors.h util.h Makefile
OBJECTS = reader.o token.o symbol.o cell.o eval.o environment.o

all: $(PROGRAM)

test_cell : cell.o

test_token : token.o

test_symbol : symbol.o

test_reader : reader.o cell.o token.o symbol.o

cell.o : $(EXTRAS) symbol.h cell.h

symbol.o : $(EXTRAS) cell.h token.h symbol.h

reader.o: $(EXTRAS) cell.h symbol.h token.h reader.h

environment.o: $(EXTRAS) symbol.h environment.h

$(OBJECTS) : $(EXTRAS)

.PHONY: clean

clean:
	-rm -f -- *.o *~ core a.out test_token test_cell test_reader
