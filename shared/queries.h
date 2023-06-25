#ifndef QUERIES_H
#define QUERIES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "kitc.h"
#include "model.h"
#include "sqlite3.h"

typedef enum query_result { Q_SUCCESS, Q_VALIDATION_ERR, Q_DB_ERROR } query_result;

query_result sqlite_version(sqlite3 *db);

/** returns array of messages
 * @param from get messages after this message id. set to NULL to get from first
 * @param to get messages up until this message id. set to NULL to get until latest
 */
query_result get_msgs_in_channel(sqlite3 *db, uint32_t channel_id, uint32_t from, uint32_t to,
                                 kitc_darray *msg_array);
// get a single user by their user ID
query_result get_user(sqlite3 *db, uint32_t user_id, user *user);
// get a single user by their username
query_result get_user_by_username(sqlite3 *db, const char *username, user *user);
query_result insert_new_msg(sqlite3 *db, uint32_t channel_id, uint32_t author_id, char *content,
                            int *inserted_id);
query_result update_presence(sqlite3 *db, uint32_t user_id, bool status);

void seed_demo_data(sqlite3 *db);

#endif
