#include <stdio.h>
// #include "client.h"
#include "../shared/protocol.h"
#include "../shared/queries.h"
#include "state_handling.h"

struct client_state {};

void handle_packet(struct client_state *c, packet *p) {
  switch (p->header.type) {
    case SYNC_MSGS: {
      break;
    }
    case SEND_MSG: {
      // client received a message
      message msg = p->data.send_msg.msg;

      // push message into in-memory cache
      // pop oldest message
      // flag data model as updated for rendering
      // insert message into sqlite, delete oldest

      // if there is some multimedia attached to the message, do we need to do
      // anything special?
      break;
    }
    default:
      fprintf(stderr, "Unhandled cmd_type");
      break;
  }
}
