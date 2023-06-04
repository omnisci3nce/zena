
#include <stdint.h>
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

/**
 * god object containing all the global state of the server
 */
typedef struct server_state {
  /** @brief array of file descriptors for connected client sockets */
  int client_sock_fds[MAX_CONCURRENT_CLIENTS];

  // channels
  channel channels[MAX_CHANNELS];

} server_state;
