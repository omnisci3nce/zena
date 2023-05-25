// This module defines the Zena protocol
//
// The protocol is a request/response protocol
// with a custom wire format (i.e. not using HTTP)
const std = @import("std");

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
    author: u32,
    // /// message contents string
    // FIXME: contents: []const u8,
};

// takes a Message and returns a slice of bytes to send over the wire
// pub fn serialise_message(msg: Message) []const u8 {
//     _ = msg;
// }

// string encoding
// | len(u32) | data |

/// takes a slice of bytes and returns a Message
pub fn deserialise_message(buffer: []const u8) Message {
    var current_idx: usize = 0;

    var id = std.mem.readIntSliceNative(u32, buffer[current_idx..]); // read 4 bytes
    current_idx += @sizeOf(u32);

    var author = std.mem.readIntSliceNative(u32, buffer[current_idx..]); // read 4 bytes
    current_idx += @sizeOf(u32);

    return Message{
        .id = id,
        .author = author,
        // TODO: contents
    };
}

const expect = std.testing.expect;
test "hello world" {
    var msg_bytes = [_]u8{ 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0 };
    var msg = deserialise_message(&msg_bytes);
    std.debug.print("msg id {d}\n", .{msg.id});

    try expect(msg.id == 1);
    try expect(msg.author == 2);
}
