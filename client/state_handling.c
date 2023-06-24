#include "../client/state_handling.h"

#include <stdio.h>
#include <stdlib.h>

#include "../shared/protocol.h"
#include "../shared/queries.h"
#include "client.h"

void client_handle_packet(client_state *c, packet *p) {
  switch (p->header.type) {
    case SYNC_MSGS: {
      break;
    }
    case MSG: {
      // client received a message
      message msg = p->data.send_msg.msg;

      // push message into in-memory cache
      // pop oldest message
      // flag data model as updated for rendering
      // insert message into sqlite, delete oldest

      // if there is some multimedia attached to the message, do we need to do
      // anything special?
      free(msg.contents);
      break;
    }
    default:
      fprintf(stderr, "Unhandled packet type");
      break;
  }
}
