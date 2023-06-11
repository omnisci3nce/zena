#ifndef MODEL_H
#define MODEL_H

#include <stdint.h>

typedef struct message {
  /** @brief This is the unique ID for the message IN the relevant 
   * channel. IDs across channels are not unique */
  uint32_t id;
  /** @brief author ID - this is unique across a single server instance.
   * (multi-server/federation is a non-goal) */
  uint32_t author;
  uint32_t channel;
  /** @brief message contents string */
  char *contents;
} message;

typedef struct user {
  uint32_t id;
  const char *username;
  const char *password;
  const char *email;
} user;

#endif
