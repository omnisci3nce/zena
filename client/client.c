#include "client.h"

#include <arpa/inet.h>   // inet_addr
#include <netinet/in.h>  // sockaddr_in
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "protocol.h"

void client_init(client_state *c) { printf("init client state\n"); }

bool client_connect(client_state *c, const char *address, int port) {
  // TCP stufffffz
  int sockfd;
  struct sockaddr_in server;

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(5000);

  // struct pollfd pfds[1];  // More if you want to monitor more
  c->pfds[0].fd = sockfd;
  c->pfds[0].events = POLLIN;  // Tell me when ready to read

  c->sockfd = sockfd;

  if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return false;
  }
}

bool client_handshake(client_state *c, const char *username, const char *password) {
  // create AUTH packet
  // TODO(omni): Make helper functions to create packets
  packet p = {.header = {.type = AUTH},
              .data.authenticate = {.username = username, .password = password}};
  printf("authenticating to server with username, password %s %s\n", username, password);

  int len = serialise_packet(&p, c->write_buf);

  printf("packet len %d\n", len);
  for (int i = 0; i < len; i++) {
    printf("%d ", c->write_buf[i]);
  }

  // send AUTH packet
  int sent = send(c->sockfd, c->write_buf, len, 0);
  printf("sent AUTH packet %d bytes got sent\n", sent);

  return true;
}

int client_run(client_state *c) {
  while (1) {
    int poll_count = poll(c->pfds, 1, -1);
    if (poll_count == -1) {
      perror("poll");
      exit(1);
    }

    if (c->pfds[0].revents & POLLIN) {
      int nbytes = recv(c->sockfd, c->read_buf, sizeof(c->read_buf), 0);
      if (nbytes <= 0) {
        // Got error or connection closed by client
        if (nbytes == 0) {
          // Connection closed
          printf("pollserver: socket hung up\n");
        } else {
          perror("recv");
        }

        close(c->sockfd);  // Bye!
        break;

      } else {
        printf("Received %d bytes from server\n%s\n", nbytes, c->read_buf);

        while (nbytes > 0) {
          packet p;
          int bytes_consumed = deserialise_packet(c->read_buf, &p);
          if (bytes_consumed <= 0) {
            return -1;
          }
          client_handle_packet(c, &p);
          nbytes -= bytes_consumed;
        }
      }
    }
  }
}
