#include <sys/types.h> 
#include <stdlib.h> 
#include <signal.h>
#include "server_socket.h"


void StartCommandProcessing(Socket local_admin);
void Quit();
SocketServer *server = NULL;


int main()
{
	server = new SocketServer(1997);
	
	/* Accepting client socket */
	while(true)
	{
		Socket client = server->Accept();

		// If current instance of app is PARENT:
		if(fork() != 0)
		{
			close(client);
			printf("Forked...\n");		
			continue;
		}

		if(client >= 0)
		{		
			char *ip = server->GetLastClientIp();
			int port = server->GetLastClientPort();
			printf("Newbie detected: %s:%i\n", ip, port);

			char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=UTF-8\x0D\x0A\x0D\x0AWelcome on multi-thread socket server!\n(Print 'help' if u xz what to do)\n";
			write(client, response, sizeof(response));			
			StartCommandProcessing(client);

			close(client);
		}
	}

	server->Shutdown();
	return 0;
}

void StartCommandProcessing(Socket local_admin)
{
	while(true)
	{
		char buffer[1024] = {0};
		read(local_admin, buffer, 1024);	

		// HTTP response
		if(strstr(buffer, "GET / HTTP"))
		{
			char response[2048] = {0};
			char garbage[] = "<br><br>HTTP request detected!<br><u>Your headers were:</u><br>";
			sprintf(response, "%s<pre>%s</pre>", garbage, buffer);
			write(local_admin, response, 1024);
			Quit();
		}

		// Raw network communication with a client:

		if(strcmp(buffer, "done\n") == 0)
			Quit();

		if(strstr(buffer, "shell"))
		{
			system(buffer+6);
			continue;
		}

		char unknown[] = "Unknown expression\n";
		write(local_admin, unknown, sizeof(unknown));
	}
}

void Quit()
{
	int pid = getpid();
	printf("Done: %d\n", pid);
	kill(pid, SIGTERM);
}