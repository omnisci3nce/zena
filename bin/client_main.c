#include <arpa/inet.h>   // inet_addr
#include <netinet/in.h>  // sockaddr_in
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "../shared/protocol.h"
#include "./state_handling.h"
#include "client.h"

const char *USAGE =
    "zena - usage\n"
    "(1) username\n"
    "(2) password";

int main(int argc, char *argv[]) {
  // parse command line arguments to get username and password
  if (argc < 3) {
    printf("%s\n", USAGE);
    return -1;
  }

  // arg 1 - username
  char *username = argv[1];
  // arg 2 - username
  char *password = argv[2];

  // init client state
  client_state client;
  client_init(&client);

  // connect to server
  client_connect(&client, "", 5000);

  // perform handshake with server authenticating the client and getting back a user id
  // NOTE: this will block
  client_handshake(&client, username, password);

  packet p = {0};
  p.header.type = FETCH_MSGS;
  struct generic_id chan_id = {.id = 1};
  p.data.generic_id = chan_id;

  int len = serialise_packet(&p, client.write_buf);
  sleep(1);
  int sent = send(client.sockfd, client.write_buf, len, 0);
  printf("sent %s packet %d bytes got sent\n", op_to_str[p.header.type], sent);

  client_run(&client);

  return 0;
}
