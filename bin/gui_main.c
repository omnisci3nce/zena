// TODO: Nuklear

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "model.h"
#include "protocol.h"
#include "raylib.h"

// forward decls
void run_client();

// temporarily use a global messages variable
message messages[64];
int msg_count = 0;

struct gui_state {
  uint32_t current_channel;
};

int main(void) {
  // dummy message
  message m = {.id = 1, .author = 1, .channel = 1, .contents = "hello world!"};
  messages[0] = m;
  msg_count++;

  // setup our state
  struct gui_state app_state = {.current_channel = 1};
  client_state client;
  pthread_t client_thread;

  InitWindow(800, 450, "Zena - #general");

  int iret = pthread_create(&client_thread, NULL, run_client, &client);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawRectangle(0, 0, 180, 450, LIGHTGRAY);
    DrawText("Zena: Channels", 20, 20, 18, DARKGRAY);

    char status_msg[1024];
    sprintf(status_msg, "Status: %s", client_status_str[client.status]);
    DrawText(status_msg, 550, 20, 18, DARKGRAY);

    // draw messages
    for (int i = 0; i < client.msg_len; i++) {
      char msg[1024];
      sprintf(msg, "%d says: '%s'", client.messages[i].author, client.messages[i].contents);
      DrawText(msg, 190, 100 + 30 + i * 30, 20, DARKGRAY);
    }

    EndDrawing();
  }

  CloseWindow();

  // teardown
  pthread_join(client_thread, NULL);

  return 0;
}

void run_client(client_state *client) {
  char *username = "omni";
  char *password = "secret_password";

  // init client state
  client_init(client);

  // connect to server
  client_connect(client, "", 5000);

  // perform handshake
  client_handshake(&client, username, password);

  client->status = INITIAL_SYNCING;

  packet p = {0};
  p.header.type = FETCH_MSGS;
  struct generic_id chan_id = {.id = 1};
  p.data.generic_id = chan_id;

  int len = serialise_packet(&p, client->write_buf);
  // sleep(1);
  int sent = send(client->sockfd, client->write_buf, len, 0);
  printf("sent %s packet %d bytes got sent\n", op_to_str[p.header.type], sent);

  client_run(client);
}
