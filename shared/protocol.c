#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pack.h"

int serialise_packet(packet *p, uint8_t *output_buf) {
  uint8_t *current_ptr = output_buf;
  uint32_t current_len = 0;
  uint32_t op = p->header.type;
  pack_u32(&current_ptr, op);
  current_len += 4;
  // skip length because we fill it in at the end after computing packet size
  current_len += 4;
  current_ptr += 4;

  switch (p->header.type) {
    case AUTH: {
      current_len += pack_string(&current_ptr, p->data.authenticate.username);
      current_len += pack_string(&current_ptr, p->data.authenticate.password);
      break;
    }
    case MSG: {
      current_len += pack_u32(&current_ptr, p->data.send_msg.msg.id);
      current_len += pack_u32(&current_ptr, p->data.send_msg.msg.author);
      current_len += pack_u32(&current_ptr, p->data.send_msg.msg.channel);
      current_len += pack_string(&current_ptr, p->data.send_msg.msg.contents);
      break;
    }
    case FETCH_MSGS: {
      // pack u32 channel id
      current_len += pack_u32(&current_ptr, p->data.generic_id.id);
      break;
    }
    default:
      return -1;
  }

  uint8_t *packet_len_ptr = output_buf + 4;
  pack_u32(&packet_len_ptr, current_len);
  return current_len;
}

int deserialise_packet(uint8_t *data_buffer, packet *p) {
  // unpack the header
  memset(p, 0, sizeof(packet));  // reset packet to zero in-case caller didn't initialise to zero
  const uint8_t *current_ptr = data_buffer;

  // opcode | length
  uint32_t opcode = unpack_u32(&current_ptr);
  uint32_t len = unpack_u32(&current_ptr);
  p->header.type = opcode;

  printf("packet len: %d packet opcode: %s\n", len, op_to_str[opcode]);
  switch (opcode) {
    case MSG:
      p->data.send_msg.msg.id = unpack_u32(&current_ptr);
      p->data.send_msg.msg.author = unpack_u32(&current_ptr);
      p->data.send_msg.msg.channel = unpack_u32(&current_ptr);
      // allocates a string to hold the message contents
      unpack_string(&current_ptr, p->data.send_msg.msg.contents);
      break;
    case FETCH_MSGS:
      p->data.generic_id.id = unpack_u32(&current_ptr);
      break;
    case AUTH: {
      unpack_string(&current_ptr, &p->data.authenticate.username);
      unpack_string(&current_ptr, &p->data.authenticate.password);
      break;
    }
    default:
      printf("unhandled packet type\n");
      return -1;
  }

  return len;
}
