#ifndef QUERIES_H
#define QUERIES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sqlite3.h>

typedef enum query_result {
  Success,
  ValidationError,
  DBError
} query_result;

query_result get_msgs_for_channel(uint32_t channel_id, uint32_t from, uint32_t to);
query_result insert_msg(sqlite3 *db, uint32_t channel_id, uint32_t author_id, char *content);

#endif
