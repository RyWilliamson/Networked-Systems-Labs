CC=clang
CFLAGS=-W -Wall -Werror
DEPS = common.h
OBJ = http_connect.o common.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

http_connect: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o http_connect
