// Josh's Core experiments

package zena

import "core:fmt"
import "core:time"
import "core:net"
import "core:encoding/json"

// --- Types and State


// A chat message
Message :: struct {
  msg_id: int, // -1 when creating a new message (client -> server)
  author: string,
  contents: string,
}

// State that is synchronised with the UI
UiState :: struct {
  messages: []Message,
}

// --- Core

Core :: struct {
  // domain data
  messages: [dynamic]Message,
  // TODO: incoming queues
  // state slices
  ui: UiState,
}


core_tick :: proc(core: ^Core) {
  fmt.println("core: tick")
}

core_run :: proc(core: ^Core) {
  fmt.println("core: boot")
  fmt.println("core: fill initial state before first paint")
  core.messages = fetch_all()
  core.ui.messages = core.messages[:]

  for {
    core_tick(core)
    // here is where we could asynchronously handle any incoming network requests or user input
    time.sleep(3 * time.Second)
  }
}


// --- Comms

MsgType :: enum u32 {
  Comms_ReqMessages   = 'M', // request all messages to date
  Comms_AllMessages   = 'm', // response holding all messages to date
  Comms_NewMessage    = 'N', // a new message (used for creation client->server, and updates server->client)
}

/* Send a Message to the {server/client} */
send_message :: proc(socket: net.TCP_Socket, message: Message) {
  // construct the wire format for the Message
  // [type][len][json]

  json_data, err := json.marshal(message)
  if err != nil {
    fmt.eprintln("Error marshalling message: ", err)
    return
  }

  msg_type := transmute([4]u8)u32le(MsgType.Comms_NewMessage)
  length := transmute([4]u8)u32le(len(json_data))

  net.send_tcp(socket, msg_type[:])
  net.send_tcp(socket, length[:])
  net.send_tcp(socket, json_data)
}

send_request_messages :: proc(socket: net.TCP_Socket) {
  msg_type := transmute([4]u8)u32le(MsgType.Comms_ReqMessages)
  length := [1]u8{0}
  net.send_tcp(socket, msg_type[:])
  net.send_tcp(socket,length[:])
  // no payload for this msg type
}

// Fetches all the messages
fetch_all :: proc() -> [dynamic]Message {
  // NOTE: Temporarily return a hard-coded array of messages for testing purposes
  msgs: [dynamic]Message
  append(&msgs, Message{
    msg_id = 1,
    author = "Angry Andrew",
    contents = "Hey, where are you? I've been waiting here 18 hours 43 minutes and 22 seconds!",
  })
  return msgs
}

// --- Entrypoint

// main :: proc() {
//   core: Core
  
//   // start the core
//   core_run(&core)
// }