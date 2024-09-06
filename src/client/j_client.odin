package client

import "../zena"
import "core:fmt"
import "core:net"
import "core:os"
import "core:strconv"

main :: proc() {
	if (len(os.args) < 2) {
		fmt.printf("Usage: %s <server port>\n", os.args[0])
		os.exit(1)
	}

	port, ok := strconv.parse_int(os.args[1], 10)
	if !ok {
		fmt.printf("Port must be a number\n")
		os.exit(1)
	}

	remote := net.Endpoint {
		address = net.IP4_Address({127, 0, 0, 1}),
		port    = port,
	}

	// create socket
	fmt.println("Dialing server @ ", remote)
	socket, err := net.dial_tcp(remote)
	if err != nil {
		fmt.eprintf("Failed to connect to remote %v", remote)
		os.exit(1)
	}

	core: zena.Core

	zena.core_run(&core, socket)
}
