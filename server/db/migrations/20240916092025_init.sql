-- migrate:up

create table users (
    user_id integer primary key,
    username string not null unique,
    password_hash string not null
);

insert into users (user_id, username, password_hash) values(1, 'josh', 'admin');

create table messages (
    msg_id integer primary key,
    contents text not null,
    author_id integer not null,
    foreign key (author_id) references users (user_id)
);

-- migrate:down

delete from messages;
drop table messages;

delete from users;
drop table users;
