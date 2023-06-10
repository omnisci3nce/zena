#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "state_handling.h"
#include "../shared/protocol.h"

void server_init(server_state* s) {
  printf("Init server state\n");
  // initialise struct data
  memset(&s->clients, 0, MAX_CONCURRENT_CLIENTS * sizeof(client));
  memset(&s->channels, 0, MAX_CHANNELS * sizeof(channel));
  s->ch_len = 0;
  memset(&s->messages, 0, 1024 * sizeof(message));
  s->msg_len = 0;

  // try open sqlite
}
