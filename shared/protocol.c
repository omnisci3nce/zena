#include <stdio.h>
#include <string.h>
#include "pack.h"
#include "protocol.h"

// we will need a **serialise** and a **deserialise**

int serialise_packet(packet *p, uint8_t *output_buf) {

  switch (p->header.type) {
    case AUTH: {
      // pack u32 p->data.authenticate.user_id
      memcpy( output_buf,&p->data.authenticate.user_id, 4);
      // pack string
      int str_len = strlen(p->data.authenticate.password);
      memcpy( output_buf+4, p->data.authenticate.password, str_len+1);
      return 4+1+str_len;
    }
    default:
      return -1;
  }
}

int deserialise_packet(uint8_t *data_buffer, packet *p) {
  // unpack the header
  memset(p, 0, sizeof(packet)); // reset packet to zero in-case caller didn't initialise to zero
  const uint8_t *current_ptr = data_buffer;

  uint32_t len = unpack_u32(&current_ptr);
  uint8_t opcode = *current_ptr;
  current_ptr++;

  printf("packet len: %d packet op: %d\n", len, opcode);

  return 0;
}
