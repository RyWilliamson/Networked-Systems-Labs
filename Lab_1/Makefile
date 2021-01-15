CC=clang
CFLAGS=-W -Wall -Werror

build: buildclient buildserver

buildclient: hello_client.o
	$(CC) -o hello_client hello_client.o

buildserver: hello_server.o
	$(CC) -o hello_server hello_server.o