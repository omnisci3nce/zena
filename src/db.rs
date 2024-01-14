//! Database connection and queries

use rusqlite::{Connection, Result};

/// Holds a database connection
pub struct Db {
    conn: rusqlite::Connection,
}

impl Db {
    pub fn init() -> Result<Self> {
        let conn = Connection::open("client.db")?;

        // testing out adding a table
        conn.execute(
            "CREATE TABLE IF NOT EXISTS channels (
            channel_id integer primary key,
            name text non null,
            created_at integer not null default (strftime('%s', 'now')),
            updated_at integer not null default (strftime('%s', 'now'))
          );",
            (),
        )?;

        let general_channel = "General".to_string();
        conn.execute(
            "INSERT INTO channels (name) VALUES (?1)",
            [&general_channel],
        )?;

        Ok(Db { conn })
    }
}

#[derive(Debug)]
pub struct Channel {
    pub id: i32,
    pub name: String,
}

pub mod queries {
    use super::{Channel, Db};
    use rusqlite::Result;

    impl Db {
        pub fn get_all_channels(&self) -> Result<Vec<Channel>> {
            let mut stmt = self.conn.prepare(
                "
            SELECT channel_id, name FROM channels
            ",
            )?;
            let channels: Vec<_> = stmt
                .query_map([], |row| {
                    Ok(Channel {
                        id: row.get(0)?,
                        name: row.get(1)?,
                    })
                })?
                .flatten() // we're ignoring errors for the time being
                .collect();
            Ok(channels)
        }
    }
}
