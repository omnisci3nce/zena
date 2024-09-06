package zena

import "core:fmt"
import "core:net"
import "core:os"
import "core:strconv"

main :: proc() {
	if (len(os.args) < 2) {
		fmt.printf("Usage: %s <port>\n", os.args[0])
		os.exit(1)
	}

	port, ok := strconv.parse_int(os.args[1], 10)
	if !ok {
		fmt.printf("Port must be a number\n")
		os.exit(1)
	}

	endpoint := net.Endpoint {
		address = net.IP4_Address({0, 0, 0, 0}),
		port    = port,
	}

	// start server
	socket, listen_err := net.listen_tcp(endpoint)
	if listen_err != nil {
		fmt.printf("Error listening on port %d: %v\n", port, listen_err)
		os.exit(1)
	}
	fmt.printf("Listening on port %d\n", port)


	fmt.println("Shutting down Zena server...")
}
