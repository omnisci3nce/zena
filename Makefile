CC=gcc

INC_DIRS=-I. -I./server/

BUILD_DIR=build/
SERVER_SRC=$(wildcard server/*.c)
CLIENT_SRC=$(wildcard server/*.c)
SHARED_SRC=$(wildcard shared/*.c)

CFLAGS= -g -Wall -Wextra -Werror -Wundef -Wdouble-promotion 
# -g        : compile with debug symbols
# -Wall     : base set of warnings
# -Wextra   : additional warnings not covered by -Wall
# -Werror   : warnings become errors
# -Wundef   : undefined macros
# -Wdouble-promotion : catch implicit converion of float to double

CFILES=protocol.c server.c
SERVER_OBJS = server.o
SHARED_OBJS = protocol.o

server: $(SERVER_OBJS) $(SHARED_OBJS)
	$(CC) $(SERVER_SRC) $(SHARED_SRC) $(INC_DIRS) bin/server_main.c -lsqlite3 -o build/server.exe

protocol.o: shared/protocol.h shared/protocol.c
	$(CC) -c shared/protocol.c -o build/$@
server.o: server/server.h server/server.c
	$(CC) -c server/server.c -o build/$@

# % is a wildcard
# %.o:server/%.c
# 	$(CC) $(CFLAGS) -c -o $@ $^ # @ = whats on LHS of rule  ^ = whats on RHS of rule

# .PHONY: client
# client: 
# 	$(CC)  $(SERVER_SRC) $(SHARED_SRC) client_main.c -lsqlite3 -o build/client.exe

clean: 
	rm -rf build/*
