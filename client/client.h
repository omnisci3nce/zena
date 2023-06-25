#ifndef CLIENT_H
#define CLIENT_H

#include "sqlite3.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <poll.h>
#include "model.h"

#define MAX_CHANNELS 32
#define MSG_CACHE_SIZE 64 // keep the last .. # of messages per channel in memory

typedef enum client_status {
  STARTING_UP,
  AUTHENTICATING,
  INITIAL_SYNCING,
  ALIVE
} client_status;

static const char *const client_status_str[] = {[STARTING_UP] = "Starting up",
                                        [AUTHENTICATING] = "Authenticating",
                                        [INITIAL_SYNCING] = "Sync in progress",
                                        [ALIVE] = "Connected"};
/**
 * god object containing all the global state of the client
 */
typedef struct client_state {
  // ----- me myself and I
  uint32_t user_id;
  char *username;

  enum client_status status;
  int sockfd;
  struct pollfd pfds[1];
  sqlite3 *db;

  // ----- b1G Daytaaaa
  channel channels[MAX_CHANNELS];

  /** TEMPORARY: in-memory array of messages */
  message messages[1024];
  size_t msg_len;

  // -----
  uint8_t write_buf[1024];
  uint8_t read_buf[1024];
} client_state;

// ----- Lifecycle methods

void client_init(client_state *c);
bool client_connect(client_state *c, const char *address, int port);

/** @brief does an initial handshake with the server to resolve a username and
 * password to a user_id and authenticate the session.
 * @param username
 * @param password
 */
bool client_handshake(client_state *c, const char *username, const char *password);
void client_shutdown(client_state *c);
/* @brief starts the main poll() loop for the client */
void client_start(client_state *c);

// ----- The Juice

int client_run(client_state *c);

#endif /* ifndef CLIENT_H */
