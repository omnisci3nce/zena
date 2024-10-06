# Zena

Lightweight messenging (as in chat) server. Clients will come in later.

## goals

Goals:

- simple setup and compilation to make contributing easy
- lightweight
- support screenshare (maybe via OBS somehow?)
- simple client guis

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
