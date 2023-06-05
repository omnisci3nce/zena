#include <stdio.h>
#include "server/server.h"
#include <sys/types.h>
#include <sys/socket.h>

int main() {
  // init our server state
  server_state server_state;
  server_init(&server_state);
  
  // tcp setup
  int listenfd;
  /*
  struct sockaddr_in serv_addr;
  listenfd = socket(, int type, int protocol)
  */
}
