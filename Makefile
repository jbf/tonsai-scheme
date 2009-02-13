CFLAGS = -Wall -DDEBUG -g

all: test_token

test_token: token.o test_token.o
	$(CC) $(CFLAGS) $@.o token.o -o $@

.c: $@.h errors.h
	$(CC) $(CFLAGS) $@.c -o $@ -c

clean:
	-rm -f -- *.o *~ core a.out
