-- migrate:up

create table channels (
  channel_id integer primary key,

  -- updateable fields
  name text non null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now'))
);

create table messages (
  msg_id integer primary key,
  author_id integer not null references users(user_id),
  channel_id integer not null references channels(channel_id),
  content text not null,

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now'))
);

create table users (
  user_id integer primary key,

  -- updateable fields
  email text, -- optional

  -- audit
  created_at integer not null default (strftime('%s', 'now')),
  updated_at integer not null default (strftime('%s', 'now')),

  -- soft delete
  deleted integer not null default 0
);

-- migrate:down

delete from users;
drop table users;
