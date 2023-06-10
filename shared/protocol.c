#include <stdio.h>
#include <string.h>
#include "protocol.h"

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
