CFLAGS = -Wall -DDEBUG -g
ALL = reader.o cell.o token.o test_cell test_token

all: $(ALL)

test_cell: cell.o test_cell.o
	$(CC) $(CFLAGS) cell.o $@.o -o $@

test_token: token.o test_token.o
	$(CC) $(CFLAGS) token.o $@.o -o $@

.c: $@.h errors.h
	$(CC) $(CFLAGS) $@.c -o $@ -c

clean:
	-rm -f -- *.o *~ core a.out test_token test_cell
