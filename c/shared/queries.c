// functions for querying sqlite database
#include <stdint.h>

typedef enum query_result {
  Success,
  ValidationError,
  DBError
} query_result;

const char* get_all_channels_query = "SELECT * from channels;";

/** returns array of messages
 * @param from get messages after this message id. set to NULL to get from first
 * @param to get messages up until this message id. set to NULL to get until latest
*/
query_result get_msgs_for_channel(uint32_t channel_id, uint32_t from, uint32_t to);

/*
* get_all_channels();
*/
