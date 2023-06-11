#ifndef NETWORK_H
#define NETWORK_H

#include <string.h>
// networking
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// create socked fd for the server on port 5000 and bind + listen
int create_listen_socket() {
  // based on https://gist.github.com/browny/5211329
  int listenfd;
  int yes = 1;
  struct sockaddr_in serv_addr;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  listen(listenfd, 10);
  return listenfd;
}

#endif
