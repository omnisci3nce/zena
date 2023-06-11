// functions for querying sqlite database
#include "queries.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../deps/kitc/include/kitc.h"
#include "protocol.h"
#include "sqlite3.h"

#define ROW_UPPER_LIMIT 10000

const char *get_all_channels_query =
    "SELECT msg_id, channel_id, author_id, content FROM messages "
    "WHERE channel_id = ?;";
const char *insert_message_query =
    "INSERT INTO messages (author_id, channel_id, content) "
    "VALUES ( ?, ?, ?);";

/** returns array of messages
 * @param from get messages after this message id. set to NULL to get from first
 * @param to get messages up until this message id. set to NULL to get until latest
 */
int get_msgs_in_channel(sqlite3 *db, uint32_t channel_id, uint32_t from, uint32_t to,
                        kitc_darray *msg_array) {
  int rc;
  sqlite3_stmt *res;
  message temp_msg;

  rc = sqlite3_prepare_v2(db, get_all_channels_query, -1, &res, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }
  // bind parameters
  sqlite3_bind_int(res, 1, channel_id);

  int i = 0;
  while (i < ROW_UPPER_LIMIT) {
    int step = sqlite3_step(res);
    if (step == SQLITE_DONE) {
      break;
    } else {
      temp_msg.id = sqlite3_column_int(res, 0);
      temp_msg.channel = sqlite3_column_int(res, 1);
      temp_msg.author = sqlite3_column_int(res, 2);
      const char *msg_contents = sqlite3_column_text(res, 3);
      temp_msg.contents = malloc(strlen(msg_contents) + 1);
      strcpy(temp_msg.contents, msg_contents);  // contents will be automatically freed next
                                                // sqlite3_step so we need to copy it out
      kitc_darray_push(msg_array, &temp_msg);
      i++;
    }
  }
  // Cleanup
  sqlite3_finalize(res);
  return i;
}

/*
 * get_all_channels();
 */

query_result insert_msg(sqlite3 *db, uint32_t channel_id, uint32_t author_id, char *content) {
  int rc = 0;
  int idx = -1;
  sqlite3_stmt *stmt;

  // prepare statement
  rc = sqlite3_prepare_v2(db, insert_message_query, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
  }

  // bind parameters
  sqlite3_bind_int(stmt, 1, author_id);
  sqlite3_bind_int(stmt, 2, channel_id);
  sqlite3_bind_text(stmt, 3, content, -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);

  sqlite3_finalize(stmt);
  return Success;
}

query_result sqlite_version(sqlite3 *db) {
  int rc = 0;
  int idx = -1;
  sqlite3_stmt *res;
  rc = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, &res, 0);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  rc = sqlite3_step(res);

  if (rc == SQLITE_ROW) {
    printf("sqlite version: %s\n", sqlite3_column_text(res, 0));
  }

  sqlite3_finalize(res);
  return Success;
}
