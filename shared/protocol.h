#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <assert.h>
#include "model.h"

struct sync_msgs {
  uint32_t channel_id;
  uint32_t latest_msg_id;
};

struct send_msg {
  message msg;
};

struct authenticate {
  uint32_t user_id;
  const char *password;
};

union packet_payload {
    struct sync_msgs sync_msgs;
    struct send_msg send_msg;
    struct authenticate authenticate;
};

typedef enum opcode {
  SEND_MSG = 0x02,  // send a message
  SYNC_MSGS = 0x04, // sync client with latest messages
  PRESENCE = 0x06,  // update the 'presence' of a user
  HEARTBEAT = 0x90,
  AUTH = 0xA0
} opcode;

static const char* const op_to_str[] = {
  [SEND_MSG] = "SEND_MSG"
};

// for now this will just use bytes but later we might use some bitfields 
typedef struct packet_header {
  enum opcode type;
} packet_header;

typedef struct packet {
  packet_header header;
  union packet_payload data;
} packet;

// ------ Serialisation

/**
 * @brief bytes -> packet 
 * @param p -> packet struct the result will be stored in
 * @return 0 = success, -1 = error
 */
int deserialise_packet(uint8_t *data_buffer, packet *p);

/**
 * @brief packet -> bytes
 * @return int length of serialised packet in the buffer. returns -1 if error
 */
int serialise_packet(packet *p, uint8_t *output_buf);

// ----- Parser

enum packet_parser_state {
  WAITING_HEADER,
  WAITING_DATA,
};

typedef struct parser {
  enum packet_parser_state state;
} parser;

#endif
