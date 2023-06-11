#include "state_handling.h"

#include <stdio.h>

#include "../shared/protocol.h"
#include "../shared/queries.h"
#include "server.h"

uint32_t oldest_msg_id_in_cache(channel *ch){};

void handle_packet(server_state *s, packet *p) {
  switch (p->header.type) {
    case SYNC_MSGS: {
      channel ch = s->channels[p->data.sync_msgs.channel_id];
      uint32_t client_last_msg = p->data.sync_msgs.latest_msg_id;
      if (client_last_msg == ch.last_msg_id) {
        // client is already caught up
      } else if (client_last_msg >= oldest_msg_id_in_cache(&ch)) {
        // client can be caught up from the cache
      } else {
        // we're going to have to go to disk (sqlite)
      }

      // for each msg between the last one client has and the latest
      for (int i = client_last_msg; client_last_msg <= ch.last_msg_id; i++) {
        // message m =
        // send msg back to client
        /*
        command msg = {
          .type = SendMsg,
          .data.send_msg = { .msg = m }
        };
        */
      }
      break;
    }
    case SEND_MSG: {
      // when server receives a SendMsg we can add it to the database

      printf("inserting new message into the database\n");
      message msg = p->data.send_msg.msg;

      // insert into database
      insert_msg(s->db, msg.channel, msg.author, msg.contents);

      // broadcast msg to all connected clients
      // TODO: broadcast_msg(); // this will queue it to be written to each clients write buffer

      break;
    }
    default:
      fprintf(stderr, "Unhandled cmd_type");
      break;
  }
}
