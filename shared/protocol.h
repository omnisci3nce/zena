#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <assert.h>
#include <stdint.h>

#include "model.h"

struct sync_msgs {
  uint32_t channel_id;
  uint32_t latest_msg_id;
};

struct send_msg {
  message msg;
};

// client sends to authenticate with server
struct authenticate {
  const char *username;
  const char *password;
};

struct auth_ack {
  uint32_t user_id;
  const char *session_key;
};

struct generic_id {
  uint32_t id;
};

union packet_payload {
  // struct startup startup;
  struct sync_msgs sync_msgs;
  struct send_msg send_msg;
  struct authenticate authenticate;
  struct generic_id generic_id;
};

typedef enum opcode {
  AUTH = 0x01,       // client authenticate with server
  MSG = 0x02,        // send a message
  SYNC_MSGS = 0x04,  // sync client with latest messages
  PRESENCE = 0x06,   // update the 'presence' of a user
  FETCH_MSGS = 0x0A,
  HEARTBEAT = 0x90,  // im alive!
  STARTUP = 0xAA,
  AUTH_ACK,
} opcode;

static const char *const op_to_str[] = {[AUTH] = "AUTH",
                                        [MSG] = "MSG",
                                        [SYNC_MSGS] = "SYNC_MSGS",
                                        [PRESENCE] = "PRESENCE",
                                        [FETCH_MSGS] = "FETCH_MSGS",
                                        [HEARTBEAT] = "HEARTBEAT",
                                        [STARTUP] = "STARTUP",
                                        [AUTH_ACK] = "AUTH_ACK"};

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
