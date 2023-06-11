#include "state_handling.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "../shared/protocol.h"
#include "../shared/queries.h"
#include "server.h"

uint32_t oldest_msg_id_in_cache(channel *ch){};
void broadcast_msg(server_state *s, message m);

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
    case MSG: {
      // when server receives a SendMsg we can add it to the database
      message msg = p->data.send_msg.msg;
      printf("msg contents: %s\ninserting new message into the database\n", msg.contents);

      // insert new message into database
      int msg_id;
      query_result q_res =
          (uint32_t)insert_msg(s->db, msg.channel, msg.author, msg.contents, &msg_id);
      p->data.send_msg.msg.id = msg_id;
      uint8_t buf[1024];
      int len = serialise_packet(p, buf);

      // broadcast msg to all connected clients
      broadcast_msg(s, msg);

      free(msg.contents);
      break;
    }
    default:
      fprintf(stderr, "Unhandled cmd_type");
      break;
  }
}

void broadcast_msg(server_state *s, message m) {
  printf("broadcasting message...\n");
  packet p = {.header = {.type = MSG}, .data.send_msg.msg = m};
  uint8_t buf[1024];
  int len = serialise_packet(&p, buf);
  for (int j = 0; j < s->clients_len; j++) {
    int dest_fd = s->clients[j].socket_fd;

    // Except the listener and ourselves
    // TEMP: send back to all clients (acts as echo server as well as broadcast)
    if (dest_fd != s->fds[0].fd) {
      if (send(dest_fd, buf, len, 0) == -1) {
        perror("send");
      }
    }
  }
}
