#ifndef CLIENT_STATE_HANDLING_H
#define CLIENT_STATE_HANDLING_H

#include "../shared/protocol.h"
#include "../shared/queries.h"

void handle_packet(struct client_state *c, packet *p);

#endif // !CLIENT_STATE_HANDLING_H
