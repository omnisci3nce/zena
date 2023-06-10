#include "shared/protocol.h"
#include <stdio.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_addr
#include <time.h>
#include <unistd.h>

int main() {
  // init client state
  uint8_t write_buf[1024];
  int len = 0;

  uint32_t my_id = 4;
  const char *passwd = "my_cool_password";

  packet p = {
    .type = AUTH,
    .data.authenticate = {
      .user_id = my_id,
      .password = passwd
    }
  };

  len = serialise_packet(&p, write_buf);

  // connect to server
  int sockfd;
  struct sockaddr_in server;

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

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
	printf("Sent packet User ID: %d Password: %s\n", p.data.authenticate.user_id, p.data.authenticate.password);
  
  return 0;
}
