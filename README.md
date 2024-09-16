# Zena

Lightweight messenging (as in chat) server and client.

## goals

Goals:

- simple setup and compilation to make contributing easy
- lightweight
- support screenshare (maybe via OBS somehow?)
- simple GUI

Non-goals (subject to change):

- decentralisation & federation

## how to run

**compiling protobufs**
```
protoc proto/*.proto \
    --go_out=. \
    --go_opt=paths=source_relative \
    --proto_path=
```