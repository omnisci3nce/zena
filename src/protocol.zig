//! This module defines the Zena protocol
//!
//! The protocol is a request/response protocol
//! with a custom wire format (i.e. not using HTTP)

pub const MsgType = enum {
    FetchMsgs,
    SendMsg,
};

pub const Message = struct {
    /// This is the unique ID for the message IN the relevant
    /// channel. IDs across channels are not unique
    id: u32,
    /// author ID - this is unique across a single server instance.
    /// (multi-server/federation is a non-goal)
    author: u32
    // /// message contents string
    // FIXME: contents: []const u8,
};


// takes a Message and returns a slice of bytes to send over the wire
// pub fn serialise_message(msg: Message) []const u8 {
//     _ = msg;
// }

//! string encoding
//! | len(u32) | data |

/// takes a slice of bytes and returns a Message
pub fn deserialise_message(buffer: []u8) Message {
    var current_idx: usize = 0;

    var id = std.mem.readIntNative(u32, buffer[current_len]); // read 4 bytes
    current_len += @sizeOf(u32);

    var author = std.mem.readIntNative(u32, buffer[current_len]); // read 4 bytes
    current_len += @sizeOf(u32);

    var string_len = std.mem.readIntNative(u32, buffer[current_len]); // read 4 bytes
    current_len += @sizeOf(u32);

    _ = buffer;
    return Message{
        .id = id,
        .author = author,
        // TODO: contents
    };
}

test "hello world" {
    var msg_bytes = [_]u8 { 0, 0, 0, 1, 0, 0, 0, 2, 0, 0,0,3 };
    var msg = deserialise_message(msg_bytes);


    try expect(msg.id == 1);
    try expect(msg.author == 2);
}
