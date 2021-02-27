CC=clang
CFLAGS=-W -Wall -Werror
DEPS = common.h
OBJ = dnslookup.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

dnslookup: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o http_connect
