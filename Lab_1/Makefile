CC=clang
CFLAGS=-W -Wall -Werror
build: buildclient buildserver

buildclient: hello_client.o common.o
	$(CC) -o hello_client hello_client.o common.o

buildserver: hello_server.o common.o
	$(CC) -o hello_server hello_server.o common.o

.PHONY: clean

clean:
	rm -f *.o hello_client hello_server
