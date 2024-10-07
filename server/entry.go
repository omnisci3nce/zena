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
	db *sql.DB
}

func newMessengingService(db *sql.DB) MessengingService {
	return MessengingService{db: db}
}

func (m *MessengingService) SendNewMessage(_ context.Context, msg *proto.NewMessage) (*proto.Message, error) {
	log.Printf("Received a new message")

	return &proto.Message{}, nil
}

func (m *MessengingService) GetChannels(_req *proto.Empty, stream grpc.ServerStreamingServer[proto.Channel]) error {
	var chans [2]proto.Channel
	chans[0].ChannelId = "abcd"
	chans[0].Name = "General"

	chans[1].ChannelId = "efgh"
	chans[1].Name = "Gaming"

	stream.Send(&chans[0])
	stream.Send(&chans[1])

	return nil
}

func InitServer() ServerApp {
	const file string = "db/database.sqlite3"
	db, _ := sql.Open("sqlite3", file)

	server := grpc.NewServer()

	messenging := newMessengingService(db)

	proto.RegisterMessengingServer(server, &messenging)

	return ServerApp{
		db:   db,
		Serv: server,
	}
}
