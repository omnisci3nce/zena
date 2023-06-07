#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "state_handling.h"
#include "../shared/protocol.h"

void server_init(server_state* s) {
  printf("Init server state\n");
  memset(&s->client_sock_fds, 0, MAX_CONCURRENT_CLIENTS * sizeof(int));
  memset(&s->channels, 0, MAX_CHANNELS * sizeof(channel));
  s->ch_len = 0;
  memset(&s->messages, 0, 1024 * sizeof(message));
  s->msg_len = 0;
}

