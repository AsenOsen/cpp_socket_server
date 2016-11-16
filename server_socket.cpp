#include "server_socket.h"

SocketServer::SocketServer(unsigned short port)
{
	/* Creating row socket */
	server = socket(AF_INET, SOCK_STREAM, 0);
	if(server < 0)
		printf("Socket error: %08x", errno);

	/* Binding port to socket */
	sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;
	int bindingResult = 
		bind(server, (sockaddr*) &server_address, sizeof(server_address));
	if(bindingResult < 0)
		printf("Binding error: %08x", errno);

	/* Start listening */
	listen(server, 5);
}

Socket SocketServer::Accept()
{
	/* Accepting client conenction */
	socklen_t addrlen = sizeof(last_client_info);
	Socket client = accept(server, (sockaddr*) &last_client_info, &addrlen);

	return client;
}

void SocketServer::Shutdown()
{
	close(server);
}