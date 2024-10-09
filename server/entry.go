// Stateful message serving

package server

import (
	"database/sql"
	"sync"

	_ "github.com/mattn/go-sqlite3"
	"github.com/omnisci3nce/zena/proto"
	"google.golang.org/grpc"
)

type ServerApp struct {
	mu   sync.Mutex
	db   *sql.DB
	Serv *grpc.Server
}

func InitServer() ServerApp {
	const file string = "server/db/database.sqlite3"
	db, _ := sql.Open("sqlite3", file)

	server := grpc.NewServer()

	messenging := newMessengingService(db)

	proto.RegisterMessengingServer(server, &messenging)

	return ServerApp{
		db:   db,
		Serv: server,
	}
}
