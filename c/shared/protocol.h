#include <cstdint>
#include <stdint.h>

typedef struct message {
  /** @brief This is the unique ID for the message IN the relevant 
   * channel. IDs across channels are not unique */
  uint32_t id;
  /** @brief author ID - this is unique across a single server instance.
   * (multi-server/federation is a non-goal) */
  uint32_t author;
  /** @brief message contents string */
  char *contents;
} message;

struct sync_msgs {
  uint32_t channel_id;
  uint32_t latest_msg_id;
};

struct send_msg {
  message msg;
};

union cmd_payload {
    struct sync_msgs sync_msgs;
    struct send_msg send_msg;
};

typedef enum cmd_type {
  SyncMsgs,
  SendMsg
} cmd_type;

typedef struct command {
  cmd_type type;
  union cmd_payload data;
} command;
