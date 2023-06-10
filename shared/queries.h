#ifndef QUERIES_H
#define QUERIES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sqlite3.h"
#include "kitc.h"

typedef enum query_result {
  Success,
  ValidationError,
  DBError
} query_result;

query_result sqlite_version(sqlite3 *db);

int get_all_msgs(sqlite3 *db, kitc_darray *msg_array);
query_result insert_msg(sqlite3 *db, uint32_t channel_id, uint32_t author_id, char *content);

#endif
