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
  /** @brief message contents string */
  char *contents;
} message;

#endif