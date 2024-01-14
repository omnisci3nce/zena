-- migrate:up

create table presence (
  user_id       integer primary key references users(user_id),
  is_online     integer not null check (is_online in (0,1)),
  last_seen_at  integer not null default (strftime('%s', 'now'))
);

-- migrate:down
delete from presence;
drop table presence;
