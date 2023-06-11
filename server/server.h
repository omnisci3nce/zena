#ifndef SERVER_H
#define SERVER_H

#include "sqlite3.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../shared/protocol.h"
#include <poll.h>

#define MAX_CONCURRENT_CLIENTS 32
#define MAX_CHANNELS 32
#define MSG_CACHE_SIZE 64 // keep the last .. number of messages in memory

typedef struct channel_options {
  // TODO: things like whether mentions are allowed, visibilty, etc
} channel_options;

// in-memory representation of channel
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
  uint32_t user_id;
  int socket_fd;

} client;

int remove_client();

/**
 * god object containing all the global state of the server
 */
typedef struct server_state {
  sqlite3 *db;

  /** @brief array of file descriptors for connected client sockets */
  client clients[MAX_CONCURRENT_CLIENTS];
  // sockets
  struct pollfd fds[MAX_CONCURRENT_CLIENTS + 1];
  int fd_count;

  /** channels */
  channel channels[MAX_CHANNELS];
  size_t ch_len;

  /** TEMPORARY: in-memory array of messages */
  message messages[1024];
  size_t msg_len;
} server_state;

bool add_to_pfds(server_state *s, int newfd);
// ----- Lifecycle methods

void server_init(server_state *s);
void server_shutdown(server_state *s);
void server_start(server_state *s);

#endif
