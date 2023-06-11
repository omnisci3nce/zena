#include "server.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../shared/protocol.h"
#include "network.h"
#include "sqlite3.h"
#include "state_handling.h"

// init server state, connect to sqlite db
void server_init(server_state *s) {
  printf("init server state\n");
  // initialise struct data
  memset(&s->clients, 0, MAX_CONCURRENT_CLIENTS * sizeof(client));
  memset(&s->channels, 0, MAX_CHANNELS * sizeof(channel));
  s->ch_len = 0;
  memset(&s->messages, 0, 1024 * sizeof(message));
  s->msg_len = 0;

  // db setup
  sqlite3 *db;
  sqlite3_stmt *res;

  const char *db_path = "db/db.sqlite";

  struct stat st;
  stat(db_path, &st);
  printf("db size: %ld bytes\n", st.st_size);

  int rc = sqlite3_open(db_path, &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    // return 1;
  }

  s->db = db;  // give the server state a pointer to the sqlite connection

  for (int i = 0; i < MAX_CONCURRENT_CLIENTS + 1; i++) {
    s->fds[i].fd = -1;
  }
  s->fd_count = 0;
}

bool add_to_pfds(server_state *s, int newfd) {
  if (s->fd_count == MAX_CONCURRENT_CLIENTS + 1) {
    return false;
  }

  s->fds[s->fd_count].fd = newfd;
  s->fds[s->fd_count].events = POLLIN;
  s->fd_count++;
  return true;
}

void server_start(server_state *s) {
  int listenfd = create_listen_socket();
  s->fds[0].fd = listenfd;
  s->fds[0].events = POLLIN;
  s->fd_count = 1;

  char remoteIP[INET6_ADDRSTRLEN];
  char buf[1024];  // Buffer for client data
  while (1) {
    printf("starting poll()\n");
    int poll_count = poll(s->fds, s->fd_count, -1);
    if (poll_count == -1) {
      perror("poll");
      exit(1);
    }

    // Run through the existing connections looking for data to read
    for (int i = 0; i < s->fd_count; i++) {
      // Check if someone's ready to read
      if (s->fds[i].revents & POLLIN) {  // We got one!!

        if (s->fds[i].fd == listenfd) {
          // If listener is ready to read, handle new connection

          struct sockaddr_storage remoteaddr;
          socklen_t addrlen = sizeof(remoteaddr);
          int newfd = accept(listenfd, (struct sockaddr *)&remoteaddr, &addrlen);

          if (newfd == -1) {
            perror("accept");
          } else {
            add_to_pfds(s, newfd);

            printf(
                "pollserver: new connection from %s on "
                "socket %d\n",
                inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr *)&remoteaddr),
                          remoteIP, INET6_ADDRSTRLEN),
                newfd);
          }
        } else {
          // If not the listener, we're just a regular client
          int nbytes = recv(s->fds[i].fd, buf, sizeof buf, 0);

          int sender_fd = s->fds[i].fd;

          // lookup associated client struct for this socket fd

          if (nbytes <= 0) {
            // Got error or connection closed by client
            if (nbytes == 0) {
              // Connection closed
              printf("pollserver: socket %d hung up\n", sender_fd);
            } else {
              perror("recv");
            }

            close(s->fds[i].fd);  // Bye!

            // del_from_pfds(pfds, i, &fd_count);
            s->fds[i].fd = -1;

          } else {
            // We got some good data from a client
            printf("Received %d bytes from socket %d\n%s\n", nbytes, s->fds[i].fd, buf);
            for (int j = 0; j < s->fd_count; j++) {
              // Send to everyone!
              int dest_fd = s->fds[j].fd;

              // Except the listener and ourselves
              if (dest_fd != listenfd && dest_fd != sender_fd) {
                if (send(dest_fd, buf, nbytes, 0) == -1) {
                  perror("send");
                }
              }
            }
          }
        }  // END handle data from client
      }    // END got ready-to-read from poll()
    }      // END looping through file descriptors
  }
}
