#include "state_handling.h"

#include <stdio.h>
#include <sys/socket.h>

#include "../shared/protocol.h"
#include "../shared/queries.h"
#include "server.h"

uint32_t oldest_msg_id_in_cache(channel *ch){};

void handle_packet(server_state *s, client *c, packet *p) {
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
      message msg = p->data.send_msg.msg;
      printf("msg contents: %s\n", msg.contents);
      printf("inserting new message into the database\n");

      // insert into database
      uint32_t id = (uint32_t)insert_msg(s->db, msg.channel, msg.author, msg.contents);
      p->data.send_msg.msg.id = id;
      uint8_t buf[1024];
      int len = serialise_packet(p, buf);

      // broadcast msg to all connected clients
      // TODO: broadcast_msg(); // this will queue it to be written to each clients write buffer
      for (int j = 0; j < s->clients_len; j++) {
        int dest_fd = s->clients[j].socket_fd;

        // Except the listener and ourselves
        if (dest_fd != s->fds[0].fd && dest_fd != c->socket_fd) {
          if (send(dest_fd, buf, len, 0) == -1) {
            perror("send");
          }
        }
      }
      break;
    }
    default:
      fprintf(stderr, "Unhandled cmd_type");
      break;
  }
}
