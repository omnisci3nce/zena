#include "state_handling.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../shared/protocol.h"
#include "../shared/queries.h"
#include "model.h"
#include "server.h"

uint32_t oldest_msg_id_in_cache(channel *ch){};
void broadcast_msg(server_state *s, message m);

void handle_packet(server_state *s, client *c, packet *p) {
  switch (p->header.type) {
    case MSG: {
      // when server receives a MSG we can add the new message into the database
      message msg = p->data.send_msg.msg;
      printf("received msg with contents: %s\ninserting new message into the database\n",
             msg.contents);

      // insert new message into database
      int msg_id;
      query_result q_res = insert_msg(s->db, msg.channel, msg.author, msg.contents, &msg_id);
      msg.id = (uint32_t)msg_id;

      // broadcast msg to all connected clients
      broadcast_msg(s, msg);

      // finally, we will update the presence for the author
      user user;
      q_res = get_user(s->db, msg.author, &user);
      if (q_res == Q_SUCCESS) {
        printf("updating presence of user\n");
        update_presence(s->db, user.id, true);
      }

      free(msg.contents);
      break;
    }
    case AUTH: {
      // client wants to log in
      struct authenticate auth = p->data.authenticate;

      // get the user associated with the id
      user user;
      query_result q_res = get_user(s->db, auth.user_id, &user);
      if (q_res == Q_SUCCESS) {
        printf("found user matching id %d - username: %s\n", user.id, user.username);
        if (strcmp(auth.password, user.password) == 0) {
          printf("the provided password matches!\n");
          // TODO: send a AUTH_ACK
        }
      } else {
        printf("get_user() query failed\n");
      }

      break;
    }
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
