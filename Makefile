CC=gcc

INC_DIRS=-I. -I./server/ -I./shared/ -I./deps/sqlite/ -I./deps/kitc/include/

BUILD_DIR=build/
SERVER_SRC=$(wildcard server/*.c)
CLIENT_SRC=$(wildcard client/*.c)
SHARED_SRC=$(wildcard shared/*.c) $(wildcard deps/kitc/src/*.c)

CFLAGS= -g -Wall -Wextra -Werror -Wundef -Wdouble-promotion 
# -g        : compile with debug symbols
# -Wall     : base set of warnings
# -Wextra   : additional warnings not covered by -Wall
# -Werror   : warnings become errors
# -Wundef   : undefined macros
# -Wdouble-promotion : catch implicit converion of float to double

CFILES=protocol.c server.c
SERVER_OBJS = server.o
# SERVER_OBJS = client.o
SHARED_OBJS = protocol.o

server: $(SERVER_OBJS) $(SHARED_OBJS)
	$(CC) $(SERVER_SRC) $(SHARED_SRC) $(INC_DIRS) build/sqlite3.o bin/server_main.c -o build/server.exe

.PHONY: sqlite
sqlite: sqlite3.o

sqlite3.o: deps/sqlite/sqlite3.c
	$(CC) deps/sqlite/sqlite3.c -c -o build/sqlite3.o

protocol.o: shared/protocol.h shared/protocol.c
	$(CC) -c shared/protocol.c -o build/$@
server.o: server/server.h server/server.c
	$(CC) -c server/server.c -o build/$@

# % is a wildcard
# %.o:server/%.c
# 	$(CC) $(CFLAGS) -c -o $@ $^ # @ = whats on LHS of rule  ^ = whats on RHS of rule

client:  $(SHARED_OBJS)
	$(CC) $(CLIENT_SRC) $(SHARED_SRC) $(INC_DIRS) build/sqlite3.o bin/client_main.c -o build/client.exe

clean: 
	rm -rf build/*

.PHONY:
format:
	clang-format -i $(SERVER_SRC) $(SHARED_SRC) $(CLIENT_SRC)
