#ifndef STATE_HANDLING_H
#define STATE_HANDLING_H

#include "server.h"
#include "../shared/protocol.h"

/** takes a tagged union command, switches on the command type and
 *  updates the server state (both data storage and the state machines associated
 *  with clients / channels)
*/
void handle_command(server_state *s, command *cmd);

#endif
