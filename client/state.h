#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include <stdint.h>

// enum connection_state {
//
// };

typedef struct client_state {
  uint32_t user_id;
  //connection_state state;

  // TODO: sqlite3 *db;
} client_state;

#endif
