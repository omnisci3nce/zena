CREATE TABLE IF NOT EXISTS "schema_migrations" (version varchar(128) primary key);
CREATE TABLE channels (
  channel_id integer primary key,

  -- updateable fields
  name text non null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now'))
);
CREATE TABLE messages (
  msg_id integer primary key,
  author_id integer not null references users(user_id),
  channel_id integer not null references channels(channel_id),
  content text not null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now'))
);
CREATE TABLE users (
  user_id integer primary key,

  -- updateable fields
  email text, -- optional

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now')),

  -- soft delete
  deleted integer not null default 0
);
-- Dbmate schema migrations
INSERT INTO "schema_migrations" (version) VALUES
  ('20230605102059');
