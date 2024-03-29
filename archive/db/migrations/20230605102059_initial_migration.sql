-- migrate:up

create table channels (
  channel_id integer primary key,

  -- updateable fields
  name text non null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now'))
);

create table users (
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

create table messages (
  msg_id integer primary key,
  author_id integer not null references users(user_id),
  channel_id integer not null references channels(channel_id),
  content text not null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now')),

  -- soft delete
  deleted integer not null default 0 -- 0 = not deleted, 1 = deleted
);

-- migrate:down

delete from messages;
drop table messages;

delete from users;
drop table users;

delete from channels;
drop table channels;
