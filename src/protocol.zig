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
    author: u32,
    /// message contents string
    contents: []u8,
};

pub fn serialise_message(msg: Message) []const u8 {
    _ = msg;
}
