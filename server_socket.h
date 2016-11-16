#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

typedef int Socket;
typedef struct sockaddr_in ClientInfo;

class SocketServer{

	private:
		Socket server;
		ClientInfo last_client_info;

	public:
		SocketServer(unsigned short port);
		Socket Accept();
		void Shutdown();

		char* GetLastClientIp()
		{
			struct in_addr incoming;
			incoming.s_addr = last_client_info.sin_addr.s_addr;

			return inet_ntoa(incoming);
		}

		int GetLastClientPort()
		{
			return last_client_info.sin_port;
		}

};