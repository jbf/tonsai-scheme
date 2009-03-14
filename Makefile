CFLAGS = -Wall -DDEBUG -g
PROGRAM = test_cell test_token test_symbol test_reader
EXTRAS = errors.h util.h Makefile
OBJECTS = reader.o token.o symbol.o cell.o

all: $(PROGRAM)

test_cell : cell.o

test_token : token.o

test_symbol : symbol.o

test_reader : reader.o cell.o token.o symbol.o

cell.o : $(EXTRAS) symbol.h

symbol.o : $(EXTRAS) cell.h

reader.o: $(EXTRAS) cell.h symbol.h token.h

$(OBJECTS) : $(EXTRAS)

.PHONY: clean

clean:
	-rm -f -- *.o *~ core a.out test_token test_cell
