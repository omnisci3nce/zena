/** @file model.h
 *  @brief in-memory representations of domain data
 */
#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>
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

typedef enum presence { OFFLINE, ACTIVE, INACTIVE } presence;

typedef struct user {
  uint32_t id;
  const char *username;
  const char *password;
  const char *email;
} user;

// TODO: things like whether mentions are allowed, visibilty, etc
typedef struct channel_options {
  bool at_everyone_ok;
  bool at_here_ok;
} channel_options;

typedef struct channel {
  char *name;
  channel_options options;
  uint32_t last_msg_id;

  // TODO: ring_buffer recent_msgs; // keep each channels most recent messages in memory for
  // fast access
} channel;

#endif
