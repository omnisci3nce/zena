CC=gcc
CFLAGS=-I.
SERVER_SRC=$(wildcard server/*.c)
SHARED_SRC=$(wildcard shared/*.c)

.PHONY: server
server: 
	$(CC) $(SERVER_SRC) $(SHARED_SRC) server_main.c -lsqlite3 -o build/server.exe

.PHONY: client
client: 
	$(CC)  $(SERVER_SRC) $(SHARED_SRC) client_main.c -lsqlite3 -o build/client.exe

clean: 
	rm -rf build/*
