#ifndef STATE_HANDLING_H
#define STATE_HANDLING_H

#include "server.h"
#include "../shared/protocol.h"

void handle_command(server_state *s, command *cmd);

#endif
