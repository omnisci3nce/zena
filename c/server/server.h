#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <stddef.h>
#include "../shared/protocol.h"

#define MAX_CONCURRENT_CLIENTS 32
#define MAX_CHANNELS 32
#define MSG_CACHE_SIZE 64 // keep the last .. number of messages in memory

typedef struct channel_options {
  // TODO: things like whether mentions are allowed, visibilty, etc
} channel_options;

typedef struct channel {
  char *name;
  channel_options options;
  uint32_t last_msg_id;

  // TODO: ring_buffer recent_msgs; // keep each channels most recent messages in memory for
                                    // fast access
} channel;

typedef struct client {
  // TODO: read buffer
  // TODO: write buffer

} client;

/**
 * god object containing all the global state of the server
 */
typedef struct server_state {
  /** @brief array of file descriptors for connected client sockets */
  int client_sock_fds[MAX_CONCURRENT_CLIENTS];

  /** channels */
  channel channels[MAX_CHANNELS];
  size_t ch_len;

  /** TEMPORARY: in-memory array of messages */
  message messages[1024];
  size_t msg_len;
} server_state;

void server_init(server_state* s);

#endif
