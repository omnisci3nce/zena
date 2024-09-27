# Make sure you have protoc installed!
.PHONY: proto

proto:
	@echo "Generating message and grpc service types from protobuf files"
	protoc proto/*.proto \
		--go_out=. \
		--go_opt=paths=source_relative \
		--go-grpc_out=. \
		--go-grpc_opt=paths=source_relative \
		--proto_path=.

# TODO: format command
# TODO: run command