package server

import (
	"context"
	"database/sql"
	"log"

	"github.com/omnisci3nce/zena/proto"
	"google.golang.org/grpc"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/status"
)

type MessengingService struct {
	proto.UnimplementedMessengingServer
	db *sql.DB
}

func newMessengingService(db *sql.DB) MessengingService {
	return MessengingService{db: db}
}

func (m *MessengingService) SendNewMessage(ctx context.Context, msg *proto.NewMessage) (*proto.Message, error) {
	log.Printf("Received a new message")

	// Prepare the SQL statement
	stmt, err := m.db.PrepareContext(ctx, `
			INSERT INTO messages (contents, author_id)
			VALUES (?, ?)
		`)
	if err != nil {
		return nil, status.Errorf(codes.Internal, "Failed to prepare statement: %v", err)
	}
	defer stmt.Close()

	result, err := stmt.ExecContext(ctx, msg.Content, msg.AuthorId) // time.Now()
	if err != nil {
		return nil, status.Errorf(codes.Internal, "Failed to insert message: %v", err)
	}

	id, err := result.LastInsertId()
	if err != nil {
		return nil, status.Errorf(codes.Internal, "Failed to get last insert ID: %v", err)
	}

	return &proto.Message{
		MsgId:   string(id),
		Content: msg.Content,
		Author:  msg.AuthorId,
		// Timestamp: time.Now().Unix(),
	}, nil
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

func (m *MessengingService) GetMessages(_req *proto.Empty, stream grpc.ServerStreamingServer[proto.Message]) error {
	rows, err := m.db.Query("SELECT msg_id, contents, author_id FROM messages")
	if err != nil {
		return status.Errorf(codes.Internal, "Failed to query messages: %v", err)
	}
	defer rows.Close()

	for rows.Next() {
		var msg proto.Message
		if err := rows.Scan(&msg.MsgId, &msg.Content, &msg.Author); err != nil {
			return status.Errorf(codes.Internal, "Failed to scan channel row: %v", err)
		}
		stream.Send(&msg)
	}

	return nil
}
