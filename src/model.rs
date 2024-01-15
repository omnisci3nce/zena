//! The (rough) data model. This is all the data that is stored in the database
//! and essentially defines what the app can do and store.

#[derive(Debug)]
pub struct Channel {
    pub id: i64,
    pub name: String,
    pub created_at: i64,
    pub updated_at: i64,
}

pub struct User {
    pub id: i64,
    pub username: String,
    pub email: Option<String>,
    // we don't query the password out
    pub created_at: i64,
    pub updated_at: i64,
    pub deleted: bool,
}

pub struct Message {
    pub id: i64,
    pub author_id: i64,
    pub channel_id: i64,
    pub created_at: i64,
    pub updated_at: i64,
    pub deleted: bool,
}
