package main

import (
	"fmt"
	"log"
	"net"

	"github.com/omnisci3nce/zena/server"
)

func main() {
	fmt.Println("Server started")

	log.Println("Starting listening on port 50051")
	port := ":50051"

	listener, err := net.Listen("tcp", port)
	if err != nil {
		log.Fatalf("failed to listen: %v", err)
	}
	log.Printf("Listening on %s", port)
	srv := server.InitServer()

	if err := srv.Serv.Serve(listener); err != nil {
		log.Fatalf("failed to serve: %v", err)
	}
}
