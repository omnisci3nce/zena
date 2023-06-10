#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../server/server.h"
#include "state_handling.h"
#include "network.h"
#include "queries.h"
#include "../deps/kitc/include/kitc.h"


// temporary
#include "sqlite3.h"

int main() {
  // init our server state
  server_state server;
  server_init(&server);

  char send_buf[1024];

  // test some db queries !
  sqlite_version(server.db);

  // insert a new message into the db
  char *hw = "hello, world!";
  insert_msg(server.db, 1, 1, hw);

  // fetch all msgs in the db and store in a dynamic array provided by kitc
  kitc_darray *messages = kitc_darray_new(sizeof(message), 2);
  int msg_len = get_msgs_in_channel(server.db, 1, NULL, NULL, messages);
  printf("number of messages in db: %d\n", msg_len);
  for (int i; i < msg_len; ++i) {
    message *msg = &((message *)messages->data)[i];
    printf("msg: %d channel: %d author: %d msg content: %s\n", msg->id, msg->channel, msg->author, msg->contents);
  }

  // tcp setup
  int listenfd = create_listen_socket();


  // cleanup server
  sqlite3_close(server.db);
  server_shutdown(&server);
}
