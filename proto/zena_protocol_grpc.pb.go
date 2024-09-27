// Code generated by protoc-gen-go-grpc. DO NOT EDIT.
// versions:
// - protoc-gen-go-grpc v1.5.1
// - protoc             v5.26.1
// source: proto/zena_protocol.proto

package proto

import (
	context "context"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
)

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
// Requires gRPC-Go v1.64.0 or later.
const _ = grpc.SupportPackageIsVersion9

const (
	Messenging_SendNewMessage_FullMethodName    = "/messaging.Messenging/SendNewMessage"
	Messenging_SubscribeChannels_FullMethodName = "/messaging.Messenging/SubscribeChannels"
)

// MessengingClient is the client API for Messenging service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://pkg.go.dev/google.golang.org/grpc/?tab=doc#ClientConn.NewStream.
type MessengingClient interface {
	SendNewMessage(ctx context.Context, in *NewMessage, opts ...grpc.CallOption) (*Message, error)
	SubscribeChannels(ctx context.Context, in *ChannelList, opts ...grpc.CallOption) (grpc.ServerStreamingClient[Message], error)
}

type messengingClient struct {
	cc grpc.ClientConnInterface
}

func NewMessengingClient(cc grpc.ClientConnInterface) MessengingClient {
	return &messengingClient{cc}
}

func (c *messengingClient) SendNewMessage(ctx context.Context, in *NewMessage, opts ...grpc.CallOption) (*Message, error) {
	cOpts := append([]grpc.CallOption{grpc.StaticMethod()}, opts...)
	out := new(Message)
	err := c.cc.Invoke(ctx, Messenging_SendNewMessage_FullMethodName, in, out, cOpts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *messengingClient) SubscribeChannels(ctx context.Context, in *ChannelList, opts ...grpc.CallOption) (grpc.ServerStreamingClient[Message], error) {
	cOpts := append([]grpc.CallOption{grpc.StaticMethod()}, opts...)
	stream, err := c.cc.NewStream(ctx, &Messenging_ServiceDesc.Streams[0], Messenging_SubscribeChannels_FullMethodName, cOpts...)
	if err != nil {
		return nil, err
	}
	x := &grpc.GenericClientStream[ChannelList, Message]{ClientStream: stream}
	if err := x.ClientStream.SendMsg(in); err != nil {
		return nil, err
	}
	if err := x.ClientStream.CloseSend(); err != nil {
		return nil, err
	}
	return x, nil
}

// This type alias is provided for backwards compatibility with existing code that references the prior non-generic stream type by name.
type Messenging_SubscribeChannelsClient = grpc.ServerStreamingClient[Message]

// MessengingServer is the server API for Messenging service.
// All implementations must embed UnimplementedMessengingServer
// for forward compatibility.
type MessengingServer interface {
	SendNewMessage(context.Context, *NewMessage) (*Message, error)
	SubscribeChannels(*ChannelList, grpc.ServerStreamingServer[Message]) error
	mustEmbedUnimplementedMessengingServer()
}

// UnimplementedMessengingServer must be embedded to have
// forward compatible implementations.
//
// NOTE: this should be embedded by value instead of pointer to avoid a nil
// pointer dereference when methods are called.
type UnimplementedMessengingServer struct{}

func (UnimplementedMessengingServer) SendNewMessage(context.Context, *NewMessage) (*Message, error) {
	return nil, status.Errorf(codes.Unimplemented, "method SendNewMessage not implemented")
}
func (UnimplementedMessengingServer) SubscribeChannels(*ChannelList, grpc.ServerStreamingServer[Message]) error {
	return status.Errorf(codes.Unimplemented, "method SubscribeChannels not implemented")
}
func (UnimplementedMessengingServer) mustEmbedUnimplementedMessengingServer() {}
func (UnimplementedMessengingServer) testEmbeddedByValue()                    {}

// UnsafeMessengingServer may be embedded to opt out of forward compatibility for this service.
// Use of this interface is not recommended, as added methods to MessengingServer will
// result in compilation errors.
type UnsafeMessengingServer interface {
	mustEmbedUnimplementedMessengingServer()
}

func RegisterMessengingServer(s grpc.ServiceRegistrar, srv MessengingServer) {
	// If the following call pancis, it indicates UnimplementedMessengingServer was
	// embedded by pointer and is nil.  This will cause panics if an
	// unimplemented method is ever invoked, so we test this at initialization
	// time to prevent it from happening at runtime later due to I/O.
	if t, ok := srv.(interface{ testEmbeddedByValue() }); ok {
		t.testEmbeddedByValue()
	}
	s.RegisterService(&Messenging_ServiceDesc, srv)
}

func _Messenging_SendNewMessage_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(NewMessage)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(MessengingServer).SendNewMessage(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: Messenging_SendNewMessage_FullMethodName,
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(MessengingServer).SendNewMessage(ctx, req.(*NewMessage))
	}
	return interceptor(ctx, in, info, handler)
}

