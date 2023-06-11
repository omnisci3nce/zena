#include "../shared/protocol.h"
#include <stdio.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_addr
#include <time.h>
#include <unistd.h>

int main() {
  /* overall control flow of initial connection and application usage */

  // struct for server connection details
  // open socket and connect to server
  // (future) negotiate TLS
  // client sends STARTUP packet -> server sets up internal state and responds with ACK
  // client sends AUTH -> server creates session key and responds AUTH_ACK / AUTH_NACK

  // -----------------------------

  // init client state
  uint8_t write_buf[1024];
  int len = 0;

  uint32_t my_id = 4;
  const char *passwd = "my_cool_password";
  const char *helloworld = "helloworld";

  packet p = {
    .header = { .type = SEND_MSG },
    .data.send_msg = {
      .msg = {
        .id = 1,
        .author = 1,
        .channel = 1,
        .contents = helloworld
      }
    }
  };

  len = serialise_packet(&p, write_buf);

  // connect to server
  int sockfd;
  struct sockaddr_in server;

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 5000 );

  // connect
	if (connect(sockfd , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

  printf("packet len %d\n", len);
  for (int i = 0; i < len; i++) {
    printf("%d ", write_buf[i]);
  }
	
	puts("\nConnected");
  // send packet from write buf
  send(sockfd, write_buf, len, 0);
	//printf("Sent packet User ID: %d Password: %s\n", p.data.authenticate.user_id, p.data.authenticate.password);
  printf("sent SEND_MSG packet\n");
  
  return 0;
}
