#include <stdint.h>
#include "server.h"
#include "../shared/protocol.h"

uint32_t oldest_msg_id_in_cache(channel *ch) {};

void handle_command(server_state *s, command *cmd) {
  switch (cmd->type) {
    case SyncMsgs: {
      channel ch = s->channels[cmd->data.sync_msgs.channel_id];
      uint32_t client_last_msg = cmd->data.sync_msgs.latest_msg_id;
      if (client_last_msg == ch.last_msg_id ) {
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
    case SendMsg:
      break;
    default:
      break;
  }
}
