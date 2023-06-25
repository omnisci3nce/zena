// TODO: Nuklear

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "client.h"
#include "model.h"
#include "protocol.h"
#include "raylib.h"

// temporarily use a global messages variable
message messages[64];
int msg_count = 0;

void run_client();

int main(void) {
  // dummy message
  message m = {.id = 1, .author = 1, .channel = 1, .contents = "hello world!"};
  messages[0] = m;
  msg_count++;

  // setup our state
  pthread_t client_thread;

  int iret = pthread_create(&client_thread, NULL, run_client, NULL);

  InitWindow(800, 450, "raylib [core] example - basic window");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 100, 20, LIGHTGRAY);

    // draw messages
    for (int i = 0; i < msg_count; i++) {
      char msg[1024];
      sprintf(msg, "%d says: '%s'", messages[i].author, messages[i].contents);
      DrawText(msg, 190, 100 + 30 + i * 50, 20, LIGHTGRAY);
    }

    EndDrawing();
  }

  CloseWindow();

  // teardown
  pthread_join(client_thread, NULL);

  return 0;
}

void run_client() {
  char *username = "omni";
  char *password = "secret_password";

  // init client state
  client_state client;
  client_init(&client);

  // connect to server
  client_connect(&client, "", 5000);

  packet p = {0};
  p.header.type = FETCH_MSGS;
  struct generic_id chan_id = {.id = 1};
  p.data.generic_id = chan_id;

  int len = serialise_packet(&p, client.write_buf);
  sleep(1);
  int sent = send(client.sockfd, client.write_buf, len, 0);
  printf("sent %s packet %d bytes got sent\n", op_to_str[p.header.type], sent);

  client_run(&client);
}
