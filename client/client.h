#ifndef CLIENT_H
#define CLIENT_H

#include <stdbool.h>
#include <stdint.h>
#include <poll.h>

/** client */
typedef struct client_state {
  uint32_t user_id;
  char *username;

  int sockfd;
  struct pollfd pfds[1];

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
int  client_handshake(client_state *c, const char *username, const char *password);
void client_shutdown(client_state *c);
void client_start(client_state *c);

#endif /* ifndef CLIENT_H */
