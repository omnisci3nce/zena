#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "server.h"
#include "state_handling.h"
#include "../shared/protocol.h"
#include "sqlite3.h"

void server_init(server_state* s) {
  printf("Init server state\n");
  // initialise struct data
  memset(&s->clients, 0, MAX_CONCURRENT_CLIENTS * sizeof(client));
  memset(&s->channels, 0, MAX_CHANNELS * sizeof(channel));
  s->ch_len = 0;
  memset(&s->messages, 0, 1024 * sizeof(message));
  s->msg_len = 0;

  // db setup
  sqlite3 *db;
  sqlite3_stmt *res;

  const char *db_path = "db/db.sqlite";

  struct stat st;
  stat(db_path, &st);
  printf("db size: %ld bytes\n", st.st_size);

  int rc = sqlite3_open(db_path, &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    //return 1;
  }

  s->db = db; // give the server state a pointer to the sqlite connection
}

