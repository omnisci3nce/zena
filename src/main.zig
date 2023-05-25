const std = @import("std");
const net = std.net;

const proto = @import("protocol.zig");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();

    // In-memory messages
    var msgs = std.ArrayList([]u8).init(allocator);
    _ = msgs;

    // Socket
    const self_addr = try net.Address.resolveIp("127.0.0.1", 3000);
    var listener = net.StreamServer.init(.{});
    try (&listener).listen(self_addr);

    std.log.info("Listening on {}; press Ctrl-C to exit...", .{self_addr});
}

// NOTE(omni): keeping this here in case I want to reference it.
//
// // Prints to stderr (it's a shortcut based on `std.io.getStdErr()`)
// std.debug.print("All your {s} are belong to us.\n", .{"codebase"});
//
// // stdout is for the actual output of your application, for example if you
// // are implementing gzip, then only the compressed bytes should be sent to
// // stdout, not any debugging messages.
// const stdout_file = std.io.getStdOut().writer();
// var bw = std.io.bufferedWriter(stdout_file);
// const stdout = bw.writer();
//
// try stdout.print("Run `zig build test` to run the tests.\n", .{});
//
// try bw.flush(); // don't forget to flush!
