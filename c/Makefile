CC=gcc
CFLAGS=-I.
SRC_FILES=$(wildcard server/*.c)

.PHONY: server
server: 
	$(CC) $(SRC_FILES) server_main.c -o build/server.exe

clean: 
	rm -rf build/*
