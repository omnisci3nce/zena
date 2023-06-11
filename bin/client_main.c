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

#include "../shared/protocol.h"
#include "./state_handling.h"
#include "state.h"

int main() {
  /* overall control flow of initial connection and application usage */

  // struct for server connection details
  // open socket and connect to server
  // (future) negotiate TLS
  // client sends STARTUP packet -> server sets up internal state and responds with ACK
  // client sends AUTH -> server creates session key and responds AUTH_ACK / AUTH_NACK

  // -----------------------------

  // init client state
  struct client_state client;
  uint8_t write_buf[1024];
  int len = 0;

  const char *helloworld = "Hello, World!";
  packet p = {.header = {.type = AUTH},
              .data.authenticate = {.user_id = 1, .password = helloworld}};

  len = serialise_packet(&p, write_buf);

  // connect to server
  int sockfd;
  struct sockaddr_in server;

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(5000);

  struct pollfd pfds[1];  // More if you want to monitor more
  pfds[0].fd = sockfd;
  pfds[0].events = POLLIN;  // Tell me when ready to read

  char buf[1024];
  // connect
  if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return 1;
  }
  puts("\nConnected");

  printf("packet len %d\n", len);
  for (int i = 0; i < len; i++) {
    printf("%d ", write_buf[i]);
  }

  int sent = send(sockfd, write_buf, len, 0);
  printf("sent AUTH packet %d bytes got sent\n", sent);

  memset(&p, 0, sizeof(packet));
  p.header.type = FETCH_MSGS;
  struct generic_id chan_id = {.id = 1};
  p.data.generic_id = chan_id;

  len = serialise_packet(&p, write_buf);
  sleep(1);
  sent = send(sockfd, write_buf, len, 0);
  printf("sent FETCH_MSG packet %d bytes got sent\n", sent);

  while (1) {
    int poll_count = poll(pfds, 1, -1);
    if (poll_count == -1) {
      perror("poll");
      exit(1);
    }

    if (pfds[0].revents & POLLIN) {
      int nbytes = recv(sockfd, buf, sizeof buf, 0);
      if (nbytes <= 0) {
        // Got error or connection closed by client
        if (nbytes == 0) {
          // Connection closed
          printf("pollserver: socket hung up\n");
        } else {
          perror("recv");
        }

        close(sockfd);  // Bye!
        break;

      } else {
        printf("Received %d bytes from server\n%s\n", nbytes, buf);

        while (nbytes > 0) {
          packet p;
          uint8_t buffer[1024];
          int bytes_consumed = deserialise_packet(buf, &p);
          if (bytes_consumed <= 0) {
            return -1;
          }
          client_handle_packet(&client, &p);
          nbytes -= bytes_consumed;
        }
      }
    }
  }

  return 0;
}
