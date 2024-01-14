/* overall control flow of initial connection and application usage */
  // struct for server connection details
  // open socket and connect to server
  // (future) negotiate TLS
  // client sends STARTUP packet -> server sets up internal state and responds with ACK
  // client sends AUTH -> server creates session key and responds AUTH_ACK / AUTH_NACK

Print out a bunch of messages

```c
  // for (int i; i < msg_len; ++i) {
  //   message *msg = &((message *)messages->data)[i];
  //   printf("msg: %d channel: %d author: %d msg content: %s\n", msg->id, msg->channel,
  //   msg->author,
  //          msg->contents);
  // }
```