func _Messenging_SubscribeChannels_Handler(srv interface{}, stream grpc.ServerStream) error {
	m := new(ChannelList)
	if err := stream.RecvMsg(m); err != nil {
		return err
	}
	return srv.(MessengingServer).SubscribeChannels(m, &grpc.GenericServerStream[ChannelList, Message]{ServerStream: stream})
}

// This type alias is provided for backwards compatibility with existing code that references the prior non-generic stream type by name.
type Messenging_SubscribeChannelsServer = grpc.ServerStreamingServer[Message]

// Messenging_ServiceDesc is the grpc.ServiceDesc for Messenging service.
// It's only intended for direct use with grpc.RegisterService,
// and not to be introspected or modified (even as a copy)
var Messenging_ServiceDesc = grpc.ServiceDesc{
	ServiceName: "messaging.Messenging",
	HandlerType: (*MessengingServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "SendNewMessage",
			Handler:    _Messenging_SendNewMessage_Handler,
		},
	},
	Streams: []grpc.StreamDesc{
		{
			StreamName:    "SubscribeChannels",
			Handler:       _Messenging_SubscribeChannels_Handler,
			ServerStreams: true,
		},
	},
	Metadata: "proto/zena_protocol.proto",
}

const (
	Auth_Login_FullMethodName = "/messaging.Auth/Login"
)

// AuthClient is the client API for Auth service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://pkg.go.dev/google.golang.org/grpc/?tab=doc#ClientConn.NewStream.
type AuthClient interface {
	Login(ctx context.Context, in *LoginDetails, opts ...grpc.CallOption) (*LoginResponse, error)
}

type authClient struct {
	cc grpc.ClientConnInterface
}

func NewAuthClient(cc grpc.ClientConnInterface) AuthClient {
	return &authClient{cc}
}

func (c *authClient) Login(ctx context.Context, in *LoginDetails, opts ...grpc.CallOption) (*LoginResponse, error) {
	cOpts := append([]grpc.CallOption{grpc.StaticMethod()}, opts...)
	out := new(LoginResponse)
	err := c.cc.Invoke(ctx, Auth_Login_FullMethodName, in, out, cOpts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// AuthServer is the server API for Auth service.
// All implementations must embed UnimplementedAuthServer
// for forward compatibility.
type AuthServer interface {
	Login(context.Context, *LoginDetails) (*LoginResponse, error)
	mustEmbedUnimplementedAuthServer()
}

// UnimplementedAuthServer must be embedded to have
// forward compatible implementations.
//
// NOTE: this should be embedded by value instead of pointer to avoid a nil
// pointer dereference when methods are called.
type UnimplementedAuthServer struct{}

func (UnimplementedAuthServer) Login(context.Context, *LoginDetails) (*LoginResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Login not implemented")
}
func (UnimplementedAuthServer) mustEmbedUnimplementedAuthServer() {}
func (UnimplementedAuthServer) testEmbeddedByValue()              {}

// UnsafeAuthServer may be embedded to opt out of forward compatibility for this service.
// Use of this interface is not recommended, as added methods to AuthServer will
// result in compilation errors.
type UnsafeAuthServer interface {
	mustEmbedUnimplementedAuthServer()
}

func RegisterAuthServer(s grpc.ServiceRegistrar, srv AuthServer) {
	// If the following call pancis, it indicates UnimplementedAuthServer was
	// embedded by pointer and is nil.  This will cause panics if an
	// unimplemented method is ever invoked, so we test this at initialization
	// time to prevent it from happening at runtime later due to I/O.
	if t, ok := srv.(interface{ testEmbeddedByValue() }); ok {
		t.testEmbeddedByValue()
	}
	s.RegisterService(&Auth_ServiceDesc, srv)
}

func _Auth_Login_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(LoginDetails)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(AuthServer).Login(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: Auth_Login_FullMethodName,
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(AuthServer).Login(ctx, req.(*LoginDetails))
	}
	return interceptor(ctx, in, info, handler)
}

// Auth_ServiceDesc is the grpc.ServiceDesc for Auth service.
// It's only intended for direct use with grpc.RegisterService,
// and not to be introspected or modified (even as a copy)
var Auth_ServiceDesc = grpc.ServiceDesc{
	ServiceName: "messaging.Auth",
	HandlerType: (*AuthServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "Login",
			Handler:    _Auth_Login_Handler,
		},
	},
	Streams:  []grpc.StreamDesc{},
	Metadata: "proto/zena_protocol.proto",
}