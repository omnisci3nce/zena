// Stateful message serving

package server

import (
	"context"
	"database/sql"
	"log"
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

type MessengingService struct {
	proto.UnimplementedMessengingServer
}

func newMessengingService() MessengingService {
	return MessengingService{}
}

func (m *MessengingService) SendNewMessage(_ context.Context, msg *proto.NewMessage) (*proto.Message, error) {
	log.Printf("Received a new message")

	return &proto.Message{}, nil
}

func InitServer() ServerApp {
	const file string = "db/database.sqlite3"
	db, _ := sql.Open("sqlite3", file)

	server := grpc.NewServer()

	messenging := newMessengingService()

	proto.RegisterMessengingServer(server, &messenging)

	return ServerApp{
		db:   db,
		Serv: server,
	}
}
