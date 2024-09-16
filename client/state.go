package client

import (
	"github.com/omnisci3nce/zena/proto"
)

type State struct {
	Messages []*proto.Message
}

func NewState() State {
	return State{
		Messages: []*proto.Message{},
	}
}
