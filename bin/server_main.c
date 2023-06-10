#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../server/server.h"
#include "state_handling.h"
#include "queries.h"
#include "../deps/kitc/include/kitc.h"

#include <sys/stat.h>

// networking
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in
#include <time.h>
#include <unistd.h>

// temporary
#include "sqlite3.h"

int main() {
  // init our server state
  server_state server_state;
  server_init(&server_state);

  char send_buf[1024];
  time_t ticks;
  
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
        return 1;
    }

  server_state.db = db; // give the server state a pointer to the sqlite connection

  // test some db queries !
  sqlite_version(db);

  // insert a new message into the db
  char *hw = "hello, world!";
  insert_msg(db, 1, 1, hw);

  // fetch all msgs in the db and store in a dynamic array provided by kitc
  kitc_darray *messages = kitc_darray_new(sizeof(message), 2);
  int msg_len = get_msgs_in_channel(db, 1, NULL, NULL, messages);
  printf("number of messages in db: %d\n", msg_len);
  for (int i; i < msg_len; ++i) {
    message *msg = &((message *)messages->data)[i];
    printf("msg id: %d msg content: %s\n", msg->id, msg->contents);
  }

  // tcp setup
  // based on https://gist.github.com/browny/5211329
  int listenfd, connfd;
  struct sockaddr_in serv_addr;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, 0, sizeof(serv_addr));
  memset(send_buf, 0, sizeof(send_buf));

  serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  listen(listenfd, 10);

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

  sqlite3_close(db);
  server_shutdown(&server_state);
}
