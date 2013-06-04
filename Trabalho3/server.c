#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include "message.h"

#define MAXCONNECTIONS 8

struct sockaddr_un localAddress()
{
	struct sockaddr_un address; address.sun_family = AF_UNIX; address.sun_path = /* alguma coisa */;

	return address;
}

int getSocket()
{
	int socket1 = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un* socketAddress = localAddress();

	int socket2 = bind(socket1, socketAddress, sizeof(socketAddress));

	listen(socket2, MAXCONNECTIONS);

	return socket2;
}

void connection(void* socket_p)
{
	
}

/*

void connection(void* socket_p)
{
	int socketId = *((int*) socket_p);
	char buffer[TEXTSIZE];
	char aux[TEXTSIZE];
	int bytesRead = 0;
	int isFirstRead = 1;

	while(!exit)
	{
		bytesRead += read(socketId, aux, TEXTSIZE - bytesRead);

		if (bytesRead < TEXTSIZE)
		{
			if(isFirstRead)
			{
				strcpy(buffer, aux);
				isFirstRead = 0;
			}
			else
			{
				strcat(buffer, aux);
			}
		}
		else
		{
			enqueue(parseMessage(buffer));
			isFirstRead = 1;
		}
	}
}

*/

void server(void* socket_p)
{
	int socketId = *((int*) socket_p);

	while(!exit)
	{
		
	}
}
