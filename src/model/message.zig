const std = @import("std");
const Allocator = std.mem.Allocator;

pub const Message = struct {
    /// This is the unique ID for the message IN the relevant
    /// channel. IDs across channels are not unique
    id: u32,
    /// author ID - this is unique across a single server instance.
    /// (multi-server/federation is a non-goal)
    author: u32,
    // /// message contents string
    contents: []const u8,
};

// TODO: implement serialise to message
// takes a Message and returns a slice of bytes to send over the wire
// pub fn serialise_message(msg: Message) []const u8 {
//     _ = msg;
// }

// string encoding
// | len(u32) | data |

/// takes a slice of bytes and returns a Message
pub fn deserialise_message(allocator: *const Allocator, buffer: []const u8) !Message {
    var current_idx: usize = 0;

    var id = std.mem.readIntSliceNative(u32, buffer[current_idx..]); // read 4 bytes
    current_idx += @sizeOf(u32);

    var author = std.mem.readIntSliceNative(u32, buffer[current_idx..]); // read 4 bytes
    current_idx += @sizeOf(u32);

    var string_len = std.mem.readIntSliceNative(u32, buffer[current_idx..]); // read 4 bytes
    current_idx += @sizeOf(u32);

    // allocate a string for contents
    const memory = try allocator.alloc(u8, string_len);
    @memcpy(memory, buffer[current_idx .. current_idx + string_len]);

    return Message{ .id = id, .author = author, .contents = memory };
}

const expect = std.testing.expect;
test "hello world" {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    //                    | id: 1    | author    | string len | hello world .... |
    var msg_bytes = [_]u8{ 1, 0, 0, 0, 2, 0, 0, 0, 11, 0, 0, 0, 104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100 };
    var msg = try deserialise_message(&allocator, &msg_bytes);
    // free the contents string
    defer allocator.free(msg.contents);

    std.debug.print("msg id {d}\n", .{msg.id});
    std.debug.print("msg author {d}\n", .{msg.author});
    std.debug.print("msg contents {s}\n", .{msg.contents});

    std.debug.print("msg bytes: {any}\n", .{msg_bytes});

    try expect(msg.id == 1);
    try expect(msg.author == 2);
    try expect(std.mem.eql(u8, msg.contents, "hello world"));
}