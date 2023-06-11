#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pack.h"

// we will need a **serialise** and a **deserialise**

int serialise_packet(packet *p, uint8_t *output_buf) {
  uint32_t current_len = 0;
  uint32_t op = SEND_MSG;
  memcpy(output_buf, &op, 4);
  current_len += 4;
  // memcpy(output_buf, &len, 4);
  //  length
  current_len += 4;

  switch (p->header.type) {
    case AUTH: {
      // pack u32 p->data.authenticate.user_id
      memcpy(output_buf, &p->data.authenticate.user_id, 4);
      // pack string
      int str_len = strlen(p->data.authenticate.password);
      memcpy(output_buf + 4, p->data.authenticate.password, str_len + 1);
      return 4 + 1 + str_len;
    }
    case SEND_MSG: {
      // pack u32 id
      memcpy(output_buf + current_len, &p->data.send_msg.msg.id, 4);
      current_len += 4;
      // pack u32 author
      memcpy(output_buf + current_len, &p->data.send_msg.msg.author, 4);
      current_len += 4;
      // pack u32 id
      memcpy(output_buf + current_len, &p->data.send_msg.msg.channel, 4);
      current_len += 4;

      uint32_t str_len = strlen(p->data.send_msg.msg.contents);
      str_len++;
      // pack string length
      memcpy(output_buf + current_len, &str_len, 4);
      current_len += 4;
      // pack string
      strcpy(output_buf + current_len, p->data.send_msg.msg.contents);
      current_len += str_len + 1;
      break;
    }
    default:
      return -1;
  }
  memcpy(output_buf + 4, &current_len, 4);
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

  printf("packet len: %d packet op: %d\n", len, opcode);
  switch (opcode) {
    case SEND_MSG:
      p->data.send_msg.msg.id = unpack_u32(&current_ptr);
      p->data.send_msg.msg.author = unpack_u32(&current_ptr);
      p->data.send_msg.msg.channel = unpack_u32(&current_ptr);
      // malloc a string
      uint32_t contents_len = unpack_u32(&current_ptr);
      printf("contents len: %d\n", contents_len);
      char *contents = malloc((contents_len) * sizeof(char));  // + 1 for '\0' terminator
      strcpy(contents, current_ptr);
      // contents[contents_len] = '\0';
      p->data.send_msg.msg.contents = contents;  // this packet now owns this memory
      break;
    default:
      break;
  }

  return 0;
}
