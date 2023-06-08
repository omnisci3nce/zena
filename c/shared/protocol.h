#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include "model.h"

struct sync_msgs {
  uint32_t channel_id;
  uint32_t latest_msg_id;
};

struct send_msg {
  message msg;
};

union packet_payload {
    struct sync_msgs sync_msgs;
    struct send_msg send_msg;
};

typedef enum opcode {
  SEND_MSG = 0x02,  // send a message
  SYNC_MSGS = 0x04, // sync client with latest messages
  PRESENCE = 0x06   // update the 'presence' of a user
} opcode;

typedef struct packet {
  opcode type;
  union packet_payload data;
} packet;

#endif
