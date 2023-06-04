
#define MAX_CONCURRENT_CLIENTS 32
#define MAX_CHANNELS 32

typedef struct channel {

} channel;

/**
 * god object containing all the global state of the server
 */
typedef struct server_state {
  /** @brief array of file descriptors for connected client sockets */
  int client_sock_fds[MAX_CONCURRENT_CLIENTS];

  // channels
  channel channels[MAX_CHANNELS];

} server_state;
