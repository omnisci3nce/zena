# Zena

Lightweight messenging (as in chat) server and client implementations in C. Heavily **WIP** as this is just an hour here
and there kind of project.

## contributing

### cloning

`git clone --recurse-submodules https://github.com/omnisci3nce/zena.git`

### database migrations

I've been using the [dbmate](https://github.com/amacneil/dbmate) utility for running
database migrations. From `zena/` run `dbmate create` and `dbmate up` to run migrations.

## goals

Goals:
* simple setup and compilation to make contributing easy
* lightweight
* support screenshare (maybe via OBS somehow?)
* simple GUI using [orca](https://orca-app.dev/index.html)

Non-goals (subject to change):
* decentralisation & federation

## Running list of things we need

- hash table (coming to `kitc` soon tm)
- pool abstraction (for `client`s)
- ring buffer
- logger
