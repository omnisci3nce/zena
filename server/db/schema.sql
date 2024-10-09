CREATE TABLE IF NOT EXISTS "schema_migrations" (version varchar(128) primary key);
CREATE TABLE users (
    user_id integer primary key,
    username string not null unique,
    password_hash string not null
);
CREATE TABLE messages (
    msg_id integer primary key,
    contents text not null,
    author_id integer not null,
    foreign key (author_id) references users (user_id)
);
-- Dbmate schema migrations
INSERT INTO "schema_migrations" (version) VALUES
  ('20240916092025');
