// Josh's Core experiments

package zena

import "core:fmt"
import "core:time"

// --- Types and State

Message :: struct {
  msg_id: int,
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
  
}

core_run :: proc(core: ^Core) {
  fmt.println("core: boot")
  fmt.println("core: fill initial state before first paint")
  core.messages = fetch_all()
  core.ui.messages = core.messages[:]

  for {
    fmt.println("core: tick")
    // here is where we could asynchronously handle any incoming network requests or user input
    time.sleep(3 * time.Second)
  }
}


// --- Comms

// Fetches all the channels (and msgs within each channel)
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

main :: proc() {
  core: Core
  
  // start the core
  core_run(&core)
}