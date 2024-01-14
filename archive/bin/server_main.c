#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "kitc.h"

#include "server.h"
#include "queries.h"
#include "state_handling.h"
#include "containers.h"

// temporary
#include "sqlite3.h"

int main() {
  // init our server state
  server_state server;
  server_init(&server);

  // test some db queries !
  sqlite_version(server.db);

  // fetch all msgs in the db and store in a dynamic array provided by kitc
  message_darray *messages = message_darray_new(2);
  get_msgs_in_channel(server.db, 1, 0, 0, messages);
  int msg_len = message_darray_len(messages);
  printf("number of messages in db: %d\n", msg_len);

  server_start(&server);

  // cleanup server
  sqlite3_close(server.db);
  // server_shutdown(&server);
}
