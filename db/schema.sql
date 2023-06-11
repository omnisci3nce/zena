CREATE TABLE IF NOT EXISTS "schema_migrations" (version varchar(128) primary key);
CREATE TABLE channels (
  channel_id integer primary key,

  -- updateable fields
  name text non null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now'))
);
CREATE TABLE users (
  user_id integer primary key,

  -- updateable fields
  username text not null,
  email text, -- optional
  password text not null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now')),

  -- soft delete
  deleted integer not null default 0
);
CREATE TABLE messages (
  msg_id integer primary key,
  author_id integer not null references users(user_id),
  channel_id integer not null references channels(channel_id),
  content text not null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now')),

  -- soft delete
  deleted integer not null default 0
);
CREATE TABLE presence (
  user_id       integer primary key references users(user_id),
  is_online     integer not null check (is_online in (0,1)),
  last_seen_at  integer not null default (strftime('%s', 'now'))
);
-- Dbmate schema migrations
INSERT INTO "schema_migrations" (version) VALUES
  ('20230605102059'),
  ('20230611103026');
