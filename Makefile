CFLAGS = -Wall -DDEBUG -g

all: token

.c: $@.h
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	-rm -f -- *.o *~ core a.out
