# Zena

_Note: this readme is written in the present tense but the project is in its infancy and thus can be taken as the future tense, implying the desired state of the project, instead._

This project defines a specification for a lightweight server<->client chat application, and a server implementation in Go.

Data between client and server is transferred via gRPC using protobufs, therefore clients can be implemented in various languages
provided they can talk protobuf and gRPC.

A full gui client implementation may be added here later but I'd like to support at least a basic web gui or something.

## goals

Goals:

- simple setup and compilation to make contributing easy
- lightweight
- support screenshare
- simple client guis by way of simple communications between server & client

Non-goals (subject to change):

- decentralisation & federation

## how to run

`go run cmd/server/server.go `
