CFLAGS = -Wall -DDEBUG -g
ALL = reader.o cell.o symbol.o token.o test_cell test_token test_symbol

all: $(ALL)

test_cell: cell.o test_cell.o
	$(CC) $(CFLAGS) cell.o $@.o -o $@

test_token: token.o test_token.o
	$(CC) $(CFLAGS) token.o $@.o -o $@

test_symbol: symbol.o test_symbol.o
	$(CC) $(CFLAGS) symbol.o $@.o -o $@

.c: $@.h errors.h
	$(CC) $(CFLAGS) $@.c -o $@ -c

clean:
	-rm -f -- *.o *~ core a.out test_token test_cell
