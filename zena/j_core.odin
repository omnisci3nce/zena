// Josh's Core experiments

package zena

import "core:encoding/endian"
import "core:encoding/json"
import "core:fmt"
import "core:net"
import "core:testing"
import "core:time"

// A chat message
Message :: struct {
	msg_id:   int, // -1 when creating a new message (client -> server)
	author:   string,
	contents: string,
}

// All chat messages to-date
AllMessages :: struct {
	messages: [dynamic]Message,
}

NetworkMsg :: union {
	Message,
	AllMessages,
}

CommsErr :: union {
	string,
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
	ui:       UiState,
}


core_tick :: proc(core: ^Core) {
	fmt.println("core: tick")
}

core_run :: proc(core: ^Core, socket: net.TCP_Socket) {
	fmt.println("core: boot")
	fmt.println("core: fill initial state before first paint")

	fmt.printfln("core: request all messages from server")
	send_request_messages(socket)
	response, timeout_err := try_receive_message(socket) // blocking receive -> we're looking for the

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
	Comms_ReqMessages = 'M', // request all messages to date
	Comms_AllMessages = 'm', // response holding all messages to date
	Comms_NewMessage  = 'N', // a new message (used for creation client->server, and updates server->client)
}

/* Send a new Message to the {server/client} */
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
	fmt.printf("Sending msg ID %d Length %d\n", msg_type, length)

	net.send_tcp(socket, msg_type[:])
	net.send_tcp(socket, length[:])
	net.send_tcp(socket, json_data)
}

// Sends the ReqMessages to the server
send_request_messages :: proc(socket: net.TCP_Socket) {
	msg_type := transmute([4]u8)u32le(MsgType.Comms_ReqMessages)
	length := [4]u8{0, 0, 0, 0}
	net.send_tcp(socket, msg_type[:])
	net.send_tcp(socket, length[:])
	// no payload for this msg type
}

try_receive_message :: proc(socket: net.TCP_Socket) -> (resp: NetworkMsg, err: CommsErr) {
	recv_buf: [1024]u8 // buffer to load data into
	n_bytes, net_err := net.recv_tcp(socket, recv_buf[:]) // TODO: handle error
	fmt.printf("Received %d bytes!\n", n_bytes)

	msg_type := endian.unchecked_get_u32le(recv_buf[0:4])
	length := endian.unchecked_get_u32le(recv_buf[4:8])

	fmt.printf("Msg ID %d Length %d\n", msg_type, length)

	if MsgType(msg_type) == MsgType.Comms_AllMessages {
		all: AllMessages
		json_err := json.unmarshal(recv_buf[8:], &all)
		fmt.printf("Got %d Messages from the server\n", len(all.messages))
		for msg in all.messages {
			fmt.printf(
				"\tID %d - Author %s - Contents '%s'\n",
				msg.msg_id,
				msg.author,
				msg.contents,
			)
		}
		return NetworkMsg(all), nil
	} else {
		return NetworkMsg{}, CommsErr("Oh no")
	}
}

// Fetches all the messages
fetch_all :: proc() -> [dynamic]Message {
	// NOTE: Temporarily return a hard-coded array of messages for testing purposes
	msgs: [dynamic]Message
	append(
		&msgs,
		Message {
			msg_id = 1,
			author = "Angry Andrew",
			contents = "Hey, where are you? I've been waiting here 18 hours 43 minutes and 22 seconds!",
		},
	)
	return msgs
}

@(test)
test_receive_message :: proc(t: ^testing.T) {

}

// --- Entrypoint

// main :: proc() {
//   core: Core

//   // start the core
//   core_run(&core)
// }
