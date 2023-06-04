struct sync_msgs {
  int channel_id;
};

union cmd_payload {
    struct sync_msgs sync_msgs;
};

typedef enum cmd_type {
  SyncMsgs
} cmd_type;

/**
*/
typedef struct command {
  cmd_type type;
  union cmd_payload data;
} command;
