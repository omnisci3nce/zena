#ifndef NETWORK_H
#define NETWORK_H

#include <string.h>
// networking
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in
#include <unistd.h>


int create_listen_socket() {
  // based on https://gist.github.com/browny/5211329
  int listenfd;
  struct sockaddr_in serv_addr;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  listen(listenfd, 10);
  return listenfd;
}

int start_loop() {

  while(1)
	{
		/* In the call to accept(), the server is put to sleep and when for an incoming
		 * client request, the three way TCP handshake* is complete, the function accept()
		 * wakes up and returns the socket descriptor representing the client socket.
		 */
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		/* As soon as server gets a request from client, it prepares the date and time and
		 * writes on the client socket through the descriptor returned by accept()
		 */
		ticks = time(NULL);
		snprintf(send_buf, sizeof(send_buf), "%.24s\r\n", ctime(&ticks));
		write(connfd, send_buf, strlen(send_buf));

		close(connfd);
		sleep(1);
	}
}

#endif
