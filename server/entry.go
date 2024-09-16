// Stateful message serving

package server

import (
	"database/sql"
	"sync"

	_ "github.com/mattn/go-sqlite3"
)

type ServerApp struct {
	mu sync.Mutex
	db *sql.DB
}

func InitServer() ServerApp {
	const file string = "db/database.sqlite3"
	db, _ := sql.Open("sqlite3", file)

	return ServerApp{
		db: db,
	}
}
