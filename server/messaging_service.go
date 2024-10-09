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
	log.Printf("Received a new message from client")

	// Prepare the SQL statement
	stmt, err := m.db.PrepareContext(ctx, `
			INSERT INTO messages (contents, author_id)
			VALUES (?, ?)
		`)
	if err != nil {
		return nil, status.Errorf(codes.Internal, "Failed to prepare statement: %v", err)
	}
	defer stmt.Close()

	result, err := stmt.ExecContext(ctx, msg.Content, msg.AuthorId) // TODO: timestamps time.Now()
	if err != nil {
		return nil, status.Errorf(codes.Internal, "Failed to insert message: %v", err)
	}

	id, err := result.LastInsertId()
	if err != nil {
		return nil, status.Errorf(codes.Internal, "Failed to get last insert ID: %v", err)
	}

	var username string
	row := m.db.QueryRow("SELECT username FROM users WHERE user_id = ?", &msg.AuthorId)
	err = row.Scan(&username)
	if err != nil {
		return nil, status.Errorf(codes.Internal, "Failed to get author's username: %v", err)
	}

	return &proto.Message{
		MsgId:      string(id),
		Content:    msg.Content,
		AuthorId:   msg.AuthorId,
		AuthorName: username,
		// Timestamp: time.Now().Unix(),
	}, nil
}

func (m *MessengingService) GetChannels(_req *proto.Empty, stream grpc.ServerStreamingServer[proto.Channel]) error {
	chans := []proto.Channel{
		{ChannelId: "abcd", Name: "General"},
		{ChannelId: "efgh", Name: "Gaming"},
	}
	for _, ch := range chans {
		stream.Send(&ch)
	}

	return nil
}

func (m *MessengingService) GetMessages(_req *proto.Empty, stream grpc.ServerStreamingServer[proto.Message]) error {
	rows, err := m.db.Query("SELECT msg_id, contents, author_id, users.username FROM messages INNER JOIN users ON users.user_id = author_id")
	if err != nil {
		return status.Errorf(codes.Internal, "Failed to query messages: %v", err)
	}
	defer rows.Close()

	for rows.Next() {
		var msg proto.Message
		if err := rows.Scan(&msg.MsgId, &msg.Content, &msg.AuthorId, &msg.AuthorName); err != nil {
			return status.Errorf(codes.Internal, "Failed to scan channel row: %v", err)
		}
		stream.Send(&msg)
	}

	return nil
}
