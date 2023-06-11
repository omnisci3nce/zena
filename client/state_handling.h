#ifndef CLIENT_STATE_HANDLING_H
#define CLIENT_STATE_HANDLING_H

#include "../shared/protocol.h"
#include "../shared/queries.h"
#include "state.h"

void client_handle_packet(client_state *c, packet *p);

#endif // !CLIENT_STATE_HANDLING_H
