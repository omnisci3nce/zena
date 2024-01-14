// functions for querying sqlite database
#include "queries.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"
#include "sqlite3.h"

#include "containers.h"

#define ROW_UPPER_LIMIT 10000


const char *get_all_channels_query =
    "SELECT msg_id, channel_id, author_id, content FROM messages "
    "WHERE channel_id = ?;";
const char *insert_message_query =
    "INSERT INTO messages (author_id, channel_id, content) "
    "VALUES ( ?, ?, ?);";

query_result get_msgs_in_channel(sqlite3 *db, uint32_t channel_id, uint32_t from, uint32_t to,
                                 message_darray *msg_array) {
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
      message_darray_push_copy(msg_array, &temp_msg);
      i++;
    }
  }
  // Cleanup
  sqlite3_finalize(res);
  return i;
}

query_result insert_msg(sqlite3 *db, uint32_t channel_id, uint32_t author_id, char *content,
                        int *inserted_id) {
  // TODO: make this a 'trace' log
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
  // TODO: error check

  *inserted_id = sqlite3_last_insert_rowid(db);

  sqlite3_finalize(stmt);
  return Q_SUCCESS;
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
  return Q_SUCCESS;
}

query_result get_user_by_username(sqlite3 *db, const char *username, user *user) {
  static const char *query =
      "SELECT user_id, username, password, email "
      "FROM users "
      "WHERE username = ?;";
  int rc;
  sqlite3_stmt *res;
  rc = sqlite3_prepare_v2(db, query, -1, &res, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
  }
  printf("username %s\n", username);

  // bind parameters
  sqlite3_bind_text(res, 1, username, -1, SQLITE_STATIC);

  int step = sqlite3_step(res);
  if (step == SQLITE_ROW) {
    user->id = sqlite3_column_int(res, 0);
    user->username = malloc(strlen(sqlite3_column_text(res, 1)) + 1);
    strcpy(user->username, sqlite3_column_text(res, 1));
    user->password = malloc(strlen(sqlite3_column_text(res, 2)) + 1);
    strcpy(user->password, sqlite3_column_text(res, 2));
    user->email = malloc(strlen(sqlite3_column_text(res, 3)) + 1);
    strcpy(user->email, sqlite3_column_text(res, 3));
  } else {
    return Q_DB_ERROR;
  }

  sqlite3_finalize(res);
  return Q_SUCCESS;
}

query_result get_user(sqlite3 *db, uint32_t user_id, user *user) {
  static const char *query =
      "SELECT user_id, username, password, email "
      "FROM users "
      "WHERE user_id = ?;";
  int rc;
  sqlite3_stmt *res;
  rc = sqlite3_prepare_v2(db, query, -1, &res, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
  }

  // bind parameters
  rc = sqlite3_bind_int(res, 1, user_id);

  int step = sqlite3_step(res);
  if (step == SQLITE_ROW) {
    user->id = sqlite3_column_int(res, 0);
    user->username = malloc(strlen(sqlite3_column_text(res, 1)) + 1);
    strcpy(user->username, sqlite3_column_text(res, 1));
    user->password = malloc(strlen(sqlite3_column_text(res, 2)) + 1);
    strcpy(user->password, sqlite3_column_text(res, 2));
    user->email = malloc(strlen(sqlite3_column_text(res, 3)) + 1);
    strcpy(user->email, sqlite3_column_text(res, 3));
  } else {
    return Q_DB_ERROR;
  }

  sqlite3_finalize(res);
  return Q_SUCCESS;
}

query_result update_presence(sqlite3 *db, uint32_t user_id, bool status) {
  static const char *query =
      "INSERT INTO presence(user_id, is_online, last_seen_at) "
      "VALUES (?, ?, UNIXEPOCH()) "
      "ON CONFLICT(user_id) DO UPDATE SET "
      "is_online=is_online, last_seen_at=UNIXEPOCH();";

  int rc;
  sqlite3_stmt *res;
  rc = sqlite3_prepare_v2(db, query, -1, &res, 0);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
  }

  rc = sqlite3_bind_int(res, 1, user_id);
  rc = sqlite3_bind_int(res, 2, (int)status);
  int step = sqlite3_step(res);

  sqlite3_finalize(res);
  return Q_SUCCESS;
}
