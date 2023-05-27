// This module defines the Zena protocol
//
// The protocol is a request/response protocol
// with a custom wire format (i.e. not using HTTP)
const data = @import("data.zig");

pub const Command = union(enum) {
    sync_msgs: SyncMsgsPayload,
    send_msg: SendMsgPayload,
};

pub const Reply = union(enum) {};

pub const SyncMsgsPayload = struct {
    channel_id: u32,
    latest_msg_id: u32,
};

pub const SendMsgPayload = struct { message: data.Message };
